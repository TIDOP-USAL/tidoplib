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
     * \brief Default constructor
     */
    GLineString3D() = default;

    explicit GLineString3D(size_t size);

    /*!
     * \brief Constructor from a 3D polyline
     * Represents a 3D polyline without style
     * \param[in] gLineString3D LineString3D class object
     * \see LineString3D
     */
    explicit GLineString3D(const LineString<Point3d> &gLineString3D);

    /*!
     * \brief Copy constructor
     */
    GLineString3D(const GLineString3D &gLineString3D) = default;

    /*!
     * \brief Move constructor
     */
    GLineString3D(GLineString3D &&gLineString3D) noexcept = default;

    ~GLineString3D() override = default;

    /*!
     * \brief Assignment copy operator
     * \param[in] gLineString3D GLineString3D object to be copied
     * \return Object reference
     */
    auto operator =(const GLineString3D& gLineString3D) -> GLineString3D& = default;

    /*!
     * \brief Assignment move operator
     * \param[in] gLineString3D GLineString3D object that moves
     * \return Object reference
     */
    auto operator =(GLineString3D&& gLineString3D) noexcept -> GLineString3D& = default;

    auto geometry() const -> const LineString<Point3d> & { return mGeometry; }
    auto geometry() -> LineString<Point3d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;

    void draw(Painter &painter) const override;
    auto window() const -> BoundingBox<Point2d> override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};



inline auto GLineString3D::isMultiEntity() const -> bool
{
    return false;
}

inline auto GLineString3D::isSimpleEntity() const -> bool
{
    return true;
}


} // namespace tl
