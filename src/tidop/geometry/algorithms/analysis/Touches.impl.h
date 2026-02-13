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

template<typename Point_t>
auto touches_impl(const Point_t &,
                  const Point_t &,
                  point_tag,
                  point_tag) -> bool
{
    return false;
}

// Point – LineString
// 
// Un punto toca una línea si:
// 
// - está en su boundary
// - no está en el interior
template<typename Point_t>
auto touches_impl(const Point<Point_t>& pt,
                  const LineString<Point_t>& ls,
                  point_tag,
                  linestring_tag) -> bool
{
    //if (ls.size() < 2) return false;
	//
    //const auto& p0 = ls.front();
    //const auto& p1 = ls.back();
	//
    //if (p0 == p1) return false; // cerrada → no boundary
	//
    //return (pt == p0 || pt == p1);
	return locatePointOnLineString(ls, pt) == Location::Boundary;
}

// Point - Polygon
template<typename Point_t, typename Polygon_t>
auto touches_impl(const Point_t &point,
                  const Polygon_t &polygon,
                  point_tag, 
                  polygon_tag) -> bool 
{
    // Un punto toca un polígono si el punto está en el borde del polígono.
    return locatePointInPolygon(polygon, point) == Location::Boundary;
}

// LineString – LineString
// Dos líneas tocan si algún endpoint de una está sobre la otra
// pero no se cruzan en el interior
//
// Estrategia eficiente:
// - Si intersects(interior, interior) → ❌
// - Si intersects(boundary, boundary) → ✔
template<typename Point_t>
auto touches_impl(const LineString<Point_t> &a,
                  const LineString<Point_t> &b,
                  linestring_tag,
                  linestring_tag) -> bool
{
    // 1. Si se cruzan en interior → false
    if (intersects_interior(a, b))
        return false;

    // 2. Boundary vs Boundary
    auto ba = boundary(a);
    auto bb = boundary(b);

    return intersects(ba, bb);
}

// Polygon - Polygon
template<typename Polygon_t>
auto touches_impl(const Polygon_t& p1, 
                  const Polygon_t& p2,
                  polygon_tag, 
                  polygon_tag) -> bool 
{
    // Dos polígonos se tocan si sus bordes intersectan pero sus interiores no.
    if (!intersects(boundary(p1), boundary(p2))) return false;

    // Verificar que un punto interior de P1 no esté dentro de P2
    // (y viceversa para seguridad si son complejos)
    if (within(representative_point(p1), p2)) return false;
    if (within(representative_point(p2), p1)) return false;

    return true;
}

// Segment – Segment
// Se tocan pero no se solapan (no colineales)
template<typename Point_t>
bool touches_impl(const Segment<Point_t>& s1,
                  const Segment<Point_t>& s2,
                  segment_tag,
                  segment_tag)
{
    // comparten endpoint
    if (s1.pt1() == s2.pt1() ||
        s1.pt1() == s2.pt2() ||
        s1.pt2() == s2.pt1() ||
        s1.pt2() == s2.pt2())
    {
        // pero no colineales solapados
        return !overlaps(s1, s2);
    }

    return false;
}

template<typename Point_t, typename Geometry_t>
auto touches_impl(const GeometryCollection<Point_t> &gc,
                  const Geometry_t &geom,
                  collection_tag,
                  geometry_tag_t<Geometry_t>) -> bool
{
    for (const auto &item : gc) {
        bool hit = std::visit([&](auto &&arg) {
            return touches(arg.get(), geom);
            }, item);

        if (hit) return true;
    }

    return false;
}

template<typename G1, typename G2, typename T1, typename T2>
auto touches_impl(const G1& g1, const G2& g2, T1 t1, T2 t2) -> bool 
{
    return touches_impl(g2, g1, t2, t1);
}


template<typename Polygon_t>
auto representative_point_impl(const Polygon_t& poly, polygon_tag) 
{
    using Point_t = typename geometry_traits<Polygon_t>::point_type;
    auto bbox = envelope(poly);
    double mid_y = (bbox.min().y() + bbox.max().y()) / 2.0;

    // 1. Encontrar todas las intersecciones de la línea y = mid_y con los anillos
    std::vector<double> x_intersections;
    
    auto collect_intersections = [&](const auto& ring) {
        for (size_t i = 0, j = ring.size() - 1; i < ring.size(); j = i++) {
            const auto& p1 = ring[i];
            const auto& p2 = ring[j];
            if ((p1.y() <= mid_y && p2.y() > mid_y) || (p2.y() <= mid_y && p1.y() > mid_y)) {
                double x = p1.x() + (mid_y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
                x_intersections.push_back(x);
            }
        }
    };

    collect_intersections(poly.outer());
    for (const auto& hole : poly.inners()) collect_intersections(hole);

    std::sort(x_intersections.begin(), x_intersections.end());

    // 2. Los intervalos [x[i], x[i+1]] alternan entre estar dentro y fuera.
    // Buscamos el punto medio del intervalo más ancho que esté "dentro".
    double best_x = 0;
    double max_width = -1.0;

    for (size_t i = 0; i + 1 < x_intersections.size(); i++) {
        double mid_x = (x_intersections[i] + x_intersections[i+1]) / 2.0;
        Point_t candidate(mid_x, mid_y);
        
        // Verificamos si el punto medio está dentro (usando tu algoritmo de ray-casting)
        if (intersects(poly, candidate)) {
            double width = x_intersections[i+1] - x_intersections[i];
            if (width > max_width) {
                max_width = width;
                best_x = mid_x;
            }
        }
    }

    // Si no se encontró (caso degenerado), devolvemos el primer punto del outer
    if (max_width < 0) return poly.outer()[0];

    return Point_t(best_x, mid_y);
}

// Para LineString: El punto medio del segmento central
template<typename Point_t>
auto representative_point_impl(const LineString<Point_t>& line, linestring_tag)
{
    if (line.empty()) return Point_t{};
    return line[line.size() / 2];
}

// Para Puntos: Es el punto mismo
template<typename Point_t>
auto representative_point_impl(const Point_t& pt, point_tag)
{
    return pt;
}

template<typename G1, typename G2>
constexpr bool intersects_interior(const G1&, const G2&)
{
    return false;
}

template<typename Point_t>
bool intersects_interior(const Segment<Point_t>& s1,
                         const Segment<Point_t>& s2)
{
    // Intersección general
    if (!intersects(s1, s2))
        return false;

    // Comparten solo extremos → NO interior
    if (s1.pt1() == s2.pt1() ||
        s1.pt1() == s2.pt2() ||
        s1.pt2() == s2.pt1() ||
        s1.pt2() == s2.pt2())
    {
        // ¿Hay algo más que el punto?
        return overlaps(s1, s2);
    }

    // Si intersectan y no es solo endpoint → interior
    return true;
}

template<typename Point_t>
bool intersects_interior(const LineString<Point_t>& a,
                         const LineString<Point_t>& b)
{
    if (a.size() < 2 || b.size() < 2)
        return false;

    for (size_t i = 0; i + 1 < a.size(); ++i) {
        Segment<Point_t> sa(a[i], a[i + 1]);

        for (size_t j = 0; j + 1 < b.size(); ++j) {
            Segment<Point_t> sb(b[j], b[j + 1]);

            if (intersects_interior(sa, sb))
                return true;
        }
    }

    return false;
}

template<typename Point_t>
bool intersects_interior(const LineString<Point_t>& ls,
                         const Segment<Point_t>& s)
{
    if (ls.size() < 2) return false;

    for (size_t i = 0; i + 1 < ls.size(); ++i) {
        Segment<Point_t> si(ls[i], ls[i + 1]);
        if (intersects_interior(si, s))
            return true;
    }
    return false;
}

} // namespace detail


template<typename G1, typename G2>
auto touches(const G1 &g1, const G2 &g2) -> bool
{
    static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    static_assert(is_geometry_v<G2>, "Second argument must be a geometry");
    
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;

    static_assert(std::is_same_v<typename point_traits<P1>::value_type,
                                 typename point_traits<P2>::value_type>,
        "Points must have same coordinate type");

    auto dispatch = [&] {
        using tag1 = geometry_tag_t<G1>;
        using tag2 = geometry_tag_t<G2>;
        return detail::touches_impl(g1, g2, tag1{}, tag2{});
    };

    if constexpr (GeometryCollectionConcept<G1> || GeometryCollectionConcept<G2>) {
        return dispatch();
    } else if constexpr (MultiGeometryConcept<G1>) {
        for (const auto &a : g1) {
            if (touches(a, g2)) return true;
        }
        return false;
    } else if constexpr (MultiGeometryConcept<G2>) {
        for (const auto &b : g2) {
            if (touches(g1, b)) return true;
        }
        return false;
    } else {
        return dispatch();
    }
}


template<typename Geometry_t>
auto representative_point(const Geometry_t& g) 
{
    return detail::representative_point_impl(g, geometry_tag_t<Geometry_t>{});
}

} // namespace tl