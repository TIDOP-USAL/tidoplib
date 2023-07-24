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


#include <list>
#include <memory>
#include <string>

#include "tidop/core/defs.h"
#include "tidop/graphic/datamodel.h"
#include "tidop/geometry/entities/window.h"

namespace tl
{


/*! \addtogroup GraphicEntities
 *  \{
 */

namespace graph
{

class GraphicEntity;

/*!
 * Capa. Puede contener elementos de uno o varios tipos. Pueden
 * ser entidades gráficas o simples
 */
class TL_EXPORT GLayer
{

public:

    /*!
     * \brief Allocator
     */
    using allocator_type = std::list<std::shared_ptr<GraphicEntity> >::allocator_type;

    /*!
     * \brief value_type
     */
    using value_type = std::list<std::shared_ptr<GraphicEntity> >::value_type;

    /*!
     * \brief Tipo entero sin signo (por lo general size_t)
     */
    using size_type = std::list<std::shared_ptr<GraphicEntity> >::size_type;

    /*!
     * \brief Tipo entero con signo (por lo general ptrdiff_t)
     */
    using difference_type = std::list<std::shared_ptr<GraphicEntity> >::difference_type;

    /*!
     * \brief std::allocator_traits<Allocator>::pointer
     */
    using pointer = std::list<std::shared_ptr<GraphicEntity> >::pointer;

    /*!
     * \brief std::allocator_traits<Allocator>::const_pointer
     */
    using const_pointer = std::list<std::shared_ptr<GraphicEntity> >::const_pointer;

    /*!
     * \brief value_type&
     */
    using reference = std::list<std::shared_ptr<GraphicEntity> >::reference;

    /*!
     * \brief const value_type&
     */
    using const_reference = std::list<std::shared_ptr<GraphicEntity> >::const_reference;

    /*!
     * \brief Iterador de acceso aleatorio
     */
    using iterator = std::list<std::shared_ptr<GraphicEntity> >::iterator;

    /*!
     * \brief Iterador constante de acceso aleatorio
     */
    using const_iterator = std::list<std::shared_ptr<GraphicEntity> >::const_iterator;

public:

    /*!
     * \brief Constructora por defecto
     */
    GLayer();

    /*!
     * \brief Constructor de copia
     * \param[in] gLayer Objeto que se copia
     */
    GLayer(const GLayer &gLayer);

    /*!
     * \brief Constructor de movimiento
     * \param[in] gLayer Objeto que se copia
     */
    GLayer(GLayer &&gLayer) TL_NOEXCEPT;


    /*!
     * \brief Constructora de lista
     * \param[in] entities listado de entidades
     */
    GLayer(std::initializer_list<std::shared_ptr<GraphicEntity>> entities);

    /*!
     * \brief Destructora
     */
    ~GLayer() = default;

    /*!
     * \brief Devuelve un iterador al inicio de la capa
     * \return Iterador al primer elemento
     */
    iterator begin() TL_NOEXCEPT;

    /*!
     * \brief Devuelve un iterador constante al inicio de la capa
     * \return Iterador al primer elemento
     */
    const_iterator begin() const TL_NOEXCEPT;

    /*!
     * \brief Devuelve un iterador al siguiente elemento después de la última entidad
     * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
     * \return Iterador al siguiente elemento después de la última entidad
     */
    iterator end() TL_NOEXCEPT;

    /*!
     * \brief Devuelve un iterador constante al siguiente elemento después de la última entidad
     * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
     * \return Iterador al siguiente elemento después de la última entidad
     */
    const_iterator end() const TL_NOEXCEPT;

    /*!
     * \brief Agrega una entidad mediante copia al final del contenedor
     * \param[in] entity Entidad que se añade
     */
    void push_back(const std::shared_ptr<GraphicEntity> &entity);

    /*!
     * \brief Agrega una entidad mediante movimiento al final del contenedor
     * \param[in] entity Entidad que se añade
     */
    void push_back(std::shared_ptr<GraphicEntity> &&entity) TL_NOEXCEPT;

    /*!
     * \brief Elimina los elementos del recipiente
     */
    void clear() TL_NOEXCEPT;

    /*!
     * \brief Comprueba si el contenedor esta vacio
     * \return true si el contenedor está vacío y false en caso contrario
     */
    bool empty() const TL_NOEXCEPT;

    /*!
     * \brief Modifica el tamaño del contenedor
     * Si el tamaño actual es menor que count, se añaden elementos adicionales. Si el tamaño actual
     * es mayor que count el contenedor se trunca al número de elementos indicados.
     * \param[in] count Nuevo tamaño del contenedor
     */
    void resize(size_type count);

    /*!
     * \brief Modifica el tamaño del contenedor
     * Si el tamaño actual es menor que count, se añaden elementos adicionales y se inicializan con value.
     * Si el tamaño actual es mayor que count el contenedor se trunca al número de elementos indicados.
     * \param[in] count Nuevo tamaño del contenedor
     * \param[in] value Valor que se asigna a los nuevos elementos
     */
    void resize(size_type count,
                const std::shared_ptr<GraphicEntity> &value);

    /*!
     * \brief Devuelve el tamaño del contenedor
     * \return Tamaño
     */
    size_type size() const TL_NOEXCEPT;

    /*!
     * \brief Asignación de copia
     */
    GLayer &operator=(const GLayer &entity);

    /*!
     * \brief Asignación de movimiento
     */
    GLayer &operator=(GLayer &&entity) TL_NOEXCEPT;

    /*!
     * \brief Elimina el intervalo
     */
    iterator erase(const_iterator first, const_iterator last);

    /*!
     * \brief Devuelve el nombre de la capa
     * \return Nombre de la capa
     */
    std::string name() const;

    /*!
     * \brief Establece el nombre de la capa
     * \param[in] name Nombre de la capa
     */
    void setName(const std::string &name);

    void addDataField(const std::shared_ptr<TableField> &field);

    std::vector<std::shared_ptr<TableField>> tableFields() const;

    Window<Point<double>> window() const;

protected:

    /*!
     * \brief Nombre de la capa
     */
    std::string mName;

    /*!
     * \brief Listado de entidades
     */
    std::list<std::shared_ptr<GraphicEntity>> mEntities;

    /*!
     * \brief Entidades seleccionadas
     */
    std::shared_ptr<GraphicEntity> mSelectEntity;

    std::vector<std::shared_ptr<TableField>> mTableFields;
};


} // namespace graph

/*! \} */ //  GraphicEntities

} // namespace tl
