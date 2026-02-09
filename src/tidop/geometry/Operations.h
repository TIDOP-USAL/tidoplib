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

template<typename Point_t>	
auto isBetween(const Point_t &p,
                    const Point_t &q,
                    const Point_t &r) -> bool 
{
    return q.x() >= std::min(p.x(), r.x()) &&
           q.x() <= std::max(p.x(), r.x()) &&
           q.y() >= std::min(p.y(), r.y()) &&
           q.y() <= std::max(p.y(), r.y());
}
		
enum class WindingOrder
{
    Colinear,
    Clockwise,
    CounterClockwise
};

template<typename Point_t>
auto orientation(const Point_t &a,
                 const Point_t &b,
                 const Point_t &c) -> WindingOrder
{
    using calc_t = typename point_traits<Point_t>::calculation_type;
    const calc_t val = (static_cast<calc_t>(b.y()) - static_cast<calc_t>(a.y())) *
        (static_cast<calc_t>(c.x()) - static_cast<calc_t>(b.x())) -
        (static_cast<calc_t>(b.x()) - static_cast<calc_t>(a.x())) *
        (static_cast<calc_t>(c.y()) - static_cast<calc_t>(b.y()));

    constexpr calc_t eps = std::numeric_limits<calc_t>::epsilon() * static_cast<calc_t>(100);

    if (std::abs(val) <= eps) return WindingOrder::Colinear; // colineal
    return (val > static_cast<calc_t>(0)) ? WindingOrder::Clockwise : WindingOrder::CounterClockwise; // horario o antihorario
}

template<typename Point>
bool pointOnSegment(const Point &a,
                    const Point &b,
                    const Point &p)
{
    // Colinealidad
    if (orientation(a, b, p) != WindingOrder::Colinear) {
        return false;
    }

    // Dentro del bounding box del segmento
    return isBetween(a, b, p);
}
		
enum class Location 
{
    Exterior,
    Interior,
    Boundary
};

template<typename Ring, typename Point>
auto locatePointInRing(const Ring &ring,
                       const Point &pt) -> Location
{
    bool inside = false;
    const std::size_t n = ring.size();

    for (std::size_t i = 0, j = n - 1; i < n; j = i++) {

        const auto& pi = ring[i];
        const auto& pj = ring[j];

        // 1 - Boundary check
        if (orientation(pj, pi, pt) == WindingOrder::Colinear &&
            isBetween(pj, pt, pi)) {
            return Location::Boundary;
        }

        // 2️ - Ray casting (horizontal ray to +∞)
        const bool intersect = ((pi.y() > pt.y()) != (pj.y() > pt.y())) &&
                               (pt.x() < (pj.x() - pi.x()) * (pt.y() - pi.y()) /
                               (pj.y() - pi.y()) + pi.x());

        if (intersect)
            inside = !inside;
    }

    return inside ? Location::Interior : Location::Exterior;
}
	
//template<typename LineString, typename Point>
//auto locatePointOnLineString(const LineString &line, 
//                             const Point &pt) -> Location
//{
//    const std::size_t n = line.size();
//    if (n < 2) {
//        return Location::Exterior;
//    }
//
//    for (std::size_t i = 0; i + 1 < n; ++i) {
//        const auto &p0 = line[i];
//        const auto &p1 = line[i + 1];
//
//        if (pointOnSegment(p0, p1, pt)) {
//            return Location::Boundary;
//        }
//    }
//
//    return Location::Exterior;
//}

template<typename Polygon, typename Point>
auto locatePointInPolygon(const Polygon &polygon, 
                          const Point &pt) -> Location
{
    // 0 - Fast envelope rejection
    if (!contains(envelope(polygon), pt)) {
        return Location::Exterior;
    }

    // 1 - Exterior ring
    const Location location = locatePointInRing(polygon.outer(), pt);

    if (location == Location::Exterior) {
        return Location::Exterior;
    }

    if (location == Location::Boundary) {
        return Location::Boundary;
    }

    // 2 - Holes
    for (const auto &hole : polygon.inners()) {

        const Location hole_location = locatePointInRing(hole, pt);

        if (hole_location == Location::Boundary) {
            return Location::Boundary;
        }

        if (hole_location == Location::Interior) {
            return Location::Exterior;
        }
    }

    // 3 - Inside polygon, not in holes
    return Location::Interior;
}

} // End namespace tl

