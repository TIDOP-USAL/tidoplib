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

// Point-Point: igualdad de coordenadas
template<typename P1, typename P2>
[[nodiscard]]
constexpr auto equals_impl(const P1 &p1,
                           const P2 &p2,
                           point_tag,
                           point_tag) -> bool
{
    constexpr size_t spatial_dims = point_traits<P1>::spatial_dims;

    for (size_t i = 0; i < spatial_dims; ++i) {
          if (p1[i] != p2[i]) return false;
    }

    return true;
}

template<typename P1, typename P2>
[[nodiscard]]
constexpr auto equals_impl(const P1 &p1, 
                           const P2 &p2,
                           double tolerance,
                           point_tag,
                           point_tag) -> bool
{
    static_assert(std::is_same_v<typename point_traits<P1>::value_type,
                                 typename point_traits<P2>::value_type>);

    constexpr size_t spatial_dims = point_traits<P1>::spatial_dims;

    for (size_t i = 0; i < spatial_dims; ++i) {
        auto v = std::abs(p1[i] - p2[i]);
        if (std::abs(static_cast<double>(p1[i]) - static_cast<double>(p2[i])) > tolerance) {
            return false;
        }
    }

    return true;
}


} // namespace detail

template<typename T>
struct default_tolerance 
{
    static constexpr double value = std::is_same_v<T, float> ? 1e-6 :
                                    std::is_same_v<T, double> ? 1e-12 :
                                    std::is_same_v<T, long double> ? 1e-12l : 0.0;
};

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, const G2 &geom2) -> bool
{	
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;

    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P1>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if constexpr (std::is_integral_v<Scalar1>) {
        return detail::equals_impl(geom1, geom2, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
    } else {
        return detail::equals_impl(geom1, geom2, default_tolerance<Scalar1>::value, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
    }
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

    static_assert(std::is_same_v<typename point_traits<P1>::value_type,
                                 typename point_traits<P2>::value_type>,
        "Points must have same coordinate type");

    return detail::equals_impl(geom1, geom2, tolerance, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}


//template<GeometryConcept G1, GeometryConcept G2>
//    requires SameSpatialDimension<G1, G2>
//[[nodiscard]]
//constexpr auto equals(const G1 &geom1, 
//                      const G2 &geom2, 
//                      const TolerancePolicy &policy) -> bool
//{
//    return equals(geom1, geom2, policy.xyTolerance());
//}


} // namespace tl