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
 * \brief 3D multi-polyline graphic class
 */
class TL_EXPORT GMultiLineString3D
  : public GraphicEntity
{
private:

    MultiLineString<Point3d> mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GMultiLineString3D() = default;

    explicit GMultiLineString3D(size_t size);

    explicit GMultiLineString3D(const MultiLineString<Point3d> &gMultiLineString3D);

    /*!
     * \brief Copy constructor
     */
    GMultiLineString3D(const GMultiLineString3D &gMultiLineString3D) = default;

    /*!
     * \brief Move constructor
     */
    GMultiLineString3D(GMultiLineString3D &&gMultiLineString3D) noexcept = default;

    ~GMultiLineString3D() override = default;

    /*!
     * \brief Assignment copy operator
     */
    auto operator =(const GMultiLineString3D &gMultiLineString3D) -> GMultiLineString3D& = default;

    /*!
     * \brief Assignment move operator
     */
    auto operator =(GMultiLineString3D &&gMultiLineString3D) noexcept -> GMultiLineString3D& = default;

    auto geometry() const -> const MultiLineString<Point3d> & { return mGeometry; }
    auto geometry() -> MultiLineString<Point3d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


inline auto GMultiLineString3D::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiLineString3D::isSimpleEntity() const -> bool
{
    return false;
}


} // namespace tl
