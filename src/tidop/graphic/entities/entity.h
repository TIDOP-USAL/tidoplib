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

#include <string>
#include <vector>
#include <array>
#include <memory>

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/graphic/styles.h"
#include "tidop/graphic/datamodel.h"

namespace tl
{


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */


/*!
 * \brief Datos que pueden ir asociados a una entidad.
 */
class TL_EXPORT GData
{
public:

    GData()
    {
    }

    GData(const GData &gData)
    {
        ///TODO: terminar
        unusedParameter(gData);
    }

    ~GData()
    {
    }

    GData &operator =(const GData &gData)
    {
        ///TODO: terminar
        unusedParameter(gData);
        return *this;
    }


};



/*!
 * \brief Clase base para las entidades gráficas
 * \see GraphicStyle, GData
 */
class TL_EXPORT GraphicEntity
  : public GraphicStyle/*,
    public GData*/
{

public:

    /*!
     * \brief Tipos de entidades gráficas
     */
    enum class Type
    {
        point_2d,                   /*!< Punto 2D */
        point_3d,                   /*!< Punto 3D */
        linestring_2d,              /*!< Polilinea 2D */
        linestring_3d,              /*!< Polilinea 3D */
        polygon_2d,                 /*!< Poligono 2D */
        polygon_3d,                 /*!< Poligono 3D */
        segment_2d,                 /*!< Segmento 2D */
        segment_3d,                 /*!< Segmento 3D */
        window,                     /*!< Ventana */
        box,                        /*!< Caja */
        multipoint_2d,              /*!< Multipunto 2D */
        multipoint_3d,              /*!< Multipunto 3D */
        multiline_2d,               /*!< Multi-línea 2D */
        multiline_3d,               /*!< Multi-línea 3D */
        multipolygon_2d,            /*!< Multi-polígono 2D */
        multipolygon_3d,            /*!< Multi-polígono 3D */
        circle,                     /*!< Circulo */
        ellipse                     /*!< Elipse */
    };

protected:

    /*!
     * \brief Tipo de entidad
     * \see Type
     */
    Type mEntityType;

    std::shared_ptr<TableRegister> mData;

public:

    explicit GraphicEntity(Type type);
    GraphicEntity(const GraphicEntity &graphicEntity);
    GraphicEntity(GraphicEntity &&graphicEntity) TL_NOEXCEPT;

    ~GraphicEntity() override = default;;

    auto operator =(const GraphicEntity& graphicEntity) -> GraphicEntity&;
    auto operator =(GraphicEntity&& graphicEntity) TL_NOEXCEPT -> GraphicEntity&;

    /*!
     * \brief Returns the entity type
     */
    auto type() const -> Type;

    /*!
     * \brief Check if it is a Multi-entity
     */
    virtual auto isMultiEntity() const -> bool = 0;

    /*!
     * \brief Check if it is a simple entity
     */
    virtual auto isSimpleEntity() const -> bool = 0;

    auto data() const -> std::shared_ptr<TableRegister>;
    void setData(std::shared_ptr<TableRegister> &data);

};


/*! \} */ // End GraphicEntities

} // End namespace tl
