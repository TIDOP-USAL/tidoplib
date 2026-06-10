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
#include <charconv>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/Exception.h"
#include "tidop/core/base/Concepts.h"

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
    static_assert(Arithmetic<T>, "Series requires an arithmetic type (integral or floating-point)");
    static_assert(!std::is_const_v<T>, "Series cannot hold const types. Use 'const Series<T>' instead.");

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

    std::vector<std::string> mStringIndex;
    std::vector<T> mData;
    template<typename Scalar> friend  std::ostream& operator<<(std::ostream&, const Series<Scalar>&);

public:

    /*!
     * \brief Default constructor.
     */
    Series() = default;

    /*!
     * \brief Copy constructor.
     * \param[in] series The series to copy from.
     */
    Series(const Series<T> &series) = default;

    /*!
     * \brief Move constructor.
     * \param[in] series The series to move from.
     */
    Series(Series<T> &&series) noexcept = default;

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
     * \brief Destructor.
     */
    ~Series() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] series The series to copy from.
     */
    auto operator = (const Series<T> &series) -> Series & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] series The series to move from.
     */
    auto operator = (Series<T> &&series) noexcept -> Series & = default;

    /*!
     * \brief Returns the size of the series.
     * \return The number of elements in the series.
     */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t;

    /*!
     * \brief Access element by position.
     * \param[in] pos The position of the element.
     * \return Reference to the element at the specified position.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t pos) -> reference;

    /*!
     * \brief Access element by position (const version).
     * \param[in] pos The position of the element.
     * \return Const reference to the element at the specified position.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t pos) const -> const_reference;

    /*!
     * \brief Access element by string key.
     * \param[in] idx The string key of the element.
     * \return The element associated with the given key.
     */
    [[nodiscard]]
    auto operator[](std::string_view idx) -> reference;

    [[nodiscard]]
    auto operator[](std::string_view idx) const -> const_reference;

    auto push_back(const T &value) -> void
    {
        mData.push_back(value);
    }

    void push_back(std::string_view key, T value)
    {
        mStringIndex.emplace_back(key);
        mData.push_back(value);
    }

    /*!
     * \brief Access element by string key.
     * \param idx The string key of the element.
     * \return The element associated with the given key.
     */
    [[nodiscard]]
    constexpr auto front() noexcept -> reference;

    /*!
     * \brief Returns a const reference to the first element in the series.
     * \return Const reference to the first element.
     */
    [[nodiscard]]
    constexpr auto front() const noexcept -> const_reference;

    /*!
     * \brief Returns a reference to the last element in the series.
     * \return Reference to the last element.
     */
    [[nodiscard]]
    constexpr auto back() noexcept -> reference;

    /*!
     * \brief Returns a const reference to the last element in the series.
     * \return Const reference to the last element.
     */
    [[nodiscard]]
    constexpr auto back() const noexcept -> const_reference;

    /*!
     * \brief Returns an iterator to the beginning of the series.
     * \return Iterator to the beginning.
     */
    [[nodiscard]]
    constexpr auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning of the series.
     * \return Const iterator to the beginning.
     */
    [[nodiscard]]
    constexpr auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the series.
     * \return Iterator to the end.
     */
    [[nodiscard]]
    constexpr auto end() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the end of the series.
     * \return Const iterator to the end.
     */
    [[nodiscard]]
    constexpr auto end() const noexcept -> const_iterator;

    /*!
     * \brief Returns a reverse iterator to the beginning of the reversed series.
     * \return Reverse iterator to the beginning of the reversed series.
     */
    [[nodiscard]]
    constexpr auto rbegin() noexcept -> reverse_iterator;

    /*!
     * \brief Returns a const reverse iterator to the beginning of the reversed series.
     * \return Const reverse iterator to the beginning of the reversed series.
     */
    [[nodiscard]]
    constexpr auto rbegin() const noexcept -> const_reverse_iterator;

    /*!
     * \brief Returns a reverse iterator to the end of the reversed series.
     * \return Reverse iterator to the end of the reversed series.
     */
    [[nodiscard]]
    constexpr auto rend() noexcept -> reverse_iterator;

    /*!
     * \brief Returns a const reverse iterator to the end of the reversed series.
     * \return Const reverse iterator to the end of the reversed series.
     */
    [[nodiscard]]
    constexpr auto rend() const noexcept -> const_reverse_iterator;

    [[nodiscard]]
    constexpr auto data() noexcept -> pointer { return mData.data(); }
    
    [[nodiscard]]
    constexpr auto data() const noexcept -> const_pointer { return mData.data(); }

};


template<typename T> inline
Series<T>::Series(std::initializer_list<T> data)
    : mData(data)
{
}

template<typename T> inline
Series<T>::Series(std::initializer_list<std::pair<std::string, T>> data)
{
    mStringIndex.reserve(data.size());
    mData.reserve(data.size());

    for (const auto &[key, val] : data) {
        mStringIndex.push_back(key);
        mData.push_back(val);
    }
}

template<typename T> inline
Series<T>::Series(std::initializer_list<std::pair<size_t, T>> data)
{
    mData.reserve(data.size());
    mStringIndex.reserve(data.size());

    for (const auto &[key, val] : data) {
        mStringIndex.push_back(std::to_string(key));
        mData.push_back(val);
    }
}

template<typename T> inline
constexpr auto Series<T>::size() const noexcept -> size_t
{
    return mData.size();
}

template<typename T>
constexpr auto Series<T>::operator[](size_t pos) -> reference
{
    TL_ASSERT(pos < mData.size(), "Series: Index out of bounds");
    return mData[pos];
}

template<typename T>
constexpr auto Series<T>::operator[](size_t pos) const -> const_reference
{
    TL_ASSERT(pos < mData.size(), "Series: Index out of bounds");
    return mData[pos];
}

template<typename T>
auto Series<T>::operator[](std::string_view idx) -> reference
{
    if (!mStringIndex.empty()) {
        if (auto it = std::ranges::find(mStringIndex, idx); it != mStringIndex.end()) {
            return mData[std::distance(mStringIndex.begin(), it)];
        }
    } else {
        // Si no hay etiquetas de texto, intentamos parsear el string como posición física
        size_t pos{0};
        auto [ptr, ec] = std::from_chars(idx.data(), idx.data() + idx.size(), pos);
        if (ec == std::errc{} && pos < mData.size()) {
            return mData[pos];
        }
    }
    TL_THROW_EXCEPTION("Series: Label or index not found");
}

template<typename T>
auto Series<T>::operator[](std::string_view idx) const -> const_reference
{
    if (!mStringIndex.empty()) {
        if (auto it = std::ranges::find(mStringIndex, idx); it != mStringIndex.end()) {
            return mData[std::distance(mStringIndex.begin(), it)];
        }
    } else {
        // Si no hay etiquetas de texto, intentamos parsear el string como posición física
        size_t pos{0};
        auto [ptr, ec] = std::from_chars(idx.data(), idx.data() + idx.size(), pos);
        if (ec == std::errc{} && pos < mData.size()) {
            return mData[pos];
        }
    }
    TL_THROW_EXCEPTION("Series: Label or index not found");
}

template<typename T>
constexpr auto Series<T>::front() noexcept -> reference
{
    return mData.front();
}

template<typename T>
constexpr auto Series<T>::front() const noexcept -> const_reference
{
    return mData.front();
}

template<typename T>
constexpr auto Series<T>::back() noexcept -> reference
{
    return mData.back();
}

template<typename T>
constexpr auto Series<T>::back() const noexcept -> const_reference
{
    return mData.back();
}

template<typename T>
constexpr auto Series<T>::begin() noexcept -> iterator
{
    return mData.begin();
}

template<typename T>
constexpr auto Series<T>::begin() const noexcept -> const_iterator
{
    return mData.begin();
}

template<typename T>
constexpr auto Series<T>::end() noexcept -> iterator
{
    return mData.end();
}

template<typename T>
constexpr auto Series<T>::end() const noexcept -> const_iterator
{
    return mData.end();
}

template<typename T>
constexpr auto Series<T>::rbegin() noexcept -> reverse_iterator
{
    return mData.rbegin();
}

template<typename T>
constexpr auto Series<T>::rbegin() const noexcept -> const_reverse_iterator
{
    return mData.rbegin();
}

template<typename T>
constexpr auto Series<T>::rend() noexcept -> reverse_iterator
{
    return mData.rend();
}

template<typename T>
constexpr auto Series<T>::rend() const noexcept -> const_reverse_iterator
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
        if (!serie.mStringIndex.empty())
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



