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
auto contains_impl(const P1 &pt1,
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
auto contains_impl(const S1 &segment1,
                   const S2 &segment2,
                   const Policy &policy,
                   segment_tag, 
                   segment_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment1.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment1.pt2());
    const auto q1 = policy.toKernelPoint<Dimension::dim2>(segment2.pt1());
    const auto q2 = policy.toKernelPoint<Dimension::dim2>(segment2.pt2());

    using KernelPoint = Policy::KernelPoint2D;

    if (TopologyKernel::equals(q1, q2)) {
        return contains(Segment<KernelPoint>(p1, p2), q1);
    }

    if (TopologyKernel::equals(p1, p2)) {
        return contains(p1, Segment<KernelPoint>(q1, q2));
    }

    if ((TopologyKernel::equals(p1, q1) && TopologyKernel::equals(p2, q2)) ||
        (TopologyKernel::equals(p1, q2) && TopologyKernel::equals(p2, q1))) {
        return true;
    }

    if (TopologyKernel::orientation(p1, p2, q1) != TopologyKernel::WindingOrder::Collinear ||
        TopologyKernel::orientation(p1, p2, q2) != TopologyKernel::WindingOrder::Collinear) {
        return false;
    }

    if (!TopologyKernel::isBetween(p1, p2, q1) ||
        !TopologyKernel::isBetween(p1, p2, q2)) {
        return false;
    }

    return true;
}


/* LineString – LineString */

template<typename LS1, typename LS2, typename Policy_t>
auto contains_impl(const LS1 &container,
                   const LS2 &containee,
                   const Policy_t &policy,
                   linestring_tag,
                   linestring_tag) -> bool
{
    if (container.size() < 2) return false;

    for (std::size_t i = 0; i + 1 < containee.size(); ++i) {
        using Point_t = typename geometry_traits<LS2>::point_type;
        Segment<Point_t> current_seg(containee[i], containee[i + 1]);

        if (!contains(container, current_seg, policy)) {
            return false;
        }
    }

    return true;
}

// Polygon-Polygon (un polígono contiene a otro)
template<typename Polygon1, typename Polygon2, typename Policy_t>
auto contains_impl(const Polygon1 &container,
                   const Polygon2 &containee,
                   const Policy_t &policy,
                   polygon_tag,
                   polygon_tag) -> bool
{
    using P1 = typename geometry_traits<Polygon1>::point_type;
    using P2 = typename geometry_traits<Polygon2>::point_type;
    bool has_point_on_container_surface = false;

    if (container.outer().size() == containee.outer().size() &&
        container.inners().empty() &&
        containee.inners().empty()) {
        bool same_outer = true;
        for (size_t i = 0; i < container.outer().size(); ++i) {
            if (!(container.outer()[i] == containee.outer()[i])) {
                same_outer = false;
                break;
            }
        }

        if (same_outer) {
            return false;
        }
    }

    // Todos los puntos del containee deben estar dentro del container (interior)
    for (const auto &pt : containee.outer()) {
        if (locatePointInRing(container.outer(), pt, policy) == Location::Exterior) {
            return false;
        }
        if (locatePointInPolygon(container, pt, policy) != Location::Exterior) {
            has_point_on_container_surface = true;
        }
    }

    if (!has_point_on_container_surface) {
        return false;
    }

    // Ningún segmento del containee intersecta los huecos del container
    for (size_t i = 0; i < containee.outer().size(); ++i) {
        size_t j = (i + 1) % containee.outer().size();
        Segment<P2> seg(containee.outer()[i], containee.outer()[j]);
        for (const auto &hole : container.inners()) {
            if (crosses(seg, hole, policy)) {
                return false;
            }
        }
    }

    // Los huecos del containee deben estar totalmente dentro de los huecos del container o fuera completamente
    for (const auto &hole : containee.inners()) {
        for (const auto &pt : hole) {
            if (!pointInAnyHole(pt, container, policy) && locatePointInPolygon(container, pt, policy) != Location::Interior) {
                return false;
            }
        }
    }

    for (const auto &container_hole : container.inners()) {
        bool same_hole = false;
        for (const auto &containee_hole : containee.inners()) {
            if (container_hole.size() != containee_hole.size()) {
                continue;
            }

            same_hole = true;
            for (size_t i = 0; i < container_hole.size(); ++i) {
                if (!(container_hole[i] == containee_hole[i])) {
                    same_hole = false;
                    break;
                }
            }

            if (same_hole) {
                break;
            }
        }

        if (same_hole || container_hole.isEmpty()) {
            continue;
        }

        double x = 0.;
        double y = 0.;
        size_t count = container_hole.size();
        if (count > 1 && container_hole.front() == container_hole.back()) {
            --count;
        }

        for (size_t i = 0; i < count; ++i) {
            x += container_hole[i].x();
            y += container_hole[i].y();
        }

        P1 hole_point(x / static_cast<double>(count), y / static_cast<double>(count));
        if (locatePointInRing(containee.outer(), hole_point, policy) == Location::Interior) {
            return false;
        }
    }

    return true;
}

template<typename Segment_t, typename Point_t, typename Policy_t>
auto contains_impl(const Segment_t &segment,
                   const Point_t &point,
                   const Policy_t &policy,
                   segment_tag,
                   point_tag) -> bool
{
    const auto p = policy.toKernelPoint<Dimension::dim2>(point);
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment.pt2());

    if (TopologyKernel::equals(p1, p2)){
        return contains(p1, p);
    }

    if (!TopologyKernel::pointOnSegment(p1, p2, p))
        return false;

    if (TopologyKernel::equals(p, p1) ||
        TopologyKernel::equals(p, p2))
        return false;

    return true;
}

// Point-Segment (caso de segmento degenerado)
template<typename Point_t, typename Segment_t, typename Policy_t>
auto contains_impl(const Point_t &point, 
                   const Segment_t &segment,
                   const Policy_t &policy,
                   point_tag,
                   segment_tag) -> bool
{
    const auto p = policy.toKernelPoint<Dimension::dim2>(point);
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment.pt2());

    if (TopologyKernel::equals(p1, p2))
        return contains(p, p1);
    else
        return false; // Un punto no puede contener un segmento no degenerado
}

template<typename Point_t, typename Policy_t>
auto contains_impl(const LineString<Point_t> &line,
                   const Point_t &pt,
                   const Policy_t &policy,
                   linestring_tag,
                   point_tag) -> bool
{
    auto loc = locatePointOnLineString(line, pt, policy);

    return loc == Location::Interior;
}

template<typename Point_t, typename Policy_t>
auto contains_impl(const LineString<Point_t> &line,
                   const Segment<Point_t> &segment,
                   const Policy_t &policy,
                   linestring_tag,
                   segment_tag) -> bool
{
    auto loc1 = locatePointOnLineString(line, segment.pt1(), policy);
    auto loc2 = locatePointOnLineString(line, segment.pt2(), policy);

    if (loc1 == Location::Exterior || loc2 == Location::Exterior)
        return false;

    const auto p = policy.toKernelPoint<Dimension::dim2>(segment.pt1());
    const auto q = policy.toKernelPoint<Dimension::dim2>(segment.pt2());

    const std::size_t n = line.size();

    for (size_t i = 0; i + 1 < n; ++i) {
        auto a = policy.toKernelPoint<Dimension::dim2>(line[i]);
        auto b = policy.toKernelPoint<Dimension::dim2>(line[i + 1]);

        // p debe estar en este edge
        if (!(TopologyKernel::orientation(a, b, p) == TopologyKernel::WindingOrder::Collinear &&
            TopologyKernel::isBetween(a, b, p)))
            continue;

        // avanzar por edges colineales
        auto start = a;
        auto end = b;

        size_t j = i + 1;

        while (j + 1 < n) {
            auto c = policy.toKernelPoint<Dimension::dim2>(line[j + 1]);

            if (TopologyKernel::orientation(start, end, c) != TopologyKernel::WindingOrder::Collinear)
                break;

            end = c;
            ++j;
        }

        // comprobar si q está dentro de la cadena colineal
        if (TopologyKernel::orientation(start, end, q) == TopologyKernel::WindingOrder::Collinear &&
            TopologyKernel::isBetween(start, end, q))
            return true;

    }

    return false;
}

template<typename Point_t, typename Policy_t>
auto contains_impl(const Segment<Point_t> &segment,
                   const LineString<Point_t> &line,
                   const Policy_t &policy,
                   segment_tag,
                   linestring_tag) -> bool
{
    for (const auto &pt : line) {
        if (locatePointOnSegment(segment, pt, policy) != Location::Interior)
            return false;
    }

    return true;
}

// BoundingBox-Point
template<typename Point_t, typename Policy_t>
auto contains_impl(const BoundingBox<Point_t> &box,
                   const Point_t &point, 
                   const Policy_t &policy,
                   bbox_tag,
                   point_tag)
{
    const auto p = policy.toKernelPoint<Dimension::dim2>(point);
    const auto box_min = policy.toKernelPoint<Dimension::dim2>(box.min());
    const auto box_max = policy.toKernelPoint<Dimension::dim2>(box.max());

    return TopologyKernel::isBetween(box_min, box_max, p);
}

// Polygon-Point
template<typename Polygon_t, typename Point_t, typename Policy_t>
auto contains_impl(const Polygon_t &polygon,
                   const Point_t &pt, 
                   const Policy_t &policy,
                   polygon_tag,
                   point_tag) -> bool
{
    return locatePointInPolygon(polygon, pt, policy) == Location::Interior;
}

template<typename Polygon_t, typename Segment_t, PrecisionPolicyConcept Policy>
auto contains_impl(const Polygon_t &polygon,
                   const Segment_t &segment,
                   const Policy &policy,
                   polygon_tag,
                   segment_tag) -> bool
{
    auto loc1 = locatePointInPolygon(polygon, segment.pt1(), policy);
    auto loc2 = locatePointInPolygon(polygon, segment.pt2(), policy);

    if (loc1 != Location::Interior ||
        loc2 != Location::Interior) {
        return false;
    }

    if (loc1 == Location::Boundary && loc2 == Location::Boundary) {
        auto mid = segment.midPoint();
        if (locatePointInPolygon(polygon, mid, policy) != Location::Interior)
            return false;
    }

    if (crosses(segment, polygon.outer(), policy))
        return false;

    for (const auto &hole : polygon.inners()) {
        if (crosses(segment, hole, policy))
            return false;
    }

    return true;
}

// Polygon-LineString (todos los puntos de la línea están dentro del polígono, incluyendo el borde)
template<typename Point_t, typename Policy_t>
auto contains_impl(const Polygon<Point_t> &polygon, 
                   const LineString<Point_t> &line,
                   const Policy_t &policy,
                   polygon_tag, 
                   linestring_tag) -> bool
{
    // Todos los puntos de la línea deben estar en el interior (no en borde)
    for (const auto &pt : line) {
        if (locatePointInPolygon(polygon, pt, policy) != Location::Interior) {
            return false;
        }

    }

    for (size_t i = 0; i + 1 < line.size(); ++i) {

        Segment<Point_t> segment(line[i], line[i + 1]);

        if (crosses(segment, polygon.outer(), policy))
            return false;

        for (const auto &hole : polygon.inners()) {
            if (crosses(segment, hole, policy)) {
                return false;
            }
        }
    }

    // Alternativa. Faltaría por controlar lineas con un solo punto
    //for (size_t i = 0; i + 1 < line.size(); ++i) {

    //    Segment<Point_t> segment(line[i], line[i + 1]);

    //    if (!contains_impl(polygon, segment, policy, polygon_tag{}, segment_tag{}))
    //        return false;
    //}
    
    return true;
}


// MultiPolygon - Point (un multipolígono contiene un punto)
template<typename Point_t, typename Policy_t>
auto contains_impl(const MultiPolygon<Point_t> &multiPolygon,
                   const Point_t &pt, 
                   const Policy_t &policy,
                   multipolygon_tag, 
                   point_tag) -> bool
{
    for (const auto &polygon : multiPolygon) {
        if (contains(polygon, pt, policy))
            return true;
    }

    return false;
}

// MultiPolygon - Polygon
template<typename Point_t, typename Policy_t>
auto contains_impl(const MultiPolygon<Point_t> &multiPolygon, 
                   const Polygon<Point_t> &polygon,
                   const Policy_t &policy,
                   multipolygon_tag, 
                   polygon_tag) -> bool
{
    for (const auto &containerPoly : multiPolygon) {
        if (!contains(containerPoly, polygon, policy)) {
            continue;
        }

        bool strictly_inside = true;
        for (const auto &pt : polygon.outer()) {
            if (locatePointInPolygon(containerPoly, pt, policy) != Location::Interior) {
                strictly_inside = false;
                break;
            }
        }

        if (strictly_inside) {
            return true;
        }
    }
    return false;
}

/* GeometryCollection with any geometry */
template<typename Point_t, typename Geometry_t, typename Policy_t>
auto contains_impl(const GeometryCollection<Point_t> &gc,
                   const Geometry_t &geom,
                   const Policy_t &policy,
                   collection_tag,
                   geometry_tag_t<Geometry_t>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
            return contains(arg.get(), geom, policy);
            }, item);

        if (hit) return true;
    }

    return false;
}

template<typename Point_t, typename Policy_t>
auto contains_impl(const Polygon<Point_t> &poly,
                   const GeometryCollection<Point_t> &gc,
                   const Policy_t &policy,
                   polygon_tag,
                   collection_tag) -> bool
{
    if (poly.empty() || gc.empty())
        return false;

    for (const auto &element : gc) {
        bool contained = std::visit(
            [&](const auto &wrapped) {
                const auto &geom = wrapped.get();
                return contains(poly, geom, policy);
            },
            element
        );

        if (!contained)
            return false;
    }

    return true;
}

template<typename G1, typename G2, typename Policy_t, typename Tag1, typename Tag2>
[[nodiscard]]
constexpr auto contains_impl(const G1 &, const G2 &, const Policy_t &, Tag1, Tag2) -> bool
{
    return false;
}


} // namespace detail


template<Geometry2DConcept G1, Geometry2DConcept G2>
auto contains(const G1 &geom1, const G2 &geom2) -> bool
{
    using P = typename geometry_traits<G1>::point_type;
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return contains(geom1, geom2, policy);
}


template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
constexpr auto contains(const G1 &geom1,
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

    if constexpr (std::is_same_v<geometry_tag_t<G1>, multipolygon_tag> &&
                  std::is_same_v<geometry_tag_t<G2>, polygon_tag>) {
        return detail::contains_impl(geom1, geom2, policy, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
    } else if constexpr (MultiGeometryConcept<G1> && !GeometryCollectionConcept<G1>) {
        return std::any_of(geom1.begin(), geom1.end(),
            [&](const auto &part) {
                return contains(part, geom2, policy);
            });
    } else if constexpr (MultiGeometryConcept<G2> && !GeometryCollectionConcept<G2>) {
        for (const auto &part : geom2) {
            if (!contains(geom1, part, policy))
                return false;
        }
        return true;
    } else {
        return detail::contains_impl(geom1, geom2, policy, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
    }
}


} // namespace tl
