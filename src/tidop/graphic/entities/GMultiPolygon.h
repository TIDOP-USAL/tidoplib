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
#include "tidop/geometry/primitives/Multipolygon.h"
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \brief Multi-polygon graphic class
 */
class TL_EXPORT GMultiPolygon
  : public GraphicEntity
{

private:

    MultiPolygon<Point2d> mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GMultiPolygon() = default;

    explicit GMultiPolygon(size_t size);

    /*!
     * \brief Constructor from a multi-polygon
     * \param multiPolygon MultiPolygon object
     * \see MultiPolygon
     */
    explicit GMultiPolygon(const MultiPolygon<Point2d> &multiPolygon);

    /*!
     * \brief Copy constructor
     * \param[in] multiPolygon Object to be copied
     */
    GMultiPolygon(const GMultiPolygon &multiPolygon) = default;

    /*!
     * \brief Move Constructor
     * \param[in] multiPolygon GMultiPolygon object that moves
     */
    GMultiPolygon(GMultiPolygon &&multiPolygon) noexcept = default;

    ~GMultiPolygon() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] multiPolygon Object to be copied
     * \return Object reference
     */
    auto operator =(const GMultiPolygon &multiPolygon) -> GMultiPolygon& = default;

    /*!
     * \brief Move assignment operator
     * \param[in] multiPolygon GPolygon object that moves
     * \return Object reference
     */
    auto operator =(GMultiPolygon &&multiPolygon) noexcept -> GMultiPolygon& = default;

    auto geometry() const -> const MultiPolygon<Point2d> & { return mGeometry; }
    auto geometry() -> MultiPolygon<Point2d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};



inline auto GMultiPolygon::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiPolygon::isSimpleEntity() const -> bool
{
    return false;
}


/*! \} */ // Fin GraphicEntities

} // namespace tl
