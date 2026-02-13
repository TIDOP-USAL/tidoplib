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
constexpr auto crosses_impl(const Point_t& p1, const Point_t& p2, point_tag, point_tag) -> bool
{
    return false; // Puntos no pueden cruzarse según OGC
}

} // namespace detail

template<GeometryConcept G1, GeometryConcept G2>
[[nodiscard]]
constexpr auto crosses(const G1 &geom1, const G2 &geom2) -> bool
{
	//static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    //static_assert(is_geometry_v<G2>, "Second argument must be a geometry");
	
    return detail::crosses_impl(geom1, geom2, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

} // namespace tl