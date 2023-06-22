/*******************************************************************************

    Units of Measurement for C# applications

    Copyright (C) Marek Aniola

    This program is provided to you under the terms of the license
    as published at https://github.com/mangh/metrology


********************************************************************************/
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Bullet_Plain.h"
#include "Bullet_Measured.h"
#include "Benchmark.h"

using namespace Bullet;
using namespace CALINE3::Metrology;

void PrintResults(
    std::vector<std::tuple<Degree, Second, Meter, Meter>> m,
    std::vector<std::tuple<double, double, double, double>> p
);

int main(/*string[] args*/)
{
    // Bullet slope: range and step values
    constexpr double MIN = 0.0;
    constexpr double MAX = 90.0;
    constexpr double STEP = 0.1;

#ifdef _WIN32
    const std::string BREAK_KEY_DESC = "Ctrl-Z + Enter";
#else
    const std::string BREAK_KEY_DESC = "Ctrl-D";
#endif

    std::setlocale(LC_ALL, "en_US.UTF-8");

    std::cout
        << "Units of Measurement for C++ applications. Copyright (C) Marek Aniola." << std::endl
        << "This program is provided to you under the terms of the license" << std::endl
        << "as published at https://github.com/mangh/metrology." << std::endl << std::endl;

    std::cout << "Range of a bullet (demo)";

    // Plain and measured (with units) bullet samplers
    Plain::Calculator plain{ 0.0, 715.0 };
    Measured::Calculator measured{ Meter{ 0.0 }, Meter_Sec{ 715.0 } };

    // Measured/plain performance ratio
    Benchmark ratio;

    do
    {
        // calculate bullet ranges while measure the time spent for the calculations
        std::vector<Plain::Calculator::result_t> p = plain.Compute(MIN, MAX, STEP);
        std::vector<Measured::Calculator::result_t> m = measured.Compute(Degree{ MIN }, Degree{ MAX }, Degree{ STEP });

        // update performance ratio
        ratio.Add(measured.elapsed / plain.elapsed);

        PrintResults(m, p);

        std::cout
            << std::endl
            << measured.benchmark.Count() << ". "
            << "quantity/plain performance ratio: "
            << std::fixed << std::setprecision(0) << measured.elapsed << " us / " 
            << std::fixed << std::setprecision(0) << plain.elapsed << " us = "
            << std::fixed << std::setprecision(2) << (measured.elapsed / plain.elapsed)
            << std::endl;

        std::cout << "Press " << BREAK_KEY_DESC << " to conclude, any non-whitespace key + Enter to retry..." << std::endl;

        std::string response;
        std::cin >> response;
    }
    while (std::cin.get() != EOF);

    std::cout
        << std::endl
        << "Average performance ratio for "
        << ratio.Count() << " run(s): "
        << std::fixed << std::setprecision(0) << measured.benchmark.Average()
        << "(+-" << std::fixed << std::setprecision(0) << measured.benchmark.StdDevPercentage() << "%) us / "
        << std::fixed << std::setprecision(0) << plain.benchmark.Average()
        << "(+-" << std::fixed << std::setprecision(0) << plain.benchmark.StdDevPercentage() << "%) us = "
        << std::fixed << std::setprecision(2) << ratio.Average()
        << "(+-" << std::fixed << std::setprecision(0) << ratio.StdDevPercentage() << "%)."
        << std::endl;

    return 0;
}

/**
 * @brief Print computed results.
 * @param m - measured results,
 * @param p - plain results.
*/
void PrintResults(
    std::vector<std::tuple<Degree, Second, Meter, Meter>> m,
    std::vector<std::tuple<double, double, double, double>> p)
{
    std::cout
        << std::endl
        << " angle    |  tmax |  xmax   |  ymax"
        << std::endl
        << "----------+-------+---------+--------"
        << std::endl;

    Degree slope;
    Second tmax;
    Meter xmax;
    Meter ymax;

    double p_slope;
    double p_tmax;
    double p_xmax;
    double p_ymax;

    // Output formats
    const char* FMT_ANGLE = "%5.2f %s";   // quantity value followed by blank and unit symbol
    const char* FMT_TIME = "%3.0f %s";    // quantity value followed by blank and unit symbol
    const char* FMT_COORD = "%5.0f %s";   // quantity value followed by blank and unit symbol

    for (std::size_t i = 0; i < m.size(); i++)
    {
        std::tie(slope, tmax, xmax, ymax) = m[i];
        std::tie(p_slope, p_tmax, p_xmax, p_ymax) = p[i];

        // Print measured results:

        std::cout
            << to_string(slope, FMT_ANGLE) << " | "
            << to_string(tmax, FMT_TIME) << " | "
            << to_string(xmax, FMT_COORD) << " | "
            << to_string(ymax, FMT_COORD)
            << std::endl;

        // Measured and plain results are to be the same. Is this the case?
        if (((p_slope -= slope.value()) != 0.0) ||
            ((p_tmax -=  tmax.value()) != 0.0) ||
            ((p_xmax -=  xmax.value()) != 0.0) ||
            ((p_ymax -=  ymax.value()) != 0.0))
        {
            std::cout << p_slope << " | " << p_tmax << " | " << p_xmax << " | " << p_ymax << "  *** (m - p) diff ***" << std::endl;
        }
    }
}
