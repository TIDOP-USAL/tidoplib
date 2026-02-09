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

// 1. Boundary de un Punto (0D): Según OGC, es el conjunto vacío.
// Devolvemos una colección vacía (MultiPoint) del mismo tipo de punto.
template<typename Point_t>
auto boundary_impl(const Point_t &, point_tag)
{
    return GeometryCollection<Point_t>{};
}

// 2. Boundary de un Segmento: Son sus dos puntos extremos.
template<typename Point_t>
auto boundary_impl(const Segment<Point_t> &s, segment_tag)
{
    //MultiPoint<Point_t> result;
    //result.push_back(s.pt1());
    //result.push_back(s.pt2());
    return MultiPoint<Point_t>{s.pt1(), s.pt2()};
}

// 3. Boundary de un LineString (1D): 
// Según OGC (Regla del Punto Final): 
// - Si es cerrada (anillo), el boundary es vacío.
// - Si no es cerrada, son los puntos inicial y final.
// cerrado → vacío
// abierto → extremos
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

// 4. Boundary de un Polígono (2D): 
// El límite de un polígono es el conjunto de sus anillos (exterior e interiores) 
// representados como un MultiLineString.
template<typename Point_t>
auto boundary_impl(const Polygon<Point_t> &poly, polygon_tag) -> MultiLineString<Point_t>
{
    MultiLineString<Point_t> result;

    if (poly.outer().empty()) return result;

    result.push_back(LineString(poly.outer()));

    for (const auto &hole : poly.inners()) {
        result.push_back(LineString(hole));
    }

    return result;
}

// 5. Boundary de MultiPoint (0D): Siempre vacío.
template<typename Point_t>
auto boundary_impl(const MultiPoint<Point_t> &, multipoint_tag)
{
    return GeometryCollection<Point_t>{};
}

// 6. Boundary de MultiLineString (1D):
// Se aplica la "Mod 2 Rule": un punto está en el boundary si aparece un número impar 
// de veces como extremo en las líneas que componen la colección.
// - implementa cancelación
// - cumple OGC
// - reutiliza boundary(ls)
template<typename Point_t>
auto boundary_impl(const MultiLineString<Point_t> &mls, multilinestring_tag) -> MultiPoint<Point_t>
{
    std::unordered_map<Point_t, int, Hash<Point_t>> counts; // O un hash map si Point_t tiene hash

    for (const auto &ls : mls) {
        auto b = boundary(ls);
        for (const auto &pt : b) {
            counts[pt]++;
        }
        // o para evitar bucle:
        //if (!line.empty() && !(line.front() == line.back())) {
        //    point_counts[line.front()]++;
        //    point_counts[line.back()]++;
        //}
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
        if (!b.empty())
            result.insert(result.end(), b.begin(), b.end());
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
            if (!b.empty())
                result.push_back(b);
        }, geom);
    }

    return result;
}

} // namespace detail

template<typename Geometry_t>
auto boundary(const Geometry_t &g)
{
    static_assert(is_geometry_v<Geometry_t>,
        "The type must be a geometry. Check if geometry_traits is specialized for this type.");

    //if constexpr (!boundary_traits<G>::has_boundary) {
    //    return typename boundary_traits<G>::type{};
    //} else {
        return detail::boundary_impl(g, geometry_tag_t<Geometry_t>{});
    //}
}

} // namespace tl