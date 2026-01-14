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

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/Polygon.h"

namespace tl
{

namespace detail
{

template<typename Point_t>
auto buffer_impl(const Point_t &center, double distance, point_tag, int segments = 32) -> Polygon<Point_t>
{
    Polygon<Point_t> poly;
    auto &outer = poly.outer();

    if (distance <= 0) return poly;

    // Calculamos los puntos alrededor del círculo
    // El ángulo de cada paso es 2 * PI / segmentos
    const double step = (2.0 * consts::pi<double>) / static_cast<double>(segments);

    for (int i = 0; i < segments; ++i) {
        double angle = i * step;
        outer.push_back(Point_t(
            center.x() + static_cast<typename Point_t::value_type>(distance * std::cos(angle)),
            center.y() + static_cast<typename Point_t::value_type>(distance * std::sin(angle))
        ));
    }

    return poly;
}

template<typename Segment_t>
auto buffer_impl(const Segment_t &ln, double distance, segment_tag) -> Polygon<typename Segment_t::value_type>
{
    using Point_t = typename Segment_t::value_type;
    using T = typename Point_t::value_type;

    // Calculamos el ángulo del segmento
    // Usamos atan2 para mayor robustez que el azimut manual
    T dx_seg = ln.pt2().x() - ln.pt1().x();
    T dy_seg = ln.pt2().y() - ln.pt1().y();
    double angle = std::atan2(dy_seg, dx_seg);

    // Ángulo perpendicular para el desplazamiento lateral
    double perp_angle = angle + consts::half_pi<T>;

    T offsetX = static_cast<T>(distance * std::cos(perp_angle));
    T offsetY = static_cast<T>(distance * std::sin(perp_angle));

    // Generamos el polígono de 4 vértices (rectángulo orientado)
    Polygon<Point_t> poly;
    auto &outer = poly.outer();

    outer.push_back(Point_t(ln.pt1().x() + offsetX, ln.pt1().y() + offsetY));
    outer.push_back(Point_t(ln.pt2().x() + offsetX, ln.pt2().y() + offsetY));
    outer.push_back(Point_t(ln.pt2().x() - offsetX, ln.pt2().y() - offsetY));
    outer.push_back(Point_t(ln.pt1().x() - offsetX, ln.pt1().y() - offsetY));

    return poly;
}

//template<typename LineString_t>
//auto buffer_impl(const LineString_t &line, double distance, linestring_tag) -> Polygon<typename LineString_t::value_type>
//{
//    using Point_t = typename LineString_t::value_type;
//    using T = typename Point_t::value_type;
//
//    Polygon<Point_t> result;
//    if (line.size() < 2) return result;
//
//    std::vector<Point_t> left_side;
//    std::vector<Point_t> right_side;
//
//    for (size_t i = 0; i < line.size() - 1; ++i) {
//        const Point_t &p1 = line[i];
//        const Point_t &p2 = line[i + 1];
//
//        // Vector director y perpendicular
//        T dx = p2.x() - p1.x();
//        T dy = p2.y() - p1.y();
//        double len = std::sqrt(dx * dx + dy * dy);
//        if (len < 1e-9) continue;
//
//        T ux = static_cast<T>((dx / len) * distance);
//        T uy = static_cast<T>((dy / len) * distance);
//
//        // Puntos desplazados (normal perpendicular: -uy, ux)
//        Point_t left1(p1.x() - uy, p1.y() + ux);
//        Point_t left2(p2.x() - uy, p2.y() + ux);
//        Point_t right1(p1.x() + uy, p1.y() - ux);
//        Point_t right2(p2.x() + uy, p2.y() - ux);
//
//        left_side.push_back(left1);
//        left_side.push_back(left2);
//
//        // El lado derecho lo guardamos para luego recorrerlo a la inversa
//        right_side.push_back(right1);
//        right_side.push_back(right2);
//    }
//
//    // Construir el anillo exterior (Outer Ring)
//    auto &outer = result.outer();
//
//    // 1. Añadimos el lado izquierdo (sentido p1 -> pN)
//    for (const auto &p : left_side) outer.push_back(p);
//
//    // 2. Añadimos el lado derecho (sentido pN -> p1) para cerrar el polígono
//    for (auto it = right_side.rbegin(); it != right_side.rend(); ++it) {
//        outer.push_back(*it);
//    }
//
//    return result;
//}
template<typename LineString_t>
auto buffer_impl(const LineString_t &line, double distance, linestring_tag) -> Polygon<typename LineString_t::value_type>
{
    using Point_t = typename LineString_t::value_type;
    using T = typename Point_t::value_type;

    Polygon<Point_t> result;
    if (line.size() < 2) return result;

    std::vector<Point_t> left_side;
    std::vector<Point_t> right_side;

    for (size_t i = 0; i < line.size() - 1; ++i) {
        const Point_t &p1 = line[i];
        const Point_t &p2 = line[i + 1];

        T dx = p2.x() - p1.x();
        T dy = p2.y() - p1.y();
        double len = std::sqrt(dx * dx + dy * dy);
        if (len < 1e-9) continue;

        // Vector unitario director (ux, uy) y perpendicular (nx, ny)
        T ux = static_cast<T>(dx / len);
        T uy = static_cast<T>(dy / len);
        T nx = -uy;
        T ny = ux;

        // --- TRATAMIENTO DE EXTREMOS (SQUARE CAPS) ---
        // Si es el primer punto, extendemos hacia atrás
        if (i == 0) {
            left_side.push_back(Point_t(p1.x() + (nx - ux) * distance, p1.y() + (ny - uy) * distance));
            right_side.push_back(Point_t(p1.x() + (-nx - ux) * distance, p1.y() + (-ny - uy) * distance));
        }

        // Puntos paralelos estándar
        left_side.push_back(Point_t(p1.x() + nx * distance, p1.y() + ny * distance));
        left_side.push_back(Point_t(p2.x() + nx * distance, p2.y() + ny * distance));

        right_side.push_back(Point_t(p1.x() - nx * distance, p1.y() - ny * distance));
        right_side.push_back(Point_t(p2.x() - nx * distance, p2.y() - ny * distance));

        // Si es el último segmento, extendemos hacia adelante
        if (i == line.size() - 2) {
            left_side.push_back(Point_t(p2.x() + (nx + ux) * distance, p2.y() + (ny + uy) * distance));
            right_side.push_back(Point_t(p2.x() + (-nx + ux) * distance, p2.y() + (-ny + uy) * distance));
        }
    }

    auto &outer = result.outer();
    for (const auto &p : left_side) outer.push_back(p);
    for (auto it = right_side.rbegin(); it != right_side.rend(); ++it) outer.push_back(*it);

    // Cerrar el polígono
    if (!outer.empty()) outer.push_back(outer[0]);

    return result;
}

template<typename Ring_t>
auto buffer_ring_helper(const Ring_t &ring, double distance) -> Ring_t
{
    using Point_t = typename Ring_t::value_type;
    using T = typename Point_t::value_type;

    Ring_t result;
    size_t n = ring.size();
    if (n < 3) return result;

    // Si el último es igual al primero, no lo procesamos doblemente en el bucle
    bool is_closed = (ring[0] == ring[n - 1]);
    size_t limit = is_closed ? n - 1 : n;

    for (size_t i = 0; i < limit; ++i) {
        // Obtenemos segmento anterior y siguiente para calcular la bisectriz
        const auto &p_prev = ring[(i + limit - 1) % limit];
        const auto &p_curr = ring[i];
        const auto &p_next = ring[(i + 1) % limit];

        // Vectores de los segmentos
        double v1x = p_curr.x() - p_prev.x();
        double v1y = p_curr.y() - p_prev.y();
        double v2x = p_next.x() - p_curr.x();
        double v2y = p_next.y() - p_curr.y();

        double l1 = std::sqrt(v1x * v1x + v1y * v1y);
        double l2 = std::sqrt(v2x * v2x + v2y * v2y);

        if (l1 < 1e-9 || l2 < 1e-9) continue;

        // Normales unitarias
        double n1x = -v1y / l1; double n1y = v1x / l1;
        double n2x = -v2y / l2; double n2y = v2x / l2;

        // Promedio de normales (dirección de la bisectriz)
        double bisex = n1x + n2x;
        double bisey = n1y + n2y;
        double blen = std::sqrt(bisex * bisex + bisey * bisey);

        if (blen < 1e-9) { // Segmentos colineales
            result.push_back(Point_t(p_curr.x() + n1x * distance, p_curr.y() + n1y * distance));
        } else {
            // Factor de escala para que el borde esté exactamente a 'distance'
            // cos(theta) = n1 . n_bisectriz
            double cos_theta = (n1x * bisex + n1y * bisey) / blen;
            double s = distance / cos_theta;
            result.push_back(Point_t(p_curr.x() + (bisex / blen) * s, p_curr.y() + (bisey / blen) * s));
        }
    }

    if (!result.empty()) result.push_back(result[0]); // Cerrar
    return result;
}

template<typename Polygon_t>
auto buffer_impl(const Polygon_t &poly, double distance, polygon_tag) -> Polygon<typename Polygon_t::value_type>
{
    using Point_t = typename Polygon_t::value_type;
    Polygon_t result;

    // 1. Buffer del anillo exterior (infla el polígono)
    // Usamos la lógica de linestring pero asegurándonos de que trate el cierre
    if (!poly.outer().empty()) {
        result.outer() = detail::buffer_ring_helper(poly.outer(), distance);
    }

    // 2. Buffer de los anillos interiores (huecos)
    // Nota: Para los huecos, la distancia actúa al revés (encoge el hueco)
    for (const auto &hole : poly.inners()) {
        if (!hole.empty()) {
            auto buffered_hole = detail::buffer_ring_helper(hole, -distance);
            if (!buffered_hole.empty()) {
                result.addInner(buffered_hole);
            }
        }
    }

    return result;
}

template<typename MultiPoint_t>
auto buffer_impl(const MultiPoint_t &mp, double distance, multipoint_tag) -> MultiPolygon<typename MultiPoint_t::value_type>
{
    MultiPolygon<typename MultiPoint_t::value_type> result;
    for (const auto &pt : mp) {
        result.push_back(buffer(pt, distance));
    }
    return result;
}

} // namespace detail


template<typename Geometry_t>
auto buffer(const Geometry_t &geometry, double distance)
{
    return detail::buffer_impl(geometry, distance, geometry_tag_t<Geometry_t>{});
}

} // namespace tl