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

namespace tl
{
	
namespace detail 
{

template<typename Ring_t>
[[nodiscard]]
constexpr auto calculate_ring_area(const Ring_t &ring) -> double
{
    if (ring.size() < 3) return 0.0;

    double area = 0.0;
    size_t n = ring.size();

    for (size_t i = 0; i < n; ++i) {
        const auto &p1 = ring[i];
        const auto &p2 = ring[(i + 1) % n]; // Conecta el último con el primero
        area += (p1.x() * p2.y()) - (p2.x() * p1.y());
    }

    return area * 0.5;
}


template<typename Point_t>
auto area_impl(const Point_t &, point_tag)
{
    return 0.0; 
}

template<typename Segment_t>
auto area_impl(const Segment_t &, segment_tag) { return 0.0; }

template<typename Polygon_t>
auto area_impl(const Polygon_t &poly, polygon_tag)
{
    double total_area = std::abs(calculate_ring_area(poly.outer()));
    for (const auto &inner : poly.inners()) {
        total_area -= std::abs(calculate_ring_area(inner));
    }

    return total_area;
}

template<typename MultiPolygon_t>
auto area_impl(const MultiPolygon_t &multiPolygon, multipolygon_tag) -> double
{
    double total_area = 0.0;

    for (const auto &polygon : multiPolygon) {
        total_area += area(polygon);
    }

    return total_area;
}

// Depende de si es 2D o 3D
//template<typename BoundingBox_t>
//auto area_impl(const BoundingBox_t &box, bbox_tag) -> double
//{
//    return static_cast<double>(box.width()) * box.height();
//}

} // namespace detail

template<typename Geometry_t>
[[nodiscard]]
constexpr auto area(const Geometry_t &g)
{
    return detail::area_impl(g, geometry_tag_t<Geometry_t>{});
}

} // namespace tl