/*******************************************************************************

    Units of Measurement for C# applications applied to
    the CALINE3 Model algorithm.

    For more information on CALINE3 and its status see:
    * https://www.epa.gov/scram/air-quality-dispersion-modeling-alternative-models#caline3
    * https://www.epa.gov/scram/2017-appendix-w-final-rule.

    Copyright (C) mangh

    This program is provided to you under the terms of the license
    as published at https://github.com/mangh/metrology.

********************************************************************************/

#include <chrono>
#include <fstream>

#include "JobReader.h"
#include "Plume.h"
#include "Report.h"

using namespace CALINE3;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Missing or invalid command line arguments" << std::endl
                  << "Usage: " << (argv[0] ? argv[0] : "CALINE3") << " /path/to/input.data" << std::endl;
        return 1;
    }

    // Locale required to read standard input file (CALINE3.EXP) and 
    // print results comparable to standard output file (CALINE3.LST):
    std::setlocale(LC_ALL, "en_US.UTF-8");

    std::ifstream input(argv[1], std::ios::in);
    if (!input.is_open())
    {
        std::cerr << argv[1] << ": failed to open." << std::endl;
        return 2;
    }

    JobReader rdr{ argv[1], input };
    Report report{std::cout};

    std::chrono::duration<double, std::micro> elapsed{ 0.0 };
    while (rdr.Read())
    {
        auto& site = rdr.LastJob();

        for(auto const &meteo : site.Meteos)
        {
            const auto t1 = std::chrono::high_resolution_clock::now();

            /// Mass concentration matrix
            std::vector<std::vector<Microgram_Meter3>> MC{ site.Links.size() };

            for(auto const &link : site.Links)
            {
                Plume plume(site, meteo, link);
                MC[link.ORDINAL] = std::vector<Microgram_Meter3>( site.Receptors.size() );

                for(auto const &receptor : site.Receptors)
                {
                    MC[link.ORDINAL][receptor.ORDINAL] = plume.ConcentrationAt(receptor);
                }
            }

            const auto t2 = std::chrono::high_resolution_clock::now();
            elapsed += t2 - t1;

            report.Print(site, meteo, MC);
        }
    }

    std::cout << std::endl << "Computation time duration (excluding I/O): " << elapsed.count() << " microseconds." << std::endl;

    return rdr.ErrorFound() ? 3 : 0;
}
