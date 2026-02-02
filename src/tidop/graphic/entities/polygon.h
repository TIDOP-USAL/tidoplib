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
#include "tidop/graphic/entities/entity.h"

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
  : public Polygon<Point2d>,
    public GraphicEntity
{
public:

    /*!
     * \brief Default constructor
     */
    GPolygon();

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
    GPolygon(const GPolygon &gPolygon);

    /*!
     * \brief Move Constructor
     * \param[in] gPolygon GPolygon object that moves
     */
    GPolygon(GPolygon &&gPolygon) TL_NOEXCEPT;

    ~GPolygon() override;

    /*!
     * \brief Copy assignment operator
     * \param[in] gPolygon Object to be copied
     * \return Object reference
     */
    auto operator =(const GPolygon& gPolygon) -> GPolygon&;

    /*!
     * \brief Move assignment operator
     * \param[in] gPolygon GPolygon object that moves
     * \return Object reference
     */
    auto operator =(GPolygon&& gPolygon) TL_NOEXCEPT -> GPolygon&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
};


/*!
 * \brief 3D polygon graphics class
 */
class TL_EXPORT GPolygon3D
  : public Polygon<Point3d>,
    public GraphicEntity
{
public:

    /*!
     * \brief Default constructor
     */
    GPolygon3D();

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
    GPolygon3D(const GPolygon3D &gPolygon3D);

    /*!
     * \brief Move Constructor
     * \param[in] gPolygon3D GPolygon object that moves
     */
    GPolygon3D(GPolygon3D &&gPolygon3D) TL_NOEXCEPT;

    ~GPolygon3D() override;

    /*!
     * \brief Copy assignment operator
     * \param[in] gPolygon3D Object to be copied
     * \return Object reference
     */
    auto operator =(const GPolygon3D& gPolygon3D) -> GPolygon3D&;

    /*!
     * \brief Move assignment operator
     * \param[in] gPolygon3D GPolygon object that moves
     * \return Object reference
     */
    auto operator =(GPolygon3D&& gPolygon3D) TL_NOEXCEPT -> GPolygon3D&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
};


/*!
 * \brief Multi-polygon graphic class
 */
class TL_EXPORT GMultiPolygon
  : public MultiPolygon<Point2d>,
    public GraphicEntity
{
public:

    /*!
     * \brief Default constructor
     */
    GMultiPolygon();

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
    GMultiPolygon(const GMultiPolygon &multiPolygon);

    /*!
     * \brief Move Constructor
     * \param[in] multiPolygon GMultiPolygon object that moves
     */
    GMultiPolygon(GMultiPolygon &&multiPolygon) TL_NOEXCEPT;

    ~GMultiPolygon() override;

    /*!
     * \brief Copy assignment operator
     * \param[in] multiPolygon Object to be copied
     * \return Object reference
     */
    auto operator =(const GMultiPolygon &multiPolygon) -> GMultiPolygon&;

    /*!
     * \brief Move assignment operator
     * \param[in] multiPolygon GPolygon object that moves
     * \return Object reference
     */
    auto operator =(GMultiPolygon &&multiPolygon) TL_NOEXCEPT -> GMultiPolygon&;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
};


/*!
 * \brief Multi-polygon 3D graphic class
 */
class TL_EXPORT GMultiPolygon3D
  : public MultiPolygon<Point3d>,
    public GraphicEntity
{
public:

    /*!
     * \brief Default constructor
     */
    GMultiPolygon3D();

    /*!
     * \brief Constructor from a MultiPolygon
     * \param multiPolygon MultiPolygon object
     * \see MultiPolygon
     */
    explicit GMultiPolygon3D(const MultiPolygon<Point3d> &multiPolygon);

    explicit GMultiPolygon3D(size_t size);

    /*!
     * \brief Copy constructor
     * \param multiPolygon3D Object to be copied
     */
    GMultiPolygon3D(const GMultiPolygon3D &multiPolygon3D);

    /*!
     * \brief Move Constructor
     * \param[in] multiPolygon3D GPolygon object that moves
     */
    GMultiPolygon3D(GMultiPolygon3D &&multiPolygon3D) TL_NOEXCEPT;

    ~GMultiPolygon3D() override;

    /*!
     * \brief Operador de asignación
     * \param multiPolygon3D Objeto GMultiPolygon3D que se copia
     * \return Referencia al objeto
     */
    auto operator =(const GMultiPolygon3D &multiPolygon3D) -> GMultiPolygon3D&;

    /*!
     * \brief Move assignment operator
     * \param[in] multiPolygon3D GMultiPolygon3D object that moves
     * \return Object reference
     */
    auto operator =(GMultiPolygon3D &&multiPolygon3D) TL_NOEXCEPT -> GMultiPolygon3D &;

    auto isMultiEntity() const -> bool override;
    auto isSimpleEntity() const -> bool override;
    auto window() const -> BoundingBox<Point2d> override;
    void draw(Painter &painter) const override;
};




inline auto GPolygon::isMultiEntity() const -> bool
{
    return false;
}

inline auto GPolygon::isSimpleEntity() const -> bool
{
    return true;
}




inline auto GPolygon3D::isMultiEntity() const -> bool
{
    return false;
}

inline auto GPolygon3D::isSimpleEntity() const -> bool
{
    return true;
}




inline auto GMultiPolygon::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiPolygon::isSimpleEntity() const -> bool
{
    return false;
}




inline auto GMultiPolygon3D::isMultiEntity() const -> bool
{
    return true;
}

inline auto GMultiPolygon3D::isSimpleEntity() const -> bool
{
    return false;
}



/*! \} */ // Fin GraphicEntities

} // Fin namespace tl
