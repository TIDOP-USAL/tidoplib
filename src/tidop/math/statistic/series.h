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
#include <string> 
#include <iomanip>

#include <tidop/core/defs.h>
#include <tidop/core/exception.h>

namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \defgroup statistics Statistics
 *  \{
 */

template<typename T>
class Series
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
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

private:

    std::vector<size_t> mIndex;
    std::vector<std::string> mStringIndex;
    std::vector<T> mData;
    template<typename Scalar> friend  std::ostream& operator<<(std::ostream&, const Series<Scalar>&);

public:

    Series() = default;

    /*!
     * \brief Series
     * \param data
     */
    Series(std::initializer_list<T> data);

    /*!
     * \brief Series
     * \param[in] data
     */
    Series(std::initializer_list<std::pair<std::string, T>> data);

    /*!
     * \brief Series
     * \param[in] data
     */
    Series(std::initializer_list<std::pair<size_t, T>> data);

    /*!
     * \brief Copy constructor
     * \param[in] series Series object to copy
     */
    Series(const Series<T> &series);

    /*!
     * \brief Move constructor
     * \param[in] series Series object to move
     */
    Series(Series<T> &&series) TL_NOEXCEPT;

    ~Series();


    /*!
     * \brief Copy assignment operator
     * \param[in] series Series object to copy
     */
    auto operator = (const Series<T> &series) -> Series &;

    /*!
     * \brief Move assignment operator
     * \param[in] series Series object to move
     */
    auto operator = (Series<T> &&series) TL_NOEXCEPT -> Series &;

    auto size() const -> size_t;

    auto operator[](size_t idx) -> reference;
    auto operator[](size_t idx) const -> const_reference;
    auto operator[](const std::string &idx) -> T;

    //void setData(std::initializer_list<T> data);
    //void setData(std::initializer_list<std::pair<std::string, T>> data);
    //void setData(std::initializer_list<std::pair<size_t, T>> data);

    auto front() -> reference;
    auto front() const -> const_reference;
    auto back()  -> reference;
    auto back() const -> const_reference;
    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;
    auto rbegin() TL_NOEXCEPT -> reverse_iterator;
    auto rbegin() const TL_NOEXCEPT -> const_reverse_iterator;
    auto rend() TL_NOEXCEPT -> reverse_iterator;
    auto rend() const TL_NOEXCEPT -> const_reverse_iterator;

};


template<typename T> inline
Series<T>::Series(std::initializer_list<T> data)
    : mData(data)
{
}

template<typename T> inline
Series<T>::Series(std::initializer_list<std::pair<std::string, T>> data)
{
    TL_TODO("En c++ 17 se puede utilizar tuple")

    size_t n = data.size();
    mStringIndex.reserve(n);
    mData.reserve(n);
    for (const auto &pair : data) {
        mStringIndex.push_back(pair.first);
        mData.push_back(pair.second);
    }
}

template<typename T> inline
Series<T>::Series(std::initializer_list<std::pair<size_t, T>> data)
{
    TL_TODO("En c++ 17 se puede utilizar tuple")

    size_t n = data.size();
    mIndex.reserve(n);
    mData.reserve(n);

    for (const auto &pair : data) {
        mIndex.push_back(pair.first);
        mData.push_back(pair.second);
    }
}

template<typename T> inline
Series<T>::Series(const Series<T> &series)
  : mIndex(series.mIndex),
    mStringIndex(series.mStringIndex),
    mData(series.mData)
{
}

template<typename T> inline
Series<T>::Series(Series<T> &&series) TL_NOEXCEPT
  : mIndex(std::forward<std::vector<size_t>>(series.mIndex)),
    mStringIndex(std::forward<std::vector<std::string>>(series.mStringIndex)),
    mData(std::forward<std::vector<T>>(series.mData))
{
}

template<typename T> inline
Series<T>::~Series()
{
}

template<typename T> inline
auto Series<T>::operator = (const Series &series) -> Series&
{
    if (this != &series) {
        this->mIndex = series.mIndex;
        this->mStringIndex = series.mStringIndex;
        this->mData = series.mData;
    }
    return *this;
}

template<typename T> inline
auto Series<T>::operator = (Series &&series) TL_NOEXCEPT -> Series&
{
    if (this != &series) {
        this->mIndex = std::forward<std::vector<size_t>>(series.mIndex);
        this->mStringIndex = std::forward<std::vector<std::string>>(series.mStringIndex);
        this->mData = std::forward<std::vector<T>>(series.mData);
    }
    return *this;
}


template<typename T> inline
auto Series<T>::size() const -> size_t
{
    return mData.size();
}

template<typename T>
auto Series<T>::operator[](size_t idx) -> reference
{
    if (mIndex.empty()) {
        return mData[idx];
    } else {
        for (size_t i = 0; i < mIndex.size(); i++) {
            if (idx == mIndex[i]) {
                return mData[i];
            }
        }
    }

    TL_THROW_EXCEPTION("");
}

template<typename T>
auto Series<T>::operator[](size_t idx) const -> const_reference
{
    if (mIndex.empty()) {
        return mData[idx];
    } else {
        for (size_t i = 0; i < mIndex.size(); i++) {
            if (idx == mIndex[i]) {
                return mData[i];
            }
        }
    }

    TL_THROW_EXCEPTION("");
}

template<typename T>
auto Series<T>::operator[](const std::string &idx) -> T
{
    T value{};

    if (mStringIndex.empty()) {

        try {

            size_t integer_idx = std::stoull(idx);
            if (mIndex.empty()) {
                value = mData[integer_idx];
            } else {
                for (size_t i = 0; i < mIndex.size(); i++) {
                    if (integer_idx == mIndex[i]) {
                        value = mData[i];
                        break;
                    }
                }
            }

        } catch (const std::invalid_argument &e) {
            printException(e);
        }

    } else {

        for (size_t i = 0; i < mStringIndex.size(); i++) {
            if (mStringIndex[i] == idx) {
                value = mData[i];
                break;
            }
        }
        TL_TODO("Devolver excepción si no se encuentra el indice??")
    }

    return value;
}

//template<typename T> inline
//void Series<T>::setData(std::initializer_list<T> data)
//{
//  mData = data;
//}
//
//template<typename T> inline
//void Series<T>::setData(std::initializer_list<std::pair<std::string, T>> data)
//{ 
//  size_t n = data.size();
//  mStringIndex.reserve(n);
//  mData.reserve(n);
//  for (auto it = data.begin(); it != data.end(); it++) {
//    mStringIndex.push_back(it->first);
//    mData.push_back(it->second);
//  }
//}
//
//template<typename T> inline
//void Series<T>::setData(std::initializer_list<std::pair<size_t, T>> data)
//{
//  size_t n = data.size();
//  mIndex.reserve(n);
//  mData.reserve(n);
//  for (auto it = data.begin(); it != data.end(); it++) {
//    mIndex.push_back(it->first);
//    mData.push_back(it->second);
//  }
//}

template<typename T> inline
auto Series<T>::front() -> reference
{
    return mData.front();
}

template<typename T> inline
auto Series<T>::front() const -> const_reference
{
    return mData.front();
}

template<typename T> inline
auto Series<T>::back() -> reference
{
    return mData.back();
}

template<typename T> inline
auto Series<T>::back() const -> const_reference
{
    return mData.back();
}

template<typename T> inline
auto Series<T>::begin() TL_NOEXCEPT -> iterator
{
    return mData.begin();
}

template<typename T> inline
auto Series<T>::begin() const TL_NOEXCEPT -> const_iterator
{
    return mData.begin();
}

template<typename T> inline
auto Series<T>::end() TL_NOEXCEPT -> iterator
{
    return mData.end();
}

template<typename T> inline
auto Series<T>::end() const TL_NOEXCEPT -> const_iterator
{
    return mData.end();
}

template<typename T> inline
auto Series<T>::rbegin() TL_NOEXCEPT -> reverse_iterator
{
    return mData.rbegin();
}

template<typename T> inline
auto Series<T>::rbegin() const TL_NOEXCEPT -> const_reverse_iterator
{
    return mData.rbegin();
}

template<typename T> inline
auto Series<T>::rend() TL_NOEXCEPT -> reverse_iterator
{
    return mData.rend();
}

template<typename T> inline
auto Series<T>::rend() const TL_NOEXCEPT -> const_reverse_iterator
{
    return mData.rend();
}


/* Print */

template<typename Scalar>
std::ostream &operator<< (std::ostream &os, const Series<Scalar> &serie)
{
    os << "Index           Value\n";
    //os << "---------------------\n";
    for (size_t i = 0; i < serie.mData.size(); i++) {
        os << std::left << std::setw(16);
        if (!serie.mIndex.empty())
            os << serie.mIndex[i];
        else if (!serie.mStringIndex.empty())
            os << serie.mStringIndex[i];
        else
            os << i;
        os << serie.mData[i] << std::endl;
    }
    //os << "---------------------" << std::endl;;
    return os;
}

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl



