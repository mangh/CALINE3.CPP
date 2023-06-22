/*******************************************************************************

    Units of Measurement for C# applications applied to
    the CALINE3 Model algorithm.

    For more information on CALINE3 and its status see:
    * https://www.epa.gov/scram/air-quality-dispersion-modeling-alternative-models#caline3
    * https://www.epa.gov/scram/2017-appendix-w-final-rule.

    Copyright (C) mangh

    This program is provided to you under the terms of the license
    as published at https://github.com/mangh/metrology.

********************************************************************************/

#ifndef PLUME_H
#define PLUME_H

#include <tuple>

#include "Job.h"
#include "WindFlow.h"
#include "LinkElement.h"

// Units required/suplementary:
#include "Meter_Sec.h"
#include "Meter2_Sec.h"
#include "Microgram_Meter3.h"

namespace CALINE3
{
    /*
     * Algorithms in this module make use of the power-law formulas:
     *
     *    sgy[m] = a * (x[m])^b  (horizontal dispersion parameter)
     *    sgz[m] = c * (x[m])^d  (vertical dispersion parameter)
     *
     *    a, b, c, d - constants dependent on stability class
     *
     * These formulas is an analytical approximation of a plot of
     * experimental data. They are assumed to give results (sgy, sgz) in [m]
     * for the distance argument (x) in [m] although such a relationship
     * cannot be derived from the formula!
     * This may be the source of dimensional inconsistencies!
     *
     * The time averaging and surface roughness corrections applied here - both
     * dimensionally unclear - give rise to further issues.
     *
     * Therefore I had to "hack" in places - mainly in the constructor - to make
     * the algorithm dimensionally consistent and getting still the same results
     * as the original CALINE3.
     *
     * I hope I did it right.
     *
     */

    using namespace Metrology;
    using namespace Maths;

    /**
     * @brief Gaussian plume calculator.
     */
    struct Plume
    {
        ///////////////////////////////////////////////////////////////////////
        ///
        ///  Constants
        ///

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Constructor(s)
        ///

        /**
         * @brief No default constructor!
         */
        Plume() = delete;

        /**
         * @brief Plume constructor.
         * @param site,
         * @param met,
         * @param link.
         */
        Plume(const Job& site, const Meteo& met, const Link& link);

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Methods
        ///

        /**
         * @brief Pollutant concentration [microgram/m3] at the receptor location.
         * @param receptor - receptor.
         * @returns
         */
        Microgram_Meter3 ConcentrationAt(const Receptor& receptor);

    private:

        /**
         * @brief Incremental concentration [microgram/m3] from the element
         * at the distance D and at the level Z.
         * @param element - link element,
         * @param D - receptor-link distance [m],
         * @param Z - receptor level (adjusted to the Link type) [m].
         * @returns 
         */
        Microgram_Meter3 ConcentrationFrom(const LinkElement& element, Meter D, Meter Z) const;

        /**
         * @brief Computes deposition factor.
         */
        double DepositionFactor(Meter SGZ, Meter2_Sec KZ, Meter Z, Meter H, Meter_Sec V1) const;

        /**
         * @brief Computes settling factor.
         */
        double SettlingFactor(Meter SGZ, Meter2_Sec KZ, Meter Z, Meter H, Meter_Sec VS) const;

        /**
         * @brief Computes Gaussian factor.
         */
        double GaussianFactor(Meter SGZ, Meter Z, Meter H, Meter MIXH) const;

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Fields: environmental conditions
        ///

        /// @brief Job site factors.
        const Job& _site;

        /// @brief Meteo conditions.
        const Meteo& _meteo;

        /// @brief Link parameters.
        const Link& _link;

        /// @brief Wind flow geometry
        const WindFlow _flow;

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Fields: Gaussian plume dispersion parameters
        ///      (essentially constant but hardly feasible to
        ///       make it constant in constructor initializer)
        ///

        /// @brief Coefficient "a" in the power-curve formula: sgy[m] = a * (distance[m])^b.
        double PY1;

        /// @brief Coefficient "b" in the power-curve formula: sgy[m] = a * (distance[m])^b.
        double PY2;

        /// @brief Coefficient "c" in power-curve formula: sgz[m] = c * (distance[m])^d.
        double PZ1;

        /// @brief Coefficient "d" in the power-curve formula: sgz[m] = c * (distance[m])^d.
        double PZ2;
    };
}

#endif /* !PLUME_H */
