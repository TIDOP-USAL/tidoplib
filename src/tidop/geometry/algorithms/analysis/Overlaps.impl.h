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

template<typename Point_t>
[[nodiscard]]
constexpr auto overlaps_impl(const Point_t& p1, const Point_t& p2, point_tag, point_tag) -> bool
{
    return false; // Puntos no pueden superponerse según OGC
}

template<typename Point_t>
[[nodiscard]]
constexpr auto overlaps_impl(const Segment<Point_t> &segment1,
                             const Segment<Point_t> &segment2,
                             segment_tag,
                             segment_tag) -> bool
{
    //const auto &p1 = seg1.pt1();
    //const auto &p2 = seg1.pt2();
    //const auto &q1 = seg2.pt1();
    //const auto &q2 = seg2.pt2();

    //if (orientation(p1, p2, q1) != WindingOrder::Colinear ||
    //    orientation(p1, p2, q2) != WindingOrder::Colinear)
    //    return false;

    //if (!intersects(seg1, seg2))
    //    return false;

    //if (equals(seg1, seg2))
    //    return false;

    //if (contains(seg1, seg2) || contains(seg2, seg1))
    //    return false;

    //return true;
    return intersectionType(segment1, segment2) == IntersectionType::Overlapping;
}

} // namespace detail

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto overlaps(const G1 &geom1, const G2 &geom2) -> bool
{
	//static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    //static_assert(is_geometry_v<G2>, "Second argument must be a geometry");
	
    return detail::overlaps_impl(geom1, geom2, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

} // namespace tl