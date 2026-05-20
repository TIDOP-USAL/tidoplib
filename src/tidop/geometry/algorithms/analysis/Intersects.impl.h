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
auto intersects_impl(const P1 &p1,
                     const P2 &p2,
                     const Policy &policy,
                     point_tag, 
                     point_tag) -> bool
{
    const auto pt1 = policy.toKernelPoint<Dimension::dim2>(p1);
    const auto pt2 = policy.toKernelPoint<Dimension::dim2>(p2);

    return TopologyKernel::equals(pt1, pt2);
}


/* Segment - Segment */

template<Segment2DConcept S1, Segment2DConcept S2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto intersects_impl(const S1 &segment1,
                     const S2 &segment2,
                     const Policy &policy,
                     segment_tag,
                     segment_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment1.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment1.pt2());
    const auto q1 = policy.toKernelPoint<Dimension::dim2>(segment2.pt1());
    const auto q2 = policy.toKernelPoint<Dimension::dim2>(segment2.pt2());

    if (TopologyKernel::equals(q1, q2)) {
        return intersects(segment1, segment2.start(), policy);
    }

    if (TopologyKernel::equals(p1, p2)) {
        return intersects(segment1.start(), segment2, policy);
    }

    return TopologyKernel::intersectionType(p1, p2, q1, q2) != TopologyKernel::IntersectionType::None;
}


/* Point - Segment */

template<typename Point_t, typename Policy_t>
[[nodiscard]]
auto intersects_impl(const Point_t &point, 
                     const Segment<Point_t> &segment,
                     const Policy_t &policy,
                     point_tag, 
                     segment_tag) -> bool
{
    const auto pt = policy.toKernelPoint<Dimension::dim2>(point);
    const auto segment_ini = policy.toKernelPoint<Dimension::dim2>(segment.pt1());
    const auto segment_end = policy.toKernelPoint<Dimension::dim2>(segment.pt2());

    if (TopologyKernel::equals(segment_ini, segment_end))
        return intersects(pt, segment_ini);

    return TopologyKernel::pointOnSegmentInclusive(segment_ini, segment_end, pt);
}



/* Point - LineString */

//TODO: Crear clase SegmentView
template<typename Point_t, typename Policy_t>
auto intersects_impl(const Point_t &pt,
                     const LineString<Point_t> &lineString,
                     const Policy_t &policy,
                     point_tag, 
                     linestring_tag) -> bool 
{
    return locatePointOnLineString(lineString, pt, policy) != Location::Exterior;
}

// LineString - Segment
template<typename Point_t, typename Policy_t>
auto intersects_impl(const LineString<Point_t> &line, 
                     const Segment<Point_t> &seg,
                     const Policy_t &policy,
                     linestring_tag, 
                     segment_tag) -> bool
{
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        if (intersects(Segment<Point_t>(line[i], line[i + 1]), seg, policy)) return true;
    }
    return false;
}

/* LineString - LineString */

template<typename Point_t, typename Policy_t>
auto intersects_impl(const LineString<Point_t> &l1, 
                     const LineString<Point_t> &l2,
                     const Policy_t &policy,
                     linestring_tag, 
                     linestring_tag) -> bool 
{
    if (!intersects(envelope(l1), envelope(l2), policy)) return false;

    for (size_t i = 0; i + 1 < l1.size(); ++i) {
        Segment<Point_t> s1(l1[i], l1[i + 1]);
        for (size_t j = 0; j + 1 < l2.size(); ++j) {
            if (intersects(s1, Segment<Point_t>(l2[j], l2[j + 1]), policy)) return true;
        }
    }
    return false;
}

/* Point - Polygon */

template<typename Point_t, typename Policy_t>
auto intersects_impl(const Point_t &point,
                     const Polygon<Point_t> &polygon,
                     const Policy_t &policy,
                     point_tag, 
                     polygon_tag) -> bool 
{
    return locatePointInPolygon(polygon, point, policy) != Location::Exterior;
}

template<typename Point_t, typename Policy_t>
auto intersects_impl(const Segment<Point_t> &segment,
                     const Polygon<Point_t> &polygon,
                     const Policy_t &policy,
                     segment_tag, 
                     polygon_tag) -> bool
{
    auto check_ring_intersection = [&](const auto &ring) {
        for (size_t i = 0; i < ring.size(); ++i) {
            size_t j = (i + 1) % ring.size();
            Segment<Point_t> edge(ring[i], ring[j]);
            if (intersects(segment, edge, policy)) return true;
        }
        return false;
    };

    // Intersecta el anillo exterior
    if (check_ring_intersection(polygon.outer())) return true;

    // Intersecta algún hueco
    for (const auto &hole : polygon.inners()) {
        if (check_ring_intersection(hole)) return true;
    }

    // Algún punto del segmento dentro del polígono (sin contar huecos)
    auto check_point_inside = [&](const Point_t &pt) {
        return locatePointInPolygon(polygon, pt, policy) == Location::Interior;
    };

    if (check_point_inside(segment.pt1())) return true;
    if (check_point_inside(segment.pt2())) return true;

    // Punto medio (por si ambos extremos caen dentro de un hueco)
    Point_t mid((segment.pt1().x() + segment.pt2().x()) * 0.5,
                (segment.pt1().y() + segment.pt2().y()) * 0.5);

    if (check_point_inside(mid)) return true;

    return false;
}

/* LineString - Polygon */
template<typename Point_t, typename Policy_t>
auto intersects_impl(const LineString<Point_t> &line,
                     const Polygon<Point_t> &polygon,
                     const Policy_t &policy,
                     linestring_tag,
                     polygon_tag) -> bool
{
    if (line.isEmpty()) return false;

    // 1️ - Algún punto dentro o en borde del polígono
    for (const auto &pt : line) {
        if (intersects(pt, polygon, policy)) return true;
    }

    // 2️ - Algún segmento intersecta la frontera del polígono
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        Segment<Point_t> seg(line[i], line[i + 1]);

        if (intersects(seg, polygon.outer(), policy)) return true;
        for (const auto &hole : polygon.inners()) {
            if (intersects(seg, hole, policy)) return true;
        }
    }

    return false;
}

/* Polygon - Polygon */

template<typename Point_t, typename Policy_t>
auto intersects_impl(const Polygon<Point_t> &poly1, 
                     const Polygon<Point_t> &poly2,
                     const Policy_t &policy,
                     polygon_tag, 
                     polygon_tag) -> bool
{
    // 1. Check if any vertex of poly1 is inside poly2
    for (const auto &point : poly1.outer()) {
        if (intersects(point, poly2, policy)) {
            return true;
        }
    }

    // 2. Check if any vertex of poly2 is inside poly1
    for (const auto &point : poly2.outer()) {
        if (intersects(point, poly1, policy)) {
            return true;
        }
    }

    // 3. Check if boundaries intersect
    auto check_rings_intersect = [](const auto &ring1, const auto &ring2, const auto &policy) {
        for (size_t i = 0; i < ring1.size(); ++i) {
            size_t j = (i + 1) % ring1.size();
            Segment<Point_t> edge1(ring1[i], ring1[j]);

            if (intersects(edge1, ring2, policy)) return true;
        }
        return false;
    };

    // Check all ring combinations
    if (check_rings_intersect(poly1.outer(), poly2.outer(), policy)) return true;

    for (const auto &hole : poly2.inners()) {
        if (check_rings_intersect(poly1.outer(), hole, policy)) return true;
    }

    for (const auto &hole : poly1.inners()) {
        if (check_rings_intersect(poly2.outer(), hole, policy)) return true;
    }

    for (const auto &hole1 : poly1.inners()) {
        for (const auto &hole2 : poly2.inners()) {
            if (check_rings_intersect(hole1, hole2, policy)) return true;
        }
    }

    return false;
}

/* GeometryCollection with any geometry */
template<typename Point_t, typename Geometry_t, typename Policy_t>
auto intersects_impl(const GeometryCollection<Point_t> &gc, 
                     const Geometry_t &geom,
                     const Policy_t &policy,
                     collection_tag, 
                     geometry_tag_t<Geometry_t>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
                return intersects(arg.get(), geom, policy);
            }, item);

        if (hit) return true;
    }

    return false;
}

/* BoundingBox - BoundingBox */
template<typename Point_t, typename Policy_t>
auto intersects_impl(const BoundingBox<Point_t> &bbox1,
                     const BoundingBox<Point_t> &bbox2,
                     const Policy_t &policy,
                     bbox_tag, 
                     bbox_tag) -> bool
{
    if (bbox1.isEmpty() || bbox2.isEmpty()) return false;

    for (size_t i = 0; i < point_traits<Point_t>::spatial_dims; ++i) {
        if (bbox1.max()[i] < bbox2.min()[i] || 
            bbox1.min()[i] > bbox2.max()[i]) {
            return false;
        }
    }
    return true;
}

/* BoundingBox - Point */
template<typename Point_t, typename Policy_t>
auto intersects_impl(const BoundingBox<Point_t> &bbox, 
                     const Point_t &point,
                     const Policy_t &policy,
                     bbox_tag, 
                     point_tag) -> bool
{
    for (size_t i = 0; i < point_traits<Point_t>::spatial_dims; ++i) {
        if (point[i] < bbox.min()[i] || point[i] > bbox.max()[i]) {
            return false;
        }
    }
    return true;
}

// Función espejo
template<typename G1, typename G2, typename Policy_t, typename Tag1, typename Tag2>
auto intersects_impl(const G1 &geom1,
                     const G2 &geom2,
                     const Policy_t &policy, 
                     Tag1 t1,
                     Tag2 t2) -> double
{
    return intersects_impl(geom2, geom1, policy, t2, t1);
}


} // namespace detail



template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto intersects(const G1 &geom1,
                const G2 &geom2,
                const Policy &policy) -> bool
{
    using P1 = typename geometry_traits<G1>::point_type;
    using P2 = typename geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    auto dispatch = [&] {
        using tag1 = geometry_tag_t<G1>;
        using tag2 = geometry_tag_t<G2>;
        return detail::intersects_impl(geom1, geom2, policy, tag1{}, tag2{});
    };

    if constexpr (GeometryCollectionConcept<G1> || GeometryCollectionConcept<G2>) {
        return dispatch();
    } else if constexpr (MultiGeometryConcept<G1>) {
        for (const auto &a : geom1) {
            if (intersects(a, geom2, policy)) return true;
        }
        return false;
    } else if constexpr (MultiGeometryConcept<G2>) {
        for (const auto &b : geom2) {
            if (intersects(geom1, b, policy)) return true;
        }
        return false;
    } else {
        return dispatch();
    }
}


template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
auto intersects(const G1 &geom1, const G2 &geom2) -> bool
{
    using P = typename geometry_traits<G1>::point_type;
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return intersects(geom1, geom2, policy);
}




template<PointConcept P, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto intersects(const Segment<P> &seg,
                const LinearRing<P> &ring,
                const Policy &policy)
{

    if (ring.size() < 2) return false;

    // 1️ - Chequear si alguno de los extremos está en el interior o en el borde
    if (locatePointInRing(ring, seg.pt1(), policy) == Location::Boundary ||
        locatePointInRing(ring, seg.pt2(), policy) == Location::Boundary) {
        return true;
    }

    // 2️ - Revisar cada arista del anillo
    for (size_t i = 0; i < ring.size(); ++i) {
        size_t j = (i + 1) % ring.size();
        Segment<P> edge(ring[i], ring[j]);

        if (intersects(seg, edge, policy)) {
            return true;
        }
    }

    return false;
}

template<PointConcept P>
[[nodiscard]]
auto intersects(const Segment<P> &seg,
                const LinearRing<P> &ring)
{
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return intersects(seg, ring, policy);
}

} // namespace tl
