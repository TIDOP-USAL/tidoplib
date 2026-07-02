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

#include "tidop/geometry/shapes/Shape.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/graphic/base/Rect.h"

namespace tl
{

/*! \addtogroup Shapes
 *  \{
 */


/*!
 * \brief Rectangle class
 *
 * The `Rectangle` class represents a rectangle, a 2D geometric shape 
 * with opposite sides of equal length.
 */
template<typename T>
class Rectangle
  : public Shape
{

public:

    using value_type = T;

private:

    T width;  ///< The width of the rectangle
    T height; ///< The height of the rectangle

public:

    /*!
     * \brief Default constructor
     *
     * Creates a rectangle with width and height equal to 0.
     */
    constexpr Rectangle() = default;

    constexpr Rectangle(const Rectangle<T> &rect) = default;
    constexpr Rectangle(Rectangle<T> &&rect) noexcept = default;

    constexpr auto operator=(const Rectangle<T> &rect) -> Rectangle<T> & = default;
    constexpr auto operator=(Rectangle<T> &&rect) noexcept -> Rectangle<T> & = default;

    /*!
     * \brief Constructor with parameters
     * \param[in] width The width of the rectangle
     * \param[in] height The height of the rectangle
     *
     * Creates a rectangle with the specified width and height.
     */
    constexpr Rectangle(T width, T height);

    /*!
     * \brief Get the area of the rectangle
     * \return The area of the rectangle
     *
     * The area of a rectangle is calculated by multiplying its width by its height.
     */
    [[nodiscard]]
    constexpr auto area() const noexcept -> double override;

    /*!
     * \brief Set the width of the rectangle
     * \param[in] width The width of the rectangle
     */
    constexpr void setWidth(T width);

    /*!
     * \brief Get the width of the rectangle
     * \return The width of the rectangle
     */
    [[nodiscard]]
    constexpr auto getWidth() const noexcept -> T;

    /*!
     * \brief Set the height of the rectangle
     * \param[in] height The height of the rectangle
     */
    constexpr void setHeight(T height);

    /*!
     * \brief Get the height of the rectangle
     * \return The height of the rectangle
     */
    [[nodiscard]]
    constexpr auto getHeight() const noexcept -> T;
};



//template<typename T>
//Rectangle<T>::Rectangle()
//  : width(0), 
//    height(0)
//{
//}


template<typename T>
constexpr Rectangle<T>::Rectangle(T width, T height)
  : width(width),
    height(height)
{
}

template<typename T>
constexpr auto Rectangle<T>::area() const noexcept -> double
{
    return static_cast<double>(width * height);
}

template<typename T>
constexpr void Rectangle<T>::setWidth(T width)
{
    this->width = width;
}

template<typename T>
constexpr auto Rectangle<T>::getWidth() const noexcept -> T
{
    return width;
}

template<typename T>
constexpr void Rectangle<T>::setHeight(T height)
{
    this->height = height;
}

template<typename T>
constexpr auto Rectangle<T>::getHeight() const noexcept -> T
{
    return height;
}


/*! \} */ 

}
