#include "gtest/gtest.h"

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

    TEST(UNIT_OPERATORS, Additive1)
    {
        const double d1 = 123.0;
        const double d2 = 234.0;

        Meter q1{ d1 };
        Meter q2{ d2 };

        EXPECT_EQ(q1 + q2, Meter{ d1 + d2 });
        EXPECT_EQ(q2 - q1, Meter{ d2 - d1 });
        EXPECT_EQ(-q2, Meter{ -d2 });
        EXPECT_TRUE(q1 != q2);
        EXPECT_TRUE(q1 < q2);
        EXPECT_TRUE(q2 > q1);

        q2 -= q1;
        EXPECT_EQ(q2, Meter{ d2 - d1 });

        q2 += q1;
        EXPECT_EQ(q2, Meter{ d2 });
    }

    TEST(UNIT_OPERATORS, Additive2)
    {
        Meter meters{ 5.0 };
        Centimeter centimeters{ 25.0 };

        Meter m{ meters + Meter(centimeters) };
        EXPECT_TRUE(Meter{ 5.25 } == m);

        Centimeter cm{ Centimeter(meters) + centimeters };
        EXPECT_TRUE(Centimeter{ 525.0 } == cm);

        m = meters - Meter(centimeters);
        EXPECT_TRUE(Meter{ 4.75 } == m);

        cm = Centimeter(meters) - centimeters;
        EXPECT_TRUE(Centimeter{ 475.0 } == cm);
    }

    TEST(UNIT_OPERATORS, IncrementDecrement)
    {
        Meter initial{ 5.0 };
        Meter post = initial++;
        EXPECT_TRUE(initial == Meter{ 6.0 });

        Meter pre = --initial;
        EXPECT_TRUE((post == pre) && (pre == initial) && (initial == Meter{ 5.0 }));
    }

    TEST(UNIT_OPERATORS, Multiplicative)
    {
        MPH speed{ 100.0 }; // 100 mph
        Minute duration{ 30.0 };    // 30 minutes

        Mile miles = speed * Hour{ duration };
        EXPECT_TRUE(Mile{50.0} == miles);

        Kilometer kilometers{ miles };
        EXPECT_TRUE(Kilometer{80.467199999999991} == kilometers);

        speed = miles / Hour{ duration };
        EXPECT_TRUE(MPH{100.0} == speed);
    }

    TEST(UNIT_OPERATORS, CompoundAssignment)
    {
        Meter meters{ 5.0 };  // 5 meters

        meters *= 2.0;  // 10 meters
        EXPECT_TRUE(Meter{10.0} == meters);

        meters /= 5.0;  // 2 meters
        EXPECT_TRUE(Meter{2.0} == meters);

        meters += Meter{ Centimeter{75.0}};
        EXPECT_TRUE(Meter{2.75} == meters);

        meters -= Meter{ Centimeter{25.0}};
        EXPECT_TRUE(Meter{2.50} == meters);
    }

    TEST(UNIT_OPERATORS, LengthComparison)
    {
        Meter x{ 123.0 };
        Meter y{ 345.0 };

        EXPECT_TRUE(x != y);
        EXPECT_TRUE(x < y);
        EXPECT_TRUE(x + y == Meter{ 123.0 + 345.0 });
        EXPECT_TRUE(y - x == Meter{ 345.0 - 123.0 });
        EXPECT_EQ(y / x, 345.0 / 123.0);
    }

    TEST(UNIT_OPERATORS, MassComparison)
    {
        {
            // 5.0 tonnes == 5000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 5000.0 };
            EXPECT_NE(tonnes.value(), kilograms.value());
            EXPECT_TRUE(tonnes == Tonne{kilograms});
        }
        {
            // 5 pounds < 5 kilograms
            Pound pounds{ 5.0 };
            Kilogram kilograms{ 5.0 };
            EXPECT_EQ(kilograms.value(), pounds.value());
            EXPECT_TRUE(pounds < Pound{kilograms});
            EXPECT_TRUE(kilograms >= Kilogram{pounds});
        }
        {
            // 5 tonnes > 3000 kilograms
            Tonne tonnes{ 5.0 };
            Kilogram kilograms{ 3000.0 };
            EXPECT_TRUE(tonnes.value() < kilograms.value());
            EXPECT_TRUE(tonnes != Tonne{kilograms});
            EXPECT_TRUE(Kilogram{tonnes} > kilograms);
            EXPECT_TRUE(Tonne{kilograms} <= tonnes);
        }
    }

    TEST(UNIT_OPERATORS, TorqueEnergy)
    {
        // Joule and NewtonMeter belong to disjoint families
        EXPECT_NE(Joule::family, NewtonMeter::family);

        // Besides, they are not very different (except the operators used):
        Meter distance{ 10.0 };
        Newton force{ 100.0 };

        Joule energy = force * distance;
        EXPECT_EQ(Joule{ 1000.0 }, energy);

        NewtonMeter torque = force ^ distance;
        EXPECT_EQ(NewtonMeter{ 1000.0 }, torque);

        Meter distanceFromEnergy = energy / force;
        Meter distanceFromTorque = torque / force;
        EXPECT_EQ(distanceFromEnergy, distanceFromTorque);

        Newton forceFromEnergy = energy / distance;
        Newton forceFromTorque = torque / distance;
        EXPECT_EQ(forceFromEnergy, forceFromTorque);
    }

    TEST(UNIT_CONVERSIONS, FloatingPointInaccuracy)
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
        EXPECT_NE(expected, calculated);
        // but...:
        EXPECT_EQ(Meter{ 99.999999999999986 }, calculated);
    }

    TEST(UNIT_CONVERSIONS, Length)
    {
        Meter meters{ 100.0 };

        Centimeter centimeters{ meters };
        Millimeter millimeters{ centimeters };
        Inch inches{ millimeters };
        Foot feet{ inches };
        Yard yards{ feet };
        Mile miles{ yards };
        meters = Meter(miles);

        EXPECT_EQ(Centimeter{ 10000.0 }, centimeters);
        EXPECT_EQ(Millimeter{ 100000.0 }, millimeters);
        EXPECT_EQ(Inch{ 3937.0078740157478 }, inches);
        EXPECT_EQ(Foot{ 328.08398950131232 }, feet);
        EXPECT_EQ(Yard{ 109.36132983377077 }, yards);
        EXPECT_EQ(Mile{ 0.062137119223733397 }, miles);
        EXPECT_EQ(Meter{ 99.999999999999986 }, meters);
    }

    TEST(UNIT_CONVERSIONS, Time)
    {
        Second seconds{ 3600.0 };

        Minute minutes{ seconds };
        Hour hours{ minutes };
        
        seconds = Second{ hours };

        EXPECT_EQ(Minute{60.0}, minutes);
        EXPECT_EQ(Hour{1.0}, hours);
        EXPECT_EQ(Second{3600.0}, seconds);
    }

    TEST(UNIT_CONVERSIONS, Mass)
    {
        Kilogram kilograms{ 1.0 };

        Gram grams{ kilograms };
        Tonne tonnes{ grams };
        Pound pounds{ tonnes };
        Ounce ounces{ pounds };

        kilograms = Kilogram{ ounces };

        EXPECT_EQ(Gram{1000.0}, grams);
        EXPECT_EQ(Tonne{0.001}, tonnes);
        EXPECT_EQ(Pound{2.2046226218487757}, pounds);
        EXPECT_EQ(Ounce{35.273961949580411}, ounces);
        EXPECT_EQ(Kilogram{1.0}, kilograms);

        EXPECT_EQ(16.0, ounces.value() / pounds.value());

        pounds = Pound{ 1.0 };
        kilograms = Kilogram{ pounds };
        grams = Gram{ kilograms };
        tonnes = Tonne{ grams };
        ounces = Ounce{ tonnes };

        pounds = Pound{ ounces };

        EXPECT_EQ(Kilogram{0.45359237}, kilograms);
        EXPECT_EQ(Gram{453.59237}, grams);
        EXPECT_EQ(Tonne{0.00045359237}, tonnes);
        EXPECT_EQ(Ounce{15.999999999999996}, ounces);
        EXPECT_EQ(Pound{0.99999999999999978}, pounds);

        EXPECT_EQ(16.0, ounces.value() / pounds.value());
    }

    TEST(UNIT_CONVERSIONS, Temperature)
    {
        DegCelsius celsius{ 100.0 };

        DegFahrenheit fahrenheit{ celsius };
        DegRankine rankine{ fahrenheit };
        DegKelvin kelvin{ rankine };

        celsius = DegCelsius{ kelvin };

        EXPECT_EQ(DegFahrenheit{180.0}, fahrenheit);
        EXPECT_EQ(DegRankine{180.0}, rankine);
        EXPECT_EQ(DegKelvin{100.0}, kelvin);
        EXPECT_EQ(DegCelsius{100.0}, celsius);
    }

    TEST(UNIT_CONVERSIONS, Frequency)
    {
        Hertz frequency{ 1000.0 };
        Vehicles_Hour veh_hr { frequency };

        frequency = Hertz{veh_hr};

        EXPECT_EQ(Hertz{1000.0}, frequency);
        EXPECT_EQ(Vehicles_Hour{3600000.0}, veh_hr);
    }

    TEST(UNIT_CONVERSIONS, PlaneAngle)
    {
        Turn turns{ 1.0 };

        Radian radians{ turns };
        Degree degrees{ radians };
        Grad grads{ degrees };

        turns = Turn{ grads };

        EXPECT_EQ(Radian{ 2.0 * PI }, radians);
        EXPECT_EQ(Degree{ 360.0 }, degrees);
        EXPECT_EQ(Grad{ 400.0 }, grads);
        EXPECT_EQ(Turn{ 1.0 }, turns);
    }

    TEST(UNIT_CONVERSIONS, RotationalSpeed)
    {
        RPM rpm{ 7200 };

        Degree_Sec degrees_sec{ rpm };
        Radian_Sec angularvelocity{ degrees_sec };

        rpm = RPM{ angularvelocity };

        EXPECT_EQ(Degree_Sec{ 120.0 * 360.0 }, degrees_sec);
        EXPECT_EQ(Radian_Sec{ 240.0 * PI }, angularvelocity - Radian_Sec{ 1.1368683772161603E-13 });
        EXPECT_EQ(RPM{ 7200.0 }, rpm - RPM{ 0.000000000001 });

        Second duration{ 1.0 };
        Turn turns = rpm * Minute{ duration };
        EXPECT_EQ(Turn{120.0}, turns - Turn{0.00000000000001});
    }

    TEST(UNIT_FORMATTING, ToString)
    {
        Meter length{ 234.0 };

        EXPECT_EQ(to_string(length), "234.000000 m");
        EXPECT_EQ(to_string(length, "%.2f [%s]"), "234.00 [m]");
    }
}
