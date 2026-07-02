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
 * \brief 2D polyline graphic class
 */
class TL_EXPORT GLineString
  : public GraphicEntity
{

private:

    LineString<Point<double>> mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GLineString() = default;

    explicit GLineString(size_t size);

    /*!
     * \brief Constructor from a polyline
     * Represents a polyline without style
     * \param[in] lineString LineString class object
     * \see LineString
     */
    explicit GLineString(const LineString<Point<double>> &lineString);

    /*!
     * \brief Copy constructor
     */
    GLineString(const GLineString &lineString) = default;

    /*!
     * \brief Move constructor
     */
    GLineString(GLineString &&lineString) noexcept = default;


    ~GLineString() override = default;

    /*!
     * \brief Assignment copy operator
     * \param[in] gLineString GLineString object to be copied
     * \return Object reference
     */
    auto operator =(const GLineString &gLineString) -> GLineString& = default;

    /*!
     * \brief Assignment move operator
     * \param[in] gLineString GLineString object that moves
     * \return Object reference
     */
    auto operator =(GLineString &&gLineString) noexcept -> GLineString& = default;

    auto geometry() const -> const LineString<Point<double>> & { return mGeometry; }
    auto geometry() -> LineString<Point<double>> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const ->BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};



inline bool GLineString::isMultiEntity() const
{
    return false;
}

inline bool GLineString::isSimpleEntity() const
{
    return true;
}


} // namespace tl
