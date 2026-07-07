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

/*! \file GPolygon3D.h
 * \brief 3D polygon graphic entity for rendering.
 *
 * This file defines the `GPolygon3D` class, which represents a 3D polygon
 * as a graphic entity that can be rendered using a `Painter`. It inherits from
 * `GraphicEntity` and stores a 3D polygon geometry (`Polygon<Point3d>`).
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::Polygon, tl::Point3d
 */

#pragma once

#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/Multipolygon.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \class GPolygon3D
 * \brief 3D polygon graphic entity.
 *
 * This class wraps a 3D polygon geometry (`Polygon<Point3d>`) and provides
 * the necessary interface for rendering via a `Painter`. It also implements
 * the `clone()` method and computes the 2D window (bounding box) of the
 * projected polygon.
 *
 * ### Example
 * \code
 * Polygon<Point3d> poly = ...;
 * GPolygon3D gpoly(poly);
 * gpoly.setPen(Pen(Color::Blue, 2.0));
 * gpoly.setBrush(Brush(Color::Red));
 * painter.draw(gpoly);
 * \endcode
 */
class TL_EXPORT GPolygon3D
  : public GraphicEntity
{

private:

    Polygon<Point3d> mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs an empty 3D polygon graphic entity.
     */
    GPolygon3D();

    /*!
     * \brief Constructs a polygon with a pre-allocated number of vertices.
     * \param[in] size Initial number of vertices (outer ring size).
     */
    explicit GPolygon3D(size_t size);

    /*!
     * \brief Constructs from a 3D polygon.
     * \param[in] polygon The 3D polygon geometry.
     */
    explicit GPolygon3D(const Polygon<Point3d> &polygon);

    /*!
     * \brief Copy constructor
     * \param[in] gPolygon3D Object to be copied
     */
    GPolygon3D(const GPolygon3D &gPolygon3D) = default;

    ~GPolygon3D() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gPolygon3D Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GPolygon3D &gPolygon3D) -> GPolygon3D & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] gPolygon3D Object to move.
     * \return Reference to this object.
     */
    auto operator =(GPolygon3D &&gPolygon3D) noexcept -> GPolygon3D & = default;

    /*!
     * \brief Returns a const reference to the underlying polygon geometry.
     * \return Const reference to `Polygon<Point3d>`.
     */
    auto geometry() const -> const Polygon<Point3d> & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying polygon geometry.
     * \return Reference to `Polygon<Point3d>`.
     */
    auto geometry() -> Polygon<Point3d> & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding box (window) of the projected polygon.
     * \return A `BoundingBox<Point2d>` representing the window extent.
     */
    auto window() const -> BoundingBox<Point2d> override;

    /*!
     * \brief Renders the polygon using the given painter.
     * \param[in] painter The painter used for rendering.
     */
    void draw(Painter &painter) const override;

    /*!
     * \brief Creates a deep copy of this graphic entity.
     * \return A unique pointer to the cloned object.
     */
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


/*! \} */ // GraphicEntities

} // namespace tl
