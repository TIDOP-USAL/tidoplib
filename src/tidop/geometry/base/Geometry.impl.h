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

/*! \file Geometry.h
 * \brief Base classes for geometric entities.
 *
 * This file defines the base classes for all geometric entities in the library.
 * It provides both a polymorphic base class and a CRTP base class for static
 * polymorphism and compile-time type information.
 * ### Classes
 * - \ref tl::Geometry : Abstract base class for geometric entities.
 * - \ref tl::GeometryBase : CRTP base class for geometric entities.
 * \see tl::GeometryContainer, tl::Dimension
 */

#pragma once

#include "tidop/geometry/algorithms/spatial/Envelope.h"


namespace tl
{
	
//template<typename Derived>
//auto Geometry<Derived>::envelope() const -> BoundingBox<typename geometry_traits<Derived>::point_type>
//{
//    return tl::envelope(this->derived());
//}
//
//template<typename Derived>
//auto Geometry<Derived>::boundingBox() const -> BoundingBox<typename geometry_traits<Derived>::point_type>
//{
//    return this->envelope();
//}

} // End namespace tl