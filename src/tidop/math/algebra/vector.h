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

#ifndef TL_MATH_VECTOR_H
#define TL_MATH_VECTOR_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <iterator>
#include <vector>
#include <array>
#include <random>

#include "tidop/core/exception.h"
#include "tidop/math/math.h"
#include "tidop/math/simd.h"
#include "tidop/math/data.h"

namespace tl
{

namespace math
{


/*! \addtogroup math
 *  \{
 */


/*! \addtogroup algebra
 *  \{
 */


template<typename T>
class VectorBase;


template<
  template<typename, size_t _size = DynamicDataStorage>
  class VectorDerived, typename T, size_t _size
>
class VectorBase<VectorDerived<T, _size>>
{

public:

  VectorBase() = default;
  ~VectorBase() = default;

  VectorDerived<T, _size> &operator+=(const VectorDerived<T, _size> &vector)
  {
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "");

#ifndef TL_HAVE_SIMD_INTRINSICS

    for (size_t i = 0; i < derived.size(); ++i) {
      derived[i] += vector[i];
    }

#else

    using namespace simd;

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    for (size_t i = 0; i < max_vector; i += packed_size) {

      packed_a.loadUnaligned(&derived[i]);
      packed_b.loadUnaligned(&vector[i]);

      packed_a += packed_b;
      packed_a.storeUnaligned(&derived[i]);

    }

    for (size_t i = max_vector; i < derived.size(); ++i) {
      derived[i] += vector[i];
    }

#endif

    return derived;
  }

  VectorDerived<T, _size> &operator-=(const VectorDerived<T, _size> &vector)
  {
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "");

#ifndef TL_HAVE_SIMD_INTRINSICS

    for (size_t i = 0; i < derived.size(); ++i) {
      derived[i] -= vector[i];
    }

#else

    using namespace simd;

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();

    size_t max_vector = (derived.size() / packed_size) * packed_size;
    for (size_t i = 0; i < max_vector; i += packed_size) {

      packed_a.loadUnaligned(&derived[i]);
      packed_b.loadUnaligned(&vector[i]);
      packed_a -= packed_b;
      packed_a.storeUnaligned(&derived[i]);

    }

    for (size_t i = max_vector; i < derived.size(); ++i) {
      derived[i] -= vector[i];
    }

#endif

    return derived;
  }

  VectorDerived<T, _size> &operator*=(const VectorDerived<T, _size> &vector)
  {
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "");

#ifndef TL_HAVE_SIMD_INTRINSICS

    for (size_t i = 0; i < derived.size(); ++i) {
      derived[i] *= vector[i];
    }

#else

    using namespace simd;

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();

    size_t max_vector = (derived.size() / packed_size) * packed_size;
    for (size_t i = 0; i < max_vector; i += packed_size) {

      packed_a.loadUnaligned(&derived[i]);
      packed_b.loadUnaligned(&vector[i]);
      packed_a *= packed_b;
      packed_a.storeUnaligned(&derived[i]);

    }

    for (size_t i = max_vector; i < derived.size(); ++i) {
      derived[i] *= vector[i];
    }

#endif

    return derived;
  }

  VectorDerived<T, _size> &operator/=(const VectorDerived<T, _size> &vector)
  {
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "");

#ifndef TL_HAVE_SIMD_INTRINSICS

    for (size_t i = 0; i < derived.size(); ++i) {
      derived[i] /= vector[i];
    }

#else

    using namespace simd;

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    for (size_t i = 0; i < max_vector; i += packed_size) {

      packed_a.loadUnaligned(&derived[i]);
      packed_b.loadUnaligned(&vector[i]);
      packed_a /= packed_b;
      packed_a.storeUnaligned(&derived[i]);

    }

    for (size_t i = max_vector; i < derived.size(); ++i) {
      derived[i] /= vector[i];
    }

#endif

    return derived;
  }

  VectorDerived<T, _size> &operator *=(T scalar)
  {
    auto &derived = this->derived();

#ifndef TL_HAVE_SIMD_INTRINSICS

    for (size_t i = 0; i < derived.size(); i++) {
      derived[i] *= scalar;
    }

#else

    using namespace simd;

    Packed<T> packed_a;
    Packed<T> packed_b(scalar);

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    for (size_t i = 0; i < max_vector; i += packed_size) {

      packed_a.loadUnaligned(&derived[i]);
      packed_a *= packed_b;
      packed_a.storeUnaligned(&derived[i]);

    }

    for (size_t i = max_vector; i < derived.size(); ++i) {
      derived[i] *= scalar;
    }

#endif

    return derived;
  }

  VectorDerived<T, _size> &operator /=(T scalar)
  {
    auto &derived = this->derived();

#ifndef TL_HAVE_SIMD_INTRINSICS

    //if (scalar != consts::zero<T>) {
      for (size_t i = 0; i < derived.size(); i++) {
        derived[i] /= scalar;
      }
    //} else {
    //  for (size_t i = 0; i < vector.size(); i++) {
    //    vector[i] = static_cast<T>(0);
    //  }
    //}

#else

    using namespace simd;

    Packed<T> packed_a;
    Packed<T> packed_b(scalar);

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    for (size_t i = 0; i < max_vector; i += packed_size) {

      packed_a.loadUnaligned(&derived[i]);
      packed_a /= packed_b;
      packed_a.storeUnaligned(&derived[i]);

    }

    for (size_t i = max_vector; i < derived.size(); ++i) {
      derived[i] /= scalar;
    }

#endif

    return derived;
  }

private:

  VectorDerived<T, _size> &derived()
  {
    return *static_cast<VectorDerived<T, _size> *>(this);
  }
  
  const VectorDerived<T, _size> &derived() const
  {
    return *static_cast<const VectorDerived<T, _size> *>(this);
  }

};



template<typename T, size_t _size = DynamicDataStorage>
class Vector
  : public VectorBase<Vector<T, _size>>
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

  using iterator       = typename Data<T, _size>::iterator;
  using const_iterator = typename Data<T, _size>::const_iterator;
  //using reverse_iterator       = typename Data<T, _size>::reverse_iterator;
  //using const_reverse_iterator = typename Data<T, _size>::const_reverse_iterator;

public:

  Vector()
    : _data(Data<T, _size>())
  {
  }

  Vector(size_t size, T value = std::numeric_limits<T>().lowest())
    : _data(Data<T, _size>(size, value))
  {
  }

  Vector(const Vector &vector)
    : _data(vector._data)
  {
  }

  Vector(Vector &&vector) TL_NOEXCEPT
    : _data(std::move(vector._data))
  {
  }

  Vector(std::initializer_list<T> values)
    : _data(Data<T, _size>(values))
  {
  }

  Vector(T *data, size_t size)
    : _data(Data<T, _size>(data, size))
  {
  }

  ~Vector() = default;

  /*!
   * \brief Operador de asignación de copia
   * \param[in] vector Objeto que se copia
   */
  Vector &operator=(const Vector &vector)
  {
    if (this != &vector) {
      this->_data = vector._data;
    }
    return (*this);
  }

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] vector Objeto que se mueve
   */
  Vector &operator=(Vector &&vector) TL_NOEXCEPT
  {
    if (this != &vector) {
      this->_data = std::forward<Data<T, _size>>(vector._data);
    }
    return (*this);
  }

public:

  void resize(size_t size)
  {
    static_assert(_size == DynamicDataStorage, "Fixed-size vector not support resize");

    _data.resize(size);
  }

  void resize(size_t size, T value)
  {
    static_assert(_size == DynamicDataStorage, "Fixed-size vector not support resize");

    _data.resize(size, value);
  }

  size_t size() const TL_NOEXCEPT
  {
    return _data.size();
  }
    
  const_reference at(size_type position) const
  { 
    return _data.at(position);
  }

  reference at(size_type position)
  {
    return _data.at(position);
  }

  const_reference operator[](size_t position) const
  {
    return _data[position];
  }

  reference operator[](size_t position)
  {
    return _data[position];
  }

  reference front()
  {
    return _data.front();
  }

  const_reference front() const
  {
    return _data.front();
  }

  reference back()
  {
    return _data.back();
  }

  const_reference back() const
  {
    return _data.back();
  }

  iterator begin() TL_NOEXCEPT
  {
    return _data.begin();
  }

  const_iterator begin() const TL_NOEXCEPT
  {
    return _data.begin();
  }

  iterator end() TL_NOEXCEPT
  {
    return _data.end();
  }

  const_iterator end() const TL_NOEXCEPT
  {
    return _data.end();
  }

  //reverse_iterator rbegin() TL_NOEXCEPT
  //const_reverse_iterator rbegin() const TL_NOEXCEPT;
  //reverse_iterator rend() TL_NOEXCEPT;
  //const_reverse_iterator rend() const TL_NOEXCEPT;

  //void fill(T value)
  //{
  //  mData.fill(value);
  //}

  double module() const;
  void normalize();
  double dotProduct(const Vector<T, _size> &vector) const;

  T *data()
  {
    return _data.data();
  }

  const T *data() const
  {
    return _data.data();
  }

  bool operator == (const Vector &vector) const;
  bool operator != (const Vector &vector) const;
  bool operator <  (const Vector &vector) const;
  bool operator <= (const Vector &vector) const;
  bool operator >  (const Vector &vector) const;
  bool operator >= (const Vector &vector) const;

  static Vector zero();
  static Vector zero(size_t size);
  static Vector unit();
  static Vector unit(size_t size);
  static Vector randon();
  static Vector randon(size_t size);

private:

  Data<T, _size> _data;

};



/* Definición de alias Vector */

using Vector2i = Vector<int, 2>;
using Vector2d = Vector<double, 2>;
using Vector2f = Vector<float, 2>;
using Vector3i = Vector<int, 3>;
using Vector3d = Vector<double, 3>;
using Vector3f = Vector<float, 3>;



template<typename T, size_t _size> inline
double Vector<T, _size>::module() const
{
  return sqrt(dotProduct(*this));
}

template<typename T, size_t _size> inline
void Vector<T, _size>::normalize()
{
  T length = static_cast<T>(this->module());
  if (length > consts::zero<T>) {
    *this /= length;
  } else {
    for (size_t i = 0; i < this->size(); i++) {
      _data[i] = consts::zero<T>;
    }
  }
}

template<typename T, size_t _size> inline
double Vector<T, _size>::dotProduct(const Vector<T, _size> &vector) const
{
  TL_ASSERT(_data.size() == vector.size(), "Different vector size");

  double dot = static_cast<double>(_data[0]) * static_cast<double>(vector[0]);
  for (size_t i = 1; i < _data.size(); i++) {
    dot += static_cast<double>(_data[i]) * static_cast<double>(vector[i]);
  }

  return dot;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator == (const Vector<T, _size> &vector) const
{
  return this->_data == vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator != (const Vector<T, _size> &vector) const
{
  return this->_data != vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator <  (const Vector<T, _size> &vector) const
{
  return this->_data < vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator <= (const Vector<T, _size> &vector) const
{
  return this->_data <= vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator >  (const Vector<T, _size> &vector) const
{
  return this->_data > vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator >= (const Vector<T, _size> &vector) const
{
  return this->_data > vector.mData;
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::zero()
{
  Vector<T, _size> vector;
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = consts::zero<T>;
  }
  return vector;
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::zero(size_t size)
{
  static_assert(_size == DynamicDataStorage, "Fixed-size vector not support resize");
  return Vector<T>(size, consts::zero<T>);
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::unit()
{
  Vector<T, _size> vector;

  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = consts::one<T>;
  }

  return vector;
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::unit(size_t size)
{
  static_assert(_size == DynamicDataStorage, "Fixed-size vector not support resize");
  return Vector<T>(size, consts::one<T>);
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::randon()
{
  Vector<T, _size> vector;

  std::random_device rd;
  std::mt19937 random_number_engine(rd());
  std::uniform_real_distribution<> distribution(0.0, 99.0);

  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = static_cast<T>(distribution(random_number_engine));
  }

  return vector;
}

template<typename T, size_t _size> inline
Vector<T, _size> Vector<T, _size>::randon(size_t size)
{
  static_assert(_size == DynamicDataStorage, "Fixed-size vector not support resize");

  Vector<T, _size> vector(size);

  std::random_device rd;
  std::mt19937 random_number_engine(rd());
  std::uniform_real_distribution<> distribution(0.0, 99.0);

  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = static_cast<T>(distribution(random_number_engine));
  }

  return vector;
}


/* Operaciones unarias */

template<typename T, size_t _size>
Vector<T, _size> operator + (const Vector<T, _size> &vector)
{
  return vector;
}

template<typename T, size_t _size>
Vector<T, _size> operator - (const Vector<T, _size> &vector)
{
  static_assert(std::is_signed<T>::value, "Requires signed type");

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
Vector<T, _size> operator - (const Vector<T, _size> &v0,
                             const Vector<T, _size> &v1)
{
  Vector<T, _size> v = v0;
  return v -= v1;
}

template<typename T, size_t _size>
Vector<T, _size> operator * (const Vector<T, _size> &v0,
                             const Vector<T, _size> &v1)
{
  Vector<T, _size> result = v0;
  return result *= v1;
//  size_t vector_size = v0.size();
//  TL_ASSERT(vector_size == v1.size(), "");
//
//  Vector<T, _size> result(v0);
//
//#ifndef TL_HAVE_SIMD_INTRINSICS
//  
//  for (size_t i = 0; i < v0.size(); i++) {
//    result[i] *= v1[i];
//  }
//
//#else
//    
//  using namespace simd;
//
//  Packed<T> packed_a;
//  Packed<T> packed_b;
//
//  constexpr size_t packed_size = packed_a.size();
//  size_t max_vector = (vector_size / packed_size) * packed_size;
//
//  for (size_t i = 0; i < max_vector; i += packed_size) {
//
//    packed_a.loadUnaligned(&result[i]);
//    packed_b.loadUnaligned(&v1[i]);
//
//    packed_a *= packed_b;
//    packed_a.storeUnaligned(&result[i]);
//  }
//
//  for (size_t i = max_vector; i < v0.size(); ++i) {
//    result[i] *= v1[i];
//  }
//
//#endif
//
//  return result;
}

template<typename T, size_t _size>
Vector<T, _size> operator / (const Vector<T, _size> &v0,
                             const Vector<T, _size> &v1)
{
  Vector<T, _size> result = v0;
  return result /= v1;
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
Vector<T, _size> operator / (const Vector<T, _size> &vector, 
                             T scalar)
{
  Vector<T, _size> v = vector;
  return v /= scalar;
}

template<typename T, size_t _size> 
double dotProduct(const Vector<T, _size> &v1,
                  const Vector<T, _size> &v2)
{
  TL_ASSERT(v1.size() == v2.size(), "Different vector size");

  double dot = static_cast<double>(v1[0]) * static_cast<double>(v2[0]);
  for (size_t i = 1; i < v1.size(); i++) {
    dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
  }
  return dot;
}




template<typename T, size_t _size>
std::ostream &operator<< (std::ostream &os, const Vector<T, _size> &vector)
{
  for (size_t i = 0; i < vector.size(); i++) {
    os << " " << vector[i] << "\n";
  }
  os << std::flush;
  return os;
}

template<typename T, size_t _size>
std::ostream &operator<< (std::ostream &os, const Vector<T, _size> *vector)
{
  for (size_t i = 0; i < vector->size(); i++) {
    os << " " << (*vector)[i] << "\n";
  }
  os << std::flush;
  return os;
}

/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_VECTOR_H

