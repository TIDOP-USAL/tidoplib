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

/*! \file disjoint.h
 * \brief Spatial predicate "disjoint" for 2D geometries following OGC Simple Features.
 *
 * This file defines the `disjoint` function, which determines whether two 2D geometries
 * have no points in common.
 *
 * According to the OGC Simple Feature Access specification, two geometries are
 * **disjoint** if their intersection is empty. This is the logical opposite of
 * `intersects`.
 *
 * Two overloads are provided:
 * - `disjoint(geom1, geom2)` – uses **native precision** (exact coordinate comparison).
 * - `disjoint(geom1, geom2, policy)` – applies a custom precision policy
 *   (e.g., fixed‑resolution snapping) before evaluation, essential for robust
 *   results with floating‑point data.
 *
 * \note Both overloads are constrained to 2D geometries (`Geometry2DConcept`).
 * \see tl::intersects, tl::touches, tl::within, tl::contains, tl::overlaps
 */


#pragma once

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/algorithms/analysis/Intersects.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if two 2D geometries are disjoint using native precision.
 *
 * Determines whether the two input geometries share no common points.
 * Equivalent to `!intersects(geom1, geom2)`.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \return `true` if the geometries are disjoint (intersection is empty);
 *         `false` otherwise.
 *
 * \note Empty geometries are disjoint from everything, including themselves,
 *       because their intersection is empty.
 *
 * ### Example
 * \code
 * Point2d p1(0, 0);
 * Point2d p2(1, 1);
 * bool d1 = disjoint(p1, p2); // true (different points)
 *
 * Point2d p3(2, 2);
 * LineString2d line{Point2d{0,0}, Point2d{1,1}, Point2d{2,2}};
 * bool d2 = disjoint(p3, line); // false (p3 lies on the line)
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto disjoint(const G1 &geom1, const G2 &geom2) -> bool;


/*!
 * \brief Checks if two 2D geometries are disjoint using a custom precision policy.
 *
 * This overload applies a user‑defined precision policy before evaluating the
 * disjoint relationship. The policy can snap coordinates to a fixed‑resolution grid,
 * effectively providing tolerance‑based evaluation that is robust against
 * floating‑point rounding. Equivalent to `!intersects(geom1, geom2, policy)`.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \tparam Policy A type satisfying PrecisionPolicyConcept (e.g., PrecisionPolicy<Scalar, Mode>).
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] policy Precision policy used to snap coordinates before evaluation.
 * \return `true` if the geometries are disjoint after applying the policy;
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
 * Point2d p(1.0001, 2.0);
 * LineString2d line{Point2d{1.0, 2.0}, Point2d{3.0, 2.0}};
 *
 * // Without policy, tiny rounding might cause intersection to be missed.
 * bool d = disjoint(p, line, pol); // false if snapped point lies on snapped line
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
constexpr auto disjoint(const G1 &geom1,
                        const G2 &geom2,
                        const Policy &policy) -> bool;


/*! \} */ 

} // End namespace tl

#include "Disjoint.impl.h"
