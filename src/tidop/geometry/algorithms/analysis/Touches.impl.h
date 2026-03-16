#include "Touches.h"
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

namespace tl
{

namespace detail 
{

/* Point - Point */

template<Point2DConcept P1, Point2DConcept P2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const P1 &,
                  const P2 &,
                  const Policy &,
                  point_tag,
                  point_tag) -> bool
{
    return false;
}


/* Segment – Segment */

template<Segment2DConcept S1, Segment2DConcept S2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const S1 &segment1,
                  const S2 &segment2,
                  const Policy &policy,
                  segment_tag,
                  segment_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment1.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment1.pt2());
    const auto q1 = policy.toKernelPoint<Dimension::dim2>(segment2.pt1());
    const auto q2 = policy.toKernelPoint<Dimension::dim2>(segment2.pt2());

    auto intersection_type = TopologyKernel::intersectionType(p1, p2, q1, q2);

    return intersection_type == TopologyKernel::IntersectionType::Endpoint ||
           intersection_type == TopologyKernel::IntersectionType::EndpointInterior;
}

/* LineString – LineString */

template<LineString2DConcept LS1, LineString2DConcept LS2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const LS1 &lineString1,
                  const LS2 &lineString2,
                  const Policy &policy,
                  linestring_tag,
                  linestring_tag) -> bool
{
    
    auto k_linestring_1 = policy.toKernelLineString<Dimension::dim2>(lineString1);
    auto k_linestring_2 = policy.toKernelLineString<Dimension::dim2>(lineString2);

    bool has_endpoint_contact = false;

    for (size_t i = 0; i + 1 < lineString1.size(); ++i) {

        const auto p1 = k_linestring_1[i];
        const auto p2 = k_linestring_1[i + 1];

        for (size_t j = 0; j + 1 < lineString2.size(); ++j) {

            const auto q1 = k_linestring_2[j];
            const auto q2 = k_linestring_2[j + 1];

            auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);

            if (type == TopologyKernel::IntersectionType::Proper||
                type == TopologyKernel::IntersectionType::Overlapping)
                return false;

            if (type == TopologyKernel::IntersectionType::Endpoint )
                has_endpoint_contact = true;
        }
    }

    return has_endpoint_contact;
}


/* Polygon - Polygon */

template<Polygon2DConcept Poly1, Polygon2DConcept Poly2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Poly1 &poly1,
                  const Poly2 &poly2,
                  const Policy &policy,
                  polygon_tag, 
                  polygon_tag) -> bool 
{
    using P = Policy::KernelPoint2D;

    auto kpoly1 = policy.toKernelPolygon<Dimension::dim2>(poly1);
    auto kpoly2 = policy.toKernelPolygon<Dimension::dim2>(poly2);

    auto for_each_segment = [](const auto &poly, auto &&callback) {
        const auto &outer = poly.outer();

        for (size_t i = 0; i + 1 < outer.size(); ++i)
            callback(outer[i], outer[i + 1]);

        for (const auto &hole : poly.inners())
            for (size_t i = 0; i + 1 < hole.size(); ++i)
                callback(hole[i], hole[i + 1]);
    };

    bool has_endpoint_contact = false;

    for_each_segment(kpoly1, [&](const P &p1, const P &p2) {
        for_each_segment(kpoly2, [&](const P &q1, const P &q2) {

            auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);

            if (type == TopologyKernel::IntersectionType::Proper)
                return false;

            if (type == TopologyKernel::IntersectionType::Endpoint || 
                type == TopologyKernel::IntersectionType::Overlapping)
                has_endpoint_contact = true;

            return true;
        });

    });

    return has_endpoint_contact;
}


/* Point - Segment */

template<Point2DConcept Point, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Point &point,
                  const Segment &segment,
                  const Policy &policy,
                  point_tag,
                  segment_tag) -> bool
{
    // Un punto toca un segmento si coincide con alguno de sus extremos (frontera)
    // Si el punto está en el interior del segmento (pero no es extremo) retorna false.
    return equalsExact(point, segment.pt1(), policy) || equalsExact(point, segment.pt2(), policy);
}


/* Point – LineString */

template<Point2DConcept Point, LineString2DConcept LineString, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Point &point,
                  const LineString &lineString,
                  const Policy &policy,
                  point_tag,
                  linestring_tag) -> bool
{
	return locatePointOnLineString(lineString, point, policy) == Location::Boundary;
}


/* Point - Polygon */

template<Point2DConcept Point, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Point &point,
                  const Polygon &polygon,
                  const Policy &policy,
                  point_tag, 
                  polygon_tag) -> bool 
{
    return locatePointInPolygon(polygon, point, policy) == Location::Boundary;
}


/* Segment - Point */

template<Segment2DConcept Segment, Point2DConcept Point, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Segment &segment, 
                  const Point &point,
                  const Policy &policy,
                  segment_tag,
                  point_tag) -> bool
{
    return touches_impl(point, segment, policy, point_tag{}, segment_tag{});
}


/* Segment – LineString */

template<Segment2DConcept Segment, LineString2DConcept LineString, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Segment &segment,
                  const LineString &lineString,
                  const Policy &policy,
                  segment_tag,
                  linestring_tag) -> bool
{
    bool has_endpoint_contact = false;

    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment.start());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment.end());
    auto k_linestring = policy.toKernelLineString<Dimension::dim2>(lineString);

    for (size_t j = 0; j + 1 < lineString.size(); ++j) {

        auto type = TopologyKernel::intersectionType(p1, p2, k_linestring[j], k_linestring[j+1]);

        if (type == TopologyKernel::IntersectionType::Proper)
            return false;

        if (type == TopologyKernel::IntersectionType::Endpoint ||
            type == TopologyKernel::IntersectionType::Overlapping)
            has_endpoint_contact = true;
    }

    return has_endpoint_contact;
}


/* Segment – Polygon */

template<Segment2DConcept Segment, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Segment &segment,
                  const Polygon &polygon,
                  const Policy &policy,
                  segment_tag,
                  polygon_tag) -> bool
{
    using P = Policy::KernelPoint2D;

    bool has_endpoint_contact = false;

    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment.start());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment.end());

    auto kpolygon = policy.toKernelPolygon<Dimension::dim2>(polygon);

    auto for_each_segment = [](const auto &poly, auto &&callback) {
        const auto &outer = poly.outer();

        for (size_t i = 0; i + 1 < outer.size(); ++i) {
            if (!callback(outer[i], outer[i + 1])) 
                return false;
        }

        for (const auto &hole : poly.inners()) {
            for (size_t i = 0; i + 1 < hole.size(); ++i) {
                if (!callback(hole[i], hole[i + 1])) 
                    return false;
            }
        }

        return true;
    };

    bool check_touch = for_each_segment(kpolygon, [&](const P &q1, const P &q2) {

        auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);

        if (type == TopologyKernel::IntersectionType::Proper)
            return false;

        if (type == TopologyKernel::IntersectionType::Endpoint ||
            type == TopologyKernel::IntersectionType::Overlapping)
            has_endpoint_contact = true;

        return true;

    });

    if (!check_touch) return false;

    return has_endpoint_contact;
}


/* LineString – Point */

template<LineString2DConcept LineString, Point2DConcept Point, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const LineString &lineString, 
                  const Point &point,
                  const Policy &policy,
                  linestring_tag,
                  point_tag) -> bool
{
    return touches_impl(point, lineString, policy, point_tag{}, linestring_tag{});
}


/* LineString – Segment */

template<LineString2DConcept LineString, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const LineString &lineString,
                  const Segment &segment,
                  const Policy &policy,
                  linestring_tag,
                  segment_tag) -> bool
{
    return touches_impl(segment, lineString, policy, segment_tag{}, linestring_tag{});
}


/* LineString – Polygon */

template<LineString2DConcept LineString, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const LineString &lineString,
                  const Polygon &polygon,
                  const Policy &policy,
                  linestring_tag,
                  polygon_tag) -> bool
{
    using P = Policy::KernelPoint2D;

    auto k_line_string = policy.toKernelLineString<Dimension::dim2>(lineString);
    auto k_polygon = policy.toKernelPolygon<Dimension::dim2>(polygon);

    

    auto for_each_segment = [](const auto &poly, auto &&callback) {
        const auto &outer = poly.outer();

        for (size_t i = 0; i + 1 < outer.size(); ++i) {
            if (!callback(outer[i], outer[i + 1])) return false;
        }

        for (const auto &hole : poly.inners()) {
            for (size_t i = 0; i + 1 < hole.size(); ++i) {
                if (!callback(hole[i], hole[i + 1])) return false;
            }
        }

        return true;
    };

    bool has_endpoint_contact = false;
    const auto &outer = polygon.outer();

    for (size_t i = 0; i + 1 < k_line_string.size(); ++i) {

        const auto p1 = k_line_string[i];
        const auto p2 = k_line_string[i + 1];

        auto loc1 = locatePointInPolygon(polygon, lineString[i], policy);
        auto loc2 = locatePointInPolygon(polygon, lineString[i + 1], policy);

        if (loc1 == Location::Interior || loc2 == Location::Interior)
            return false;

        // Si ambos puntos están en el borde, comprobar un punto intermedio para descartar el caso de que la línea toque el borde pero no cruce el polígono
        if (loc1 == Location::Boundary && loc2 == Location::Boundary) {
            auto mid = Segment<P>(lineString[i], lineString[i + 1]).midPoint();

            if (locatePointInPolygon(polygon, mid, policy) == Location::Interior)
                return false;
        }

        //if ((locatePointInRing(polygon.outer(), lineString[i], policy) == Location::Interior ||
        //     locatePointInRing(polygon.outer(), lineString[i+1], policy) == Location::Interior) &&
        //    polygon.inners().empty()) return false;

        for (const auto &hole : polygon.inners()) {

            auto loc_ring1 = locatePointInRing(hole, lineString[i], policy);
            auto loc_ring2 = locatePointInRing(hole, lineString[i + 1], policy);

            if (loc1 == Location::Boundary) {
                if (loc_ring1 != Location::Boundary && loc_ring2 == Location::Interior)
                    return false;
            }
            if (loc2 == Location::Boundary) {
                if (loc_ring2 != Location::Boundary && loc_ring1 == Location::Interior)
                    return false;
            }
            //if (locatePointInRing(hole, p1, policy) == Location::Interior ||
            //    locatePointInRing(hole, p2, policy) == Location::Interior) return false;
        }

        bool check_touch = for_each_segment(k_polygon, [&](const P &q1, const P &q2) {

            auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);

            if (type == TopologyKernel::IntersectionType::Proper)
                return false;

            if (type == TopologyKernel::IntersectionType::Endpoint ||
                type == TopologyKernel::IntersectionType::Overlapping)
                has_endpoint_contact = true;

            return true;
        });

        if (!check_touch) return false;
    }

    return has_endpoint_contact;
}


/* Polygon - Point */

template<Polygon2DConcept Polygon, Point2DConcept Point, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Polygon &polygon,
                  const Point &point,
                  const Policy &policy,
                  polygon_tag,
                  point_tag) -> bool
{
    return touches_impl(point, polygon, policy, point_tag{}, polygon_tag{});
}


/* Polygon - Segment */

template<Polygon2DConcept Polygon, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Polygon &polygon,
                  const Segment &segment,
                  const Policy &policy,
                  polygon_tag, 
                  segment_tag) -> bool
{
    return touches_impl(segment, polygon, policy, segment_tag{}, polygon_tag{});
}


/* Polygon - LineString */


template<Polygon2DConcept Polygon, LineString2DConcept LineString, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto touches_impl(const Polygon &polygon,
                  const LineString &lineString,
                  const Policy &policy,
                  polygon_tag,
                  linestring_tag) -> bool
{
    return touches_impl(lineString, polygon, policy, linestring_tag{}, polygon_tag{});  
}


template<typename Point_t, typename Geometry_t, PrecisionPolicyConcept Policy>
auto touches_impl(const GeometryCollection<Point_t> &gc,
                  const Geometry_t &geom,
                  const Policy &policy,
                  collection_tag,
                  geometry_tag_t<Geometry_t>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
            return touches(arg.get(), geom, policy);
            }, item);

        if (hit) return true;
    }

    return false;
}

template<typename Point_t, typename Geometry_t, PrecisionPolicyConcept Policy>
auto touches_impl(const Geometry_t &geom,
                  const GeometryCollection<Point_t> &gc,
                  const Policy &policy,
                  geometry_tag_t<Geometry_t>,
                  collection_tag) -> bool
{
    return touches_impl(gc, geom, policy, collection_tag{}, geometry_tag_t<Geometry_t>{});
}


} // namespace detail


template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
auto touches(const G1 &geom1, const G2 &geom2) -> bool
{
    using Scalar = typename point_traits<geometry_traits<G1>::point_type>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return touches(geom1, geom2, policy);
}

template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
constexpr auto touches(const G1 &geom1, const G2 &geom2, const Policy &policy) -> bool
{
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if(geom1.isEmpty() || geom2.isEmpty())
        return false;

    if constexpr (MultiGeometryConcept<G1> && !GeometryCollectionConcept<G1>) {
        for (const auto &a : geom1) {
            if (touches(a, geom2, policy)) return true;
        }
        return false;
    } else if constexpr (MultiGeometryConcept<G2> && !GeometryCollectionConcept<G2>) {
        for (const auto &b : geom2) {
            if (touches(geom1, b, policy)) return true;
        }
        return false;
    } else {
        using tag1 = geometry_tag_t<G1>;
        using tag2 = geometry_tag_t<G2>;
        return detail::touches_impl(geom1, geom2, policy, tag1{}, tag2{});
    }
}



} // namespace tl