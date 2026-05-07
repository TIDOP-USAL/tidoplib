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

/*! \file contains.h
 * \brief Spatial predicate "contains" following OGC Simple Features.
 *
 * This file defines the `contains` function, which determines whether one geometry
 * spatially contains another.
 *
 * According to the OGC Simple Feature Access specification, a geometry A **contains**
 * a geometry B if and only if:
 * - B is completely inside the interior of A,
 * - no point of B lies on the boundary of A,
 * - and A ≠ B (i.e., they are not exactly equal).
 *
 * Two overloads are provided:
 * - `contains(geom1, geom2)` – uses **native precision** (direct coordinate comparison).
 * - `contains(geom1, geom2, policy)` – applies a custom precision policy
 *   (e.g., fixed‑resolution snapping) before evaluation, essential for robust
 *   results with floating‑point data.
 *
 * \note The function is currently constrained to 2D geometries (`Geometry2DConcept`).
 * \see tl::within, tl::covers, tl::coveredBy, tl::intersects, tl::touches
 */

#pragma once

#include <variant>

#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/base/TopologyKernel.h"
#include "tidop/geometry/Operations.h"
#include "tidop/geometry/algorithms/analysis/Crosses.h"
#include "tidop/geometry/algorithms/analysis/Intersects.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if the first geometry contains the second using native precision.
 *
 * Evaluates the contains relationship according to the OGC definition:
 * - The interior of `geom2` must be completely inside the interior of `geom1`.
 * - No point of `geom2` may lie on the boundary of `geom1`.
 * - `geom1` and `geom2` must not be equal.
 *
 * This overload uses **native precision**, meaning coordinates are compared
 * exactly as stored. For floating‑point types this may be too strict due to
 * rounding errors; consider using the overload with a precision policy for
 * tolerance‑based evaluation.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \param[in] geom1 The containing geometry (A).
 * \param[in] geom2 The geometry that may be contained (B).
 * \return `true` if `geom1` contains `geom2` according to the OGC definition;
 *         `false` otherwise.
 *
 * \note Empty geometries never contain anything (result is `false`).
 * \note If the two geometries are of types that cannot satisfy the relationship
 *       (e.g., a point cannot contain a polygon), the result is `false`.
 *
 * ### Example
 * \code
 * Polygon2d poly({Point2d{0,0}, Point2d{10,0}, Point2d{10,10}, Point2d{0,10}, Point2d{0,0}});
 * Point2d inside(5,5);
 * Point2d onBoundary(10,5);
 * Point2d outside(15,15);
 *
 * bool c1 = contains(poly, inside);    // true
 * bool c2 = contains(poly, onBoundary); // false (point on boundary)
 * bool c3 = contains(poly, outside);   // false
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
auto contains(const G1 &geom1, const G2 &geom2) -> bool;


/*!
 * \brief Checks if the first geometry contains the second using a custom precision policy.
 *
 * This overload applies a user‑defined precision policy before evaluating the
 * contains relationship. The policy can snap coordinates to a fixed‑resolution grid,
 * effectively providing tolerance‑based evaluation that is robust against
 * floating‑point rounding.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \tparam Policy A type satisfying PrecisionPolicyConcept (e.g., PrecisionPolicy<Scalar, Mode>).
 * \param[in] geom1 The containing geometry (A).
 * \param[in] geom2 The geometry that may be contained (B).
 * \param[in] policy Precision policy used to snap coordinates before evaluation.
 * \return `true` if `geom1` contains `geom2` after applying the policy; `false` otherwise.
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
 * Polygon2d poly({Point2d{0,0}, Point2d{10,0}, Point2d{10,10}, Point2d{0,10}, Point2d{0,0}});
 * Point2d nearBoundary(9.999, 5.0); // extremely close to boundary
 *
 * // Without policy, this might be false due to rounding.
 * bool c = contains(poly, nearBoundary, pol); // true if snapped point lies inside
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto contains(const G1 &geom1,
                        const G2 &geom2, 
                        const Policy &policy) -> bool;


/*! \} */ 

} // End namespace tl

#include "Contains.impl.h"
