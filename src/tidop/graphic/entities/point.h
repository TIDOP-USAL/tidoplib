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
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/graphic/entities/entity.h"

namespace tl
{

class Painter;

/*! \addtogroup GraphicEntities
 *  \{
 */

/*!
 * \brief Point graphic class
 */
class TL_EXPORT GPoint
  : public GraphicEntity
{

private:

    Point2d mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GPoint() = default;

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
    GPoint(const GPoint &pt) = default;

    /*!
     * \brief Move Constructor
     * \param[in] pt Objeto GPoint object that moves
     */
    GPoint(GPoint &&pt) noexcept = default;

    ~GPoint() override;

    /*!
     * \brief Copy assignment operator
     * \param gPoint GPoint object to be assigned
     * \return Object reference
     */
    auto operator =(const GPoint &gPoint) -> GPoint& = default;

    /*!
     * \brief Move assignment operator
     * \param gPoint GPoint object that moves
     * \return Object reference
     */
    auto operator =(GPoint&& gPoint) noexcept -> GPoint& = default;

    auto geometry() const -> const Point2d& { return mGeometry; }
    auto geometry() -> Point2d& { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};

/*!
 * \brief 3D Point graphics class
 */
class TL_EXPORT GPoint3D
  : public GraphicEntity
{

private:

    Point3d mGeometry;

public:

    /*!
     * \brief Default constructor
     */
    GPoint3D() = default;

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
    GPoint3D(const GPoint3D &pt) = default;

    /*!
     * \brief Move Constructor
     * \param[in] pt Objeto GPoint3D object that moves
     */
    GPoint3D(GPoint3D &&pt) noexcept = default;


    ~GPoint3D() override;

    /*!
     * \brief Copy assignment operator
     * \param gPoint GPoint3D object to be copied
     * \return Object reference
     */
    auto operator =(const GPoint3D &gPoint) -> GPoint3D& = default;

    /*!
     * \brief Move assignment operator
     * \param[in] gPoint GPoint3D object that moves
     * \return Object reference
     */
    auto operator =(GPoint3D &&gPoint) noexcept -> GPoint3D& = default;

    auto geometry() const -> const Point3d& { return mGeometry; }
    auto geometry() -> Point3d& { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};


/*!
 * \brief Multi-point graphic class
 */
class TL_EXPORT GMultiPoint
  : public GraphicEntity
{
private:

    MultiPoint<Point<double>> mGeometry;

public:

    GMultiPoint() = default;
    explicit GMultiPoint(size_t size);
    explicit GMultiPoint(const MultiPoint<Point<double>> &multiPoint);
    GMultiPoint(const GMultiPoint &gMultiPoint) = default;
    GMultiPoint(GMultiPoint &&gMultiPoint) noexcept = default;
    ~GMultiPoint() override;

    auto operator =(const GMultiPoint& gMultiPoint) -> GMultiPoint& = default;
    auto operator =(GMultiPoint &&gMultiPoint) noexcept -> GMultiPoint& = default;

    auto geometry() const -> const MultiPoint<Point<double>> & { return mGeometry; }
    auto geometry() -> MultiPoint<Point<double>> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const ->BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
};




/*!
 * \brief Multi-point 3D graphic class
 */
class TL_EXPORT GMultiPoint3D
  : public GraphicEntity
{

private:

    MultiPoint<Point3d> mGeometry;

public:

    GMultiPoint3D() = default;
    explicit GMultiPoint3D(size_t size);
    explicit GMultiPoint3D(const MultiPoint<Point3d> &multiPoint);
    GMultiPoint3D(const GMultiPoint3D &gMultiPoint3D) = default;
    GMultiPoint3D(GMultiPoint3D &&gMultiPoint3D) noexcept = default;
    ~GMultiPoint3D() override;

    auto operator =(const GMultiPoint3D &gMultiPoint3D) -> GMultiPoint3D& = default;
    auto operator =(GMultiPoint3D &&gMultiPoint3D) noexcept -> GMultiPoint3D& = default;
    
    auto geometry() const -> const MultiPoint<Point3d> & { return mGeometry; }
    auto geometry() -> MultiPoint<Point3d> & { return mGeometry; }

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const ->BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
    auto clone() const -> std::unique_ptr<GraphicEntity> override;
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

