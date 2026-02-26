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

/*! \file equalsExact.h
 * \brief Spatial equality predicates following OGC Simple Features.
 *
 * This file defines the `equalsExact` family of functions, which test whether two
 * geometries are spatially equal within a given tolerance. Only spatial dimensions
 * (X, Y, Z) are considered; measure (M) is ignored, in accordance with OGC Simple
 * Feature Access specification.
 *
 * The functions are overloaded to accept:
 * - Default tolerance adapted to the coordinate type (float, double, int).
 * - Explicit absolute tolerance as a double.
 * - TolerancePolicy object.
 *
 * All overloads enforce that the two geometries have the same spatial dimension
 * (2D, 3D) via the `SameSpatialDimension` concept.
 *
 * \see tl::TolerancePolicy, tl::GeometryConcept, tl::SameSpatialDimension
 */

#pragma once

#include <variant>

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/Operations.h"
#include "tidop/geometry/base/TolerancePolicy.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

/*!
 * \brief Spatial equality with default tolerance.
 *
 * Compares two geometries using a default tolerance that depends on their
 * coordinate type:
 * - For `float`: 1e-6f
 * - For `double`: 1e-12
 * - For integral types: 0 (exact comparison)
 *
 * The comparison is performed only on the spatial dimensions (X, Y, Z);
 * any measure component (M) is ignored, following OGC Simple Feature Access.
 *
 * \tparam G1 First geometry type, must satisfy GeometryConcept.
 * \tparam G2 Second geometry type, must satisfy GeometryConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \return `true` if the two geometries are spatially equal within the default
 *         tolerance; `false` otherwise.
 *
 * \note If the two geometries are of different types (e.g., Point vs LineString),
 *       the result is always `false` (even if they represent the same set of points).
 * \note Empty geometries of the same type are considered equal (e.g., two empty
 *       MultiPoints compare equal).
 *
 * ### Example
 * \code
 * Point2d p1(1.0, 2.0);
 * Point2d p2(1.0 + 1e-13, 2.0 - 1e-13);
 * bool eq = equalsExact(p1, p2); // true (within 1e-12)
 *
 * Point2i pi1(1, 2);
 * Point2i pi2(1, 2);
 * bool eq_int = equalsExact(pi1, pi2); // true (exact)
 * \endcode
 */
template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1, const G2 &geom2) -> bool;


/*!
 * \brief Spatial equality with explicit absolute tolerance.
 *
 * Compares two geometries using a user-provided absolute tolerance.
 * The tolerance applies to each spatial coordinate independently:
 * \f$ |c1_i - c2_i| \le \text{tolerance} \f$ for all spatial dimensions i.
 *
 * \tparam G1 First geometry type, must satisfy GeometryConcept.
 * \tparam G2 Second geometry type, must satisfy GeometryConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] tolerance Absolute tolerance value (must be non‑negative).
 * \return `true` if the two geometries are spatially equal within the given
 *         tolerance; `false` otherwise.
 *
 * \note For geometries composed of multiple elements (LineString, Polygon,
 *       multi‑geometries), all corresponding elements must satisfy the tolerance.
 * \note The tolerance is applied uniformly to all spatial dimensions; for
 *       per‑axis tolerances use the TolerancePolicy overload.
 *
 * ### Example
 * \code
 * Point2d p1(1.0, 2.0);
 * Point2d p2(1.0 + 1e-8, 2.0 - 1e-8);
 * bool eq = equalsExact(p1, p2, 1e-7); // true (1e-8 ≤ 1e-7)
 * bool eq2 = equalsExact(p1, p2, 1e-9); // false
 * \endcode
 */
template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1, const G2 &geom2, double tolerance) -> bool;


/*!
 * \brief Spatial equality using a tolerance policy.
 *
 * Compares two geometries using a TolerancePolicy object, which may specify
 * different tolerances per axis (e.g., XY tolerance vs Z tolerance) and
 * other settings.
 *
 * \tparam G1 First geometry type, must satisfy GeometryConcept.
 * \tparam G2 Second geometry type, must satisfy GeometryConcept.
 * \param[in] geom1 First geometry.
 * \param[in] geom2 Second geometry.
 * \param[in] policy Tolerance policy defining comparison thresholds.
 * \return `true` if the two geometries are spatially equal according to the
 *         policy; `false` otherwise.
 *
 * \note This overload typically delegates to the double‑tolerance version
 *       using `policy.xyTolerance()` (or an appropriate per‑axis combination).
 *
 * ### Example
 * \code
 * TolerancePolicy pol(1e-6);            // uniform tolerance
 * TolerancePolicy pol2(1e-6, 1e-4);     // separate XY and Z tolerances
 * bool eq = equalsExact(geom1, geom2, pol);
 * \endcode
 */
template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equalsExact(const G1 &geom1, 
                      const G2 &geom2, 
                      const TolerancePolicy &policy) -> bool;

/*! \} */ 

} // End namespace tl

#include "EqualsExact.impl.h"
