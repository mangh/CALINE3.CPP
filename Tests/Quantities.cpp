#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// Length
#include "Meter.h"
#include "Centimeter.h"
#include "Inch.h"
#include "Foot.h"
#include "Millimeter.h"
#include "Mile.h"
#include "Yard.h"
#include "Kilometer.h"

// Time
#include "Second.h"
#include "Minute.h"
#include "Hour.h"

// Mass
#include "Kilogram.h"
#include "Gram.h"
#include "Pound.h"
#include "Ounce.h"
#include "Tonne.h"

// Temperatures
#include "DegKelvin.h"
#include "DegCelsius.h"
#include "DegRankine.h"
#include "DegFahrenheit.h"

// Frequency
#include "Hertz.h"
#include "Vehicles_Hour.h"

// Plane Angle
#include "Radian.h"
#include "Degree.h"
#include "Grad.h"
#include "Turn.h"

// Velocity
#include "MPH.h"

// Angular Frequency
#include "RPM.h"
#include "Radian_Sec.h"
#include "Degree_Sec.h"

// Force, Energy, Torque
#include "Joule.h"
#include "Newton.h"
#include "NewtonMeter.h"

#define CHECK_ALMOST_EQUAL(estimated, accurate)                     \
    {                                                               \
        CHECK(typeid(estimated) == typeid(accurate));               \
        CHECK_THAT(estimated.value(),                               \
            Catch::Matchers::WithinRel(accurate.value(), 1.0e-15)); \
    }

namespace CALINE3::Metrology
{
    constexpr double PI = 3.141592653589793238462643383279502884L;

    TEST_CASE("addition/subtraction (quantities in same unit)", "[quantities][operators]")
    {
        const double d1 = 123.0;
        const double d2 = 234.0;

        Meter q1{ d1 };
        Meter q2{ d2 };

        CHECK(q1 + q2 == Meter{ d1 + d2 });
        CHECK(q2 - q1 == Meter{ d2 - d1 });
        CHECK(-q2 == Meter{ -d2 });
        CHECK(q1 != q2);
        CHECK(q1 < q2);
        CHECK(q2 > q1);

        q2 -= q1;
        CHECK(q2 == Meter{ d2 - d1 });

        q2 += q1;
        CHECK(q2 == Meter{ d2 });
    }

    TEST_CASE("addition/subtraction (quantities in different units)", "[quantities][operators]")
    {
        Meter meters{ 5.0 };
        Centimeter centimeters{ 25.0 };

        Meter m{ meters + Meter(centimeters) };
        CHECK(Meter{ 5.25 } == m);

        Centimeter cm{ Centimeter(meters) + centimeters };
        CHECK(Centimeter{ 525.0 } == cm);

        m = meters - Meter(centimeters);
        CHECK(Meter{ 4.75 } == m);

        cm = Centimeter(meters) - centimeters;
        CHECK(Centimeter{ 475.0 } == cm);
    }

    TEST_CASE("increment/decrement (quantities)", "[quantities][operators]")
    {
        Meter initial{ 5.0 };
        Meter post = initial++;
        CHECK(initial == Meter{ 6.0 });

        Meter pre = --initial;
        CHECK(((post == pre) && (pre == initial) && (initial == Meter{ 5.0 })));
    }

    TEST_CASE("multiplication (quantities)", "[quantities][operators]")
    {
        MPH speed{ 100.0 }; // 100 mph
        Minute duration{ 30.0 };    // 30 minutes

        Mile miles = speed * Hour{ duration };
        CHECK_ALMOST_EQUAL(miles, Mile{50.0});

        Kilometer kilometers{ miles };
        CHECK_ALMOST_EQUAL(kilometers, Kilometer{80.4672});

        speed = miles / Hour{ duration };
        CHECK_ALMOST_EQUAL(speed, MPH{100.0});
    }

    TEST_CASE("compound assignment (quantities)", "[quantities][operators]")
    {
        Meter meters{ 5.0 };  // 5 meters

        meters *= 2.0;  // 10 meters
        CHECK(Meter{10.0} == meters);

        meters /= 5.0;  // 2 meters
        CHECK(Meter{2.0} == meters);

        meters += Meter{ Centimeter{75.0}};
        CHECK(Meter{2.75} == meters);

        meters -= Meter{ Centimeter{25.0}};
        CHECK(Meter{2.50} == meters);
    }

    TEST_CASE("length comparison", "[quantities][operators]")
    {
        Meter x{ 123.0 };
        Meter y{ 345.0 };

        CHECK(x != y);
        CHECK(x < y);
        CHECK(x + y == Meter{ 123.0 + 345.0 });
        CHECK(y - x == Meter{ 345.0 - 123.0 });
        CHECK(y / x == 345.0 / 123.0);
    }

    TEST_CASE("mass comparison", "[quantities][operators]")
    {
        {
            // 5.0 tonnes == 5000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 5000.0 };
            CHECK(tonnes.value() != kilograms.value());
            CHECK(tonnes == Tonne{kilograms});
        }
        {
            // 5 pounds < 5 kilograms
            Pound pounds{ 5.0 };
            Kilogram kilograms{ 5.0 };
            CHECK(kilograms.value() == pounds.value());
            CHECK(pounds < Pound{kilograms});
            CHECK(kilograms >= Kilogram{pounds});
        }
        {
            // 5 tonnes > 3000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 3000.0 };
            CHECK(tonnes.value() < kilograms.value());
            CHECK(tonnes != Tonne{kilograms});
            CHECK(Kilogram{tonnes} > kilograms);
            CHECK(Tonne{kilograms} <= tonnes);
        }
    }

    TEST_CASE("torque/energy", "[quantities][operators]")
    {
        // Joule and NewtonMeter belong to disjoint families
        CHECK(Joule::family != NewtonMeter::family);

        // Besides, they are not very different (except the operators used):
        Meter distance{ 10.0 };
        Newton force{ 100.0 };

        Joule energy = force * distance;
        CHECK(Joule{ 1000.0 } == energy);

        NewtonMeter torque = force ^ distance;
        CHECK(NewtonMeter{ 1000.0 } == torque);

        Meter distanceFromEnergy = energy / force;
        Meter distanceFromTorque = torque / force;
        CHECK(distanceFromEnergy == distanceFromTorque);

        Newton forceFromEnergy = energy / distance;
        Newton forceFromTorque = torque / distance;
        CHECK(forceFromEnergy == forceFromTorque);
    }

    TEST_CASE("floating point inaccuracy (quantities)", "[quantities][conversions]")
    {
        Meter expected{ 100.0 };

        Meter calculated{
            Yard{
                Foot{
                    Inch{
                        Centimeter{
                            Millimeter{expected }
                        }
                    }
                }
            }
        };

        // Unfortunately, due to the floating-point quirks ...:
        CHECK(expected != calculated);
        // but...:
        CHECK_ALMOST_EQUAL(calculated, expected);
        // CHECK(Meter{ 99.999999999999986 } == calculated);
    }

    TEST_CASE("length conversion", "[quantities][conversions]")
    {
        Meter meters{ 100.0 };

        Centimeter centimeters{ meters };
        Millimeter millimeters{ centimeters };
        Inch inches{ millimeters };
        Foot feet{ inches };
        Yard yards{ feet };
        Mile miles{ yards };
        meters = Meter(miles);

        CHECK_ALMOST_EQUAL(centimeters, Centimeter{ 10000.0 });
        CHECK_ALMOST_EQUAL(millimeters, Millimeter{ 100000.0 });
        CHECK_ALMOST_EQUAL(inches, Inch{ 3937.0078740157478 });
        CHECK_ALMOST_EQUAL(feet, Foot{ 328.08398950131232 });
        CHECK_ALMOST_EQUAL(yards, Yard{ 109.36132983377077 });
        CHECK_ALMOST_EQUAL(miles, Mile{ 0.062137119223733391 });
        CHECK_ALMOST_EQUAL(meters, Meter{ 100.0 });
    }

    TEST_CASE("time conversion", "[quantities][conversions]")
    {
        Second seconds{ 3600.0 };

        Minute minutes{ seconds };
        Hour hours{ minutes };
        
        seconds = Second{ hours };

        CHECK_ALMOST_EQUAL(minutes, Minute{60.0});
        CHECK_ALMOST_EQUAL(hours, Hour{1.0});
        CHECK_ALMOST_EQUAL(seconds, Second{3600.0});
    }

    TEST_CASE("mass conversion", "[quantities][conversions]")
    {
        Kilogram kilograms{ 1.0 };

        Gram grams{ kilograms };
        Tonne tonnes{ grams };
        Pound pounds{ tonnes };
        Ounce ounces{ pounds };

        kilograms = Kilogram{ ounces };

        CHECK_ALMOST_EQUAL(grams, Gram{1000.0});
        CHECK_ALMOST_EQUAL(tonnes, Tonne{0.001});
        CHECK_ALMOST_EQUAL(pounds, Pound{2.2046226218487757});
        CHECK_ALMOST_EQUAL(ounces, Ounce{35.273961949580411});
        CHECK_ALMOST_EQUAL(kilograms, Kilogram{1.0});

        CHECK(16.0 == ounces.value() / pounds.value());

        pounds = Pound{ 1.0 };
        kilograms = Kilogram{ pounds };
        grams = Gram{ kilograms };
        tonnes = Tonne{ grams };
        ounces = Ounce{ tonnes };

        pounds = Pound{ ounces };

        CHECK_ALMOST_EQUAL(kilograms, Kilogram{0.45359237});
        CHECK_ALMOST_EQUAL(grams, Gram{453.59237});
        CHECK_ALMOST_EQUAL(tonnes, Tonne{0.00045359237});
        CHECK_ALMOST_EQUAL(ounces, Ounce{15.999999999999996});
        CHECK_ALMOST_EQUAL(pounds, Pound{0.99999999999999978});

        CHECK(16.0 == ounces.value() / pounds.value());
    }

    TEST_CASE("temperature conversion (quantities)", "[quantities][conversions]")
    {
        DegCelsius celsius{ 100.0 };

        DegFahrenheit fahrenheit{ celsius };
        DegRankine rankine{ fahrenheit };
        DegKelvin kelvin{ rankine };

        celsius = DegCelsius{ kelvin };

        CHECK_ALMOST_EQUAL(fahrenheit, DegFahrenheit{180.0});
        CHECK_ALMOST_EQUAL(rankine, DegRankine{180.0});
        CHECK_ALMOST_EQUAL(kelvin, DegKelvin{100.0});
        CHECK_ALMOST_EQUAL(celsius, DegCelsius{100.0});
    }

    TEST_CASE("frequency conversion", "[quantities][conversions]")
    {
        Hertz frequency{ 1000.0 };
        Vehicles_Hour veh_hr { frequency };

        frequency = Hertz{veh_hr};

        CHECK_ALMOST_EQUAL(frequency, Hertz{1000.0});
        CHECK_ALMOST_EQUAL(veh_hr, Vehicles_Hour{3600000.0});

    }

    TEST_CASE("plane angle conversion", "[quantities][conversions]")
    {
        Turn turns{ 1.0 };

        Radian radians{ turns };
        Degree degrees{ radians };
        Grad grads{ degrees };

        turns = Turn{ grads };

        CHECK_ALMOST_EQUAL(radians, Radian{ 2.0 * PI });
        CHECK_ALMOST_EQUAL(degrees, Degree{ 360.0 });
        CHECK_ALMOST_EQUAL(grads, Grad{ 400.0 });
        CHECK_ALMOST_EQUAL(turns, Turn{ 1.0 });
    }

    TEST_CASE("rotational speed conversion", "[quantities][conversions]")
    {
        RPM rpm{ 7200 };

        Degree_Sec degrees_sec{ rpm };
        Radian_Sec angularvelocity{ degrees_sec };

        rpm = RPM{ angularvelocity };

        CHECK_ALMOST_EQUAL(degrees_sec, Degree_Sec{ 120.0 * 360.0 });
        CHECK_ALMOST_EQUAL(angularvelocity, Radian_Sec{ 240.0 * PI });
        CHECK_ALMOST_EQUAL(rpm, RPM{ 7200.0 });

        Second duration{ 1.0 };
        Turn turns = rpm * Minute{ duration };
        CHECK_ALMOST_EQUAL(turns, Turn{120.0});
    }

    TEST_CASE("to_string (quantities)", "[quantities][formatting]")
    {
        Meter length{ 234.0 };

        CHECK(to_string(length) == "234.000000 m");
        CHECK(to_string(length, "%.2f [%s]") == "234.00 [m]");
    }
}
