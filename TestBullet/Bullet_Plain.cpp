#include <chrono>

#include "Bullet_Plain.h"

namespace Bullet::Plain
{
    Calculator::result_t Calculator::CalculateRange(double slope)
    {
        // the angle (in Radians) at which the projectile is launched
        double angle = (Math_PI / 180.0) * slope;

        // the vertical component of the velocity
        double vy = v * sin(angle);

        // the time it takes for the projectile to finish its trajectory:
        double tmax = (vy + std::sqrt(vy * vy + 2.0 * g * h)) / g;

        // the total horizontal distance traveled by the projectile
        double xmax = v * cos(angle) * tmax;

        // maximum vertical displacement of the projectile
        double ymax = h;
        for (double t = 0.0; t < tmax; t++)
        {
            auto y = h + (vy - g * t) * t / 2.0;
            if (y > ymax) ymax = y;
        }

        return std::make_tuple(slope, tmax, xmax, ymax);
    }

    std::vector<Calculator::result_t> Calculator::Compute(double min, double max, double step)
    {
        results.clear();
        //benchmark.Reset();

        const auto t1 = std::chrono::high_resolution_clock::now();
        for (double slope = min; slope < max; slope += step) results.push_back(CalculateRange(slope));
        const auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> duration = t2 - t1;

        benchmark.Add(elapsed = duration.count());
        return results;
    }
}
