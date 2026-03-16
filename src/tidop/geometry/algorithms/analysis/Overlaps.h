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

/*! \file overlaps.h
 * \brief Spatial predicate "overlaps" for 2D geometries following OGC Simple Features.
 *
 * This file defines the `overlaps` function, which determines whether two geometries
 * of the same dimension overlap in their interiors.
 *
 * According to the OGC Simple Feature Access specification, two geometries **overlap**
 * if:
 * - They have the same dimension,
 * - The intersection of their interiors has the same dimension as the geometries
 *   themselves,
 * - The intersection is different from (i.e., a proper subset of) each of the
 *   geometries.
 *
 * Two overloads are provided:
 * - `overlaps(geom1, geom2)` – uses **native precision** (exact coordinate comparison).
 * - `overlaps(geom1, geom2, policy)` – applies a custom precision policy
 *   (e.g., fixed‑resolution snapping) before evaluation, essential for robust
 *   results with floating‑point data.
 *
 * \note Both overloads are constrained to 2D geometries (`Geometry2DConcept`).
 * \see tl::intersects, tl::touches, tl::within, tl::contains, tl::crosses, tl::disjoint
 */

#pragma once

#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/algorithms/analysis/Equals.h"
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/base/TopologyKernel.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if two 2D geometries overlap using native precision.
 *
 * Determines whether the two input geometries overlap according to the OGC definition.
 * For two polygons, this means they share a region of area (their interiors intersect)
 * but neither contains the other. For two line strings, they must cross or share a
 * common segment (but not just a point) and not one be contained in the other.
 *
 * This overload uses exact coordinate comparison (native precision).
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \return `true` if the geometries overlap; `false` otherwise.
 *
 * \note Empty geometries never overlap.
 *
 * ### Example
 * \code
 * Polygon2d poly1({Point2d{0,0}, Point2d{2,0}, Point2d{2,2}, Point2d{0,2}, Point2d{0,0}});
 * Polygon2d poly2({Point2d{1,1}, Point2d{3,1}, Point2d{3,3}, Point2d{1,3}, Point2d{1,1}});
 * bool o = overlaps(poly1, poly2); // true (they share the square [1,2]x[1,2])
 *
 * Polygon2d poly3({Point2d{0,0}, Point2d{1,0}, Point2d{1,1}, Point2d{0,1}, Point2d{0,0}});
 * bool o2 = overlaps(poly1, poly3); // false (poly3 is contained in poly1)
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto overlaps(const G1 &geom1,
                        const G2 &geom2) -> bool;

/*!
 * \brief Checks if two 2D geometries overlap using a custom precision policy.
 *
 * This overload applies a user‑defined precision policy before evaluating the
 * overlap relationship. The policy can snap coordinates to a fixed‑resolution grid,
 * effectively providing tolerance‑based evaluation that is robust against
 * floating‑point rounding.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \tparam Policy A type satisfying PrecisionPolicyConcept (e.g., PrecisionPolicy<Scalar, Mode>).
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] policy Precision policy used to snap coordinates before evaluation.
 * \return `true` if the geometries overlap after applying the policy;
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
 * Polygon2d poly1({Point2d{0,0}, Point2d{2,0}, Point2d{2,2}, Point2d{0,2}, Point2d{0,0}});
 * Polygon2d poly2({Point2d{1.0001,1.0001}, Point2d{3,1}, Point2d{3,3}, Point2d{1,3}, Point2d{1.0001,1.0001}});
 *
 * // With exact comparison, a tiny rounding might make them disjoint.
 * bool o = overlaps(poly1, poly2, pol); // true if snapped polygons overlap
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto overlaps(const G1 &geom1,
                        const G2 &geom2,
                        const Policy &policy) -> bool;



/*! \} */ 

} // End namespace tl

#include "Overlaps.impl.h"
