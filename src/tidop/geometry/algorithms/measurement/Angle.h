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

namespace tl
{
	
/*! \addtogroup Measurements
 *  \{
 */

/*!
 * \brief Calculate the angle relative to the X-axis (OX) in 2D.
 */
template<typename G>
auto angleOX(const G &g) -> double;

/*!
 * \brief Calculate the angle relative to the Y-axis (OY) in 2D.
 */
template<typename G>
auto angleOY(const G &g) -> double;

/*!
 * \brief Compute the angle between two geometries (vectors, segments, planes).
 */
template<typename G1, typename G2>
auto angle(const G1 &g1, const G2 &g2) -> double;

/*! \} */ 

} // End namespace tl

#include "Angle.impl.h"
