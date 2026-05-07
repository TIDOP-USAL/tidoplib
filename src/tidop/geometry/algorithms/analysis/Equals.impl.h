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

template<PointConcept P1, PointConcept P2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto equals_impl(const P1 &point1, 
                           const P2 &point2,
                           const Policy &policy,
                           point_tag,
                           point_tag) -> bool
{
    if (policy.snap(point1.x()) != policy.snap(point2.x()) ||
        policy.snap(point1.y()) != policy.snap(point2.y())) return false;

    if constexpr (point_traits<P1>::spatial_dims > 2) {
        if (policy.snap(point1.z()) != policy.snap(point2.z())) return false;
    }

    if constexpr (point_traits<P1>::spatial_dims > 3) {
        if (policy.snap(point1.w()) != policy.snap(point2.w())) return false;
    }

    return true;
}


/* Segment – Segment */

template<SegmentConcept S1, SegmentConcept S2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto equals_impl(const S1 &segment1, 
                           const S2 &segment2,
                           const Policy &policy,
                           segment_tag, 
                           segment_tag) -> bool
{
    return (equals(segment1.pt1(), segment2.pt1(), policy) && equals(segment1.pt2(), segment2.pt2(), policy)) ||
           (equals(segment1.pt1(), segment2.pt2(), policy) && equals(segment1.pt2(), segment2.pt1(), policy));
}


/* LineString – LineString */

template<typename Ring1, typename Ring2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto rings_equal(const Ring1 &r1, const Ring2 &r2, const Policy &policy) -> bool
{
    if (r1.size() != r2.size()) return false;

    //size_t n = r1.size();
    size_t n = r1.size() - 1;
    if (n == 0) return true;

    // Buscar un desplazamiento donde coincida el primer punto de r1 con algún punto de r2
    for (size_t start = 0; start < n; ++start) {
        if (!equals(r1[0], r2[start], policy)) continue;

        bool match = true;
        for (size_t i = 0; i < n; ++i) {
            if (!equals(r1[i], r2[(start + i) % n], policy)) {
                match = false;
                break;
            }
        }
        if (match) return true;

        match = true;
        for (size_t i = 0; i < n; ++i) {
            size_t idx = (n + start - i) % n;
            if (!equals(r1[i], r2[idx], policy)) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }

    return false;
}

template<typename LS1, typename LS2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto equals_impl(const LS1 &line1,
                           const LS2 &line2,
                           const Policy &policy,
                           linestring_tag,
                           linestring_tag) -> bool
{
    if (line1.size() != line2.size()) return false;

    const size_t n = line1.size();
    if (n == 0) return true;

    bool closed1 = equals(line1.front(), line1.back(), policy);
    bool closed2 = equals(line2.front(), line2.back(), policy);

    if (closed1 && closed2) {
        return rings_equal(line1, line2, policy);
    } else {
        auto check_order = [&](bool reverse) {
            for (size_t i = 0; i < n; ++i) {
                if (!equals(line1[i], line2[reverse ? (n - 1 - i) : i], policy))
                    return false;
            }
            return true;
        };

        return check_order(false) || check_order(true);
    }
}

template<typename Poly1, typename Poly2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto equals_impl(const Poly1 &poly1,
                           const Poly2 &poly2,
                           const Policy &policy,
                           polygon_tag,
                           polygon_tag) -> bool
{
    if (!rings_equal(poly1.outer(), poly2.outer(), policy))
        return false;

    if (poly1.inners().size() != poly2.inners().size())
        return false;

    std::vector<bool> used(poly2.inners().size(), false);
    for (const auto &ring1 : poly1.inners()) {
        bool found = false;
        for (size_t j = 0; j < poly2.inners().size(); ++j) {
            if (!used[j] && rings_equal(ring1, poly2.inners()[j], policy)) {
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
template<typename P, typename MP, PrecisionPolicyConcept Policy>
constexpr auto equals_impl(const P &point, 
                           const MP &multipoint,
                           const Policy &policy,
                           point_tag,
                           multipoint_tag) -> bool
{
    if (multipoint.empty()) return false;

    for (const auto &p : multipoint)
        if (!equals(p, point, policy))
            return false;

    return true;
}

// MultiPoint - Point
template<typename MP, typename P, PrecisionPolicyConcept Policy>
constexpr auto equals_impl(const MP &multipoint,
                           const P &point,
                           const Policy &policy,
                           multipoint_tag,
                           point_tag) -> bool
{
    return equals(point, multipoint, policy);
}

// MultiPoint - MultiPoint
template<typename MP1, typename MP2, PrecisionPolicyConcept Policy>
constexpr auto equals_impl(const MP1 &mp1,
                           const MP2 &mp2,
                           const Policy &policy,
                           multipoint_tag,
                           multipoint_tag) -> bool
{
    if (mp1.size() != mp2.size()) return false;
    for (size_t i = 0; i < mp1.size(); ++i)
        if (!equals(mp1[i], mp2[i], policy)) return false;
    return true;
}

template<typename GC1, typename GC2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto equals_impl(const GC1 &col1,
                 const GC2 &col2,
                 const Policy &policy,
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
                    return equals(geometry1, geometry2, policy);
                } else {
                    return false; // tipos no geométricos (no debería ocurrir)
                }
                }, col2[i]);
            }, col1[i]);
        if (!elem_equal) return false;
    }
    return true;
}

template<typename G1, typename G2, PrecisionPolicyConcept Policy, typename Tag1, typename Tag2>
[[nodiscard]]
constexpr auto equals_impl(const G1 &, const G2 &, const Policy &, double, Tag1, Tag2) -> bool
{
    return false;
}

} // namespace detail


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, const G2 &geom2) -> bool
{	
    using P = typename geometry_traits<G1>::point_type;
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return equals(geom1, geom2, policy);
}


template<GeometryConcept G1, GeometryConcept G2, PrecisionPolicyConcept Policy>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, 
                      const G2 &geom2, 
                      const Policy &policy) -> bool
{
    using P1 = typename geometry_traits<G1>::point_type;
    using P2 = typename geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    return detail::equals_impl(geom1, geom2, policy, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

} // namespace tl