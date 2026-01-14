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
#include "tidop/geometry/base/EntityContainer.h"
#include "tidop/math/algebra/vector.h"

#include <array>

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */

template<typename Point_t>
class MultiPoint 
  : public GeometryBase<MultiPoint<Point_t>>, 
    public EntityContainer<Point_t> 
{
	
public:

    using EntityContainer<Point_t>::EntityContainer;
};


using MultiPoint2i = MultiPoint<Point2i>;
using MultiPoint3i = MultiPoint<Point3i>;
using MultiPoint2f = MultiPoint<Point2f>;
using MultiPoint3f = MultiPoint<Point3f>;
using MultiPoint2d = MultiPoint<Point2d>;
using MultiPoint3d = MultiPoint<Point3d>;

/*! \} */

} // End namespace tl
