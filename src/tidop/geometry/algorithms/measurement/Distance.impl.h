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

#include <cmath>
#include "tidop/geometry/algorithms/projection.h"
#include "tidop/geometry/algorithms/analysis/Contains.h"
#include "tidop/geometry/algorithms/analysis/Intersects.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/geometry/base/Traits.h"

namespace tl
{

namespace detail 
{

template<typename Point_t>
auto distance_impl(const Point_t &p1,
                   const Point_t &p2,
                   point_tag, 
                   point_tag) -> double
{
    return (p2 - p1).module();
}

template<typename Point_t>
auto distance_impl(const Point_t &point, 
                   const Segment<Point_t> &segment, 
                   point_tag, 
                   segment_tag) -> double
{
    auto result = project(point, segment);

    if (result.isBeforeStart()) {
        return distance(point, segment.pt1());
    }
    if (result.isAfterEnd()) {
        return distance(point, segment.pt2());
    }

    // El punto más cercano es la proyección ortogonal
    return distance(point, result.closestPoint);
}

/* Calcula la distancia de un punto a una línea */
template<typename Point_t>
auto distance_impl(const Point_t &point,
                   const LineString<Point_t> &lineString, 
                   point_tag, 
                   linestring_tag) -> double
{
    if (lineString.empty()) return std::numeric_limits<double>::max();
    if (lineString.size() == 1) return distance(point, lineString[0]);

    double min_dist = std::numeric_limits<double>::max();

    for (std::size_t i = 0; i < lineString.size() - 1; ++i) {
        // Creamos un segmento temporal para cada tramo de la línea
        Segment<Point_t> edge(lineString[i], lineString[i + 1]);
        double d = distance(point, edge);

        if (d < min_dist) {
            min_dist = d;
        }
    }

    return min_dist;
}


/* Calcula la distancia de un punto a un polígono */
template<typename Point_t>
auto distance_impl(const Point_t &point, 
                   const Polygon<Point_t> &polygon,
                   point_tag, 
                   polygon_tag) -> double
{
    // 1. Si el punto está contenido en el polígono (incluyendo huecos), la distancia es 0
    if (contains(polygon, point)) {
        return 0.0;
    }

    // 2. Si está fuera, calculamos la distancia mínima a los bordes
    double min_dist = std::numeric_limits<double>::max();

    auto check_ring = [&](const auto &ring) {
        if (ring.empty()) return;
        // Importante: Un anillo de N puntos tiene N segmentos
        for (size_t i = 0; i < ring.size(); ++i) {
            Segment<Point_t> edge(ring[i], ring[(i + 1) % ring.size()]);
            min_dist = std::min(min_dist, distance(point, edge));
        }
        };

    check_ring(polygon.outer());
    for (const auto &hole : polygon.inners()) {
        check_ring(hole);
    }

    return min_dist;
}

template<typename Point_t>
auto distance_impl(const Point_t &point, 
                   const MultiPoint<Point_t> &multiPoint, 
                   point_tag, 
                   multipoint_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();
    for (const auto &item : multiPoint) {
        min_dist = std::min(min_dist, distance(point, item));
    }
    return min_dist;
}

template<typename Point_t>
auto distance_impl(const Point_t &point, 
                   const MultiLineString<Point_t> &multiLine, 
                   point_tag, 
                   multilinestring_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();
    for (const auto &line : multiLine) {
        double d = distance(point, line);
        if (d < min_dist) min_dist = d;

        if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
    }

    return min_dist;
}

template<typename Point_t>
auto distance_impl(const Point_t &point, 
                   const MultiPolygon<Point_t> &multiPolygon, 
                   point_tag, 
                   multipolygon_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();
    for (const auto &polygon : multiPolygon) {
        double d = distance(point, polygon);
        if (d < min_dist) min_dist = d;

        if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
    }
    return min_dist;
}

/* Calcula la distancia mínima entre dos segmentos */
template<typename Point_t>
auto distance_impl(const Segment<Point_t> &segment1, 
                   const Segment<Point_t> &segment2, 
                   segment_tag, 
                   segment_tag) -> double
{
    if (intersects(segment1, segment2))
        return 0.0;

    return std::min({
        distance(segment1.pt1(), segment2),
        distance(segment1.pt2(), segment2),
        distance(segment2.pt1(), segment1),
        distance(segment2.pt2(), segment1)
        });
}

template<typename Point_t>
auto distance_impl(const Segment<Point_t> &segment,
                   const LineString<Point_t> &line,
                   segment_tag,
                   linestring_tag) -> double
{
    if (line.empty()) return std::numeric_limits<double>::max();

    double min_dist = std::numeric_limits<double>::max();

    for (size_t i = 0; i + 1 < line.size(); ++i) {
        Segment<Point_t> seg(line[i], line[i + 1]);
        double d = distance(segment, seg);
        min_dist = std::min(min_dist, d);
        if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
    }

    return min_dist;
}

template<typename Point_t>
auto distance_impl(const LineString<Point_t> &l1, 
                   const LineString<Point_t> &l2, 
                   linestring_tag,
                   linestring_tag) -> double
{
    if (l1.empty() || l2.empty())
        return std::numeric_limits<double>::max();

    double min_dist = std::numeric_limits<double>::max();

    for (size_t i = 0; i + 1 < l1.size(); ++i) {
        Segment<Point_t> s1(l1[i], l1[i + 1]);
        min_dist = std::min(min_dist, distance(s1, l2));
        if (min_dist == 0.0) return 0.0;
    }

    return min_dist;
}

template<typename Point_t>
auto distance_impl(const LineString<Point_t> &line,
                   const Polygon<Point_t> &polygon,
                   linestring_tag, 
                   polygon_tag) -> double
{
    // 1. Si la línea intersecta o está dentro del polígono (excluyendo huecos)
    for (const auto &point : line) {
        if (contains(polygon, point)) {
            return 0.0;
        }
    }

    // 2. Distancia a los bordes
    double min_dist = std::numeric_limits<double>::max();

    auto check_ring = [&](const auto &ring) {
        for (size_t i = 0; i < ring.size(); ++i) {
            size_t j = (i + 1) % ring.size();
            Segment<Point_t> edge(ring[i], ring[j]);

            // Distancia de cada segmento de la línea al borde
            for (size_t k = 0; k + 1 < line.size(); ++k) {
                Segment<Point_t> seg(line[k], line[k + 1]);
                double d = distance(seg, edge);
                min_dist = std::min(min_dist, d);
                if (min_dist <= std::numeric_limits<double>::epsilon()) return true;
            }
        }
        return false;
        };

    if (check_ring(polygon.outer())) return 0.0;
    for (const auto &hole : polygon.inners()) {
        if (check_ring(hole)) return 0.0;
    }

    return min_dist;
}

//template<typename Point_t>
//auto distance_impl(const Polygon<Point_t> &poly1,
//                   const Polygon<Point_t> &poly2,
//                   polygon_tag, polygon_tag) -> double
//{
//    // 1. Chequeo rápido con envelopes
//    auto env1 = envelope(poly1);
//    auto env2 = envelope(poly2);
//    if (env1.isEmpty() || env2.isEmpty()) {
//        return std::numeric_limits<double>::max();
//    }
//
//    // Si los envelopes no se intersectan, podemos calcular distancia aproximada
//    //if (!intersects(env1, env2)) {
//    //    // Distancia entre envelopes (optimización)
//    //    return envelope_distance(env1, env2);
//    //}
//
//    // 2. Si un polígono está dentro del otro (excluyendo huecos)
//    Point_t test_point = poly1.outer()[0];  // Tomamos un punto del borde
//    if (contains(poly2, test_point)) {
//        // Verificar que no esté en un hueco
//        bool in_hole = false;
//        for (const auto &hole : poly2.inners()) {
//            if (contains_point_in_ring(hole, test_point)) {
//                in_hole = true;
//                break;
//            }
//        }
//        if (!in_hole) return 0.0;
//    }
//
//    test_point = poly2.outer()[0];
//    if (contains(poly1, test_point)) {
//        bool in_hole = false;
//        for (const auto &hole : poly1.inners()) {
//            if (contains_point_in_ring(hole, test_point)) {
//                in_hole = true;
//                break;
//            }
//        }
//        if (!in_hole) return 0.0;
//    }
//
//    // 3. Distancia entre bordes
//    double min_dist = std::numeric_limits<double>::max();
//
//    auto check_rings = [&](const auto &ring1, const auto &ring2) {
//        for (size_t i = 0; i < ring1.size(); ++i) {
//            size_t next_i = (i + 1) % ring1.size();
//            Segment<Point_t> edge1(ring1[i], ring1[next_i]);
//
//            for (size_t j = 0; j < ring2.size(); ++j) {
//                size_t next_j = (j + 1) % ring2.size();
//                Segment<Point_t> edge2(ring2[j], ring2[next_j]);
//
//                double d = distance(edge1, edge2);
//                min_dist = std::min(min_dist, d);
//                if (min_dist <= std::numeric_limits<double>::epsilon()) return true;
//            }
//        }
//        return false;
//        };
//
//    // Combinaciones: exterior-exterior, exterior-huecos, huecos-exterior, huecos-huecos
//    if (check_rings(poly1.outer(), poly2.outer())) return 0.0;
//
//    for (const auto &hole1 : poly1.inners()) {
//        if (check_rings(hole1, poly2.outer())) return 0.0;
//    }
//
//    for (const auto &hole2 : poly2.inners()) {
//        if (check_rings(poly1.outer(), hole2)) return 0.0;
//    }
//
//    for (const auto &hole1 : poly1.inners()) {
//        for (const auto &hole2 : poly2.inners()) {
//            if (check_rings(hole1, hole2)) return 0.0;
//        }
//    }
//
//    return min_dist;
//}

template<typename Point_t>
auto distance_impl(const MultiPoint<Point_t> &mp1,
                   const MultiPoint<Point_t> &mp2,
                   multipoint_tag, multipoint_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();

    for (const auto &p1 : mp1) {
        for (const auto &p2 : mp2) {
            double d = distance(p1, p2);
            min_dist = std::min(min_dist, d);
            if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
        }
    }

    return min_dist;
}

// MultiLineString - MultiLineString
template<typename Point_t>
auto distance_impl(const MultiLineString<Point_t> &mls1,
                   const MultiLineString<Point_t> &mls2,
                   multilinestring_tag, multilinestring_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();

    for (const auto &line1 : mls1) {
        for (const auto &line2 : mls2) {
            double d = distance(line1, line2);
            min_dist = std::min(min_dist, d);
            if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
        }
    }

    return min_dist;
}

// MultiPolygon - MultiPolygon
template<typename Point_t>
auto distance_impl(const MultiPolygon<Point_t> &mp1,
                   const MultiPolygon<Point_t> &mp2,
                   multipolygon_tag, multipolygon_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();

    for (const auto &poly1 : mp1) {
        for (const auto &poly2 : mp2) {
            double d = distance(poly1, poly2);
            min_dist = std::min(min_dist, d);
            if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
        }
    }

    return min_dist;
}

template<typename Point_t, typename Geometry_t>
auto distance_impl(const GeometryCollection<Point_t> &gc,
                   const Geometry_t &geometry,
                   collection_tag, 
                   geometry_tag_t<Geometry_t>) -> double
{
    double min_dist = std::numeric_limits<double>::max();

    for (const auto &geom : gc) {
        std::visit([&](auto &&arg) {
            min_dist = std::min(min_dist, distance(arg.get(), geometry));
            }, geom);

        if (min_dist == 0.0) return 0.0;
    }
    return min_dist;
}

template<typename Point_t>
auto distance_impl(const GeometryCollection<Point_t> &gc1,
                   const GeometryCollection<Point_t> &gc2,
                   collection_tag, collection_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();

    for (const auto &geom1 : gc1) {
        for (const auto &geom2 : gc2) {
            std::visit([&](auto &&arg1, auto &&arg2) {
                min_dist = std::min(min_dist, distance(arg1.get(), arg2.get()));
                }, geom1, geom2);

            if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
        }
    }

    return min_dist;
}


// Función espejo
template<typename G1, typename G2, typename Tag1, typename Tag2>
auto distance_impl(const G1 &g1, const G2 &g2, Tag1 t1, Tag2 t2) -> double
{
    return distance_impl(g2, g1, t2, t1);
}

} // namespace detail


template<typename G1, typename G2>
auto distance(const G1 &g1, const G2 &g2) -> double
{
    static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    static_assert(is_geometry_v<G2>, "Second argument must be a geometry");
    static_assert((std::is_same_v<
        geometry_traits<G1>::point_type,
        geometry_traits<G2>::point_type>),
        "All geometries must use the same point type.");

    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;
    return detail::distance_impl(g1, g2, tag1{}, tag2{});
}

} // namespace tl