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

// Longitud de un punto es 0 según OGC
template<typename Point_t>
[[nodiscard]]
constexpr auto length_impl(const Point_t &, point_tag) -> double
{
    return 0.0;
}

template<typename Segment_t>
auto length_impl(const Segment_t &s, segment_tag) -> double
{
    return (s.pt2() - s.pt1()).module();
}

template<typename LineString_t>
auto length_impl(const LineString_t &ls, linestring_tag) -> double 
{
    if (ls.size() < 2) return 0.0;

    double total = 0.0;
    for (std::size_t i = 0; i < ls.size() - 1; ++i) {
        total += distance(ls[i], ls[i + 1]);
    }
    return total;
}

template<typename Ring_t>
auto calculate_ring_perimeter(const Ring_t &ring) -> double
{
    if (ring.size() < 2) return 0.0;

    double p = 0.0;
    std::size_t n = ring.size();

    for (std::size_t i = 0; i < n; ++i) {
        p += distance(ring[i], ring[(i + 1) % n]);
    }

    return p;
}

template<typename Polygon_t>
auto length_impl(const Polygon_t &poly, polygon_tag) -> double
{
    double total = calculate_ring_perimeter(poly.outer());

    for (const auto &inner : poly.inners()) {
        total += calculate_ring_perimeter(inner);
    }

    return total;
}

} // namespace detail

// API
template<GeometryConcept G>
[[nodiscard]]
constexpr auto length(const G& g) -> double
{
    return detail::length_impl(g, geometry_tag_t<G>{});
}

} // namespace tl