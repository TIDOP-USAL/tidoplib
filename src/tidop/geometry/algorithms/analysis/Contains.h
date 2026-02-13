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

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/Operations.h"

#include <variant>

namespace tl
{

/*! \addtogroup SpatialPredicates
 *  \{
 */

// Según OGC Simple Features contains(A, B) es true si y solo si:
//
// - B está completamente en el interior de A
//
// - ningún punto de B está en la frontera de A
//
// - A ≠ B

template<typename G1, typename G2>
[[nodiscard]]
auto contains(const G1 &g1, const G2 &g2) -> bool;

/*! \} */ 

} // End namespace tl

#include "Contains.impl.h"
