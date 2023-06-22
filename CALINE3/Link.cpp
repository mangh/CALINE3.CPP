#include <iomanip>

#include "Link.h"

namespace CALINE3
{
    ///////////////////////////////////////////////////////////////////
    ///
    ///     Methods
    ///

    std::tuple<Meter, Meter, Meter> Link::TransformReceptorCoordinates(const Receptor &rcp) const
    {
        Meter LR = Distance(XL1, YL1, rcp.XR, rcp.YR);

        /// Receptor angle with respect to link
        Radian lbrg = Radian(m_lbrg);

        Radian GAMMA = Azimuth(XL1, YL1, rcp.XR, rcp.YR) - lbrg;

        Meter D = LR * sin(GAMMA);
        Meter L = LR * cos(GAMMA) - m_ll;

        Meter Z = rcp.ZR;
        if ((TYP != "AG") && (TYP != "BR"))
        {
            Meter D1 = m_w2 + 2.0 * abs(HL);
            if (abs(D) < D1)
            {
                // 2:1 SLOPE ASSUMED
                Z -= (abs(D) <= m_w2) ? HL : HL * (1.0 - (abs(D) - m_w2) / (2.0 * abs(HL)));
            }
        }

        return std::make_tuple(D, L, Z);
    }

    double Link::DepressedSectionFactor(Meter D) const
    {
        return ((m_hds >= DEPRESSED_SECTION_DEPTH_THRESHOLD) || (abs(D) >= (m_w2 - 3.0 * m_hds))) ? 1.0 :
            (abs(D) <= m_w2) ? m_dstr : m_dstr - (m_dstr - 1.0) * ((abs(D) - m_w2) / (-3.0 * m_hds));
    }

    ///////////////////////////////////////////////////////////////////
    ///
    ///     Formatting
    ///

    std::ostream& operator<<(std::ostream& os, const Link& link)
    {
        // = > $"{ORDINAL}. {LNK} ({TYP}) :: BRG={LBRG} :: LL={LL} :: ({XL1}, {YL1}) to ({XL2}, {YL2}) :: VPHL={VPHL} :: EFL={EFL}";
        return os
            << link.ORDINAL << ". "
            << link.LNK
            << " (" << link.TYP << ")"
            << " :: BRG=" << link.LBRG()
            << " :: LL=" << link.LL()
            << " :: (" << link.XL1 << ", " << link.YL1 << ") to (" << link.XL2 << ", " << link.YL2 << ")"
            << " :: VPHL=" << link.VPHL
            << " :: EFL=" << link.EFL;
    }
}
