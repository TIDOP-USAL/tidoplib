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

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/vector/detail/Assign.h"

namespace tl
{

/*! \addtogroup Vector
 *  \{
 */


template<typename T>
class VectorView
  : public VectorBase<VectorView<T>>
{
private:

    T *mData;
    size_t mIni;
    size_t mSize;

public:

    using value_type = std::remove_cv_t<T>;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = T *;
    using const_iterator = const T *;

public:


    constexpr VectorView(T *data, size_t ini, size_t size);

    template<VectorExpr Expr>
    constexpr auto operator=(const Expr &expr) -> VectorView &;

    /*!
     * \brief Returns an iterator to the first element of the row.
     */
    [[nodiscard]]
    constexpr auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the first element.
     */
    [[nodiscard]]
    constexpr auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to one past the last element.
     */
    [[nodiscard]]
    constexpr auto end() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to one past the last element.
     */
    [[nodiscard]]
    constexpr auto end() const noexcept -> const_iterator;

    /*!
     * \brief Returns the number of elements in the vector
     */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t;

    /*!
     * \brief Fills the entire row with a given value.
     * \param[in] value Value to assign to each element.
     */
    constexpr void fill(T value);

    [[nodiscard]]
    constexpr auto operator[](size_t pos) const -> const_reference;
    [[nodiscard]]
    constexpr auto operator[](size_t pos) -> reference;

    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool;

#ifdef TL_HAVE_SIMD_INTRINSICS
    auto packet(size_t i) const noexcept -> Packed<value_type>;
#endif
};


/* VectorView implementation */

template<typename T>
constexpr VectorView<T>::VectorView(T *data, size_t ini, size_t size)
  : mData(data),
    mIni(ini),
    mSize(size)
{
}

template<typename T>
template<VectorExpr Expr>
constexpr auto VectorView<T>::operator=(const Expr &expr) -> VectorView &
{
    detail::assign_vector_view(*this, expr);
    return *this;
}

template<typename T>
constexpr auto VectorView<T>::begin() noexcept -> iterator
{
    return &mData[mIni];
}

template<typename T>
constexpr auto VectorView<T>::begin() const noexcept -> const_iterator
{
    return &mData[mIni];
}

template<typename T>
constexpr auto VectorView<T>::end() noexcept -> iterator
{
    return &mData[mIni] + mSize;
}

template<typename T>
constexpr auto VectorView<T>::end() const noexcept -> const_iterator
{
  return &mData[mIni] + mSize;
}

template<typename T>
constexpr auto VectorView<T>::size() const noexcept -> size_t
{
    return mSize;
}

template<typename T>
constexpr void VectorView<T>::fill(T value)
{
    std::fill(begin(), end(), value);
}

template<typename T>
constexpr auto VectorView<T>::operator[](size_t pos) const -> const_reference
{
    return mData[mIni + pos];
}

template<typename T>
constexpr auto VectorView<T>::operator[](size_t pos) -> reference
{
    return mData[mIni + pos];
}

template<typename T>
constexpr auto VectorView<T>::aliases(const void *ptr) const -> bool
{
    return mData == ptr;
}

#ifdef TL_HAVE_SIMD_INTRINSICS
template<typename T>
auto VectorView<T>::packet(size_t i) const noexcept -> Packed<value_type>
{
    Packed<value_type> p;
    p.loadUnaligned(&mData[mIni + i]);
    return p;
}
#endif

/*! \} */

} // End namespace tl