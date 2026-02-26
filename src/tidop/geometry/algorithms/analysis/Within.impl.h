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

// Point-Point
template<typename P1, typename P2>
[[nodiscard]]
constexpr auto within_impl(const P1 &p1,
                           const P2 &p2, 
                           double tolerance, 
                           point_tag,
                           point_tag) -> bool
{
    return equalsExact(p1, p2, tolerance);
}

// Segment - Segment
template<typename S1, typename S2>
[[nodiscard]]
constexpr auto within_impl(const S1 &seg1, 
                           const S2 &seg2,
                           double tolerance,
                           segment_tag, 
                           segment_tag) -> bool
{
    return contains(seg2, seg1, tolerance);
}

template<typename LS>
[[nodiscard]]
constexpr auto within_impl(const LS &ls1,
                           const LS &ls2,
                           double tolerance,
                           linestring_tag,
                           linestring_tag) -> bool
{
    return contains(ls2, ls1, tolerance);
}

template<typename Poly>
constexpr auto within_impl(const Poly &p1,
                           const Poly &p2,
                           double tolerance,
                           polygon_tag,
                           polygon_tag) -> bool
{
//    return contains(p2, p1, tolerance);
    // ningún punto del exterior
    for (const auto &pt : p1.outer())
        if (locatePointInPolygon(p2, pt) == Location::Exterior)
            return false;

    // comprobar un punto interior real de p1
    auto interior_point = representativePoint(p1);

    if (locatePointInPolygon(p2, interior_point) != Location::Interior)
        return false;

    return true;
}

// Point - MultiPoint
//template<typename P, typename MP>
//[[nodiscard]]
//constexpr auto within_impl(const P &point, 
//                           const MP &multipoint,
//                           double tolerance,
//                           point_tag, 
//                           multipoint_tag) -> bool
//{
//    for (const auto &p : multipoint)
//        if (equalsExact(point, p, tolerance))
//            return true;
//    return false;
//}

// Point - Segment
template<typename P, typename S>
constexpr bool within_impl(const P &p,
                           const S &seg,
                           double tolerance,
                           point_tag,
                           segment_tag)
{
    return contains(seg, p, tolerance);
}

// Point - LineString
template<typename P, typename LS>
constexpr bool within_impl(const P &p,
                           const LS &ls,
                           double tolerance,
                           point_tag,
                           linestring_tag)
{
    return locatePointOnLineString(ls, p, tolerance) == Location::Interior;
}

template<typename P, typename Poly>
constexpr bool within_impl(const P &p,
                           const Poly &poly,
                           double tolerance,
                           point_tag,
                           polygon_tag)
{
    return locatePointInPolygon(poly, p) == Location::Interior;
}

template<typename LS, typename Poly>
auto within_impl(const LS &ls,
                 const Poly &poly,
                 double tolerance,
                 linestring_tag,
                 polygon_tag) -> bool
{
    // ningún vértice puede estar en el exterior
    for (const auto &p : ls)
        if (locatePointInPolygon(poly, p) == Location::Exterior)
            return false;

    // comprobar que el interior del LS esté en el interior del polígono
    // basta comprobar un punto interior de cada segmento
    bool hasInteriorPointInside = false;

    for (size_t i = 0; i + 1 < ls.size(); ++i) {

        Segment seg(ls[i], ls[i + 1]);

        auto mid = seg.midPoint();
        auto loc = locatePointInPolygon(poly, mid);

        if (loc == Location::Exterior)
            return false;

        if (loc == Location::Interior)
            hasInteriorPointInside = true;
    }
    return hasInteriorPointInside;
}

template<typename S, typename LS>
constexpr bool within_impl(const S &seg,
                           const LS &ls,
                           double tolerance,
                           segment_tag,
                           linestring_tag)
{
    for (size_t i = 0; i + 1 < ls.size(); ++i) {
        Segment candidate(ls[i], ls[i + 1]);
        if (contains(candidate, seg, tolerance))
            return true;
    }
    return false;
}

template<typename S, typename Poly>
constexpr bool within_impl(const S &seg,
                           const Poly &poly,
                           double tolerance,
                           segment_tag,
                           polygon_tag)
{
    //if (locatePointInPolygon(poly, seg.start()) != Location::Interior)
    //    return false;

    //if (locatePointInPolygon(poly, seg.end()) != Location::Interior)
    //    return false;

    //auto boundary_poly = boundary(poly);

    //if (intersects(seg, boundary_poly))
    //    return false;

    //return true;
    // ningún punto puede estar fuera
    if (locatePointInPolygon(poly, seg.start()) == Location::Exterior)
        return false;

    if (locatePointInPolygon(poly, seg.end()) == Location::Exterior)
        return false;

    // el segmento no puede cruzar al exterior
    if (intersects(seg, boundary(poly))) // o equivalente
        return false;

    // el interior del segmento debe intersectar el interior del polígono
    // Basta con que algún punto interior esté dentro.
    auto mid = seg.midPoint();
    if (locatePointInPolygon(poly, mid) != Location::Interior)
        return false;

    return true;
}

template<typename LS, typename S>
constexpr bool within_impl(const LS &ls,
                           const S &seg,
                           double tolerance,
                           linestring_tag,
                           segment_tag)
{
    if (ls.size() != 2)
        return false;

    Segment candidate(ls[0], ls[1]);
    return contains(seg, candidate, tolerance);
}

template<typename GC, typename G2>
bool within_impl(const GC &collection, 
                 const G2 &geom, 
                 double tolerance,
                 collection_tag, 
                 geometry_tag_t<G2>) 
{
    for (const auto &item : collection) {
        if (!std::visit([&](auto &&arg) {
                return within(arg.get(), geom, tolerance);
            }, item)) 
            return false;
    }

    return true;
}

// Segunda geometría es colección
template<typename G1, typename GC>
bool within_impl(const G1 &geom,
                 const GC &collection,
                 double tolerance,
                 geometry_tag_t<G1>, 
                 collection_tag) 
{
    for (const auto &item : collection) {
        if(std::visit([&](auto &&arg) {
            return within(geom, arg.get(), tolerance);
            }, item)) return true;
    }

    return false;
}

template<typename G1, typename G2, typename Tag1, typename Tag2>
[[nodiscard]]
constexpr auto within_impl(const G1 &, const G2 &, double, Tag1, Tag2) -> bool
{
    return false;
}

} // namespace detail


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto within(const G1 &geom1, const G2 &geom2) -> bool
{
    using Scalar = typename point_traits<geometry_traits<G1>::point_type>::value_type;
    return within(geom1, geom2, default_tolerance<Scalar>::value);
}

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto within(const G1 &geom1, const G2 &geom2, double tolerance) -> bool
{
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if (geom1.isEmpty() || geom2.isEmpty()) return false;

    auto dispatch = [&] {
        using tag1 = geometry_tag_t<G1>;
        using tag2 = geometry_tag_t<G2>;
        return detail::within_impl(geom1, geom2, tolerance, tag1{}, tag2{});
    };

    if constexpr (GeometryCollectionConcept<G1> || GeometryCollectionConcept<G2>) {
        return dispatch();
    } else if constexpr (MultiGeometryConcept<G1>) {
        for (const auto &a : geom1) {
            if (!within(a, geom2, tolerance))
                return false;
        }
        return true;
    } else if constexpr (MultiGeometryConcept<G2>) {
        for (const auto &b : geom2) {
            if (within(geom1, b, tolerance)) return true;
        }
        return false;
    } else {
        return dispatch();
    }

    //return detail::within_impl(geom1, geom2, tolerance, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto within(const G1 &geom1,
                      const G2 &geom2,
                      const TolerancePolicy &policy) -> bool
{
    return within(geom1, geom2, policy.xyTolerance());
}

} // namespace tl