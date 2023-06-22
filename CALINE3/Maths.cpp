#include "Maths.h"

namespace CALINE3::Maths
{
    using namespace Metrology;

    //////////////////////////////////////////////////////////////////////
    //
    //  2-dimensional Euclidean geometry.
    //

    Radian Azimuth(Meter a, Meter b, Meter x, Meter y)
    {
        static constexpr Radian ZERO{ 0.0 };
        static const Radian PI{ std::acos(-1.0) };
        static const Radian PI_2 = PI / 2.0;
        static const Radian PI3_2 = PI_2 * 3.0;

        return
            (x > a) ? PI_2 - atan((y - b) / (x - a)) :
            (x < a) ? PI3_2 - atan((y - b) / (x - a)) :
            (y < b) ? PI : ZERO;
    }

    Meter Distance(Meter x1, Meter y1, Meter x2, Meter y2)
    {
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    //////////////////////////////////////////////////////////////////////
    //
    //  Mathematical statistics.
    //

    double Erf(double x)
    {
        double t = 1.0 / (1.0 + 0.3275911 * ((x < 0) ? -x : x));
        double erfx = exp(-x * x) * t * (0.254829592 + t * (-0.284496736 + t * (1.421413741 + t * (-1.453152027 + t * 1.061405429))));
        return (x < 0) ? erfx - 1.0 : 1.0 - erfx;
    }
}