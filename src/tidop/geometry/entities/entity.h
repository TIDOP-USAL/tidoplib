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

#include "tidop/config.h"

#include <iterator>
#include <limits>
#include <numeric>
#include <vector>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/flags.h"

namespace tl
{


/*! \defgroup geometry Geometric Entities
 *  Puntos, lineas, ...
 *  \{
 */

template<typename T> class Point;
template<typename T> class Point3;
template<typename T> class Window;
template<typename T> class BoundingBox;


/*!
 * \brief Base class for geometric entities
 */
class TL_EXPORT Entity
{

public:

    /*!
     * \brief Types of geometric entities
     */
    enum class Type
    {
        /* Dimension */
        geom2d = (0 << 0),                        /*!< 2D Geometry */
        geom3d = (1 << 0),                        /*!< 3D Geometry */
        geom4d = (1 << 1),                        /*!< 4D Geometry */

        /* Multi-entity */
        multi_entity = (1 << 2),                  /*!< Multi-entity */

        /* 2D entities */
        point2d = (1 << 3),                       /*!< 2D Point */
        linestring2d = (1 << 4),                  /*!< 2D Polyline */
        polygon2d = (1 << 5),                     /*!< 2D Polygon */
        segment2d = (1 << 6),                     /*!< 2D Segment */
        circle = (1 << 7),                        /*!< Circle */
        ellipse = (1 << 8),                       /*!< Ellipse */
        triangle = (1 << 9),                      /*!< Triangle */
        square = (1 << 10),                       /*!< Square */
        rectangle = (1 << 11),                    /*!< Rectangle */

        /* 3D entities */
        point3d = point2d | geom3d,               /*!< 3D Point */
        linestring3d = linestring2d | geom3d,     /*!< 3D Polyline */
        polygon3d = polygon2d | geom3d,           /*!< 3D Polygon */
        segment3d = segment2d | geom3d,           /*!< 3D Segment */

        /* multientidades */
        multipoint2d = point2d | multi_entity,      /*!< 2D Multipoint */
        multipoint3d = point3d | multi_entity,      /*!< 3D Multipoint */
        multiline2d = linestring2d | multi_entity,  /*!< 2D Multi-line */
        multiline3d = linestring3d | multi_entity,  /*!< 3D Multi-line */
        multipolygon2d = polygon2d | multi_entity,  /*!< 2D Multi-polygon */
        multipoygon3d = polygon3d | multi_entity,   /*!< 3D Multi-polygon */

        /* Special types */
        envelope = (1 << 20),                       /*!< Envelope */
        window = envelope,                          /*!< Window */
        bounding_box = envelope | geom3d            /*!< Bounding box */
    };

private:

    /*!
     * \brief Type of entity
     * \see type
     */
    EnumFlags<Type> mEntityType;

public:

    Entity(Type type) : mEntityType(type) {}
    Entity(const Entity &entity) = default;
    Entity(Entity &&entity) TL_NOEXCEPT : mEntityType(std::move(entity.mEntityType)) {}
    virtual ~Entity() = default;
    
    /*!
     * \brief Returns the type of entity
     */
    auto type() const -> Type;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const Entity &entity) -> Entity &;
    
    /*!
     * \brief Move assignment operator
     */
    auto operator = (Entity &&entity) TL_NOEXCEPT -> Entity &;

    /*!
     * \brief Checks if an entity is 3D
     * \return True if it is a 3D entity
     */
    auto is3D() const -> bool;

};
ALLOW_BITWISE_FLAG_OPERATIONS(Entity::Type)



/* ---------------------------------------------------------------------------------- */

//TODO: Añadir constructor a partir de iteradores first end

/*!
 * \brief Contenedor para entidades geométricas
 * 
 * Es un simple wrapper de std::vector para que las clases de geometrías 
 * se puedan utilizar como contenedores.
 */
template<typename Entity_t>
class EntityContainer
{

public:

    using allocator_type = typename std::vector<Entity_t>::allocator_type;
    using value_type = typename std::vector<Entity_t>::value_type;
    using size_type = typename std::vector<Entity_t>::size_type;
    using difference_type = typename std::vector<Entity_t>::difference_type;
    using pointer = typename std::vector<Entity_t>::pointer;
    using const_pointer = typename std::vector<Entity_t>::const_pointer;
    using reference = typename std::vector<Entity_t>::reference;
    using const_reference = typename std::vector<Entity_t>::const_reference;
    using iterator = typename std::vector<Entity_t>::iterator;
    using const_iterator = typename std::vector<Entity_t>::const_iterator;

private:

    /*!
     * \brief Conjunto de puntos
     */
    std::vector<Entity_t> mEntities;

public:

    EntityContainer();
    EntityContainer(size_type size);
    EntityContainer(const EntityContainer &entity);
    EntityContainer(EntityContainer &&entity) TL_NOEXCEPT;
    EntityContainer(std::vector<Entity_t> entities);
    EntityContainer(std::initializer_list<Entity_t> entities);
   
    virtual ~EntityContainer() = default;
    
    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;

    void push_back(const Entity_t &entity);
    void push_back(Entity_t &&entity);
    
    /*!
     * \brief Returns a constant reference to the element at the specified position.
     * return Constant reference to the element
     */
    auto at(size_type position) const -> const_reference;
    
    /*!
     * \brief Returns a reference to the element at the specified position.
     * return Reference to the element
     */
    auto at(size_type position) -> reference;
    
    /*!
     * \brief Removes the elements from the container
     */
    void clear();
      
    /*!
     * \brief Check if the container is empty
     * \return true if the container is empty, false otherwise
     */
    auto empty() const -> bool;
    
    /*!
     * \brief Sets the size of the container
     * \param[in] size Size of the container
     */
    void reserve(size_type size);
    
    /*!
     * \brief Resizing of the container
     * If the current size is less than count, additional elements are added. If the current 
     * size is greater than count the container is truncated to the specified number of elements.
     * \param[in] count New container size
     */
    void resize(size_type count);
    
    /*!
     * \brief Resizing of the container
     * If the current size is less than count, additional elements are added and initialised with value.
     * If the current size is greater than count the container is truncated to the specified number of elements.
     * \param[in] count New container size
     * \param[in] value Value to be assigned to the new elements
     */
    void resize(size_type count, const Entity_t &value);
    
    /*!
     * \brief Size of the container
     */
    auto size() const -> size_type;
    
    /*!
     * \brief Returns a reference to the element at the specified position.
     * No check is made to see if the element to be accessed is within the limits.
     * return Constant reference to the element
     */
    auto operator[](size_type position) const -> const_reference;
    
    /*!
     * \brief Returns a reference to the element at the specified position.
     * No check is made to see if the element to be accessed is within the limits.
     * return Reference to the element
     */
    auto operator[](size_type position) -> reference;
    
    /*!
     * \brief Copy assignment operator
     */
    auto operator=(const EntityContainer<Entity_t> &entity) -> EntityContainer<Entity_t>&;
    
    /*!
     * \brief Move assignment operator
     */
    auto operator=(EntityContainer<Entity_t> &&entity) TL_NOEXCEPT -> EntityContainer<Entity_t>&;
    
    /*!
     * \brief Delete the interval
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

};




// Entity Implementation


inline auto Entity::type() const -> Type 
{
    return mEntityType.flags(); 
}

inline auto Entity::operator = (const Entity &entity) -> Entity &
{
    if (this != &entity) {
        mEntityType = entity.mEntityType;
    }

    return *this;
}
    
inline auto Entity::operator = (Entity &&entity) TL_NOEXCEPT -> Entity &
{
    if (this != &entity) {
        mEntityType = std::move(entity.mEntityType);
    }

    return *this;
}

inline auto Entity::is3D() const -> bool
{
    return mEntityType.isEnabled(Type::geom3d);
}



// Implementación EntityContainer

template<typename Entity_t>
EntityContainer<Entity_t>::EntityContainer() 
  : mEntities(0)
{
}

template<typename Entity_t>
EntityContainer<Entity_t>::EntityContainer(size_type size)
  : mEntities(size)
{
}

template<typename Entity_t>
EntityContainer<Entity_t>::EntityContainer(const EntityContainer &entity)
  : mEntities(entity.mEntities)
{
}

template<typename Entity_t>
EntityContainer<Entity_t>::EntityContainer(EntityContainer &&entity) TL_NOEXCEPT
  : mEntities(std::move(entity.mEntities))
{
}

template<typename Entity_t>
EntityContainer<Entity_t>::EntityContainer(std::vector<Entity_t> entities)
  : mEntities(std::move(entities))
{
}

template<typename Entity_t>
EntityContainer<Entity_t>::EntityContainer(std::initializer_list<Entity_t> entities)
  : mEntities(entities)
{
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::begin() TL_NOEXCEPT -> iterator
{
    return mEntities.begin();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::begin() const TL_NOEXCEPT -> const_iterator
{
    return mEntities.cbegin();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::end() TL_NOEXCEPT -> iterator 
{
    return mEntities.end();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::end() const TL_NOEXCEPT -> const_iterator 
{
    return mEntities.cend();
}

template<typename Entity_t>
void EntityContainer<Entity_t>::push_back(const Entity_t &entity)
{
    mEntities.push_back(entity);
}

template<typename Entity_t>
void EntityContainer<Entity_t>::push_back(Entity_t &&entity)
{
    mEntities.push_back(std::forward<Entity_t>(entity));
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::at(size_type position) const -> const_reference 
{
    return mEntities.at(position);
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::at(size_type position) -> reference 
{
    return mEntities.at(position);
}

template<typename Entity_t>
void EntityContainer<Entity_t>::clear() 
{ 
    mEntities.clear();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::empty() const -> bool
{
    return mEntities.empty();
}

template<typename Entity_t>
void EntityContainer<Entity_t>::reserve(size_type size)
{
    mEntities.reserve(size);
}

template<typename Entity_t>
void EntityContainer<Entity_t>::resize(size_type count)
{
    mEntities.resize(count);
}

template<typename Entity_t>
void EntityContainer<Entity_t>::resize(size_type count, const Entity_t &value)
{
    mEntities.resize(count, value);
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::size() const -> size_type
{ 
    return mEntities.size();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::operator[](size_type position) const -> const_reference 
{
    return mEntities[position];
}
  
template<typename Entity_t>
auto EntityContainer<Entity_t>::operator[](size_type position) -> reference 
{
    return mEntities[position];
}


template<typename Entity_t>
auto EntityContainer<Entity_t>::operator=(const EntityContainer<Entity_t> &entity) -> EntityContainer<Entity_t>&
{
    if (this != &entity) {
        this->mEntities = entity.mEntities;
    }

    return (*this);
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::operator=(EntityContainer<Entity_t> &&entity) TL_NOEXCEPT -> EntityContainer<Entity_t>&
{
    if (this != &entity) {
        this->mEntities.clear();
        this->mEntities = std::move(entity.mEntities);
    }

    return (*this);
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::erase(const_iterator first, const_iterator last) -> iterator
{
    return mEntities.erase(first, last);
}

/*! \} */ // end of geometry

} // End namespace tl
