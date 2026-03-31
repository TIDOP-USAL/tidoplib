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

/*! \file envelope.h
 * \brief Minimum bounding box (envelope) computation for geometries.
 *
 * This file defines the envelope algorithm, which computes the axis-aligned
 * minimum bounding box (also known as envelope or extent) that contains a given
 * geometry or a set of geometries. The result is a BoundingBox object that uses
 * the coordinate type of the input geometries but strips any measure information
 * (e.g., M-coordinates) to ensure a pure geometric bounding box.
 *
 * The envelope is a fundamental spatial operation used for spatial indexing,
 * culling, and approximations.
 *
 * \note If the input geometry is empty (e.g., empty LineString, empty MultiPoint),
 *       the function returns an empty BoundingBox (default-constructed).
 *
 * \see tl::BoundingBox, tl::GeometryConcept, tl::remove_measure_t
 */
#pragma once

#include <variant>
#include <type_traits>

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{
	
/*! \addtogroup SpatialOperations
 *  \{
 */

/*!
 * \brief Computes the minimum bounding box (envelope) of a geometry.
 * 
 * This function computes the axis-aligned bounding box that contains the given geometry.
 * If the geometry is empty, the result is an empty BoundingBox. The returned BoundingBox
 * uses the same coordinate type as the geometry but discards any measure components
 * (e.g., M-values) to produce a pure geometric extent.
 *
 * \tparam G Type of the geometry. Must satisfy the GeometryConcept.
 * \param[in] g Input geometry.
 * \return BoundingBox containing the input geometry. The point type of the returned
 *         BoundingBox is the same as the geometry's point type with measures removed.
 *         If the geometry is empty, returns a default-constructed (empty) BoundingBox.
 * 
 * ### Example
 * \code
 * Point2d p(1.0, 2.0);
 * auto bbox = envelope(p);  // bbox.min() == bbox.max() == Point2d(1.0,2.0)
 * 
 * LineString2d line{Point2d(0,0), Point2d(1,1), Point2d(2,0)};
 * auto bbox2 = envelope(line);  // bbox2.min() == {0,0}, bbox2.max() == {2,1}
 * 
 * MultiPoint2d emptyPoints;
 * auto bbox3 = envelope(emptyPoints);  // empty BoundingBox
 * \endcode
 */
template<GeometryConcept G>
[[nodiscard]]
auto envelope(const G &g);

/*!
 * \brief Computes the combined envelope of multiple geometries.
 *
 * This function computes the minimum bounding box that contains all input geometries.
 * It is equivalent to computing the envelope of each geometry and merging them using
 * the `merge` function. All geometries must use the same underlying point type
 * (same coordinate type and dimension), though they may include measure information
 * which is ignored in the result.
 *
 * \tparam Geometry_t Type of the first geometry. Must satisfy GeometryConcept.
 * \tparam Geometries Types of remaining geometries. Each must satisfy GeometryConcept.
 *
 * \param[in] g First geometry.
 * \param[in] gs Remaining geometries.
 *
 * \return BoundingBox containing all input geometries. The point type is the same as
 *         the geometries' point type with measures removed. If all geometries are empty,
 *         returns an empty BoundingBox.
 *
 * ### Example
 * \code
 * Point2d p1(0, 0);
 * Point2d p2(2, 2);
 * LineString2d line{Point2d(1, 1), Point2d(3, 3)};
 * auto bbox = envelope(p1, p2, line);  // bbox.min() == {0, 0}, bbox.max() == {3, 3}
 * \endcode
 */
template<typename Geometry_t, typename... Geometries>
[[nodiscard]]
auto envelope(const Geometry_t &g, const Geometries&... gs);

/*! \} */ 

} // End namespace tl

#include "Envelope.impl.h"
