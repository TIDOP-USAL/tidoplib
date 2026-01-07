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

namespace tl
{
	
namespace geometry
{

namespace detail 
{

/* Distancia entre dos puntos de cualquier dimensión. */
template<typename Point_t>
auto distance_impl(const Point_t &p1, const Point_t &p2, point_tag, point_tag) -> double
{
    return (p2 - p1).module();
}

/* Calcula la distancia de un punto a un segmento */
template<typename Point_t, typename Segment_t>
auto distance_impl(const Point_t &point, const Segment_t &segment, point_tag, segment_tag) -> double
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

//template<typename Segment_t, typename Point_t>
//auto distance_impl(const Segment_t &segment, const Point_t &point, segment_tag, point_tag) -> double
//{
//    return distance_impl(point, segment, point_tag{}, segment_tag{});
//}

/* Calcula la distancia mínima entre dos segmentos */
template<typename S1, typename S2>
auto distance_impl(const S1 &segment1, const S2 &segment2, segment_tag, segment_tag) -> double
{
    return std::min({
        distance(segment1.p1(), segment2),
        distance(segment1.p2(), segment2),
        distance(segment2.p1(), segment1),
        distance(segment2.p2(), segment1)
        });
}

/* Calcula la distancia de un punto a una línea */
template<typename Point_t, typename LineString_t>
auto distance_impl(const Point_t &point, const LineString_t &lineString, point_tag, linestring_tag) -> double
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
template<typename Point_t, typename Polygon_t>
auto distance_impl(const Point_t &point, const Polygon_t &polygon, point_tag, polygon_tag) -> double
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

template<typename Point_t, typename MultiPoint_t>
auto distance_impl(const Point_t &point, const MultiPoint_t &multiPoint, point_tag, multipoint_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();
    for (const auto &item : multiPoint) {
        min_dist = std::min(min_dist, distance(point, item));
    }
    return min_dist;
}

template<typename Point_t, typename MultiLineString_t>
auto distance_impl(const Point_t &point, const MultiLineString_t &multiLine, point_tag, multilinestring_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();
    for (const auto &line : multiLine) {
        double d = distance(point, line);
        if (d < min_dist) min_dist = d;

        if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
    }

    return min_dist;
}

template<typename Point_t, typename MultiPolygon_t>
auto distance_impl(const Point_t &point, const MultiPolygon_t &multiPolygon, point_tag, multipolygon_tag) -> double
{
    double min_dist = std::numeric_limits<double>::max();
    for (const auto &polygon : multiPolygon) {
        double d = distance(point, polygon);
        if (d < min_dist) min_dist = d;

        if (min_dist <= std::numeric_limits<double>::epsilon()) return 0.0;
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
    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;
    return detail::distance_impl(g1, g2, tag1{}, tag2{});
}

} // namespace tl
} // namespace geometry