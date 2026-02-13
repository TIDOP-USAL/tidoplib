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

// Point-Point
template<typename Point_t>
[[nodiscard]]
constexpr auto within_impl(const Point_t& p1, const Point_t& p2, point_tag, point_tag) -> bool
{
    return equals(p1, p2);
}

} // namespace detail


template<GeometryConcept G1, GeometryConcept G2>
[[nodiscard]]
constexpr auto within(const G1 &geom1, const G2 &geom2) -> bool
{
    return detail::within_impl(geom1, geom2, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

} // namespace tl