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

#ifndef TL_MATH_UTILS_SIMD_H
#define TL_MATH_UTILS_SIMD_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <immintrin.h>
#include <type_traits>


namespace tl
{

namespace math
{


/*! \addtogroup Math
 *  \{
 */

// <xmmintrin.h> : SSE, operations on 4 single precision floating point numbers (float).
// <emmintrin.h> : SSE 2, operations on integers and on 2 double precision floating point numbers (double).
// <pmmintrin.h> : SSE 3, horizontal operations on SIMD registers.
// <tmmintrin.h> : SSSE 3, additional instructions.
// <smmintrin.h> : SSE 4.1, dot product and many operations on integers
// <nmmintrin.h> : SSE 4.2, additional instructions.
// <immintrin.h> : AVX, operations on integers, 8 float or 4 double.


/// Visual Studio X86 
/// /arch:[IA32|SSE|SSE2|AVX|AVX2|AVX512]
/// Visual Studio X64 
/// /arch:[AVX|AVX2|AVX512]




template<typename T>
struct PackedTraits;

template<typename T>
class Packed;

template<>
struct PackedTraits<Packed<float>>
{
  using simd_type = __m256;
  using value_type = float;
  static constexpr size_t size = 8;
};

template<>
struct PackedTraits<Packed<double>>
{
  using simd_type = __m256d;
  using value_type = double;
  static constexpr size_t size = 4;
};

template<>
struct PackedTraits<Packed<int>>
{
  using simd_type = __m256i;
  using value_type = int;
  static constexpr size_t size = 8;
};

template<>
struct PackedTraits<Packed<unsigned int>>
{
  using simd_type = __m256i;
  using value_type = unsigned int;
  static constexpr size_t size = 8;
};

template<>
struct PackedTraits<Packed<char>>
{
  using simd_type = __m256i;
  using value_type = char;
  static constexpr size_t size = 32;
};

template<>
struct PackedTraits<Packed<unsigned char>>
{
  using simd_type = __m256i;
  using value_type = unsigned char;
  static constexpr size_t size = 32;
};

template<>
struct PackedTraits<Packed<long>>
{
  using simd_type = __m256i;
  using value_type = long;
  static constexpr size_t size = 4;
};

template<>
struct PackedTraits<Packed<unsigned long>>
{
  using simd_type = __m256i;
  using value_type = unsigned long;
  static constexpr size_t size = 4;
};


template<typename T>
class PackedBase
{

public:

  using value_type = typename PackedTraits<T>::value_type;
  using simd_type = typename PackedTraits<T>::simd_type;

public:

  PackedBase()
  {
  }

  PackedBase(const simd_type &packed)
    : mValue(packed)
  {
  }

  simd_type &loadAligned(const value_type *src);

  simd_type &loadUnaligned(const value_type *src);

  void storeAligned(value_type *dst) const;

  void storeUnaligned(value_type *dst) const;

  T &operator=(const simd_type &packed)
  {
    mValue = packed;
    return *this;
  }

  operator simd_type() const {
    return mValue;
  }

  T &operator()()
  {
    return *static_cast<T *>(this);
  }

  const T &operator()() const 
  {
    return *static_cast<const T *>(this);
  }

  size_t size() const
  {
    return PackedTraits<T>::size;
  }

  T &operator+=(const T &packed)
  {
    (*this)() = (*this)() + packed;
    return *this;
  }

  T &operator+=(const value_type &scalar)
  {
    (*this)() = (*this)() + T(scalar);
    return *this;
  }

private:

  simd_type mValue;
  //size_t mSize = PackedTraits<T>::size;

};


template<typename T>
class Packed
  : public PackedBase<Packed<T>>
{

public:

  Packed() : PackedBase<Packed<T>>()
  {
  }

  Packed(const typename PackedTraits<Packed<T>>::simd_type &packed) : PackedBase<Packed<T>>(packed)
  {}

};


/// TODO: no tiene que devolver Packed

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedAligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r = _mm256_load_ps(data);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedAligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r = _mm256_load_pd(data);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedAligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r;// = _mm256_loadu_epi32(data);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedUnaligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r = _mm256_loadu_ps(data);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedUnaligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r = _mm256_loadu_pd(data);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedUnaligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r;// = _mm256_loadu_epi32(data);
  return r;
}


template<typename T, typename U> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, void>::type
storePackedAligned(T *data, U &result)
{
  _mm256_store_ps(data, result);
}

template<typename T, typename U> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, void>::type
storePackedAligned(T *data, U &result)
{
  _mm256_store_pd(data, result);
}

template<typename T, typename U> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  void>::type
storePackedAligned(T *data, U &result)
{
  _mm256_storeu_si256(data, result);
}

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, void>::type
storePackedUnaligned(T *data, Packed<T> &result)
{
  _mm256_storeu_ps(data, result);
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, void>::type
storePackedUnaligned(T *data, Packed<T> &result)
{
  _mm256_storeu_pd(data, result);
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  void>::type
storePackedUnaligned(T *data, Packed<T> &result)
{
  //_mm256_storeu_pd(data, result);
}



/// Addition

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
add(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r = _mm256_add_ps(packed1, packed2);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
add(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r = _mm256_add_pd(packed1, packed2);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value, Packed<T>>::type
add(const Packed<T> &packed1, const Packed<T> &packed2)
{

  //Packed<T> r = _mm256_add_epi8(packed1, packed2);
  //_mm256_add_epi16(packed1, packed2);
  //_mm256_add_epi32(packed1, packed2);
  //_mm256_add_epi64(packed1, packed2);
}


template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value,
  Packed<T>>::type
  madd(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r = _mm256_add_ps(packed1, packed2);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value,
  Packed<T>>::type
  madd(const Packed<T> &A, const Packed<T> &B, Packed<T> &C)
{
  Packed<T> r = _mm256_fmadd_pd(A, B, C);
  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  Packed<T>>::type
  madd(const Packed<T> &A, const Packed<T> &B, Packed<T> &C)
{
  Packed<T> r;// = _mm256_abs_epi64(A, B, C);
  return r;
}




template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
mul(const Packed<T> &packed1, const Packed<T> &packed2)
{
  return _mm256_mul_ps(packed1, packed2);
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
mul(const Packed<T> &packed1, const Packed<T> &packed2)
{
  return _mm256_mul_pd(packed1, packed2);
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value, Packed<T>>::type
mul(const Packed<T> &packed1, const Packed<T> &packed2)
{
  
}


/* PackedBase Implementation */

template<typename T>
typename PackedBase<T>::simd_type &PackedBase<T>::loadAligned(const value_type *src)
{
  mValue = loadPackedAligned(src);
  return mValue; // TODO: para que devolver una referencia
}

template<typename T>
typename PackedBase<T>::simd_type &PackedBase<T>::loadUnaligned(const value_type *src)
{
  mValue = loadPackedUnaligned(src);
  return mValue;
}

template<typename T>
void PackedBase<T>::storeAligned(value_type *dst) const
{
  storePackedAligned(dst, mValue);
}

template<typename T>
void PackedBase<T>::storeUnaligned(value_type *dst) const
{
  storePackedUnaligned(dst, mValue);
}






//template<typename T>
//inline PackedBase<T> operator+(const PackedBase<T> &packed,
//                               const typename PackedTraits<T>::type &scalar)
//{
//  return packed() + T(scalar);
//}
//
//template<typename T>
//inline PackedBase<T> operator+(const typename PackedTraits<T>::type &scalar,
//                               const PackedBase<T> &packed)
//{
//  return T(scalar) + packed();
//}


template<typename T>
inline Packed<T> operator+(const Packed<T> &packed1, const Packed<T> &packed2)
{
  return add(packed1, packed2);
}

//template<typename T>
//inline Packed<T> operator-(const Packed<T> &packed1, const Packed<T> &packed2)
//{
//  return _mm_sub_ps(packed1, packed2);
//}

template<typename T>
inline Packed<T> operator*(const Packed<T> &packed1, const Packed<T> &packed2)
{
  return mul(packed1, packed2);
}

//template<typename T>
//inline Packed<T> operator/(const Packed<T> &packed1, const Packed<T> &packed2)
//{
//  return _mm_div_ps(packed1, packed2);
//}


/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_UTILS_SIMD_H

