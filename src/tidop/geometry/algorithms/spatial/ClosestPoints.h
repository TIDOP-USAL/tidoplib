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

#include "tidop/geometry/base/Traits.h"

#include <variant>

namespace tl
{
	
/*! \addtogroup Algorithms
 *  \{
 */

/*!
 * \brief Finds the closest points between two geometries.
 * 
 * Returns a pair of points: the first from geometry A, the second from geometry B,
 * that minimize the distance between the two geometries.
 * 
 * \tparam G1 Type of first geometry.
 * \tparam G2 Type of second geometry.
 * 
 * \param g1 First geometry.
 * \param g2 Second geometry.
 * 
 * \return GeometryCollection containing the closest points (point from g1, point from g2).
 * 
 * \note If the geometries intersect, returns two equal points (or very close).
 * \note For empty geometries returns empty GeometryCollection.
 * 
 * \see distance(), intersects()
 */
template<typename G1, typename G2>
auto closestPoints(const G1 &g1, const G2 &g2)  -> GeometryCollection<typename geometry_traits<G1>::point_type>;
				 
				 
/*! \} */ 

} // End namespace tl

#include "ClosestPoints.impl.h"
