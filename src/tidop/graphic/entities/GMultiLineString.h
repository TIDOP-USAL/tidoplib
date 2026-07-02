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
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \brief Multi-polyline graphic class
 */
class TL_EXPORT GMultiLineString
  : public GraphicEntity
{

private:

    MultiLineString<Point2d> mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GMultiLineString() = default;

    explicit GMultiLineString(size_t size);

    /*!
     * \brief Constructor from a MultiLineString
     * \see MultiLineString
     */
    explicit GMultiLineString(const MultiLineString<Point<double>> &multiLineString);

    /*!
     * \brief Copy constructor
     */
    GMultiLineString(const GMultiLineString &gMultiLineString) = default;

    /*!
     * \brief Move constructor
     */
    GMultiLineString(GMultiLineString &&gMultiLineString) noexcept = default;

    ~GMultiLineString() override = default;

    /*!
     * \brief Assignment copy operator
     */
    auto operator =(const GMultiLineString &gMultiLineString) -> GMultiLineString& = default;

    /*!
     * \brief Assignment move operator
     */
    auto operator =(GMultiLineString &&gMultiLineString) noexcept -> GMultiLineString& = default;

    auto geometry() const -> const MultiLineString<Point2d> & { return mGeometry; }
    auto geometry() -> MultiLineString<Point2d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


inline auto GMultiLineString::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiLineString::isSimpleEntity() const -> bool
{
    return false;
}


} // namespace tl
