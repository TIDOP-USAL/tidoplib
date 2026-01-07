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
	
namespace geometry
{
	
/*! \addtogroup GeometricEntities
 *  \{
 */

template<typename Point_t>
class MultiPolygon
  : public GeometryBase<MultiPolygon<Point_t>>, 
    public EntityContainer<Polygon<Point_t>> 
{
    using EntityContainer<Polygon<Point_t>>::EntityContainer;
};

using MultiPolygon2i = MultiPolygon<Point2i>;
using MultiPolygon3i = MultiPolygon<Point3i>;
using MultiPolygon2f = MultiPolygon<Point2f>;
using MultiPolygon3f = MultiPolygon<Point3f>;
using MultiPolygon2d = MultiPolygon<Point2d>;
using MultiPolygon3d = MultiPolygon<Point3d>;


/*! \} */

}
}
