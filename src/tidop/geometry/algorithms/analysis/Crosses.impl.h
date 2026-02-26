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
constexpr auto crosses_impl(const Point_t& p1, const Point_t& p2, point_tag, point_tag) -> bool
{
    return false; // Puntos no pueden cruzarse según OGC
}

template<typename Point_t>
constexpr auto crosses_impl(const Point_t &, const Segment<Point_t> &, point_tag, segment_tag) -> bool
{
    return false;
}

template<typename Segment_t>
constexpr auto crosses_impl(const Segment_t &s1,
                            const Segment_t &s2,
                            segment_tag,
                            segment_tag) -> bool
{
    //if (!intersects(envelope(s1), envelope(s2))) return false;

    //auto data = computeIntersectionData(s1, s2);

    //if (!(data.o1 != data.o2 && data.o3 != data.o4))
    //    return false;

    //if (data.o1 == WindingOrder::Colinear || data.o2 == WindingOrder::Colinear ||
    //    data.o3 == WindingOrder::Colinear || data.o4 == WindingOrder::Colinear)
    //    return false;
    return intersectionType(s1, s2) == IntersectionType::Proper;
}


// Función espejo
template<typename G1, typename G2, typename Tag1, typename Tag2>
auto crosses_impl(const G1 &g1, const G2 &g2, Tag1 t1, Tag2 t2) -> double
{
    return crosses_impl(g2, g1, t2, t1);
}

} // namespace detail

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto crosses(const G1 &geom1, const G2 &geom2) -> bool
{
    using Point_t = typename geometry_traits<G1>::point_type;

    static_assert(point_traits<Point_t>::spatial_dims == 2,
        "crosses(Segment, Segment) only defined for 2D");

    return detail::crosses_impl(geom1, geom2, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

} // namespace tl