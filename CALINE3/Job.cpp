#include "Job.h"

namespace CALINE3
{
    ///////////////////////////////////////////////////////////////////
    ///
    ///     Formatting
    ///

    std::ostream& operator<<(std::ostream& os, const Job& job)
    {
        // $"{ORDINAL}. {JOB}: {NR} receptor(s); {RUN}";
        return os
            << job.ORDINAL << ". "
            << job.JOB << ": "
            << job.Meteos.size() << " meteos(s), "
            << job.Links.size() << " link(s), "
            << job.Receptors.size() << " receptor(s). "
            << "RUN: " << job.RUN;
    }
}
