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

#include "tidop/config.h"

#include "tidop/core/base/Concepts.h"
#include "tidop/geometry/base/Traits.h"

namespace tl
{


/* CONCEPTS (C++20) */

/*!
 * \brief Concept for geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept GeometryConcept = is_geometry_v<G>;

/*!
 * \brief Concept for 2D geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept Geometry2DConcept = GeometryConcept<G> && is_2d_v<G>;

/*!
 * \brief Concept for 3D geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept Geometry3DConcept = GeometryConcept<G> && is_3d_v<G>;

/*!
 * \brief Concept for 4D geometry types.
 * \tparam G Type to test.
 */
template<typename G>
concept Geometry4DConcept = GeometryConcept<G> && is_4d_v<G>;

template<typename P>
concept PointConcept = is_geometry_v<P> &&
                       geometry_type_v<P> == GeometryType::point &&
                       std::regular<P>;

/*! \} */ 

} // End namespace tl