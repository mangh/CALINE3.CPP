#include <catch2/catch_test_macros.hpp>

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


namespace CALINE3::Metrology
{
    constexpr double PI = 3.141592653589793238462643383279502884L;

    TEST_CASE("addition/subtraction (quantities in same unit)", "[quantities][operators]")
    {
        const double d1 = 123.0;
        const double d2 = 234.0;

        Meter q1{ d1 };
        Meter q2{ d2 };

        REQUIRE(q1 + q2 == Meter{ d1 + d2 });
        REQUIRE(q2 - q1 == Meter{ d2 - d1 });
        REQUIRE(-q2 == Meter{ -d2 });
        REQUIRE(q1 != q2);
        REQUIRE(q1 < q2);
        REQUIRE(q2 > q1);

        q2 -= q1;
        REQUIRE(q2 == Meter{ d2 - d1 });

        q2 += q1;
        REQUIRE(q2 == Meter{ d2 });
    }

    TEST_CASE("addition/subtraction (quantities in different units)", "[quantities][operators]")
    {
        Meter meters{ 5.0 };
        Centimeter centimeters{ 25.0 };

        Meter m{ meters + Meter(centimeters) };
        REQUIRE(Meter{ 5.25 } == m);

        Centimeter cm{ Centimeter(meters) + centimeters };
        REQUIRE(Centimeter{ 525.0 } == cm);

        m = meters - Meter(centimeters);
        REQUIRE(Meter{ 4.75 } == m);

        cm = Centimeter(meters) - centimeters;
        REQUIRE(Centimeter{ 475.0 } == cm);
    }

    TEST_CASE("increment/decrement (quantities)", "[quantities][operators]")
    {
        Meter initial{ 5.0 };
        Meter post = initial++;
        REQUIRE(initial == Meter{ 6.0 });

        Meter pre = --initial;
        REQUIRE(((post == pre) && (pre == initial) && (initial == Meter{ 5.0 })));
    }

    TEST_CASE("multiplication (quantities)", "[quantities][operators]")
    {
        MPH speed{ 100.0 }; // 100 mph
        Minute duration{ 30.0 };    // 30 minutes

        Mile miles = speed * Hour{ duration };
        REQUIRE(Mile{50.0} == miles);

        Kilometer kilometers{ miles };
        REQUIRE(Kilometer{80.467199999999991} == kilometers);

        speed = miles / Hour{ duration };
        REQUIRE(MPH{100.0} == speed);
    }

    TEST_CASE("compound assignment (quantities)", "[quantities][operators]")
    {
        Meter meters{ 5.0 };  // 5 meters

        meters *= 2.0;  // 10 meters
        REQUIRE(Meter{10.0} == meters);

        meters /= 5.0;  // 2 meters
        REQUIRE(Meter{2.0} == meters);

        meters += Meter{ Centimeter{75.0}};
        REQUIRE(Meter{2.75} == meters);

        meters -= Meter{ Centimeter{25.0}};
        REQUIRE(Meter{2.50} == meters);
    }

    TEST_CASE("length comparison", "[quantities][operators]")
    {
        Meter x{ 123.0 };
        Meter y{ 345.0 };

        REQUIRE(x != y);
        REQUIRE(x < y);
        REQUIRE(x + y == Meter{ 123.0 + 345.0 });
        REQUIRE(y - x == Meter{ 345.0 - 123.0 });
        REQUIRE(y / x == 345.0 / 123.0);
    }

    TEST_CASE("mass comparison", "[quantities][operators]")
    {
        {
            // 5.0 tonnes == 5000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 5000.0 };
            REQUIRE(tonnes.value() != kilograms.value());
            REQUIRE(tonnes == Tonne{kilograms});
        }
        {
            // 5 pounds < 5 kilograms
            Pound pounds{ 5.0 };
            Kilogram kilograms{ 5.0 };
            REQUIRE(kilograms.value() == pounds.value());
            REQUIRE(pounds < Pound{kilograms});
            REQUIRE(kilograms >= Kilogram{pounds});
        }
        {
            // 5 tonnes > 3000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 3000.0 };
            REQUIRE(tonnes.value() < kilograms.value());
            REQUIRE(tonnes != Tonne{kilograms});
            REQUIRE(Kilogram{tonnes} > kilograms);
            REQUIRE(Tonne{kilograms} <= tonnes);
        }
    }

    TEST_CASE("torque/energy", "[quantities][operators]")
    {
        // Joule and NewtonMeter belong to disjoint families
        REQUIRE(Joule::family != NewtonMeter::family);

        // Besides, they are not very different (except the operators used):
        Meter distance{ 10.0 };
        Newton force{ 100.0 };

        Joule energy = force * distance;
        REQUIRE(Joule{ 1000.0 } == energy);

        NewtonMeter torque = force ^ distance;
        REQUIRE(NewtonMeter{ 1000.0 } == torque);

        Meter distanceFromEnergy = energy / force;
        Meter distanceFromTorque = torque / force;
        REQUIRE(distanceFromEnergy == distanceFromTorque);

        Newton forceFromEnergy = energy / distance;
        Newton forceFromTorque = torque / distance;
        REQUIRE(forceFromEnergy == forceFromTorque);
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
        REQUIRE(expected != calculated);
        // but...:
        REQUIRE(Meter{ 99.999999999999986 } == calculated);
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

        REQUIRE(Centimeter{ 10000.0 } == centimeters);
        REQUIRE(Millimeter{ 100000.0 } == millimeters);
        REQUIRE(Inch{ 3937.0078740157478 } == inches);
        REQUIRE(Foot{ 328.08398950131232 } == feet);
        REQUIRE(Yard{ 109.36132983377077 } == yards);
        REQUIRE(Mile{ 0.062137119223733397 } == miles);
        REQUIRE(Meter{ 99.999999999999986 } == meters);
    }

    TEST_CASE("time conversion", "[quantities][conversions]")
    {
        Second seconds{ 3600.0 };

        Minute minutes{ seconds };
        Hour hours{ minutes };
        
        seconds = Second{ hours };

        REQUIRE(Minute{60.0} == minutes);
        REQUIRE(Hour{1.0} == hours);
        REQUIRE(Second{3600.0} == seconds);
    }

    TEST_CASE("mass conversion", "[quantities][conversions]")
    {
        Kilogram kilograms{ 1.0 };

        Gram grams{ kilograms };
        Tonne tonnes{ grams };
        Pound pounds{ tonnes };
        Ounce ounces{ pounds };

        kilograms = Kilogram{ ounces };

        REQUIRE(Gram{1000.0} == grams);
        REQUIRE(Tonne{0.001} == tonnes);
        REQUIRE(Pound{2.2046226218487757} == pounds);
        REQUIRE(Ounce{35.273961949580411} == ounces);
        REQUIRE(Kilogram{1.0} == kilograms);

        REQUIRE(16.0 == ounces.value() / pounds.value());

        pounds = Pound{ 1.0 };
        kilograms = Kilogram{ pounds };
        grams = Gram{ kilograms };
        tonnes = Tonne{ grams };
        ounces = Ounce{ tonnes };

        pounds = Pound{ ounces };

        REQUIRE(Kilogram{0.45359237} == kilograms);
        REQUIRE(Gram{453.59237} == grams);
        REQUIRE(Tonne{0.00045359237} == tonnes);
        REQUIRE(Ounce{15.999999999999996} == ounces);
        REQUIRE(Pound{0.99999999999999978} == pounds);

        REQUIRE(16.0 == ounces.value() / pounds.value());
    }

    TEST_CASE("temperature conversion (quantities)", "[quantities][conversions]")
    {
        DegCelsius celsius{ 100.0 };

        DegFahrenheit fahrenheit{ celsius };
        DegRankine rankine{ fahrenheit };
        DegKelvin kelvin{ rankine };

        celsius = DegCelsius{ kelvin };

        REQUIRE(DegFahrenheit{180.0} == fahrenheit);
        REQUIRE(DegRankine{180.0} == rankine);
        REQUIRE(DegKelvin{100.0} == kelvin);
        REQUIRE(DegCelsius{100.0} == celsius);
    }

    TEST_CASE("frequency conversion", "[quantities][conversions]")
    {
        Hertz frequency{ 1000.0 };
        Vehicles_Hour veh_hr { frequency };

        frequency = Hertz{veh_hr};

        REQUIRE(Hertz{1000.0} == frequency);
        REQUIRE(Vehicles_Hour{3600000.0} == veh_hr);
    }

    TEST_CASE("plane angle conversion", "[quantities][conversions]")
    {
        Turn turns{ 1.0 };

        Radian radians{ turns };
        Degree degrees{ radians };
        Grad grads{ degrees };

        turns = Turn{ grads };

        REQUIRE(Radian{ 2.0 * PI } == radians);
        REQUIRE(Degree{ 360.0 } == degrees);
        REQUIRE(Grad{ 400.0 } == grads);
        REQUIRE(Turn{ 1.0 } == turns);
    }

    TEST_CASE("rotational speed conversion", "[quantities][conversions]")
    {
        RPM rpm{ 7200 };

        Degree_Sec degrees_sec{ rpm };
        Radian_Sec angularvelocity{ degrees_sec };

        rpm = RPM{ angularvelocity };

        REQUIRE(Degree_Sec{ 120.0 * 360.0 } == degrees_sec);
        REQUIRE(Radian_Sec{ 240.0 * PI } == angularvelocity - Radian_Sec{ 1.1368683772161603E-13 });
        REQUIRE(RPM{ 7200.0 } == rpm - RPM{ 0.000000000001 });

        Second duration{ 1.0 };
        Turn turns = rpm * Minute{ duration };
        REQUIRE(Turn{120.0} == turns - Turn{0.00000000000001});
    }

    TEST_CASE("to_string (quantities)", "[quantities][formatting]")
    {
        Meter length{ 234.0 };

        REQUIRE(to_string(length) == "234.000000 m");
        REQUIRE(to_string(length, "%.2f [%s]") == "234.00 [m]");
    }
}

