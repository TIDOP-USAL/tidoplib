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

/*! \file GLineString.h
 * \brief 2D polyline (LineString) graphic entity for rendering.
 *
 * This file defines the `GLineString` class, which represents a 2D polyline
 * as a graphic entity that can be rendered using a `Painter`. It inherits from
 * `GraphicEntity` and stores a `LineString<Point<double>>` geometry.
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::LineString, tl::Point2d
 */

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
 * \class GLineString
 * \brief 2D polyline (LineString) graphic entity.
 *
 * This class wraps a 2D polyline geometry (`LineString<Point<double>>`) and
 * provides the necessary interface for rendering via a `Painter`. It also
 * implements the `clone()` method and computes the bounding window of the
 * polyline.
 *
 * ### Example
 * \code
 * LineString<Point2d> line = {{0, 0}, {10, 10}, {20, 0}};
 * GLineString gline(line);
 * gline.setPen(Pen(Color::Blue, 2.0));
 * painter.draw(gline);
 * \endcode
 */
class TL_EXPORT GLineString
  : public GraphicEntity
{

private:

    LineString<Point<double>> mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs an empty 2D polyline graphic entity.
     */
    GLineString();

    /*!
     * \brief Constructs with a pre-allocated number of vertices.
     * \param[in] size Initial number of points in the polyline.
     */
    explicit GLineString(size_t size);

    /*!
     * \brief Constructs from a 2D polyline.
     * \param[in] lineString The 2D polyline geometry.
     */
    explicit GLineString(const LineString<Point<double>> &lineString);

    /*!
     * \brief Copy constructor.
     * \param[in] lineString Object to copy.
     */
    GLineString(const GLineString &lineString) = default;

    /*!
     * \brief Move constructor.
     * \param[in] lineString Object to move.
     */
    GLineString(GLineString &&lineString) noexcept = default;

    ~GLineString() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gLineString Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GLineString &gLineString) -> GLineString & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] gLineString Object to move.
     * \return Reference to this object.
     */
    auto operator =(GLineString &&gLineString) noexcept -> GLineString & = default;

    /*!
     * \brief Returns a const reference to the underlying polyline geometry.
     * \return Const reference to `LineString<Point<double>>`.
     */
    auto geometry() const -> const LineString<Point<double>> & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying polyline geometry.
     * \return Reference to `LineString<Point<double>>`.
     */
    auto geometry() -> LineString<Point<double>> & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the polyline.
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
