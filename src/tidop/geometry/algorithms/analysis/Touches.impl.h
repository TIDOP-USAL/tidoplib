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


template<typename P1, typename P2>
auto touches_impl(const P1 &,
                  const P2 &,
                  point_tag,
                  point_tag) -> bool
{
    return false;
}


template<typename Point_t, typename Segment_t>
auto touches_impl(const Point_t &point,
                  const Segment_t &segment,
                  point_tag,
                  segment_tag) -> bool
{
    // Un punto toca un segmento si coincide con alguno de sus extremos (frontera)
    // Si el punto está en el interior del segmento (pero no es extremo) retorna false.
    return equals(point, segment.pt1()) || equals(point, segment.pt2());
}

// Point – LineString
template<typename Point_t, typename LineString_t>
auto touches_impl(const Point_t &point,
                  const LineString_t &lineString,
                  point_tag,
                  linestring_tag) -> bool
{
	return locatePointOnLineString(lineString, point) == Location::Boundary;
}

// Point - Polygon
template<typename Point_t, typename Polygon_t>
auto touches_impl(const Point_t &point,
                  const Polygon_t &polygon,
                  point_tag, 
                  polygon_tag) -> bool 
{
    return locatePointInPolygon(polygon, point) == Location::Boundary;
}

// LineString – LineString
template<typename Point_t>
auto touches_impl(const LineString<Point_t> &a,
                  const LineString<Point_t> &b,
                  linestring_tag,
                  linestring_tag) -> bool
{
    bool has_endpoint_contact = false;

    for (size_t i = 0; i + 1 < a.size(); ++i) {
        Segment<Point_t> sa(a[i], a[i + 1]);

        for (size_t j = 0; j + 1 < b.size(); ++j) {
            Segment<Point_t> sb(b[j], b[j + 1]);

            auto type = intersectionType(sa, sb);

            if (type == IntersectionType::Proper ||
                type == IntersectionType::Overlapping)
                return false;

            if (type == IntersectionType::Endpoint)
                has_endpoint_contact = true;
        }
    }

    return has_endpoint_contact;
}

// Polygon - Polygon
template<typename Polygon_t>
auto touches_impl(const Polygon_t& p1, 
                  const Polygon_t& p2,
                  polygon_tag, 
                  polygon_tag) -> bool 
{
    using Point_t = typename geometry_traits<Polygon_t>::point_type;

    // Lambda para recorrer todos los segmentos de todos los anillos
    auto for_each_segment = [](const Polygon_t &poly, auto &&callback) {
        const auto &outer = poly.outer();
        for (size_t i = 0; i + 1 < outer.size(); ++i)
            callback(Segment<Point_t>{outer[i], outer[i + 1]});
        for (const auto &hole : poly.inners())
            for (size_t i = 0; i + 1 < hole.size(); ++i)
                callback(Segment<Point_t>{hole[i], hole[i + 1]});
        };

    bool has_endpoint_contact = false;

    for_each_segment(p1, [&](const Segment<Point_t> &s1) {
        for_each_segment(p2, [&](const Segment<Point_t> &s2) {
            auto type = intersectionType(s1, s2);

            if (type == IntersectionType::Proper || type == IntersectionType::Overlapping)
                return false; // interior o solapamiento → no toca

            if (type == IntersectionType::Endpoint)
                has_endpoint_contact = true;
            });
        });

    return has_endpoint_contact;
}

// Segment – Segment
template<typename Point_t>
bool touches_impl(const Segment<Point_t>& s1,
                  const Segment<Point_t>& s2,
                  segment_tag,
                  segment_tag)
{
    return intersectionType(s1, s2) == IntersectionType::Endpoint;
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


} // namespace detail


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
auto touches(const G1 &g1, const G2 &g2) -> bool
{
    //static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    //static_assert(is_geometry_v<G2>, "Second argument must be a geometry");

    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");
    //static_assert(std::is_same_v<typename point_traits<P1>::value_type,
    //                             typename point_traits<P2>::value_type>,
    //    "Points must have same coordinate type");

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



} // namespace tl