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

/*!
 * \file Size.h
 * \brief Two-dimensional size representation with arithmetic operations.
 *
 * This file defines the `Size` class template, which represents the dimensions
 * (width and height) of a two-dimensional object. It provides arithmetic
 * operations, type conversions, and validation methods.
 *
 * \ingroup Base
 * \see tl::Rect, tl::Point
 */

#pragma once

#include <limits>
#include <numeric>
#include <array>

#include "tidop/core/base/Exception.h"
#include "tidop/geometry/primitives/Point.h"

namespace tl
{


/*! \addtogroup Base
 *  \{
 */


/*!
 * \class Size
 * \brief Represents the width and height of a two-dimensional object.
 *
 * \tparam T Arithmetic type (e.g., `int`, `float`, `double`).
 *
 * This class stores two dimensions (width and height) and provides operations
 * for arithmetic, comparison, and validation. It is used as a building block
 * for geometric types such as `Rect`.
 *
 * ### Example
 * \code
 * Size<int> screenSize(1920, 1080);
 * Size<int> thumbSize = screenSize / 4;
 *
 * if (screenSize.isValid()) {
 *     Size<float> normalized = Size<float>(screenSize) / 1920.0f;
 * }
 * \endcode
 */
template<typename T>
class Size
{

    static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");

public:

    using value_type = T;

private:

    std::array<T, 2> mDimensions{0, 0};

public:

    /*!
     * \brief Default constructor.
     * Constructs a empty Size object. isValid() returns false
     * and isEmpty() return true.
     */
    constexpr Size() = default;

    /*!
     * \brief Constructs a size with the given width and height.
     * \param[in] width Size width
     * \param[in] height Size height
     */
    constexpr Size(T width, T height);

    /*!
     * \brief Copy constructor
     * \param[in] size Size object to copy
     */
    constexpr Size(const Size &size) = default;

    /*!
     * \brief Move constructor
     * \param[in] size Size object to move
     */
    constexpr Size(Size &&size) noexcept = default;

    ~Size() = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] size Size object to copy
     */
    constexpr auto operator = (const Size &size) -> Size& = default;

    /*!
     * \brief Move assignment operator
     * \param[in] size Size object to move
     */
    constexpr auto operator = (Size &&size) noexcept -> Size& = default;

    /*!
     * \brief Returns the width.
     * \return The width.
     */
    [[nodiscard]]
    constexpr auto width() const noexcept -> T;

    /*!
     * \brief Returns the height.
     * \return The height.
     */
    [[nodiscard]]
    constexpr auto height() const noexcept -> T;

    /*!
     * \brief Sets the width.
     * \param[in] width New width.
     */
    constexpr void setWidth(T width) noexcept;

    /*!
     * \brief Sets the height.
     * \param[in] height New height.
     */
    constexpr void setHeight(T height) noexcept;

    /*!
     * \brief Check if Size object is empty
     * \return Returns true if either of the width and height is less than or equal to 0; otherwise returns false.
     */
    [[nodiscard]]
    constexpr auto isEmpty() const noexcept -> bool;

    /*!
     * \brief Check if Size object is valid
     * \return Returns true if both the width and height is equal to or greater than 0; otherwise returns false.
     */
    [[nodiscard]]
    constexpr auto isValid() const noexcept -> bool;

    /*!
     * \brief Calculates the area of the size.
     * \return The product of width and height.
     */
    [[nodiscard]]
    constexpr auto area() const noexcept -> T;

    constexpr auto operator += (const Size<T> &other) noexcept -> Size<T> &;
    constexpr auto operator -= (const Size<T> &other) noexcept -> Size<T> &;
    constexpr auto operator *= (T scalar) noexcept -> Size &;
    constexpr auto operator /= (T scalar) -> Size &;

    template<typename T>
    [[nodiscard]]
    constexpr auto operator == (const Size<T> &other) -> bool
    {
        return (this->mDimensions == other.mDimensions);
    }

    template<typename T>
    [[nodiscard]]
    constexpr auto operator != (const Size<T> &other) -> bool
    {
        return (this->mDimensions != other.mDimensions);
    }

    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Size<T2>() const;

};


using Sizei = Size<int>;
using Sizef = Size<float>;
using Sized = Size<double>;




template<typename T>
constexpr Size<T>::Size(T width, T height)
  : mDimensions{width, height}
{
}

template<typename T>
constexpr auto Size<T>::width() const noexcept -> T
{
    return mDimensions[0];
}

template<typename T>
constexpr auto Size<T>::height() const noexcept -> T
{
    return mDimensions[1];
}

template<typename T>
constexpr void Size<T>::setWidth(T width) noexcept
{ 
    mDimensions[0] = width;
}

template<typename T>
constexpr void Size<T>::setHeight(T height) noexcept {
    mDimensions[1] = height;
}

template<typename T>
constexpr auto Size<T>::isEmpty() const noexcept -> bool
{
    return mDimensions[0] <= static_cast<T>(0) || mDimensions[1] <= static_cast<T>(0);
}

template<typename T>
constexpr auto Size<T>::isValid() const noexcept -> bool
{
    return mDimensions[0] > static_cast<T>(0) && mDimensions[1] > static_cast<T>(0);
}

template<typename T>
constexpr auto Size<T>::area() const noexcept -> T
{
    return mDimensions[0] * mDimensions[1];
}

template<typename T>
constexpr auto Size<T>::operator += (const Size<T> &other) noexcept -> Size<T> &
{
    mDimensions[0] += other.mDimensions[0];
    mDimensions[1] += other.mDimensions[1];
    return *this;
}

template<typename T>
constexpr auto Size<T>::operator -= (const Size<T> &other) noexcept -> Size<T> &
{
    mDimensions[0] -= other.mDimensions[0];
    mDimensions[1] -= other.mDimensions[1];
    return *this;
}

template<typename T>
constexpr auto Size<T>::operator *= (T scalar) noexcept -> Size<T> &
{
    mDimensions[0] *= scalar;
    mDimensions[1] *= scalar;
    return *this;
}

template<typename T>
constexpr auto Size<T>::operator /= (T scalar) -> Size<T> &
{
    TL_ASSERT(scalar != static_cast<T>(0), "Division by zero: scalar cannot be zero");

    mDimensions[0] /= scalar;
    mDimensions[1] /= scalar;
    return *this;
}

template<typename T> template<typename T2>
Size<T>::operator Size<T2>() const
{
    Size<T2> size(numberCast<T2>(this->mDimensions[0]), 
                  numberCast<T2>(this->mDimensions[1]));
    return size;
}




/*!
 * \brief Addition operator for Size objects.
 * \param[in] lhs First size
 * \param[in] rhs Second size to add
 * \return A new Size object with summed dimensions
 */
template<typename T>
[[nodiscard]]
constexpr auto operator + (Size<T> lhs, const Size<T> &rhs) noexcept -> Size<T>
{
    lhs += rhs;
    return lhs;
}

/*!
 * \brief Subtraction operator for Size objects.
 * \param[in] lhs First size
 * \param[in] rhs Second size to subtract
 * \return A new Size object with subtracted dimensions
 */
template<typename T>
[[nodiscard]]
constexpr auto operator - (Size<T> lhs, const Size<T> &rhs) noexcept -> Size<T>
{
    lhs -= rhs;
    return lhs;
}

/*!
 * \brief Scalar multiplication operator for Size objects.
 * \param[in] size The size to scale
 * \param[in] scalar The scalar value to multiply by
 * \return A new Size object with scaled dimensions
 */
template<typename T>
[[nodiscard]]
constexpr auto operator * (Size<T> size, T scalar) noexcept -> Size<T>
{
    size *= scalar;
    return size;
}

/*!
 * \brief Scalar multiplication operator for Size objects.
 * \param[in] scalar The scalar value to multiply by
 * \param[in] size The size to scale
 * \return A new Size object with scaled dimensions
 */
template<typename T>
[[nodiscard]] 
constexpr auto operator * (T scalar, Size<T> size) noexcept -> Size<T>
{
    size *= scalar;
    return size;
}

/*!
 * \brief Scalar division operator for Size objects.
 * \param[in] size The size to divide
 * \param[in] scalar The scalar value to divide by (must not be zero)
 * \return A new Size object with divided dimensions
 * \exception std::invalid_argument If scalar is zero
 */
template<typename T>
[[nodiscard]]
constexpr auto operator / (Size<T> size, T scalar) -> Size<T>
{
    size /= scalar;
    return size;
}

/*! \} */ // end of geometry

} // namespace tl
