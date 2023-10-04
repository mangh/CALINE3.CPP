#ifndef METROLOGY_MATH_H
#define METROLOGY_MATH_H

#include <cmath>

// Units required/suplementary:
#include "Degree.h"
#include "Meter.h"
#include "Meter_Sec.h"
#include "Meter2.h"
#include "Meter2_Sec2.h"
#include "Radian.h"

namespace CALINE3::Metrology
{
    ///////////////////////////////////////////////////////////////////////
    ///
    ///  Constants
    ///

    ///////////////////////////////////////////////////////////////////////
    ///
    ///  Metrology math
    ///

    inline double sin(Radian angle) { return std::sin(angle.value()); }
    inline double cos(Radian angle) { return std::cos(angle.value()); }
    inline Radian atan(double x) { return Radian(std::atan(x)); }
    inline Degree abs(Degree d) { return Degree(std::abs(d.value())); }
    inline Meter abs(Meter d) { return Meter(std::abs(d.value())); }
    inline Meter sqrt(Meter2 area) { return Meter(std::sqrt(area.value())); }
    inline Meter_Sec sqrt(Meter2_Sec2 sq_velocity) { return Meter_Sec(std::sqrt(sq_velocity.value())); }

}
#endif /* !METROLOGY_MATH_H */

