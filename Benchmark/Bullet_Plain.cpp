#include "Bullet_Plain.h"

namespace Bullet::Plain
{
    size_t Calculator::Compute(double min, double max, double step)
    {
        results.clear();
        for (double slope = min; slope < max; slope += step)
        {
            results.push_back(CalculateRange(slope));
        }
        return results.size();
    }

    Calculator::result_t Calculator::CalculateRange(double slope)
    {
        // the angle (in Radians) at which the projectile is launched
        double angle = (Math_PI / 180.0) * slope;

        // the vertical component of the velocity
        double vy = v * std::sin(angle);

        // the time it takes for the projectile to finish its trajectory:
        double tmax = (vy + std::sqrt(vy * vy + 2.0 * g * h)) / g;

        // the total horizontal distance traveled by the projectile
        double xmax = v * std::cos(angle) * tmax;

        // maximum vertical displacement of the projectile
        double ymax = h;
        for (double t = 0.0; t < tmax; t++)
        {
            auto y = h + (vy - g * t) * t / 2.0;
            if (y > ymax) ymax = y;
        }

        return std::make_tuple(slope, tmax, xmax, ymax);
    }
}

