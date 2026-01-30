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

#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{

namespace detail 
{

template<typename BoundingBox_t, typename Point_t>
auto contains_impl(const BoundingBox_t &box,
                   const Point_t &pt, 
                   bbox_tag,
                   point_tag)
{
    for (size_t i = 0; i < VectorTraits<Point_t>::size; ++i) {
        if (pt[i] < box.pt1()[i] || pt[i] > box.pt2()[i]) {
            return false;
        }
    }

    return true;
}

//template<typename Polygon_t, typename Point_t>
//auto contains_impl(const Polygon_t &polygon, const Point_t &pt, polygon_tag, point_tag) -> bool
//{
//    auto is_inside_ring = [&](const auto &ring) {
//        bool inside = false;
//        size_t n = ring.size();
//        if (n < 3) return false;
//
//        for (size_t i = 0, j = n - 1; i < n; j = i++) {
//            // Comprobamos si el rayo horizontal cruza el segmento (ring[i], ring[j])
//            if (((ring[i].y() > pt.y()) != (ring[j].y() > pt.y())) &&
//                (pt.x() < (ring[j].x() - ring[i].x()) * (pt.y() - ring[i].y()) / (ring[j].y() - ring[i].y()) + ring[i].x())) {
//                inside = !inside;
//            }
//        }
//        return inside;
//    };
//
//    // 1. El punto debe estar dentro del anillo exterior
//    if (!is_inside_ring(polygon.outer())) {
//        return false;
//    }
//
//    // 2. El punto NO debe estar dentro de ningún hueco (inner ring)
//    for (const auto &hole : polygon.inners()) {
//        if (is_inside_ring(hole)) {
//            return false;
//        }
//    }
//
//    return true;
//}

template<typename Polygon_t, typename Point_t>
auto contains_impl(const Polygon_t &polygon,
                   const Point_t &pt, 
                   polygon_tag,
                   point_tag) -> bool
{
    // 1. Check rápido: Si el punto no está en el Bounding Box, no puede estar en el polígono
    auto bbox = envelope(polygon);
    if (!contains(envelope(polygon), pt)) {
        return false;
    }

    auto is_inside_ring = [&](const auto &ring) {
        bool inside = false;
        size_t n = ring.size();
        for (size_t i = 0, j = n - 1; i < n; j = i++) {
            if (((ring[i].y() > pt.y()) != (ring[j].y() > pt.y())) &&
                (pt.x() < (ring[j].x() - ring[i].x()) * (pt.y() - ring[i].y()) / (ring[j].y() - ring[i].y()) + ring[i].x())) {
                inside = !inside;
            }
        }
        return inside;
    };

    if (!is_inside_ring(polygon.outer())) return false;

    for (const auto &hole : polygon.inners()) {
        if (is_inside_ring(hole)) return false; // Si cae en un hueco, está fuera
    }

    return true;
}

template<typename MultiPolygon_t, typename Point_t>
auto contains_impl(const MultiPolygon_t &multiPolygon,
                   const Point_t &pt, 
                   multipolygon_tag, 
                   point_tag) -> bool
{
    for (const auto &polygon : multiPolygon) {
        if (contains(polygon, pt)) return true;
    }

    return false;
}

} // namespace detail


template<typename G1, typename G2>
auto contains(const G1 &g1, const G2 &g2) -> bool
{
    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;
    return detail::contains_impl(g1, g2, tag1{}, tag2{});
}

} // namespace tl