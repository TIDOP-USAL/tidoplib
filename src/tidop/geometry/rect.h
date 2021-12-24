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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GEOMETRY_RECT_H
#define TL_GEOMETRY_RECT_H

#include "config_tl.h"

#include <limits>
#include <numeric>

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/window.h"
#include "tidop/geometry/size.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

template<typename T>
class Rect
{

public:

  typedef T value_type;

public:

  Rect();
  Rect(T x, T y, T width, T height);
  Rect(const Point<T> &topLeft, 
       const Point<T> &bottomRight);
  Rect(const Point<T> &topLeft, 
       const Size<T> &size);
  Rect(const Point<T> &topLeft, 
       T width, 
       T height);
  Rect(const Rect &rect);
  Rect(Rect &&rect) TL_NOEXCEPT;

  Rect &operator = (const Rect &rect);
  Rect &operator = (Rect &&rect) TL_NOEXCEPT;

  Point<T> topLeft() const;
  Point<T> topRight() const;
  Point<T> bottomRight() const;
  Point<T> bottomLeft() const;
  Size<T> size() const;
  bool isEmpty() const;
  bool isValid() const;
  bool contains(const Point<T> &pt) const;
  Window<Point<T>> window() const;
  void normalized();

  template<typename T2> operator Rect<T2>() const;

public:

  T x;
  T y;
  T width;
  T height;
};


using RectI = Rect<int>;
using RectF = Rect<float>;
using RectD = Rect<double>;


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
inline bool Rect<T>::contains(const Point<T>& pt) const
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
  if (std::is_integral<T2>::value) {
    return Rect<T2>(roundToInteger(this->x),
                    roundToInteger(this->y),
                    roundToInteger(this->width),
                    roundToInteger(this->height));
  } else {
    return Rect<T2>(static_cast<T2>(this->x),
                    static_cast<T2>(this->y),
                    static_cast<T2>(this->width),
                    static_cast<T2>(this->height));
  }
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

#endif // TL_GEOMETRY_RECT_H
