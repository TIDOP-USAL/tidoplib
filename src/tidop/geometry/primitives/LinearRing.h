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
#include "tidop/geometry/base/GeometryContainer.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/measurement/Length.h"
#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{
	
/*! \addtogroup Primitives
 *  \{
 */

/*!
 * \class LinearRing
 * \brief A closed sequence of points forming a ring.
 *
 * \tparam Point_t Type of the points in the ring (e.g., Point2d, Point3f).
 */
template<typename Point_t>
class LinearRing
  : public GeometryContainer<Point_t>
{

public:

    using GeometryContainer<Point_t>::GeometryContainer;
    
public:

    /*!
     * \brief Checks if the ring is closed.
     * \return true if the ring is closed (first and last points are equal), false otherwise.
     */
    auto isClosed() const -> bool;

    /*!
     * \brief Checks if the ring is valid.
     * \return true if the ring is closed and has at least 3 points, false otherwise.
     */
    auto isValid() const -> bool;
};

// TYPE ALIASES FOR LINEARRING

/*! \brief 2D linear ring with integer coordinates. */
using LinearRing2i = LinearRing<Point2i>;

/*! \brief 2D linear ring with double coordinates. */
using LinearRing2d = LinearRing<Point2d>;

/*! \brief 2D linear ring with float coordinates. */
using LinearRing2f = LinearRing<Point2f>;

/*! \brief 3D linear ring with integer coordinates. */
using LinearRing3i = LinearRing<Point3i>;

/*! \brief 3D linear ring with double coordinates. */
using LinearRing3d = LinearRing<Point3d>;

/*! \brief 3D linear ring with float coordinates. */
using LinearRing3f = LinearRing<Point3f>;


// METHOD IMPLEMENTATIONS

template<typename Point_t>
auto LinearRing<Point_t>::isClosed() const -> bool
{
    if (this->size() < 3) {
        return false;
    }
    return this->front() == this->back();
}

template<typename Point_t>
auto LinearRing<Point_t>::isValid() const -> bool
{
    return isClosed();
}

/*! \} */

} // End namespace tl