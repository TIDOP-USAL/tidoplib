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

/*! \file EntityContainer.h
 * \brief Generic container for geometric entities.
 *
 * This file defines the EntityContainer class template, which provides
 * a wrapper around std::vector for storing collections of geometric entities.
 * It offers a complete STL-like interface with additional geometric utilities.
 * ### Classes
 * - \ref tl::EntityContainer : Generic container for geometric entities.
 * \see tl::GeometryBase, tl::Geometry
 */

#pragma once

#include "tidop/config.h"

#include <vector>

namespace tl
{

/*! \addtogroup Geometry
 *  \{
 */


 /*!
  * \class EntityContainer
  * \brief Container for geometric entities.
  *
  * A simple wrapper around `std::vector` to store and manage geometric entities.
  * Provides STL-compatible iterators and operations.
  *
  * \tparam Entity_t Type of geometric entities to store.
  */
template<typename Entity_t>
class EntityContainer 
{

public:

    /*! \brief Allocator type. */
    using allocator_type = typename std::vector<Entity_t>::allocator_type;

    /*! \brief Value type (Entity_t). */
    using value_type = typename std::vector<Entity_t>::value_type;

    /*! \brief Size type. */
    using size_type = typename std::vector<Entity_t>::size_type;

    /*! \brief Difference type. */
    using difference_type = typename std::vector<Entity_t>::difference_type;

    /*! \brief Pointer type. */
    using pointer = typename std::vector<Entity_t>::pointer;

    /*! \brief Const pointer type. */
    using const_pointer = typename std::vector<Entity_t>::const_pointer;

    /*! \brief Reference type. */
    using reference = typename std::vector<Entity_t>::reference;

    /*! \brief Const reference type. */
    using const_reference = typename std::vector<Entity_t>::const_reference;

    /*! \brief Iterator type. */
    using iterator = typename std::vector<Entity_t>::iterator;

    /*! \brief Const iterator type. */
    using const_iterator = typename std::vector<Entity_t>::const_iterator;

    /*! \brief Reverse iterator type. */
    using reverse_iterator = typename std::vector<Entity_t>::reverse_iterator;

private:

    std::vector<Entity_t> mEntities; /*!< Internal vector storing entities. */

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
    EntityContainer(EntityContainer &&entity) noexcept;

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
    
    /*!
     * \brief Returns an iterator to the beginning.
     * \return Iterator to the first element.
     */
    auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning.
     * \return Const iterator to the first element.
     */
    auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to the end.
     * \return Iterator to the element following the last element.
     */
    auto end() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the end.
     * \return Const iterator to the element following the last element.
     */
    auto end() const noexcept -> const_iterator;

    /*! \brief Returns a const iterator to the beginning. */
    auto cbegin() const noexcept -> const_iterator;

    /*! \brief Returns a const iterator to the end. */
    auto cend() const noexcept -> const_iterator;

    /*! \brief Returns a reverse iterator to the beginning. */
    auto rbegin() noexcept -> reverse_iterator;

    /*! \brief Returns a reverse iterator to the end. */
    auto rend() noexcept -> reverse_iterator;
	
    /*!
     * \brief Adds an entity to the end.
     * \param[in] entity Entity to add.
     */
    void push_back(const Entity_t &entity);

    /*!
     * \brief Adds an entity to the end (move version).
     * \param[in] entity Entity to move.
     */
    void push_back(Entity_t &&entity);

    /*!
     * \brief Constructs an entity in-place at the end.
     * \tparam Args Types of arguments to forward to the entity constructor.
     * \param[in] args Arguments to forward to the entity constructor.
     */
	template<typename... Args>
    void emplace_back(Args&&... args)
    {
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
     * \brief Returns the number of elements in the container.
     * \return Number of elements.
     */
    auto size() const noexcept -> size_type;

    /*!
     * \brief Returns the number of elements that can be held in currently allocated storage.
     * \return Current capacity.
     */
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
     * \brief Copy assignment operator.
     * \param[in] entity Another EntityContainer to copy.
     * \return Reference to this container.
     */
    auto operator=(const EntityContainer<Entity_t> &entity) -> EntityContainer<Entity_t> &;

    /*!
     * \brief Move assignment operator.
     * \param[in] entity Another EntityContainer to move.
     * \return Reference to this container.
     */
    auto operator=(EntityContainer<Entity_t> &&entity) noexcept -> EntityContainer<Entity_t> &;
    
    /*!
     * \brief Erases elements in the specified range.
     * \param[in] first Iterator to the first element to erase.
     * \param[in] last Iterator to one past the last element to erase.
     * \return Iterator following the last erased element.
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;
	
    /*!
     * \brief Inserts elements from a range.
     * \tparam InputIt Input iterator type.
     * \param[in] pos Iterator before which the content will be inserted.
     * \param[in] first Iterator to the first element to insert.
     * \param[in] last Iterator to one past the last element to insert.
     */
    template<typename InputIt>
    void insert(const_iterator pos, InputIt first, InputIt last);
    
    /*!
     * \brief Inserts elements from an initializer list.
     * \param[in] pos Iterator before which the content will be inserted.
     * \param[in] ilist Initializer list of elements to insert.
     */
    void insert(const_iterator pos, std::initializer_list<Entity_t> ilist);
	
    /*!
     * \brief Equality comparison operator.
     * \param[in] other Another EntityContainer to compare with.
     * \return true if containers are equal, false otherwise.
     */
    auto operator==(const EntityContainer &other) const -> bool
    {
        return mEntities == other.mEntities;
    }

    /*!
     * \brief Inequality comparison operator.
     * \param[in] other Another EntityContainer to compare with.
     * \return true if containers are not equal, false otherwise.
     */
    auto operator!=(const EntityContainer &other) const -> bool
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
EntityContainer<Entity_t>::EntityContainer(EntityContainer &&entity) noexcept
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
auto EntityContainer<Entity_t>::begin() noexcept -> iterator
{
    return mEntities.begin();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::begin() const noexcept -> const_iterator
{
    return mEntities.cbegin();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::end() noexcept -> iterator 
{
    return mEntities.end();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::end() const noexcept -> const_iterator 
{
    return mEntities.cend();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::cbegin() const noexcept -> const_iterator
{ 
    return mEntities.cbegin();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::cend() const noexcept -> const_iterator
{ 
    return mEntities.cend(); 
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::rbegin() noexcept -> reverse_iterator
{ 
    return mEntities.rbegin();
}

template<typename Entity_t>
auto EntityContainer<Entity_t>::rend() noexcept -> reverse_iterator
{ 
    return mEntities.rend();
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
auto EntityContainer<Entity_t>::operator=(EntityContainer<Entity_t> &&entity) noexcept -> EntityContainer<Entity_t>&
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

template<typename Entity_t>
void EntityContainer<Entity_t>::insert(const_iterator pos, std::initializer_list<Entity_t> ilist)
{
    mEntities.insert(pos, ilist);
}

template<typename Entity_t>
template<typename InputIt>
void EntityContainer<Entity_t>::insert(const_iterator pos, InputIt first, InputIt last)
{
    mEntities.insert(pos, first, last);
}

/*! \} */ 


} // End namespace tl