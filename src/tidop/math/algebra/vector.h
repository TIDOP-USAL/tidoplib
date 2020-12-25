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

#ifndef TL_MATH_VECTOR_H
#define TL_MATH_VECTOR_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <iterator>
#include <vector>
#include <array>
#include <valarray>

#include "tidop/core/exception.h"

namespace tl
{

namespace math
{

constexpr auto DynamicVector = std::numeric_limits<size_t>().max();


/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *  \{
 */


template<typename T, size_t _size>
class VectorBase;

template<typename T, size_t _size>
class VectorBase
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

  using iterator       = typename std::array<T, _size>::iterator;
  using const_iterator = typename std::array<T, _size>::const_iterator;
  using reverse_iterator       = typename std::array<T, _size>::reverse_iterator;
  using const_reverse_iterator = typename std::array<T, _size>::const_reverse_iterator;

public:

  VectorBase();
  VectorBase(size_t size, T val);
  VectorBase(const VectorBase &vector);
  VectorBase(VectorBase &&vector) TL_NOEXCEPT;
  VectorBase(std::initializer_list<T> values);
  VectorBase(T *data, size_t size);

  void resize(size_t size);
  void resize(size_t size, T value);

  size_t size() const TL_NOEXCEPT
  {
    return _size;
  }
    
  const_reference at(size_type position) const
  { 
    return mData.at(position);
  }

  reference at(size_type position)
  {
    return mData.at(position);
  }

  const_reference operator[](size_t position) const
  {
    return mData[position];
  }

  reference operator[](size_t position)
  {
    return mData[position];
  }

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  iterator begin() TL_NOEXCEPT;
  const_iterator begin() const TL_NOEXCEPT;
  iterator end() TL_NOEXCEPT;
  const_iterator end() const TL_NOEXCEPT;
  reverse_iterator rbegin() TL_NOEXCEPT;
  const_reverse_iterator rbegin() const TL_NOEXCEPT;
  reverse_iterator rend() TL_NOEXCEPT;
  const_reverse_iterator rend() const TL_NOEXCEPT;

  //void fill(T value)
  //{
  //  mData.fill(value);
  //}

  VectorBase &operator=(const VectorBase &vector)
  {
    if (this != &vector) {
      this->mData = vector.mData;
    }
    return (*this);
  }

  VectorBase &operator=(VectorBase &&vector) TL_NOEXCEPT
  {
    if (this != &vector) {
      this->mData = std::forward<std::array<T, _size>>(vector.mData);
    }
    return (*this);
  }

  T *data()
  {
    return mData.data();
  }

  bool operator == (const VectorBase &vector) const;
  bool operator != (const VectorBase &vector) const;
  bool operator <  (const VectorBase &vector) const;
  bool operator <= (const VectorBase &vector) const;
  bool operator >  (const VectorBase &vector) const;
  bool operator >= (const VectorBase &vector) const;

protected:

  std::array<T, _size> mData;

};

template<typename T, size_t _size> inline
VectorBase<T, _size>::VectorBase()
  : mData()
{
  this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size> inline
VectorBase<T, _size>::VectorBase(size_t size, T val)
  : mData()
{
  //static_assert(_size == DynamicVector, "Fixed-size vector not support resize");
  this->mData.fill(val);
}

template<typename T, size_t _size> inline
VectorBase<T, _size>::VectorBase(const VectorBase<T, _size> &vector)
  : mData(vector.mData)
{
}

template<typename T, size_t _size> inline
VectorBase<T, _size>::VectorBase(VectorBase<T, _size> &&vector) TL_NOEXCEPT
  : mData(std::move(vector.mData))
{
}
  
template<typename T, size_t _size> inline
VectorBase<T, _size>::VectorBase(std::initializer_list<T> values)
{
  size_t n = values.size();
  if (n == _size){
    std::copy(values.begin(), values.end(), mData.begin());
  } else if (n > _size){
    std::copy(values.begin(), values.end(), mData.begin());
    std::fill(mData.begin() + n, mData.end(), static_cast<T>(0));
  } else {
    std::copy(values.begin(), values.begin() + n, mData.begin());
  }
}

template<typename T, size_t _size> inline
VectorBase<T, _size>::VectorBase(T *data, size_t size)
  : mData(data)
{
}

template<typename T, size_t _size> inline
void VectorBase<T, _size>::resize(size_t size)
{
  static_assert(_size == DynamicVector, "Fixed-size vector not support resize");
}

template<typename T, size_t _size> inline
void VectorBase<T, _size>::resize(size_t size, T value)
{
  static_assert(_size == DynamicVector, "Fixed-size vector not support resize");
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::reference VectorBase<T, _size>::front()
{
  return mData.front();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::const_reference VectorBase<T, _size>::front() const
{
  return mData.front();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::reference VectorBase<T, _size>::back()
{
  return mData.back();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::const_reference VectorBase<T, _size>::back() const
{
  return mData.back();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::iterator VectorBase<T, _size>::begin() TL_NOEXCEPT
{
  return mData.begin();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::const_iterator VectorBase<T, _size>::begin() const TL_NOEXCEPT
{
  return mData.cbegin();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::iterator VectorBase<T, _size>::end() TL_NOEXCEPT
{
  return mData.end();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::const_iterator VectorBase<T, _size>::end() const TL_NOEXCEPT
{
  return mData.cend();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::reverse_iterator VectorBase<T, _size>::rbegin() TL_NOEXCEPT
{
  return mData.rbegin();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::const_reverse_iterator VectorBase<T, _size>::rbegin() const TL_NOEXCEPT
{
  return mData.rbegin();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::reverse_iterator VectorBase<T, _size>::rend() TL_NOEXCEPT
{
  return mData.rend();
}

template<typename T, size_t _size> inline
typename VectorBase<T, _size>::const_reverse_iterator VectorBase<T, _size>::rend() const TL_NOEXCEPT
{
  return mData.rend();
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator == (const VectorBase<T, _size> &vector) const
{
  return this->mData == vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator != (const VectorBase<T, _size> &vector) const
{
  return this->mData != vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator <  (const VectorBase<T, _size> &vector) const
{
  return this->mData < vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator <= (const VectorBase<T, _size> &vector) const
{
  return this->mData <= vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator >  (const VectorBase<T, _size> &vector) const
{
  return this->mData > vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator >= (const VectorBase<T, _size> &vector) const
{
  return this->mData > vector.mData;
}




template<typename T>
class VectorBase<T, DynamicVector>
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

  using iterator       = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;
  using reverse_iterator       = typename std::vector<T>::reverse_iterator;
  using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

public:

  VectorBase()
  {
  }
  
  VectorBase(size_t size,
             T val)
    : mData(size, val)
  {
  }

  VectorBase(const VectorBase &vector)
    : mData(vector.mData)
  {
  }
    
  VectorBase(VectorBase &&vector) TL_NOEXCEPT
    : mData(std::move(vector.mData))
  {
  }
 
  VectorBase(std::initializer_list<T> values)
    : mData(values)
  {
  }
    
  VectorBase(T *data, size_t size)
    : mData(size)
  {
    for (size_t i = 0; i < size; i++) {
      mData[i] = data[i];
    }
  }

  void resize(size_t size)
  {
    mData.resize(size);
  }
  
  void resize(size_t size, T value)
  {
    mData.resize(size, value);
  }

  size_t size() const TL_NOEXCEPT
  {
    return mData.size();
  }

  const_reference at(size_type position) const
  { 
    return mData.at(position);
  }

  reference at(size_type position)
  {
    return mData.at(position);
  }

  const_reference operator[](size_t position) const
  {
    return mData[position];
  }

  reference operator[](size_t position)
  {
    return mData[position];
  }

  //void fill(T value)
  //{
  //  mData.fill(value);
  //}

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  iterator begin() TL_NOEXCEPT;
  const_iterator begin() const TL_NOEXCEPT;
  iterator end() TL_NOEXCEPT;
  const_iterator end() const TL_NOEXCEPT;
  reverse_iterator rbegin() TL_NOEXCEPT;
  const_reverse_iterator rbegin() const TL_NOEXCEPT;
  reverse_iterator rend() TL_NOEXCEPT;
  const_reverse_iterator rend() const TL_NOEXCEPT;

  VectorBase &operator=(const VectorBase &vector)
  {
    if (this != &vector) {
      this->mData = vector.mData;
    }
    return (*this);
  }

  VectorBase &operator=(VectorBase &&vector) TL_NOEXCEPT
  {
    if (this != &vector) {
      this->mData = std::forward<std::vector<T>>(vector.mData);
    }
    return (*this);
  }

  T *data()
  {
    return mData.data();
  }

  bool operator == (const VectorBase &vector) const;
  bool operator != (const VectorBase &vector) const;
  bool operator <  (const VectorBase &vector) const;
  bool operator <= (const VectorBase &vector) const;
  bool operator >  (const VectorBase &vector) const;
  bool operator >= (const VectorBase &vector) const;

private:

  TL_TODO("¿Probar con std::valarray?")
  std::vector<T> mData;

};

template<typename T> inline
typename VectorBase<T, DynamicVector>::reference VectorBase<T, DynamicVector>::front()
{
  return mData.front();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::const_reference VectorBase<T, DynamicVector>::front() const
{
  return mData.front();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::reference VectorBase<T, DynamicVector>::back()
{
  return mData.back();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::const_reference VectorBase<T, DynamicVector>::back() const
{
  return mData.back();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::iterator VectorBase<T, DynamicVector>::begin() TL_NOEXCEPT
{
  return mData.begin();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::const_iterator VectorBase<T, DynamicVector>::begin() const TL_NOEXCEPT
{
  return mData.cbegin();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::iterator VectorBase<T, DynamicVector>::end() TL_NOEXCEPT
{
  return mData.end();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::const_iterator VectorBase<T, DynamicVector>::end() const TL_NOEXCEPT
{
  return mData.cend();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::reverse_iterator VectorBase<T, DynamicVector>::rbegin() TL_NOEXCEPT
{
  return mData.rbegin();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::const_reverse_iterator VectorBase<T, DynamicVector>::rbegin() const TL_NOEXCEPT
{
  return mData.rbegin();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::reverse_iterator VectorBase<T, DynamicVector>::rend() TL_NOEXCEPT
{
  return mData.rend();
}

template<typename T> inline
typename VectorBase<T, DynamicVector>::const_reverse_iterator VectorBase<T, DynamicVector>::rend() const TL_NOEXCEPT
{
  return mData.rend();
}

template<typename T> inline
bool VectorBase<T, DynamicVector>::operator == (const VectorBase<T, DynamicVector> &vector) const
{
  return this->mData == vector.mData;
}

template<typename T> inline
bool VectorBase<T, DynamicVector>::operator != (const VectorBase<T, DynamicVector> &vector) const
{
  return this->mData != vector.mData;
}

template<typename T> inline
bool VectorBase<T, DynamicVector>::operator <  (const VectorBase<T, DynamicVector> &vector) const
{
  return this->mData < vector.mData;
}

template<typename T> inline
bool VectorBase<T, DynamicVector>::operator <= (const VectorBase<T, DynamicVector> &vector) const
{
  return this->mData <= vector.mData;
}

template<typename T> inline
bool VectorBase<T, DynamicVector>::operator >  (const VectorBase<T, DynamicVector> &vector) const
{
  return this->mData > vector.mData;
}

template<typename T> inline
bool VectorBase<T, DynamicVector>::operator >= (const VectorBase<T, DynamicVector> &vector) const
{
  return this->mData > vector.mData;
}





template<typename T, size_t _size = DynamicVector>
class Vector
  : public VectorBase<T, _size>
{

public:
  
  Vector();
  Vector(size_t size, T val = -std::numeric_limits<T>().max());
  Vector(const Vector &vector);
  Vector(Vector &&vector) TL_NOEXCEPT;
  Vector(std::initializer_list<T> values);
  Vector(T *data, size_t size);
  ~Vector() = default;

  Vector &operator=(const Vector &vector);
  Vector &operator=(Vector &&vector) TL_NOEXCEPT;

  double module() const;
  void normalize();

  static Vector zero();
  static Vector zero(size_t size);
  static Vector unit();
  static Vector unit(size_t size);
};


/* Definición de alias Vector */


typedef Vector<int, 2>    Vector2i;
typedef Vector<double, 2> Vector2d;
typedef Vector<float, 2>  Vector2f;
typedef Vector<int, 3>    Vector3i;
typedef Vector<double, 3> Vector3d;
typedef Vector<float, 3>  Vector3f;



/* Implementación Vector */

template<typename T, size_t _size> inline
Vector<T, _size>::Vector()
{
}
  
template<typename T, size_t _size> inline
Vector<T, _size>::Vector(size_t size, T val)
  : VectorBase(size, val)
{
}
 
template<typename T, size_t _size> inline
Vector<T, _size>::Vector(const Vector &vector)
  : VectorBase(vector)
{
}

template<typename T, size_t _size> inline
Vector<T, _size>::Vector(Vector &&vector) TL_NOEXCEPT
  : VectorBase(std::forward<VectorBase<T, _size>>(vector))
{
}

template<typename T, size_t _size> inline
Vector<T, _size>::Vector(std::initializer_list<T> values)
  : VectorBase(values)
{
}

template<typename T, size_t _size> inline
Vector<T, _size>::Vector(T *data, size_t size)
  : VectorBase(data, size)
{
}

template<typename T, size_t _size> inline
Vector<T, _size> &Vector<T, _size>::operator=(const Vector<T, _size> &vector)
{
  if (this != &vector) {
    VectorBase::operator=(vector);
  }
  return (*this);
}

template<typename T, size_t _size> inline
Vector<T, _size> &Vector<T, _size>::operator=(Vector<T, _size> &&vector) TL_NOEXCEPT
{
  if (this != &vector) {
    VectorBase::operator=(std::forward<VectorBase<T, _size>>(vector));
  }
  return (*this);
}

template<typename T, size_t _size> inline
double Vector<T, _size>::module() const
{
  return sqrt(dotProduct(*this, *this));
}

template<typename T, size_t _size> inline
void Vector<T, _size>::normalize()
{
  T length = static_cast<T>(this->module());
  if (length > static_cast<T>(0)) {
    *this /= length;
  } else {
    for (size_t i = 0; i < mVector.size(); i++) {
      this->mVector[i] = static_cast<T>(0);
    }
  }
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::zero()
{ 
  Vector<T, _size> vector;
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = static_cast<size_t>(0);
  }
  return vector;
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::zero(size_t size)
{ 
  static_assert(_size == DynamicVector, "Fixed-size vector not support resize");
  return Vector<T>(size, static_cast<size_t>(0));
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::unit()
{
  Vector<T, _size> vector;
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = static_cast<size_t>(1);
  }
  return vector;
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::unit(size_t size)
{
  static_assert(_size == DynamicVector, "Fixed-size vector not support resize");
  return Vector<T>(size, static_cast<size_t>(1));
}

/* Operaciones unarias */

template<typename T, size_t _size>  static
Vector<T, _size> operator + (const Vector<T, _size> &vector)
{
  return vector;
}

template<typename T, size_t _size> static
Vector<T, _size> operator - (const Vector<T, _size> &vector)
{
  Vector<T, _size> v = vector;
  for (size_t i = 0; i < vector.size(); i++) {
    v[i] = -vector[i];
  }
  return v;
}

/* Operaciones binarias */

template<typename T, size_t _size>
Vector<T, _size> operator + (const Vector<T, _size> &v0,
                              const Vector<T, _size> &v1)
{
  Vector<T, _size> v = v0;
  return v += v1;
}

template<typename T, size_t _size>
Vector<T, _size> &operator += (Vector<T, _size> &v0, 
                                const Vector<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] += v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector<T, _size> operator - (const Vector<T, _size> &v0,
                              const Vector<T, _size> &v1)
{
  Vector<T, _size> v = v0;
  return v -= v1;
}

template<typename T, size_t _size>
Vector<T, _size> &operator -= (Vector<T, _size> &v0, 
                                const Vector<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] -= v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector<T, _size> operator*(Vector<T, _size> const& v0,
                            Vector<T, _size> const& v1)
{
  Vector<T, _size> result = v0;
  return result *= v1;
}

template<typename T, size_t _size>
Vector<T, _size> &operator *= (Vector<T, _size> &v0, 
                                const Vector<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] *= v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector<T, _size> operator / (const Vector<T, _size> &v0,
                              const Vector<T, _size> &v1)
{
  Vector<T, _size> result = v0;
  return result /= v1;
}

template<typename T, size_t _size>
Vector<T, _size> &operator /= (Vector<T, _size> &v0, 
                                const Vector<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] /= v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector<T, _size> operator * (const Vector<T, _size> &vector, 
                              T scalar)
{
  Vector<T, _size> v = vector;
  return v *= scalar;
}

template<typename T, size_t _size>
Vector<T, _size> operator * (T scalar, 
                              const Vector<T, _size> &vector)
{
  Vector<T, _size> v = vector;
  return v *= scalar;
}

template<typename T, size_t _size>
Vector<T, _size> &operator *= (Vector<T, _size> &vector, 
                                T scalar)
{
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] *= scalar;
  }
  return vector;
}

template<typename T, size_t _size>
Vector<T, _size> operator / (const Vector<T, _size> &vector, 
                              T scalar)
{
  Vector<T, _size> v = vector;
  return v /= scalar;
}

template<typename T, size_t _size>
Vector<T, _size> &operator /= (Vector<T, _size> &vector,
                                T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (size_t i = 0; i < vector.size(); i++) {
      vector[i] /= scalar;
    }
  } else {
    for (size_t i = 0; i < vector.size(); i++) {
      vector[i] = static_cast<T>(0);
    }
  }
  return vector;
}

template<typename T, size_t _size> 
double dotProduct(const Vector<T, _size> &v1,
                  const Vector<T, _size> &v2)
{
  TL_ASSERT(v1.size() == v2.size(), "");

  double dot = static_cast<double>(v1.at(0)) * static_cast<double>(v2[0]);
  for (size_t i = 1; i < v1.size(); i++) {
    dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
  }
  return dot;
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_VECTOR_H

