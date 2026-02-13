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

#include <variant>
#include <type_traits>

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{
	
/*! \addtogroup Algorithms
 *  \{
 */

/*!
 * \brief Computes the minimum bounding box (envelope) of a geometry.
 * 
 * This function computes the axis-aligned bounding box that contains the given geometry.
 *
 * \tparam Geometry_t Type of the geometry. Must be a valid geometry type.
 * 
 * \param[in] g Input geometry.
 * 
 * \return BoundingBox containing the input geometry.
 * 
 * \throws std::invalid_argument if the geometry is invalid.
 * 
 * \note For empty geometries, returns a default-constructed (empty) BoundingBox.
 * 
 * ### Example
 * \code
 * Point2d p(1.0, 2.0);
 * auto bbox = envelope(p);  // bbox.min() == bbox.max() == p
 * 
 * LineString2d line{{0, 0}, {1, 1}, {2, 0}};
 * auto bbox2 = envelope(line);  // bbox2.min() == {0, 0}, bbox2.max() == {2, 1}
 * \endcode
 */
template<typename Geometry_t>
auto envelope(const Geometry_t &g);

/*!
 * \brief Computes the combined envelope of multiple geometries.
 *
 * This function computes the minimum bounding box that contains all input geometries.
 * It's equivalent to computing the envelope of each geometry and merging them.
 *
 * \tparam Geometry_t Type of the first geometry.
 * \tparam Geometries Types of remaining geometries.
 *
 * \param[in] g First geometry.
 * \param[in] gs Remaining geometries.
 *
 * \return BoundingBox containing all input geometries.
 *
 * \note If no geometries are provided, returns a default-constructed BoundingBox.
 *       For a single geometry, equivalent to calling envelope(const Geometry_t&).
 *
 * ### Example
 * \code
 * Point2d p1(0, 0);
 * Point2d p2(2, 2);
 * LineString2d line{{1, 1}, {3, 3}};
 * auto bbox = envelope(p1, p2, line);  // bbox.min() == {0, 0}, bbox.max() == {3, 3}
 * \endcode
 */
template<typename Geometry_t, typename... Geometries>
auto envelope(const Geometry_t &g, const Geometries&... gs);

/*! \} */ 

} // End namespace tl

#include "Envelope.impl.h"
