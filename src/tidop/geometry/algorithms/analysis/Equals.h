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
 * Compares only spatial dimensions (X,Y,Z) with a type-adapted absolute tolerance:
 * - float:  1e-6
 * - double: 1e-12
 * - int:    0 (exact comparison)
 *
 * Measure (M) is ignored (OGC Simple Feature Access).
 *
 * \see operator== for exact comparison including measure.
 */
template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, const G2 &geom2) -> bool;


/*!
 * \brief Spatial equality with explicit absolute tolerance.
 *
 * \param tolerance Absolute tolerance for coordinate comparison.
 */
template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, const G2 &geom2, double tolerance) -> bool;



template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, 
                      const G2 &geom2, 
                      const TolerancePolicy &policy) -> bool;

/*! \} */ 

} // End namespace tl

#include "Equals.impl.h"
