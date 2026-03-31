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

/*! \file EqualsExact.h
 * \brief Spatial equality predicates following OGC Simple Features.
 *
 * This file defines the `equalsExact` family of functions, which test whether two
 * geometries are spatially equal within a given tolerance. Only spatial dimensions
 * (X, Y, Z) are considered; measure (M) is ignored, in accordance with OGC Simple
 * Feature Access specification.
 *
 * The functions are overloaded to accept:
 * - Default tolerance adapted to the coordinate type (float, double, int).
 * - A precision policy (`PrecisionPolicy`) that defines how coordinates are
 *   compared (e.g., using fixed resolution).
 *
 * All overloads enforce that the two geometries have the same spatial dimension
 * (2D, 3D) via the `SameSpatialDimension` concept.
 *
 * \see tl::PrecisionPolicy, tl::GeometryConcept, tl::SameSpatialDimension
 */

#pragma once

#include <variant>

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
 * \brief Spatial equality using native precision.
 *
 * Compares two geometries by directly comparing their coordinate values
 * without any snapping or tolerance. For integral types this is exact;
 * for floating‑point types it performs a direct (bitwise) comparison,
 * which may fail for values that are mathematically equal but differ
 * due to rounding. If you need a tolerance‑based comparison, use the
 * overload that accepts a precision policy.
 *
 * The comparison is performed only on the spatial dimensions (X, Y, Z);
 * any measure component (M) is ignored, following OGC Simple Feature Access.
 *
 * \tparam G1 First geometry type, must satisfy GeometryConcept.
 * \tparam G2 Second geometry type, must satisfy GeometryConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \return `true` if the two geometries are spatially equal according to
 *         native (exact) comparison; `false` otherwise.
 *
 * \note If the two geometries are of different types (e.g., Point vs LineString),
 *       the result is always `false` (even if they represent the same set of points).
 * \note Empty geometries of the same type are considered equal (e.g., two empty
 *       MultiPoints compare equal).
 *
 * ### Example
 * \code
 * Point2d p1(1.0, 2.0);
 * Point2d p2(1.0, 2.0);
 * bool eq = equalsExact(p1, p2); // true
 *
 * Point2d p3(1.0 + 1e-15, 2.0); // tiny rounding difference
 * bool eq2 = equalsExact(p1, p3); // false (direct comparison)
 * \endcode
 */
template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1,
                           const G2 &geom2) -> bool;

/*!
 * \brief Spatial equality using a precision policy.
 *
 * Compares two geometries using a `PrecisionPolicy` object. The policy defines
 * how coordinates are snapped or compared, for instance by converting them to
 * a fixed-resolution integer grid. This enables robust, deterministic equality
 * tests independent of floating-point roundoff.
 *
 * \tparam G1 First geometry type, must satisfy GeometryConcept.
 * \tparam G2 Second geometry type, must satisfy GeometryConcept.
 * \tparam Policy A type satisfying `PrecisionPolicyConcept` (e.g., `PrecisionPolicy<Scalar, Mode>`).
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] policy Precision policy that provides the `snap` operation for coordinates.
 * \return `true` if the two geometries are spatially equal according to the policy;
 *         `false` otherwise.
 *
 * ### Example
 * \code
 * using Policy = PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel>;
 * Policy pol(1e-3);
 *
 * Point2d p1(1.0, 2.0);
 * Point2d p2(1.0000001, 2.0000001);
 * bool eq = equalsExact(p1, p2, pol); // true (both snap to same grid point)
 * \endcode
 */
template<GeometryConcept G1, GeometryConcept G2, PrecisionPolicyConcept Policy>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1,
                           const G2 &geom2,
                           const Policy &policy);

/*! \} */ 

} // End namespace tl

#include "EqualsExact.impl.h"
