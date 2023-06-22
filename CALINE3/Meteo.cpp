#include <iomanip>

#include "Meteo.h"

namespace CALINE3
{
    ///////////////////////////////////////////////////////////////////
    ///
    ///     Constants
    ///

    constexpr Degree DEG_360{ 360.0 };
    constexpr Degree DEG_180{ 180.0 };

    ///////////////////////////////////////////////////////////////////
    ///
    ///     Constructor(s)
    ///

    Meteo::Meteo(std::size_t ordinal, Meter_Sec u, Degree brg, int clas, Meter mixh, Ppm amb) :
        ORDINAL(ordinal),
        U(u),
        BRG1(brg),
        CLAS(clas),
        MIXH(mixh),
        AMB(amb)
    {
        // conversion to vector orientation
        brg += DEG_180;
        m_brg = (brg < DEG_360) ? brg : brg - DEG_360;
    }

    ///////////////////////////////////////////////////////////////////
    ///
    ///     Formatting
    ///

    std::ostream& operator<<(std::ostream& os, const Meteo& met)
    {
        // $"{ORDINAL}. Class {TAG} :: U={U} :: BRG={BRG1} :: MIXH={MIXH} :: AMB={AMB}";
        return os
            << met.ORDINAL << ". "
            << "Class " << met.TAG()
            << " :: U=" << met.U
            << " :: BRG=" << met.BRG1
            << " :: MIXH=" << met.MIXH
            << " :: AMB=" << met.AMB;
    }
}