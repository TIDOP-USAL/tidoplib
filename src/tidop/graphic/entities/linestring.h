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

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/linestring.h"
#include "tidop/geometry/entities/multilinestring.h"
#include "tidop/graphic/entities/entity.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */


/*!
 * \brief 2D polyline graphic class
 */
class TL_EXPORT GLineString
  : public LineString<Point<double>>,
    public GraphicEntity
{

public:

    /*!
     * \brief Default constructor
     */
    GLineString();

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
    GLineString(const GLineString &lineString);

    /*!
     * \brief Move constructor
     */
    GLineString(GLineString &&lineString) TL_NOEXCEPT;


    ~GLineString() override;

    /*!
     * \brief Assignment copy operator
     * \param[in] gLineString GLineString object to be copied
     * \return Object reference
     */
    auto operator =(const GLineString &gLineString) -> GLineString&;

    /*!
     * \brief Assignment move operator
     * \param[in] gLineString GLineString object that moves
     * \return Object reference
     */
    auto operator =(GLineString &&gLineString) TL_NOEXCEPT -> GLineString&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;

};





/*!
 * \brief 3D polyline graphic class
 */
class TL_EXPORT GLineString3D
  : public LineString3D<Point3<double>>,
    public GraphicEntity
{
public:

    /*!
     * \brief Default constructor
     */
    GLineString3D();

    explicit GLineString3D(size_t size);

    /*!
     * \brief Constructor from a 3D polyline
     * Represents a 3D polyline without style
     * \param[in] gLineString3D LineString3D class object
     * \see LineString3D
     */
    explicit GLineString3D(const LineString3D<Point3<double>> &gLineString3D);

    /*!
     * \brief Copy constructor
     */
    GLineString3D(const GLineString3D &gLineString3D);

    /*!
     * \brief Move constructor
     */
    GLineString3D(GLineString3D &&gLineString3D) TL_NOEXCEPT;

    ~GLineString3D() override;

    /*!
     * \brief Assignment copy operator
     * \param[in] gLineString3D GLineString3D object to be copied
     * \return Object reference
     */
    auto operator =(const GLineString3D& gLineString3D) -> GLineString3D&;

    /*!
     * \brief Assignment move operator
     * \param[in] gLineString3D GLineString3D object that moves
     * \return Object reference
     */
    auto operator =(GLineString3D&& gLineString3D) TL_NOEXCEPT -> GLineString3D&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
};




/*!
 * \brief Multi-polyline graphic class
 */
class TL_EXPORT GMultiLineString
  : public MultiLineString<Point<double>>,
    public GraphicEntity
{
public:

    /*!
     * \brief Default constructor
     */
    GMultiLineString();

    explicit GMultiLineString(size_t size);

    /*!
     * \brief Constructor from a MultiLineString
     * \see MultiLineString
     */
    explicit GMultiLineString(const MultiLineString<Point<double>> &multiLineString);

    /*!
     * \brief Copy constructor
     */
    GMultiLineString(const GMultiLineString &gMultiLineString);

    /*!
     * \brief Move constructor
     */
    GMultiLineString(GMultiLineString &&gMultiLineString) TL_NOEXCEPT;

    ~GMultiLineString() override;

    /*!
     * \brief Assignment copy operator
     */
    auto operator =(const GMultiLineString &gMultiLineString) -> GMultiLineString&;

    /*!
     * \brief Assignment move operator
     */
    auto operator =(GMultiLineString &&gMultiLineString) TL_NOEXCEPT -> GMultiLineString&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
};



/*!
 * \brief 3D multi-polyline graphic class
 */
class TL_EXPORT GMultiLineString3D
  : public MultiLineString3D<Point3<double>>,
    public GraphicEntity
{
public:

    /*!
     * \brief Default constructor
     */
    GMultiLineString3D();

    explicit GMultiLineString3D(size_t size);

    explicit GMultiLineString3D(const MultiLineString3D<Point3<double>> &gMultiLineString3D);

    /*!
     * \brief Copy constructor
     */
    GMultiLineString3D(const GMultiLineString3D &gMultiLineString3D);

    /*!
     * \brief Move constructor
     */
    GMultiLineString3D(GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT;

    ~GMultiLineString3D() override;

    /*!
     * \brief Assignment copy operator
     */
    auto operator =(const GMultiLineString3D &gMultiLineString3D) -> GMultiLineString3D&;

    /*!
     * \brief Assignment move operator
     */
    auto operator =(GMultiLineString3D &&gMultiLineString3D) TL_NOEXCEPT -> GMultiLineString3D&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;

};






inline bool GLineString::isMultiEntity() const
{
    return false;
}

inline bool GLineString::isSimpleEntity() const
{
    return true;
}



inline auto GLineString3D::isMultiEntity() const -> bool
{
    return false;
}

inline auto GLineString3D::isSimpleEntity() const -> bool
{
    return true;
}




inline auto GMultiLineString::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiLineString::isSimpleEntity() const -> bool
{
    return false;
}




inline auto GMultiLineString3D::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiLineString3D::isSimpleEntity() const -> bool
{
    return false;
}




/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
