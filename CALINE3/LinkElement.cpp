#include "LinkElement.h"

namespace CALINE3
{
    ////////////////////////////////////////////////////////////////////////////
    /// 
    ///      Methods
    ///

    bool LinkElement::GetProfile(Meter D, Microgram_Meter_Sec& QE, Meter& YE, Meter& FET) const
    {
        // Y distance from element center to receptor (plume centerline offset)
        YE = ECLD * sin(flow.PHI()) - D * cos(flow.PHI());

        // Element fetch
        FET = ECLD * cos(flow.PHI()) + D * sin(flow.PHI());

        // Central sub-element lineal source strength
        if (FET <= -CSL2)
        {
            return false;
        }
        else if (FET < CSL2)
        {
            // receptor within element
            FET = (CSL2 + FET) / 2.0;
            QE = master.Q1() * (FET / master.W2());
        }
        else
        {
            QE = master.Q1() * (CSL2 / master.W2());
        }

        return true;
    }

    Microgram_Meter_Sec LinkElement::SourceStrength(Microgram_Meter_Sec QE, Meter SGY, Meter YE) const
    {
        // Constants
        static const double SQRT_2{ std::sqrt(2.0) };

        // Weighting factor
        static const double WT[] = { 0.25, 0.75, 1.0, 0.75, 0.25 };

        // Sub-element source strength loop
        Meter Y[6]{};

        Y[0] = YE + ELL2;
        Y[1] = Y[0] - EN2;
        Y[2] = Y[1] - EN2;
        Y[3] = Y[2] - 2.0 * EM2;
        Y[4] = Y[3] - EN2;
        Y[5] = Y[4] - EN2;

        // Add up strengths of all subelements
        Microgram_Meter_Sec FAC2{ 0.0 };
        for (int j = 0; j < 5; j++)
        {
            FAC2 += QE * WT[j] *
                /* PD = normal probability density = */
                (Erf(Y[j] / SGY / SQRT_2) - Erf(Y[j + 1] / SGY / SQRT_2)) / 2.0;
        }

        return FAC2;
    }

    ////////////////////////////////////////////////////////////////////////////
    /// 
    ///      Formatting
    ///

    std::ostream& operator<<(std::ostream& os, const LinkElement& elem)
    {
        // $"ECLD={ECLD} : EL2={EL2} :: ELL2={ELL2} : CSL2={CSL2} :: EM2={EM2} : EN2={EN2}";
        return os
            << "ECLD=" << elem.ECLD << " : "
            << "EL2=" << elem.EL2 << " :: "
            << "ELL2=" << elem.ELL2 << " : "
            << "CSL2=" << elem.CSL2 << " :: "
            << "EM2=" << elem.EM2 << " : "
            << "EN2" << elem.EN2;
    }
}