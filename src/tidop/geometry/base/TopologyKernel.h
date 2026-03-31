/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include "tidop/config.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/Concepts.h"

namespace tl
{
	
/*! \addtogroup Geometry
 *  \{
 */


class TopologyKernel
{

public:

    enum class WindingOrder
    {
        Collinear,
        Clockwise,
        CounterClockwise
    };

    enum class IntersectionType
    {
        None,
        Proper,
        Endpoint,
        EndpointInterior,
        Overlapping
    };

    struct SegmentIntersectionData
    {
        WindingOrder o1;
        WindingOrder o2;
        WindingOrder o3;
        WindingOrder o4;
    };

public:

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr bool equals(const P &a,
                                 const P &b) noexcept
    {
        return (a.x() == b.x() && a.y() == b.y());
    }

    // --------------------------------------------------
    // Orientación
    // --------------------------------------------------

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto orientation(const P &a,
                                      const P &b,
                                      const P &c) noexcept -> WindingOrder
    {    
        const auto det = (b.x() - a.x()) * (c.y() - a.y()) -
                         (b.y() - a.y()) * (c.x() - a.x());

        if (det > 0)
            return WindingOrder::CounterClockwise;

        if (det < 0)
            return WindingOrder::Clockwise;

        return WindingOrder::Collinear;
    }

    // --------------------------------------------------
    // Relaciones colineales
    // --------------------------------------------------

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto isBetween(const P &a,
                                    const P &b,
                                    const P &p) noexcept -> bool
    {
        return p.x() >= std::min(a.x(), b.x()) &&
               p.x() <= std::max(a.x(), b.x()) &&
               p.y() >= std::min(a.y(), b.y()) &&
               p.y() <= std::max(a.y(), b.y());
    }

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto pointOnSegment(const P &a,
                                         const P &b,
                                         const P &p) noexcept -> bool
    {
        if (orientation(a, b, p) != WindingOrder::Collinear)
            return false;

        return isBetween(a, b, p);
    }

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto pointOnSegmentInclusive(const P &a, 
                                                  const P &b, 
                                                  const P &p) noexcept -> bool
    {
        return equals(p, a) ||
               equals(p, b) ||
               (orientation(a, b, p) == WindingOrder::Collinear &&
                isBetween(a, b, p));
    }

    // --------------------------------------------------
    // Segmentos
    // --------------------------------------------------

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto computeIntersectionData(const P &p1,
                                                  const P &p2,
                                                  const P &q1,
                                                  const P &q2) noexcept -> SegmentIntersectionData
    {
        return {
            orientation(p1, p2, q1),
            orientation(p1, p2, q2),
            orientation(q1, q2, p1),
            orientation(q1, q2, p2)
        };
    }

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto isEndpointIntersection(const P &p1,
                                                 const P &p2,
                                                 const P &q1,
                                                 const P &q2) noexcept -> bool
    {
        return equals(p1, q1) ||
               equals(p1, q2) ||
               equals(p2, q1) ||
               equals(p2, q2);
    }

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr bool isEndpointOnSegment(const P &p1, const P &p2,
                                              const P &q1, const P &q2) noexcept
    {
        return pointOnSegment(p1, p2, q1) ||
               pointOnSegment(p1, p2, q2) ||
               pointOnSegment(q1, q2, p1) ||
               pointOnSegment(q1, q2, p2);
    }

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto intersectionType(const P &p1,
                                           const P &p2,
                                           const P &q1,
                                           const P &q2) noexcept -> IntersectionType
    {
        const auto data = computeIntersectionData(p1, p2, q1, q2);

        bool are_collinear = data.o1 == WindingOrder::Collinear &&
                             data.o2 == WindingOrder::Collinear &&
                             data.o3 == WindingOrder::Collinear &&
                             data.o4 == WindingOrder::Collinear;

        // Proper intersection
        if (data.o1 != data.o2 &&
            data.o3 != data.o4 &&
            data.o1 != WindingOrder::Collinear &&
            data.o2 != WindingOrder::Collinear &&
            data.o3 != WindingOrder::Collinear &&
            data.o4 != WindingOrder::Collinear) {
            return IntersectionType::Proper;
        }

        // Collinear case
        if (are_collinear) {

            if (overlapsCollinear(p1, p2, q1, q2))
                return IntersectionType::Overlapping;

            if (isEndpointIntersection(p1, p2, q1, q2))
                return IntersectionType::Endpoint;

            if (isEndpointOnSegment(p1, p2, q1, q2))
                return IntersectionType::EndpointInterior;

        } else {

            if (isEndpointIntersection(p1, p2, q1, q2))
                return IntersectionType::Endpoint;

            if (isEndpointOnSegment(p1, p2, q1, q2))
                return IntersectionType::EndpointInterior;

        }

        return IntersectionType::None;
    }

    template<Point2DConcept P>
    [[nodiscard]]
    static constexpr auto overlapsCollinear(const P &p1,
                                            const P &p2,
                                            const P &q1,
                                            const P &q2) noexcept -> bool
    {
        using T = typename point_traits<P>::value_type;

        T min1, max1, min2, max2;

        if (std::abs(p1.x() - p2.x()) >= std::abs(p1.y() - p2.y())) {
            min1 = std::min(p1.x(), p2.x());
            max1 = std::max(p1.x(), p2.x());
            min2 = std::min(q1.x(), q2.x());
            max2 = std::max(q1.x(), q2.x());
        } else {
            min1 = std::min(p1.y(), p2.y());
            max1 = std::max(p1.y(), p2.y());
            min2 = std::min(q1.y(), q2.y());
            max2 = std::max(q1.y(), q2.y());
        }

        return std::max(min1, min2) < std::min(max1, max2);
    }

};


//class TopologyKernelOperations
//{
//
//public:
//
//    enum class Location
//    {
//        Exterior,
//        Interior,
//        Boundary
//    };
//
//public:
//
//};



/*! \} */ 

} // End namespace tl
