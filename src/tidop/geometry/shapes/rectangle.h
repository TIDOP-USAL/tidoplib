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

#include "tidop/geometry/shapes/shape.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/rect.h"

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
    Rectangle();

    /*!
     * \brief Constructor with parameters
     * \param[in] width The width of the rectangle
     * \param[in] height The height of the rectangle
     *
     * Creates a rectangle with the specified width and height.
     */
    Rectangle(T width, T height);

    /*!
     * \brief Get the area of the rectangle
     * \return The area of the rectangle
     *
     * The area of a rectangle is calculated by multiplying its width by its height.
     */
    auto area() const -> double override;

    /*!
     * \brief Set the width of the rectangle
     * \param[in] width The width of the rectangle
     */
    void setWidth(T width);

    /*!
     * \brief Get the width of the rectangle
     * \return The width of the rectangle
     */
    auto getWidth() const->T;

    /*!
     * \brief Set the height of the rectangle
     * \param[in] height The height of the rectangle
     */
    void setHeight(T height);

    /*!
     * \brief Get the height of the rectangle
     * \return The height of the rectangle
     */
    auto getHeight() const->T;
};



template<typename T>
Rectangle<T>::Rectangle()
  : width(0), 
    height(0)
{
}


template<typename T>
Rectangle<T>::Rectangle(T width, T height)
  : width(width),
    height(height)
{
}

template<typename T>
auto Rectangle<T>::area() const -> double
{
    return static_cast<double>(width * height);
}

template<typename T>
void Rectangle<T>::setWidth(T width)
{
    this->width = width;
}

template<typename T>
auto Rectangle<T>::getWidth() const -> T
{
    return width;
}

template<typename T>
void Rectangle<T>::setHeight(T height)
{
    this->height = height;
}

template<typename T>
auto Rectangle<T>::getHeight() const -> T
{
    return height;
}


/*! \} */ 

}
