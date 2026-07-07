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

/*! \file GMultiPoint.h
 * \brief 2D multi-point graphic entity for rendering.
 *
 * This file defines the `GMultiPoint` class, which represents a collection
 * of 2D points as a graphic entity that can be rendered using a `Painter`.
 * It inherits from `GraphicEntity` and stores a `MultiPoint<Point<double>>` geometry.
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::MultiPoint, tl::Point2d
 */

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
 * \class GMultiPoint
 * \brief 2D multi-point graphic entity.
 *
 * This class wraps a 2D multi-point geometry (`MultiPoint<Point<double>>`) and
 * provides the necessary interface for rendering via a `Painter`. It also
 * implements the `clone()` method and computes the bounding window of the
 * multi-point collection.
 *
 * ### Example
 * \code
 * MultiPoint<Point2d> mp = {Point2d{10.0, 20.0}, Point2d{30.0, 40.0}, Point2d{50.0, 60.0}};
 * GMultiPoint gmp(mp);
 * gmp.setSymbol(Symbol(Symbol::Type::Square, 6.0));
 * painter.draw(gmp);
 * \endcode
 */
class TL_EXPORT GMultiPoint
  : public GraphicEntity
{
private:

    MultiPoint<Point<double>> mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs an empty 2D multi-point graphic entity.
     */
    GMultiPoint();

    /*!
     * \brief Constructs with a pre-allocated number of points.
     * \param[in] size Initial number of points in the collection.
     */
    explicit GMultiPoint(size_t size);

    /*!
     * \brief Constructs from a 2D multi-point.
     * \param[in] multiPoint The 2D multi-point geometry.
     */
    explicit GMultiPoint(const MultiPoint<Point<double>> &multiPoint);

    /*!
     * \brief Copy constructor.
     * \param[in] gMultiPoint Object to copy.
     */
    GMultiPoint(const GMultiPoint &gMultiPoint) = default;

    /*!
     * \brief Move constructor.
     * \param[in] gMultiPoint Object to move.
     */
    GMultiPoint(GMultiPoint &&gMultiPoint) noexcept = default;

    ~GMultiPoint() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gMultiPoint Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GMultiPoint &gMultiPoint) -> GMultiPoint & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] gMultiPoint Object to move.
     * \return Reference to this object.
     */
    auto operator =(GMultiPoint &&gMultiPoint) noexcept -> GMultiPoint & = default;

    /*!
     * \brief Returns a const reference to the underlying multi-point geometry.
     * \return Const reference to `MultiPoint<Point<double>>`.
     */
    auto geometry() const -> const MultiPoint<Point<double>> & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying multi-point geometry.
     * \return Reference to `MultiPoint<Point<double>>`.
     */
    auto geometry() -> MultiPoint<Point<double>> & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the multi-point collection.
     * \return A `BoundingBox<Point2d>` enclosing the multi-point.
     */
    auto window() const -> BoundingBox<Point2d> override;

    /*!
     * \brief Renders the multi-point collection using the given painter.
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

