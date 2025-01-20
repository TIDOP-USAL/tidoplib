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


#include <vector>
#include <array>

#include "tidop/math/math.h"


namespace tl
{

constexpr auto DynamicData = std::numeric_limits<size_t>::max();

/*! \addtogroup MathBase
 *  \{
 */

/*!
 * \class DataSize
 * \brief Calculates the size of a data structure based on its rows and columns.
 *
 * This class provides a static method to compute the size of a data structure.
 * If the number of rows is set to `DynamicData`, it returns `DynamicData`. Otherwise, it calculates
 * the size as the product of the number of rows and columns.
 *
 * \tparam T The type of the elements.
 * \tparam _rows The number of rows in the data structure.
 * \tparam _cols The number of columns in the data structure. Defaults to 1.
 */
template<typename T, size_t _rows, size_t _cols = 1>
class DataSize
{

public:

    /*!
     * \brief Computes the total size of the data structure.
     * \return The total size as the product of rows and columns, or `DynamicData` if rows are `DynamicData`.
     */
    static constexpr size_t size()
    {
        return _rows == DynamicData ? DynamicData : _rows * _cols;
    }

};


/*!
 * \class Data
 * \brief Static data storage container.
 *
 * The `Data` class provides a static storage mechanism for elements of type `T`.
 * It uses a fixed-size array internally and provides various operations to manipulate
 * the data, including accessing elements, iterating over the elements, and comparing data objects.
 *
 * \tparam T The type of the elements stored in the container.
 * \tparam _size The fixed size of the data container.
 */
template<typename T, size_t _size>
class Data
{

public:

    using value_type = T;               //!< The type of the elements stored.
    using size_type = size_t;           //!< The type used for indexing and size.
    using pointer = T *;                //!< Pointer to the data.
    using const_pointer = const T *;    //!< Constant pointer to the data.
    using reference = T &;              //!< Reference to the elements.
    using const_reference = const T &;  //!< Constant reference to the elements.

    using iterator = typename std::array<T, _size>::iterator;               //!< Iterator type.
    using const_iterator = typename std::array<T, _size>::const_iterator;   //!< Constant iterator type.

public:

    /*!
     * \brief Default constructor.
     * Initializes the data container.
     */
    Data();

    /*!
     * \brief Constructs the data container with a specific size.
     * \param[in] size The size of the data container.
     */
    Data(size_t size);

    /*!
     * \brief Constructs the data container with a specific size and initial value.
     * \param[in] size The size of the data container.
     * \param[in] value The initial value for all elements.
     */
    Data(size_t size, T value);

    /*!
     * \brief Copy constructor.
     * \param[in] matrix The data object to copy from.
     */
    Data(const Data &matrix);

    /*!
     * \brief Move constructor.
     * \param[in] matrix The data object to move from.
     */
    Data(Data &&matrix) TL_NOEXCEPT;

    /*!
     * \brief Constructor from an initializer list.
     * \param[in] values The initializer list containing the values.
     */
    Data(std::initializer_list<T> values);

    /*!
     * \brief Constructs the data container from a raw pointer.
     * \param[in] data The raw pointer to the data.
     * \param[in] size The size of the data (optional).
     */
    explicit Data(const T *data, size_t size = 0);

    virtual ~Data() = default;

    /*!
     * \brief Assignment operator.
     * \param[in] matrix The data object to assign from.
     * \return A reference to the assigned data object.
     */
    auto operator = (const Data &matrix) -> Data &;

    /*!
     * \brief Move assignment operator.
     * \param[in] matrix The data object to move from.
     * \return A reference to the moved data object.
     */
    auto operator = (Data &&matrix) TL_NOEXCEPT -> Data &;

    /*!
     * \brief Accesses an element at a given position with bounds checking.
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) -> reference;

    /*!
     * \brief Accesses an element at a given position with bounds checking (const version).
     * \param[in] position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) const -> const_reference;

    /*!
     * \brief Accesses an element at a given position without bounds checking.
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     */
    auto operator[](size_t position) TL_NOEXCEPT -> reference;

    /*!
     * \brief Accesses an element at a given position without bounds checking (const version).
     * \param[in] position The position of the element to access.
     * \return A const reference to the element at the specified position.
     */
    auto operator[](size_t position) const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Assigns a value to all elements in the data container.
     * \param[in] value The value to assign.
     * \return A reference to the modified data container.
     */
    auto operator=(T value) -> Data&;

    /*!
     * \brief Compares two data containers for equality.
     * \param[in] data The data container to compare with.
     * \return True if the data containers are equal, false otherwise.
     */
    auto operator == (const Data &data) const -> bool;

    /*!
     * \brief Compares two data containers for inequality.
     * \param[in] data The data container to compare with.
     * \return True if the data containers are not equal, false otherwise.
     */
    auto operator != (const Data &data) const -> bool;

    /*!
     * \brief Compares two data containers for less than.
     * \param[in] data The data container to compare with.
     * \return True if this data container is less than the other, false otherwise.
     */
    auto operator < (const Data &data) const -> bool;

    /*!
     * \brief Compares two data containers for less than or equal to.
     * \param[in] data The data container to compare with.
     * \return True if this data container is less than or equal to the other, false otherwise.
     */
    auto operator <= (const Data &data) const -> bool;

    /*!
     * \brief Compares two data containers for greater than.
     * \param[in] data The data container to compare with.
     * \return True if this data container is greater than the other, false otherwise.
     */
    auto operator > (const Data &data) const -> bool;

    /*!
     * \brief Compares two data containers for greater than or equal to.
     * \param[in] data The data container to compare with.
     * \return True if this data container is greater than or equal to the other, false otherwise.
     */
    auto operator >= (const Data &data) const -> bool;

    /*!
     * \brief Accesses the first element in the data container.
     * \return A reference to the first element.
     */
    auto front() TL_NOEXCEPT -> reference;

    /*!
     * \brief Accesses the first element in the data container (const version).
     * \return A const reference to the first element.
     */
    auto front() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Accesses the last element in the data container.
     * \return A reference to the last element.
     */
    auto back() TL_NOEXCEPT -> reference;

    /*!
     * \brief Accesses the last element in the data container (const version).
     * \return A const reference to the last element.
     */
    auto back() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Returns an iterator to the beginning of the data container.
     * \return An iterator to the beginning.
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning of the data container.
     * \return A const iterator to the beginning.
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the data container.
     * \return An iterator to the end.
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the end of the data container.
     * \return A const iterator to the end.
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns a pointer to the underlying data.
     * \return A pointer to the data.
     */
    auto data() TL_NOEXCEPT -> pointer;

    /*!
     * \brief Returns a const pointer to the underlying data.
     * \return A const pointer to the data.
     */
    auto data() const TL_NOEXCEPT -> const_pointer;

    /*!
     * \brief Gets the size of the data container.
     * \return The size of the data container.
     */
    auto size() const TL_NOEXCEPT -> size_t { return mData.size(); }

private:

    std::array<T, _size> mData;

};





/*!
 * \brief Dynamic data storage class.
 *
 * This class provides dynamic storage for data using a vector. It supports
 * various operations such as element access, assignment, and iterators,
 * as well as vector resizing and manipulation. This class is specialized
 * for dynamic-sized data storage.
 *
 * \tparam T The type of elements in the data container.
 */
template<typename T>
class Data<T, DynamicData>
{

public:

    using value_type = T;               //!< The type of the elements stored.
    using size_type = size_t;           //!< The type used for indexing and size.
    using pointer = T *;                //!< Pointer to the data.
    using const_pointer = const T *;    //!< Constant pointer to the data.
    using reference = T &;              //!< Reference to the elements.
    using const_reference = const T &;  //!< Constant reference to the elements.

    using iterator = typename std::vector<T>::iterator;             //!< Iterator type.
    using const_iterator = typename std::vector<T>::const_iterator; //!< Constant iterator type.

public:

    /*!
     * \brief Default constructor that initializes an empty data container.
     */
    Data() = default;

    /*!
     * \brief Constructor that initializes a data container with a given size.
     * \param size The size of the data container.
     */
    Data(size_t size);

    /*!
     * \brief Constructor that initializes a data container with a given size and an initial value.
     * \param[in] size The size of the data container.
     * \param[in] val The value with which to initialize all elements.
     */
    Data(size_t size, T val);

    /*!
     * \brief Copy constructor that creates a new data container by copying another.
     * \param[in] matrix The data container to be copied.
     */
    Data(const Data &matrix);

    /*!
     * \brief Move constructor that transfers ownership of data from another container.
     * \param[in] matrix The data container from which the data is moved.
     */
    Data(Data &&matrix) TL_NOEXCEPT;

    /*!
     * \brief Constructor that initializes the data container from an initializer list.
     * \param[in] values An initializer list of values to initialize the container.
     */
    Data(std::initializer_list<T> values);

    /*!
     * \brief Constructor that initializes the data container from a raw pointer and size.
     * \param[in] data A pointer to the data.
     * \param[in] size The size of the data.
     */
    Data(const T *data, size_t size);

    virtual ~Data() = default;

    /*!
     * \brief Copy assignment operator that copies data from another container.
     * \param[in] matrix The data container to copy from.
     * \return A reference to the modified data container.
     */
    auto operator = (const Data &matrix) -> Data &;

    /*!
     * \brief Move assignment operator that transfers ownership of data from another container.
     * \param[in] matrix The data container from which to move data.
     * \return A reference to the modified data container.
     */
    auto operator = (Data &&matrix) TL_NOEXCEPT -> Data &;

    /*!
     * \brief Accesses an element at the specified position with bounds checking.
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) -> reference;

    /*!
     * \brief Accesses an element at the specified position with bounds checking (const version).
     * \param[in] position The position of the element to access.
     * \return A constant reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) const -> const_reference;

    /*!
     * \brief Accesses an element at the specified position without bounds checking.
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     */
    auto operator[](size_t position) TL_NOEXCEPT -> reference;

    /*!
     * \brief Accesses an element at the specified position without bounds checking (const version).
     * \param[in] position The position of the element to access.
     * \return A constant reference to the element at the specified position.
     */
    auto operator[](size_t position) const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Assigns a single value to all elements in the data container.
     * \param[in] value The value to assign to all elements.
     */
    void operator=(T value);

    /*!
     * \brief Accesses the first element of the data container.
     * \return A reference to the first element.
     */
    auto front() TL_NOEXCEPT -> reference;

    /*!
     * \brief Accesses the first element of the data container (const version).
     * \return A constant reference to the first element.
     */
    auto front() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Accesses the last element of the data container.
     * \return A reference to the last element.
     */
    auto back() TL_NOEXCEPT -> reference;

    /*!
     * \brief Accesses the last element of the data container (const version).
     * \return A constant reference to the last element.
     */
    auto back() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Returns an iterator to the beginning of the data container.
     * \return An iterator to the first element.
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a constant iterator to the beginning of the data container.
     * \return A constant iterator to the first element.
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the data container.
     * \return An iterator to the last element.
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a constant iterator to the end of the data container.
     * \return A constant iterator to the last element.
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns a pointer to the underlying data.
     * \return A pointer to the data.
     */
    auto data() TL_NOEXCEPT -> pointer;

    /*!
     * \brief Returns a constant pointer to the underlying data.
     * \return A constant pointer to the data.
     */
    auto data() const TL_NOEXCEPT -> const_pointer;

    /*!
     * \brief Returns the size of the data container.
     * \return The size of the data container.
     */
    auto size() const TL_NOEXCEPT -> size_t { return mData.size(); }

private:

    std::vector<T> mData;

};




/* Implementation */

/* Static Data implementation */

template<typename T, size_t _size>
Data<T, _size>::Data()
  : mData()
{
    this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size>
Data<T, _size>::Data(size_t  /*size*/)
  : mData()
{
    this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size>
Data<T, _size>::Data(size_t  /*size*/, T val)
  : mData()
{
    this->mData.fill(val);
}

template<typename T, size_t _size>
Data<T, _size>::Data(const Data &matrix)
  : mData(matrix.mData)
{
}

template<typename T, size_t _size>
Data<T, _size>::Data(Data &&matrix) TL_NOEXCEPT
    : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _size>
Data<T, _size>::Data(std::initializer_list<T> values)
{
    size_t n = values.size();
    if (n == _size) {
        std::copy(values.begin(), values.end(), mData.begin());
    } else if (n < _size) {
        std::copy(values.begin(), values.end(), mData.begin());
        std::fill(mData.begin() + n, mData.end(), consts::zero<T>);
    } else {
        std::copy(values.begin(), values.begin() + _size, mData.begin());
    }
}

template<typename T, size_t _size>
Data<T, _size>::Data(const T *data,
                     size_t /*size*/)
  : mData(data)
{
}

template<typename T, size_t _size>
auto Data<T, _size>::operator = (const Data &matrix) -> Data &
{
    if (this != &matrix) {
        this->mData = matrix.mData;
    }
    return *this;
}

template<typename T, size_t _size>
auto Data<T, _size>::operator = (Data &&matrix) TL_NOEXCEPT -> Data &
{
    if (this != &matrix) {
        this->mData = std::move(matrix.mData);
    }
    return *this;
}

template<typename T, size_t _size>
auto Data<T, _size>::at(size_t position) -> reference
{
    return mData.at(position);
}

template<typename T, size_t _size>
auto Data<T, _size>::at(size_t position) const -> const_reference
{
    return mData.at(position);
}

template<typename T, size_t _size>
auto Data<T, _size>::operator[](size_t position) TL_NOEXCEPT -> reference
{
    return mData[position];
}

template<typename T, size_t _size>
auto Data<T, _size>::operator[](size_t position) const TL_NOEXCEPT -> const_reference
{
    return mData[position];
}

template<typename T, size_t _size>
auto Data<T, _size>::operator=(T value) -> Data &
{
    mData.fill(value);
    return *this;
}

template<typename T, size_t _size>
auto Data<T, _size>::operator == (const Data &data) const -> bool
{
    return this->mData == data.mData;
}

template<typename T, size_t _size>
auto Data<T, _size>::operator != (const Data &data) const -> bool
{
    return this->mData != data.mData;
}

template<typename T, size_t _size>
auto Data<T, _size>::operator < (const Data &data) const -> bool
{
    return this->mData < data.mData;
}

template<typename T, size_t _size>
auto Data<T, _size>::operator <= (const Data &data) const -> bool
{
    return this->mData <= data.mData;
}

template<typename T, size_t _size>
auto Data<T, _size>::operator > (const Data &data) const -> bool
{
    return this->mData > data.mData;
}

template<typename T, size_t _size>
auto Data<T, _size>::operator >= (const Data &data) const -> bool
{
    return this->mData >= data.mData;
}

template<typename T, size_t _size>
auto Data<T, _size>::front() TL_NOEXCEPT -> reference
{
    return mData.front();
}

template<typename T, size_t _size>
auto Data<T, _size>::front() const TL_NOEXCEPT -> const_reference
{
    return mData.front();
}

template<typename T, size_t _size>
auto Data<T, _size>::back() TL_NOEXCEPT -> reference
{
    return mData.back();
}

template<typename T, size_t _size>
auto Data<T, _size>::back() const TL_NOEXCEPT -> const_reference
{
    return mData.back();
}

template<typename T, size_t _size>
auto Data<T, _size>::begin() TL_NOEXCEPT -> iterator
{
    return mData.begin();
}

template<typename T, size_t _size>
auto Data<T, _size>::begin() const TL_NOEXCEPT -> const_iterator
{
    return mData.begin();
}

template<typename T, size_t _size>
auto Data<T, _size>::end() TL_NOEXCEPT -> iterator
{
    return mData.end();
}

template<typename T, size_t _size>
auto Data<T, _size>::end() const TL_NOEXCEPT -> const_iterator
{
    return mData.end();
}

template<typename T, size_t _size>
auto Data<T, _size>::data() TL_NOEXCEPT -> pointer
{
    return mData.data();
}

template<typename T, size_t _size>
auto Data<T, _size>::data() const TL_NOEXCEPT -> const_pointer
{
    return mData.data();
}



/* Dynamic Data implementation */

template<typename T>
Data<T, DynamicData>::Data(size_t size)
  : mData(size, -std::numeric_limits<T>().max())
{
}

template<typename T>
Data<T, DynamicData>::Data(size_t size, T val)
  : mData(size, val)
{
}

template<typename T>
Data<T, DynamicData>::Data(const Data &matrix)
  : mData(matrix.mData)
{
}

template<typename T>
Data<T, DynamicData>::Data(Data &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))
{
}

template<typename T>
Data<T, DynamicData>::Data(std::initializer_list<T> values)
{
    mData.resize(values.size());
    std::copy(values.begin(), values.end(), mData.begin());
}

template<typename T>
Data<T, DynamicData>::Data(const T *data, size_t size)
    : mData(size, -std::numeric_limits<T>().max())
{
    mData.assign(data, data + mData.size());
}

template<typename T>
auto Data<T, DynamicData>::operator = (const Data<T, DynamicData> &matrix) -> Data &
{
    if (this != &matrix) {
        this->mData = matrix.mData;
    }
    return *this;
}

template<typename T>
auto Data<T, DynamicData>::operator = (Data<T, DynamicData> &&matrix) TL_NOEXCEPT -> Data &
{
    if (this != &matrix) {
        this->mData = std::move(matrix.mData);
    }
    return *this;
}

template<typename T>
auto Data<T, DynamicData>::at(size_t position) -> reference
{
    return mData.at(position);
}

template<typename T>
auto Data<T, DynamicData>::at(size_t position) const -> const_reference
{
    return mData.at(position);
}

template<typename T>
auto Data<T, DynamicData>::operator[](size_t position) TL_NOEXCEPT -> reference
{
    return mData[position];
}

template<typename T>
auto Data<T, DynamicData>::operator[](size_t position) const TL_NOEXCEPT -> const_reference
{
    return mData[position];
}

template<typename T>
void Data<T, DynamicData>::operator=(T value)
{
    std::fill(mData.begin(), mData.end(), value);
}

template<typename T>
auto Data<T, DynamicData>::front() TL_NOEXCEPT -> reference
{
    return mData.front();
}

template<typename T>
auto Data<T, DynamicData>::front() const TL_NOEXCEPT -> const_reference
{
    return mData.front();
}

template<typename T>
auto Data<T, DynamicData>::back() TL_NOEXCEPT -> reference
{
    return mData.back();
}

template<typename T>
auto Data<T, DynamicData>::back() const TL_NOEXCEPT -> const_reference
{
    return mData.back();
}

template<typename T>
auto Data<T, DynamicData>::begin() TL_NOEXCEPT  -> iterator
{
    return mData.begin();
}

template<typename T>
auto Data<T, DynamicData>::begin() const TL_NOEXCEPT-> const_iterator
{
    return mData.begin();
}

template<typename T>
auto Data<T, DynamicData>::end() TL_NOEXCEPT -> iterator
{
    return mData.end();
}

template<typename T>
auto Data<T, DynamicData>::end() const TL_NOEXCEPT -> const_iterator
{
    return mData.end();
}

template<typename T>
auto Data<T, DynamicData>::data() TL_NOEXCEPT -> pointer
{
    return mData.data();
}

template<typename T>
auto Data<T, DynamicData>::data() const TL_NOEXCEPT -> const_pointer
{
    return mData.data();
}


/*! \} */

} // End namespace tl
