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

#ifndef TL_MATH_UTILS_SIMD_H
#define TL_MATH_UTILS_SIMD_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/math/math.h"

#ifdef TL_HAVE_SIMD_INTRINSICS

#ifdef TL_HAVE_AVX
#include <immintrin.h>
#elif defined TL_HAVE_SSE4_2
#include <nmmintrin.h>
#elif defined TL_HAVE_SSE4_1
#include <smmintrin.h>
#elif defined TL_HAVE_SSE3
#include <pmmintrin.h>
#elif defined TL_HAVE_SSE2
#include <emmintrin.h>
#endif
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


namespace simd
{

template<typename T>
struct PackedTraits;

template<typename T>
class Packed;

template<>
struct PackedTraits<Packed<float>>
{
  using value_type = float;
#ifdef TL_HAVE_AVX
  using simd_type = __m256;
  static constexpr size_t size = 8;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128;
  static constexpr size_t size = 4;
#endif
};

template<>
struct PackedTraits<Packed<double>>
{
  using value_type = double;
#ifdef TL_HAVE_AVX
  using simd_type = __m256d;
  static constexpr size_t size = 4;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128d;
  static constexpr size_t size = 2;
#endif
};

template<>
struct PackedTraits<Packed<int8_t>>
{
  using value_type = int8_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;
  static constexpr size_t size = 32;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 16;
#endif
};

template<>
struct PackedTraits<Packed<uint8_t>>
{
  using value_type = uint8_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;
  static constexpr size_t size = 32;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 16;
#endif
};

template<>
struct PackedTraits<Packed<int16_t>>
{
  using value_type = int16_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;
  static constexpr size_t size = 16;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 8;
#endif
};

template<>
struct PackedTraits<Packed<uint16_t>>
{
  using value_type = uint16_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;
  static constexpr size_t size = 16;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 8;
#endif
};

template<>
struct PackedTraits<Packed<int32_t>>
{
  using value_type = int32_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;
  static constexpr size_t size = 8;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 4;
#endif
};

template<>
struct PackedTraits<Packed<uint32_t>>
{
  using value_type = uint32_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;

  static constexpr size_t size = 8;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 4;
#endif
};

template<>
struct PackedTraits<Packed<int64_t>>
{
  using value_type = int64_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;
  static constexpr size_t size = 4;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 2;
#endif
};

template<>
struct PackedTraits<Packed<uint64_t>>
{
  using value_type = uint64_t;
#ifdef TL_HAVE_AVX2
  using simd_type = __m256i;
  static constexpr size_t size = 4;
#elif defined TL_HAVE_SSE2
  using simd_type = __m128i;
  static constexpr size_t size = 2;
#endif
};


template<typename T>
class PackedBase
{

public:

  using value_type = typename PackedTraits<T>::value_type;
  using simd_type = typename PackedTraits<T>::simd_type;

public:

  PackedBase() = default;
  PackedBase(const simd_type &packed);
  PackedBase(value_type scalar);

  simd_type &loadAligned(const value_type *src);
  simd_type &loadUnaligned(const value_type *src);
  void storeAligned(value_type *dst) const;
  void storeUnaligned(value_type *dst) const;

  void setScalar(value_type value);

  /*!
   * \brief  Assignment operator
   * Assign from intrinsic type
   */
  PackedBase<T> &operator=(const simd_type &packed);

  /*!
   * \brief Type cast operator to convert to intrinsic type
   */
  operator simd_type() const;

  static constexpr size_t size();

private:

  simd_type mValue;

};


template<typename T>
class Packed
  : public PackedBase<Packed<T>>
{

public:

  Packed();
  Packed(const typename PackedTraits<Packed<T>>::simd_type &packed);
  Packed(typename PackedTraits<Packed<T>>::value_type scalar);

  Packed<T> &operator+=(const Packed<T> &packed);
  Packed<T> &operator-=(const Packed<T> &packed);
  Packed<T> &operator*=(const Packed<T> &packed);
  Packed<T> &operator/=(const Packed<T> &packed);

  Packed<T> operator++(int);
  Packed<T> &operator++();
  Packed<T> operator--(int);
  Packed<T> &operator--();

  /*!
   * \brief Suma de los elementos de un vector
   */
  T sum();
};




template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedAligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r;

#ifdef TL_HAVE_AVX
  r = _mm256_load_ps(data);
#elif defined TL_HAVE_SSE
  r = _mm_load_ps(data);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedAligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r;

#ifdef TL_HAVE_AVX
  r = _mm256_load_pd(data);
#elif defined TL_HAVE_SSE2
  r = _mm_load_pd(data);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedAligned(const T *data)
{
  using P = Packed<typename std::remove_cv<T>::type>;
  using simd_type = typename P::simd_type;

  P r;

#ifdef TL_HAVE_AVX2
  r = _mm256_load_si256(reinterpret_cast<simd_type const *>(data));
#elif defined TL_HAVE_SSE2
  r = _mm_load_si128(reinterpret_cast<simd_type const *>(data));
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedUnaligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r;

#ifdef TL_HAVE_AVX
  r = _mm256_loadu_ps(data);
#elif defined TL_HAVE_SSE
  r = _mm_loadu_ps(data);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedUnaligned(const T *data)
{
  Packed<typename std::remove_cv<T>::type> r;

#ifdef TL_HAVE_AVX
  r = _mm256_loadu_pd(data);
#elif defined TL_HAVE_SSE2
  r = _mm_loadu_pd(data);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  Packed<typename std::remove_cv<T>::type>>::type
loadPackedUnaligned(const T *data)
{
  using P = Packed<typename std::remove_cv<T>::type>;
  using simd_type = typename P::simd_type;

  P r;

#ifdef TL_HAVE_AVX2
  r = _mm256_loadu_si256(reinterpret_cast<simd_type const *>(data));
#elif defined TL_HAVE_SSE2
  r = _mm_loadu_si128(reinterpret_cast<simd_type const *>(data));
#endif

  return r;
}


template<typename T, typename U> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, void>::type
storePackedAligned(T *data, U &result)
{
#ifdef TL_HAVE_AVX
  _mm256_store_ps(data, result);
#elif defined TL_HAVE_SSE
  _mm_store_ps(data, result);
#endif
}

template<typename T, typename U> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, void>::type
storePackedAligned(T *data, U &result)
{
#ifdef TL_HAVE_AVX
  _mm256_store_pd(data, result);
#elif defined TL_HAVE_SSE2
  _mm_store_pd(data, result);
#endif
}

template<typename T, typename U> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  void>::type
storePackedAligned(T *data, U &result)
{
  using P = Packed<typename std::remove_cv<T>::type>;
  using simd_type = typename P::simd_type;

#ifdef TL_HAVE_AVX2
  _mm256_store_si256(reinterpret_cast<simd_type *>(data), result);
#elif defined TL_HAVE_SSE2
  _mm_store_si128(reinterpret_cast<simd_type *>(data), result);
#endif
}

template<typename T, typename U> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, void>::type
storePackedUnaligned(T *data, U &result)
{
#ifdef TL_HAVE_AVX
  _mm256_storeu_ps(data, result);
#elif defined TL_HAVE_SSE
  _mm_storeu_ps(data, result);
#endif
}

template<typename T, typename U> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, void>::type
storePackedUnaligned(T *data, U &result)
{
#ifdef TL_HAVE_AVX
  _mm256_storeu_pd(data, result);
#elif defined TL_HAVE_SSE2
  _mm_storeu_pd(data, result);
#endif
}

template<typename T, typename U> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  void>::type
storePackedUnaligned(T *data, U &result)
{
  using P = Packed<typename std::remove_cv<T>::type>;
  using simd_type = typename P::simd_type;

#ifdef TL_HAVE_AVX2
  _mm256_storeu_si256(reinterpret_cast<simd_type *>(data), result);
#elif defined TL_HAVE_SSE2
  _mm_storeu_si128(reinterpret_cast<simd_type *>(data), result);
#endif
}


template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
set(T data)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX
  r = _mm256_set1_ps(data);
#elif defined TL_HAVE_SSE
  r = _mm_set1_ps(data);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value,
  Packed<T>>::type
set(T data)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX
  r = _mm256_set1_pd(data);
#elif defined TL_HAVE_SSE2
  r = _mm_set1_pd(data);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value,
  Packed<T>>::type
set(T data)
{
  using P = Packed<typename std::remove_cv<T>::type>;

#ifdef TL_HAVE_AVX2
  if (P::size() == 32)
    return _mm256_set1_epi8(data);
  else if (P::size() == 16)
    return _mm256_set1_epi16(data);
  else if (P::size() == 8)
    return _mm256_set1_epi32(data);
  else if (P::size() == 4)
    return _mm256_set1_epi64x(data);
#elif defined TL_HAVE_SSE2
  if (P::size() == 16)
    return _mm_set1_epi8(data);
  else if (P::size() == 8)
    return _mm_set1_epi16(data);
  else if (P::size() == 4)
    return _mm_set1_epi32(data);
  else if (P::size() == 2)
    return _mm_set1_epi64x(data);
#endif
}


/// Addition

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
add(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX
  r = _mm256_add_ps(packed1, packed2);
#elif defined TL_HAVE_SSE
  r = _mm_add_ps(packed1, packed2);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
add(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX
  r = _mm256_add_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
  r = _mm_add_pd(packed1, packed2);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value, Packed<T>>::type
add(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX2
  if (packed1.size() == 32)
    r = _mm256_add_epi8(packed1, packed2);
  else if (packed1.size() == 16)
    r = _mm256_add_epi16(packed1, packed2);
  else if (packed1.size() == 8)
    r = _mm256_add_epi32(packed1, packed2);
  else if (packed1.size() == 4)
    r = _mm256_add_epi64(packed1, packed2);
#elif defined TL_HAVE_SSE2
  if (packed1.size() == 16)
    r = _mm_add_epi8(packed1, packed2);
  else if (packed1.size() == 8)
    r = _mm_add_epi16(packed1, packed2);
  else if (packed1.size() == 4)
    r = _mm_add_epi32(packed1, packed2);
  else if (packed1.size() == 2)
    r = _mm_add_epi64(packed1, packed2);
#endif

  return r;
}

/// Subtract

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
sub(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX
  r = _mm256_sub_ps(packed1, packed2);
#elif defined TL_HAVE_SSE
  r = _mm_sub_ps(packed1, packed2);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
sub(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX
  r = _mm256_sub_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
  r = _mm_sub_pd(packed1, packed2);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value, Packed<T>>::type
sub(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX2
  if (packed1.size() == 32)
    r = _mm256_sub_epi8(packed1, packed2);
  else if (packed1.size() == 16)
    r = _mm256_sub_epi16(packed1, packed2);
  else if (packed1.size() == 8)
    r = _mm256_sub_epi32(packed1, packed2);
  else if (packed1.size() == 4)
    r = _mm256_sub_epi64(packed1, packed2);
#elif defined TL_HAVE_SSE2
  if (packed1.size() == 16)
    r = _mm_sub_epi8(packed1, packed2);
  else if (packed1.size() == 8)
    r = _mm_sub_epi16(packed1, packed2);
  else if (packed1.size() == 4)
    r = _mm_sub_epi32(packed1, packed2);
  else if (packed1.size() == 2)
    r = _mm_sub_epi64(packed1, packed2);
#endif

  return r;
}


//template<typename T> inline
//typename std::enable_if<
//  std::is_same<float, typename std::remove_cv<T>::type>::value,
//  Packed<T>>::type
//  madd(const Packed<T> &packed1, const Packed<T> &packed2)
//{
//  Packed<T> r = _mm256_add_ps(packed1, packed2);
//  return r;
//}
//
//template<typename T> inline
//typename std::enable_if<
//  std::is_same<double, typename std::remove_cv<T>::type>::value,
//  Packed<T>>::type
//  madd(const Packed<T> &A, const Packed<T> &B, Packed<T> &C)
//{
//  Packed<T> r = _mm256_fmadd_pd(A, B, C);
//  return r;
//}
//
//template<typename T> inline
//typename std::enable_if<
//  std::is_integral<typename std::remove_cv<T>::type>::value,
//  Packed<T>>::type
//  madd(const Packed<T> &A, const Packed<T> &B, Packed<T> &C)
//{
//  Packed<T> r;// = _mm256_abs_epi64(A, B, C);
//  return r;
//}


/// Multiplication

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
mul(const Packed<T> &packed1, const Packed<T> &packed2)
{
  //Packed<T> r;

#ifdef TL_HAVE_AVX
  return _mm256_mul_ps(packed1, packed2);
#elif defined TL_HAVE_SSE
  return _mm_mul_ps(packed1, packed2);
#endif

  return Packed<T>();
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
mul(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX
  r = _mm256_mul_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
  r = _mm_mul_pd(packed1, packed2);
#endif

  return r;
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value, Packed<T>>::type
mul(const Packed<T> &packed1, const Packed<T> &packed2)
{
  Packed<T> r;

#ifdef TL_HAVE_AVX2
  if (packed1.size() == 32) {
    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/master/vectori256.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.
    // There is no 8-bit multiply in AVX2. Split into two 16-bit multiplications
    __m256i aodd = _mm256_srli_epi16(packed1, 8);              // odd numbered elements of a
    __m256i bodd = _mm256_srli_epi16(packed2, 8);              // odd numbered elements of b
    __m256i muleven = _mm256_mullo_epi16(packed1, packed2);    // product of even numbered elements
    __m256i mulodd = _mm256_mullo_epi16(aodd, bodd);           // product of odd  numbered elements
    mulodd = _mm256_slli_epi16(mulodd, 8);                     // put odd numbered elements back in place
    __m256i mask = _mm256_set1_epi32(0x00FF00FF);              // mask for even positions
    r = _mm256_blendv_epi8(mask, muleven, mulodd);                        // interleave even and odd
  } else if (packed1.size() == 16) {
    r = _mm256_mullo_epi16(packed1, packed2);
  } else if (packed1.size() == 8) {
    r = _mm256_mullo_epi32(packed1, packed2);
  } else if (packed1.size() == 4) {
    r = _mm256_mullo_epi64(packed1, packed2);
  }
#elif defined TL_HAVE_SSE2
  if (packed1.size() == 16) {
    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/d1e06dd3fa86a3ac052dde8f711f722f6d5c9762/vectori128.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.
    // There is no 8-bit multiply in SSE2. Split into two 16-bit multiplies
    __m128i aodd = _mm_srli_epi16(packed1, 8);         // odd numbered elements of a
    __m128i bodd = _mm_srli_epi16(packed2, 8);         // odd numbered elements of b
    __m128i muleven = _mm_mullo_epi16(packed1, packed2);     // product of even numbered elements
    __m128i mulodd = _mm_mullo_epi16(aodd, bodd);// product of odd  numbered elements
    mulodd = _mm_slli_epi16(mulodd, 8);          // put odd numbered elements back in place
    __m128i mask = _mm_set1_epi32(0x00FF00FF);   // mask for even positions
#ifdef TL_HAVE_SSE4_1
    r = _mm_blendv_epi8(mulodd, muleven, mask);
#else
    r = _mm_or_si128(_mm_and_si128(mask, muleven), _mm_andnot_si128(mask, mulodd));
#endif
  } else if (packed1.size() == 8) {
    r = _mm_mullo_epi16(packed1, packed2);
  } else if (packed1.size() == 4) {
    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/d1e06dd3fa86a3ac052dde8f711f722f6d5c9762/vectori128.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.
#  if defined TL_HAVE_SSE4_1
    r = _mm_mullo_epi32(packed1, packed2);
#  else
    __m128i a13 = _mm_shuffle_epi32(packed1, 0xF5);
    __m128i b13 = _mm_shuffle_epi32(packed2, 0xF5);
    __m128i prod02 = _mm_mul_epu32(packed1, packed2);
    __m128i prod13 = _mm_mul_epu32(a13, b13);
    __m128i prod01 = _mm_unpacklo_epi32(prod02, prod13);
    __m128i prod23 = _mm_unpackhi_epi32(prod02, prod13);
    r = _mm_unpacklo_epi64(prod01, prod23);
#  endif
  } else if (packed1.size() == 2) {
    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/d1e06dd3fa86a3ac052dde8f711f722f6d5c9762/vectori128.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.
    // Split into 32-bit multiplies
#  if defined TL_HAVE_SSE4_1
    __m128i bswap = _mm_shuffle_epi32(packed2, 0xB1);      // b0H,b0L,b1H,b1L (swap H<->L)
    __m128i prodlh = _mm_mullo_epi32(packed1, bswap);      // a0Lb0H,a0Hb0L,a1Lb1H,a1Hb1L, 32 bit L*H products
    __m128i zero = _mm_setzero_si128();                    // 0
    __m128i prodlh2 = _mm_hadd_epi32(prodlh, zero);        // a0Lb0H+a0Hb0L,a1Lb1H+a1Hb1L,0,0
    __m128i prodlh3 = _mm_shuffle_epi32(prodlh2, 0x73);    // 0, a0Lb0H+a0Hb0L, 0, a1Lb1H+a1Hb1L
    __m128i prodll = _mm_mul_epu32(packed1, packed2);      // a0Lb0L,a1Lb1L, 64 bit unsigned products
    __m128i prod = _mm_add_epi64(prodll, prodlh3);         // a0Lb0L+(a0Lb0H+a0Hb0L)<<32, a1Lb1L+(a1Lb1H+a1Hb1L)<<32
    r = prod;
#  else
    ///TODO: Error
    //int64_t aa[2], bb[2];
    //packed1.storeUnaligned(&aa[0]);                                     // split into elements
    //packed2.storeUnaligned(&bb[0]);
    //r = _mm_set_epi64x(aa[0] * bb[0], aa[1] * bb[1]);     // multiply elements separetely
#  endif
  }
#endif

  return r;
}


template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
div(const Packed<T> &packed1, const Packed<T> &packed2)
{
#ifdef TL_HAVE_AVX
  return _mm256_div_ps(packed1, packed2);
#elif defined TL_HAVE_SSE2
  return _mm_div_ps(packed1, packed2);
#endif
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
  div(const Packed<T> &packed1, const Packed<T> &packed2)
{
#ifdef TL_HAVE_AVX
  return _mm256_div_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
  return _mm_div_pd(packed1, packed2);
#endif
}

/// División entre enteros no permitida

/// Suma de todos los elementos de un vector
template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, T>::type
horizontal_sum(const Packed<T> &packed)
{
  /// Copy from 'vector class library':
  /// https://github.com/vectorclass/version2/blob/master/vectorf256.h
  /// (c) Copyright 2012-2021 Agner Fog.
  /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX
  __m128 sum1 = _mm_add_ps(_mm256_castps256_ps128(packed), _mm256_extractf128_ps(packed, 1));
  __m128 t1 = _mm_hadd_ps(sum1, sum1);
  __m128 t2 = _mm_hadd_ps(t1, t1);
  return _mm_cvtss_f32(t2);
#elif defined TL_HAVE_SSE3
  // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
  __m128 t1 = _mm_hadd_ps(packed, packed);
  __m128 t2 = _mm_hadd_ps(t1, t1);
  return _mm_cvtss_f32(t2);
#else
  __m128 t1 = _mm_movehl_ps(packed, packed);
  __m128 t2 = _mm_add_ps(packed, t1);
  __m128 t3 = _mm_shuffle_ps(t2, t2, 1);
  __m128 t4 = _mm_add_ss(t2, t3);
  return _mm_cvtss_f32(t4);
#endif
}


template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, T>::type
horizontal_sum(const Packed<T> &packed)
{
  /// Copy from 'vector class library':
  /// https://github.com/vectorclass/version2/blob/master/vectorf256.h
  /// (c) Copyright 2012-2021 Agner Fog.
  /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX
  __m128d sum1 = _mm_add_pd(_mm256_castpd256_pd128(packed), _mm256_extractf128_pd(packed, 1));
  __m128d t1 = _mm_unpackhi_pd(sum1, sum1);
  __m128d t2 = _mm_add_pd(sum1, t1);
  return _mm_cvtsd_f64(t2);
#elif defined TL_HAVE_SSE2
  __m128d t1 = _mm_unpackhi_pd(packed, packed);
  __m128d t2 = _mm_add_pd(packed, t1);
  return _mm_cvtsd_f64(t2);
#else
  __m128  t0 = _mm_castpd_ps(packed);
  __m128d t1 = _mm_castps_pd(_mm_movehl_ps(t0, t0));
  __m128d t2 = _mm_add_sd(packed, t1);
  return _mm_cvtsd_f64(t2);
#endif
}

template<typename T> inline
typename std::enable_if<
  std::is_integral<typename std::remove_cv<T>::type>::value, T>::type
horizontal_sum(const Packed<T> &packed)
{
  T sum{};

  /// Copy from 'vector class library':
  /// https://github.com/vectorclass/version2/blob/master/vectori256.h
  /// (c) Copyright 2012-2021 Agner Fog.
  /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX2
  if (packed.size() == 32) {
    __m256i sum1 = _mm256_sad_epu8(packed, _mm256_setzero_si256());
    __m256i sum2 = _mm256_shuffle_epi32(sum1, 2);
    __m256i sum3 = _mm256_add_epi16(sum1, sum2);
    __m128i sum4 = _mm256_extracti128_si256(sum3, 1);
    __m128i sum5 = _mm_add_epi16(_mm256_castsi256_si128(sum3), sum4);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum5));
  } else if (packed.size() == 16) {
    // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128i sum1 = _mm_add_epi16(_mm256_extracti128_si256(packed, 1), _mm256_castsi256_si128(packed));
    __m128i sum2 = _mm_add_epi16(sum1, _mm_unpackhi_epi64(sum1, sum1));
    __m128i sum3 = _mm_add_epi16(sum2, _mm_shuffle_epi32(sum2, 1));
    __m128i sum4 = _mm_add_epi16(sum3, _mm_shufflelo_epi16(sum3, 1));
    sum = static_cast<T>(_mm_cvtsi128_si32(sum4));               // truncate to 16 bits
  } else if (packed.size() == 8) {
    // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128i sum1 = _mm_add_epi32(_mm256_extracti128_si256(packed, 1), _mm256_castsi256_si128(packed));
    __m128i sum2 = _mm_add_epi32(sum1, _mm_unpackhi_epi64(sum1, sum1));
    __m128i sum3 = _mm_add_epi32(sum2, _mm_shuffle_epi32(sum2, 1));
    sum = static_cast<T>(_mm_cvtsi128_si32(sum3));
  } else if (packed.size() == 4) {
    __m256i sum1 = _mm256_shuffle_epi32(packed, 0x0E);                // high element
    __m256i sum2 = _mm256_add_epi64(packed, sum1);                    // sum
    __m128i sum3 = _mm256_extracti128_si256(sum2, 1);                 // get high part
    __m128i sum4 = _mm_add_epi64(_mm256_castsi256_si128(sum2), sum3); // add low and high parts
#ifdef __x86_64__
    sum = static_cast<T>(_mm_cvtsi128_si64(sum4));
#else
    // 64 bit registers not available
    union
    {
      __m128i m;
      int64_t y;
    } u;
    _mm_storel_epi64(&u.m, sum4);
    sum = static_cast<T>(u.y);
#endif
  }
#elif defined TL_HAVE_SSE2
  if (packed.size() == 16) {
    __m128i sum1 = _mm_sad_epu8(packed, _mm_setzero_si128());
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);
    __m128i sum3 = _mm_add_epi16(sum1, sum2);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum3));
  } else if (packed.size() == 8) {
#if defined TL_HAVE_SSSE3
    __m128i sum1 = _mm_hadd_epi16(packed, packed);                   // horizontally add 8 elements in 3 steps
    __m128i sum2 = _mm_hadd_epi16(sum1, sum1);
    __m128i sum3 = _mm_hadd_epi16(sum2, sum2);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum3));       // 16 bit sum
#elif defined TL_HAVE_SSE2
    __m128i sum1 = _mm_unpackhi_epi64(packed, packed);     // 4 high elements
    __m128i sum2 = _mm_add_epi16(packed, sum1);            // 4 sums
    __m128i sum3 = _mm_shuffle_epi32(sum2, 0x01);          // 2 high elements
    __m128i sum4 = _mm_add_epi16(sum2, sum3);              // 2 sums
    __m128i sum5 = _mm_shufflelo_epi16(sum4, 0x01);        // 1 high element
    __m128i sum6 = _mm_add_epi16(sum4, sum5);              // 1 sum
    sum = static_cast<T>(_mm_cvtsi128_si32(sum6));         // 16 bit sum
#endif
  } else if (packed.size() == 4) {
#if defined TL_HAVE_SSSE3
    // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128i sum1 = _mm_hadd_epi32(packed, packed);                   // horizontally add 4 elements in 2 steps
    __m128i sum2 = _mm_hadd_epi32(sum1, sum1);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum2));             // 32 bit sum
#elif defined TL_HAVE_SSE2
    __m128i sum1 = _mm_unpackhi_epi64(packed, packed);     // 2 high elements
    __m128i sum2 = _mm_add_epi32(packed, sum1);            // 2 sums
    __m128i sum3 = _mm_shuffle_epi32(sum2, 0x01);          // 1 high element
    __m128i sum4 = _mm_add_epi32(sum2, sum3);              // 2 sums
    sum = static_cast<T>(_mm_cvtsi128_si32(sum4));         // 32 bit sum
#endif
  } else if (packed.size() == 2) {
    __m128i sum1 = _mm_unpackhi_epi64(packed, packed);               // high element
    __m128i sum2 = _mm_add_epi64(packed, sum1);                 // sum
#ifdef __x86_64__
    return _mm_cvtsi128_si64(sum2);
#else
    // 64 bit registers not available
    union
    {
      __m128i m;
      int64_t y;
    } u;
    _mm_storel_epi64(&u.m, sum2);
    return u.y;
#endif
  }
#endif

  return sum;
}





/* PackedBase Implementation */

template<typename T>
PackedBase<T>::PackedBase(const typename PackedBase<T>::simd_type &packed)
  : mValue(packed)
{
}

template<typename T>
PackedBase<T>::PackedBase(typename PackedBase<T>::value_type scalar)
  : mValue(set(scalar))
{
}

template<typename T> inline
typename PackedBase<T>::simd_type &PackedBase<T>::loadAligned(const value_type *src)
{
  mValue = loadPackedAligned(src);
  return mValue; // TODO: para que devolver una referencia
}

template<typename T> inline
typename PackedBase<T>::simd_type &PackedBase<T>::loadUnaligned(const value_type *src)
{
  mValue = loadPackedUnaligned(src);
  return mValue;
}

template<typename T> inline
void PackedBase<T>::storeAligned(value_type *dst) const
{
  storePackedAligned(dst, mValue);
}

template<typename T> inline
void PackedBase<T>::storeUnaligned(value_type *dst) const
{
  storePackedUnaligned(dst, mValue);
}

template<typename T> inline
void PackedBase<T>::setScalar(value_type value)
{
  mValue = set(value);
}

template<typename T> inline
PackedBase<T> &PackedBase<T>::operator=(const typename PackedBase<T>::simd_type &packed)
{
  mValue = packed;
  return *this;
}

template<typename T> inline
constexpr size_t PackedBase<T>::size()
{
  return PackedTraits<T>::size;
}

template<typename T> inline
PackedBase<T>::operator simd_type() const
{
  return mValue;
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




/* Packed Implementation */



/* Packed overload operators */

template<typename T> inline
typename std::enable_if<
  std::is_same<float, typename std::remove_cv<T>::type>::value, Packed<T>>::type
  operator - (const Packed<T> &packet)
{
  return _mm_xor_ps(packet, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
}

template<typename T> inline
typename std::enable_if<
  std::is_same<double, typename std::remove_cv<T>::type>::value, Packed<T>>::type
  operator - (const Packed<T> &packet)
{
  return _mm_xor_pd(packet, _mm_castsi128_pd(_mm_setr_epi32(0, 0x80000000, 0, 0x80000000)));
}

template<typename T> inline
Packed<T> operator+(const Packed<T> &packed1,
                    const Packed<T> &packed2)
{
  return add(packed1, packed2);
}

template<typename T> inline
Packed<T> operator+(const Packed<T> &packed, T scalar)
{
  return packed + Packed<T>(scalar);
}

template<typename T> inline
Packed<T> operator+(T scalar, const Packed<T> &packed)
{
  return Packed<T>(scalar) + packed;
}

template<typename T> inline
Packed<T> operator-(const Packed<T> &packed1,
                    const Packed<T> &packed2)
{
  return sub(packed1, packed2);
}

template<typename T> inline
Packed<T> operator-(const Packed<T> &packed, T scalar)
{
  return packed - Packed<T>(scalar);
}

template<typename T> inline
Packed<T> operator-(T scalar, const Packed<T> &packed)
{
  return Packed<T>(scalar) - packed;
}

template<typename T> inline
Packed<T> operator*(const Packed<T> &packed1,
                    const Packed<T> &packed2)
{
  return mul(packed1, packed2);
}

template<typename T> inline
Packed<T> operator*(const Packed<T> &packed, T scalar)
{
  return packed * Packed<T>(scalar);
}

template<typename T> inline
Packed<T> operator*(T scalar, const Packed<T> &packed)
{
  return Packed<T>(scalar) * packed;
}

template<typename T> inline
Packed<T> operator/(const Packed<T> &packed1,
                    const Packed<T> &packed2)
{
  return div(packed1, packed2);
}

template<typename T> inline
Packed<T> operator/(const Packed<T> &packed, T scalar)
{
  return packed / Packed<T>(scalar);
}

template<typename T> inline
Packed<T> operator/(T scalar, const Packed<T> &packed)
{
  return Packed<T>(scalar) / packed;
}

template<typename T> inline
Packed<T>::Packed() : PackedBase<Packed<T>>()
{
}

template<typename T> inline
Packed<T>::Packed(const typename PackedTraits<Packed<T>>::simd_type &packed)
  : PackedBase<Packed<T>>(packed)
{
}

template<typename T> inline
Packed<T>::Packed(typename PackedTraits<Packed<T>>::value_type scalar)
  : PackedBase<Packed<T>>(scalar)
{
}

template<typename T> inline
Packed<T> &Packed<T>::operator+=(const Packed<T> &packed)
{
  *this = *this + packed;
  return *this;
}

template<typename T> inline
Packed<T> &Packed<T>::operator-=(const Packed<T> &packed)
{
  *this = *this - packed;
  return *this;
}

template<typename T> inline
Packed<T> &Packed<T>::operator*=(const Packed<T> &packed)
{
  *this = *this * packed;
  return *this;
}

template<typename T> inline
Packed<T> &Packed<T>::operator/=(const Packed<T> &packed)
{
  *this = *this / packed;
  return *this;
}

template<typename T> inline
Packed<T> Packed<T>::operator++(int)
{
  Packed<T> packet = *this;
  *this += consts::one<T>;
  return *packet;
}

template<typename T> inline
Packed<T> &Packed<T>::operator++()
{
  *this += consts::one<T>;
  return *this;
}

template<typename T> inline
Packed<T> Packed<T>::operator--(int)
{
  Packed<T> packet = *this;
  *this -= consts::one<T>;
  return *packet;
}

template<typename T> inline
Packed<T> &Packed<T>::operator--()
{
  *this -= consts::one<T>;
  return *this;
}

template<typename T> inline
T Packed<T>::sum()
{
  return horizontal_sum(*this);
}




//template<typename P, size_t _size = PackedTraits<P>::size>
//class PacketMul
//{
//  static P solve(const P &packed1, const P &packed2);
//
//};
//
//template<typename P, size_t _size>
//inline P PacketMul<P, _size>::solve(const P &packed1, const P &packed2)
//{
//  return P();
//}
//
//template<>
//inline Packed<float> PacketMul<Packed<float>>::solve(const Packed<float> &packed1, const Packed<float> &packed2)
//{
//  Packed<float> r;
//  
//  #ifdef TL_HAVE_AVX
//    r = _mm256_mul_ps(packed1, packed2);
//  #elif defined TL_HAVE_SSE2
//    r = _mm_mul_ps(packed1, packed2);
//  #endif
//  
//  return r;
//}
//
//template<>
//inline Packed<double> PacketMul<Packed<double>>::solve(const Packed<double>& packed1, const Packed<double>& packed2)
//{
//  Packed<double> r;
//
//#ifdef TL_HAVE_AVX
//    r = _mm256_mul_pd(packed1, packed2);
//#elif defined TL_HAVE_SSE2
//    r = _mm_mul_pd(packed1, packed2);
//#endif
//
//  return r;
//}

//template<>
//class PacketMul<Packed<float>>
//{
//  static Packed<float> solve(const Packed<float> &packed1, const Packed<float> &packed2)
//  {
//    Packed<float> r;
//
//#ifdef TL_HAVE_AVX
//    r = _mm256_mul_ps(packed1, packed2);
//#elif defined TL_HAVE_SSE2
//    r = _mm_mul_ps(packed1, packed2);
//#endif
//
//    return r;
//  }
//};
//
//template<>
//class PacketMul<Packed<double>>
//{
//  static Packed<double> solve(const Packed<double> &packed1, const Packed<double> &packed2)
//  {
//    Packed<double> r;
//
//#ifdef TL_HAVE_AVX
//    r = _mm256_mul_pd(packed1, packed2);
//#elif defined TL_HAVE_SSE2
//    r = _mm_mul_pd(packed1, packed2);
//#endif
//
//    return r;
//  }
//};
//
//template<>
//class PacketMul<Packed<int32_t>, 32>
//{
//  static Packed<int32_t> solve(const Packed<int32_t> &packed1, const Packed<int32_t> &packed2)
//  {
//    Packed<int32_t> r;
//
//#ifdef TL_HAVE_AVX
//      /// Copy from 'vector class library':
//      /// https://github.com/vectorclass/version2/blob/master/vectori256.h
//      /// (c) Copyright 2012-2021 Agner Fog.
//      /// Apache License version 2.0 or later.
//      // There is no 8-bit multiply in AVX2. Split into two 16-bit multiplications
//      __m256i aodd = _mm256_srli_epi16(packed1, 8);              // odd numbered elements of a
//      __m256i bodd = _mm256_srli_epi16(packed2, 8);              // odd numbered elements of b
//      __m256i muleven = _mm256_mullo_epi16(packed1, packed2);    // product of even numbered elements
//      __m256i mulodd = _mm256_mullo_epi16(aodd, bodd);           // product of odd  numbered elements
//      mulodd = _mm256_slli_epi16(mulodd, 8);                     // put odd numbered elements back in place
//      __m256i mask = _mm256_set1_epi32(0x00FF00FF);              // mask for even positions
//      r = _mm256_blendv_epi8(mask, muleven, mulodd);             // interleave even and odd
//#endif
//
//    return r;
//  }
//};
//
//template<>
//class PacketMul<Packed<int32_t>, 32>
//{
//  static Packed<int32_t> solve(const Packed<int32_t>& packed1, const Packed<int32_t>& packed2)
//  {
//    Packed<int32_t> r;
//
//#ifdef TL_HAVE_AVX
//    /// Copy from 'vector class library':
//    /// https://github.com/vectorclass/version2/blob/master/vectori256.h
//    /// (c) Copyright 2012-2021 Agner Fog.
//    /// Apache License version 2.0 or later.
//    // There is no 8-bit multiply in AVX2. Split into two 16-bit multiplications
//    __m256i aodd = _mm256_srli_epi16(packed1, 8);              // odd numbered elements of a
//    __m256i bodd = _mm256_srli_epi16(packed2, 8);              // odd numbered elements of b
//    __m256i muleven = _mm256_mullo_epi16(packed1, packed2);    // product of even numbered elements
//    __m256i mulodd = _mm256_mullo_epi16(aodd, bodd);           // product of odd  numbered elements
//    mulodd = _mm256_slli_epi16(mulodd, 8);                     // put odd numbered elements back in place
//    __m256i mask = _mm256_set1_epi32(0x00FF00FF);              // mask for even positions
//    r = _mm256_blendv_epi8(mask, muleven, mulodd);             // interleave even and odd
//#endif
//
//    return r;
//  }
//};



} // End namespace simd

/*! \} */ // end of Math

} // End namespace math

} // End namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS

#endif // TL_MATH_UTILS_SIMD_H

