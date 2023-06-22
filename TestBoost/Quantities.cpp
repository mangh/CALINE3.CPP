#define BOOST_TEST_MODULE Units of Measurement
#include <boost/test/unit_test.hpp>

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

    BOOST_AUTO_TEST_SUITE(UNIT_OPERATORS)

    BOOST_AUTO_TEST_CASE(Additive1)
    {
        const double d1 = 123.0;
        const double d2 = 234.0;

        Meter q1{ d1 };
        Meter q2{ d2 };

        BOOST_CHECK_EQUAL(q1 + q2, Meter{ d1 + d2 });
        BOOST_CHECK_EQUAL(q2 - q1, Meter{ d2 - d1 });
        BOOST_CHECK_EQUAL(-q2, Meter{ -d2 });
        BOOST_TEST(q1 != q2);
        BOOST_TEST(q1 < q2);
        BOOST_TEST(q2 > q1);

        q2 -= q1;
        BOOST_CHECK_EQUAL(q2, Meter{ d2 - d1 });

        q2 += q1;
        BOOST_CHECK_EQUAL(q2, Meter{ d2 });
    }

    BOOST_AUTO_TEST_CASE(Additive2)
    {
        Meter meters{ 5.0 };
        Centimeter centimeters{ 25.0 };

        Meter m{ meters + Meter(centimeters) };
        BOOST_TEST(Meter{ 5.25 } == m);

        Centimeter cm{ Centimeter(meters) + centimeters };
        BOOST_TEST(Centimeter{ 525.0 } == cm);

        m = meters - Meter(centimeters);
        BOOST_TEST(Meter{ 4.75 } == m);

        cm = Centimeter(meters) - centimeters;
        BOOST_TEST(Centimeter{ 475.0 } == cm);
    }

    BOOST_AUTO_TEST_CASE(IncrementDecrement)
    {
        Meter initial{ 5.0 };
        Meter post = initial++;
        BOOST_TEST(initial == Meter{ 6.0 });

        Meter pre = --initial;
        BOOST_TEST(((post == pre) && (pre == initial) && (initial == Meter{ 5.0 })));
    }

    BOOST_AUTO_TEST_CASE(Multiplicative)
    {
        MPH speed{ 100.0 }; // 100 mph
        Minute duration{ 30.0 };    // 30 minutes

        Mile miles = speed * Hour{ duration };
        BOOST_TEST(Mile{50.0} == miles);

        Kilometer kilometers{ miles };
        BOOST_TEST(Kilometer{80.467199999999991} == kilometers);

        speed = miles / Hour{ duration };
        BOOST_TEST(MPH{100.0} == speed);
    }

    BOOST_AUTO_TEST_CASE(CompoundAssignment)
    {
        Meter meters{ 5.0 };  // 5 meters

        meters *= 2.0;  // 10 meters
        BOOST_TEST(Meter{10.0} == meters);

        meters /= 5.0;  // 2 meters
        BOOST_TEST(Meter{2.0} == meters);

        meters += Meter{ Centimeter{75.0}};
        BOOST_TEST(Meter{2.75} == meters);

        meters -= Meter{ Centimeter{25.0}};
        BOOST_TEST(Meter{2.50} == meters);
    }

    BOOST_AUTO_TEST_CASE(LengthComparison)
    {
        auto x = Meter{ 123.0 };
        auto y = Meter{ 345.0 };

        BOOST_TEST(x != y);
        BOOST_TEST(x < y);
        BOOST_TEST(x + y == Meter{ 123.0 + 345.0 });
        BOOST_TEST(y - x == Meter{ 345.0 - 123.0 });
        BOOST_CHECK_EQUAL(y / x, 345.0 / 123.0);
    }

    BOOST_AUTO_TEST_CASE(MassComparison)
    {
        {
            // 5.0 tonnes == 5000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 5000.0 };
            BOOST_CHECK_NE(tonnes.value(), kilograms.value());
            BOOST_TEST(tonnes == Tonne{kilograms});
        }
        {
            // 5 pounds < 5 kilograms
            Pound pounds{ 5.0 };
            Kilogram kilograms{ 5.0 };
            BOOST_CHECK_EQUAL(kilograms.value(), pounds.value());
            BOOST_TEST(pounds < Pound{kilograms});
            BOOST_TEST(kilograms >= Kilogram{pounds});
        }
        {
            // 5 tonnes > 3000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 3000.0 };
            BOOST_TEST(tonnes.value() < kilograms.value());
            BOOST_TEST(tonnes != Tonne{kilograms});
            BOOST_TEST(Kilogram{tonnes} > kilograms);
            BOOST_TEST(Tonne{kilograms} <= tonnes);
        }
    }

    BOOST_AUTO_TEST_CASE(TorqueEnergy)
    {
        // Joule and NewtonMeter belong to disjoint families
        BOOST_CHECK_NE(Joule::family, NewtonMeter::family);

        // Besides, they are not very different (except the operators used):
        Meter distance{ 10.0 };
        Newton force{ 100.0 };

        Joule energy = force * distance;
        BOOST_CHECK_EQUAL(Joule{ 1000.0 }, energy);

        NewtonMeter torque = force ^ distance;
        BOOST_CHECK_EQUAL(NewtonMeter{ 1000.0 }, torque);

        Meter distanceFromEnergy = energy / force;
        Meter distanceFromTorque = torque / force;
        BOOST_CHECK_EQUAL(distanceFromEnergy, distanceFromTorque);

        Newton forceFromEnergy = energy / distance;
        Newton forceFromTorque = torque / distance;
        BOOST_CHECK_EQUAL(forceFromEnergy, forceFromTorque);
    }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(UNIT_CONVERSIONS)

    BOOST_AUTO_TEST_CASE(FloatingPointInaccuracy)
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
        BOOST_CHECK_NE(expected, calculated);
        // but...:
        BOOST_CHECK_EQUAL(Meter{ 99.999999999999986 }, calculated);
    }

    BOOST_AUTO_TEST_CASE(Lengths)
    {
        Meter meters{ 100.0 };

        Centimeter centimeters{ meters };
        Millimeter millimeters{ centimeters };
        Inch inches{ millimeters };
        Foot feet{ inches };
        Yard yards{ feet };
        Mile miles{ yards };
        meters = Meter(miles);

        BOOST_CHECK_EQUAL(Centimeter{ 10000.0 }, centimeters);
        BOOST_CHECK_EQUAL(Millimeter{ 100000.0 }, millimeters);
        BOOST_CHECK_EQUAL(Inch{ 3937.0078740157478 }, inches);
        BOOST_CHECK_EQUAL(Foot{ 328.08398950131232 }, feet);
        BOOST_CHECK_EQUAL(Yard{ 109.36132983377077 }, yards);
        BOOST_CHECK_EQUAL(Mile{ 0.062137119223733397 }, miles);
        BOOST_CHECK_EQUAL(Meter{ 99.999999999999986 }, meters);
    }

    BOOST_AUTO_TEST_CASE(Time)
    {
        Second seconds{ 3600.0 };

        Minute minutes{ seconds };
        Hour hours{ minutes };
        
        seconds = Second{ hours };

        BOOST_CHECK_EQUAL(Minute{60.0}, minutes);
        BOOST_CHECK_EQUAL(Hour{1.0}, hours);
        BOOST_CHECK_EQUAL(Second{3600.0}, seconds);
    }

    BOOST_AUTO_TEST_CASE(Mass)
    {
        Kilogram kilograms{ 1.0 };

        Gram grams{ kilograms };
        Tonne tonnes{ grams };
        Pound pounds{ tonnes };
        Ounce ounces{ pounds };

        kilograms = Kilogram{ ounces };

        BOOST_CHECK_EQUAL(Gram{1000.0}, grams);
        BOOST_CHECK_EQUAL(Tonne{0.001}, tonnes);
        BOOST_CHECK_EQUAL(Pound{2.2046226218487757}, pounds);
        BOOST_CHECK_EQUAL(Ounce{35.273961949580411}, ounces);
        BOOST_CHECK_EQUAL(Kilogram{1.0}, kilograms);

        BOOST_CHECK_EQUAL(16.0, ounces.value() / pounds.value());

        pounds = Pound{ 1.0 };
        kilograms = Kilogram{ pounds };
        grams = Gram{ kilograms };
        tonnes = Tonne{ grams };
        ounces = Ounce{ tonnes };

        pounds = Pound{ ounces };

        BOOST_CHECK_EQUAL(Kilogram{0.45359237}, kilograms);
        BOOST_CHECK_EQUAL(Gram{453.59237}, grams);
        BOOST_CHECK_EQUAL(Tonne{0.00045359237}, tonnes);
        BOOST_CHECK_EQUAL(Ounce{15.999999999999996}, ounces);
        BOOST_CHECK_EQUAL(Pound{0.99999999999999978}, pounds);

        BOOST_CHECK_EQUAL(16.0, ounces.value() / pounds.value());
    }

    BOOST_AUTO_TEST_CASE(Temperature)
    {
        DegCelsius celsius{ 100.0 };

        DegFahrenheit fahrenheit{ celsius };
        DegRankine rankine{ fahrenheit };
        DegKelvin kelvin{ rankine };

        celsius = DegCelsius{ kelvin };

        BOOST_CHECK_EQUAL(DegFahrenheit{180.0}, fahrenheit);
        BOOST_CHECK_EQUAL(DegRankine{180.0}, rankine);
        BOOST_CHECK_EQUAL(DegKelvin{100.0}, kelvin);
        BOOST_CHECK_EQUAL(DegCelsius{100.0}, celsius);
    }

    BOOST_AUTO_TEST_CASE(Frequency)
    {
        Hertz frequency{ 1000.0 };
        Vehicles_Hour veh_hr { frequency };

        frequency = Hertz{veh_hr};

        BOOST_CHECK_EQUAL(Hertz{1000.0}, frequency);
        BOOST_CHECK_EQUAL(Vehicles_Hour{3600000.0}, veh_hr);
    }

    BOOST_AUTO_TEST_CASE(PlaneAngle)
    {
        Turn turns{ 1.0 };

        Radian radians{ turns };
        Degree degrees{ radians };
        Grad grads{ degrees };

        turns = Turn{ grads };

        BOOST_CHECK_EQUAL(Radian{ 2.0 * PI }, radians);
        BOOST_CHECK_EQUAL(Degree{ 360.0 }, degrees);
        BOOST_CHECK_EQUAL(Grad{ 400.0 }, grads);
        BOOST_CHECK_EQUAL(Turn{ 1.0 }, turns);
    }

    BOOST_AUTO_TEST_CASE(RotationalSpeed)
    {
        RPM rpm{ 7200 };

        Degree_Sec degrees_sec{ rpm };
        Radian_Sec angularvelocity{ degrees_sec };

        rpm = RPM{ angularvelocity };

        BOOST_CHECK_EQUAL(Degree_Sec{ 120.0 * 360.0 }, degrees_sec);
        BOOST_CHECK_EQUAL(Radian_Sec{ 240.0 * PI }, angularvelocity - Radian_Sec{ 1.1368683772161603E-13 });
        BOOST_CHECK_EQUAL(RPM{ 7200.0 }, rpm - RPM{ 0.000000000001 });

        Second duration{ 1.0 };
        Turn turns = rpm * Minute{ duration };
        BOOST_CHECK_EQUAL(Turn{120.0}, turns - Turn{0.00000000000001});
    }
    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(UNIT_FORMATTING)

    BOOST_AUTO_TEST_CASE(Formatting)
    {
        Meter length{ 234.0 };

        BOOST_CHECK_EQUAL(to_string(length), "234.000000 m");
        BOOST_CHECK_EQUAL(to_string(length, "%.2f [%s]"), "234.00 [m]");
    }

    BOOST_AUTO_TEST_SUITE_END()
}
