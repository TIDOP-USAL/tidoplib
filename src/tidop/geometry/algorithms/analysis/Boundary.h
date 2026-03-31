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

/*! \file Boundary.h
 * \brief Boundary computation for geometric entities following OGC Simple Features.
 *
 * This file defines the boundary algorithm, which returns the topological boundary
 * of a geometry according to the Open Geospatial Consortium (OGC) Simple Feature
 * Access standard. The boundary of a geometry is the set of points that are
 * in the closure of the geometry but not in its interior.
 * 
 * ### OGC Boundary Rules:
 * - **Point**: empty set
 * - **MultiPoint**: empty set
 * - **LineString**: endpoints if not closed; empty if closed or degenerate
 * - **LinearRing**: empty set (treated as closed LineString)
 * - **Polygon**: collection of its rings (exterior and interior) as MultiLineString
 * - **MultiLineString**: points appearing an odd number of times as endpoints (Mod 2 rule)
 * - **MultiPolygon**: union of boundaries of constituent polygons
 * - **GeometryCollection**: union of boundaries of constituent geometries
 *
 * \see tl::GeometryConcept, tl::GeometryType, OGC Simple Feature Access - Part 1: Common Architecture
 */

#pragma once

#include <variant>
#include <unordered_map>

#include "tidop/geometry/base/Concepts.h"

namespace tl
{
	
/*! \addtogroup GeometricAlgorithms
 *  \{
 */

/*!
 * \brief Computes the topological boundary of a geometry.
 *
 * This function template computes the boundary of any geometry type that satisfies
 * the GeometryConcept. The result type depends on the input geometry:
 * - For 0D geometries (Point, MultiPoint): returns an empty GeometryCollection
 * - For 1D geometries (LineString): returns a MultiPoint containing endpoints (if open)
 * - For 2D geometries (Polygon): returns a MultiLineString containing all rings
 * - For collections (MultiLineString, MultiPolygon, GeometryCollection): returns
 *   the appropriate collection type following OGC rules.
 *
 * \tparam G A type satisfying the GeometryConcept.
 * \param[in] geometry The input geometry.
 * \return The boundary of the geometry as a geometry collection of appropriate type.
 *         The exact return type depends on the input:
 *         - `GeometryCollection<Point_t>` for Point, MultiPoint
 *         - `MultiPoint<Point_t>` for LineString, MultiLineString
 *         - `MultiLineString<Point_t>` for Polygon, MultiPolygon
 *         - `GeometryCollection<Point_t>` for GeometryCollection
 *
 * \note The implementation follows OGC Simple Feature Access specification.
 * 
 * ### Example (2D point):
 * \code{.cpp}
 * Point2d p(1.0, 2.0);
 * auto b = boundary(p); // empty GeometryCollection
 * \endcode
 * 
 * ### Example (open LineString):
 * \code{.cpp}
 * LineString2d line;
 * line.push_back(Point2d(0,0));
 * line.push_back(Point2d(1,1));
 * line.push_back(Point2d(2,0));
 * auto b = boundary(line); // MultiPoint containing (0,0) and (2,0)
 * \endcode
 * 
 * ### Example (closed LineString):
 * \code{.cpp}
 * LineString2d ring;
 * ring.push_back(Point2d(0,0));
 * ring.push_back(Point2d(1,1));
 * ring.push_back(Point2d(2,0));
 * ring.push_back(Point2d(0,0)); // closed
 * auto b = boundary(ring); // empty MultiPoint
 * \endcode
 * 
 * ### Example (Polygon with hole):
 * \code{.cpp}
 * Polygon2d poly;
 * // outer ring
 * LinearRing2d outer = {Point2d(0,0), Point2d(10,0), Point2d(10,10), Point2d(0,10), Point2d(0,0)};
 * // inner ring (hole)
 * LinearRing2d inner = {Point2d(2,2), Point2d(8,2), Point2d(8,8), Point2d(2,8), Point2d(2,2)};
 * poly = Polygon2d(outer, {inner});
 * auto b = boundary(poly); // MultiLineString containing both rings
 * \endcode
 */
template<GeometryConcept G>
[[nodiscard]]
auto boundary(const G &geometry);



/*! \} */ 

} // End namespace tl

#include "Boundary.impl.h"
