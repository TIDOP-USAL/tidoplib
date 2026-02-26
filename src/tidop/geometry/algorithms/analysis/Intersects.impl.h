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


// ============================================================================
// Intersection implementations for basic types
// ============================================================================

/* Point - Point */

template<typename Point_t>
auto intersects_impl(const Point_t &p1, 
                     const Point_t &p2, 
                     point_tag, 
                     point_tag) -> bool
{
    return equals(p1, p2);
}

/* Point - Segment */

template<typename Point_t>
auto intersects_impl(const Point_t &point, 
                     const Segment<Point_t> &segment,
                     point_tag, 
                     segment_tag) -> bool
{
    using calc_t = typename point_traits<Point_t>::calculation_type;

    auto proj = project(point, segment);
    return !proj.isBeforeStart() && !proj.isAfterEnd() &&
           distance(point, proj.closestPoint) < default_tolerance<calc_t>::value;
}

/* Segment - Segment */

//TODO: Versión para 2D/ND con el algoritmo de Dan Sunday
//if (!intersects(envelope(s1), envelope(s2))) return false;
//
//if constexpr (point_traits<Point_t>::spatial_dims == 2) {
//    // CÓDIGO DE ORIENTACIÓN (es más rápido para 2D plano)
//} else {
//    // PARA 3D O MÁS:
//    using calc_t = typename point_traits<Point_t>::calculation_type;
//    constexpr calc_t eps = std::numeric_limits<calc_t>::epsilon() * 1000;
//    
//    // Distance utiliza algoritmo de Dan Sunday
//    return distance(s1, s2) < eps; // Intersectan si la distancia mínima es ~0
//}

template<typename Point_t>
auto intersects_impl(const Segment<Point_t> &s1, 
                     const Segment<Point_t> &s2,
                     segment_tag, 
                     segment_tag) -> bool
{
    // Solo para 2D por ahora
    static_assert(point_traits<Point_t>::spatial_dims == 2, "Segment intersection currently only supports 2D");

    //if (!intersects(envelope(s1), envelope(s2))) return false;

    //auto data = computeIntersectionData(s1, s2);

    //// Intersección interior-interior
    //if (data.o1 != data.o2 && data.o3 != data.o4) return true;

    //const auto &p1 = s1.pt1();
    //const auto &p2 = s1.pt2();
    //const auto &q1 = s2.pt1();
    //const auto &q2 = s2.pt2();

    //// Casos especiales (colinealidad)
    //if (data.o1 == WindingOrder::Colinear && isBetween(p1, q1, p2)) return true;
    //if (data.o2 == WindingOrder::Colinear && isBetween(p1, q2, p2)) return true;
    //if (data.o3 == WindingOrder::Colinear && isBetween(q1, p1, q2)) return true;
    //if (data.o4 == WindingOrder::Colinear && isBetween(q1, p2, q2)) return true;

    //return false;
    return intersectionType(s1, s2) != IntersectionType::None;
}

/* Point - LineString */

//TODO: Crear clase SegmentView
template<typename Point_t>
auto intersects_impl(const Point_t &pt,
                     const LineString<Point_t> &lineString,
                     point_tag, 
                     linestring_tag) -> bool 
{
    const std::size_t n = lineString.size();
    if (n < 2) return false;

    for (size_t i = 0; i + 1 < n; ++i) {
        if (intersects(Segment<Point_t>(lineString[i], lineString[i + 1]), pt))
            return true;
    }

    return false;
    //return locatePointOnLineString(lineString, pt) != Location::Boundary;
}

// LineString - Segment
template<typename Point_t>
auto intersects_impl(const LineString<Point_t> &line, 
                     const Segment<Point_t> &seg,
                     linestring_tag, 
                     segment_tag) -> bool
{
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        if (intersects(Segment<Point_t>(line[i], line[i + 1]), seg)) return true;
    }
    return false;
}

/* LineString - LineString */

template<typename Point_t>
auto intersects_impl(const LineString<Point_t> &l1, 
                     const LineString<Point_t> &l2,
                     linestring_tag, 
                     linestring_tag) -> bool 
{
    if (!intersects(envelope(l1), envelope(l2))) return false;

    for (size_t i = 0; i + 1 < l1.size(); ++i) {
        Segment<Point_t> s1(l1[i], l1[i + 1]);
        for (size_t j = 0; j + 1 < l2.size(); ++j) {
            if (intersects(s1, Segment<Point_t>(l2[j], l2[j + 1]))) return true;
        }
    }
    return false;
}

/* Point - Polygon */

template<typename Point_t>
auto intersects_impl(const Point_t &point,
                     const Polygon<Point_t> &polygon,
                     point_tag, 
                     polygon_tag) -> bool 
{
    //// Fast bounding box check
    //if (!intersects(envelope(polygon), point)) {
    //    return false;
    //}

    //const auto location = locatePointInRing(polygon.outer(), point);

    //if (location == Location::Exterior)
    //    return false;

    //if (location == Location::Boundary)
    //    return true;

    //for (const auto &hole : polygon.inners()) {
    //    const auto hole_location = locatePointInRing(hole, point);

    //    if (hole_location != Location::Exterior)
    //        return false; // boundary o interior de un agujero ⇒ no intersecta
    //}

    //return true;
    return locatePointInPolygon(polygon, point) != Location::Exterior;
}

template<typename Point_t>
auto intersects_impl(const Segment<Point_t> &segment,
                     const Polygon<Point_t> &polygon,
                     segment_tag, 
                     polygon_tag) -> bool
{
    //// 1. Check if segment intersects any edge of polygon
    //auto check_ring_intersection = [&](const auto &ring) {
    //    for (size_t i = 0; i < ring.size(); ++i) {
    //        size_t j = (i + 1) % ring.size();
    //        Segment<Point_t> edge(ring[i], ring[j]);
    //        if (intersects(segment, edge)) {
    //            return true;
    //        }
    //    }
    //    return false;
    //};

    //if (check_ring_intersection(polygon.outer())) {
    //    return true;
    //}

    //for (const auto &hole : polygon.inners()) {
    //    if (check_ring_intersection(hole)) {
    //        return true;
    //    }
    //}

    //// 2. Check if segment is completely inside polygon (excluding holes)
    //// For intersects, we just need one point inside
    //if (contains(polygon, segment.pt1()) &&
    //    !pointInAnyHole(segment.pt1(), polygon)) {
    //    return true;
    //}

    //if (contains(polygon, segment.pt2()) &&
    //    !pointInAnyHole(segment.pt2(), polygon)) {
    //    return true;
    //}

    //// 3. Check middle point in case both endpoints are in holes
    //Point_t mid_point(
    //    (segment.pt1().x() + segment.pt2().x()) * 0.5,
    //    (segment.pt1().y() + segment.pt2().y()) * 0.5
    //);

    //if (contains(polygon, mid_point) &&
    //    !pointInAnyHole(mid_point, polygon)) {
    //    return true;
    //}

    //return false;
    auto check_ring_intersection = [&](const auto &ring) {
        for (size_t i = 0; i < ring.size(); ++i) {
            size_t j = (i + 1) % ring.size();
            Segment<Point_t> edge(ring[i], ring[j]);
            if (intersects(segment, edge)) return true;
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
        return contains(polygon, pt) && !pointInAnyHole(pt, polygon);
        };

    if (check_point_inside(segment.pt1())) return true;
    if (check_point_inside(segment.pt2())) return true;

    // Punto medio (por si ambos extremos caen dentro de un hueco)
    Point_t mid(
        (segment.pt1().x() + segment.pt2().x()) * 0.5,
        (segment.pt1().y() + segment.pt2().y()) * 0.5
    );

    if (check_point_inside(mid)) return true;

    return false;
}

/* LineString - Polygon */

//template<typename Point_t>
//auto intersects_impl(const LineString<Point_t> &line,
//                     const Polygon<Point_t> &poly,
//                     linestring_tag,
//                     polygon_tag) -> bool
//{
//    // Si algún segmento de la línea cruza el borde del polígono
//    //TODO: Aún no tengo boundary
//    //if (intersects(boundary(poly), line)) return true;
//
//    // Si no cruzan, la línea puede estar toda dentro o toda fuera
//    if (line.empty()) return false;
//    return intersects(poly, line[0]); // Basta con chequear un punto
//}
//template<typename Point_t>
//auto intersects_impl(const LineString<Point_t> &line, 
//                     const Polygon<Point_t> &polygon,
//                     linestring_tag,
//                     polygon_tag) -> bool
//{
//    if (line.empty()) return false;
//
//    // 1. Check if any point of line is inside polygon
//    for (const auto &point : line) {
//        if (intersects(point, polygon)) {
//            return true;
//        }
//    }
//
//    // 2. Check if any segment intersects polygon boundary
//    auto check_ring_intersection = [&](const auto &ring) {
//        for (size_t i = 0; i < ring.size(); ++i) {
//            size_t j = (i + 1) % ring.size();
//            Segment<Point_t> edge(ring[i], ring[j]);
//
//            for (size_t k = 0; k + 1 < line.size(); ++k) {
//                Segment<Point_t> seg(line[k], line[k + 1]);
//                if (intersects(seg, edge)) {
//                    return true;
//                }
//            }
//        }
//        return false;
//        };
//
//    if (check_ring_intersection(polygon.outer())) return true;
//
//    for (const auto &hole : polygon.inners()) {
//        if (check_ring_intersection(hole)) return true;
//    }
//
//    return false;
//}
template<typename Point_t>
auto intersects_impl(const LineString<Point_t> &line,
                     const Polygon<Point_t> &polygon,
                     linestring_tag,
                     polygon_tag) -> bool
{
    if (line.isEmpty()) return false;

    // 1️ - Algún punto dentro o en borde del polígono
    for (const auto &pt : line) {
        if (intersects(pt, polygon)) return true;
    }

    // 2️ - Algún segmento intersecta la frontera del polígono
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        Segment<Point_t> seg(line[i], line[i + 1]);

        if (intersects(seg, polygon.outer())) return true;
        for (const auto &hole : polygon.inners()) {
            if (intersects(seg, hole)) return true;
        }
    }

    return false;
}

/* Polygon - Polygon */

// Polygon - Polygon
//template<typename Polygon_t>
//auto intersects_impl(const Polygon_t &p1,
//                     const Polygon_t &p2,
//                     polygon_tag, 
//                     polygon_tag) -> bool 
//{
//    // 1. ¿Se cruzan los bordes?
//    //TODO: Aún no tengo boundary
//    //if (intersects(boundary(p1), boundary(p2))) return true;
//
//    // 2. ¿P1 está dentro de P2? (chequeamos un punto del exterior)
//    if (intersects(p1, p2.outer()[0])) return true;
//
//    // 3. ¿P2 está dentro de P1?
//    if (intersects(p2, p1.outer()[0])) return true;
//
//    return false;
//}
template<typename Point_t>
auto intersects_impl(const Polygon<Point_t> &poly1, 
                     const Polygon<Point_t> &poly2,
                     polygon_tag, 
                     polygon_tag) -> bool
{
    // 1. Check if any vertex of poly1 is inside poly2
    for (const auto &point : poly1.outer()) {
        if (intersects(point, poly2)) {
            return true;
        }
    }

    // 2. Check if any vertex of poly2 is inside poly1
    for (const auto &point : poly2.outer()) {
        if (intersects(point, poly1)) {
            return true;
        }
    }

    // 3. Check if boundaries intersect
    auto check_rings_intersect = [](const auto &ring1, const auto &ring2) {
        for (size_t i = 0; i < ring1.size(); ++i) {
            size_t j = (i + 1) % ring1.size();
            Segment<Point_t> edge1(ring1[i], ring1[j]);

            //for (size_t k = 0; k < ring2.size(); ++k) {
            //    size_t l = (k + 1) % ring2.size();
            //    Segment<Point_t> edge2(ring2[k], ring2[l]);

            //    if (intersects(edge1, edge2)) {
            //        return true;
            //    }
            //}
            if (intersects(edge1, ring2)) return true;
        }
        return false;
        };

    // Check all ring combinations
    if (check_rings_intersect(poly1.outer(), poly2.outer())) return true;

    for (const auto &hole : poly2.inners()) {
        if (check_rings_intersect(poly1.outer(), hole)) return true;
    }

    for (const auto &hole : poly1.inners()) {
        if (check_rings_intersect(poly2.outer(), hole)) return true;
    }

    for (const auto &hole1 : poly1.inners()) {
        for (const auto &hole2 : poly2.inners()) {
            if (check_rings_intersect(hole1, hole2)) return true;
        }
    }

    return false;
}

/* GeometryCollection with any geometry */
template<typename Point_t, typename Geometry_t>
auto intersects_impl(const GeometryCollection<Point_t> &gc, 
                     const Geometry_t &geom,
                     collection_tag, 
                     geometry_tag_t<Geometry_t>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
                return intersects(arg.get(), geom);
            }, item);

        if (hit) return true;
    }

    return false;
}

/* BoundingBox - BoundingBox */
template<typename Point_t>
auto intersects_impl(const BoundingBox<Point_t> &bbox1,
                     const BoundingBox<Point_t> &bbox2,
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
template<typename Point_t>
auto intersects_impl(const BoundingBox<Point_t> &bbox, 
                     const Point_t &point,
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
template<typename G1, typename G2, typename Tag1, typename Tag2>
auto intersects_impl(const G1 &g1, const G2 &g2, Tag1 t1, Tag2 t2) -> double
{
    return intersects_impl(g2, g1, t2, t1);
}


} // namespace detail



template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
auto intersects(const G1 &g1, const G2 &g2) -> bool
{
    //static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    //static_assert(is_geometry_v<G2>, "Second argument must be a geometry");

    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;

    static_assert(std::is_same_v<typename point_traits<P1>::value_type,
                                 typename point_traits<P2>::value_type>,
        "Points must have same coordinate type");

    auto dispatch = [&] {
        using tag1 = geometry_tag_t<G1>;
        using tag2 = geometry_tag_t<G2>;
        return detail::intersects_impl(g1, g2, tag1{}, tag2{});
    };

    if constexpr (GeometryCollectionConcept<G1> || GeometryCollectionConcept<G2>) {
        return dispatch();
    } else if constexpr (MultiGeometryConcept<G1>) {
        for (const auto &a : g1) {
            if (intersects(a, g2)) return true;
        }
        return false;
    } else if constexpr (MultiGeometryConcept<G2>) {
        for (const auto &b : g2) {
            if (intersects(g1, b)) return true;
        }
        return false;
    } else {
        return dispatch();
    }
}


template<typename Point_t>
auto intersects(const Segment<Point_t> &seg, const LinearRing<Point_t> &ring)
{

    if (ring.size() < 2) return false;

    // 1️ - Chequear si alguno de los extremos está en el interior o en el borde
    if (locatePointInRing(ring, seg.pt1()) != Location::Exterior ||
        locatePointInRing(ring, seg.pt2()) != Location::Exterior) {
        return true;
    }

    // 2️ - Revisar cada arista del anillo
    for (size_t i = 0; i < ring.size(); ++i) {
        size_t j = (i + 1) % ring.size();
        Segment<Point_t> edge(ring[i], ring[j]);

        if (intersects(seg, edge)) {
            return true;
        }
    }

    return false;
}



} // namespace tl