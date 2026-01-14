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

#include <vector>

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief Container for geometric entities.
 *
 * A simple wrapper around `std::vector` to store and manage geometric entities.
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

    /*!
     * \brief Default constructor
     * Initializes an empty entity container.
     */
    EntityContainer();

    /*!
     * \brief Constructs an entity container with a predefined size.
     * \param[in] size Number of entities to allocate space for.
     * The container is initialized with the given size but does not necessarily populate entities.
     */
    EntityContainer(size_type size);

    /*!
     * \brief Copy constructor
     * \param[in] entity Another EntityContainer to copy.
     */
    EntityContainer(const EntityContainer &entity);

    /*!
     * \brief Move constructor
     * \param[in] entity Another EntityContainer to move.
     */
    EntityContainer(EntityContainer &&entity) TL_NOEXCEPT;

    /*!
     * \brief Constructs an entity container from a vector of entities.
     * \param[in] entities A vector containing Entity_t objects.
     * Initializes the container with the provided entities.
     */
    EntityContainer(std::vector<Entity_t> entities);

    /*!
     * \brief Constructs an entity container from an initializer list.
     * \param[in] entities An initializer list containing Entity_t objects.
     */
    EntityContainer(std::initializer_list<Entity_t> entities);
   
    virtual ~EntityContainer() = default;
    
    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;

    auto cbegin() const TL_NOEXCEPT { return mEntities.cbegin(); }
    auto cend() const TL_NOEXCEPT { return mEntities.cend(); }
    auto rbegin() TL_NOEXCEPT { return mEntities.rbegin(); }
    auto rend() TL_NOEXCEPT { return mEntities.rend(); }
	
    void push_back(const Entity_t &entity);
    void push_back(Entity_t &&entity);
    
	template<typename... Args>
    void emplace_back(Args&&... args) {
        mEntities.emplace_back(std::forward<Args>(args)...);
    }
	
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
    auto size() const noexcept -> size_type;
    
    auto capacity() const noexcept -> size_type;

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
	
    template<typename InputIt>
    void insert(const_iterator pos, InputIt first, InputIt last)
    {
        mEntities.insert(pos, first, last);
    }
    
    void insert(const_iterator pos, std::initializer_list<Entity_t> ilist)
    {
        mEntities.insert(pos, ilist);
    }
	
    bool operator==(const EntityContainer& other) const 
    {
        return mEntities == other.mEntities;
    }
    
    bool operator!=(const EntityContainer& other) const 
    {
        return !(*this == other);
    }
};


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
auto EntityContainer<Entity_t>::size() const noexcept -> size_type
{ 
    return mEntities.size();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::capacity() const noexcept -> size_type
{
    return mEntities.capacity();
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

/*! \} */ 

} // End namespace tl