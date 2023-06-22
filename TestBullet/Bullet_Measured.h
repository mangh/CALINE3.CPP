#ifndef BULLET_MEASURED_H
#define BULLET_MEASURED_H

#include <tuple>
#include <vector>

#include "Benchmark.h"

// Units required/suplementary:
#include "Degree.h"
#include "Meter.h"
#include "Meter_Sec.h"
#include "Meter_Sec2.h"
#include "Second.h"

namespace Bullet::Measured
{
    using namespace CALINE3::Metrology;

    struct Calculator
    {
        using result_t = std::tuple<Degree, Second, Meter, Meter>;

        const Meter_Sec2 g{ 9.80665 };   // the gravitational acceleration
        const Meter h;      // the initial height of the projectile
        const Meter_Sec v;  // the velocity at which the projectile is launched

        std::vector<result_t> results;
        double elapsed;
        Benchmark benchmark;

        Calculator(Meter height, Meter_Sec velocity) :
            h(height),
            v(velocity),
            results(),
            elapsed(),
            benchmark()
        {
        }

        result_t CalculateRange(Degree slope);

        std::vector<result_t> Compute(Degree min, Degree max, Degree step);
    };
}
#endif /* !BULLET_MEASURED_H */
