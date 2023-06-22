#include "Plume.h"

namespace CALINE3
{
    ///////////////////////////////////////////////////////////////////////
    ///
    ///  Constants
    ///

    const double SQRT_2{ std::sqrt(2.0) };
    const double SQRT_2PI{ std::sqrt(2.0 * std::acos(-1.0)) };
    constexpr Meter_Sec ZERO_VELOCITY{ 0.0 };
    constexpr Microgram_Meter3 ZERO_CONCENTRATION{ 0.0 };
    constexpr Meter MAX_MIXH{ 1000.0 };

    ////////////////////////////////////////////////////////////////////////////
    /// 
    ///      Constructor(s)
    ///

    Plume::Plume(const Job& site, const Meteo& meteo, const Link& link) :
        _site(site),
        _meteo(meteo),
        _link(link),
        _flow(meteo, link)
    {
        /***************************************
         *
         * sgy(1[m])     = a * (1[m])^b = a = a * e^(b*ln(1[m])     = met.AY1 * site.RFAC_3CM_02 * site.AFAC_3MIN_02;
         * sgy(10000[m]) = a * (10000[m])^b = a * e^(b*ln(10000[m]) = met.AY2 * site.RFAC_3CM_007 * site.AFAC_3MIN_02;
         *
         * b = (ln(sgy(10000[m]) - ln(sgy(1[m]))) / (ln(10000[m]) - ln(1[m])
         *
         */

        // a-coefficient
        PY1 = _meteo.AY1() * _site.RFAC_3CM_02 * _site.AFAC_3MIN_02;

        // sgy(10000[m])
        double PY10 = _meteo.AY2() * _site.RFAC_3CM_007 * _site.AFAC_3MIN_02;

        // b-coefficient
        PY2 = log(PY10 / PY1) / log(Link::MAX_LENGTH / Link::MIN_LENGTH);

        /***************************************
         * 
         * Mixing zone residence time [s]:
         * 
         */

        Second TR = link.DSTR() * link.W2() / meteo.U;

        /***************************************
         *
         * To relate SGZI (sgz initial vertical dispersion parameter) and TR
         * (mixing zone residence time) the following equation:
         *
         *      SGZI[m] = 1.8[?] + 0.11[?] * TR[s]
         *
         * has been derived from the General Motors Data Base.
         *
         * NOTE THE MISSING UNITS [?]: I guess it should be like this:
         * 
         */

        Meter SGZI = Meter{ 1.8 } + Meter_Sec{ 0.11 } *TR;

        // SGZI need to be adjusted for the averaging time (but it is considered
        // to be independent of surface roughness and atmospheric stability class).
        // It is always defined as occurring at a distance W2 downwind from the
        // element centerpoint i.e. it equals sgz(W2[m]):
        Meter SGZ1 = SGZI * _site.AFAC_30MIN_02;

        // sgz(10000[m]) adjusted for roughness and averaging time:
        Meter SZ10 = Meter{ _meteo.AZ() * _site.RFAC_10CM_007 * _site.AFAC_3MIN_02 };

        /***************************************
         *
         * sgz(W2[m])    = c * (W2[m])^d    = c * e^(d∙ln(W2[m]))
         * sgz(10000[m]) = c * (10000[m])^d = c * e^(d∙ln(10000[m]))
         *
         * d = (ln(sgz(10000)) - ln(sgz(W2))) / (ln(10000) - ln(W2))
         *
         * ln(sgz(W2)/c(W2)) = ln(sgz(W2)) - ln(c(W2)) = d * ln(W2)
         * ln(sgz(10000)/c(10000)) = ln(sgz(10000)) - ln(c(10000)) = d * ln(10000)
         *
         * ln(c) = (ln(c(10000)) - ln(c(W2))) / 2
         *       = (ln(sgz(10000)) + ln(sgz(W2)) - d * (ln(10000) + ln(W2))) / 2
         *       = (ln(sgz(10000) * sgz(W2)) - d*(ln(10000 * W2))) / 2
         *
         * c = sqrt(sgz(10000) * sgz(W2)) / pow(sqrt(10000 * W2), d)
         *
         */

        // d-coefficient
        PZ2 = log(SZ10 / SGZ1) / log(Link::MAX_LENGTH / _link.W2());

        // c-coefficient
        PZ1 = sqrt(SZ10 * SGZ1) / pow(sqrt(Link::MAX_LENGTH * _link.W2()), PZ2);
    }

    ///////////////////////////////////////////////////////////////////////////
    // 
    //      Methods
    //

    Microgram_Meter3 Plume::ConcentrationAt(const Receptor& receptor)
    {
        Meter D;    // distance (perpendicular to the link)
        Meter L;    // offset (parallel to the link, relative to its start position)
        Meter Z;    // level (adjusted for the link type)
        std::tie(D, L, Z) = _link.TransformReceptorCoordinates(receptor);

        // Assuming point 0 at the receptor orthogonal projection on link line:
        Meter DWL = -(_link.LL() + L);
        Meter UWL = -L;

        // Mass Concentration
        Microgram_Meter3 C{ 0.0 };

        // Add up the concentrations from the UPWIND elements:
        for (Meter elemEnd{ 0.0 }, EL = _link.WL; elemEnd < UWL; EL *= _flow.BASE())
        {
            // Next element
            Meter elemStart{ elemEnd };
            elemEnd += EL;
            // Any element reached?
            if (elemEnd > DWL)
            {
                LinkElement elem { _link, _flow,
                    std::max(elemStart, DWL), 
                    std::min(elemEnd, UWL)
                };
                C += ConcentrationFrom(elem, D, Z);
            }
        }

        // Add up the concentrations from the DOWNWIND elements:
        for (Meter elemStart{ 0.0 }, EL = _link.WL; elemStart > DWL; EL *= _flow.BASE())
        {
            // Next element
            Meter elemEnd{ elemStart };
            elemStart -= EL;
            // Any element reached?
            if (elemStart < UWL)
            {
                LinkElement elem { _link, _flow,
                    std::max(elemStart, DWL),
                    std::min(elemEnd, UWL) 
                };
                C += ConcentrationFrom(elem, D, Z);
            }
        }

        return C;
    }

    Microgram_Meter3 Plume::ConcentrationFrom(const LinkElement& element, Meter D, Meter Z) const
    {
        // Get element profile:
        Microgram_Meter_Sec QE;  // central subelement lineal strength [microgram/(m * s)]
        Meter YE;               // plume centerline offset [m]
        Meter FET;              // element fetch [m]
        if (!element.GetProfile(D, QE, YE, FET))
        {
            return ZERO_CONCENTRATION;  // Element does NOT contribute.
        }

        // Horizontal standard deviation (sigma-y) of the emission distribution:
        Meter SGY{ PY1 * pow(FET, PY2) };

        // Vertical standard deviation (sigma-z) of the emission distribution:
        Meter SGZ{ PZ1 * pow(FET, PZ2) };

        // Vertical diffusivity estimate:
        Meter2_Sec KZ{ SGZ * SGZ / (2.0 * FET / _meteo.U) };

        Microgram_Meter3 FACT =
            element.SourceStrength(QE, SGY, YE) / (SQRT_2PI * SGZ * _meteo.U);

        // Adjust for depressed section wind speed
        FACT *= _link.DepressedSectionFactor(D);

        // Deposition correction
        double FAC3 = DepositionFactor(SGZ, KZ, Z, _link.H(), _site.V1);
        if (std::isnan(FAC3))
        {
            return ZERO_CONCENTRATION;
        }
        else
        {
            // Settling correction
            FACT *= SettlingFactor(SGZ, KZ, Z, _link.H(), _site.VS);

            // Incremental concentration from the element
            double FAC5 = GaussianFactor(SGZ, Z, _link.H(), _meteo.MIXH);
            return FACT * (FAC5 - FAC3);
        }
    }

    double Plume::DepositionFactor(Meter SGZ, Meter2_Sec KZ, Meter Z, Meter H, Meter_Sec V1) const
    {
        double FAC3 = 0.0;
        if (V1 != ZERO_VELOCITY)
        {
            double ARG = (V1 * SGZ / KZ + (Z + H) / SGZ) / SQRT_2;
            if (ARG > 5.0)
            {
                FAC3 = std::nan("");
            }
            else
            {
                FAC3 =
                    SQRT_2PI * V1 * SGZ
                    * exp(V1 * (Z + H) / KZ + 0.5 * (V1 * SGZ / KZ) * (V1 * SGZ / KZ))
                    * Erf(ARG)
                    / KZ;

                if (FAC3 > 2.0) FAC3 = 2.0;
            }
        }
        return FAC3;
    }

    double Plume::SettlingFactor(Meter SGZ, Meter2_Sec KZ, Meter Z, Meter H, Meter_Sec VS) const
    {
        return (VS == ZERO_VELOCITY) ? 1.0 : exp(-VS * (Z - H) / (2.0 * KZ) - (VS * SGZ / KZ) * (VS * SGZ / KZ) / 8.0);
    }

    double Plume::GaussianFactor(Meter SGZ, Meter Z, Meter H, Meter MIXH) const
    {
        double FAC5 = 0.0;
        double CNT = 0.0;
        double EXLS = 0.0;
        while (true)
        {
            double ARG1 = -0.5 * ((Z + H + 2.0 * CNT * MIXH) / SGZ) * ((Z + H + 2.0 * CNT * MIXH) / SGZ);
            double EXP1 = (ARG1 < -44.0) ? 0.0 : exp(ARG1);

            double ARG2 = -0.5 * ((Z - H + 2.0 * CNT * MIXH) / SGZ) * ((Z - H + 2.0 * CNT * MIXH) / SGZ);
            double EXP2 = (ARG2 < -44.0) ? 0.0 : exp(ARG2);

            FAC5 += EXP1 + EXP2;

            if (MIXH >= MAX_MIXH)
                break;  // Bypass mixing height calculation

            if (((EXP1 + EXP2 + EXLS) == 0.0) && (CNT <= 0.0))
                break;

            if (CNT <= 0.0)
            {
                CNT = abs(CNT) + 1.0;
                EXLS = 0.0;
            }
            else
            {
                CNT = -1.0 * CNT;
                EXLS = EXP1 + EXP2;
            }
        }
        return FAC5;
    }
}
