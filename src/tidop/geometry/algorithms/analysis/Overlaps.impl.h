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
constexpr auto overlaps_impl(const P1 &,
                             const P2 &,
                             const Policy &,
                             point_tag,
                             point_tag) -> bool
{
    return false; // Puntos no pueden superponerse según OGC
}


/* Segment – Segment */

template<Segment2DConcept S1, Segment2DConcept S2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto overlaps_impl(const S1 &segment1,
                             const S2 &segment2,
                             const Policy &policy,
                             segment_tag,
                             segment_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment1.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment1.pt2());
    const auto q1 = policy.toKernelPoint<Dimension::dim2>(segment2.pt1());
    const auto q2 = policy.toKernelPoint<Dimension::dim2>(segment2.pt2());

    if (TopologyKernel::equals(p1, p2) ||
        TopologyKernel::equals(q1, q2)) {
        return false;
    }

    auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);
    if (type != TopologyKernel::IntersectionType::Overlapping) return false;

    bool q_in_p = TopologyKernel::isBetween(p1, p2, q1) &&
                  TopologyKernel::isBetween(p1, p2, q2);
    bool p_in_q = TopologyKernel::isBetween(q1, q2, p1) &&
                  TopologyKernel::isBetween(q1, q2, p2);

    //bool q_in_p = pointOnSegment(p1, p2, q1) &&
    //              pointOnSegment(p1, p2, q2);

    //bool p_in_q = pointOnSegment(q1, q2, p1) &&
    //              pointOnSegment(q1, q2, p2);

    return !(q_in_p || p_in_q);

}

} // namespace detail



template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto overlaps(const G1 &geom1, 
                        const G2 &geom2) -> bool
{
    using Scalar = typename point_traits<geometry_traits<G1>::point_type>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return overlaps(geom1, geom2, policy);
}

template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto overlaps(const G1 &geom1,
                        const G2 &geom2,
                        const Policy &policy) -> bool
{
    using P1 = geometry_traits<G1>::point_type;
    using P2 = geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if (geom1.isEmpty() || geom2.isEmpty())
        return false;

    return detail::overlaps_impl(geom1, geom2, policy, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

} // namespace tl