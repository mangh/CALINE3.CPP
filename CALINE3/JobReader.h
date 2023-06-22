#ifndef JOBREADER_H
#define JOBREADER_H

#include <istream>
#include <regex>
#include <string>

#include "Job.h"

namespace CALINE3
{
    struct JobReader
    {
        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Constructor(s)
        ///

        /**
         * @brief JobReader constructor.
         * @param id - input stream identity (e.g. file path),
         * @param is - input stream to read Job(s) from. 
        */
        JobReader(const char *id, std::istream &is)
            : m_id(id), m_is(is), m_lineno(0), m_error(false), m_jobs(), m_ordinal(0)
        {
        }

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Method(s)
        ///

        /**
         * @brief Read Job data (including Receptor set, RUN parameters, Link collection and Meteo conditions).
         * @return @c true on successfully read Job, @c false otherwise.
         * @remarks Sample input JOB line:
         * @code{.txt}
         * EXAMPLE ONE                              60. 10.   0.   0. 1        1.
         * @endcode
         */
        bool Read();

        /**
         * @brief Returns the last read job.
        */
        const Job &LastJob() { return m_jobs.back(); }

        bool ErrorFound() { return m_error; }

    private:

        /**
         * @brief Read the next line from the input stream
         * (the lines are numbered to indicate the position of a possible error).
         * @param line - buffer for the line to read.
         * @return @c true when line has been read, @c false otherwise (EOF).
        */
        bool read_line(std::string& line);

        /**
         * @brief Trim leading and trailing spaces from a string.
         * @param source - original, input string.
         * @return Trimmed string.
        */
        std::string trim(std::string source);

        /**
         * @brief Read all Receptor lines (in the number Job.NR as declared in the parent Job).
         * @param job - parent Job.
         * @return true when all declared Receptor lines have been read; false otherwise.
         * @remarks Sample input RECEPTOR line:
         * @code{.txt}
         * RECP. 1                    30.        0.       1.8
         * @endcode
         */
        bool ReadReceptors(Job &job);

        /**
         * @brief Read RUN parameters: TITLE, NL (number of links) and NM (number of meteo conditions).
         * @param job - parent Job.
         * @returns true when parameters have been read; false otherwise.
         * @remarks Sample input RUN line:
         * @code{.txt}
         * URBAN LOCATION: INTERSECTION              6  1
         * @endcode
         */
        bool ReadRunParameters(Job& job, std::size_t& NL, std::size_t& NM);

        /**
         * @brief Read all Link lines (in the number of NL as previously read).
         * @param job - parent Job,
         * @param NL - number of Link lines to be read.
         * @returns true when all declared Link lines have been read; false otherwise.
         * @remarks Sample input LINK line:
         * @code{.txt}
         * LINK A              AG     0. -5000.     0.  5000.   7500. 30.  0. 30.
         * @endcode
         */
        bool ReadLinks(Job& job, std::size_t NL);

        /**
         * @brief Read all Meteo lines (in the number of NM as previously read).
         * @param job - parent Job,
         * @param NM - number of Meteo conditions to be read.
         * @returns true when all declared Meteo lines have been read; false otherwise.
         * @remarks Sample input METEO line:
         * @code{.txt}
         *   1.270.6 1000. 3.0
         * @endcode
         */
        bool ReadMeteos(Job& job, std::size_t NM);

        ////////////////////////////////////////////////////////////////////////////
        /// 
        ///      Fields
        ///

        std::regex leading_space_regex{ "^\\s+" };
        std::regex trailing_space_regex{ "\\s+$" };
        std::string empty{ "" };

        const char* m_id;           /// Input stream identity (e.g. file path).
        std::istream& m_is;         /// Input stream.
        std::size_t m_lineno;       /// Input stream line number.
        bool m_error;               /// Error found while reading the input stream?
        std::vector<Job> m_jobs;    /// List of read Jobs.
        std::size_t m_ordinal;      /// JOB ordinal number.
    };
}

#endif /* !JOBREADER_H */