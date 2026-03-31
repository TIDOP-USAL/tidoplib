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

template<PointConcept Point>
auto boundary_impl(const Point &, point_tag)
{
    return GeometryCollection<Point>{};
}

template<PointConcept Point>
auto boundary_impl(const Segment<Point> &segment, segment_tag)
{
    return MultiPoint<Point>{segment.pt1(), segment.pt2()};
}

template<PointConcept Point>
auto boundary_impl(const LineString<Point> &lineString,
                   linestring_tag) -> MultiPoint<Point>
{
    MultiPoint<Point> result;

    if (lineString.size() < 2)
        return result;

    if (lineString.isClosed()) {
        result.push_back(lineString.front());
        result.push_back(lineString.back());
    }

    return result;
}

template<PointConcept Point>
auto boundary_impl(const Polygon<Point> &polygon,
                   polygon_tag) -> MultiLineString<Point>
{
    MultiLineString<Point> result;

    if (polygon.outer().isEmpty())
        return result;

    result.push_back(LineString(polygon.outer()));

    for (const auto &hole : polygon.inners()) {
        if (!hole.isEmpty()) {
            result.push_back(LineString(hole));
        }
    }

    return result;
}

template<PointConcept Point>
auto boundary_impl(const MultiPoint<Point> &, multipoint_tag)
{
    return GeometryCollection<Point>{};
}

template<PointConcept Point>
auto boundary_impl(const MultiLineString<Point> &multiLineString,
                   multilinestring_tag) -> MultiPoint<Point>
{
    std::unordered_map<Point, int, Hash<Point>> counts;

    for (const auto &lineString : multiLineString) {
        if (!lineString.isEmpty() && !(lineString.front() == lineString.back())) {
            counts[lineString.front()]++;
            counts[lineString.back()]++;
        }
    }

    MultiPoint<Point> result;
    for (auto &[pt, count] : counts) {
        if (count % 2 == 1)
            result.push_back(pt);
    }

    return result;
}

template<PointConcept Point>
auto boundary_impl(const MultiPolygon<Point> &multiPolygon,
                   multipolygon_tag) -> MultiLineString<Point>
{
    MultiLineString<Point> result;

    for (const auto &polygon : multiPolygon) {
        auto b = boundary(polygon);
        if (!b.isEmpty())
            result.insert(result.end(), 
                         std::make_move_iterator(b.begin()), 
                         std::make_move_iterator(b.end()));
    }

    return result;
}

template<PointConcept Point>
auto boundary_impl(const GeometryCollection<Point> &geometryCollection,
                   collection_tag) -> GeometryCollection<Point>
{
    GeometryCollection<Point> result;

    for (const auto &geom : geometryCollection) {
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