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

/*! \file GMultiPolygon3D.h
 * \brief 3D multi-polygon graphic entity for rendering.
 *
 * This file defines the `GMultiPolygon3D` class, which represents a collection
 * of 3D polygons as a graphic entity that can be rendered using a `Painter`.
 * It inherits from `GraphicEntity` and stores a `MultiPolygon<Point3d>` geometry.
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::MultiPolygon, tl::Point3d
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
 * \class GMultiPolygon3D
 * \brief 3D multi-polygon graphic entity.
 *
 * This class wraps a 3D multi-polygon geometry (`MultiPolygon<Point3d>`) and
 * provides the necessary interface for rendering via a `Painter`. It also
 * implements the `clone()` method and computes the 2D bounding window of the
 * projected multi-polygon.
 *
 * ### Example
 * \code
 * MultiPolygon<Point3d> mp = ...;
 * GMultiPolygon3D gmp(mp);
 * gmp.setPen(Pen(Color::Blue, 2.0));
 * gmp.setBrush(Brush(Color::Red, Brush::Style::Solid));
 * painter.draw(gmp);
 * \endcode
 */
class TL_EXPORT GMultiPolygon3D
  : public GraphicEntity
{

private:

    MultiPolygon<Point3d> mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs an empty 3D multi-polygon graphic entity.
     */
    GMultiPolygon3D();

    /*!
     * \brief Constructs from a 3D multi-polygon.
     * \param[in] multiPolygon The 3D multi-polygon geometry.
     */
    explicit GMultiPolygon3D(const MultiPolygon<Point3d> &multiPolygon);

    /*!
     * \brief Constructs with a pre-allocated number of polygons.
     * \param[in] size Initial number of polygons in the collection.
     */
    explicit GMultiPolygon3D(size_t size);

    /*!
     * \brief Copy constructor.
     * \param[in] multiPolygon3D Object to copy.
     */
    GMultiPolygon3D(const GMultiPolygon3D &multiPolygon3D) = default;

    /*!
     * \brief Move constructor.
     * \param[in] multiPolygon3D Object to move.
     */
    GMultiPolygon3D(GMultiPolygon3D &&multiPolygon3D) noexcept = default;

    ~GMultiPolygon3D() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] multiPolygon3D Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GMultiPolygon3D &multiPolygon3D) -> GMultiPolygon3D & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] multiPolygon3D Object to move.
     * \return Reference to this object.
     */
    auto operator =(GMultiPolygon3D &&multiPolygon3D) noexcept -> GMultiPolygon3D & = default;

    /*!
     * \brief Returns a const reference to the underlying multi-polygon geometry.
     * \return Const reference to `MultiPolygon<Point3d>`.
     */
    auto geometry() const -> const MultiPolygon<Point3d> & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying multi-polygon geometry.
     * \return Reference to `MultiPolygon<Point3d>`.
     */
    auto geometry() -> MultiPolygon<Point3d> & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the projected multi-polygon.
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
