#include "Bullet_Measured.h"
#include "Math.h"

namespace Bullet::Measured
{
    using namespace CALINE3::Metrology;

    size_t Calculator::Compute(Degree min, Degree max, Degree step)
    {
        results.clear();
        for (Degree slope = min; slope < max; slope += step)
        {
            results.push_back(CalculateRange(slope));
        }
        return results.size();
    }

    Calculator::result_t Calculator::CalculateRange(Degree slope)
    {
        // the angle (in Radians) at which the projectile is launched
        Radian angle{ slope };

        // the vertical component of the velocity
        Meter_Sec vy = v * sin(angle);

        // the time it takes for the projectile to finish its trajectory:
        Second tmax = (vy + sqrt(vy * vy + 2.0 * g * h)) / g;

        // the total horizontal distance traveled by the projectile
        Meter xmax = v * cos(angle) * tmax;

        // maximum vertical displacement of the projectile
        Meter ymax = h;
        for (Second t{0.0}; t < tmax; ++t)
        {
            Meter y = h + (vy - g * t) * t / 2.0;
            if (y > ymax) ymax = y;
        }

        return std::make_tuple(slope, tmax, xmax, ymax);
    }
}

