#include <boost/test/unit_test.hpp>

// Units required/suplementary:
#include "Celsius.h"
#include "Fahrenheit.h"
#include "Kelvin.h"
#include "Rankine.h"

namespace CALINE3::Metrology
{
    BOOST_AUTO_TEST_SUITE(LEVEL_CONVERSIONS)

    BOOST_AUTO_TEST_CASE(Temperatures)
    {
        Celsius celsius{ 100.0 };
        Fahrenheit fahrenheit { celsius };
        Rankine rankine { fahrenheit };
        Kelvin kelvin { rankine };
        celsius = Celsius(kelvin);

        BOOST_CHECK_EQUAL( Fahrenheit(212.0), fahrenheit );
        BOOST_CHECK_EQUAL( Rankine(671.67), rankine );
        BOOST_CHECK_EQUAL( Kelvin(373.15), kelvin );
        BOOST_CHECK_EQUAL( Celsius(100.0), celsius );
    }

    BOOST_AUTO_TEST_CASE(FloatingPointInaccuracy)
    {
        Fahrenheit expected{ 123.45 };
        auto calculated = Fahrenheit(Kelvin(Rankine(Celsius(expected))));

        // Unfortunately, due to the floating-point quirks ...:
        BOOST_CHECK_NE(expected, calculated);
        // but...:
        BOOST_CHECK_EQUAL(Fahrenheit{123.45000000000005}, calculated);
    }

    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(LEVEL_OPERATORS)

    BOOST_AUTO_TEST_CASE(Comparison)
    {
        {
            // 100 C == 671.67 R
            Rankine rankines { 671.67 };
            Celsius celsiuses { 100.0 };
            BOOST_TEST(rankines == Rankine(celsiuses));
            BOOST_TEST(Celsius(rankines) == celsiuses);
        }
        {
            // 100 C > 100 R
            Rankine rankines { 100.0 };
            Celsius celsiuses { 100.0 };
            BOOST_TEST(rankines < Rankine(celsiuses));
            BOOST_TEST(celsiuses >= Celsius(rankines));
        }
        {
            Celsius celsiuses { 100.0 };
            BOOST_TEST(celsiuses == Celsius(Kelvin{ 100.0 + 273.15 }));
            BOOST_TEST(Kelvin(celsiuses) > Kelvin{ 100.0 });
            BOOST_TEST(Kelvin{ 100.0 } < Kelvin(celsiuses));
            BOOST_TEST(Kelvin{ 100.0 } <= Kelvin(celsiuses));
        }
    }

    BOOST_AUTO_TEST_CASE(Additive)
    {
        DegKelvin kelvins{ 5.0 };
        DegCelsius growth {kelvins };

        BOOST_TEST(Celsius{ 100.0 } + growth == Celsius{ 105.0 });
        BOOST_TEST(Celsius{ 105.0 } - growth == Celsius{ 100.0 });
        BOOST_TEST(Celsius{ 105.0 } - Celsius{ 100.0 } == growth);

        DegRankine rankines{ 9.0 };
        growth = DegCelsius(rankines);

        BOOST_TEST(Celsius{ 100.0 } + growth == Celsius{ 105.0 });
        BOOST_TEST(growth + Celsius{ 100.0 } == Celsius(Fahrenheit{ 221.0}));
        BOOST_TEST(Celsius{ 100.0 } + growth == growth + Celsius{ 100.0 });
        BOOST_TEST(Celsius{ 105.0 } - growth == Celsius{ 100.0 });
        BOOST_TEST(Celsius{ 105.0 } - Celsius{ 100.0 } == growth);
    }

    BOOST_AUTO_TEST_CASE(IncrementDecrement)
    {
        Fahrenheit temperature{ 5.0 };
        auto post = temperature++;
        BOOST_TEST(temperature == Fahrenheit{ 6.0 });

        auto pre = --temperature;
        BOOST_TEST(post == pre);
        BOOST_TEST(pre == temperature);
        BOOST_TEST(temperature == Fahrenheit{5.0});
    }
    BOOST_AUTO_TEST_SUITE_END()

    BOOST_AUTO_TEST_SUITE(LEVEL_FORMATTING)

    BOOST_AUTO_TEST_CASE(Formatting)
    {
        Fahrenheit temperature{ 234.0 };

        BOOST_CHECK_EQUAL(to_string(temperature), "234.000000 deg.F");
        BOOST_CHECK_EQUAL(to_string(temperature, "%.2f [%s]"), "234.00 [deg.F]");
    }

    BOOST_AUTO_TEST_SUITE_END()
}