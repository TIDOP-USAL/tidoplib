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
constexpr auto crosses_impl(const P1 &,
                            const P2 &,
                            const Policy &,
                            point_tag,
                            point_tag) -> bool
{
    return false; // Puntos no pueden cruzarse según OGC
}


/* Segment – Segment */

template<Segment2DConcept S1, Segment2DConcept S2, PrecisionPolicyConcept Policy>
[[nodiscard]] 
constexpr auto crosses_impl(const S1 &segment1,
                            const S2 &segment2,
                            const Policy &policy,
                            segment_tag,
                            segment_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment1.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment1.pt2());
    const auto q1 = policy.toKernelPoint<Dimension::dim2>(segment2.pt1());
    const auto q2 = policy.toKernelPoint<Dimension::dim2>(segment2.pt2());

    return TopologyKernel::intersectionType(p1, p2, q1, q2) == TopologyKernel::IntersectionType::Proper;
}


/* LineString – LineString */

template<typename Point_t>
auto checkSideSwitch(const Point_t &p_prev,
                     const Point_t &v,
                     const Point_t &p_next,
                     const Point_t &q1,
                     const Point_t &q2) -> bool
{
    auto orient_prev = TopologyKernel::orientation(q1, q2, p_prev);
    auto orient_next = TopologyKernel::orientation(q1, q2, p_next);

    return (orient_prev == TopologyKernel::WindingOrder::Clockwise && orient_next == TopologyKernel::WindingOrder::CounterClockwise) ||
           (orient_prev == TopologyKernel::WindingOrder::CounterClockwise && orient_next == TopologyKernel::WindingOrder::Clockwise);
}

template<typename Point_t>
bool isVertexCrossing(const Point_t &v, 
                      const Point_t &q1, 
                      const Point_t &q3,
                      const Point_t &p1, 
                      const Point_t &p3)
{
    auto or_p1_q1 = TopologyKernel::orientation(v, q1, p1);
    auto or_p3_q1 = TopologyKernel::orientation(v, q1, p3);
    auto or_p1_q3 = TopologyKernel::orientation(v, q3, p1);
    auto or_p3_q3 = TopologyKernel::orientation(v, q3, p3);

    bool p1_left_q1 = (or_p1_q1 == TopologyKernel::WindingOrder::CounterClockwise);
    bool p3_left_q1 = (or_p3_q1 == TopologyKernel::WindingOrder::CounterClockwise);
    bool p1_left_q3 = (or_p1_q3 == TopologyKernel::WindingOrder::CounterClockwise);
    bool p3_left_q3 = (or_p3_q3 == TopologyKernel::WindingOrder::CounterClockwise);

    return (p1_left_q1 != p3_left_q1) && (p1_left_q3 != p3_left_q3);
}


template<LineString2DConcept LS1, LineString2DConcept LS2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto crosses_impl(const LS1 &lineString1,
                  const LS2 &lineString2,
                  const Policy &policy,
                  linestring_tag,
                  linestring_tag) -> bool
{
    auto k_linestring_1 = policy.toKernelLineString<Dimension::dim2>(lineString1);
    auto k_linestring_2 = policy.toKernelLineString<Dimension::dim2>(lineString2);

    bool proper_intersection = false;

    if (lineString1.size() < 2 || lineString2.size() < 2) return false;

    for (size_t i = 0; i + 1 < lineString1.size(); ++i) {

        const auto p1 = k_linestring_1[i];
        const auto p2 = k_linestring_1[i + 1];

        for (size_t j = 0; j + 1 < lineString2.size(); ++j) {

            const auto q1 = k_linestring_2[j];
            const auto q2 = k_linestring_2[j + 1];

            auto type = TopologyKernel::intersectionType(p1,p2,q1,q2);

            if (type == TopologyKernel::IntersectionType::Overlapping)
                return false;

            if (type == TopologyKernel::IntersectionType::Proper) {
                proper_intersection = true;
            } else if (type == TopologyKernel::IntersectionType::EndpointInterior) {

                if (i + 2 < lineString1.size()) {
                    if (TopologyKernel::pointOnSegmentInclusive(q1, q2, p2)) {
                        if (checkSideSwitch(p1, p2, k_linestring_1[i + 2], q1, q2)) {
                            proper_intersection = true;
                        }
                    }
                }

                if (j + 2 < lineString2.size()) {
                    if (TopologyKernel::pointOnSegmentInclusive(p1, p2, q2)) {
                        if (checkSideSwitch(k_linestring_2[j], q2, k_linestring_2[j + 2], p1, p2)) {
                            proper_intersection = true;
                        }
                    }
                }
            } else if (type == TopologyKernel::IntersectionType::Endpoint) {

                if (i + 2 < lineString1.size() && j + 2 < lineString2.size()) {
                    const auto &v = p2;
                    const auto &p1_ptr = p1;
                    const auto &p3_ptr = k_linestring_1[i + 2];
                    const auto &q1_ptr = q1;
                    const auto &q3_ptr = k_linestring_2[j + 2];

                    if (isVertexCrossing(v, q1_ptr, q3_ptr, p1_ptr, p3_ptr) && 
                        isVertexCrossing(v, p1_ptr, p3_ptr, q1_ptr, q3_ptr)) {
                        proper_intersection = true;
                    }
                }
            }
        }
    }

    return proper_intersection;
}


/* Polygon - Polygon */

template<Polygon2DConcept P1, Polygon2DConcept P2, PrecisionPolicyConcept Policy>
auto crosses_impl(const P1 &poly1,
    const P2 &poly2,
    const Policy &policy,
    polygon_tag,
    polygon_tag) -> bool
{
    return false;
}



/* Point */

template<Point2DConcept Point, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
constexpr auto crosses_impl(const Point &, 
                            const Segment &,
                            const Policy &, 
                            point_tag, 
                            segment_tag) -> bool
{
    return false;
}

template<Point2DConcept Point, LineString2DConcept LineString, PrecisionPolicyConcept Policy>
constexpr auto crosses_impl(const Point &,
                            const LineString &,
                            const Policy &,
                            point_tag, 
                            linestring_tag) -> bool
{
    return false;
}

template<Point2DConcept Point, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
constexpr auto crosses_impl(const Point &,
                            const Polygon &, 
                            const Policy &,
                            point_tag, 
                            polygon_tag) -> bool
{
    return false;
}

/* Segment */

template<Segment2DConcept Segment, Point2DConcept Point, PrecisionPolicyConcept Policy>
constexpr auto crosses_impl(const Segment &,
                            const Point &,
                            const Policy &, 
                            segment_tag,
                            point_tag) -> bool
{
    return false;
}


/* Segment – LineString */

template<Segment2DConcept Segment, LinearGeometry2DConcept LineString, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto crosses_impl(const Segment &segment,
                  const LineString &lineString,
                  const Policy &policy,
                  segment_tag,
                  linestring_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment.start());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment.end());
    auto k_linestring = policy.toKernelLineString<Dimension::dim2>(lineString);

    bool proper_intersection = false;

    for (size_t i = 0; i + 1 < lineString.size(); ++i) {

        auto type = TopologyKernel::intersectionType(p1, p2, k_linestring[i], k_linestring[i + 1]);

        if (type == TopologyKernel::IntersectionType::Overlapping)
            return false;

        if (type == TopologyKernel::IntersectionType::Proper)
            proper_intersection = true;
    }

    return proper_intersection;
}


template<Segment2DConcept Segment, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
auto crosses_impl(const Segment &segment,
                  const Polygon &polygon,
                  const Policy &policy,
                  segment_tag,
                  polygon_tag) -> bool
{
    //if (!intersects(segment, polygon, policy))
    //    return false;

    //if (within(segment, polygon, policy))
    //    return false;

    //if (touches(segment, polygon, policy))
    //    return false;

    //return true;

    using P = Policy::KernelPoint2D;

    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment.start());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment.end());
    auto kpolygon = policy.toKernelPolygon<Dimension::dim2>(polygon);

    bool proper_intersection = false;

    auto for_each_segment = [](const auto &poly, auto &&callback) {
        const auto &outer = poly.outer();

        for (size_t i = 0; i + 1 < outer.size(); ++i)
            callback(outer[i], outer[i + 1]);

        for (const auto &hole : poly.inners())
            for (size_t i = 0; i + 1 < hole.size(); ++i)
                callback(hole[i], hole[i + 1]);
        };

    for_each_segment(kpolygon, [&](const P &q1, const P &q2) {

        auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);

        if (type == TopologyKernel::IntersectionType::Overlapping)
            return false;

        if (type == TopologyKernel::IntersectionType::Proper)
            proper_intersection = true;

        return true;

        });

    return proper_intersection;
}

/* LineString */

template<LineString2DConcept LineString, Point2DConcept Point, PrecisionPolicyConcept Policy>
constexpr auto crosses_impl(const LineString &,
                            const Point &,
                            const Policy &,
                            linestring_tag, 
                            point_tag) -> bool
{
    return false;
}

template<LinearGeometry2DConcept LineString, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
auto crosses_impl(const LineString &lineString,
                  const Segment &segment,
                  const Policy &policy,
                  linestring_tag,
                  segment_tag) -> bool
{
    return crosses_impl(segment, lineString, policy, segment_tag{}, linestring_tag{});
}

template<LineString2DConcept LineString, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
auto crosses_impl(const LineString &lineString,
                  const Polygon &polygon,
                  const Policy &policy,
                  linestring_tag, 
                  polygon_tag) -> bool
{
    for (size_t i = 0; i + 1 < lineString.size(); ++i) {
        Segment<typename geometry_traits<LineString>::point_type> edge(lineString[i], lineString[i + 1]);
        if (crosses(edge, polygon, policy))
            return true;
    }
    return false;
}

/* Polygon */

template<Polygon2DConcept Polygon, Point2DConcept Point, PrecisionPolicyConcept Policy>
constexpr auto crosses_impl(const Polygon &, 
                            const Point &,
                            const Policy &,
                            polygon_tag, 
                            point_tag) -> bool
{
    return false;
}

template<Polygon2DConcept Polygon, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
auto crosses_impl(const Polygon &polygon, 
                  const Segment &segment,
                  const Policy &policy,
                  polygon_tag,
                  segment_tag) -> bool
{
    return false;
}

template<LineString2DConcept LineString, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
auto crosses_impl(const Polygon &polygon,
                  const LineString &lineString,
                  const Policy &policy,
                  polygon_tag,
                  linestring_tag) -> bool
{
    return false;
}

/* GeometryCollection */ 

template<GeometryCollection2DConcept GeometryCollection, Geometry2DConcept Geometry, PrecisionPolicyConcept Policy>
auto crosses_impl(const GeometryCollection &gc,
                  const Geometry &geom,
                  const Policy &policy,
                  collection_tag,
                  geometry_tag_t<Geometry>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
            return crosses(arg.get(), geom, policy);
            }, item);

        if (hit) return true;
    }

    return false;
}

template<Geometry2DConcept Geometry, Point2DConcept Point, PrecisionPolicyConcept Policy>
auto crosses_impl(const Geometry &geom,
                  const GeometryCollection<Point> &gc,
                  const Policy &policy,
                  geometry_tag_t<Geometry>,
                  collection_tag) -> bool
{
    return crosses_impl(gc, geom, policy, collection_tag{}, geometry_tag_t<Geometry>{});
}

} // namespace detail

template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto crosses(const G1 &geom1, const G2 &geom2) -> bool
{
    using P = typename geometry_traits<G1>::point_type;
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return crosses(geom1, geom2, policy);
}

template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto crosses(const G1 &geom1, 
                       const G2 &geom2,
                       const Policy &policy) -> bool
{
    using P1 = typename geometry_traits<G1>::point_type;
    using P2 = typename geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if (geom1.isEmpty() || geom2.isEmpty())
        return false;

    if constexpr (MultiGeometryConcept<G1> && !GeometryCollectionConcept<G1>) {
        for (const auto &a : geom1) {
            if (crosses(a, geom2, policy)) return true;
        }
        return false;
    } else if constexpr (MultiGeometryConcept<G2> && !GeometryCollectionConcept<G2>) {
        for (const auto &b : geom2) {
            if (crosses(geom1, b, policy)) return true;
        }
        return false;
    } else {
        return detail::crosses_impl(geom1, geom2, policy, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
    }
}

template<PointConcept P, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto crosses(const Segment<P> &segment,
             const LinearRing<P> &ring,
             const Policy &policy) -> bool
{
    return detail::crosses_impl(segment, ring, policy, segment_tag{}, linestring_tag{});
}

template<PointConcept P>
[[nodiscard]]
auto crosses(const Segment<P> &segment,
             const LinearRing<P> &ring) -> bool
{
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return crosses(segment, ring, policy);
}

} // namespace tl