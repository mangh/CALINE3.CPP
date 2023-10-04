#include <catch2/catch_test_macros.hpp>

// Temperature
#include "Celsius.h"
#include "Fahrenheit.h"
#include "Kelvin.h"
#include "Rankine.h"

namespace CALINE3::Metrology
{
    TEST_CASE("temperature conversion (levels)", "[levels][conversions]")
    {
        Celsius celsius{ 100.0 };

        Fahrenheit fahrenheit { celsius };
        Rankine rankine { fahrenheit };
        Kelvin kelvin { rankine };

        celsius = Celsius(kelvin);

        REQUIRE(Fahrenheit{212.0} == fahrenheit);
        REQUIRE(Rankine{671.67} == rankine);
        REQUIRE(Kelvin{373.15} == kelvin);
        REQUIRE(Celsius{100.0} == celsius);
    }

    TEST_CASE("floating point inaccuracy (levels)", "[levels][conversions]")
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
        REQUIRE(expected != calculated);
        // but...:
        REQUIRE(Fahrenheit{123.45000000000005} == calculated);
    }

    TEST_CASE("temperature comparison (levels)", "[levels][operators]")
    {
        {
            // 100 C == 671.67 R
            Rankine rankine { 671.67 };
            Celsius celsius { 100.0 };
            REQUIRE(rankine == Rankine(celsius));
            REQUIRE(Celsius(rankine) == celsius);
        }
        {
            // 100 C > 100 R
            Rankine rankine { 100.0 };
            Celsius celsius { 100.0 };
            REQUIRE(rankine < Rankine(celsius));
            REQUIRE(celsius >= Celsius(rankine));
        }
        {
            Celsius celsius { 100.0 };
            REQUIRE(celsius == Celsius(Kelvin{ 100.0 + 273.15 }));
            REQUIRE(Kelvin(celsius) > Kelvin{ 100.0 });
            REQUIRE(Kelvin{ 100.0 } < Kelvin(celsius));
            REQUIRE(Kelvin{ 100.0 } <= Kelvin(celsius));
        }
    }

    TEST_CASE("addition/subtraction (levels)", "[levels][operators]")
    {
        DegKelvin kelvins{ 5.0 };
        DegCelsius growth {kelvins };

        REQUIRE(Celsius{ 100.0 } + growth == Celsius{ 105.0 });
        REQUIRE(Celsius{ 105.0 } - growth == Celsius{ 100.0 });
        REQUIRE(Celsius{ 105.0 } - Celsius{ 100.0 } == growth);

        DegRankine rankines{ 9.0 };
        growth = DegCelsius(rankines);

        REQUIRE(Celsius{ 100.0 } + growth == Celsius{ 105.0 });
        REQUIRE(growth + Celsius{ 100.0 } == Celsius(Fahrenheit{ 221.0}));
        REQUIRE(Celsius{ 100.0 } + growth == growth + Celsius{ 100.0 });
        REQUIRE(Celsius{ 105.0 } - growth == Celsius{ 100.0 });
        REQUIRE(Celsius{ 105.0 } - Celsius{ 100.0 } == growth);
    }

    TEST_CASE("increment/decrement (levels)", "[levels][operators]")
    {
        Fahrenheit temperature{ 5.0 };
        auto post = temperature++;
        REQUIRE(temperature == Fahrenheit{ 6.0 });
        auto pre = --temperature;
        REQUIRE(((post == pre) && (pre == temperature) && (temperature == (Fahrenheit)5.0)));
    }

    TEST_CASE("to_string (levels)", "[levels][formatting]")
    {
        Fahrenheit temperature{ 234.0 };

        REQUIRE(to_string(temperature) == "234.000000 deg.F");
        REQUIRE(to_string(temperature, "%.2f [%s]") == "234.00 [deg.F]");
    }
}

