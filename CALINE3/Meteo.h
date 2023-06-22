#ifndef METEO_H
#define METEO_H

#include <string>
#include <iostream>

// Units required/suplementary:
#include "Degree.h"
#include "Meter.h"
#include "Meter_Sec.h"
#include "Ppm.h"

namespace CALINE3
{
    using namespace Metrology;

    /**
     * @brief Meteo conditions:
     * @param U - wind speed,
     * @param BRG - wind direction,
     * @param CLAS - stability class,
     * @param MIXH - mixing height,
     * @param AMB - ambient concentration of pollutant.
     */
    struct Meteo
    {
    private:

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Constants
        ///

        // Coefficients (dependent on stability class) for
        // the power-law formulas used in the Plume module

        static constexpr double _AZ[6]{ 1112.0, 556.0, 353.0, 219.0, 124.0, 56.0 };
        static constexpr double _AY1[6]{ 0.46, 0.29, 0.18, 0.11, 0.087, 0.057 };
        static constexpr double _AY2[6]{ 1831.0, 1155.0, 717.0, 438.0, 346.0, 227.0 };

        /// @brief Stability class tags.
        static constexpr const char* STB[6] = { "A", "B", "C", "D", "E", "F" };

    public:

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Properties
        ///

        /// @brief Meteo ordinal number
        const std::size_t ORDINAL;

        /// @brief Wind speed [m/s].
        /// @pre : U >= 1 m/s.
        /// Along-wind diffusion can be considered negligible relative to U >= 1 m/s (Gaussian assumption).
        const Meter_Sec U;

        /// @brief Wind angle for output [deg].
        /// @remarks Original input angle; see BRG for the angle used in computation.
        const Degree BRG1;

        /// @brief Stability class in numeric format (1-6=A-F).
        const int CLAS;

        /// @brief Mixing height [m]
        const Meter MIXH;

        /// @brief Ambient concentration of pollutant [ppm].
        const Ppm AMB;

    private:

        /// @brief Wind azimuth bearing [degree] measured relative to positive Y-axis.
        /// @pre : 0 [deg] <= BRG <= 360 [deg].
        /// @remarks Example: BRG = 270 [deg] for a wind from the West.
        Degree m_brg;

    public:

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Constructor(s)
        ///

        /**
         * @brief No default constructor.
         */
        Meteo() = delete;

        /**
         * @brief Meteo constructor.
         * @param ordinal - meteo ordinal number,
         * @param u - wind speed [m/s],
         * @param brg - wind direction [deg],
         * @param clas - stability class,
         * @param mixh - mixing height [m],
         * @param amb - ambient concentration of pollutant [ppm].
         */
        Meteo(std::size_t ordinal, Meter_Sec u, Degree brg, int clas, Meter mixh, Ppm amb);

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Methods
        ///

        /**
         * @brief Coefficient (1/2) used in computing Sigma-y (horizontal dispersion).
         */
        double AY1() const { return _AY1[CLAS - 1]; }

        /**
         * @brief Coefficient (2/2) used in computing Sigma-y (horizontal dispersion).
         */
        double AY2() const { return _AY2[CLAS - 1]; }

        /**
         * @brief Coefficient used in computing Sigma-z (vertical dispersion).
         */
        double AZ() const { return _AZ[CLAS - 1]; }

        /**
         * @brief Stability class tag.
         */
        std::string TAG() const { return STB[CLAS - 1]; }

        /**
         * @brief Wind bearing [degree] measured relative to positive Y-axis.
         */
        Degree BRG() const { return m_brg; }

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Formatting
        ///

        friend std::ostream& operator<<(std::ostream& os, const Meteo& met);
    };
}
#endif /* !METEO_H */
