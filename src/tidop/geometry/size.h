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


#ifndef TL_GEOMETRY_SIZE_H
#define TL_GEOMETRY_SIZE_H

#include "config_tl.h"

#include <limits>
#include <numeric>

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/window.h"

namespace tl
{


template<typename T>
class Size
{

public:

  Size();
  Size(T width, T height);
  Size(const Size &size);
  Size(Size &&size) TL_NOEXCEPT;

  Size &operator = (const Size &size);
  Size &operator = (Size &&size) TL_NOEXCEPT;

  bool isEmpty() const;

  /*!
   * \brief Conversi�n de tipo
   */
  template<typename T2> operator Size<T2>() const;

public:

  typedef T value_type;
  T width;
  T height;
};


typedef Size<int> SizeI;
typedef Size<float> SizeF;
typedef Size<double> SizeD;



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

template<typename T> inline
Size<T>::Size(Size &&size) TL_NOEXCEPT
  : width(size.width), 
    height(size.height)
{
}

template<typename T> inline
Size<T> &Size<T>::operator = (const Size &size)
{
  if (this != &size) {
    this->width = size.width;
    this->height = size.height;
  }
  return *this;
}

template<typename T> inline
Size<T> &Size<T>::operator = (Size &&size) TL_NOEXCEPT
{
  if (this != &size) {
    this->width = size.width;
    this->height = size.height;
  }
  return *this;
}

template<typename T>
inline bool tl::Size<T>::isEmpty() const
{
  return width <= 0 || height <= 0;
}

template<typename T> template<typename T2> inline 
Size<T>::operator Size<T2>() const
{
  if (std::is_integral<T2>::value) {
    return Size<T2>(TL_ROUND_TO_INT(this->width),
                    TL_ROUND_TO_INT(this->height));
  } else {
    return Size<T2>(static_cast<T2>(this->width),
                    static_cast<T2>(this->height));
  }
}

template<typename T> static inline
bool operator == (const Size<T> &size1, const Size<T> &size2)
{
  return (size1.width == size2.width && 
          size1.height == size2.height);
}

template<typename T> static inline
bool operator != (const Size<T> &size1, const Size<T> &size2)
{
  return (size1.width != size2.width || 
          size1.height != size2.height);
}

template<typename T> static inline
Size<T>& operator += (Size<T> &size1, const Size<T> &size2)
{
  size1.width += size2.width;
  size1.height += size2.height;
  return size1;
}

template<typename T> static inline
Size<T>& operator -= (Size<T> &size1, const Size<T> &size2)
{
  size1.width -= size2.width;
  size1.height -= size2.height;
  return size1;
}

template<typename T> static inline
Size<T> operator + (const Size<T> &size1, const Size<T> &size2)
{
  return Size<T>(size1.width + size2.width, 
                 size1.height + size2.height);
}

template<typename T> static inline
Size<T> operator - (const Size<T> &size1, const Size<T> &size2)
{
  return Size<T>(size1.width - size2.width,
                 size1.height - size2.height);
}

template<typename T> static inline
Size<T> operator * (const Size<T> &size, T scalar)
{
  return Size<T>(size.width * scalar, 
                 size.height * scalar);
}

template<typename T> static inline
Size<T> & operator *= (Size<T> &size, T scalar)
{
  size.width *= scalar;
  size.height *= scalar;
  return size;
}

template<typename T> static inline
Size<T>& operator /= (Size<T> &size, T scalar)
{
  size.width /= scalar;
  size.height /= scalar;
  return size;
}

template<typename T> static inline
Size<T> operator / (const Size<T> &size, T scalar)
{
  return Size<T>(size.width / scalar, 
                 size.height / scalar);
}


} // End namespace tl

#endif // TL_GEOMETRY_SIZE_H