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

/*! \file GPoint3D.h
 * \brief 3D point graphic entity for rendering.
 *
 * This file defines the `GPoint3D` class, which represents a 3D point
 * as a graphic entity that can be rendered using a `Painter`. It inherits from
 * `GraphicEntity` and stores a 3D point geometry (`Point3d`).
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::Point3d
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
 * \class GPoint3D
 * \brief 3D point graphic entity.
 *
 * This class wraps a 3D point geometry (`Point3d`) and provides the necessary
 * interface for rendering via a `Painter`. It also implements the `clone()`
 * method and computes the 2D bounding window of the point (projected to 2D).
 *
 * ### Example
 * \code
 * GPoint3D gpoint(1.0, 2.0, 3.0);
 * gpoint.setSymbol(Symbol(Symbol::Type::Circle, 10.0));
 * painter.draw(gpoint);
 * \endcode
 */
class TL_EXPORT GPoint3D
  : public GraphicEntity
{

private:

    Point3d mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs a 3D point at the origin `(0, 0, 0)`.
     */
    GPoint3D();

    /*!
     * \brief Constructs a 3D point from coordinates.
     * \param[in] x X-coordinate.
     * \param[in] y Y-coordinate.
     * \param[in] z Z-coordinate.
     */
    GPoint3D(double x, double y, double z);

    /*!
     * \brief Constructs a 3D point from a `Point3d` object.
     * \param[in] pt The 3D point.
     */
    explicit GPoint3D(const Point3<double> &pt);

    /*!
     * \brief Copy constructor.
     * \param[in] pt Object to copy.
     */
    GPoint3D(const GPoint3D &pt) = default;

    /*!
     * \brief Move constructor.
     * \param[in] pt Object to move.
     */
    GPoint3D(GPoint3D &&pt) noexcept = default;


    ~GPoint3D() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gPoint Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GPoint3D &gPoint) -> GPoint3D & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] gPoint Object to move.
     * \return Reference to this object.
     */
    auto operator =(GPoint3D &&gPoint) noexcept -> GPoint3D & = default;

    /*!
     * \brief Returns a const reference to the underlying point geometry.
     * \return Const reference to `Point3d`.
     */
    auto geometry() const -> const Point3d & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying point geometry.
     * \return Reference to `Point3d`.
     */
    auto geometry() -> Point3d & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the point (projected to 2D).
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

