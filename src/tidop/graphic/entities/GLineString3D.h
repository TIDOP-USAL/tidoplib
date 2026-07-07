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
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \brief 3D polyline graphic class
 */
class TL_EXPORT GLineString3D
  : public GraphicEntity
{

private:

    LineString<Point3d> mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs an empty 3D polyline graphic entity.
     */
    GLineString3D();

    /*!
     * \brief Constructs with a pre-allocated number of vertices.
     * \param[in] size Initial number of points in the polyline.
     */
    explicit GLineString3D(size_t size);

    /*!
     * \brief Constructs from a 3D polyline.
     * \param[in] lineString The 3D polyline geometry.
     */
    explicit GLineString3D(const LineString<Point3d> &lineString);

    /*!
     * \brief Copy constructor.
     * \param[in] gLineString3D Object to copy.
     */
    GLineString3D(const GLineString3D &gLineString3D) = default;

    /*!
     * \brief Move constructor.
     * \param[in] gLineString3D Object to move.
     */
    GLineString3D(GLineString3D &&gLineString3D) noexcept = default;

    /*! \brief Destructor. */
    ~GLineString3D() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gLineString3D Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GLineString3D &gLineString3D) -> GLineString3D & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] gLineString3D Object to move.
     * \return Reference to this object.
     */
    auto operator =(GLineString3D &&gLineString3D) noexcept -> GLineString3D & = default;

    /*!
     * \brief Returns a const reference to the underlying polyline geometry.
     * \return Const reference to `LineString<Point3d>`.
     */
    auto geometry() const -> const LineString<Point3d> & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying polyline geometry.
     * \return Reference to `LineString<Point3d>`.
     */
    auto geometry() -> LineString<Point3d> & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the projected polyline.
     * \return A `BoundingBox<Point2d>` enclosing the polyline.
     */
    auto window() const -> BoundingBox<Point2d> override;

    /*!
     * \brief Renders the polyline using the given painter.
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
