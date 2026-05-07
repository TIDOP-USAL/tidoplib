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

/*! \file intersects.h
 * \brief Spatial predicate "intersects" for 2D geometries following OGC Simple Features.
 *
 * This file defines the `intersects` function, which determines whether two geometries
 * share any point in common.
 *
 * According to the OGC Simple Feature Access specification, two geometries **intersect**
 * if their intersection is non‑empty. This is the logical opposite of `disjoint`.
 *
 * Two main overloads are provided:
 * - `intersects(geom1, geom2)` – uses **native precision** (exact coordinate comparison).
 * - `intersects(geom1, geom2, policy)` – applies a custom precision policy
 *   (e.g., fixed‑resolution snapping) before evaluation, essential for robust
 *   results with floating‑point data.
 *
 * Additionally, specialized overloads for `Segment` and `LinearRing` are included
 * for efficiency and clarity in common geometric operations.
 *
 * \note All overloads are constrained to 2D geometries (`Geometry2DConcept` or
 *       specific point types).
 * \see tl::disjoint, tl::touches, tl::within, tl::contains, tl::overlaps, tl::crosses
 */

#pragma once

#include <ranges>
#include <variant>

#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/base/TopologyKernel.h"
#include "tidop/geometry/Operations.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Checks if two 2D geometries intersect using a custom precision policy.
 *
 * Determines whether the two input geometries share any common point.
 * The evaluation is performed after applying the given precision policy,
 * which can snap coordinates to a fixed‑resolution grid, making the test
 * robust against floating‑point rounding.
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \tparam Policy A type satisfying PrecisionPolicyConcept (e.g., PrecisionPolicy<Scalar, Mode>).
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] policy Precision policy used to snap coordinates before evaluation.
 * \return `true` if the geometries intersect after applying the policy;
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
 * Policy pol(1e-6);
 *
 * Point2d p(1.0000001, 2.0);
 * LineString2d line{Point2d{1.0, 2.0}, Point2d{3.0, 2.0}};
 *
 * // Without policy, tiny rounding might miss the intersection.
 * bool i = intersects(p, line, pol); // true if snapped point lies on snapped line
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto intersects(const G1 &geom1,
                const G2 &geom2,
                const Policy &policy) -> bool;


/*!
 * \brief Checks if two 2D geometries intersect using native precision.
 *
 * Determines whether the two input geometries share any common point.
 * This overload uses exact coordinate comparison (native precision).
 *
 * \tparam G1 First geometry type, must satisfy Geometry2DConcept.
 * \tparam G2 Second geometry type, must satisfy Geometry2DConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \return `true` if the geometries intersect; `false` otherwise.
 *
 * \note Empty geometries never intersect anything (result is `false`).
 *
 * ### Example
 * \code
 * Point2d p(0, 0);
 * LineString2d line{Point2d{0,0}, Point2d{1,1}};
 * bool i = intersects(p, line); // true
 *
 * Point2d p2(10, 10);
 * bool i2 = intersects(p2, line); // false
 * \endcode
 */
template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
auto intersects(const G1 &geom1, const G2 &geom2) -> bool;


/*!
 * \brief Checks if a segment intersects a linear ring using a precision policy.
 *
 * Specialized overload for efficiency when testing intersection between a line
 * segment and a closed ring (e.g., the boundary of a polygon). The test is
 * performed after snapping coordinates with the given policy.
 *
 * \tparam P Point type, must satisfy PointConcept.
 * \tparam Policy Precision policy type.
 * \param[in] seg The line segment.
 * \param[in] ring The linear ring (closed).
 * \param[in] policy Precision policy.
 * \return `true` if the segment and the ring intersect; `false` otherwise.
 *
 * \note A segment may intersect a ring at a point (touching) or along an edge.
 */
template<PointConcept P, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto intersects(const Segment<P> &seg, 
                const LinearRing<P> &ring,
                const Policy &policy);


/*!
 * \brief Checks if a segment intersects a linear ring using native precision.
 *
 * Convenience overload that uses exact coordinate comparison.
 *
 * \tparam P Point type, must satisfy PointConcept.
 * \param[in] seg The line segment.
 * \param[in] ring The linear ring (closed).
 * \return `true` if the segment and the ring intersect; `false` otherwise.
 */
template<PointConcept P>
[[nodiscard]]
auto intersects(const Segment<P> &seg,
                const LinearRing<P> &ring);

/*! \} */

} // End namespace tl

#include "Intersects.impl.h"
