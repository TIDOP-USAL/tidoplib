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

#include <limits>
#include <numeric>

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/window.h"

namespace tl
{


/*! \addtogroup geometry
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

    /*!
     * \brief Default constructor.
     * Constructs a empty Size object. isValid() returns false
     * and isEmpty() return true.
     */
    Size();

    /*!
     * \brief Constructs a size with the given width and height.
     * \param[in] width Size width
     * \param[in] height Size height
     */
    Size(T width, T height);

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
    auto isEmpty() const -> bool;

    /*!
     * \brief Check if Size object is valid
     * \return Returns true if both the width and height is equal to or greater than 0; otherwise returns false.
     */
    auto isValid() const -> bool;

    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Size<T2>() const;

public:

    T width;
    T height;
};


using Sizei = Size<int>;
using Sizef = Size<float>;
using Sized = Size<double>;



template<typename T> inline
Size<T>::Size()
  : width{0},
    height{0}
{
}

template<typename T> inline
Size<T>::Size(T width, T height)
  : width(width),
    height(height)
{
}

template<typename T> inline
Size<T>::Size(const Size &size)
  : width(size.width),
    height(size.height)
{
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
auto Size<T>::isEmpty() const -> bool
{
    return width <= static_cast<T>(0) || height <= static_cast<T>(0);
}

template<typename T>
auto Size<T>::isValid() const -> bool
{
    return width > static_cast<T>(0) && height > static_cast<T>(0);
}

template<typename T> template<typename T2>
Size<T>::operator Size<T2>() const
{
    Size<T2> size;

    size.width = numberCast<T2>(this->width);
    size.height = numberCast<T2>(this->height);

    return size;
}

template<typename T>
bool operator == (const Size<T> &size1, const Size<T> &size2)
{
    return (size1.width == size2.width &&
            size1.height == size2.height);
}

template<typename T>
bool operator != (const Size<T> &size1, const Size<T> &size2)
{
    return (size1.width != size2.width ||
            size1.height != size2.height);
}

template<typename T>
Size<T> &operator += (Size<T> &size1, const Size<T> &size2)
{
    size1.width += size2.width;
    size1.height += size2.height;

    return size1;
}

template<typename T>
Size<T> &operator -= (Size<T> &size1, const Size<T> &size2)
{
    size1.width -= size2.width;
    size1.height -= size2.height;

    return size1;
}

template<typename T>
Size<T> operator + (const Size<T> &size1, const Size<T> &size2)
{
    return Size<T>(size1.width + size2.width,
                   size1.height + size2.height);
}

template<typename T>
Size<T> operator - (const Size<T> &size1, const Size<T> &size2)
{
    return Size<T>(size1.width - size2.width,
                   size1.height - size2.height);
}

template<typename T>
Size<T> operator * (const Size<T> &size, T scalar)
{
    return Size<T>(size.width * scalar,
                   size.height * scalar);
}

template<typename T>
Size<T> &operator *= (Size<T> &size, T scalar)
{
    size.width *= scalar;
    size.height *= scalar;

    return size;
}

template<typename T>
Size<T> &operator /= (Size<T> &size, T scalar)
{
    size.width /= scalar;
    size.height /= scalar;

    return size;
}

template<typename T>
Size<T> operator / (const Size<T> &size, T scalar)
{
    return Size<T>(size.width / scalar,
                   size.height / scalar);
}

/*! \} */ // end of geometry

} // End namespace tl
