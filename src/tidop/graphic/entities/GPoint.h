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

/*! \file GPoint.h
 * \brief 2D point graphic entity for rendering.
 *
 * This file defines the `GPoint` class, which represents a 2D point
 * as a graphic entity that can be rendered using a `Painter`. It inherits from
 * `GraphicEntity` and stores a 2D point geometry (`Point2d`).
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::Point2d
 */

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
 * \class GPoint
 * \brief 2D point graphic entity.
 *
 * This class wraps a 2D point geometry (`Point2d`) and provides the necessary
 * interface for rendering via a `Painter`. It also implements the `clone()`
 * method and computes the bounding window of the point.
 *
 * ### Example
 * \code
 * GPoint gpoint(10.0, 20.0);
 * gpoint.setSymbol(Symbol(Symbol::Type::Circle, 8.0));
 * painter.draw(gpoint);
 * \endcode
 */
class TL_EXPORT GPoint
  : public GraphicEntity
{

private:

    Point2d mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs a 2D point at the origin `(0, 0)`.
     */
    GPoint();

    /*!
     * \brief Constructs a 2D point from coordinates.
     * \param[in] x X-coordinate.
     * \param[in] y Y-coordinate.
     */
    GPoint(double x, double y);

    /*!
     * \brief Constructs a 2D point from a `Point2d` object.
     * \param[in] pt The 2D point.
     */
    explicit GPoint(const Point<double> &pt);

    /*!
     * \brief Copy constructor.
     * \param[in] pt Object to copy.
     */
    GPoint(const GPoint &pt) = default;

    /*!
     * \brief Move constructor.
     * \param[in] pt Object to move.
     */
    GPoint(GPoint &&pt) noexcept = default;


    ~GPoint() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gPoint Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GPoint &gPoint) -> GPoint & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] gPoint Object to move.
     * \return Reference to this object.
     */
    auto operator =(GPoint &&gPoint) noexcept -> GPoint & = default;

    /*!
     * \brief Returns a const reference to the underlying point geometry.
     * \return Const reference to `Point2d`.
     */
    auto geometry() const -> const Point2d & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying point geometry.
     * \return Reference to `Point2d`.
     */
    auto geometry() -> Point2d & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the point.
     * \return A `BoundingBox<Point2d>` containing the point.
     */
    auto window() const -> BoundingBox<Point2d> override;

    /*!
     * \brief Renders the point using the given painter.
     * \param[in] painter The painter used for rendering.
     */
    void draw(Painter &painter) const override;

    /*!
     * \brief Creates a deep copy of this graphic entity.
     * \return A unique pointer to the cloned object.
     */
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};

/*! \} */

} // namespace tl

