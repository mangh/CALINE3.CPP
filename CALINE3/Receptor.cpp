#include <iomanip>

#include "Receptor.h"

namespace CALINE3
{
    std::ostream& operator<<(std::ostream& os, const Receptor& rcp)
    {
        return os
            << rcp.ORDINAL << ". "
            << rcp.RCP
            << " :: X=" << rcp.XR
            << " :: Y=" << rcp.YR
            << " :: Z=" << rcp.ZR;
    }
}