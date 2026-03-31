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
#include "tidop/geometry/base/PrecisionPolicy.h"
#include "tidop/geometry/base/TopologyKernel.h"

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */


template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, const G2 &geom2) -> bool;


template<GeometryConcept G1, GeometryConcept G2, PrecisionPolicyConcept Policy>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
constexpr auto equals(const G1 &geom1, 
                      const G2 &geom2, 
                      const Policy &policy) -> bool;

/*! \} */ 

} // End namespace tl

#include "Equals.impl.h"
