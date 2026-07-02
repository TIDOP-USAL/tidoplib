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
#include "tidop/graphic/entities/GraphicEntity.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \brief Polygon graphic class
 */
class TL_EXPORT GPolygon
  : public GraphicEntity
{

private:

    Polygon<Point2d> mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GPolygon() = default;

    explicit GPolygon(size_t size);

    /*!
     * \brief Constructor from a polygon
     * \param[in] polygon Polygon class object
     * \see Polygon
     */
    explicit GPolygon(const Polygon<Point2d> &polygon);

    /*!
     * \brief Copy constructor
     * \param[in] gPolygon Object to be copied
     */
    GPolygon(const GPolygon &gPolygon) = default;

    /*!
     * \brief Move Constructor
     * \param[in] gPolygon GPolygon object that moves
     */
    GPolygon(GPolygon &&gPolygon) noexcept = default;

    ~GPolygon() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] gPolygon Object to be copied
     * \return Object reference
     */
    auto operator =(const GPolygon& gPolygon) -> GPolygon& = default;

    /*!
     * \brief Move assignment operator
     * \param[in] gPolygon GPolygon object that moves
     * \return Object reference
     */
    auto operator =(GPolygon&& gPolygon) noexcept -> GPolygon& = default;

    auto geometry() const -> const Polygon<Point2d> & { return mGeometry; }
    auto geometry() -> Polygon<Point2d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


inline auto GPolygon::isMultiEntity() const -> bool
{
    return false;
}

inline auto GPolygon::isSimpleEntity() const -> bool
{
    return true;
}


/*! \} */ // Fin GraphicEntities

} // namespace tl
