#ifndef MATHS_H
#define MATHS_H

#include <cmath>

// Units required/suplementary:
#include "Degree.h"
#include "Meter.h"
#include "Meter_Sec.h"
#include "Meter2.h"
#include "Meter2_Sec2.h"
#include "Ppm.h"
#include "Radian.h"

namespace CALINE3::Maths
{
    using namespace Metrology;

    ///////////////////////////////////////////////////////////////////////
    ///
    ///  Constants
    ///

    ///////////////////////////////////////////////////////////////////////
    ///
    ///  Metrology math
    ///

    // Metrology math

    inline double sin(Radian angle) { return std::sin(angle.value()); }
    inline double cos(Radian angle) { return std::cos(angle.value()); }
    inline Radian atan(double x) { return Radian(std::atan(x)); }
    inline Degree abs(Degree d) { return Degree(std::abs(d.value())); }
    inline Meter abs(Meter d) { return Meter(std::abs(d.value())); }
    inline Meter sqrt(Meter2 area) { return Meter(std::sqrt(area.value())); }
    inline Meter_Sec sqrt(Meter2_Sec2 sq_velocity) { return Meter_Sec(std::sqrt(sq_velocity.value())); }
    inline Ppm round(Ppm q) { return Ppm(std::round(q.value())); }

    /**
     * @brief Exponentiation used in power-law formulas for dispersion parameters.
     * @param d - distance/length [m],
     * @param y - exponent [dimensionless].
     * @returns distance/length raised to the power of y (d^y); result expressed in [m].
     * @remarks
     * Power-law formula:
     * 
     *   Sigma[m] = a * (d[m])^b;   a, b - constants
     * 
     * is an analytical approximation of the plot of experimental data.
     * It is assumed to give results in [m] for the distance argument(x) in [m]
     * although such a relationship cannot be derived from the formula (unless y == 1)!
     */
    inline Meter pow(Meter d, double y) { return Meter(std::pow(d.value(), y)); }

    // Some standard math

    inline double abs(double x) { return std::abs(x); }
    inline double log(double x) { return std::log(x); }
    inline double exp(double x) { return std::exp(x); }
    inline double pow(double x, double y) { return std::pow(x, y); }
    inline double sqrt(double x) { return std::sqrt(x); }

    ///////////////////////////////////////////////////////////////////////
    ///
    ///  2-dimensional Euclidean geometry.
    ///

    /**
     * @brief Clockwise angle [rad] formed by North direction (Y-axis) and
     * vector starting from the point (a,b) and terminating at the point (x,y).
     * @param a - X-coordinate of start point [m],
     * @param b - Y-coordinate of start point [m],
     * @param x - X-coordinate of end point [m],
     * @param y - Y-coordinate of end point [m]
     * @returns 0 <= angle < 2*PI [rad].
     */
    Radian Azimuth(Meter a, Meter b, Meter x, Meter y);

    /**
     * @brief Distance between points on a 2D-plane.
     * 
     * @param x1 - X-coordinate of the 1st point [m],
     * @param y1 - Y-coordinate of the 1st point [m],
     * @param x2 - X-coordinate of the 2nd point [m],
     * @param y2 - Y-coordinate of the 2nd point [m]
     * @returns distance [m] between the points
     */
    Meter Distance(Meter x1, Meter y1, Meter x2, Meter y2);

    //////////////////////////////////////////////////////////////////////
    //
    //  Mathematical statistics.
    //

    /**
     * @brief Gauss error function
     * (maximum error: 1.5e-7).
     * @param x - any (positive or negative) value.
     * @returns For a random variable Y that is normally distributed
     * with mean 0 and standard deviation 1/sqrt(2),
     * erf(x) is the probability that Y falls in the range [-x, x].
     * @remarks See: Abramowitz and Stegun approximation in Wikipedia (http://en.wikipedia.org/wiki/Error_function).
     */
    double Erf(double x);
}
#endif /* !MATHS_H */
