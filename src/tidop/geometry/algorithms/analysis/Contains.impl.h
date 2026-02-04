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
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPolygon.h"

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

// LineString - Point(un punto en la línea, pero no necesariamente en los extremos, depende de la definición)
template<typename Point_t>
auto contains_impl(const LineString<Point_t> &line, 
                   const Point_t &point,
                   linestring_tag,
                   point_tag) -> bool
{
    if (line.empty()) return false;
    if (line.size() == 1) return line[0] == point;

    // Revisar todos los segmentos
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        Segment<Point_t> seg(line[i], line[i + 1]);

        // Proyectar punto en segmento
        auto proj = project(point, seg);

        // Está en el segmento pero NO en los extremos
        if (!proj.isBeforeStart() && !proj.isAfterEnd() &&
            distance(point, proj.closestPoint) < std::numeric_limits<double>::epsilon()) {
            // Chequear que no sea un extremo
            if (distance(point, seg.pt1()) > std::numeric_limits<double>::epsilon() &&
                distance(point, seg.pt2()) > std::numeric_limits<double>::epsilon()) {
                return true;
            }
        }
    }

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

// Polygon-LineString (todos los puntos de la línea están dentro del polígono, incluyendo el borde)
template<typename Point_t>
auto contains_impl(const Polygon<Point_t> &polygon, 
                   const LineString<Point_t> &line,
                   polygon_tag, 
                   linestring_tag) -> bool
{
    if (line.empty()) return true; // Polígono contiene línea vacía

    // Todos los puntos deben estar dentro (no en el borde)
    for (const auto &point : line) {
        if (!contains(polygon, point)) {
            return false;
        }
    }

    // Además, la línea no debe cruzar ningún hueco
    // Esto es más complejo - necesitas verificar intersecciones
    // Implementación básica por ahora
    return true;
}

// Polygon-Polygon (un polígono contiene a otro)
template<typename Point_t>
auto contains_impl(const Polygon<Point_t> &container, 
                   const Polygon<Point_t> &containee,
                   polygon_tag, 
                   polygon_tag) -> bool
{
    // 1. Todos los puntos del polígono contenido deben estar dentro del contenedor
    for (const auto &point : containee.outer()) {
        if (!contains(container, point)) {
            return false;
        }
    }

    // 2. Los huecos del contenedor NO deben intersectar el polígono contenido
    // 3. Los huecos del contenido deben estar dentro de los huecos del contenedor o fuera completamente

    // Implementación simplificada: chequea si un punto del borde está dentro
    // y si no hay intersección entre los bordes (excepto posiblemente tangente)
    // Esto es una simplificación - la implementación completa es compleja

    return true; // Placeholder
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

} // namespace detail


template<typename G1, typename G2>
auto contains(const G1 &g1, const G2 &g2) -> bool
{
    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;
    return detail::contains_impl(g1, g2, tag1{}, tag2{});
}

} // namespace tl