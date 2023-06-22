#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <string>
#include <iostream>

// Units required/suplementary:
#include "Meter.h"

namespace CALINE3
{
    using namespace Metrology;

    /**
     * @brief Receptor info:
     * @param RCP - description,
     * @param XR - X-coordinate,
     * @param YR - Y-coordinate,
     * @param ZR - Z-coordinate.
     */
    struct Receptor
    {
        ///////////////////////////////////////////////////////////////////
        ///
        ///     Properties
        ///

        /// @brief Receptor ordinal number (within the receptor set).
        const std::size_t ORDINAL;

        /// @brief Receptor description.
        const std::string RCP;

        /// @brief Receptor X-coordinate [m].
        const Meter XR;

        /// @brief Receptor Y-coordinate [m].
        const Meter YR;

        /// @brief Receptor Z-coordinate [m].
        /// @pre
        ///   ZR >= 0: Gaussian plume reflected at air-surface interface;
        ///   model assumes plume transport over horizontal plane.
        ///   For receptors within a depressed area it is permitted: ZR >= H (where H <= 0).
        const Meter ZR;

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Constructor(s)
        ///

        /**
         * @brief No default constructor!
         */
        Receptor() = delete;

        /**
         * @brief Receptor constructor
         * @param ordinal - receptor ordinal number,
         * @param rcp     - receptor description,
         * @param xr      - receptor X-coordinate [m],
         * @param yr      - receptor Y-coordinate [m],
         * @param zr      - receptor Z-coordinate [m].
         */
        Receptor(std::size_t ordinal, std::string rcp, Meter xr, Meter yr, Meter zr) :
            ORDINAL(ordinal),
            RCP(rcp),
            XR(xr),
            YR(yr),
            ZR(zr)
        {
        }

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Formatting
        ///

        friend std::ostream& operator<<(std::ostream& os, const Receptor& rcp);
    };
}

#endif /* !RECEPTOR_H */