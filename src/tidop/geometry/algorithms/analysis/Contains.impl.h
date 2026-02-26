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

// Point-Point (un punto contiene a otro si son iguales)
template<typename Point_t>
auto contains_impl(const Point_t &pt1,
                   const Point_t &pt2,
                   double tolerance,
                   point_tag,
                   point_tag)
{
    return equalsExact(pt1, pt2, tolerance);
}

template<typename Point_t>
auto contains_impl(const Segment<Point_t> &seg1,
                   const Segment<Point_t> &seg2,
                   double tolerance,
                   segment_tag, 
                   segment_tag) -> bool
{
    if (equalsExact(seg1, seg2, tolerance)) return true;

    const auto &p1 = seg1.pt1();
    const auto &p2 = seg1.pt2();
    const auto &q1 = seg2.pt1();
    const auto &q2 = seg2.pt2();

    if (orientation(p1, p2, q1) != WindingOrder::Colinear ||
        orientation(p1, p2, q2) != WindingOrder::Colinear)
        return false;

    if (!isBetween(p1, q1, p2) ||
        !isBetween(p1, q2, p2))
        return false;

    if (q1 == q2) {
        // Un segmento no contiene a sus propios extremos
        if (equalsExact(q1, p1, tolerance) || equalsExact(q1, p2, tolerance)) return false;
    } else if (p1 == p2) {
        if (equalsExact(p1, q1, tolerance) || equalsExact(p1, q2, tolerance)) return false;
    }

    return true;
}

// LineString - LineString
template<typename Point_t>
auto contains_impl(const LineString<Point_t> &container,
                   const LineString<Point_t> &containee,
                   double tolerance,
                   linestring_tag,
                   linestring_tag) -> bool
{
    if (equalsExact(container, containee, tolerance))
        return true;

    // Todos los puntos del containee deben estar sobre container
    for (const auto &pt : containee) {
        if (locatePointOnLineString(container, pt, tolerance) == Location::Exterior)
            return false;
    }

    // Todos los segmentos del containee deben estar contenidos
    for (size_t i = 0; i + 1 < containee.size(); ++i) {
        Segment<Point_t> seg(containee[i], containee[i + 1]);

        bool found = false;

        for (size_t j = 0; j + 1 < container.size(); ++j) {
            Segment<Point_t> containerSeg(container[j], container[j + 1]);

            if (contains(containerSeg, seg, tolerance)) {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

// Polygon-Polygon (un polígono contiene a otro)
template<typename Point_t>
auto contains_impl(const Polygon<Point_t> &container,
                   const Polygon<Point_t> &containee,
                   double tolerance,
                   polygon_tag,
                   polygon_tag) -> bool
{
    // Todos los puntos del containee deben estar dentro del container (interior)
    for (const auto &pt : containee.outer()) {
        if (locatePointInPolygon(container, pt) != Location::Interior) {
            return false;
        }
    }

    // Ningún segmento del containee intersecta los huecos del container
    for (size_t i = 0; i < containee.outer().size(); ++i) {
        size_t j = (i + 1) % containee.outer().size();
        Segment<Point_t> seg(containee.outer()[i], containee.outer()[j]);
        for (const auto &hole : container.inners()) {
            if (intersects(seg, hole)) {
                return false;
            }
        }
    }

    // Los huecos del containee deben estar totalmente dentro de los huecos del container o fuera completamente
    for (const auto &hole : containee.inners()) {
        for (const auto &pt : hole) {
            if (!pointInAnyHole(pt, container) && locatePointInPolygon(container, pt) != Location::Interior) {
                return false;
            }
        }
    }

    return true;
}


// BoundingBox-Point
template<typename Point_t>
auto contains_impl(const BoundingBox<Point_t> &box,
                   const Point_t &pt, 
                   double tolerance,
                   bbox_tag,
                   point_tag)
{
    for (size_t i = 0; i < VectorTraits<Point_t>::size; ++i) {
        if (pt[i] < box.min()[i] || pt[i] > box.max()[i]) {
            return false;
        }
    }

    return true;
}

// Polygon-Point
template<typename Polygon_t, typename Point_t>
auto contains_impl(const Polygon_t &polygon,
                   const Point_t &pt, 
                   double tolerance,
                   polygon_tag,
                   point_tag) -> bool
{
    return locatePointInPolygon(polygon, pt) == Location::Interior;
}

// Polygon-LineString (todos los puntos de la línea están dentro del polígono, incluyendo el borde)
template<typename Point_t>
auto contains_impl(const Polygon<Point_t> &polygon, 
                   const LineString<Point_t> &line,
                   double tolerance,
                   polygon_tag, 
                   linestring_tag) -> bool
{
    if (line.isEmpty()) return true; // Línea vacía siempre está contenida

    // 1️ - Todos los puntos de la línea deben estar en el interior (no en borde)
    for (const auto &pt : line) {
        if (locatePointInPolygon(polygon, pt) != Location::Interior) {
            return false;
        }
    }

    // 2️ - Ningún segmento debe intersectar los huecos
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        Segment<Point_t> seg(line[i], line[i + 1]);
        for (const auto &hole : polygon.inners()) {
            if (intersects(seg, hole)) {
                return false;
            }
        }
    }

    return true;
}


// MultiPolygon - Point (un multipolígono contiene un punto)
template<typename Point_t>
auto contains_impl(const MultiPolygon<Point_t> &multiPolygon,
                   const Point_t &pt, 
                   double tolerance,
                   multipolygon_tag, 
                   point_tag) -> bool
{
    for (const auto &polygon : multiPolygon) {
        if (contains(polygon, pt))
            return true;
    }

    return false;
}

// MultiPolygon - Polygon
template<typename Point_t>
auto contains_impl(const MultiPolygon<Point_t> &multiPolygon, 
                   const Polygon<Point_t> &polygon,
                   double tolerance,
                   multipolygon_tag, 
                   polygon_tag) -> bool
{
    for (const auto &containerPoly : multiPolygon) {
        if (contains(containerPoly, polygon)) {
            return true;
        }
    }
    return false;
}

/* GeometryCollection with any geometry */
template<typename Point_t, typename Geometry_t>
auto contains_impl(const GeometryCollection<Point_t> &gc,
                   const Geometry_t &geom,
                   double tolerance,
                   collection_tag,
                   geometry_tag_t<Geometry_t>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
            return contains(arg.get(), geom);
            }, item);

        if (hit) return true;
    }

    return false;
}


template<typename G1, typename G2, typename Tag1, typename Tag2>
[[nodiscard]]
constexpr auto contains_impl(const G1 &, const G2 &, double, Tag1, Tag2) -> bool
{
    return false;
}


} // namespace detail


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
auto contains(const G1 &geom1, const G2 &geom2) -> bool
{
    using Scalar = typename point_traits<geometry_traits<G1>::point_type>::value_type;

    return contains(geom1, geom2, default_tolerance<Scalar>::value);

}

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto contains(const G1 &geom1,
                        const G2 &geom2,
                        double tolerance) -> bool
{
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    return detail::contains_impl(geom1, geom2, tolerance, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto contains(const G1 &geom1,
                        const G2 &geom2,
                        const TolerancePolicy &policy) -> bool
{
    return contains(geom1, geom2, policy.xyTolerance());
}


} // namespace tl