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

#include <variant>
#include <type_traits>
#include "Envelope.h"

namespace tl
{

namespace detail
{

template<typename Point_t>
auto envelope_impl(const Point_t &p, point_tag) 
{
    return BoundingBox<Point_t>(p, p);
}

template<typename Point_t>
auto envelope_impl(const Segment<Point_t> &s, segment_tag)
{
    return BoundingBox<Point_t>(s.pt1(), s.pt2());
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

template<typename Point_t>
auto envelope_impl(const MultiLineString<Point_t> &multiLineString, multilinestring_tag)
{
    BoundingBox<Point_t> total;
    for (const auto &line : multiLineString) {
        total = merge(total, envelope_impl(line, linestring_tag{}));
    }
    return total;
}

template<typename Point_t>
auto envelope_impl(const MultiPolygon<Point_t> &mp, multipolygon_tag)
{
    BoundingBox<Point_t> bbox;
    for (const auto &poly : mp) {
        bbox = merge(bbox, envelope_impl(poly, polygon_tag{}));
    }
    return bbox;
}

template<typename Point_t>
auto envelope_impl(const GeometryCollection<Point_t> &geometryCollection, collection_tag)
{
    BoundingBox<Point_t> bbox;

    for (const auto &geom : geometryCollection) {
        std::visit([&](auto &&arg) {

            const auto &geometry = arg.get();
            using GeometryType = std::decay_t<decltype(geometry)>;

            bbox = merge(bbox, envelope_impl(geometry, geometry_tag_t<GeometryType>{}));

        }, geom);
    }

    return bbox;
}

} // namespace detail

template<typename Geometry_t>
auto envelope(const Geometry_t &g) 
{
    return detail::envelope_impl(g, geometry_tag_t<Geometry_t>{});
}

template<typename Geometry_t, typename ...Geometries>
auto envelope(const Geometry_t &g, const Geometries & ...gs)
{
    auto bbox = envelope(g);
    ((bbox = merge(bbox, envelope(gs))), ...);
    return bbox;
}

} // namespace tl