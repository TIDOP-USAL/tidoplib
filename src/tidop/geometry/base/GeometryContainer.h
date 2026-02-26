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

/*! \file GeometryContainer.h
 * \brief Generic container for geometric entities.
 *
 * This file defines the GeometryContainer class template, which provides
 * a wrapper around std::vector for storing collections of geometric entities.
 * It offers a complete STL-like interface with additional geometric utilities.
 * ### Classes
 * - \ref tl::GeometryContainer : Generic container for geometric entities.
 * \see tl::GeometryBase, tl::Geometry
 */

#pragma once

#include "tidop/config.h"

#include "tidop/geometry/base/Concepts.h"

#include <vector>

namespace tl
{

/*! \addtogroup Geometry
 *  \{
 */


/*!
 * \class GeometryContainer
 * \brief Container for geometric entities.
 *
 * A simple wrapper around `std::vector` to store and manage geometric entities.
 * Provides STL-compatible iterators and operations.
 *
 * \tparam Geometry_t Type of geometric entities to store.
 */
template<typename Geometry_t>
class GeometryContainer 
{
    static_assert(GeometryConcept<Geometry_t>,
                  "GeometryCollection requires a geometry type");

public:

    /*! \brief Value type (Geometry_t). */
    using value_type = typename std::vector<Geometry_t>::value_type;

    /*! \brief Reference type. */
    using reference = typename std::vector<Geometry_t>::reference;

    /*! \brief Const reference type. */
    using const_reference = typename std::vector<Geometry_t>::const_reference;

    /*! \brief Iterator type. */
    using iterator = typename std::vector<Geometry_t>::iterator;

    /*! \brief Const iterator type. */
    using const_iterator = typename std::vector<Geometry_t>::const_iterator;

    /*! \brief Reverse iterator type. */
    using reverse_iterator = typename std::vector<Geometry_t>::reverse_iterator;

private:

    std::vector<Geometry_t> mEntities; /*!< Internal vector storing entities. */

public:

    /*!
     * \brief Default constructor
     * Initializes an empty entity container.
     */
    GeometryContainer() = default;

    /*!
     * \brief Constructs an entity container with a predefined size.
     * \param[in] size Number of entities to allocate space for.
     * The container is initialized with the given size but does not necessarily populate entities.
     */
    explicit GeometryContainer(size_t size);

    /*!
     * \brief Copy constructor
     * \param[in] entity Another GeometryContainer to copy.
     */
    GeometryContainer(const GeometryContainer &entity) = default;

    /*!
     * \brief Move constructor
     * \param[in] entity Another GeometryContainer to move.
     */
    GeometryContainer(GeometryContainer &&entity) noexcept = default;

    /*!
     * \brief Constructs an entity container from a vector of entities.
     * \param[in] entities A vector containing Geometry_t objects.
     * Initializes the container with the provided entities.
     */
    explicit GeometryContainer(std::vector<Geometry_t> entities);

    /*!
     * \brief Constructs an entity container from an initializer list.
     * \param[in] entities An initializer list containing Geometry_t objects.
     */
    GeometryContainer(std::initializer_list<Geometry_t> entities);
   
    /*!
     * \brief Constructs a GeometryContainer from a range defined by iterators.
     *
     * This constructor initializes the container with elements from the iterator range [first, last).
     * The elements are copied from the source range into the container's internal storage.
     *
     * \tparam It Iterator type that must satisfy std::input_iterator and have a value type
     *           convertible to Geometry_t.
     * \param[in] first Iterator to the first element in the source range.
     * \param[in] last Iterator to one past the last element in the source range.
     *
     * \see GeometryContainer(R&&)
     */
    template<std::input_iterator It>
        requires std::convertible_to<std::iter_value_t<It>, Geometry_t>
    GeometryContainer(It first, It last)
      : mEntities(first, last) 
    {
    }

    /*!
     * \brief Constructs a GeometryContainer from a range object.
     *
     * This constructor initializes the container with elements from the specified range.
     * The range can be any C++20 range type (containers, views, etc.) whose elements
     * are convertible to Geometry_t.
     *
     * \tparam R Range type that must satisfy std::ranges::input_range and have a value type
     *           convertible to Geometry_t.
     * \param[in] range The source range containing elements to copy into the container.
     *
     * \see GeometryContainer(It, It)
     */
    template<std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, Geometry_t>
    explicit GeometryContainer(R &&range)
    {
        // Si el rango es forward_range y sized_range, hacemos reserve para optimizar
        if constexpr (std::ranges::forward_range<R> && std::ranges::sized_range<R>) {
            mEntities.reserve(std::ranges::size(range));
        }

        mEntities.insert(
            mEntities.end(),
            std::ranges::begin(range),
            std::ranges::end(range)
        );
    }

    ~GeometryContainer() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] entity Another GeometryContainer to copy.
     * \return Reference to this container.
     */
    auto operator=(const GeometryContainer<Geometry_t> &entity) -> GeometryContainer<Geometry_t> & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] entity Another GeometryContainer to move.
     * \return Reference to this container.
     */
    auto operator=(GeometryContainer<Geometry_t> &&entity) noexcept -> GeometryContainer<Geometry_t> & = default;

    /*!
     * \brief Returns an iterator to the beginning.
     * \return Iterator to the first element.
     */
    [[nodiscard]] 
    constexpr auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning.
     * \return Const iterator to the first element.
     */
    [[nodiscard]]
    constexpr auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to the end.
     * \return Iterator to the element following the last element.
     */
    [[nodiscard]] 
    constexpr auto end() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the end.
     * \return Const iterator to the element following the last element.
     */
    [[nodiscard]]
    constexpr auto end() const noexcept -> const_iterator;

    /*! \brief Returns a const iterator to the beginning. */
    [[nodiscard]]
    constexpr auto cbegin() const noexcept -> const_iterator;

    /*! \brief Returns a const iterator to the end. */
    [[nodiscard]]
    constexpr auto cend() const noexcept -> const_iterator;

    /*! \brief Returns a reverse iterator to the beginning. */
    [[nodiscard]]
    constexpr auto rbegin() noexcept -> reverse_iterator;

    /*! \brief Returns a reverse iterator to the end. */
    [[nodiscard]]
    constexpr auto rend() noexcept -> reverse_iterator;
	
    [[nodiscard]]
    constexpr auto front() noexcept -> reference;

    [[nodiscard]]
    constexpr auto front() const noexcept -> const_reference;

    [[nodiscard]]
    constexpr auto back() noexcept -> reference;

    [[nodiscard]]
    constexpr auto back() const noexcept -> const_reference;

    /*!
     * \brief Adds an entity to the end.
     * \param[in] entity Entity to add.
     */
    void push_back(const Geometry_t &entity);

    /*!
     * \brief Adds an entity to the end (move version).
     * \param[in] entity Entity to move.
     */
    void push_back(Geometry_t &&entity);

    /*!
     * \brief Constructs an entity in-place at the end.
     * \tparam Args Types of arguments to forward to the entity constructor.
     * \param[in] args Arguments to forward to the entity constructor.
     */
	template<typename... Args>
    requires std::constructible_from<Geometry_t, Args...>
    void emplace_back(Args&&... args)
    {
        mEntities.emplace_back(std::forward<Args>(args)...);
    }
	
    /*!
     * \brief Returns a constant reference to the element at the specified position.
     * return Constant reference to the element
     */
    [[nodiscard]]
    auto at(size_t position) const -> const_reference;
    
    /*!
     * \brief Returns a reference to the element at the specified position.
     * return Reference to the element
     */
    [[nodiscard]]
    auto at(size_t position) -> reference;
    
    /*!
     * \brief Removes the elements from the container
     */
    constexpr void clear() noexcept;
      
    /*!
     * \brief Check if the container is empty
     * \return true if the container is empty, false otherwise
     */
    [[nodiscard]] 
    auto isEmpty() const -> bool;
    
    /*!
     * \brief Sets the size of the container
     * \param[in] size Size of the container
     */
    void reserve(size_t size);
    
    /*!
     * \brief Resizing of the container
     * If the current size is less than count, additional elements are added. If the current 
     * size is greater than count the container is truncated to the specified number of elements.
     * \param[in] count New container size
     */
    void resize(size_t count);
    
    /*!
     * \brief Resizing of the container
     * If the current size is less than count, additional elements are added and initialised with value.
     * If the current size is greater than count the container is truncated to the specified number of elements.
     * \param[in] count New container size
     * \param[in] value Value to be assigned to the new elements
     */
    void resize(size_t count, const Geometry_t &value);
    
    /*!
     * \brief Returns the number of elements in the container.
     * \return Number of elements.
     */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t;

    /*!
     * \brief Returns the number of elements that can be held in currently allocated storage.
     * \return Current capacity.
     */
    [[nodiscard]]
    constexpr auto capacity() const noexcept -> size_t;

    /*!
     * \brief Returns a reference to the element at the specified position.
     * No check is made to see if the element to be accessed is within the limits.
     * return Constant reference to the element
     */
    [[nodiscard]] 
    auto operator[](size_t position) const -> const_reference;
    
    /*!
     * \brief Returns a reference to the element at the specified position.
     * No check is made to see if the element to be accessed is within the limits.
     * return Reference to the element
     */
    [[nodiscard]] 
    auto operator[](size_t position) -> reference;
    
    /*!
     * \brief Erases elements in the specified range.
     * \param[in] first Iterator to the first element to erase.
     * \param[in] last Iterator to one past the last element to erase.
     * \return Iterator following the last erased element.
     */
    constexpr auto erase(const_iterator first, const_iterator last) -> iterator;

    /*!
     * \brief Removes the element at the specified position.
     *
     * Removes the element at the given iterator position and returns an iterator
     * pointing to the element that followed the removed element (or end() if the
     * removed element was the last one).
     *
     * \param[in] pos Iterator to the element to remove. Must be a valid dereferenceable
     *                iterator within the container.
     * \return Iterator pointing to the element that now occupies the position of
     *         the removed element, or end() if the last element was removed.
     */
    constexpr auto erase(const_iterator pos) -> iterator;
    
    /*!
     * \brief Inserts elements from an initializer list.
     * \param[in] pos Iterator before which the content will be inserted.
     * \param[in] ilist Initializer list of elements to insert.
     */
    constexpr auto insert(const_iterator pos, std::initializer_list<Geometry_t> ilist) -> iterator;

    /*!
     * \brief Inserts elements from a range.
     * \tparam InputIt Input iterator type.
     * \param[in] pos Iterator before which the content will be inserted.
     * \param[in] first Iterator to the first element to insert.
     * \param[in] last Iterator to one past the last element to insert.
     */
    template<std::input_iterator It>
        requires std::convertible_to<std::iter_value_t<It>, Geometry_t>
    constexpr auto insert(const_iterator pos, It first, It last) -> iterator;

	
    /*!
     * \brief Equality comparison operator.
     * \param[in] other Another GeometryContainer to compare with.
     * \return true if containers are equal, false otherwise.
     */
    [[nodiscard]] 
    auto operator==(const GeometryContainer &other) const -> bool
    {
        return mEntities == other.mEntities;
    }

    /*!
     * \brief Inequality comparison operator.
     * \param[in] other Another GeometryContainer to compare with.
     * \return true if containers are not equal, false otherwise.
     */
    [[nodiscard]] 
    auto operator!=(const GeometryContainer &other) const -> bool
    {
        return !(*this == other);
    }
};



template<typename Geometry_t>
GeometryContainer<Geometry_t>::GeometryContainer(size_t size)
  : mEntities(size)
{
}

template<typename Geometry_t>
GeometryContainer<Geometry_t>::GeometryContainer(std::vector<Geometry_t> entities)
  : mEntities(std::move(entities))
{
}

template<typename Geometry_t>
GeometryContainer<Geometry_t>::GeometryContainer(std::initializer_list<Geometry_t> entities)
  : mEntities(entities)
{
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::begin() noexcept -> iterator
{
    return mEntities.begin();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::begin() const noexcept -> const_iterator
{
    return mEntities.cbegin();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::end() noexcept -> iterator
{
    return mEntities.end();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::end() const noexcept -> const_iterator
{
    return mEntities.cend();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::cbegin() const noexcept -> const_iterator
{ 
    return mEntities.cbegin();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::cend() const noexcept -> const_iterator
{ 
    return mEntities.cend(); 
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::rbegin() noexcept -> reverse_iterator
{ 
    return mEntities.rbegin();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::rend() noexcept -> reverse_iterator
{ 
    return mEntities.rend();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::front() noexcept -> reference
{
    return mEntities.front();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::front() const noexcept -> const_reference
{
    return mEntities.front();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::back() noexcept -> reference
{
    return mEntities.back();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::back() const noexcept -> const_reference
{
    return mEntities.back();
}

template<typename Geometry_t>
void GeometryContainer<Geometry_t>::push_back(const Geometry_t &entity)
{
    mEntities.push_back(entity);
}

template<typename Geometry_t>
void GeometryContainer<Geometry_t>::push_back(Geometry_t &&entity)
{
    mEntities.push_back(std::forward<Geometry_t>(entity));
}

template<typename Geometry_t>
auto GeometryContainer<Geometry_t>::at(size_t position) const -> const_reference 
{
    return mEntities.at(position);
}

template<typename Geometry_t>
auto GeometryContainer<Geometry_t>::at(size_t position) -> reference 
{
    return mEntities.at(position);
}

template<typename Geometry_t>
constexpr void GeometryContainer<Geometry_t>::clear() noexcept
{ 
    mEntities.clear();
}

template<typename Geometry_t>
auto GeometryContainer<Geometry_t>::isEmpty() const -> bool
{
    return mEntities.empty();
}

template<typename Geometry_t>
void GeometryContainer<Geometry_t>::reserve(size_t size)
{
    mEntities.reserve(size);
}

template<typename Geometry_t>
void GeometryContainer<Geometry_t>::resize(size_t count)
{
    mEntities.resize(count);
}

template<typename Geometry_t>
void GeometryContainer<Geometry_t>::resize(size_t count, const Geometry_t &value)
{
    mEntities.resize(count, value);
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::size() const noexcept -> size_t
{ 
    return mEntities.size();
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::capacity() const noexcept -> size_t
{
    return mEntities.capacity();
}

template<typename Geometry_t>
auto GeometryContainer<Geometry_t>::operator[](size_t position) const -> const_reference 
{
    return mEntities[position];
}
  
template<typename Geometry_t>
auto GeometryContainer<Geometry_t>::operator[](size_t position) -> reference 
{
    return mEntities[position];
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::erase(const_iterator first, const_iterator last) -> iterator
{
    return mEntities.erase(first, last);
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::erase(const_iterator pos) -> iterator
{
    return mEntities.erase(pos);
}

template<typename Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::insert(const_iterator pos,
                                                   std::initializer_list<Geometry_t> ilist) -> iterator
{
    return mEntities.insert(pos, ilist);
}

template<typename Geometry_t>
template<std::input_iterator It>
    requires std::convertible_to<std::iter_value_t<It>, Geometry_t>
constexpr auto GeometryContainer<Geometry_t>::insert(const_iterator pos,
                                                   It first, 
                                                   It last) -> iterator
{
    return mEntities.insert(pos, first, last);
}


/*! \} */ 


} // End namespace tl