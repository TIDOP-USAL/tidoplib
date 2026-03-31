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
constexpr auto within_impl(const P1 &pt1,
                           const P2 &pt2,
                           const Policy &policy,
                           point_tag,
                           point_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(pt1);
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(pt2);

    return TopologyKernel::equals(p1, p2);
}


/* Segment – Segment */

template<Segment2DConcept S1, Segment2DConcept S2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto within_impl(const S1 &seg1, 
                           const S2 &seg2,
                           const Policy &policy,
                           segment_tag, 
                           segment_tag) -> bool
{
    return contains(seg2, seg1, policy);
}

template<LineString2DConcept LS1, LineString2DConcept LS2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto within_impl(const LS1 &ls1,
                           const LS2 &ls2,
                           const Policy &policy,
                           linestring_tag,
                           linestring_tag) -> bool
{
    return contains(ls2, ls1, policy);
}

template<Polygon2DConcept Poly1, Polygon2DConcept Poly2, PrecisionPolicyConcept Policy>
[[nodiscard]] 
constexpr auto within_impl(const Poly1 &p1,
                           const Poly2 &p2,
                           const Policy &policy,
                           polygon_tag,
                           polygon_tag) -> bool
{
//    return contains(p2, p1, tolerance);
    // ningún punto del exterior
    for (const auto &pt : p1.outer())
        if (locatePointInPolygon(p2, pt, policy) == Location::Exterior)
            return false;

    // comprobar un punto interior real de p1
    auto interior_point = representativePoint(p1);

    if (locatePointInPolygon(p2, interior_point, policy) != Location::Interior)
        return false;

    return true;
}

// Point - MultiPoint
//template<typename P, typename MP>
//[[nodiscard]]
//constexpr auto within_impl(const P &point, 
//                           const MP &multipoint,
//                           double tolerance,
//                           point_tag, 
//                           multipoint_tag) -> bool
//{
//    for (const auto &p : multipoint)
//        if (equalsExact(point, p, tolerance))
//            return true;
//    return false;
//}

// Point - Segment
template<Point2DConcept Point, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
constexpr bool within_impl(const Point &point,
                           const Segment &segment,
                           const Policy &policy,
                           point_tag,
                           segment_tag)
{
    return contains(segment, point, policy);
}

// Point - LineString
template<Point2DConcept Point, LineString2DConcept LineString, PrecisionPolicyConcept Policy>
constexpr bool within_impl(const Point &point,
                           const LineString &lineString,
                           const Policy &policy,
                           point_tag,
                           linestring_tag)
{
    return locatePointOnLineString(lineString, point, policy) == Location::Interior;
}

template<Point2DConcept Point, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
constexpr bool within_impl(const Point &point,
                           const Polygon &polygon,
                           const Policy &policy,
                           point_tag,
                           polygon_tag)
{
    return locatePointInPolygon(polygon, point, policy) == Location::Interior;
}

template< LineString2DConcept LineString, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
auto within_impl(const LineString &lineString,
                 const Polygon &polygon,
                 const Policy &policy,
                 linestring_tag,
                 polygon_tag) -> bool
{
    // ningún vértice puede estar en el exterior
    for (const auto &point : lineString)
        if (locatePointInPolygon(polygon, point, policy) == Location::Exterior)
            return false;

    // comprobar que el interior del LS esté en el interior del polígono
    // basta comprobar un punto interior de cada segmento
    bool hasInteriorPointInside = false;

    for (size_t i = 0; i + 1 < lineString.size(); ++i) {

        Segment seg(lineString[i], lineString[i + 1]);

        auto mid = seg.midPoint();
        auto loc = locatePointInPolygon(polygon, mid, policy);

        if (loc == Location::Exterior)
            return false;

        if (loc == Location::Interior)
            hasInteriorPointInside = true;
    }
    return hasInteriorPointInside;
}

template<Segment2DConcept Segment, LineString2DConcept LineString, PrecisionPolicyConcept Policy>
constexpr bool within_impl(const Segment &segment,
                           const LineString &lineString,
                           const Policy &policy,
                           segment_tag,
                           linestring_tag)
{
    for (size_t i = 0; i + 1 < lineString.size(); ++i) {
        Segment candidate(lineString[i], lineString[i + 1]);
        if (contains(candidate, segment, policy))
            return true;
    }
    return false;
}

template<Segment2DConcept Segment, Polygon2DConcept Polygon, PrecisionPolicyConcept Policy>
constexpr bool within_impl(const Segment &segment,
                           const Polygon &polygon,
                           const Policy &policy,
                           segment_tag,
                           polygon_tag)
{
    if (locatePointInPolygon(polygon, segment.start(), policy) == Location::Exterior)
        return false;

    if (locatePointInPolygon(polygon, segment.end(), policy) == Location::Exterior)
        return false;

    // el segmento no puede cruzar al exterior
    if (intersects(segment, boundary(polygon), policy)) // o equivalente
        return false;

    // el interior del segmento debe intersectar el interior del polígono
    // Basta con que algún punto interior esté dentro.
    auto mid = segment.midPoint();
    if (locatePointInPolygon(polygon, mid, policy) != Location::Interior)
        return false;

    return true;
}

template<LineString2DConcept LineString, Segment2DConcept Segment, PrecisionPolicyConcept Policy>
constexpr bool within_impl(const LineString &lineString,
                           const Segment &segment,
                           const Policy &policy,
                           linestring_tag,
                           segment_tag)
{
    if (lineString.size() != 2)
        return false;

    Segment candidate(lineString[0], lineString[1]);
    return contains(segment, candidate, policy);
}

template<GeometryCollection2DConcept GeometryCollection, Geometry2DConcept Geometry, PrecisionPolicyConcept Policy>
bool within_impl(const GeometryCollection &collection,
                 const Geometry &geom,
                 const Policy &policy,
                 collection_tag, 
                 geometry_tag_t<Geometry>)
{
    for (const auto &item : collection) {
        if (!std::visit([&](auto &&arg) {
                return within(arg.get(), geom, policy);
            }, item)) 
            return false;
    }

    return true;
}

// Segunda geometría es colección
template<typename G1, typename GC, PrecisionPolicyConcept Policy>
bool within_impl(const G1 &geom,
                 const GC &collection,
                 const Policy &policy,
                 geometry_tag_t<G1>, 
                 collection_tag) 
{
    for (const auto &item : collection) {
        if(std::visit([&](auto &&arg) {
            return within(geom, arg.get(), policy);
            }, item)) return true;
    }

    return false;
}

template<typename G1, typename G2, PrecisionPolicyConcept Policy, typename Tag1, typename Tag2>
[[nodiscard]]
constexpr auto within_impl(const G1 &, const G2 &, const Policy &, Tag1, Tag2) -> bool
{
    return false;
}

} // namespace detail


template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto within(const G1 &geom1, const G2 &geom2) -> bool
{
    using P = geometry_traits<G1>::point_type;
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return within(geom1, geom2, policy);
}

template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto within(const G1 &geom1,
                      const G2 &geom2,
                      const Policy &policy) -> bool
{
    using P1 = typename geometry_traits<G1>::point_type;
    using P2 = typename geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if (geom1.isEmpty() || geom2.isEmpty()) return false;

    if constexpr (MultiGeometryConcept<G1> && !GeometryCollectionConcept<G1>) {
        for (const auto &a : geom1) {
            if (!within(a, geom2, policy))
                return false;
        }
        return true;
    } else if constexpr (MultiGeometryConcept<G2> && !GeometryCollectionConcept<G2>) {
        for (const auto &b : geom2) {
            if (within(geom1, b, policy)) return true;
        }
        return false;
    } else {
        using tag1 = geometry_tag_t<G1>;
        using tag2 = geometry_tag_t<G2>;
        return detail::within_impl(geom1, geom2, policy, tag1{}, tag2{});
    }
}

} // namespace tl