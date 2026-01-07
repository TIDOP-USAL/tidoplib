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
	
namespace geometry
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


template<typename Poly_t>
auto envelope_impl(const Poly_t &poly, polygon_tag)
{
    return envelope_from_container(poly.outer());
}

template<typename MultiPoint_t>
auto envelope_impl(const MultiPoint_t &mp, multipoint_tag)
{
    return envelope_from_container(mp);
}

} // namespace detail


template<typename Geometry_t>
auto envelope(const Geometry_t &g) 
{
    return detail::envelope_impl(g, geometry_tag_t<Geometry_t>{});
}

} // namespace tl
} // namespace geometry