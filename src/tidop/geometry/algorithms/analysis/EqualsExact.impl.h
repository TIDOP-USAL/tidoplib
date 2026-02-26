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
[[nodiscard]]
constexpr auto equals_exact_impl(const P1 &point1, 
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


template<typename S1, typename S2>
[[nodiscard]]
constexpr auto equals_exact_impl(const S1 &segment1, 
                                 const S2 &segment2,
                                 double tolerance,
                                 segment_tag, 
                                 segment_tag) -> bool
{
    return equalsExact(segment1.pt1(), segment2.pt1(), tolerance) &&
           equalsExact(segment1.pt2(), segment2.pt2(), tolerance);
}


template<typename LS1, typename LS2>
[[nodiscard]]
constexpr auto equals_exact_impl(const LS1 &ls1,
                                 const LS2 &ls2,
                                 double tolerance,
                                 linestring_tag,
                                 linestring_tag) -> bool
{
    if (ls1.size() != ls2.size())
        return false;

    for (size_t i = 0; i < ls1.size(); ++i) {
        if (!equalsExact(ls1[i], ls2[i], tolerance))
            return false;
    }

    return true;
}


template<typename Poly1, typename Poly2>
[[nodiscard]]
constexpr auto equals_exact_impl(const Poly1 &poly1,
                                 const Poly2 &poly2,
                                 double tolerance,
                                 polygon_tag,
                                 polygon_tag) -> bool
{
    if (poly1.outer().size() != poly2.outer().size())
        return false;

    for (size_t i = 0; i < poly1.outer().size(); ++i) {
        if (!equalsExact(poly1.outer()[i], poly2.outer()[i], tolerance))
            return false;
    }

    if (poly1.inners().size() != poly2.inners().size())
        return false;

    for (size_t h = 0; h < poly1.inners().size(); ++h) {
        const auto &hole1 = poly1.inners()[h];
        const auto &hole2 = poly2.inners()[h];

        if (hole1.size() != hole2.size())
            return false;

        for (size_t i = 0; i < hole1.size(); ++i) {
            if (!equalsExact(hole1[i], hole2[i], tolerance))
                return false;
        }
    }

    return true;
}


template<typename GC1, typename GC2>
[[nodiscard]]
auto equals_exact_impl(const GC1 &col1,
                       const GC2 &col2,
                       double tolerance,
                       collection_tag,
                       collection_tag) -> bool
{
    if (col1.size() != col2.size()) return false;

    for (size_t i = 0; i < col1.size(); ++i) {
        bool elem_equal = std::visit([&](const auto &arg1, const auto &arg2) {
            
            const auto &geometry1 = arg1.get();
            const auto &geometry2 = arg2.get();

            using G1 = std::decay_t<decltype(geometry1)>;
            using G2 = std::decay_t<decltype(geometry2)>;

            if constexpr (GeometryConcept<G1> && GeometryConcept<G2>) {
                return equalsExact(geometry1, geometry2, tolerance);
            } else {
                return false; // tipos no geométricos (no debería ocurrir)
            }
            }, col1[i], col2[i]);

        if (!elem_equal) return false;
    }

    return true;
}


template<typename G1, typename G2, typename Tag1, typename Tag2>
[[nodiscard]]
constexpr auto equals_exact_impl(const G1 &, const G2 &, double, Tag1, Tag2) -> bool
{
    return false;
}

} // namespace detail


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1, const G2 &geom2) -> bool
{	
    using Scalar = typename point_traits<geometry_traits<G1>::point_type>::value_type;

    return equalsExact(geom1, geom2, default_tolerance<Scalar>::value);
}


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1, 
                           const G2 &geom2,
                           double tolerance) -> bool
{
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if constexpr (!std::is_same_v<geometry_tag_t<G1>, geometry_tag_t<G2>>) {
        return false;
    } else if constexpr (GeometryCollectionConcept<G1> || !MultiGeometryConcept<G1>) {
        return detail::equals_exact_impl(geom1, geom2, tolerance, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
    } else if constexpr (MultiGeometryConcept<G1>) {
        if (geom1.size() != geom2.size()) return false;
        for (size_t i = 0; i < geom1.size(); ++i)
            if (!equalsExact(geom1[i], geom2[i], tolerance)) return false;
        return true;
    } else {
        return false; // casos no contemplados
    }
}


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1, 
                           const G2 &geom2, 
                           const TolerancePolicy &policy) -> bool
{
    return equalsExact(geom1, geom2, policy.xyTolerance());
}


} // namespace tl