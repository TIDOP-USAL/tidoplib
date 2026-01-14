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

#include <cmath>
#include <algorithm>

namespace tl
{
	
namespace detail 
{

template<typename Segment_t>
auto angle_impl(const Segment_t& s, segment_tag) -> double 
{
    static_assert(geometry_traits<Segment_t>::dimension == Dimension::dim2, "angle(segment) for OX is only defined in 2D");
    
    auto v = s.vector();
    return std::atan2(static_cast<double>(v.y()), static_cast<double>(v.x()));
}

template<typename Segment_t>
auto angle_between_impl(const Segment_t& s1, const Segment_t& s2, segment_tag, segment_tag) -> double 
{
    auto v1 = s1.vector();
    auto v2 = s2.vector();
    
    double dot = v1.dot(v2);
    double mag = v1.module() * v2.module();
    
    if (mag < 1e-9) return 0.0;
    
    // clamp para evitar errores de precisión fuera de [-1, 1]
    double cos_theta = std::clamp(dot / mag, -1.0, 1.0);
    return std::acos(cos_theta);
}


} // namespace detail

template<typename G1, typename G2>
auto angle(const G1 &g1, const G2 &g2) -> double
{
    return detail::angle_between_impl(g1, g2, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

template<typename G>
auto angleOX(const G &g) -> double
{
    return detail::angle_impl(g, geometry_tag_t<G>{});
}

template<typename G>
auto angleOY(const G &g) -> double
{
    // En 2D, el ángulo respecto a Y es PI/2 - angleOX
    return consts::half_pi<double> -angleOX(g);
}

} // namespace tl