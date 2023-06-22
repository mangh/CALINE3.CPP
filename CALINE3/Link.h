#ifndef LINK_H
#define LINK_H

#include <tuple>

#include "Receptor.h"

// Units required/suplementary:
#include "Vehicles_Hour.h"
#include "Gram_Mile.h"
#include "Hertz.h"
#include "Microgram_Meter.h"
#include "Microgram_Meter_Sec.h"

#include "Maths.h"

namespace CALINE3
{
    using namespace Metrology;
    using namespace Maths;

    /**
     * @brief Link raw data independent of Meteo conditions.
     * @param LNK - link title;
     * @param TYP - highway type,
     * @param XL1 - X coordinate of link endpoint 1;
     * @param YL1 - Y coordinate of link endpoint 1;
     * @param XL2 - X coordinate of link endpoint 2;
     * @param YL2 - Y coordinate of link endpoint 2;
     * @param VPHL - traffic volume;
     * @param EFL - emission factor;
     * @param HL - source height;
     * @param WL - mixing zone width.
     */
    struct Link
    {
        ///////////////////////////////////////////////////////////////////
        ///
        ///     Constants
        ///

        static constexpr Meter MIN_HEIGHT{ -10.0 };
        static constexpr Meter MAX_HEIGHT{ 10.0 };
        static constexpr Meter HEIGHT_UNIT{ 1.0 };
        static constexpr Meter MAX_LENGTH{ 10000.0 };
        static constexpr Meter MIN_LENGTH{ 1.0 };

        static constexpr Meter DEPRESSED_SECTION_DEPTH_THRESHOLD{ -1.5 };

    private:

        /// @brief Link tags
        static constexpr const char *TAG[20] =
        {
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
            "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T"
        };

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Basic Properties
        ///

    public:

        /// @brief Link ordinal number.
        const std::size_t ORDINAL;

        /// @brief Link title (description)
        const std::string LNK;

        /// @brief Highway type: AJ=At-Grade, FL=Fill, BR=Bridge, DP=Depressed.
        const std::string TYP;

        /// @brief X-coordinate of link endpoint 1 [m].
        const Meter XL1;

        /// @brief Y-coordinate of link endpoint 1 [m].
        const Meter YL1;

        /// @brief X-coordinate of link endpoint 2 [m].
        const Meter XL2;

        /// @brief Y-coordinate of link endpoint 2 [m].
        const Meter YL2;

        /// @brief Traffic volume [vehicles/hour].
        const Vehicles_Hour VPHL;

        /// @brief Emission factor [g/mile/vehicle].
        const Gram_Mile EFL;

        /// @brief Link height [m].
        /// @pre : -10 m <= HL <= 10 m (model is not verified outside of that range).
        const Meter HL;

        /// @brief Link width [m] (mixing zone width).
        /// @pre : WL >= 10 m: minimum of 1 lane plus 3 meters per side of link.
        const Meter WL;

    private:

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Derived Properties
        ///

        /// @brief Height [m] adjusted for the link type (see also HL).
        Meter m_h;

        /// @brief Link length [m] as defined by endpoint coordinates.
        /// @pre : WL <= LL <= 10 km (there must be
        /// WL <= LL for correct element resolution, and
        /// LL <= 10 km since vertical dispersion curve approximations
        /// are only valid for downwind distances of 10 km or less).
        Meter m_ll;

        /// @brief Link bearing [degrees] for output.
        Degree m_lbrg;

        /// @brief Highway half-width [m] (that is: WL/2).
        Meter m_w2;

        /// @brief Lineal source strength parallel to the highway [microgram/(m*s)].
        /// @remarks
        /// Q1 = 0,172603109 * VPHL * EFL
        /// [vehicles/hr] * [g/mile/vehicle] = 
        ///     10^6/3600/1609,344 [microgram/(meter*sec)] =
        ///     0,172603109 [microgram/(meter*sec)]
        Microgram_Meter_Sec m_q1;

        /// @brief Height HL [m] adjusted for the depressed section.
        Meter m_hds;

        /// @brief Residence time factor for depressed section.
        /// CALINE3 increases the residence time within the mixing zone by
        /// the empirically derived factor based on Los Angeles data. 
        double m_dstr;

    public:

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Constructor(s)
        ///

        /**
         * @brief No default constructor.
         */
        Link() = delete;

        /**
         * @brief Link constructor
         * @param ordinal - link ordinal number,
         * @param lnk - link description,
         * @param typ - link type ("AJ", "FL", "BR", "DP"),
         * @param xl1 - x-coordinate of link endpoint 1 [m],
         * @param yl1 - y-coordinate of link endpoint 1 [m],
         * @param xl2 - x-coordinate of link endpoint 2 [m],
         * @param yl2 - y-coordinate of link endpoint 2 [m],
         * @param vphl - traffic volume [vehicles/hour],
         * @param efl - emission factor [g/mile/vehicle],
         * @param hl - source height [m],
         * @param wl - mixing zone width [m].
         */
        Link(std::size_t ordinal, std::string lnk, std::string typ, Meter xl1, Meter yl1, Meter xl2, Meter yl2, Vehicles_Hour vphl, Gram_Mile efl, Meter hl, Meter wl) :
            ORDINAL(ordinal),
            LNK(lnk),
            TYP(typ),
            XL1(xl1),
            YL1(yl1),
            XL2(xl2),
            YL2(yl2),
            VPHL(vphl),
            EFL(efl),
            HL(hl),
            WL(wl)
        {
            m_ll = Distance(XL1, YL1, XL2, YL2);
            if (m_ll < WL)
            {
                throw std::invalid_argument("Link \"" + lnk + "\" length(" + to_string(m_ll) +") must be greater than or equal to link width(" + to_string(WL) + ")");
            }
            if ((HL < MIN_HEIGHT) || (MAX_HEIGHT < HL))
            {
                throw std::invalid_argument("Source must be within 10 meters of datum (HL[\"" + LNK + " = " + to_string(HL) + ")");
            }

            // Height adjusted for the link type
            m_h = ((TYP == "DP") || (TYP == "FL")) ? Meter(0.0) : HL;

            // Highway half-width
            m_w2 = WL / 2.0;

            // Adjustments for a depressed section:
            if (HL < DEPRESSED_SECTION_DEPTH_THRESHOLD)
            {
                // For depressed sections greater than 1.5 meters deep, CALINE3 increases the
                // residence time within the mixing zone by the following empirically derived
                // factor based on Los Angeles data:
                m_hds = HL;

                // Note: another power-law formula used to approximate empirical data.
                // Unlike dispersion formulas, it has to return dimensionless result
                // for an input in [meters]: that is why it divides HDS / HEIGHT_UNIT
                // to get dimensionless argument required in the standard pow() function:
                m_dstr = 0.72 * pow(abs(m_hds / HEIGHT_UNIT), 0.83);
            }
            else
            {
                m_hds = Meter(1.0);
                m_dstr = 1.0;
            }

            // Link bearing
            m_lbrg = Degree(Azimuth(XL1, YL1, XL2, YL2));
            // Lineal source strength
            m_q1 = Microgram_Meter(EFL) * Hertz(VPHL);
        }

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Methods
        ///

        /**
         * @brief Link tag
         */
        std::string COD() const { return TAG[ORDINAL]; }

        /**
         * @brief Height [m] adjusted for the link type (see also HL).
         */
        Meter H() const { return m_h; }

        /**
         * @brief Link length [m] as defined by endpoint coordinates.
         */
        Meter LL() const { return m_ll; }

        /**
         * @brief Link bearing [degrees] for output.
         */
        Degree LBRG() const { return m_lbrg; }

        /**
         * @brief Highway half-width [m] (that is: WL/2).
         */
        Meter W2() const { return m_w2; }

        /**
         * @brief Lineal source strength parallel to the highway [microgram/(m*s)].
         */
        Microgram_Meter_Sec Q1() const { return m_q1; }

        /**
         * @brief Height HL [m] adjusted for the depressed section.
         */
        Meter HDS() const { return m_hds; }

        /**
         * @brief Residence time factor for depressed section.
         */
        double DSTR() const { return m_dstr; }

        /**
         * @brief Get receptor coordinates relative to the link start position.
         * @param rcp - receptor
         * @returns A tuple (D, L, Z) where:
         * D - receptor-link distance, measured perpendicular to the link,
         * L - receptor offset relative to the link start position, measured parallel to the link,
         * Z - receptor level adjusted for the link type.
         */
        std::tuple<Meter, Meter, Meter> TransformReceptorCoordinates(const Receptor &rcp) const;

        /**
         * @brief Depressed section factor for a receptor at the distance given.
         * @param D - receptor-link distance.
         * @returns Depressed section factor.
         */
        double DepressedSectionFactor(Meter D) const;

        ///////////////////////////////////////////////////////////////////
        ///
        ///     Formatting
        ///

        friend std::ostream& operator<<(std::ostream& os, const Link& link);
    };
}
#endif /* !LINK_H */
