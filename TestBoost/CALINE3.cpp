#include <boost/test/unit_test.hpp>

#include <sstream>

#include "../CALINE3/JobReader.h"
#include "../CALINE3/Plume.h"

// Test input data
const char* test_data = R"sample(EXAMPLE FOUR                             60.100.   0.   0.12        1.
RECP. 1                  -350.       30.       1.8
RECP. 2                     0.       30.       1.8
RECP. 3                   750.      100.       1.8
RECP. 4                   850.       30.       1.8
RECP. 5                  -850.     -100.       1.8
RECP. 6                  -550.     -100.       1.8
RECP. 7                  -350.     -100.       1.8
RECP. 8                    50.     -100.       1.8
RECP. 9                   450.     -100.       1.8
RECP. 10                  800.     -100.       1.8
RECP. 11                 -550.       25.       1.8
RECP. 12                 -550.       25.       6.1
URBAN LOCATION: MULTIPLE LINKS, ETC.      6  4
LINK A              AG   500.     0.  3000.     0.   9700. 30.  0. 23.
LINK B              DP   500.     0.  1000.   100.   1200.150. -2. 13.
LINK C              AG -3000.     0.   500.     0.  10900. 30.  0. 23.
LINK D              AG -3000.   -75.  3000.   -75.   9300. 30.  0. 23.
LINK E              BR  -500.   200.  -500.  -300.   4000. 50. 6.1 27.
LINK F              BR  -100.   200.  -100.  -200.   5000. 50. 6.1 27.
 1.  0.6 1000.12.0
 1. 90.6 1000. 7.0
 1.180.6 1000. 5.0
 1.270.6 1000. 6.7
)sample";

// Expected test results (dimensionless mass concentrations)
double test_results[4][6][12] =
{
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0.0035990200542924855, 3637.3905593118443, 0, 0 },
        { 0, 0, 0, 3236.2999074329236, 0, 0, 0, 0, 0.046132577015889881, 1634.3597087515257, 0, 0 },
        { 0, 0, 0, 0, 4087.3770202576379, 4087.3770202576379, 4087.3770202576361, 4087.3770202576375, 4087.372975997986, 0, 0, 0 },
        { 0, 0, 0, 0, 6869.7942521390269, 6869.794252139025, 6869.7942521389878, 6869.7942521389587, 6869.794252139026, 6869.794252138945, 0, 0 },
        { 0, 0, 0, 0, 0, 344.53387287904945, 6.1449684843324574e-05, 0, 0, 0, 26.055705968760208, 24.152225659748591 },
        { 0, 2.6980974100484905e-05, 0, 0, 0, 0, 0, 7.6812106054155701e-05, 0, 0, 0, 0 }
    },
    {
        { 6463.038065280547, 9125.0689423517615, 3590.5953845010981, 13687.541844273805, 3561.3807616238587, 4008.1221723439598, 4290.5196439755591, 4495.2001295653872, 4028.4674653169877, 3511.9800448779861, 5568.1075358808184, 5527.6293779606958 },
        { 1493.591456499636, 2455.8523857827636, 2967.3479722315383, 0.021226441583868595, 402.39325752065798, 376.09895417351566, 319.53608296377882, 91.889005155747228, 0.035524982661012047, 0, 1192.0600729088069, 1181.0206955972631 },
        { 10137.131821352983, 6671.8261480635447, 0, 0, 2100.8270941607566, 1296.9794148261881, 760.54502384028297, 43.700926966675858, 0, 0, 13996.547565741286, 13331.766867215189 },
        { 4477.9880357201537, 4122.2718836574159, 1065.8430205435495, 3030.9238862496168, 17562.565574439795, 17280.475672064003, 17073.671805840404, 16604.984049394068, 16041.057200458354, 15439.842086254164, 4949.4461712183593, 4903.6780056375237 },
        { 0, 0, 0, 0, 1268.1978748935226, 2613.9465496261255, 0, 0, 0, 0, 2613.9465496261255, 2327.0818060193169 },
        { 1853.8642522478949, 0, 0, 0, 1044.2490026973869, 1399.2145604219331, 1853.8523536212808, 0, 0, 0, 1404.9254755420384, 1365.6256334064942 }
    },
    {
        { 0, 0, 3637.3905593118443, 6581.7499944218744, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 2877.388437488567, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 7395.9871071338639, 7395.987107133833, 0, 0, 0, 0, 0, 0, 0, 0, 8051.6943465802342, 4375.0858292571593 },
        { 3402.1552745297231, 3402.1552745297217, 2621.0746109717593, 3402.1552745297258, 0, 0, 0, 0, 0, 0, 3487.3950723299113, 3020.2593906078246 },
        { 0.00079762327635938125, 0, 0, 0, 0, 58.29360782771893, 7.7648080226117373e-12, 0, 0, 0, 447.56429984969958, 425.97640009535667 },
        { 0, 0.02138309964122766, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0.077482312561287986, 3911.9557411650039, 0, 0, 0, 0, 0, 0.96548442610972773, 0, 0 },
        { 0, 0, 0.080582337908343044, 5957.3574745386277, 0, 0, 0, 0, 0, 0.22868147562609165, 0, 0 },
        { 16370.800002967168, 16925.511166497578, 6005.3991732295171, 13574.403902546395, 3856.0915685530776, 4369.8127676554814, 4677.5754136914275, 5223.4769704414875, 5693.3560783561443, 6053.505138117187, 18613.514829951968, 17910.221936991034 },
        { 3717.2589909867256, 4122.2718836574204, 2300.0922212057708, 4917.6499000933991, 15343.434609333704, 15881.255772344342, 16192.8523460257, 16729.833503716207, 17179.001508089947, 17517.595515190565, 3728.3723613941274, 3687.7831560561203 },
        { 1850.354438185931, 1067.6178456515599, 584.2089383337543, 623.03212696312994, 0, 0, 1850.3544381859306, 1018.7945073841264, 773.19338372013976, 649.93955063915746, 0, 0 },
        { 0, 2698.9334441048622, 961.71287887366736, 960.01983615996664, 0, 0, 0, 2312.9430477136384, 1257.1555027060215, 925.00087429854329, 0, 0 }
    }
};

namespace CALINE3::Metrology
{
    BOOST_AUTO_TEST_SUITE(CALINE3)

    BOOST_AUTO_TEST_CASE(ComputingConcentration)
    {
        std::setlocale(LC_ALL, "en_US.UTF-8");
        std::istringstream input_stream{ test_data };
        JobReader job_reader{ "INTERNAL DATA", input_stream };

        BOOST_TEST(job_reader.Read());

        const Job& site = job_reader.LastJob();

        for (auto const& meteo : site.Meteos)
        {
            for (auto const& link : site.Links)
            {
                Plume plume(site, meteo, link);

                for (auto const& receptor : site.Receptors)
                {
                    Microgram_Meter3 MC = plume.ConcentrationAt(receptor);
                    BOOST_TEST_INFO("MC (mass concentration of the pollutant) = " << MC);
                    BOOST_TEST_INFO("- at the receptor: " << receptor);
                    BOOST_TEST_INFO("- from the link: " << link);
                    BOOST_TEST_INFO("- exposed to the meteo: " << meteo);
                    BOOST_CHECK_CLOSE_FRACTION(MC.value(), test_results[meteo.ORDINAL][link.ORDINAL][receptor.ORDINAL], 1.0e-15);
                }
            }
        }
    }

    BOOST_AUTO_TEST_SUITE_END()
}

