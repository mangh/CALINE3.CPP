#ifndef BULLET_PLAIN_H
#define BULLET_PLAIN_H

#include <cmath>
#include <tuple>
#include <vector>

#include "Benchmark.h"

namespace Bullet::Plain
{
    struct Calculator
    {
        using result_t = std::tuple<double, double, double, double>;

        const double Math_PI{ std::acos(-1.0) };
        const double g = 9.80665; // the gravitational acceleration (Meter/Sec2)
        const double h;           // the initial height of the projectile (Meter)
        const double v;           // the velocity at which the projectile is launched (Meter/Sec)

        std::vector<result_t> results;
        double elapsed;
        Benchmark benchmark;

        Calculator(double height, double velocity) :
            h(height),
            v(velocity),
            results(),
            elapsed(),
            benchmark()
        {
        }

        result_t CalculateRange(double slope);

        std::vector<result_t> Compute(double min, double max, double step);
    };
}
#endif /* !BULLET_PLAIN_H */
