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
#include <unordered_map>

namespace tl
{
// Según OGC Simple Features:
// 
// Geometría	Boundary
// Point	vacío
// MultiPoint	vacío
// LineString	sus puntos inicial y final si son distintos
// LinearRing	vacío
// Polygon	conjunto de anillos
// MultiLineString	unión de los boundaries
// GeometryCollection	unión de los boundaries
	
/*! \addtogroup Algorithms
 *  \{
 */

template<typename Geometry_t>
[[nodiscard]]
auto boundary(const Geometry_t &g);



/*! \} */ 

} // End namespace tl

#include "Boundary.impl.h"
