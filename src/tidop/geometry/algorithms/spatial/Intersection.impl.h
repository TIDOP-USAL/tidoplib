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

template<Point2DConcept P1, Point2DConcept P2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto intersection_impl(const P1 &p1, 
                       const P2 &p2, 
                       const Policy &policy,
                       point_tag, 
                       point_tag) -> std::optional<common_point_without_measure_t<P1, P2>>
{
    using Point_t = common_point_without_measure_t<P1, P2>;

    const auto pt1 = policy.toKernelPoint<Dimension::dim2>(p1);
    const auto pt2 = policy.toKernelPoint<Dimension::dim2>(p2);

    if (TopologyKernel::equals(pt1, pt2)) {
        return static_cast<Point_t>(p1);
    }

    return std::nullopt;
}

/* BoundingBox - BoundingBox */

template<typename BBox_t, PrecisionPolicyConcept Policy>
auto intersection_impl(const BBox_t &b1, 
                       const BBox_t &b2, 
                       const Policy &policy, 
                       bbox_tag, bbox_tag) -> BBox_t
{
    using Point_t = typename geometry_traits<BBox_t>::point_type;
    constexpr size_t Dim = dimension_value(geometry_traits<BBox_t>::dimension);

    Point_t new_min, new_max;

    for (size_t i = 0; i < Dim; ++i) {
        new_min[i] = std::max(b1.min()[i], b2.min()[i]);
        new_max[i] = std::min(b1.max()[i], b2.max()[i]);

        // Si en alguna dimensión no solapan, el BBox resultante es inválido
        if (new_min[i] > new_max[i]) {
            return BBox_t(); // Retorna BBox vacío (min > max)
        }
    }

    return BBox_t(new_min, new_max);
}

//template<typename G1, typename G2, PrecisionPolicyConcept Policy>
//auto intersection_impl(const G1 &, const G2 &, const Policy &, ...)
//{
//    static_assert(always_false<G1>::value, "intersection not implemented for these geometry types");
//}


} // namespace detail



template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto intersection(const G1 &geom1,
                  const G2 &geom2,
                  const Policy &policy)
{
    using P1 = typename geometry_traits<G1>::point_type;
    using P2 = typename geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;

    return detail::intersection_impl(geom1, geom2, policy, tag1{}, tag2{});
}

template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
auto intersection(const G1 &geom1,
                  const G2 &geom2)
{
    using P = typename geometry_traits<G1>::point_type;
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return intersection(geom1, geom2, policy);
}


} // namespace tl