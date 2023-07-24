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

namespace graph
{

class TL_EXPORT GPoint
  : public Point<double>,
    public GraphicEntity
{

public:

    /*!
     * \brief Constructora por defecto
     */
    GPoint();

    /*!
     * \brief Constructora
     * \param[in] x Coordenada x
     * \param[in] y Coordenada y
     */
    GPoint(double x, double y);

    /*!
     * \brief Constructor a partir de un punto
     * \param[in] pt Punto
     * \see Point
     */
    GPoint(const Point<double> &pt);

    /*!
     * \brief Constructor de copia
     * \param[in] pt Objeto GPoint que se copia
     */
    GPoint(const GPoint &pt);

    /*!
     * \brief Constructor de movimiento
     * \param[in] pt Objeto GPoint que se mueve
     */
    GPoint(GPoint &&pt) TL_NOEXCEPT;

    /*!
     * \brief Destructora
     */
    ~GPoint() override;

    /*!
     * \brief Operador de asignación
     * \param gPoint Objeto GPoint que se asigna
     * \return Referencia al objeto
     */
    GPoint &operator = (const GPoint &gPoint);

    /*!
     * \brief Operador de asignación de movimiento
     * \param gPoint Objeto GPoint que se mueve
     * \return Referencia al objeto
     */
    GPoint &operator = (GPoint &&gPoint) TL_NOEXCEPT;

    bool isMultiEntity() const override;
    bool isSimpleEntity() const override;
};


class TL_EXPORT GPoint3D
  : public Point3<double>,
    public GraphicEntity
{

public:

    /*!
     * \brief Constructora por defecto
     */
    GPoint3D();

    /*!
     * \brief Constructora
     * \param[in] x Coordenada x
     * \param[in] y Coordenada y
     * \param[in] z Coordenada z
     */
    GPoint3D(double x, double y, double z);

    /*!
     * \brief Constructor a partir de un punto
     * \param[in] pt Punto
     * \see Point3
     */
    GPoint3D(const Point3<double> &pt);

    /*!
     * \brief Constructor de copia
     * \param[in] pt Objeto GPoint3D que se copia
     */
    GPoint3D(const GPoint3D &pt);

    /*!
     * \brief Constructor de movimiento
     * \param[in] pt Objeto GPoint3D que se mueve
     */
    GPoint3D(GPoint &&pt) TL_NOEXCEPT;

    /*!
     * \brief Destructora
     */
    ~GPoint3D() override;

    /*!
     * \brief Operador de asignación
     * \param gPoint Objeto GPoint3D que se copia
     * \return Referencia al objeto
     */
    GPoint3D &operator = (const GPoint3D &gPoint);

    /*!
     * \brief Operador de asignación de movimiento
     * \param[in] gPoint Objeto GPoint3D que se mueve
     */
    GPoint3D &operator = (GPoint3D &&gPoint) TL_NOEXCEPT;

    bool isMultiEntity() const override;
    bool isSimpleEntity() const override;
};


class TL_EXPORT GMultiPoint
  : public MultiPoint<Point<double>>,
    public GraphicEntity
{

public:

    GMultiPoint();
    GMultiPoint(const MultiPoint<Point<double>> &multiPoint);
    GMultiPoint(const GMultiPoint &gMultiPoint);
    ~GMultiPoint() override;

    GMultiPoint &operator = (const GMultiPoint &gMultiPoint);

    bool isMultiEntity() const override;
    bool isSimpleEntity() const override;
};


class TL_EXPORT GMultiPoint3D
  : public MultiPoint3D<Point3<double>>,
    public GraphicEntity
{

public:

    GMultiPoint3D();
    GMultiPoint3D(const MultiPoint3D<Point3<double>> &multiPoint);
    GMultiPoint3D(const GMultiPoint3D &gMultiPoint);
    ~GMultiPoint3D() override;

    GMultiPoint3D &operator = (const GMultiPoint3D &gMultiPoint);

    bool isMultiEntity() const override;
    bool isSimpleEntity() const override;
};

} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace tl

