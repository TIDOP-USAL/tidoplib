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

#include "tidop/geometry/base/TopologyKernel.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

namespace tl
{

// Esto sería mejor (Segment, Point)
// ahora es (s.p1(), p, s.p2()) que es confuso
template<typename Point_t>	
[[nodiscard]]
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
    Collinear,
    Clockwise,
    CounterClockwise
};

template<typename Point_t>
[[nodiscard]]
auto orientation(const Point_t &a,
                 const Point_t &b,
                 const Point_t &c,
                 double tolerance = 0.0) -> WindingOrder
{
    using calc_t = typename point_traits<Point_t>::calculation_type;
    const calc_t val = (static_cast<calc_t>(b.y()) - static_cast<calc_t>(a.y())) *
                       (static_cast<calc_t>(c.x()) - static_cast<calc_t>(b.x())) -
                       (static_cast<calc_t>(b.x()) - static_cast<calc_t>(a.x())) *
                       (static_cast<calc_t>(c.y()) - static_cast<calc_t>(b.y()));

    //constexpr calc_t eps = std::numeric_limits<calc_t>::epsilon() * static_cast<calc_t>(100);
    if (std::abs(val) <= tolerance) return WindingOrder::Collinear; // colineal
    return (val > static_cast<calc_t>(0)) ? WindingOrder::Clockwise : WindingOrder::CounterClockwise; // horario o antihorario
}

template<typename Point_t>
[[nodiscard]]
auto pointOnSegment(const Point_t &a,
                    const Point_t &b,
                    const Point_t &p) -> bool
{
    // Colinealidad
    if (orientation(a, b, p) != WindingOrder::Collinear) {
        return false;
    }

    // Dentro del bounding box del segmento
    return isBetween(a, p, b);
}
		
enum class Location 
{
    Exterior,
    Interior,
    Boundary
};

template<typename Ring, typename Point, typename Policy_t>
[[nodiscard]]
auto locatePointInRing(const Ring &ring,
                       const Point &point,
                       const Policy_t &policy) -> Location
{
    bool inside = false;
    const std::size_t n = ring.size();

    const auto p = policy.toKernelPoint<Dimension::dim2>(point);

    for (std::size_t i = 0, j = n - 1; i < n; j = i++) {

        const auto pi = policy.toKernelPoint<Dimension::dim2>(ring[i]);
        const auto pj = policy.toKernelPoint<Dimension::dim2>(ring[j]);

        // 1 - Boundary check
        if (TopologyKernel::orientation(pj, pi, p) == TopologyKernel::WindingOrder::Collinear &&
            TopologyKernel::isBetween(pj, pi, p)) {
            return Location::Boundary;
        }

        // 2️ - Ray casting (horizontal ray to +∞)
        const bool intersect = ((pi.y() > p.y()) != (pj.y() > p.y())) &&
                               (p.x() < (pj.x() - pi.x()) * (p.y() - pi.y()) /
                               (pj.y() - pi.y()) + pi.x());

        if (intersect)
            inside = !inside;
    }

    return inside ? Location::Interior : Location::Exterior;
}
	
template<typename Segment_t, typename Point_t, typename Policy_t>
[[nodiscard]]
auto locatePointOnSegment(const Segment_t &segment,
                          const Point_t &point,
                          const Policy_t &policy) -> Location
{
    const auto p = policy.toKernelPoint<Dimension::dim2>(point);
    const auto a = policy.toKernelPoint<Dimension::dim2>(segment.pt1());
    const auto b = policy.toKernelPoint<Dimension::dim2>(segment.pt2());

    if (TopologyKernel::equals(p, a) || TopologyKernel::equals(p, b)) {
        return Location::Boundary;
    }

    if (TopologyKernel::orientation(a, b, p) == TopologyKernel::WindingOrder::Collinear &&
        TopologyKernel::isBetween(a, b, p)) {
        return Location::Interior;
    }

    return Location::Exterior;
}

template<typename LineString_t, typename Point_t, typename Policy_t>
[[nodiscard]]
auto locatePointOnLineString(const LineString_t &ls,
                             const Point_t &point,
                             const Policy_t &policy) -> Location
{
    const std::size_t n = ls.size();
    if (n < 2)
        return Location::Exterior;

     const auto p = policy.toKernelPoint<Dimension::dim2>(point);
     const auto p1 = policy.toKernelPoint<Dimension::dim2>(ls.front());
     const auto p2 = policy.toKernelPoint<Dimension::dim2>(ls.back());
     bool isClosed = TopologyKernel::equals(p1, p2);

    if (!isClosed &&
        (TopologyKernel::equals(p, p1) ||
         TopologyKernel::equals(p, p2))) {
        return Location::Boundary;
    }
    
    for (std::size_t i = 0; i + 1 < n; ++i) {
        const auto p_ini = policy.toKernelPoint<Dimension::dim2>(ls[i]);
        const auto p_end = policy.toKernelPoint<Dimension::dim2>(ls[i + 1]);
        if (TopologyKernel::pointOnSegmentInclusive(p_ini, p_end, p))
            return Location::Interior;
    }

    return Location::Exterior;
}

template<typename Polygon, typename Point, typename Policy_t>
[[nodiscard]]
auto locatePointInPolygon(const Polygon &polygon, 
                          const Point &pt,
                          const Policy_t &policy) -> Location
{
    // TODO: Añadir locatePointInBox
    //if (!contains(envelope(polygon), pt, policy)) {
    //    return Location::Exterior;
    //}

    const Location location = locatePointInRing(polygon.outer(), pt, policy);

    if (location == Location::Exterior) {
        return Location::Exterior;
    }

    if (location == Location::Boundary) {
        return Location::Boundary;
    }

    for (const auto &hole : polygon.inners()) {

        const Location hole_location = locatePointInRing(hole, pt, policy);

        if (hole_location == Location::Boundary) {
            return Location::Boundary;
        }

        if (hole_location == Location::Interior) {
            return Location::Exterior;
        }
    }

    return Location::Interior;
}

template<typename Point_t, typename Polygon_t, typename Policy_t>
[[nodiscard]]
auto pointInAnyHole(const Point_t &point,
                    const Polygon_t &polygon,
                    const Policy_t &policy) -> bool
{
    for (const auto &hole : polygon.inners()) {
        auto loc = locatePointInRing(hole, point, policy);
        if (loc == Location::Interior || loc == Location::Boundary) {
            return true; // punto dentro o en borde de un hueco
        }
    }
    return false;
}



struct SegmentIntersectionData
{
    WindingOrder o1;
    WindingOrder o2;
    WindingOrder o3;
    WindingOrder o4;
};

template<typename Point_t>
[[nodiscard]]
constexpr auto computeIntersectionData(const Segment<Point_t> &s1,
                                       const Segment<Point_t> &s2) -> SegmentIntersectionData
{
    const auto &p1 = s1.pt1();
    const auto &p2 = s1.pt2();
    const auto &q1 = s2.pt1();
    const auto &q2 = s2.pt2();

    return { orientation(p1, p2, q1),
             orientation(p1, p2, q2),
             orientation(q1, q2, p1),
             orientation(q1, q2, p2) };
}

//template<typename Segment_t>
//constexpr bool colinearOverlap(const Segment_t &s1,
//                               const Segment_t &s2)
//{
//    const auto &p1 = s1.pt1();
//    const auto &p2 = s1.pt2();
//    const auto &q1 = s2.pt1();
//    const auto &q2 = s2.pt2();
//
//    bool q1_on_s1 = pointOnSegment(p1, p2, q1);
//    bool q2_on_s1 = pointOnSegment(p1, p2, q2);
//    bool p1_on_s2 = pointOnSegment(q1, q2, p1);
//    bool p2_on_s2 = on_segment(q1, q2, p2);
//
//    int count =
//        (q1_on_s1 ? 1 : 0) +
//        (q2_on_s1 ? 1 : 0) +
//        (p1_on_s2 ? 1 : 0) +
//        (p2_on_s2 ? 1 : 0);
//
//    // Si al menos dos puntos están en el otro segmento
//    // entonces comparten longitud (no solo punto)
//    return count >= 2;
//}

template<typename Segment_t>
constexpr bool isEndpointIntersection(const Segment_t &s1,
                                      const Segment_t &s2)
{
    const auto &p1 = s1.pt1();
    const auto &p2 = s1.pt2();
    const auto &q1 = s2.pt1();
    const auto &q2 = s2.pt2();

    return equals(p1, q1) ||
           equals(p1, q2) ||
           equals(p2, q1) ||
           equals(p2, q2);
}

namespace detail
{

template<typename Polygon_t>
auto representative_point_impl(const Polygon_t &poly, polygon_tag)
{
    using Point_t = typename geometry_traits<Polygon_t>::point_type;
    auto bbox = envelope(poly);
    double mid_y = (bbox.min().y() + bbox.max().y()) / 2.0;

    std::vector<double> x_intersections;

    auto collect_intersections = [&](const auto &ring) {
        for (size_t i = 0, j = ring.size() - 1; i < ring.size(); j = i++) {
            const auto &p1 = ring[i];
            const auto &p2 = ring[j];
            if ((p1.y() <= mid_y && p2.y() > mid_y) || (p2.y() <= mid_y && p1.y() > mid_y)) {
                double x = p1.x() + (mid_y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
                x_intersections.push_back(x);
            }
        }
        };

    collect_intersections(poly.outer());
    for (const auto &hole : poly.inners()) collect_intersections(hole);

    std::sort(x_intersections.begin(), x_intersections.end());

    double best_x = 0;
    double max_width = -1.0;

    for (size_t i = 0; i + 1 < x_intersections.size(); i++) {
        double mid_x = (x_intersections[i] + x_intersections[i + 1]) / 2.0;
        Point_t candidate(mid_x, mid_y);

        if (intersects(poly, candidate)) {
            double width = x_intersections[i + 1] - x_intersections[i];
            if (width > max_width) {
                max_width = width;
                best_x = mid_x;
            }
        }
    }

    if (max_width < 0) return poly.outer()[0];

    return Point_t(best_x, mid_y);
}

// Para LineString: El punto medio del segmento central
template<typename Point_t>
auto representative_point_impl(const LineString<Point_t> &line, linestring_tag)
{
    if (line.isEmpty()) return Point_t{};
    return line[line.size() / 2];
}

// Para Puntos: Es el punto mismo
template<typename Point_t>
auto representative_point_impl(const Point_t &pt, point_tag)
{
    return pt;
}

template<typename Point_t>
auto representative_point_impl(const Segment<Point_t> &segment, segment_tag)
{
    if (segment.isEmpty()) return Point_t{};
    return segment.midPoint();
}

} // namespace detail

template<typename Geometry_t>
auto representativePoint(const Geometry_t &g)
{
    return detail::representative_point_impl(g, geometry_tag_t<Geometry_t>{});
}



} // End namespace tl

