#include "gtest/gtest.h"

// Temperature
#include "Celsius.h"
#include "Fahrenheit.h"
#include "Kelvin.h"
#include "Rankine.h"

namespace CALINE3::Metrology
{
    TEST(LEVEL_CONVERSIONS, Temperatures)
    {
        Celsius celsius{ 100.0 };
        Fahrenheit fahrenheit { celsius };
        Rankine rankine { fahrenheit };
        Kelvin kelvin { rankine };
        celsius = Celsius(kelvin);

        EXPECT_EQ(Fahrenheit{212.0}, fahrenheit);
        EXPECT_EQ(Rankine{671.67}, rankine);
        EXPECT_EQ(Kelvin{373.15}, kelvin);
        EXPECT_EQ(Celsius{100.0}, celsius);
    }

    TEST(LEVEL_CONVERSIONS, FloatingPointInaccuracy)
    {
        Fahrenheit expected{ 123.45 };
        
        Fahrenheit calculated{
            Kelvin{
                Rankine{
                    Celsius{ expected }
                }
            }
        };

        // Unfortunately, due to the floating-point quirks ...:
        EXPECT_NE(expected, calculated);
        // but...:
        EXPECT_EQ(Fahrenheit{123.45000000000005}, calculated);
    }

    TEST(LEVEL_OPERATORS, Comparison)
    {
        {
            // 100 C == 671.67 R
            Rankine rankine { 671.67 };
            Celsius celsius { 100.0 };
            EXPECT_TRUE(rankine == Rankine(celsius));
            EXPECT_TRUE(Celsius(rankine) == celsius);
        }
        {
            // 100 C > 100 R
            Rankine rankine { 100.0 };
            Celsius celsius { 100.0 };
            EXPECT_TRUE(rankine < Rankine(celsius));
            EXPECT_TRUE(celsius >= Celsius(rankine));
        }
        {
            Celsius celsius { 100.0 };
            EXPECT_TRUE(celsius == Celsius(Kelvin{ 100.0 + 273.15 }));
            EXPECT_TRUE(Kelvin(celsius) > Kelvin{ 100.0 });
            EXPECT_TRUE(Kelvin{ 100.0 } < Kelvin(celsius));
            EXPECT_TRUE(Kelvin{ 100.0 } <= Kelvin(celsius));
        }
    }

    TEST(LEVEL_OPERATORS, Additive)
    {
        DegKelvin kelvins{ 5.0 };
        DegCelsius growth {kelvins };

        EXPECT_TRUE(Celsius{ 100.0 } + growth == Celsius{ 105.0 });
        EXPECT_TRUE(Celsius{ 105.0 } - growth == Celsius{ 100.0 });
        EXPECT_TRUE(Celsius{ 105.0 } - Celsius{ 100.0 } == growth);

        DegRankine rankines{ 9.0 };
        growth = DegCelsius(rankines);

        EXPECT_TRUE(Celsius{ 100.0 } + growth == Celsius{ 105.0 });
        EXPECT_TRUE(growth + Celsius{ 100.0 } == Celsius(Fahrenheit{ 221.0}));
        EXPECT_TRUE(Celsius{ 100.0 } + growth == growth + Celsius{ 100.0 });
        EXPECT_TRUE(Celsius{ 105.0 } - growth == Celsius{ 100.0 });
        EXPECT_TRUE(Celsius{ 105.0 } - Celsius{ 100.0 } == growth);
    }

    TEST(LEVEL_OPERATORS, IncrementDecrement)
    {
        Fahrenheit temperature{ 5.0 };
        auto post = temperature++;
        EXPECT_TRUE(temperature == Fahrenheit{ 6.0 });
        auto pre = --temperature;
        EXPECT_TRUE((post == pre) && (pre == temperature) && (temperature == (Fahrenheit)5.0));
    }

    TEST(LEVEL_FORMATTING, ToString)
    {
        Fahrenheit temperature{ 234.0 };

        EXPECT_EQ(to_string(temperature), "234.000000 deg.F");
        EXPECT_EQ(to_string(temperature, "%.2f [%s]"), "234.00 [deg.F]");
    }
}
