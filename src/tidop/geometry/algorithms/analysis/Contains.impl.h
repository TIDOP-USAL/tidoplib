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

//#include <cmath>
//
//#include "tidop/geometry/algorithms/spatial/Envelope.h"
//#include "tidop/geometry/spatial/BoundingBox.h"
//#include "tidop/geometry/primitives/Point.h"
//#include "tidop/geometry/primitives/LineString.h"
//#include "tidop/geometry/primitives/Polygon.h"
//#include "tidop/geometry/primitives/MultiPolygon.h"

namespace tl
{

namespace detail 
{

// Point-Point (un punto contiene a otro si son iguales)
template<typename Point_t>
auto contains_impl(const Point_t &pt1,
                   const Point_t &pt2,
                   point_tag,
                   point_tag)
{
    return pt1 == pt2;
}

template<typename Point_t>
auto contains_impl(const Segment<Point_t> &line,
                   const Point_t &point,
                   segment_tag,
                   point_tag) -> bool
{
    return false;
}

template<typename Point_t>
auto contains_impl(const Point_t &point,
                   const Segment<Point_t> &line,
                   point_tag,
                   segment_tag) -> bool
{
    return false;
}

// LineString - Point
// OGC: un punto no puede estar en el interior de un LineString, aunque caiga sobre un segmento.
template<typename Point_t>
auto contains_impl(const LineString<Point_t> &line, 
                   const Point_t &point,
                   linestring_tag,
                   point_tag) -> bool
{
    return false;
}

template<typename Point_t>
auto contains_impl(const Point_t &point,
                   const LineString<Point_t> &line,
                   point_tag,
                   linestring_tag) -> bool
{
    return false;
}

// BoundingBox-Point
template<typename Point_t>
auto contains_impl(const BoundingBox<Point_t> &box,
                   const Point_t &pt, 
                   bbox_tag,
                   point_tag)
{
    for (size_t i = 0; i < VectorTraits<Point_t>::size; ++i) {
        if (pt[i] < box.min()[i] || pt[i] > box.max()[i]) {
            return false;
        }
    }

    return true;
}

// Polygon-Point
template<typename Polygon_t, typename Point_t>
auto contains_impl(const Polygon_t &polygon,
                   const Point_t &pt, 
                   polygon_tag,
                   point_tag) -> bool
{
    return locatePointInPolygon(polygon, pt) == Location::Interior;
}

template<typename Polygon_t, typename Point_t>
auto contains_impl(const Point_t &pt,
                   const Polygon_t &polygon,
                   point_tag,   
                   polygon_tag) -> bool
{
    return false;
}

// Polygon-LineString (todos los puntos de la línea están dentro del polígono, incluyendo el borde)
template<typename Point_t>
auto contains_impl(const Polygon<Point_t> &polygon, 
                   const LineString<Point_t> &line,
                   polygon_tag, 
                   linestring_tag) -> bool
{
    if (line.empty()) return true; // Línea vacía siempre está contenida

    // 1️ - Todos los puntos de la línea deben estar en el interior (no en borde)
    for (const auto &pt : line) {
        if (locatePointInPolygon(polygon, pt) != Location::Interior) {
            return false;
        }
    }

    // 2️ - Ningún segmento debe intersectar los huecos
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        Segment<Point_t> seg(line[i], line[i + 1]);
        for (const auto &hole : polygon.inners()) {
            if (intersects(seg, hole)) {
                return false;
            }
        }
    }

    return true;
}

template<typename Point_t>
auto contains_impl(const LineString<Point_t> &line, 
                   const Polygon<Point_t> &polygon,
                   linestring_tag,
                   polygon_tag) -> bool
{
    return false;
}

// Polygon-Polygon (un polígono contiene a otro)
template<typename Point_t>
auto contains_impl(const Polygon<Point_t> &container, 
                   const Polygon<Point_t> &containee,
                   polygon_tag, 
                   polygon_tag) -> bool
{
    // 1️ - Todos los puntos del containee deben estar dentro del container (interior)
    for (const auto &pt : containee.outer()) {
        if (locatePointInPolygon(container, pt) != Location::Interior) {
            return false;
        }
    }

    // 2️ - Ningún segmento del containee intersecta los huecos del container
    for (size_t i = 0; i < containee.outer().size(); ++i) {
        size_t j = (i + 1) % containee.outer().size();
        Segment<Point_t> seg(containee.outer()[i], containee.outer()[j]);
        for (const auto &hole : container.inners()) {
            if (intersects(seg, hole)) {
                return false;
            }
        }
    }

    // 3️ - Los huecos del containee deben estar totalmente dentro de los huecos del container o fuera completamente
    for (const auto &hole : containee.inners()) {
        for (const auto &pt : hole) {
            if (!pointInAnyHole(pt, container) && locatePointInPolygon(container, pt) != Location::Interior) {
                return false;
            }
        }
    }

    return true;
}

// MultiPolygon - Point (un multipolígono contiene un punto)
template<typename Point_t>
auto contains_impl(const MultiPolygon<Point_t> &multiPolygon,
                   const Point_t &pt, 
                   multipolygon_tag, 
                   point_tag) -> bool
{
    for (const auto &polygon : multiPolygon) {
        if (contains(polygon, pt))
            return true;
    }

    return false;
}

// MultiPolygon - Polygon
template<typename Point_t>
auto contains_impl(const MultiPolygon<Point_t> &multiPolygon, 
                   const Polygon<Point_t> &polygon,
                   multipolygon_tag, 
                   polygon_tag) -> bool
{
    for (const auto &containerPoly : multiPolygon) {
        if (contains(containerPoly, polygon)) {
            return true;
        }
    }
    return false;
}

/* GeometryCollection with any geometry */
template<typename Point_t, typename Geometry_t>
auto contains_impl(const GeometryCollection<Point_t> &gc,
                   const Geometry_t &geom,
                   collection_tag,
                   geometry_tag_t<Geometry_t>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
            return contains(arg.get(), geom);
            }, item);

        if (hit) return true;
    }

    return false;
}

} // namespace detail


template<typename G1, typename G2>
[[nodiscard]]
auto contains(const G1 &g1, const G2 &g2) -> bool
{
    static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    static_assert(is_geometry_v<G2>, "Second argument must be a geometry");

    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;
    return detail::contains_impl(g1, g2, tag1{}, tag2{});

    //auto dispatch = [&] {
    //    using tag1 = geometry_tag_t<G1>;
    //    using tag2 = geometry_tag_t<G2>;
    //    return detail::contains_impl(g1, g2, tag1{}, tag2{});
    //};

    //if constexpr (GeometryCollectionConcept<G1> || GeometryCollectionConcept<G2>) {
    //    return dispatch();
    //} else if constexpr (MultiGeometryConcept<G1> && MultiGeometryConcept<G2>) {
    //    for (const auto &a : g1) {
    //        for (const auto &b : g2)
    //            if (contains(a, b)) return true;
    //    }
    //    return false;
    //} else if constexpr (!MultiGeometryConcept<G1> && MultiGeometryConcept<G2>) {
    //    return false;
    //} else {
    //    return dispatch();
    //}
}

} // namespace tl