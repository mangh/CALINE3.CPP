#include "Report.h"

// Units required/suplementary:
#include "Gram_Meter3.h"
#include "Ratio.h"

namespace CALINE3
{
    using namespace Metrology;

    ////////////////////////////////////////////////////////////////////////////
    /// 
    ///      Constants
    ///

    /// @brief PPM factor [mcg/m3] to convert concentration from [mcg/m3] to [ppm].
    const Microgram_Meter3 Report::FPPM { MOWT / MOVL };

    ////////////////////////////////////////////////////////////////////////////
    /// 
    ///      Methods
    ///

    Ppm Report::ToPPM(Microgram_Meter3 mc)
    {
        return round(10.0 * Ppm(Ratio{ mc / FPPM })) / 10.0;
    }

    std::string Report::LinkCodes(const std::string& separator, const std::vector<Link>& links)
    {
        std::string codes{};
        for (auto const &link: links)
        {
            if (codes.size() > 0) codes += separator;
            codes += link.COD();
        }
        return codes;
    }

    void Report::PrintJobAndMeteo(const Job& site, const Meteo& met)
    {
        const char *title = "                            CALINE3: CALIFORNIA LINE SOURCE DISPERSION MODEL - SEPTEMBER, 1979/2022 C++ VERSION            PAGE ";

        os  << title << (++PageCount)
            << std::endl
            << std::endl
            << std::endl;
        
        os  << "     JOB: " << std::left << std::setw(53) << site.JOB << "RUN: " << std::setw(40) << site.RUN
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

        os  << "       I.  SITE VARIABLES"
            << std::endl
            << std::endl
            << std::endl;
        
        os  << "      U = " << std::right << std::setw(4) << std::setprecision(1) << std::fixed << met.U.value() << " M/S"
            << "            CLAS = " << std::setw(3) << met.CLAS << "  (" << met.TAG() << ")"
            << "        VS = " << std::setw(5) << std::setprecision(1) << site.VS1.value() << " CM/S"
            << "       ATIM = " << std::setw(3) << std::setprecision(0) << site.ATIM.value() << "  MINUTES"
            << "                   MIXH = " << std::setw(5) << std::setprecision(0) << met.MIXH.value() << " M"
            << std::endl;

        os  << "    BRG = " << std::setw(3) << std::setprecision(0) << met.BRG1.value() << "  DEGREES"
            << "          Z0 =" << std::setw(4) << std::setprecision(0) << site.Z0.value() << "  CM"
            << "         VD = " << std::setw(5) << std::setprecision(1) << site.VD1.value() << " CM/S"
            << "        AMB =" << std::setw(5) << std::setprecision(1) << met.AMB.value() << " PPM"
            << std::endl
            << std::endl;
    }

    void Report::PrintLinks(const std::vector<Link>& links)
    {
        os  << std::endl
            << std::endl
            << std::endl
            << "      II.  LINK VARIABLES"
            << std::endl
            << std::endl
            << std::endl;

        os  << "       LINK DESCRIPTION     *      LINK COORDINATES (M)      * LINK LENGTH  LINK BRG   TYPE  VPH     EF     H    W"
            << std::endl;

        os  << "                            *   X1      Y1      X2      Y2   *     (M)       (DEG)                 (G/MI)  (M)  (M)"
            << std::endl;

        os  << "   -------------------------*--------------------------------*-------------------------------------------------------"
            << std::endl;

        for (auto const& link : links)
        {
            os  << std::fixed
                << std::right << std::setw(4) << link.COD() << ". "
                << std::left << std::setw(22) << link.LNK << "*"
                << std::right << std::setw(6) << std::setprecision(0) << link.XL1.value()
                << std::right << std::setw(8) << std::setprecision(0) << link.YL1.value()
                << std::right << std::setw(8) << std::setprecision(0) << link.XL2.value()
                << std::right << std::setw(8) << std::setprecision(0) << link.YL2.value() << "  *"
                << std::right << std::setw(9) << std::setprecision(0) << link.LL().value()
                << std::right << std::setw(10) << std::setprecision(0) << link.LBRG().value()
                << std::right << std::setw(9) << link.TYP
                << std::right << std::setw(7) << std::setprecision(0) << link.VPHL.value()
                << std::right << std::setw(7) << std::setprecision(1) << link.EFL.value()
                << std::right << std::setw(6) << std::setprecision(1) << link.HL.value()
                << std::right << std::setw(6) << std::setprecision(1) << link.WL.value() << std::endl;
        }
        os << std::endl;
    }

    void Report::PrintReceptorsHeader()
    {
        os  << std::endl
            << "     III.  RECEPTOR LOCATIONS AND MODEL RESULTS"
            << std::endl
            << std::endl
            << std::endl;
    }

    void Report::PrintReceptor(const Receptor &receptor, size_t SEQNO)
    {
        os  << std::fixed
            << std::right << std::setw(5) << SEQNO << ". "
            << std::left << std::setw(21) << receptor.RCP << "* "
            << std::right << std::setw(8) << std::setprecision(0) << receptor.XR.value()
            << std::right << std::setw(9) << std::setprecision(0) << receptor.YR.value()
            << std::right << std::setw(10) << std::setprecision(1) <<receptor.ZR.value();
    }

    void Report::PrintConcentrations(const std::vector<std::vector<Microgram_Meter3>> &MC, size_t R)
    {
        for (auto const& mass_conc : MC)
        {
            auto ppm = ToPPM(mass_conc[R]);
            os << std::fixed << std::right << std::setw(5) << std::setprecision(1) << ppm.value();
        }
    }

    void Report::PrintTotalConcentration(Ppm amb, const std::vector<std::vector<Microgram_Meter3>> &MC, size_t R)
    {
        //auto total = TotalConcentrationAtReceptor(MC, R) + amb;
        Ppm CSUM{ 0.0 };
        for (auto const& mass_conc : MC)
        {
            CSUM += ToPPM(mass_conc[R]);
        }
        CSUM += amb;
        os << "   *" << std::right << std::setw(5) << std::setprecision(1) << CSUM.value();
    }

    void Report::Print(const Job& site, const Meteo& meteo, const std::vector<std::vector<Microgram_Meter3>> &MC)
    {
        // CALINE3: CALIFORNIA LINE SOURCE DISPERSION MODEL - SEPTEMBER, 1979 VERSION
        // I. SITE VARIABLES
        PrintJobAndMeteo(site, meteo);

        // II.  LINK VARIABLES
        PrintLinks(site.Links);

        // III.  RECEPTOR LOCATIONS AND MODEL RESULTS
        if (site.Links.size() <= 1)
        {
            PrintReceptorsHeader();

            os  << "                            *        COORDINATES (M)        *  CO" << std::endl
                << "       RECEPTOR             *      X        Y        Z      * (PPM)" << std::endl
                << "   -------------------------*-------------------------------*-------" << std::endl;

            for (std::size_t I = 0; I < site.Receptors.size(); I++)
            {
                PrintReceptor(site.Receptors[I], I + 1);
                PrintTotalConcentration(meteo.AMB, MC, I);
                os << std::endl;
            }
        }
        else if (site.Links.size() <= 10)
        {
            PrintReceptorsHeader();

            std::string linkCodes = LinkCodes("    ", site.Links);

            os  << "                            *                               * TOTAL *             CO/LINK" << std::endl
                << "                            *        COORDINATES (M)        * + AMB *              (PPM)" << std::endl
                << "       RECEPTOR             *      X        Y        Z      * (PPM) *   " << linkCodes << std::endl
                << "   -------------------------*-------------------------------*-------*---" << std::string(linkCodes.size()+1, '-')
                << std::endl;

            for (std::size_t I = 0; I < site.Receptors.size(); I++)
            {
                PrintReceptor(site.Receptors[I], I + 1);
                PrintTotalConcentration(meteo.AMB, MC, I);
                os << "  *";
                PrintConcentrations(MC, I);
                os << std::endl;
            }
        }
        else
        {
            PrintJobAndMeteo(site, meteo);

            PrintReceptorsHeader();

            os  << "                            *                               * TOTAL *" << std::endl
                << "                            *        COORDINATES (M)        * + AMB *" << std::endl
                << "       RECEPTOR             *      X        Y        Z      * (PPM) *" << std::endl
                << "   -------------------------*-------------------------------*-------*" << std::endl;

            for (std::size_t I = 0; I < site.Receptors.size(); I++)
            {
                PrintReceptor(site.Receptors[I], I + 1);
                PrintTotalConcentration(meteo.AMB, MC, I);
                os<< std::endl;
            }

            PrintJobAndMeteo(site, meteo);

            //std::string linkCodes = LinkCodes("    ", site.Links);

            os  << std::endl
                << "IV.  MODEL RESULTS (RECEPTOR-LINK MATRIX)"
                << std::endl
                << "                            *                                                     CO/LINK"
                << "                            *        COORDINATES (M)        * + AMB *              (PPM)"
                << "       RECEPTOR             *      X        Y        Z      * (PPM) * " << LinkCodes("    ", site.Links)
                << std::string(102, '-')
                << std::endl;

            for (std::size_t I = 0; I < site.Receptors.size(); I++)
            {
                os << (I + 1) << "." << site.Receptors[I].RCP;
                PrintConcentrations(MC, I);
                os << std::endl;
            }
        }
    }
}
