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
 * \brief Spatial predicate "disjoint" following OGC Simple Features.
 *
 * This file defines the `disjoint` function, which determines whether two geometries
 * have no points in common.
 *
 * According to the OGC Simple Feature Access specification, two geometries are
 * **disjoint** if their intersection is empty.
 *
 * The function is implemented in terms of `intersects` (i.e., `!intersects(geom1, geom2)`)
 * and uses **native precision** (exact coordinate comparison). For tolerance‑based
 * evaluation, consider applying a precision policy via other spatial predicates.
 *
 * \see tl::intersects, tl::touches, tl::within, tl::contains, tl::overlaps
 */

#pragma once

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/base/PrecisionPolicy.h"

namespace tl
{


template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto disjoint(const G1 &geom1, const G2 &geom2) -> bool
{
    using Scalar = typename point_traits<geometry_traits<G1>::point_type>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return disjoint(geom1, geom2, policy);
}


template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto disjoint(const G1 &geom1,
                        const G2 &geom2,
                        const Policy &policy) -> bool
{
    return !intersects(geom1, geom2, policy);
}

} // namespace tl