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

#include "tidop/core/base/type_conversions.h"
#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/math/algebra/vector.h"

#include <array>

namespace tl
{
	
/*! \addtogroup GeometricEntities
 *  \{
 */


template<typename Point_t>
class MultiLineString 
  : public GeometryBase<MultiLineString<Point_t>>, 
    public EntityContainer<LineString<Point_t>> 
{
    using EntityContainer<LineString<Point_t>>::EntityContainer;
};

using MultiLineString2i = MultiLineString<Point2i>;
using MultiLineString3i = MultiLineString<Point3i>;
using MultiLineString2f = MultiLineString<Point2f>;
using MultiLineString3f = MultiLineString<Point3f>;
using MultiLineString2d = MultiLineString<Point2d>;
using MultiLineString3d = MultiLineString<Point3d>;

/*! \} */

} // End namespace tl
