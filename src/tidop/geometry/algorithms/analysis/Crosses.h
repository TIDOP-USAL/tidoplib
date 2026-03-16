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

/*! \file crosses.h
 * \brief Spatial predicate "crosses" following OGC Simple Features.
 *
 * This file defines the `crosses` function, which determines whether two geometries
 * spatially cross each other.
 *
 * According to the OGC Simple Feature Access specification, two geometries **cross**
 * if:
 * - The geometries have some but not all interior points in common,
 * - The dimension of the intersection is less than the maximum dimension of the
 *   two geometries, and
 * - The intersection lies in the interior of both geometries (for certain type
 *   combinations) or involves the boundary appropriately.
 *
 * Several overloads are provided:
 * - Generic `crosses(geom1, geom2)` – works for any two 2D geometries using native precision.
 * - `crosses(geom1, geom2, policy)` – applies a custom precision policy.
 * - Specialized overloads for `Segment` and `LinearRing` for efficiency and clarity.
 *
 * \see tl::intersects, tl::overlaps, tl::touches, tl::within
 */

#pragma once

#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/Operations.h"
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/base/TopologyKernel.h"


namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if two 2D geometries cross using native precision.
 *
 * Evaluates the crosses relationship according to the OGC definition.
 * The exact semantics depend on the dimension and type of the geometries.
 * Common cases:
 * - A LineString crosses a Polygon if it enters and exits the polygon,
 *   intersecting the boundary at two points.
 * - A LineString crosses another LineString if they intersect at a point
 *   that is interior to both.
 *
 * This overload uses **native precision** (exact coordinate comparison).
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \return `true` if the two geometries cross; `false` otherwise.
 *
 * \note If the geometries are of types that cannot cross (e.g., two points),
 *       the result is always `false`.
 * \note Empty geometries never cross anything.
 *
 * ### Example
 * \code
 * LineString2d line{Point2d{0,0}, Point2d{10,10}};
 * Polygon2d poly({Point2d{5,0}, Point2d{15,0}, Point2d{15,10}, Point2d{5,10}, Point2d{5,0}});
 * bool result = crosses(line, poly); // true (line enters and exits the polygon)
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto crosses(const G1 &geom1, const G2 &geom2) -> bool;


/*!
 * \brief Checks if two 2D geometries cross using a custom precision policy.
 *
 * This overload applies a user‑defined precision policy before evaluating the
 * crosses relationship. The policy can snap coordinates to a fixed‑resolution grid,
 * providing tolerance‑based evaluation robust against floating‑point rounding.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \tparam Policy A type satisfying PrecisionPolicyConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] policy Precision policy used to snap coordinates before evaluation.
 * \return `true` if the geometries cross after applying the policy; `false` otherwise.
 */
template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto crosses(const G1 &geom1,
                       const G2 &geom2,
                       const Policy &policy) -> bool;


/*!
 * \brief Checks if a segment crosses a linear ring using a precision policy.
 *
 * Specialized overload for efficiency and clarity when testing whether a line
 * segment crosses a closed ring (boundary of a polygon). This is a common
 * operation in computational geometry.
 *
 * \tparam P Point type, must satisfy PointConcept.
 * \tparam Policy Precision policy type.
 * \param[in] segment The line segment.
 * \param[in] ring The linear ring (closed).
 * \param[in] policy Precision policy.
 * \return `true` if the segment crosses the ring; `false` otherwise.
 *
 * \note A segment crosses a ring if it has an odd number of intersections
 *       with the ring (entering/exiting). Zero or an even number indicates
 *       no crossing.
 */
template<PointConcept P, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto crosses(const Segment<P> &segment,
             const LinearRing<P> &ring,
             const Policy &policy) -> bool;


/*!
 * \brief Checks if a segment crosses a linear ring using native precision.
 *
 * Convenience overload that uses native precision.
 *
 * \tparam P Point type, must satisfy PointConcept.
 * \param[in] segment The line segment.
 * \param[in] ring The linear ring (closed).
 * \return `true` if the segment crosses the ring; `false` otherwise.
 */
template<PointConcept P>
[[nodiscard]]
auto crosses(const Segment<P> &segment,
             const LinearRing<P> &ring) -> bool;

/*! \} */ 

} // End namespace tl

#include "Crosses.impl.h"
