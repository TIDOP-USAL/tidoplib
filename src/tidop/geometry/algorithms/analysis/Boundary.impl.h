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

/*! \file Boundary.impl.h
 * \brief Implementation details for boundary algorithm.
 * 
 * This file contains the tag-dispatched implementations of the boundary
 * algorithm for each geometry type. These functions are internal and
 * should not be used directly.
 */

#pragma once

namespace tl
{

namespace detail
{

template<typename Point_t>
auto boundary_impl(const Point_t &, point_tag)
{
    return GeometryCollection<Point_t>{};
}

template<typename Point_t>
auto boundary_impl(const Segment<Point_t> &s, segment_tag)
{
    return MultiPoint<Point_t>{s.pt1(), s.pt2()};
}

template<typename Point_t>
auto boundary_impl(const LineString<Point_t> &ls, linestring_tag) -> MultiPoint<Point_t>
{
    MultiPoint<Point_t> result;

    if (ls.size() < 2)
        return result;

    const auto &p0 = ls.front();
    const auto &p1 = ls.back();

    if (p0 != p1) {
        result.push_back(p0);
        result.push_back(p1);
    }

    return result;
}

template<typename Point_t>
auto boundary_impl(const Polygon<Point_t> &poly, polygon_tag) -> MultiLineString<Point_t>
{
    MultiLineString<Point_t> result;

    if (poly.outer().isEmpty()) 
        return result;

    result.push_back(LineString(poly.outer()));

    for (const auto &hole : poly.inners()) {
        if (!hole.isEmpty()) {
            result.push_back(LineString(hole));
        }
    }

    return result;
}

template<typename Point_t>
auto boundary_impl(const MultiPoint<Point_t> &, multipoint_tag)
{
    return GeometryCollection<Point_t>{};
}

template<typename Point_t>
auto boundary_impl(const MultiLineString<Point_t> &mls, multilinestring_tag) -> MultiPoint<Point_t>
{
    std::unordered_map<Point_t, int, Hash<Point_t>> counts;

    for (const auto &ls : mls) {
        if (!ls.isEmpty() && !(ls.front() == ls.back())) {
            counts[ls.front()]++;
            counts[ls.back()]++;
        }
    }

    MultiPoint<Point_t> result;
    for (auto &[pt, count] : counts) {
        if (count % 2 == 1)
            result.push_back(pt);
    }

    return result;
}

template<typename Point_t>
auto boundary_impl(const MultiPolygon<Point_t> &mp, multipolygon_tag) -> MultiLineString<Point_t>
{
    MultiLineString<Point_t> result;

    for (const auto &poly : mp) {
        auto b = boundary(poly);
        if (!b.isEmpty())
            result.insert(result.end(), 
                         std::make_move_iterator(b.begin()), 
                         std::make_move_iterator(b.end()));
    }

    return result;
}

template<typename Point_t>
auto boundary_impl(const GeometryCollection<Point_t> &gc, collection_tag) -> GeometryCollection<Point_t>
{
    GeometryCollection<Point_t> result;

    for (const auto &geom : gc) {
        std::visit([&](const auto &arg) {
            const auto &geometry = arg.get();
            using GeometryType = std::decay_t<decltype(geometry)>;
            auto b = boundary_impl(geometry, geometry_tag_t<GeometryType>{});
            if (!b.isEmpty())
                result.push_back(std::move(b));
        }, geom);
    }

    return result;
}

} // namespace detail


template<GeometryConcept G>
[[nodiscard]]
auto boundary(const G &geometry)
{
    return detail::boundary_impl(geometry, geometry_tag_t<G>{});
}

} // namespace tl