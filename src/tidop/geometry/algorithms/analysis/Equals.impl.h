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

// Point-Point equals with tolerance
template<typename P1, typename P2>
[[nodiscard]]
constexpr auto equals_impl(const P1 &point1, 
                           const P2 &point2,
                           double tolerance,
                           point_tag,
                           point_tag) -> bool
{
    using Scalar = typename point_traits<P1>::value_type;

    constexpr size_t spatial_dims = point_traits<P1>::spatial_dims;

    for (size_t i = 0; i < spatial_dims; ++i) {
        if constexpr (std::is_integral_v<Scalar>) {
            if (point1[i] != point2[i]) return false;
        } else {
            if (std::abs(static_cast<double>(point1[i]) - static_cast<double>(point2[i])) > tolerance) {
                return false;
            }
        }
    }

    return true;
}

// Segment-Segment equals
template<typename S1, typename S2>
[[nodiscard]]
constexpr auto equals_impl(const S1 &segment1, 
                           const S2 &segment2,
                           double tolerance,
                           segment_tag, 
                           segment_tag) -> bool
{
    return (equals(segment1.pt1(), segment2.pt1(), tolerance) && equals(segment1.pt2(), segment2.pt2(), tolerance)) ||
           (equals(segment1.pt1(), segment2.pt2(), tolerance) && equals(segment1.pt2(), segment2.pt1(), tolerance));
}

template<typename LS1, typename LS2>
[[nodiscard]]
constexpr auto equals_impl(const LS1 &line1,
                           const LS2 &line2,
                           double tolerance,
                           linestring_tag,
                           linestring_tag) -> bool
{
    if (line1.size() != line2.size()) return false;

    // Comparación directa
    bool same_order = true;
    for (size_t i = 0; i < line1.size(); ++i) {
        if (!equals(line1[i], line2[i], tolerance)) {
            same_order = false;
            break;
        }
    }
    if (same_order) return true;
}

template<typename Ring1, typename Ring2>
[[nodiscard]]
bool rings_equal(const Ring1 &r1, const Ring2 &r2, double tolerance) {
    if (r1.size() != r2.size()) return false;
    size_t n = r1.size();
    if (n == 0) return true;

    // Buscar un desplazamiento donde coincida el primer punto de r1 con algún punto de r2
    for (size_t start = 0; start < n; ++start) {
        if (!equals(r1[0], r2[start], tolerance)) continue;

        bool match = true;
        for (size_t i = 0; i < n; ++i) {
            if (!equals(r1[i], r2[(start + i) % n], tolerance)) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

template<typename Poly1, typename Poly2>
[[nodiscard]]
constexpr auto equals_impl(const Poly1 &poly1,
                           const Poly2 &poly2,
                           double tolerance,
                           polygon_tag,
                           polygon_tag) -> bool
{
    // Exterior (puede empezar en distinto punto)
    if (!rings_equal(poly1.outer(), poly2.outer(), tolerance))
        return false;

    // Interiores como conjunto
    if (poly1.inners().size() != poly2.inners().size())
        return false;

    // Para evitar O(n^2) desordenado, puedes copiar los interiores a vectores
    // y buscar correspondencias uno a uno (cada anillo de poly1 debe encontrar
    // un anillo igual en poly2). Asumimos que los anillos pueden ser idénticos.
    std::vector<bool> used(poly2.inners().size(), false);
    for (const auto &ring1 : poly1.inners()) {
        bool found = false;
        for (size_t j = 0; j < poly2.inners().size(); ++j) {
            if (!used[j] && rings_equal(ring1, poly2.inners()[j], tolerance)) {
                used[j] = true;
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

// Point - MultiPoint
template<typename P, typename MP>
constexpr auto equals_impl(const P &point, 
                           const MP &multipoint,
                           double tolerance, 
                           point_tag,
                           multipoint_tag) -> bool
{
    if (multipoint.empty()) return false;

    for (const auto &p : multipoint)
        if (!equals(p, point, tolerance))
            return false;

    return true;
}

// MultiPoint - Point
template<typename MP, typename P>
constexpr auto equals_impl(const MP &multipoint,
                           const P &point,
                           double tolerance,
                           multipoint_tag,
                           point_tag) -> bool
{
    return equals(point, multipoint, tolerance);
}

// MultiPoint - MultiPoint
template<typename MP1, typename MP2>
constexpr auto equals_impl(const MP1 &mp1,
                           const MP2 &mp2,
                           double tolerance,
                           multipoint_tag,
                           multipoint_tag) -> bool
{
    if (mp1.size() != mp2.size()) return false;
    for (size_t i = 0; i < mp1.size(); ++i)
        if (!equals(mp1[i], mp2[i], tolerance)) return false;
    return true;
}

template<typename GC1, typename GC2>
[[nodiscard]]
auto equals_impl(const GC1 &col1,
                 const GC2 &col2,
                 double tolerance,
                 collection_tag,
                 collection_tag) -> bool
{
    if (col1.size() != col2.size()) return false;
    for (size_t i = 0; i < col1.size(); ++i) {
        bool elem_equal = std::visit([&](const auto &arg1) {

            const auto &geometry1 = arg1.get();

            using G1 = std::decay_t<decltype(geometry1)>;
            return std::visit([&](const auto &arg2) {

                const auto &geometry2 = arg2.get();

                using G2 = std::decay_t<decltype(geometry2)>;
                if constexpr (GeometryConcept<G1> && GeometryConcept<G2>) {
                    return equals(geometry1, geometry2, tolerance);
                } else {
                    return false; // tipos no geométricos (no debería ocurrir)
                }
                }, col2[i]);
            }, col1[i]);
        if (!elem_equal) return false;
    }
    return true;
}

template<typename G1, typename G2, typename Tag1, typename Tag2>
[[nodiscard]]
constexpr auto equals_impl(const G1 &, const G2 &, double, Tag1, Tag2) -> bool
{
    return false;
}

} // namespace detail


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, const G2 &geom2) -> bool
{	
    using Scalar = typename point_traits<geometry_traits<G1>::point_type>::value_type;

    return equals(geom1, geom2, default_tolerance<Scalar>::value);
}


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, 
                      const G2 &geom2,
                      double tolerance) -> bool
{
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    return detail::equals_impl(geom1, geom2, tolerance, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, 
                      const G2 &geom2, 
                      const TolerancePolicy &policy) -> bool
{
    return equals(geom1, geom2, policy.xyTolerance());
}

// Puede hacerse así pero hay que ver el rendimiento
//template<GeometryConcept G1, GeometryConcept G2>
//constexpr auto equals(const G1 &a,
//    const G2 &b,
//    double tolerance) -> bool
//{
//    return within(a, b, tolerance) &&
//           within(b, a, tolerance);
//}

} // namespace tl