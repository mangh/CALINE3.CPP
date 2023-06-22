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
#ifndef JOB_H
#define JOB_H

#include <ostream>
#include <string>
#include <vector>

#include "Receptor.h"
#include "Meteo.h"
#include "Link.h"

// Units required/suplementary:
#include "Centimeter.h"
#include "Centimeter_Sec.h"
#include "Minute.h"

namespace CALINE3
{
    using namespace Metrology;
    using namespace Maths;

    /**
     * @brief Job info.
     * @param JOB - job title/description;
     * @param ATIM - averaging time;
     * @param Z0 - surface roughness;
     * @param VS - settling velocity;
     * @param VD - deposition velocity;
     * @param NR - number of receptors;
     * @param SCAL - scale factor to convert lengths in meters;
     * @param RUN - run description
     * --and--
     * @param Links - link list;
     * @param Receptors - receptor list;
     * @param Meteos - meteo conditions.
     */
    struct Job
    {
        ///////////////////////////////////////////////////////////////////
        ///
        ///     Constants
        ///

        // Averaging time 3 minutes
        static constexpr Minute ATIM_3MIN = Minute(3.0);
        // Averaging time 30 minutes
        static constexpr Minute ATIM_30MIN = Minute(30.0);

        // Roughness length 3 centimeters
        static constexpr Centimeter Z0_3CM = Centimeter(3.0);
        // Roughness length 10 centimeters
        static constexpr Centimeter Z0_10CM = Centimeter(10.0);

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Properties
        ///

        /// @brief Job ordinal number.
        const std::size_t ORDINAL;

        /// @brief Job title (description).
        const std::string JOB;

        /// @brief Averaging time [minute].
        /// @pre : 3 min <= ATIM <= 120 min (reasonable limits of power law approximation).
        const Minute ATIM;

        /// @brief Surface roughness [cm].
        /// @pre : 3 cm <= Z0 <= 400 cm (reasonable limits of power law approximation).
        const Centimeter Z0;

        /// @brief Settling velocity [cm/sec]
        /// @remarks Original input velocity; see VS for the velocity used in computation.
        const Centimeter_Sec VS1;

        /// @brief Deposition velocity [cm/sec]
        /// @remarks Original input velocity; see VD for the velocity used in computation.
        const Centimeter_Sec VD1;

        /// @brief Number of receptors.
        /// @pre : NR <= 20.
        const std::size_t NR;

        /// @brief Scale factor to convert coordinates, link height and width to meters.
        /// @remarks SCAL=1.0 when coordinates, link height and width are input in meters.
        const double SCAL;

        /// @brief Settling velocity [m/sec]
        /// i.e. the rate at which a particle falls with respect to its immediate surroundings.
        const Meter_Sec VS;

        /// @brief Deposition velocity [m/sec]
        /// i.e. the rate at which pollutant can be adsorbed or assimilated by a surface.
        /// @remarks If the settling velocity VS > 0, the deposition velocity should be set equal to the settling velocity.
        const Meter_Sec VD;

        /// @brief V1 = VD - VS / 2.0 [m/sec].
        const Meter_Sec V1;

        /// @brief Run title (description).
        std::string RUN;

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Properties: Time averaging factors
        ///

        /// @brief Time averaging factor = (ATIM/3.0)^0.2.
        const double AFAC_3MIN_02;

        /// @brief Time averaging factor = (ATIM/30.0)^0.2.
        const double AFAC_30MIN_02;

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Properties: Surface roughness factors
        ///

        /// @brief Surface roughness related factor = (Z0/3.0)^0.2.
        const double RFAC_3CM_02;

        /// @brief Surface roughness related factor = (Z0/3.0)^0.07.
        const double RFAC_3CM_007;

        /// @brief Surface roughness related factor = (Z0/10.0)^0.07.
        const double RFAC_10CM_007;

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Properties: Job details (meteo conditions, links, receptors)
        ///

        /// @brief Meteo collection.
        std::vector<Meteo> Meteos;

        /// @brief Link collection.
        std::vector<Link> Links;

        /// @brief Receptor collection.
        std::vector<Receptor> Receptors;

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Constructor(s)
        ///

        /**
         * @brief No default constructor!
         */
        Job() = delete;

        /**
         * @brief Job constructor.
         * @param ordinal - Job ordinal number
         * @param job - Job title (description)
         * @param atim - averaging time
         * @param z0 - surface roughness
         * @param vs - settling velocity
         * @param vd - deposition velocity
         * @param nr - number of receptors
         * @param scal - scale factor to convert lengths in meters
         */
        Job(std::size_t ordinal, std::string job, Minute atim, Centimeter z0, Centimeter_Sec vs, Centimeter_Sec vd, std::size_t nr, double scal) :
            ORDINAL(ordinal),
            JOB(job),
            ATIM(atim),
            Z0(z0),
            VS1(vs),
            VD1(vd),
            NR(nr),
            SCAL(scal),

            // Convert [cm/s] to [m/s]
            // VS(METER_SEC_FROM(Centimeter_Sec, VS1)),
            // VD(METER_SEC_FROM(Centimeter_Sec, VD1)),
            VS(Meter_Sec(VS1)),
            VD(Meter_Sec(VD1)),
            V1(VD - VS / 2.0),

            RUN(""),
            
            AFAC_3MIN_02(pow((ATIM / ATIM_3MIN), 0.2)),
            AFAC_30MIN_02(pow(ATIM / ATIM_30MIN, 0.2)),
            RFAC_3CM_02(pow(Z0 / Z0_3CM, 0.2)),
            RFAC_3CM_007(pow(Z0 / Z0_3CM, 0.07)),
            RFAC_10CM_007(pow(Z0 / Z0_10CM, 0.07))
        {
            Receptors = std::vector<Receptor>();
            Links = std::vector<Link>();
            Meteos = std::vector<Meteo>();
        }

        void setRUN(std::string run) { RUN = run; }

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Formatting
        ///

        friend std::ostream& operator<<(std::ostream& os, const Job& job);
    };
}
#endif /* !JOB_H */
