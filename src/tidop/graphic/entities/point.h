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
#include "tidop/geometry/entities/multipoint.h"
#include "tidop/graphic/entities/entity.h"

namespace tl
{

/*! \addtogroup GraphicEntities
 *  \{
 */

 /*!
  * \brief Point graphic class
  */
class TL_EXPORT GPoint
  : public Point<double>,
    public GraphicEntity
{

public:

    /*!
     * \brief Default constructor
     */
    GPoint();

    /*!
     * \brief Constructor
     * \param[in] x x coordinate
     * \param[in] y y coordinate
     */
    GPoint(double x, double y);

    /*!
     * \brief Constructor from a Point
     * \param[in] pt Point
     * \see Point
     */
    explicit GPoint(const Point<double> &pt);

    /*!
     * \brief Copy constructor
     * \param[in] pt Objeto GPoint to be copied
     */
    GPoint(const GPoint &pt);

    /*!
     * \brief Move Constructor
     * \param[in] pt Objeto GPoint object that moves
     */
    GPoint(GPoint &&pt) TL_NOEXCEPT;

    ~GPoint() override;

    /*!
     * \brief Copy assignment operator
     * \param gPoint GPoint object to be assigned
     * \return Object reference
     */
    auto operator =(const GPoint &gPoint) -> GPoint&;

    /*!
     * \brief Move assignment operator
     * \param gPoint GPoint object that moves
     * \return Object reference
     */
    auto operator =(GPoint&& gPoint) TL_NOEXCEPT -> GPoint&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
};

/*!
 * \brief 3D Point graphics class
 */
class TL_EXPORT GPoint3D
  : public Point3<double>,
    public GraphicEntity
{

public:

    /*!
     * \brief Default constructor
     */
    GPoint3D();

    /*!
     * \brief Constructor
     * \param[in] x Coordinate x
     * \param[in] y Coordinate y
     * \param[in] z Coordinate z
     */
    GPoint3D(double x, double y, double z);

    /*!
     * \brief Constructor from a Point
     * \param[in] pt Point
     * \see Point3
     */
    explicit GPoint3D(const Point3<double> &pt);

    /*!
     * \brief Copy constructor
     * \param[in] pt GPoint3D object to be copied
     */
    GPoint3D(const GPoint3D &pt);

    /*!
     * \brief Move Constructor
     * \param[in] pt Objeto GPoint3D object that moves
     */
    GPoint3D(GPoint3D &&pt) TL_NOEXCEPT;


    ~GPoint3D() override;

    /*!
     * \brief Copy assignment operator
     * \param gPoint GPoint3D object to be copied
     * \return Object reference
     */
    auto operator =(const GPoint3D &gPoint) -> GPoint3D&;

    /*!
     * \brief Move assignment operator
     * \param[in] gPoint GPoint3D object that moves
     * \return Object reference
     */
    auto operator =(GPoint3D &&gPoint) TL_NOEXCEPT -> GPoint3D&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
};


/*!
 * \brief Multi-point graphic class
 */
class TL_EXPORT GMultiPoint
  : public MultiPoint<Point<double>>,
    public GraphicEntity
{

public:

    GMultiPoint();
    explicit GMultiPoint(size_t size);
    explicit GMultiPoint(const MultiPoint<Point<double>> &multiPoint);
    GMultiPoint(const GMultiPoint &gMultiPoint);
    GMultiPoint(GMultiPoint &&gMultiPoint) TL_NOEXCEPT;
    ~GMultiPoint() override;

    auto operator =(const GMultiPoint& gMultiPoint) -> GMultiPoint&;
    auto operator =(GMultiPoint &&gMultiPoint) TL_NOEXCEPT -> GMultiPoint&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
};




/*!
 * \brief Multi-point 3D graphic class
 */
class TL_EXPORT GMultiPoint3D
  : public MultiPoint3D<Point3<double>>,
    public GraphicEntity
{

public:

    GMultiPoint3D();
    explicit GMultiPoint3D(size_t size);
    explicit GMultiPoint3D(const MultiPoint3D<Point3<double>> &multiPoint);
    GMultiPoint3D(const GMultiPoint3D &gMultiPoint3D);
    GMultiPoint3D(GMultiPoint3D &&gMultiPoint3D) TL_NOEXCEPT;
    ~GMultiPoint3D() override;

    auto operator =(const GMultiPoint3D &gMultiPoint3D) -> GMultiPoint3D&;
    auto operator =(GMultiPoint3D &&gMultiPoint3D) TL_NOEXCEPT -> GMultiPoint3D&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
};




inline auto GPoint::isMultiEntity() const -> bool
{
    return false;
}

inline auto GPoint::isSimpleEntity() const -> bool
{
    return true;
}



inline auto GPoint3D::isMultiEntity() const -> bool
{
    return false;
}

inline auto GPoint3D::isSimpleEntity() const -> bool
{
    return true;
}



inline auto GMultiPoint::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiPoint::isSimpleEntity() const -> bool
{
    return false;
}




inline auto GMultiPoint3D::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiPoint3D::isSimpleEntity() const -> bool
{
    return false;
}


/*! \} */ // Fin GraphicEntities

} // Fin namespace tl

