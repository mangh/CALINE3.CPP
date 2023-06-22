#ifndef REPORT_H
#define REPORT_H

#include <iomanip>
#include <ostream>

#include "Job.h"
#include "Meteo.h"
#include "Link.h"

// Units required/suplementary:
#include "Gram_Mole.h"
#include "Meter3_Mole.h"
#include "Microgram_Meter3.h"
#include "Ppm.h"

namespace CALINE3
{
    using namespace Metrology;

    struct Report
    {
        ///////////////////////////////////////////////////////////////////////////
        // 
        //      Constants
        //

        /// @brief Molecular mass (weight) of CO [g/mol].
        /// @remarks
        /// According to Wikipedia (https://en.wikipedia.org/wiki/Carbon_monoxide)
        /// molar mass of CO = 28.010 [g/mol] that is greater than the value 28.0 [g/mol]
        /// used in the original CALINE3 application! Is that relevant?
        static constexpr Gram_Mole MOWT{ 28.0 };

        /// @brief Molar volume of an ideal gas [m3/mol]. 
        /// @remarks
        /// The molar volume (https://en.wikipedia.org/wiki/Molar_volume)
        /// of an ideal gas at 1 atmosphere of pressure is:
        /// 0.022413969545014... [m3/mol] at 0 deg.C,
        /// 0.024465403697038... [m3/mol] at 25 deg.C.
        static constexpr Meter3_Mole MOVL{ 0.0245 };

        /// @brief PPM factor [mcg/m3] to convert concentration from [mcg/m3] to [ppm].
        static const Microgram_Meter3 FPPM;

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Constructor(s)
        ///
        
        /**
         * @brief Report constructor.
         * @param ostr - report output stream.
         */
        Report(std::ostream &ostr)
            : os(ostr), PageCount(0)
        {}

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Methods
        ///

        /**
         * @brief Prints concentration matrix computed for a given site and meteo conditions.
         * @param site - site conditions,
         * @param meteo - meteo conditions,
         * @param MC - mass concentration matrix.
        */
        void Print(const Job& site, const Meteo& meteo, const std::vector<std::vector<Microgram_Meter3>> &MC);

    private:
        
        /**
         * @brief Converts concentration from [mcg/m3] to [ppm] unit.
         * @param mc - input (mass) concentration [mcg/m3].
         * @returns Concentration expressed in [ppm] units.
         */
        Ppm ToPPM(Microgram_Meter3 mc);

        std::string LinkCodes(const std::string& separator, const std::vector<Link>& links);

        void PrintJobAndMeteo(const Job& site, const Meteo& met);

        void PrintLinks(const std::vector<Link>& links);

        void PrintReceptorsHeader();

        void PrintReceptor(const Receptor &receptor, size_t SEQNO);

        /**
         * @brief Print concentrations at receptor point, in cross section of Links.
         * @param MC - mass concentration [microgram/meter3] array (MC[links][receptors]),
         * @param R - receptor index.
         */
        void PrintConcentrations(const std::vector<std::vector<Microgram_Meter3>> &MC, size_t R);

        /**
         * @brief Prints total (including ambient) concentration [ppm] at receptor point.
         * @param amb - ambient concentration,
         * @param MC - mass concentration [microgram/meter3] array (MC[links][receptors]),
         * @param R - receptor index.
         * @return Total concentration [ppm] at the receptor.
         */
        void PrintTotalConcentration(Ppm amb, const std::vector<std::vector<Microgram_Meter3>> &MC, size_t R);

        ///////////////////////////////////////////////////////////////////////////
        // 
        //      Fields
        //

        std::ostream &os;
        int PageCount;
    };
}

#endif /* !REPORT_H */
