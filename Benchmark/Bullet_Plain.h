#ifndef BULLET_PLAIN_H
#define BULLET_PLAIN_H

#include <cmath>
#include <tuple>
#include <vector>

namespace Bullet::Plain
{
    struct Calculator
    {
        using result_t = std::tuple<double, double, double, double>;

        const double Math_PI{ std::acos(-1.0) };
        const double g = 9.80665; // the gravitational acceleration (Meter/Sec2)
        const double h;           // the initial height of the projectile (Meter)
        const double v;           // the velocity at which the projectile is launched (Meter/Sec)

        Calculator(double height, double velocity) :
            h(height),
            v(velocity),
            results()
        {
        }
    
        size_t Compute(double min, double max, double step);
        std::vector<result_t> Results() { return results; };

    private:

        result_t CalculateRange(double slope);
        std::vector<result_t> results;
    };
}
#endif /* !BULLET_PLAIN_H */

