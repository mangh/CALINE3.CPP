#include <chrono>

#include "Bullet_Measured.h"
#include "Maths.h"

namespace Bullet::Measured
{
    using namespace CALINE3::Maths;
        
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

    std::vector<Calculator::result_t> Calculator::Compute(Degree min, Degree max, Degree step)
    {
        results.clear();
        //benchmark.Reset();

        const auto t1 = std::chrono::steady_clock::now();
        for (Degree slope = min; slope < max; slope += step) results.push_back(CalculateRange(slope));
        const auto t2 = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::micro> duration = t2 - t1;

        benchmark.Add(elapsed = duration.count());
        return results;
    }
}
