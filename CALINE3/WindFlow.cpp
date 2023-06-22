#include "WindFlow.h"

namespace CALINE3
{
    ///////////////////////////////////////////////////////////////////////////
    // 
    //      Constants
    //

    constexpr Degree DEG_70{ 70.0 };
    constexpr Degree DEG_50{ 50.0 };
    constexpr Degree DEG_20{ 20.0 };
    constexpr Degree DEG_90{ 90.0 };
    constexpr Degree DEG_180{ 180.0 };
    constexpr Degree DEG_270{ 270.0 };
    constexpr Degree DEG_360{ 360.0 };

    ///////////////////////////////////////////////////////////////////////////
    // 
    //      Constructor(s)
    //

    WindFlow::WindFlow(const Meteo& meteo, const Link& link)
    {
        // Wind angle with respect to link
        Degree phi = meteo.BRG() - link.LBRG();

        // teta = phi % 90
        Degree teta = abs(phi);
        if (teta >= DEG_270) teta = DEG_360 - teta;
        else if (teta >= DEG_180) teta -= DEG_180;
        else if (teta > DEG_90) teta = DEG_180 - teta;

        m_phi = Radian(phi);
        m_teta = Radian(teta);

        // Set element growth base
        m_base = 
            (teta < DEG_20) ? 1.1 :
            (teta < DEG_50) ? 1.5 :
            (teta < DEG_70) ? 2.0 : 4.0;

        // Residence time
        m_tr = link.DSTR() * link.W2() / meteo.U;
    }
}
