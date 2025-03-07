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

#include <tidop/core/base/defs.h>
#include <tidop/core/base/exception.h>

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */


/*!
 * \brief A class template to represent a series of data with optional string or numeric keys.
 *
 * The `Series` class allows for storage and access to a series of values indexed by integers, 
 * strings, or a combination of both. This provides flexibility in accessing elements 
 * through their position or a user-defined key.
 *
 * \tparam T The type of elements stored in the series.
 *
 * ### Example Usage
 *
 * Example with integer indices:
 * \code{.cpp}
 * Series<int> s{1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
 * // s[0] returns 1, s[1] returns 2, and so on.
 * \endcode
 *
 * Example with string keys:
 * \code{.cpp}
 * Series<double> s2{{"A", 2.0}, {"B", 5.0}}; 
 * // s2[0] and s2["A"] return 2.0.
 * // s2[1] and s2["B"] return 5.0.
 * \endcode
 *
 * Example with numeric keys:
 * \code{.cpp}
 * Series<double> s3{{3, 2.0}, {7, 3.0}};
 * // s3[3] and s3["3"] return 2.0.
 * // s3[7] and s3["7"] return 3.0.
 * \endcode
 *
 * Example usage with `DescriptiveStatistics`:
 * \code{.cpp}
 * Series<double> s_1{8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};
 * DescriptiveStatistics<double> stat_1(s_1);
 * auto min = stat_1.min(); // 5.5
 * auto max = stat_1.max(); // 9.0
 * auto sum = stat_1.sum(); // 69.25
 * auto mean = stat_1.mean(); // 7.695
 * \endcode
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

    /*!
     * \brief Default constructor.
     */
    Series() = default;

    /*!
     * \brief Constructs a series from an initializer list of values.
     * \param[in] data The list of values to initialize the series with.
     */
    Series(std::initializer_list<T> data);

    /*!
     * \brief Constructs a series from an initializer list of string-value pairs.
     * \param[in] data The list of string-value pairs to initialize the series with.
     */
    Series(std::initializer_list<std::pair<std::string, T>> data);

    /*!
     * \brief Constructs a series from an initializer list of size_t-value pairs.
     * \param[in] data The list of size_t-value pairs to initialize the series with.
     */
    Series(std::initializer_list<std::pair<size_t, T>> data);

    /*!
     * \brief Copy constructor.
     * \param[in] series The series to copy from.
     */
    Series(const Series<T> &series);

    /*!
     * \brief Move constructor.
     * \param[in] series The series to move from.
     */
    Series(Series<T> &&series) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~Series();

    /*!
     * \brief Copy assignment operator.
     * \param[in] series The series to copy from.
     */
    auto operator = (const Series<T> &series) -> Series &;

    /*!
     * \brief Move assignment operator.
     * \param[in] series The series to move from.
     */
    auto operator = (Series<T> &&series) TL_NOEXCEPT -> Series &;

    /*!
     * \brief Returns the size of the series.
     * \return The number of elements in the series.
     */
    auto size() const -> size_t;

    /*!
     * \brief Access element by index.
     * \param[in] idx The index of the element.
     * \return Reference to the element at the specified index.
     */
    auto operator[](size_t idx) -> reference;

    /*!
     * \brief Access element by index (const version).
     * \param[in] idx The index of the element.
     * \return Const reference to the element at the specified index.
     */
    auto operator[](size_t idx) const -> const_reference;

    /*!
     * \brief Access element by string key.
     * \param[in] idx The string key of the element.
     * \return The element associated with the given key.
     */
    auto operator[](const std::string &idx) -> T;

    //void setData(std::initializer_list<T> data);
    //void setData(std::initializer_list<std::pair<std::string, T>> data);
    //void setData(std::initializer_list<std::pair<size_t, T>> data);

    /*!
     * \brief Access element by string key.
     * \param idx The string key of the element.
     * \return The element associated with the given key.
     */
    auto front() -> reference;

    /*!
     * \brief Returns a const reference to the first element in the series.
     * \return Const reference to the first element.
     */
    auto front() const -> const_reference;

    /*!
     * \brief Returns a reference to the last element in the series.
     * \return Reference to the last element.
     */
    auto back()  -> reference;

    /*!
     * \brief Returns a const reference to the last element in the series.
     * \return Const reference to the last element.
     */
    auto back() const -> const_reference;

    /*!
     * \brief Returns an iterator to the beginning of the series.
     * \return Iterator to the beginning.
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning of the series.
     * \return Const iterator to the beginning.
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the series.
     * \return Iterator to the end.
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the end of the series.
     * \return Const iterator to the end.
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns a reverse iterator to the beginning of the reversed series.
     * \return Reverse iterator to the beginning of the reversed series.
     */
    auto rbegin() TL_NOEXCEPT -> reverse_iterator;

    /*!
     * \brief Returns a const reverse iterator to the beginning of the reversed series.
     * \return Const reverse iterator to the beginning of the reversed series.
     */
    auto rbegin() const TL_NOEXCEPT -> const_reverse_iterator;

    /*!
     * \brief Returns a reverse iterator to the end of the reversed series.
     * \return Reverse iterator to the end of the reversed series.
     */
    auto rend() TL_NOEXCEPT -> reverse_iterator;

    /*!
     * \brief Returns a const reverse iterator to the end of the reversed series.
     * \return Const reverse iterator to the end of the reversed series.
     */
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

/*! \} */

} // End namespace tl



