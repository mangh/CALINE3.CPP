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

#ifndef WIND_FLOW_H
#define WIND_FLOW_H

#include "Meteo.h"
#include "Link.h"

// Units required/suplementary:
#include "Second.h"

namespace CALINE3
{
    using namespace Metrology;
    using namespace Maths;

    /**
     * @brief Geometry of the wind flow (relative to the link)
     * @param BASE - element growth factor;
     * @param PHI - the angle between the wind and the link;
     * @param TETA - the normalized angle between the wind and the link (PHI % 90);
     */
    struct WindFlow
    {
        ///////////////////////////////////////////////////////////////////////////
        // 
        //      Constants
        //

        ///////////////////////////////////////////////////////////////////////////
        // 
        //      Properties
        //

    private:

        /// @brief Element growth factor [dimensionless].
        double m_base;

        /// @brief The angle [rad] between the wind direction and the direction of the link.
        Radian m_phi;

        /// @brief The normalized angle [rad] between the wind and the roadway (that is PHI % 90 [rad]).
        Radian m_teta;

        /// @brief Mixing zone residence time [s].
        Second m_tr;

    public:

        ///////////////////////////////////////////////////////////////////////////
        // 
        //      Constructor(s)
        //

        /**
         * @brief No default constructor!
         */
        WindFlow() = delete;

        /**
         * @brief WindFlow constructor.
         * @param meteo - meteo conditions,
         * @param steady - raw (steady) link.
         */
        WindFlow(const Meteo& meteo, const Link& steady);

        ///////////////////////////////////////////////////////////////////////////
        // 
        //      Methods
        //

        /**
         * @brief Element growth factor [dimensionless].
         */
        double BASE() const { return m_base; }

        /**
         * @brief The angle [rad] between the wind direction and the direction of the roadway.
         */
        Radian PHI() const { return m_phi; }

        /**
         * @brief The normalized angle [rad] between the wind and the roadway (that is PHI % 90 [rad]).
         */
        Radian TETA() const { return m_teta; }

        /**
         * @brief Mixing zone residence time [s].
         */
        Second TR() const { return m_tr; }

    };
}
#endif /* !WIND_FLOW_H */
