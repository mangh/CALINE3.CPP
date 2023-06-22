#include "JobReader.h"

namespace CALINE3
{
    ////////////////////////////////////////////////////////////////////////////
    /// 
    ///      Job method(s)
    ///

    bool JobReader::Read()
    {
        try
        {
            std::string line;
            if (read_line(line))
            {
                m_jobs.emplace_back(
                    /*ORDINAL*/ m_ordinal++,
                    /*JOB*/     trim(line.substr(0, 40)),
                    /*ATIM*/    Minute(stod(line.substr(40, 4))),
                    /*Z0*/      Centimeter(stod(line.substr(44, 4))),
                    /*VS*/      Centimeter_Sec(stod(line.substr(48, 5))),
                    /*VD*/      Centimeter_Sec(stod(line.substr(53, 5))),
                    /*NR*/      stoul(line.substr(58, 2)),
                    /*SCAL*/    stod(line.substr(60, 10))
                );

                Job& job = m_jobs.back();
                std::size_t NL, NM;
                if (ReadReceptors(job) &&
                    ReadRunParameters(job, NL, NM) &&
                    ReadLinks(job, NL) &&
                    ReadMeteos(job, NM))
                {
                    return true;
                }
            }
        }
        catch (std::invalid_argument const& ex)
        {
            std::cerr << m_id << ": file corrupted at line " << m_lineno << " (" << ex.what() << ")." << std::endl;
            m_error = true;
        }
        return false;
    }

    bool JobReader::read_line(std::string& line)
    {
        bool done = std::getline(m_is, line) ? true : false;
        if (done) ++m_lineno;
        return done;
    }

    std::string JobReader::trim(std::string source)
    {
        return std::regex_replace(std::regex_replace(source, leading_space_regex, empty), trailing_space_regex, empty);
    }

    bool JobReader::ReadReceptors(Job& job)
    {
        for (std::size_t m_ordinal = 0; m_ordinal < job.NR; m_ordinal++)
        {
            std::string line;
            if (!read_line(line))
                return false;
            job.Receptors.emplace_back(
                /*ORDINAL*/ m_ordinal,
                /*RCP*/ trim(line.substr(0, 20)),
                /*XR*/  Meter(job.SCAL * stod(line.substr(20, 10))),
                /*YR*/  Meter(job.SCAL * stod(line.substr(30, 10))),
                /*ZR*/  Meter(job.SCAL * stod(line.substr(40, 10)))
            );
        }
        return true;
    }

    bool JobReader::ReadRunParameters(Job& job, std::size_t& NL, std::size_t& NM)
    {
        std::string line;
        if (read_line(line))
        {
            job.RUN = trim(line.substr(0, 40));
            NL = stoi(line.substr(40, 3));
            NM = stoi(line.substr(43, 3));
            return true;
        }
        else
        {
            NL = 0;
            NM = 0;
            return false;
        }
    }

    bool JobReader::ReadLinks(Job& job, std::size_t NL)
    {
        for (std::size_t m_ordinal = 0; m_ordinal < NL; m_ordinal++)
        {
            std::string line;
            if (!read_line(line))
                return false;
            job.Links.emplace_back(
                /*ORDINAL*/ m_ordinal,
                /*LNK*/  trim(line.substr(0, 20)),
                /*TYP*/  line.substr(20, 2),
                /*XL1*/  Meter(job.SCAL * stod(line.substr(22, 7))),
                /*YL1*/  Meter(job.SCAL * stod(line.substr(29, 7))),
                /*XL2*/  Meter(job.SCAL * stod(line.substr(36, 7))),
                /*YL2*/  Meter(job.SCAL * stod(line.substr(43, 7))),
                /*VPHL*/ Vehicles_Hour(stod(line.substr(50, 8))),
                /*EFL*/  Gram_Mile(stod(line.substr(58, 4))),
                /*HL*/   Meter(job.SCAL * stod(line.substr(62, 4))),
                /*WL*/   Meter(job.SCAL * stod(line.substr(66, 4)))
            );
        }
        return true;
    }

    bool JobReader::ReadMeteos(Job& job, std::size_t NM)
    {
        for (std::size_t m_ordinal = 0; m_ordinal < NM; m_ordinal++)
        {
            std::string line;
            if (!read_line(line))
                return false;
            job.Meteos.emplace_back(
                /*ORDINAL*/ m_ordinal,
                /*U   */ Meter_Sec(stod(line.substr(0, 3))),
                /*BRG */ Degree(stod(line.substr(3, 4))),
                /*CLAS*/ stoi(line.substr(7, 1)),
                /*MIXH*/ Meter(stod(line.substr(8, 6))),
                /*AMB */ Ppm(stod(line.substr(14, 4)))
            );
        }
        return true;
    }
}
