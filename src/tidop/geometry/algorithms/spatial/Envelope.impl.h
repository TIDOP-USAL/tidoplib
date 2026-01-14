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

#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{

namespace detail
{

template<typename Point_t>
auto envelope_impl(const Point_t &p, point_tag) 
{
    return BoundingBox<Point_t>(p, p);
}

template<typename Segment_t>
auto envelope_impl(const Segment_t &s, segment_tag)
{
    return BoundingBox<typename Segment_t::value_type>(s.pt1(), s.pt2());
}

template<typename Container_t>
auto envelope_from_container(const Container_t &container)
{
    using Point_t = typename Container_t::value_type;
    if (container.empty()) return BoundingBox<Point_t>();

    Point_t min_p = container[0];
    Point_t max_p = container[0];

    for (const auto &p : container) {
        for (size_t i = 0; i < VectorTraits<Point_t>::size; ++i) {
            if (p[i] < min_p[i]) min_p[i] = p[i];
            if (p[i] > max_p[i]) max_p[i] = p[i];
        }
    }
    return BoundingBox<Point_t>(min_p, max_p);
}


template<typename Polygon_t>
auto envelope_impl(const Polygon_t &polygon, polygon_tag)
{
    return envelope_from_container(polygon.outer());
}

template<typename LineString_t>
auto envelope_impl(const LineString_t &lineString, linestring_tag)
{
    return envelope_from_container(lineString);
}

template<typename MultiPoint_t>
auto envelope_impl(const MultiPoint_t &mp, multipoint_tag)
{
    return envelope_from_container(mp);
}

template<typename MultiLineString_t>
auto envelope_impl(const MultiLineString_t &multiLineString, multilinestring_tag)
{
    using Point_t = typename geometry_traits<MultiLineString_t>::point_type;
    BoundingBox<Point_t> total;
    for (const auto &line : multiLineString) {
        total = merge(total, envelope_impl(line, linestring_tag{}));
    }
    return total;
}

template<typename MultiPolygon_t>
auto envelope_impl(const MultiPolygon_t &mp, multipolygon_tag)
{
    using Point_t = typename geometry_traits<MultiPolygon_t>::point_type;
    BoundingBox<Point_t> total;
    for (const auto &poly : mp) {
        total = merge(total, envelope_impl(poly, polygon_tag{}));
    }
    return total;
}

} // namespace detail


template<typename Geometry_t>
auto envelope(const Geometry_t &g) 
{
    return detail::envelope_impl(g, geometry_tag_t<Geometry_t>{});
}

} // namespace tl