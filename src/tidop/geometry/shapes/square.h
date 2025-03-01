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

#include <numeric>

#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/rect.h"

namespace tl
{

/*! \addtogroup Shapes
 *  \{
 */


/*!
 * \brief Square class
 *
 * The `Square` class represents a square, a 2D geometric shape where 
 * all sides are of equal length.
 */
template<typename T>
class Square
  : public Shape
{

public:

    using value_type = T;

private:

    T side;

public:

    /*!
     * \brief Default constructor
     *
     * Creates a square with side length 0.
     */
    Square();

    /*!
     * \brief Constructor with parameter
     * \param[in] side The length of one side of the square
     *
     * Creates a square with the specified side length.
     */
    Square(T side);

    /*!
     * \brief Get the area of the square
     * \return The area of the square
     *
     * The area of a square is simply the side length squared.
     */
    auto area() const -> double override;

    /*!
     * \brief Set the side length
     * \param[in] side The length of the square's side
     */
    void setSide(T side);

    /*!
     * \brief Get the side length
     * \return The length of the square's side
     */
    auto getSide() const -> T;
};



template<typename T>
Square<T>::Square()
  : side(0)
{
}

template<typename T>
Square<T>::Square(T side)
  : side(side)
{
}

template<typename T>
auto Square<T>::area() const -> double
{
    return static_cast<double>(side * side);
}

template<typename T>
void Square<T>::setSide(T side)
{
    this->side = side;
}

template<typename T>
auto Square<T>::getSide() const -> T 
{
    return side;
}


/*! \} */ 

}
