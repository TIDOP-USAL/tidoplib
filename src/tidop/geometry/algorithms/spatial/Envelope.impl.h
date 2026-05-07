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

template<PointConcept Point>
auto envelope_impl(const Point &point,
                   point_tag)
{
    using P = remove_measure_t<Point>;
    return BoundingBox<P>(static_cast<P>(point), static_cast<P>(point));
}

template<SegmentConcept Segment>
auto envelope_impl(const Segment &segment,
                   segment_tag)
{
    using P = remove_measure_t<geometry_traits<Segment>::point_type>;
    return BoundingBox<P>(static_cast<P>(segment.pt1()), 
                          static_cast<P>(segment.pt2()));
}

template<typename Container_t>
auto envelope_from_container(const Container_t &container)
{
    using Point_t = typename Container_t::value_type;
    using P = remove_measure_t<Point_t>;

    // Solo se devería llamar desde envelope, que ya verifica si la geometría está vacía, pero se añade esta comprobación por seguridad.
    if (container.isEmpty()) return BoundingBox<P>();

    Point_t min_point = container[0];
    Point_t max_point = container[0];

    for (const auto &point : container) {
        for (size_t i = 0; i < point_traits<Point_t>::spatial_dims; ++i) {
            if (point[i] < min_point[i]) min_point[i] = point[i];
            if (point[i] > max_point[i]) max_point[i] = point[i];
        }
    }

    return BoundingBox<P>(static_cast<P>(min_point), static_cast<P>(max_point));
}


template<PolygonConcept Polygon>
auto envelope_impl(const Polygon &polygon, 
                   polygon_tag)
{
    return envelope_from_container(polygon.outer());
}

template<LineStringConcept LineString>
auto envelope_impl(const LineString &lineString, 
                   linestring_tag)
{
    return envelope_from_container(lineString);
}

template<typename MultiPoint_t>
auto envelope_impl(const MultiPoint_t &multiPoint,
                   multipoint_tag)
{
    return envelope_from_container(multiPoint);
}

template<typename Point_t>
auto envelope_impl(const MultiLineString<Point_t> &multiLineString,
                   multilinestring_tag)
{
    BoundingBox<remove_measure_t<Point_t>> bbox;

    for (const auto &lineString : multiLineString) {
        bbox = merge(bbox, envelope_impl(lineString, linestring_tag{}));
    }

    return bbox;
}

template<typename Point_t>
auto envelope_impl(const MultiPolygon<Point_t> &multiPolygon,
                   multipolygon_tag)
{
    BoundingBox<remove_measure_t<Point_t>> bbox;

    for (const auto &polygon : multiPolygon) {
        bbox = merge(bbox, envelope_impl(polygon, polygon_tag{}));
    }

    return bbox;
}

template<GeometryCollectionConcept GeometryCollection>
auto envelope_impl(const GeometryCollection &geometryCollection, 
                   collection_tag)
{
    using P = remove_measure_t<geometry_traits<GeometryCollection>::point_type>;

    BoundingBox<P> bbox;

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

template<GeometryConcept G>
[[nodiscard]]
auto envelope(const G &g)
{
    if (g.isEmpty()) return BoundingBox<remove_measure_t<typename geometry_traits<G>::point_type>>();

    return detail::envelope_impl(g, geometry_tag_t<G>{});
}

template<typename Geometry_t, typename ...Geometries>
[[nodiscard]]
auto envelope(const Geometry_t &g, const Geometries & ...gs)
{
    static_assert((is_geometry_v<Geometry_t> && ... && is_geometry_v<Geometries>),
        "All types must be geometries. Check if geometry_traits is specialized for each type.");

    // Verificar que todos los tipos de punto son iguales
    static_assert((std::is_same_v<typename geometry_traits<Geometry_t>::point_type,
                                  typename geometry_traits<Geometries>::point_type> && ...),
        "All geometries must use the same point type.");

    auto bbox = envelope(g);
    ((bbox = merge(bbox, envelope(gs))), ...);
    return bbox;
}

} // namespace tl