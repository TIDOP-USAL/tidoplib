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
#include "tidop/geometry/size.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

 /*!
  * \brief Class for 2D rectangles.
  *
  */
template<typename T>
class Rect
{

public:

    using value_type = T;

public:

    /*!
     * \brief Default constructor.
     * Constructs a empty Rect object. isValid() returns false
     * and isEmpty() return true.
     */
    Rect();
    
    /*!
     * \brief Constructs a rectangle with top-left corner (x, y) and width and height
     * \param[in] x Rectangle left coordinate
     * \param[in] y Rectangle top coordinate
     * \param[in] width Rectangle width
     * \param[in] height Rectangle height
     */
    Rect(T x, T y, T width, T height);
    
    /*!
     * \brief Constructs a rectangle with top-left and bottom-right corners
     * \param[in] topLeft Rectangle top-left corner
     * \param[in] bottomRight Rectangle bottom-right corner
     */
    Rect(const Point<T> &topLeft, 
         const Point<T> &bottomRight);
    
    /*!
     * \brief Constructs a rectangle with top-left corner and size (width==height)
     * \param[in] topLeft Rectangle top-left corner
     * \param[in] size Rectangle width and height size
     */
    Rect(const Point<T> &topLeft, 
         const Size<T> &size);
    
    /*!
     * \brief Constructs a rectangle with top-left corner and dimensions (width and height)
     * \param[in] topLeft Rectangle top-left corner
     * \param[in] width Rectangle width
     * \param[in] height Rectangle height
     */
    Rect(const Point<T> &topLeft, 
         T width, 
         T height);
    
    /*!
     * \brief Copy constructor
     * \param[in] rect Rect object to copy
     */
    Rect(const Rect &rect);
    
    /*!
     * \brief Move constructor
     * \param[in] rect Rect object to move
     */
    Rect(Rect &&rect) TL_NOEXCEPT;
    
    /*!
     * \brief Copy assignment operator
     * \param[in] rect Rect object to copy
     */
    Rect &operator = (const Rect &rect);
    
    /*!
     * \brief Move assignment operator
     * \param[in] rect Rect object to move
     */
    Rect &operator = (Rect &&rect) TL_NOEXCEPT;
    
    /*!
     * \brief Return top-left corner
     */
    Point<T> topLeft() const;
    
    /*!
     * \brief Return top-right corner
     */
    Point<T> topRight() const;
    
    /*!
     * \brief Return bottom-right corner
     */
    Point<T> bottomRight() const;
    
    /*!
     * \brief Return bottom-left corner
     */
    Point<T> bottomLeft() const;
    
    /*!
     * \brief Returns the size of the rectangle.
     */
    Size<T> size() const;
    
    /*!
     * \brief Check if Rect object is empty. 
     * \return Returns true if the rectangle is empty, otherwise returns false. 
     */
    bool isEmpty() const;
    
    /*!
     * \brief Check if Rect object is valid.
     * \return Returns true if the rectangle is valid, otherwise returns false.
     */
    bool isValid() const;
    
    /*!
     * \brief Check if a point is contained in the rectangle.
     * \param[in] pt Point to check if is contained.
     * \return Returns true if the rectangle contains the point.
    */
    bool contains(const Point<T> &pt) const;
    
    /*!
     * \brief Transform a Rect object to a Window object
     * \return Window
     */
    Window<Point<T>> window() const;
    
    /*!
     * \brief Normalize the rectangle.
     */
    void normalized();
    
    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Rect<T2>() const;

public:

  T x;
  T y;
  T width;
  T height;
};


using Recti = Rect<int>;
using Rectf = Rect<float>;
using Rectd = Rect<double>;


template<typename T> inline
Rect<T>::Rect()
  : x{0},
    y{0},
    width{0},
    height{0}
{
}

template<typename T> inline
Rect<T>::Rect(T x, T y, T width, T height)
  : x(x), 
    y(y), 
    width(width), 
    height(height)
{
}

template<typename T> inline 
Rect<T>::Rect(const Point<T> &topLeft, 
              const Point<T> &bottomRight)
  : x(topLeft.x), 
    y(topLeft.y), 
    width(bottomRight.x - topLeft.x), 
    height(bottomRight.y - topLeft.y)
{
}

template<typename T> inline
Rect<T>::Rect(const Point<T> &topLeft, 
              const Size<T> &size)
  : x(topLeft.x), 
    y(topLeft.y), 
    width(size.width), 
    height(size.height)
{
}

template<typename T> inline
Rect<T>::Rect(const Point<T> &topLeft,
              T width, T height)
  : x(topLeft.x), 
    y(topLeft.y), 
    width(width), 
    height(height)
{
}

template<typename T> inline
Rect<T>::Rect(const Rect &rect)
  : x(rect.x), 
    y(rect.y), 
    width(rect.width), 
    height(rect.height)
{
}

template<typename T> inline
Rect<T>::Rect(Rect &&rect) TL_NOEXCEPT
  : x(rect.x), 
    y(rect.y), 
    width(rect.width), 
    height(rect.height)
{
}

template<typename T> inline
Rect<T> &Rect<T>::operator = (const Rect &rect)
{
    if (this != &rect) {
        this->x = rect.x;
        this->y = rect.y;
        this->width = rect.width;
        this->height = rect.height;
    }

    return *this;
}

template<typename T> inline
Rect<T> &Rect<T>::operator = (Rect &&rect) TL_NOEXCEPT
{
    if (this != &rect) {
        this->x = rect.x;
        this->y = rect.y;
        this->width = rect.width;
        this->height = rect.height;
    }

    return *this;
}

template<typename T> inline
Point<T> Rect<T>::topLeft() const
{
    return Point<T>(this->x, this->y);
}

template<typename T> inline
Point<T> Rect<T>::topRight() const
{
    return Point<T>(this->x + this->width, this->y);
}

template<typename T>
inline Point<T> Rect<T>::bottomRight() const
{
    return Point<T>(this->x + this->width, 
                    this->y + this->height);
}

template<typename T>
inline Point<T> Rect<T>::bottomLeft() const
{
    return Point<T>(this->x, this->y + this->height);
}

template<typename T> inline
Size<T> tl::Rect<T>::size() const
{
    return Size<T>(this->width, this->height);
}

template<typename T>
inline bool tl::Rect<T>::isEmpty() const
{
    return width <= static_cast<T>(0) || height <= static_cast<T>(0);
}

template<typename T>
inline bool tl::Rect<T>::isValid() const
{
    return width > static_cast<T>(0) && height > static_cast<T>(0);
}

template<typename T>
inline bool Rect<T>::contains(const Point<T> &pt) const
{
    return (this->x <= pt.x &&
            pt.x < this->x + this->width &&
            this->y <= pt.y &&
            pt.y < this->y + this->height);
}

template<typename T> inline
Window<Point<T>> Rect<T>::window() const
{
    return Window<Point<T>>(Point<T>(this->x, this->y),
                            Point<T>(this->x + this->width,
                            this->y + this->height));
}

template<typename T>
inline void tl::Rect<T>::normalized()
{
    if (!this->isValid()) {
        if (this->width < static_cast<T>(0)) {
            this->x += this->width;
            this->width = -this->width;
        }
        if (this->height < static_cast<T>(0)) {
            this->y += this->height;
            this->height = -this->height;
        }
    }
}

template<typename T> template<typename T2> inline 
Rect<T>::operator Rect<T2>() const
{
    Rect<T2> rect(numberCast<T2>(this->x),
                  numberCast<T2>(this->y),
                  numberCast<T2>(this->width),
                  numberCast<T2>(this->height));

    return rect;
}

template<typename T> static inline
bool operator == (const Rect<T> &rect1,
                  const Rect<T> &rect2)
{
    return (rect1.x == rect2.x &&
            rect1.y == rect2.y &&
            rect1.width == rect2.width &&
            rect1.height == rect2.height);
}

template<typename T> static inline
bool operator != (const Rect<T> &rect1,
                  const Rect<T> &rect2)
{
    return (rect1.x != rect2.x ||
            rect1.y != rect2.y ||
            rect1.width != rect2.width ||
            rect1.height != rect2.height);
}

template<typename T> static inline
Rect<T> intersect(const Rect<T> &rect1,
                  const Rect<T> &rect2)
{
    Rect<T> rect;

    rect.x = std::max(rect1.x, rect2.x);
    rect.y = std::max(rect1.y, rect2.y);
    Point<T> bottomRight1 = rect1.bottomRight();
    Point<T> bottomRight2 = rect2.bottomRight();
    rect.width = std::min(bottomRight1.x, bottomRight2.x) - rect.x;
    rect.height = std::min(bottomRight1.y, bottomRight2.y) - rect.y;
    if (rect.width < 0) rect.width = 0;
    if (rect.height < 0) rect.height = 0;

    return rect;
}

/*! \} */ // end of geometry

} // End namespace tl
