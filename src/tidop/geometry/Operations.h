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
    Colinear,
    Clockwise,
    CounterClockwise
};

template<typename Point_t>
[[nodiscard]]
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

template<typename Point_t>
[[nodiscard]]
auto pointOnSegment(const Point_t &a,
                    const Point_t &b,
                    const Point_t &p) -> bool
{
    // Colinealidad
    if (orientation(a, b, p) != WindingOrder::Colinear) {
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

template<typename Ring, typename Point>
[[nodiscard]]
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
	
template<typename Segment_t, typename Point_t>
[[nodiscard]]
auto locatePointOnSegment(const Segment_t &segment,
                          const Point_t &point) -> Location
{
    const auto &a = segment.pt1();
    const auto &b = segment.pt2();

    if (point == a || point == b) {
        return Location::Boundary;
    }

    if (orientation(a, b, point) == WindingOrder::Colinear &&
        isBetween(a, point, b)) {
        return Location::Interior;
    }

    return Location::Exterior;
}

template<typename LineString_t, typename Point_t>
[[nodiscard]]
auto locatePointOnLineString(const LineString_t &ls,
                             const Point_t &point,
                             double tol = 0.) -> Location
{
    const std::size_t n = ls.size();
    if (n < 2)
        return Location::Exterior;

    const bool isClosed = equalsExact(ls.front(), ls.back(), tol);

    // Boundary check (solo extremos globales)
    if (!isClosed &&
        (equalsExact(point, ls.front(), tol) ||
         equalsExact(point, ls.back(), tol))) {
        return Location::Boundary;
    }

    // Interior check
    for (std::size_t i = 0; i + 1 < n; ++i) {
        if (pointOnSegment(ls[i], ls[i + 1], point/*, tol*/))
            return Location::Interior;
    }

    return Location::Exterior;
}

template<typename Polygon, typename Point>
[[nodiscard]]
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

template<typename Point_t, typename Polygon_t>
[[nodiscard]]
auto pointInAnyHole(const Point_t &point,
                    const Polygon_t &polygon) -> bool
{
    for (const auto &hole : polygon.inners()) {
        auto loc = locatePointInRing(hole, point);
        if (loc == Location::Interior || loc == Location::Boundary) {
            return true; // punto dentro o en borde de un hueco
        }
    }
    return false;
}



template<typename Point_t>
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
                                       const Segment<Point_t> &s2) -> SegmentIntersectionData<Point_t>
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

enum class IntersectionType
{
    None,
    Proper,      // interior-interior (X shape)
    Endpoint,    // boundary-boundary
    Overlapping  // colinear overlap
};

template<typename Segment_t>
constexpr auto intersectionType(const Segment_t &s1,
                                const Segment_t &s2) -> IntersectionType
{
    using point_type = typename geometry_traits<Segment_t>::point_type;
    static_assert(point_traits<point_type>::spatial_dims == 2, "Only 2D supported");
    using T = typename point_traits<point_type>::value_type;

    if (!intersects(envelope(s1), envelope(s2)))
        return IntersectionType::None;

    auto data = computeIntersectionData(s1, s2);

    if (data.o1 != data.o2 && data.o3 != data.o4 &&
        data.o1 != WindingOrder::Colinear &&
        data.o2 != WindingOrder::Colinear &&
        data.o3 != WindingOrder::Colinear &&
        data.o4 != WindingOrder::Colinear) {
        return IntersectionType::Proper;
    }

    const auto &p1 = s1.pt1();
    const auto &p2 = s1.pt2();
    const auto &q1 = s2.pt1();
    const auto &q2 = s2.pt2();

    if (data.o1 == WindingOrder::Colinear &&
        data.o2 == WindingOrder::Colinear &&
        data.o3 == WindingOrder::Colinear &&
        data.o4 == WindingOrder::Colinear) {
        //int count = (isBetween(p1, q1, p2) ? 1 : 0) +
        //            (isBetween(p1, q2, p2) ? 1 : 0) +
        //            (isBetween(q1, p1, q2) ? 1 : 0) +
        //            (isBetween(q1, p2, q2) ? 1 : 0);

        //if (count >= 2)
        //    return IntersectionType::Overlapping;
        T min1;
        T max1;
        T min2;
        T max2;

        if (std::abs(p1.x() - p2.x()) >= std::abs(p1.y() - p2.y())) {
            min1 = std::min(p1.x(), p2.x());
            max1 = std::max(p1.x(), p2.x());
            min2 = std::min(q1.x(), q2.x());
            max2 = std::max(q1.x(), q2.x());
        } else {
            min1 = std::min(p1.y(), p2.y());
            max1 = std::max(p1.y(), p2.y());
            min2 = std::min(q1.y(), q2.y());
            max2 = std::max(q1.y(), q2.y());
        }

        if (std::max(min1, min2) < std::min(max1, max2))
            return IntersectionType::Overlapping;

    }

    if (isEndpointIntersection(s1, s2))
        return IntersectionType::Endpoint;

    return IntersectionType::None;
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

