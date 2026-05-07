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

/*! \file touches.h
 * \brief Spatial predicate "touches" following OGC Simple Features.
 *
 * This file defines the `touches` function, which determines whether two
 * geometries touch at their boundaries but do not intersect in their interiors.
 *
 * According to the OGC Simple Feature Access specification, two geometries
 * **touch** if:
 * - The intersection of their interiors is empty, and
 * - The intersection of one geometry's boundary with the other geometry is non‑empty,
 *   or vice‑versa (or both).
 *
 * In other words, the geometries have at least one point in common, but only
 * at their boundaries (no interior overlap).
 *
 * Two overloads are provided:
 * - `touches(geom1, geom2)` – uses **native precision** (direct coordinate comparison).
 * - `touches(geom1, geom2, policy)` – applies a custom precision policy
 *   (e.g., fixed‑resolution snapping) before evaluation, which can be essential
 *   for robust results with floating‑point data.
 *
 * \see tl::intersects, tl::disjoint, tl::within, tl::overlaps, tl::crosses
 */

#pragma once

#include <variant>

#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/base/TopologyKernel.h"
#include "tidop/geometry/Operations.h"
#include "tidop/geometry/algorithms/analysis/Overlaps.h"
#include "tidop/geometry/algorithms/analysis/Boundary.h"
#include "tidop/geometry/algorithms/analysis/Within.h"
#include "tidop/geometry/algorithms/analysis/Crosses.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if two geometries touch using native precision.
 *
 * The interiors must be disjoint, but at least one point of the boundary of one
 * geometry must lie on the boundary or interior of the other.
 *
 * This overload uses **native precision**, meaning coordinates are compared
 * exactly as stored. For floating‑point types, this may be too strict due to
 * rounding errors; consider using the overload with a precision policy for
 * tolerance‑based evaluation.
 *
 * \tparam G1 First geometry type, must satisfy GeometryConcept.
 * \tparam G2 Second geometry type, must satisfy GeometryConcept.
 * \param[in] g1 First geometry.
 * \param[in] g2 Second geometry.
 * \return `true` if the two geometries touch according to the OGC definition;
 *         `false` otherwise.
 *
 * \note If the geometries are of types that cannot touch (e.g., two points
 *       cannot touch because they either coincide or are disjoint), the result
 *       is `false` (coincident points are considered equal, not touching).
 * \note Empty geometries never touch anything; the result is always `false`.
 *
 * ### Example
 * \code
 * Point2d p(1, 1);
 * LineString2d line{{0,0}, {2,2}};
 * bool result = touches(p, line); // true if p lies exactly on the endpoint of line
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
auto touches(const G1 &g1, const G2 &g2) -> bool;


/*!
 * \brief Checks if two geometries touch using a custom precision policy.
 *
 * This overload applies a user‑defined precision policy before evaluating the
 * spatial relationship. The policy can snap coordinates to a fixed‑resolution grid,
 * effectively providing tolerance‑based evaluation that is robust against
 * floating‑point rounding.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept (or more generally
 *            any GeometryConcept, but constrained here for 2D examples).
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \tparam Policy A type satisfying PrecisionPolicyConcept (e.g., PrecisionPolicy<Scalar, Mode>).
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] policy Precision policy used to snap coordinates before evaluation.
 * \return `true` if the geometries touch after applying the policy; `false` otherwise.
 *
 * \note The policy is applied individually to each coordinate of the geometries.
 *       For `PrecisionPolicy<Scalar, PrecisionModel::FixedPrecisionModel>`, this
 *       means all coordinates are rounded to the nearest grid point at the given
 *       resolution.
 *
 * ### Example
 * \code
 * using Policy = PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel>;
 * Policy pol(1e-6); // resolution of 1e-6
 *
 * Point2d p(1.0, 2.0);
 * LineString2d line{{0.9999999, 2.0}, {3.0, 2.0}};
 * bool result = touches(p, line, pol); // true if the snapped point touches the snapped line
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto touches(const G1 &geom1,
                       const G2 &geom2,
                       const Policy &policy) -> bool;


/*! \} */ 

} // End namespace tl

#include "Touches.impl.h"
