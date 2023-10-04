#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <iostream>
#include "Bullet_Plain.h"
#include "Bullet_Measured.h"

#include "Meter2.h"
#include "Foot.h"
#include "Yard.h"

using namespace CALINE3::Metrology;
using PlainCalculator = Bullet::Plain::Calculator;
using MeasuredCalculator = Bullet::Measured::Calculator;

#define MATH_PI 3.141592653589793238462643383279502884L
#define MATH_E  2.718281828459045235360287471352662498L

TEST_CASE("Bullet", "[!benchmark]")
{
    // Bullet slope: range and step values
    constexpr double MIN = 0.0;
    constexpr double MAX = 90.0;
    constexpr double STEP = 0.1;

    // Plain and measured (with units) bullet samplers
    PlainCalculator plain{ 0.0, 715.0 };
    MeasuredCalculator measured{ Meter{ 0.0 }, Meter_Sec{ 715.0 } };

    BENCHMARK("bullet plain")
    {
        return plain.Compute(MIN, MAX, STEP);
    };

    BENCHMARK("bullet measured")
    {
        return measured.Compute(Degree{ MIN }, Degree{ MAX }, Degree{ STEP });
    };

    std::vector<PlainCalculator::result_t> p = plain.Results();
    std::vector<MeasuredCalculator::result_t> m = measured.Results();

    REQUIRE(p.size() == m.size());

    Degree Slope; Second Tmax; Meter Xmax; Meter Ymax;
    double slope; double tmax; double xmax; double ymax;

    for (std::size_t i = 0; i < m.size(); i++)
    {
        std::tie(Slope, Tmax, Xmax, Ymax) = m[i];
        std::tie(slope, tmax, xmax, ymax) = p[i];

        // Measured and plain results must be equal. Is this the case?
        CHECK(slope == Slope.value());
        CHECK(tmax == Tmax.value());
        CHECK(xmax == Xmax.value());
        CHECK(ymax == Ymax.value());
    }
}

TEST_CASE("Math", "[!benchmark]")
{
    double x = MATH_PI;
    double y = MATH_E;

    Meter X{ MATH_PI };
    Meter Y{ MATH_E };

    SECTION("addition")
    {
        REQUIRE((x+y) == (X+Y).value());

        BENCHMARK("addition plain"){ return (x + y); };
        BENCHMARK("addition measured"){ return (X + Y); };
    }

    SECTION("subtraction")
    {
        REQUIRE((x-y) == (X-Y).value());

        BENCHMARK("subtraction plain"){ return (x - y); };
        BENCHMARK("subtraction measured"){ return (X - Y); };
    }

    SECTION("multiplication")
    {
        REQUIRE((x*y) == (X*Y).value());

        BENCHMARK("multiplication plain"){ return (x * y); };
        BENCHMARK("multiplication measured"){ return (X * Y); };
    }

    SECTION("division")
    {
        Second S{ y };

        REQUIRE((x/y) == (X/S).value());

        BENCHMARK("division plain"){ return (x / y); };
        BENCHMARK("division measured"){ return (X / S); };
    }

    SECTION("conversion")
    {
        REQUIRE(((Foot::factor / Meter::factor) * x) == Foot{X}.value());

        BENCHMARK("conversion plain"){ return ((Foot::factor / Meter::factor) * x); };
        BENCHMARK("conversion measured"){ return Foot{X}; };

    }

    SECTION("assignment")
    {
        double z{ MATH_PI };
        Meter Z{ MATH_E };

        BENCHMARK("assignment plain"){ return z = x; };
        BENCHMARK("assignment measured"){ return Z = X; };

        REQUIRE(z == Z.value());
    }

    SECTION("mix")
    {
        double z {0.0};
        Meter Z{0.0};

        BENCHMARK("mix plain")
        {
            double sum = x + y;
            double diff = x - y;
            double product = x * y;
            double quotient = x / y;
            double yards = (Yard::factor / Meter::factor) * sum;
            return z = product / (sum + diff) * quotient + ((Meter::factor / Yard::factor) * yards);
           
        };

        BENCHMARK("mix measured")
        {
            Meter sum = X + Y;
            Meter diff = X - Y;
            Meter2 product = X * Y;
            double quotient = X / Y;
            Yard yards = Yard(sum);
            return Z = product / (sum + diff) * quotient + Meter(yards);
           
        };

        REQUIRE(z == Z.value());
    }
}

