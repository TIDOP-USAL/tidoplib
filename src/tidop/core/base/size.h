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
 * \file size.h
 * \brief Two-dimensional size representation with arithmetic operations
 *
 * This module provides a generic size class for representing the dimensions of two-dimensional objects.
 * It supports various arithmetic operations, type conversions, and validation methods.
 *
 * ### Classes
 *
 * - \ref Size<T> - Generic size implementation for type T
 *
 * ### Features
 *
 * - Type-safe size storage with arithmetic type validation
 * - Arithmetic operations (addition, subtraction, scaling)
 * - Type conversion between compatible size types
 * - Dimension validation (positive, valid, empty checks)
 * - Constexpr support for compile-time evaluation
 * - Support for all arithmetic types (int, float, double, etc.)
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * #include "tidop/core/base/size.h"
 *
 * tl::Sizei screenSize(1920, 1080);
 * tl::Sizei thumbnailSize = screenSize / 4;
 *
 * if (screenSize.isValid()) {
 *     tl::Sizef normalized = static_cast<tl::Sizef>(screenSize) / 1920.0f;
 * }
 *
 * tl::Sizei doubleSize = screenSize * 2;
 * \endcode
 *
 * \see tl::Sizei, tl::Sizef, tl::Sized
 */

#pragma once

#include <limits>
#include <numeric>

#include "tidop/core/base/exception.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/window.h"

namespace tl
{


/*! \addtogroup Base
 *  \{
 */


/*!
 * \brief Class for specifying the size of a two-dimensional object.
 *
 */
template<typename T>
class Size
{

public:

    using value_type = T;

public:

    T width;
    T height;

public:

    /*!
     * \brief Default constructor.
     * Constructs a empty Size object. isValid() returns false
     * and isEmpty() return true.
     */
    constexpr Size();

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
    Size(const Size &size);

    /*!
     * \brief Move constructor
     * \param[in] size Size object to move
     */
    Size(Size &&size) TL_NOEXCEPT;

    ~Size() = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] size Size object to copy
     */
    auto operator = (const Size &size) -> Size&;

    /*!
     * \brief Move assignment operator
     * \param[in] size Size object to move
     */
    auto operator = (Size &&size) TL_NOEXCEPT -> Size&;

    /*!
     * \brief Check if Size object is empty
     * \return Returns true if either of the width and height is less than or equal to 0; otherwise returns false.
     */
    constexpr auto isEmpty() const -> bool;

    /*!
     * \brief Check if Size object is valid
     * \return Returns true if both the width and height is equal to or greater than 0; otherwise returns false.
     */
    constexpr auto isValid() const -> bool;

    /*!
     * \brief Calculates the area of the size.
     * \return The product of width and height.
     */
    constexpr auto area() const -> T;

    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Size<T2>() const;

};


using Sizei = Size<int>;
using Sizef = Size<float>;
using Sized = Size<double>;



template<typename T>
constexpr Size<T>::Size()
  : width{0},
    height{0}
{
    static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
}

template<typename T>
constexpr Size<T>::Size(T width, T height)
  : width(width),
    height(height)
{
    static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
}

template<typename T>
Size<T>::Size(const Size &size)
  : width(size.width),
    height(size.height)
{
    static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
}

template<typename T>
Size<T>::Size(Size &&size) TL_NOEXCEPT
  : width(size.width),
    height(size.height)
{
}

template<typename T>
auto Size<T>::operator = (const Size &size) -> Size<T>&
{
    if (this != &size) {
        this->width = size.width;
        this->height = size.height;
    }

    return *this;
}

template<typename T>
auto Size<T>::operator = (Size &&size) TL_NOEXCEPT -> Size<T>&
{
    if (this != &size) {
        this->width = size.width;
        this->height = size.height;
    }

    return *this;
}

template<typename T>
constexpr auto Size<T>::isEmpty() const -> bool
{
    return width <= static_cast<T>(0) || height <= static_cast<T>(0);
}

template<typename T>
constexpr auto Size<T>::isValid() const -> bool
{
    return width > static_cast<T>(0) && height > static_cast<T>(0);
}

template<typename T>
constexpr auto Size<T>::area() const -> T
{
    return width * height;
}

template<typename T> template<typename T2>
Size<T>::operator Size<T2>() const
{
    Size<T2> size;

    size.width = numberCast<T2>(this->width);
    size.height = numberCast<T2>(this->height);

    return size;
}



/*!
 * \brief Equality comparison operator for Size objects.
 * \param[in] size1 First size to compare
 * \param[in] size2 Second size to compare
 * \return True if both width and height are equal; otherwise false.
 */
template<typename T>
bool operator == (const Size<T> &size1, const Size<T> &size2)
{
    return (size1.width == size2.width &&
            size1.height == size2.height);
}

/*!
 * \brief Inequality comparison operator for Size objects.
 * \param[in] size1 First size to compare
 * \param[in] size2 Second size to compare
 * \return True if either width or height differs; otherwise false.
 */
template<typename T>
bool operator != (const Size<T> &size1, const Size<T> &size2)
{
    return (size1.width != size2.width ||
            size1.height != size2.height);
}

/*!
 * \brief Addition assignment operator for Size objects.
 * \param[in,out] size1 First size to modify
 * \param[in] size2 Second size to add
 * \return Reference to the modified size1
 */
template<typename T>
Size<T> &operator += (Size<T> &size1, const Size<T> &size2)
{
    size1.width += size2.width;
    size1.height += size2.height;

    return size1;
}

/*!
 * \brief Subtraction assignment operator for Size objects.
 * \param[in,out] size1 First size to modify
 * \param[in] size2 Second size to subtract
 * \return Reference to the modified size1
 */
template<typename T>
Size<T> &operator -= (Size<T> &size1, const Size<T> &size2)
{
    size1.width -= size2.width;
    size1.height -= size2.height;

    return size1;
}

/*!
 * \brief Addition operator for Size objects.
 * \param[in] size1 First size
 * \param[in] size2 Second size to add
 * \return A new Size object with summed dimensions
 */
template<typename T>
Size<T> operator + (const Size<T> &size1, const Size<T> &size2)
{
    return Size<T>(size1.width + size2.width,
                   size1.height + size2.height);
}

/*!
 * \brief Subtraction operator for Size objects.
 * \param[in] size1 First size
 * \param[in] size2 Second size to subtract
 * \return A new Size object with subtracted dimensions
 */
template<typename T>
Size<T> operator - (const Size<T> &size1, const Size<T> &size2)
{
    return Size<T>(size1.width - size2.width,
                   size1.height - size2.height);
}

/*!
 * \brief Scalar multiplication operator for Size objects.
 * \param[in] size The size to scale
 * \param[in] scalar The scalar value to multiply by
 * \return A new Size object with scaled dimensions
 */
template<typename T>
Size<T> operator * (const Size<T> &size, T scalar)
{
    return Size<T>(size.width * scalar,
                   size.height * scalar);
}

/*!
 * \brief Scalar multiplication assignment operator for Size objects.
 * \param[in,out] size The size to modify
 * \param[in] scalar The scalar value to multiply by
 * \return Reference to the modified size
 */
template<typename T>
Size<T> &operator *= (Size<T> &size, T scalar)
{
    size.width *= scalar;
    size.height *= scalar;

    return size;
}

/*!
 * \brief Scalar division assignment operator for Size objects.
 * \param[in,out] size The size to modify
 * \param[in] scalar The scalar value to divide by (must not be zero)
 * \return Reference to the modified size
 * \exception std::invalid_argument If scalar is zero
 */
template<typename T>
Size<T> &operator /= (Size<T> &size, T scalar)
{
    if (scalar == static_cast<T>(0)) {
        TL_THROW_EXCEPTION("Division by zero: scalar cannot be zero");
    }
    
    size.width /= scalar;
    size.height /= scalar;

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
Size<T> operator / (const Size<T> &size, T scalar)
{
    if (scalar == static_cast<T>(0)) {
        TL_THROW_EXCEPTION("Division by zero: scalar cannot be zero");
    }
    
    return Size<T>(size.width / scalar,
                   size.height / scalar);
}

/*! \} */ // end of geometry

} // End namespace tl
