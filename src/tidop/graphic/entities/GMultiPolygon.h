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

/*! \file GMultiPolygon.h
 * \brief 2D multi-polygon graphic entity for rendering.
 *
 * This file defines the `GMultiPolygon` class, which represents a collection
 * of 2D polygons as a graphic entity that can be rendered using a `Painter`.
 * It inherits from `GraphicEntity` and stores a `MultiPolygon<Point2d>` geometry.
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::MultiPolygon, tl::Point2d
 */

#pragma once

#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Multipolygon.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \class GMultiPolygon
 * \brief 2D multi-polygon graphic entity.
 *
 * This class wraps a 2D multi-polygon geometry (`MultiPolygon<Point2d>`) and
 * provides the necessary interface for rendering via a `Painter`. It also
 * implements the `clone()` method and computes the bounding window of the
 * multi-polygon.
 *
 * ### Example
 * \code
 * MultiPolygon<Point2d> mp = ...;
 * GMultiPolygon gmp(mp);
 * gmp.setPen(Pen(Color::Blue, 2.0));
 * gmp.setBrush(Brush(Color::Red, Brush::Style::Solid));
 * painter.draw(gmp);
 * \endcode
 */
class TL_EXPORT GMultiPolygon
  : public GraphicEntity
{

private:

    MultiPolygon<Point2d> mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs an empty 2D multi-polygon graphic entity.
     */
    GMultiPolygon();

    /*!
     * \brief Constructs with a pre-allocated number of polygons.
     * \param[in] size Initial number of polygons in the collection.
     */
    explicit GMultiPolygon(size_t size);

    /*!
     * \brief Constructs from a 2D multi-polygon.
     * \param[in] multiPolygon The 2D multi-polygon geometry.
     */
    explicit GMultiPolygon(const MultiPolygon<Point2d> &multiPolygon);

    /*!
     * \brief Copy constructor.
     * \param[in] multiPolygon Object to copy.
     */
    GMultiPolygon(const GMultiPolygon &multiPolygon) = default;

    /*!
     * \brief Move constructor.
     * \param[in] multiPolygon Object to move.
     */
    GMultiPolygon(GMultiPolygon &&multiPolygon) noexcept = default;

    ~GMultiPolygon() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] multiPolygon Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GMultiPolygon &multiPolygon) -> GMultiPolygon & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] multiPolygon Object to move.
     * \return Reference to this object.
     */
    auto operator =(GMultiPolygon &&multiPolygon) noexcept -> GMultiPolygon & = default;

    /*!
     * \brief Returns a const reference to the underlying multi-polygon geometry.
     * \return Const reference to `MultiPolygon<Point2d>`.
     */
    auto geometry() const -> const MultiPolygon<Point2d> & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying multi-polygon geometry.
     * \return Reference to `MultiPolygon<Point2d>`.
     */
    auto geometry() -> MultiPolygon<Point2d> & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the multi-polygon.
     * \return A `BoundingBox<Point2d>` enclosing the multi-polygon.
     */
    auto window() const -> BoundingBox<Point2d> override;

    /*!
     * \brief Renders the multi-polygon using the given painter.
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
