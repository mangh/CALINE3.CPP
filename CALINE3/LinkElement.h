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

#ifndef LINK_ELEMENT_H
#define LINK_ELEMENT_H

#include "Link.h"
#include "WindFlow.h"

namespace CALINE3
{
    using namespace Metrology;
    using namespace Maths;

    /**
     * @brief An element (section) of the Link as seen from a Receptor.
     * @remarks Links are divided into a series of elements from which
     * incremental concentrations are computed and then summed
     * to form a total concentration estimate for a particular Receptor location.
     */
    struct LinkElement
    {
        ///////////////////////////////////////////////////////////////////////
        ///
        ///  Constants
        ///


        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Properties
        ///

        /// @brief Master (source) Link.
        const Link &master;

        /// @brief Wind flow parameters.
        const WindFlow &flow;

        /// @brief Element half-length.
        const Meter EL2;

        /// @brief Element centerline distance.
        const Meter ECLD;

        /// @brief Equivalent line half-length.
        const Meter ELL2;

        /// @brief Central sub-element half-length.
        const Meter CSL2;

        /// @brief Central sub-element half-width.
        const Meter EM2;

        /// @brief Peripheral sub-element width.
        const Meter EN2;

        ///////////////////////////////////////////////////////////////////////////
        // 
        //      Constructor(s)
        //

        /**
         * @brief No default constructor!
         */
        LinkElement() = delete;

        /**
         * @brief LinkElement constructor.
         * @param master - master (source) Link,
         * @param flow - wind flow paramaters,
         * @param ED1 - element start position,
         * @param ED2 - element end position.
         */
        LinkElement(const Link& master, const WindFlow &flow, const Meter ED1, const Meter ED2) :
            master(master),
            flow(flow),
            EL2(abs(ED2 - ED1) / 2.0),
            ECLD(-(ED1 + ED2) / 2.0),
            ELL2(master.W2()* cos(flow.TETA()) + EL2 * sin(flow.TETA())),
            CSL2((flow.TETA() >= atan(master.W2() / EL2)) ? master.W2() / sin(flow.TETA()) : EL2 / cos(flow.TETA())),
            EM2(abs(EL2 * sin(flow.TETA()) - master.W2() * cos(flow.TETA()))),
            EN2((ELL2 - EM2) / 2.0)
        {
        }

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Methods
        ///

        /**
         * @brief Computes LinkElement profile as seen from the receptor at the distance "D".
         * @param D - receptor-link distance (perpendicular to the link) [m],
         * @param QE - central subelement lineal strength [microgram/(m*s)],
         * @param YE - plume centerline offset [m],
         * @param FET - element fetch [m].
         * @returns true if the element contributes to the pollution and the profile
         * (QE, YE, FET) has been computed; false otherwise.
         */
        bool GetProfile(Meter D, Microgram_Meter_Sec& QE, Meter& YE, Meter& FET) const;

        /**
         * @brief Computes element source strength [microgram/(m*s)].
         * @param QE - central subelement lineal strength [microgram/(m*s)],
         * @param SGY - sigmay/horizontal dispersion parameter [m],
         * @param YE - plume centerline offset [m].
         * @returns Element source strength [microgram/(m*s)].
         */
        Microgram_Meter_Sec SourceStrength(Microgram_Meter_Sec QE, Meter SGY, Meter YE) const;

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Formatting
        ///

        friend std::ostream& operator<<(std::ostream& os, const LinkElement& elem);
    };
}

#endif /* !LINK_ELEMENT_H */
