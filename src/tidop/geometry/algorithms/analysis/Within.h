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

/*! \file within.h
 * \brief Spatial predicate "within" for 2D geometries following OGC Simple Features.
 *
 * This file defines the `within` function, which determines whether one geometry
 * is completely inside another.
 *
 * According to the OGC Simple Feature Access specification, a geometry A is **within**
 * a geometry B if and only if:
 * - A is completely inside the interior of B,
 * - no point of A lies on the boundary of B,
 * - and A ≠ B (i.e., they are not exactly equal).
 *
 * This is the inverse of the `contains` predicate.
 *
 * Two overloads are provided:
 * - `within(geom1, geom2)` – uses **native precision** (exact coordinate comparison).
 * - `within(geom1, geom2, policy)` – applies a custom precision policy
 *   (e.g., fixed‑resolution snapping) before evaluation, essential for robust
 *   results with floating‑point data.
 *
 * \note Both overloads are constrained to 2D geometries (`Geometry2DConcept`).
 * \see tl::contains, tl::covers, tl::coveredBy, tl::intersects, tl::touches
 */

#pragma once

#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/base/TopologyKernel.h"
#include "tidop/geometry/algorithms/analysis/contains.h"
#include "tidop/geometry/algorithms/analysis/Intersects.h"
#include "tidop/geometry/algorithms/analysis/Boundary.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if the first geometry is within the second using native precision.
 *
 * Evaluates the within relationship according to the OGC definition:
 * - The interior of `geom1` must be completely inside the interior of `geom2`.
 * - No point of `geom1` may lie on the boundary of `geom2`.
 * - `geom1` and `geom2` must not be equal.
 *
 * This overload uses exact coordinate comparison (native precision).
 *
 * \tparam G1 First geometry type (the one that may be within), must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type (the containing one), must satisfy Geometry2DConcept.
 * \param[in] geom1 The geometry that may be within.
 * \param[in] geom2 The geometry that may contain.
 * \return `true` if `geom1` is within `geom2` according to the OGC definition;
 *         `false` otherwise.
 *
 * \note Empty geometries are never within anything (result is `false`).
 * \note If the two geometries are of types that cannot satisfy the relationship
 *       (e.g., a polygon cannot be within a point), the result is `false`.
 *
 * ### Example
 * \code
 * Polygon2d poly({Point2d{0,0}, Point2d{10,0}, Point2d{10,10}, Point2d{0,10}, Point2d{0,0}});
 * Point2d inside(5,5);
 * Point2d onBoundary(10,5);
 * Point2d outside(15,15);
 *
 * bool w1 = within(inside, poly);   // true
 * bool w2 = within(onBoundary, poly); // false (point on boundary)
 * bool w3 = within(outside, poly);  // false
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto within(const G1 &geom1, const G2 &geom2) -> bool;


/*!
 * \brief Checks if the first geometry is within the second using a custom precision policy.
 *
 * This overload applies a user‑defined precision policy before evaluating the
 * within relationship. The policy can snap coordinates to a fixed‑resolution grid,
 * effectively providing tolerance‑based evaluation that is robust against
 * floating‑point rounding.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \tparam Policy A type satisfying PrecisionPolicyConcept (e.g., PrecisionPolicy<Scalar, Mode>).
 * \param[in] geom1 The geometry that may be within.
 * \param[in] geom2 The geometry that may contain.
 * \param[in] policy Precision policy used to snap coordinates before evaluation.
 * \return `true` if `geom1` is within `geom2` after applying the policy;
 *         `false` otherwise.
 *
 * \note The policy is applied individually to each coordinate of the geometries.
 *       For `PrecisionPolicy<Scalar, PrecisionModel::FixedPrecisionModel>`, this
 *       means all coordinates are rounded to the nearest grid point at the given
 *       resolution.
 *
 * ### Example
 * \code
 * using Policy = PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel>;
 * Policy pol(0.001);
 *
 * Polygon2d poly({{0,0}, {10,0}, {10,10}, {0,10}, {0,0}});
 * Point2d nearBoundary(9.999, 5.0); // extremely close to boundary
 *
 * // Without policy, this might be false due to rounding.
 * bool w = within(nearBoundary, poly, pol); // true if snapped point lies inside
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto within(const G1 &geom1,
                      const G2 &geom2,
                      const Policy &policy) -> bool;


/*! \} */ 

} // End namespace tl

#include "Within.impl.h"
