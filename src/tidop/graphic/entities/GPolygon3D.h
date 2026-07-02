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
 * \brief 3D polygon graphics class
 */
class TL_EXPORT GPolygon3D
  : public GraphicEntity
{

private:

    Polygon<Point3d> mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GPolygon3D() = default;

    explicit GPolygon3D(size_t size);

    /*!
     * \brief Constructor from a 3D polygon
     * \param[in] polygon Polygon object
     * \see Polygon3D
     */
    explicit GPolygon3D(const Polygon<Point3d> &polygon);

    /*!
     * \brief Copy constructor
     * \param[in] gPolygon3D Object to be copied
     */
    GPolygon3D(const GPolygon3D &gPolygon3D) = default;

    /*!
     * \brief Move Constructor
     * \param[in] gPolygon3D GPolygon object that moves
     */
    GPolygon3D(GPolygon3D &&gPolygon3D) noexcept = default;

    ~GPolygon3D() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] gPolygon3D Object to be copied
     * \return Object reference
     */
    auto operator =(const GPolygon3D& gPolygon3D) -> GPolygon3D& = default;

    /*!
     * \brief Move assignment operator
     * \param[in] gPolygon3D GPolygon object that moves
     * \return Object reference
     */
    auto operator =(GPolygon3D&& gPolygon3D) noexcept -> GPolygon3D& = default;

    auto geometry() const -> const Polygon<Point3d> & { return mGeometry; }
    auto geometry() -> Polygon<Point3d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


inline auto GPolygon3D::isMultiEntity() const -> bool
{
    return false;
}

inline auto GPolygon3D::isSimpleEntity() const -> bool
{
    return true;
}


/*! \} */ // Fin GraphicEntities

} // namespace tl
