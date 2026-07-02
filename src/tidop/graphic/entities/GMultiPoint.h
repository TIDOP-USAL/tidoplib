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

#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */

/*!
 * \brief Multi-point graphic class
 */
class TL_EXPORT GMultiPoint
  : public GraphicEntity
{
private:

    MultiPoint<Point<double>> mGeometry;

public:

    GMultiPoint() = default;
    explicit GMultiPoint(size_t size);
    explicit GMultiPoint(const MultiPoint<Point<double>> &multiPoint);
    GMultiPoint(const GMultiPoint &gMultiPoint) = default;
    GMultiPoint(GMultiPoint &&gMultiPoint) noexcept = default;
    ~GMultiPoint() override = default;

    auto operator =(const GMultiPoint& gMultiPoint) -> GMultiPoint& = default;
    auto operator =(GMultiPoint &&gMultiPoint) noexcept -> GMultiPoint& = default;

    auto geometry() const -> const MultiPoint<Point<double>> & { return mGeometry; }
    auto geometry() -> MultiPoint<Point<double>> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const ->BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


inline auto GMultiPoint::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiPoint::isSimpleEntity() const -> bool
{
    return false;
}


/*! \} */ // Fin GraphicEntities

} // namespace tl

