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

/*! \file GMultiLineString.h
 * \brief 2D multi-line string graphic entity for rendering.
 *
 * This file defines the `GMultiLineString` class, which represents a collection
 * of 2D polylines (MultiLineString) as a graphic entity that can be rendered
 * using a `Painter`. It inherits from `GraphicEntity` and stores a
 * `MultiLineString<Point2d>` geometry.
 *
 * \ingroup GraphicEntities
 * \see tl::GraphicEntity, tl::MultiLineString, tl::Point2d
 */

#pragma once

#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \class GMultiLineString
 * \brief 2D multi-line string graphic entity.
 *
 * This class wraps a 2D multi-line string geometry (`MultiLineString<Point2d>`)
 * and provides the necessary interface for rendering via a `Painter`. It also
 * implements the `clone()` method and computes the bounding window of the
 * multi-line string collection.
 *
 * ### Example
 * \code
 * MultiLineString<Point2d> mls = ...;
 * GMultiLineString gmls(mls);
 * gmls.setPen(Pen(Color::Blue, 1.5));
 * painter.draw(gmls);
 * \endcode
 */
class TL_EXPORT GMultiLineString
  : public GraphicEntity
{

private:

    MultiLineString<Point2d> mGeometry;

public:

    /*!
     * \brief Default constructor.
     * Constructs an empty 2D multi-line string graphic entity.
     */
    GMultiLineString();

    /*!
     * \brief Constructs with a pre-allocated number of polylines.
     * \param[in] size Initial number of polylines in the collection.
     */
    explicit GMultiLineString(size_t size);

    /*!
     * \brief Constructs from a 2D multi-line string.
     * \param[in] multiLineString The 2D multi-line string geometry.
     */
    explicit GMultiLineString(const MultiLineString<Point<double>> &multiLineString);

    /*!
     * \brief Copy constructor.
     * \param[in] gMultiLineString Object to copy.
     */
    GMultiLineString(const GMultiLineString &gMultiLineString) = default;

    /*!
     * \brief Move constructor.
     * \param[in] gMultiLineString Object to move.
     */
    GMultiLineString(GMultiLineString &&gMultiLineString) noexcept = default;

    /*! \brief Destructor. */
    ~GMultiLineString() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gMultiLineString Object to copy.
     * \return Reference to this object.
     */
    auto operator =(const GMultiLineString &gMultiLineString) -> GMultiLineString & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] gMultiLineString Object to move.
     * \return Reference to this object.
     */
    auto operator =(GMultiLineString &&gMultiLineString) noexcept -> GMultiLineString & = default;

    /*!
     * \brief Returns a const reference to the underlying multi-line string geometry.
     * \return Const reference to `MultiLineString<Point2d>`.
     */
    auto geometry() const -> const MultiLineString<Point2d> & { return mGeometry; }

    /*!
     * \brief Returns a mutable reference to the underlying multi-line string geometry.
     * \return Reference to `MultiLineString<Point2d>`.
     */
    auto geometry() -> MultiLineString<Point2d> & { return mGeometry; }

    /*!
     * \brief Computes the 2D bounding window of the multi-line string collection.
     * \return A `BoundingBox<Point2d>` enclosing the multi-line string.
     */
    auto window() const -> BoundingBox<Point2d> override;

    /*!
     * \brief Renders the multi-line string collection using the given painter.
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
