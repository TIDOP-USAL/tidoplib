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

/* Point - Point */
template<typename Point_t>
auto closest_points_impl(const Point_t& p1, 
                        const Point_t& p2, 
                        point_tag, 
                        point_tag) -> GeometryCollection<Point_t>
{
	GeometryCollection<Point_t> geometry_collection;
	geometry_collection.addPoint(p1);
	geometry_collection.addPoint(p2);
    return geometry_collection;
}

/* Point - Segment */
template<typename Point_t>
auto closest_points_impl(const Point_t& point, 
                        const Segment<Point_t>& segment,
                        point_tag, 
                        segment_tag) -> GeometryCollection<Point_t>
{
    auto closest = project(point, segment);
	GeometryCollection<Point_t> geometry_collection;
	geometry_collection.addPoint(point);
	geometry_collection.addPoint(closest);
    return geometry_collection;
}

/* Segment - Segment */
template<typename Point_t>
auto closest_points_impl(const Segment<Point_t>& s1, 
                         const Segment<Point_t>& s2,
                         segment_tag,
                         segment_tag) -> std::pair<Point_t, Point_t>
{
    using calc_t = typename point_traits<Point_t>::calculation_type;
    
    auto u = s1.pt2() - s1.pt1();
    auto v = s2.pt2() - s2.pt1();
    auto w = s1.pt1() - s2.pt1();

    calc_t a = dotProduct(u, u);         // always >= 0
    calc_t b = dotProduct(u, v);
    calc_t c = dotProduct(v, v);         // always >= 0
    calc_t d = dotProduct(u, w);
    calc_t e = dotProduct(v, w);
    calc_t D = a * c - b * b;     // determinante
    calc_t sc, sN, sD = D;        // s = sN / sD, default sD = D
    calc_t tc, tN, tD = D;        // t = tN / tD, default tD = D

    constexpr calc_t eps = std::numeric_limits<calc_t>::epsilon() * 100;

    // Calcular los parámetros de los puntos más cercanos en las rectas infinitas
    if (D < eps) { // Las rectas son casi paralelas
        sN = 0.0;  // Forzamos s a 0 para resolver la indeterminación
        sD = 1.0;
        tN = e;
        tD = c;
    } else {       // Rectas no paralelas
        sN = (b * e - c * d);
        tN = (a * e - b * d);
        if (sN < 0.0) {        // s < 0 -> el punto más cercano está "atrás" de s1.pt1
            sN = 0.0;
            tN = e;
            tD = c;
        } else if (sN > sD) {  // s > 1 -> el punto está "delante" de s1.pt2
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    // Ahora ajustamos t al rango [0, 1]
    if (tN < 0.0) {            // t < 0
        tN = 0.0;
        // Reajustamos s basándonos en t = 0
        if (-d < 0.0) sN = 0.0;
        else if (-d > a) sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) {      // t > 1
        tN = tD;
        // Reajustamos s basándonos en t = 1
        if ((-d + b) < 0.0) sN = 0;
        else if ((-d + b) > a) sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }

    // Parámetros finales
    sc = (std::abs(sN) < eps ? 0.0 : sN / sD);
    tc = (std::abs(tN) < eps ? 0.0 : tN / tD);

    auto point1 = s1.pt1() + (u * sc);
	auto point2 = s2.pt1() + (v * tc);
	
	GeometryCollection<Point_t> geometry_collection;
	geometry_collection.addPoint(point1);
	geometry_collection.addPoint(point2);
    return geometry_collection;
}

/* Point - LineString */
template<typename Point_t>
auto closest_points_impl(const Point_t &point, 
                         const LineString<Point_t> &line,
                         point_tag, 
                         linestring_tag) -> GeometryCollection<Point_t>
{
    if (line.empty()) return {};
    
    double minDist = std::numeric_limits<double>::max();
    Point_t closest = line[0];
    
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        Segment<Point_t> seg(line[i], line[i + 1]);
        auto closest_on_segment = project(point, seg);
        double dist = distance(point, closest_on_segment);
        
        if (dist < minDist) {
            minDist = dist;
            closest = closest_on_segment;
        }
    }
    
	GeometryCollection<Point_t> geometry_collection;
	geometry_collection.addPoint(point);
	geometry_collection.addPoint(closest);
    return geometry_collection;
}


template<typename G1, typename G2, typename Tag1, typename Tag2>
auto closest_points_impl(const G1 &g1, const G2 &g2, Tag1 t1, Tag2 t2) -> GeometryCollection<typename geometry_traits<G1>::point_type>
{
    return closest_points_impl(g2, g1, t2, t1);
}

} // namespace detail

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
auto closestPoints(const G1 &g1, const G2 &g2)  -> GeometryCollection<typename geometry_traits<G1>::point_type>
{
    static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    static_assert(is_geometry_v<G2>, "Second argument must be a geometry");
    static_assert((std::is_same_v<
        geometry_traits<G1>::point_type,
        geometry_traits<G2>::point_type>),
        "All geometries must use the same point type.");

    //TODO: habría que añadir isEmpty() a Geometry
	//if (g1.isEmpty() || g2.isEmpty()) return {};
	
    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;
    return detail::closest_points_impl(g1, g2, tag1, tag2);
}


} // namespace tl