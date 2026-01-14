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

#include <utility>

#include "tidop/geometry/base/Dimension.h"
#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/EntityContainer.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/geometry/algorithms/measurement/Length.h"

namespace tl
{
	
/*! \addtogroup GeometricEntities
 *  \{
 */

 /**
  * \brief A LineString is a Curve with linear interpolation between points.
  */
template<typename Point_t>
class LineString 
  : public GeometryBase<LineString<Point_t>>,
    public EntityContainer<Point_t>
{
public:

    using value_type = Point_t;

public:

    using EntityContainer<Point_t>::EntityContainer;

    auto numPoints() const { return this->size(); }

    // Un acceso rápido para saber si es cerrada
    bool isClosed() const
    {
        if (this->size() < 2) return false;
        return this->front() == this->back();
    }

    auto boundingBox() const
    {
        return envelope(*this);
    }

    auto length() const -> double
    {
        return tl::length(*this);
    }
};

using LineString2i = LineString<Point2i>;
using LineString2f = LineString<Point2f>;
using LineString2d = LineString<Point2d>;
using LineString3i = LineString<Point3i>;
using LineString3f = LineString<Point3f>;
using LineString3d = LineString<Point3d>;

/*! \} */

} // End namespace tl