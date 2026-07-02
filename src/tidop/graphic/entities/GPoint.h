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

#include "tidop/geometry/primitives/Point.h"
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
 * \brief Point graphic class
 */
class TL_EXPORT GPoint
  : public GraphicEntity
{

private:

    Point2d mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GPoint() = default;

    /*!
     * \brief Constructor
     * \param[in] x x coordinate
     * \param[in] y y coordinate
     */
    GPoint(double x, double y);

    /*!
     * \brief Constructor from a Point
     * \param[in] pt Point
     * \see Point
     */
    explicit GPoint(const Point<double> &pt);

    /*!
     * \brief Copy constructor
     * \param[in] pt Objeto GPoint to be copied
     */
    GPoint(const GPoint &pt) = default;

    /*!
     * \brief Move Constructor
     * \param[in] pt Objeto GPoint object that moves
     */
    GPoint(GPoint &&pt) noexcept = default;

    ~GPoint() override = default;

    /*!
     * \brief Copy assignment operator
     * \param gPoint GPoint object to be assigned
     * \return Object reference
     */
    auto operator =(const GPoint &gPoint) -> GPoint& = default;

    /*!
     * \brief Move assignment operator
     * \param gPoint GPoint object that moves
     * \return Object reference
     */
    auto operator =(GPoint&& gPoint) noexcept -> GPoint& = default;

    auto geometry() const -> const Point2d& { return mGeometry; }
    auto geometry() -> Point2d& { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


inline auto GPoint::isMultiEntity() const -> bool
{
    return false;
}

inline auto GPoint::isSimpleEntity() const -> bool
{
    return true;
}


/*! \} */ // Fin GraphicEntities

} // namespace tl

