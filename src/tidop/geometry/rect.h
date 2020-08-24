/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


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


template<typename T>
class Rect
{

public:

  Rect();
  Rect(T x, T y, T width, T height);
  Rect(const Point<T> &topLeft, const Point<T> &bottomRight);
  Rect(const Point<T> &topLeft, const Size<T> &size);
  Rect(const Point<T> &topLeft, T width, T height);
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
  bool contains(const Point<T> &pt) const;
  Window<Point<T>> window() const;

  /*!
   * \brief Conversión de tipo
   */
  template<typename T2> operator Rect<T2>() const;

public:

  typedef T value_type;
  T x;
  T y;
  T width;
  T height;
};


typedef Rect<int> RectI;
typedef Rect<float> RectF;
typedef Rect<double> RectD;



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
Rect<T>::Rect(const Point<T> &topLeft, const Point<T> &bottomRight)
  : x(topLeft.x), 
    y(topLeft.y), 
    width(bottomRight.x - topLeft.x), 
    height(bottomRight.y - topLeft.y)
{
}

template<typename T> inline
Rect<T>::Rect(const Point<T> &topLeft, const Size<T> &size)
  : x(topLeft.x), 
    y(topLeft.y), 
    width(size.width), 
    height(size.height)
{
}

template<typename T> inline
Rect<T>::Rect(const Point<T> &topLeft, T width, T height)
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
  return Point<T>(this->x + this->width, this->y + this->height);
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
  return width <= 0 || height <= 0;
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
                          Point<T>(this->x + this->width, this->y + this->height));
}

template<typename T> template<typename T2> inline 
Rect<T>::operator Rect<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Rect<T2>(TL_ROUND_TO_INT(this->x),
                    TL_ROUND_TO_INT(this->y),
                    TL_ROUND_TO_INT(this->width),
                    TL_ROUND_TO_INT(this->height));
  } else {
    return Rect<T2>(static_cast<T2>(this->x),
                    static_cast<T2>(this->y),
                    static_cast<T2>(this->width),
                    static_cast<T2>(this->height));
  }
}



template<typename T> static inline
bool operator == (const Rect<T> &rect1, const Rect<T> &rect2)
{
  return (rect1.x == rect2.x && 
          rect1.y == rect2.y && 
          rect1.width == rect2.width && 
          rect1.height == rect2.height);
}

template<typename T> static inline
bool operator != (const Rect<T> &rect1, const Rect<T> &rect2)
{
  return (rect1.x != rect2.x || 
          rect1.y != rect2.y || 
          rect1.width != rect2.width || 
          rect1.height != rect2.height);
}

template<typename T> static inline
Rect<T> intersect(const Rect<T> &rect1, const Rect<T> &rect2)
{
  Rect<T> rect;
  rect.x = std::max(rect1.x, rect2.x);
  rect.y = std::max(rect1.y, rect2.y);
  Point<T> bottomRight1 = rect1.bottomRight();
  Point<T> bottomRight2 = rect2.bottomRight();
  rect.width = std::min(bottomRight1.x, bottomRight2.x) - rect.x;
  rect.height = std::min(bottomRight1.y, bottomRight2.y) - rect.y;
  return rect;
}


} // End namespace tl

#endif // TL_GEOMETRY_RECT_H
