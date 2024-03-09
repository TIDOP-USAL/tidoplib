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

/*! \addtogroup math
 *  \{
 */

template<typename T, size_t _rows, size_t _cols = 1>
class DataSize
{

public:

    static constexpr size_t size()
    {
        return _rows == DynamicData ? DynamicData : _rows * _cols;
    }

};


/*!
 * \brief Static data storage
 */
template<typename T, size_t _size>
class Data
{

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = typename std::array<T, _size>::iterator;
    using const_iterator = typename std::array<T, _size>::const_iterator;

public:

    Data();
    Data(size_t size);
    Data(size_t size, T value);
    Data(const Data &matrix);
    Data(Data &&matrix) TL_NOEXCEPT;
    Data(std::initializer_list<T> values);
    explicit Data(const T *data, size_t size = 0);
    virtual ~Data() = default;

    auto operator = (const Data &matrix) -> Data &;
    auto operator = (Data &&matrix) TL_NOEXCEPT -> Data &;

    auto at(size_t position) -> reference;
    auto at(size_t position) const -> const_reference;
    auto operator[](size_t position) -> reference;
    auto operator[](size_t position) const->const_reference;

    void operator=(T value);

    auto front() -> reference;
    auto front() const->const_reference;
    auto back() -> reference;
    auto back() const -> const_reference;
    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;

    auto data() -> pointer;
    auto data() const -> const_pointer;

    size_t size() const { return mData.size(); }

private:

    std::array<T, _size> mData;

};



/*------------------------------------------------------------------------*/


/*!
 * \brief Dynamic data storage
 */
template<typename T>
class Data<T, DynamicData>
{

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

public:

    Data() = default;
    Data(size_t size);
    Data(size_t size, T val);
    Data(const Data &matrix);
    Data(Data &&matrix) TL_NOEXCEPT;
    Data(std::initializer_list<T> values);
    Data(const T *data, size_t size);
    virtual ~Data() = default;

    auto operator = (const Data &matrix)->Data &;
    auto operator = (Data &&matrix) TL_NOEXCEPT->Data &;

    auto at(size_t position) -> reference;
    auto at(size_t position) const -> const_reference;
    auto operator[](size_t position) -> reference;
    auto operator[](size_t position) const -> const_reference;

    void operator=(T value);

    auto front() -> reference;
    auto front() const -> const_reference;
    auto back() -> reference;
    auto back() const -> const_reference;
    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;

    auto data() -> pointer;
    auto data() const -> const_pointer;

    size_t size() const { return mData.size(); }

private:

    std::vector<T> mData;

};



/*------------------------------------------------------------------------*/
/* Implementation                                                         */
/*------------------------------------------------------------------------*/


/* Static Data implementation */

template<typename T, size_t _size>
inline Data<T, _size>::Data()
  : mData()
{
    this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size>
inline Data<T, _size>::Data(size_t  /*size*/)
  : mData()
{
    this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size>
inline Data<T, _size>::Data(size_t  /*size*/, T val)
  : mData()
{
    this->mData.fill(val);
}

template<typename T, size_t _size>
inline Data<T, _size>::Data(const Data &matrix)
  : mData(matrix.mData)
{
}

template<typename T, size_t _size>
inline Data<T, _size>::Data(Data &&matrix) TL_NOEXCEPT
    : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _size>
inline Data<T, _size>::Data(std::initializer_list<T> values)
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
inline Data<T, _size>::Data(const T *data,
                            size_t /*size*/)
  : mData(data)
{
}

template<typename T, size_t _size>
inline auto Data<T, _size>::operator = (const Data &matrix) -> Data &
{
    if (this != &matrix) {
        this->mData = matrix.mData;
    }
    return *this;
}

template<typename T, size_t _size>
inline auto Data<T, _size>::operator = (Data &&matrix) TL_NOEXCEPT -> Data &
{
    if (this != &matrix) {
        this->mData = std::move(matrix.mData);
    }
    return *this;
}

template<typename T, size_t _size>
inline auto Data<T, _size>::at(size_t position) -> reference
{
    return mData.at(position);
}

template<typename T, size_t _size>
inline auto Data<T, _size>::at(size_t position) const -> const_reference
{
    return mData.at(position);
}

template<typename T, size_t _size>
inline auto Data<T, _size>::operator[](size_t position) -> reference
{
    return mData[position];
}

template<typename T, size_t _size>
inline auto Data<T, _size>::operator[](size_t position) const -> const_reference
{
    return mData[position];
}

template<typename T, size_t _size>
inline void Data<T, _size>::operator=(T value)
{
    mData.fill(value);
}

template<typename T, size_t _size>
inline auto Data<T, _size>::front() -> reference
{
    return mData.front();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::front() const -> const_reference
{
    return mData.front();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::back() -> reference
{
    return mData.back();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::back() const -> const_reference
{
    return mData.back();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::begin() TL_NOEXCEPT -> iterator
{
    return mData.begin();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::begin() const TL_NOEXCEPT -> const_iterator
{
    return mData.begin();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::end() TL_NOEXCEPT -> iterator
{
    return mData.end();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::end() const TL_NOEXCEPT -> const_iterator
{
    return mData.end();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::data() -> pointer
{
    return mData.data();
}

template<typename T, size_t _size>
inline auto Data<T, _size>::data() const -> const_pointer
{
    return mData.data();
}



/* Dynamic Data implementation */

template<typename T>
inline Data<T, DynamicData>::Data(size_t size)
  : mData(size, -std::numeric_limits<T>().max())
{
}

template<typename T>
inline Data<T, DynamicData>::Data(size_t size, T val)
  : mData(size, val)
{
}

template<typename T>
inline Data<T, DynamicData>::Data(const Data &matrix)
  : mData(matrix.mData)
{
}

template<typename T>
inline Data<T, DynamicData>::Data(Data &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))
{
}

template<typename T>
inline Data<T, DynamicData>::Data(std::initializer_list<T> values)
{
    mData.resize(values.size());
    std::copy(values.begin(), values.end(), mData.begin());
}

template<typename T>
inline Data<T, DynamicData>::Data(const T *data, size_t size)
    : mData(size, -std::numeric_limits<T>().max())
{
    mData.assign(data, data + mData.size());
}

template<typename T>
inline auto Data<T, DynamicData>::operator = (const Data<T, DynamicData> &matrix) -> Data &
{
    if (this != &matrix) {
        this->mData = matrix.mData;
    }
    return *this;
}

template<typename T>
inline auto Data<T, DynamicData>::operator = (Data<T, DynamicData> &&matrix) TL_NOEXCEPT -> Data &
{
    if (this != &matrix) {
        this->mData = std::move(matrix.mData);
    }
    return *this;
}

template<typename T>
inline auto Data<T, DynamicData>::at(size_t position) -> reference
{
    return mData.at(position);
}

template<typename T>
inline auto Data<T, DynamicData>::at(size_t position) const -> const_reference
{
    return mData.at(position);
}

template<typename T>
inline auto Data<T, DynamicData>::operator[](size_t position) -> reference
{
    return mData[position];
}

template<typename T>
inline auto Data<T, DynamicData>::operator[](size_t position) const -> const_reference
{
    return mData[position];
}

template<typename T>
inline void Data<T, DynamicData>::operator=(T value)
{
    std::fill(mData.begin(), mData.end(), value);
}

template<typename T>
inline auto Data<T, DynamicData>::front() -> reference
{
    return mData.front();
}

template<typename T>
inline auto Data<T, DynamicData>::front() const-> const_reference
{
    return mData.front();
}

template<typename T>
inline auto Data<T, DynamicData>::back() -> reference
{
    return mData.back();
}

template<typename T>
inline auto Data<T, DynamicData>::back() const -> const_reference
{
    return mData.back();
}

template<typename T>
inline auto Data<T, DynamicData>::begin() TL_NOEXCEPT  -> iterator
{
    return mData.begin();
}

template<typename T>
inline auto Data<T, DynamicData>::begin() const TL_NOEXCEPT-> const_iterator
{
    return mData.begin();
}

template<typename T>
inline auto Data<T, DynamicData>::end() TL_NOEXCEPT -> iterator
{
    return mData.end();
}

template<typename T>
inline auto Data<T, DynamicData>::end() const TL_NOEXCEPT -> const_iterator
{
    return mData.end();
}

template<typename T>
inline auto Data<T, DynamicData>::data() -> pointer
{
    return mData.data();
}

template<typename T>
inline auto Data<T, DynamicData>::data() const -> const_pointer
{
    return mData.data();
}


/*! \} */ // end of math

} // End namespace tl
