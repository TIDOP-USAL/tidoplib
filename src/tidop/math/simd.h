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

#include "tidop/config.h"
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
class Packed
{

public:

    using value_type = typename PackedTraits<Packed<T>>::value_type;
    using simd_type = typename PackedTraits<Packed<T>>::simd_type;

public:

    Packed() = default;
    Packed(const Packed &packed);
    Packed(Packed &&packed) TL_NOEXCEPT;
    Packed(simd_type packed);
    Packed(value_type scalar);

    void loadAligned(const value_type *src);
    void loadUnaligned(const value_type *src);
    void storeAligned(value_type *dst) const;
    void storeUnaligned(value_type *dst) const;

    void setScalar(value_type value);

    /*!
     * \brief  Assignment operator
     * Assign from intrinsic type
     */
    Packed<T> &operator=(simd_type packed);

    /*!
     * \brief Type cast operator to convert to intrinsic type
     */
    operator simd_type() const;

    static constexpr size_t size();

    auto operator=(const Packed<T> &packed) -> Packed<T>&;
    auto operator=(Packed<T> &&packed) TL_NOEXCEPT -> Packed<T>&;

    auto operator+=(const Packed<T> &packed) -> Packed<T>&;
    auto operator-=(const Packed<T> &packed) -> Packed<T>&;
    auto operator*=(const Packed<T> &packed) -> Packed<T>&;
    auto operator/=(const Packed<T> &packed) -> Packed<T>&;

    auto operator++(int) -> Packed<T>;
    auto operator++() -> Packed<T>&;
    auto operator--(int) -> Packed<T>;
    auto operator--() -> Packed<T>&;

    /*!
     * \brief Sum of the elements of a vector
     */
    auto sum() -> T;

    static auto zero() -> Packed;

private:

    simd_type mValue;
};


/// \cond

namespace internal
{

template<typename T>
auto loadPackedAligned(const T *data) -> enableIfFloat<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_load_ps(data);
#elif defined TL_HAVE_SSE
    return _mm_load_ps(data);
#endif
}

template<typename T>
auto loadPackedAligned(const T *data) -> enableIfDouble<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_load_pd(data);
#elif defined TL_HAVE_SSE2
    return _mm_load_pd(data);
#endif
}

template<typename T>
auto loadPackedAligned(const T *data) -> enableIfIntegral<T, typename Packed<T>::simd_type>
{
    using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
    return _mm256_load_si256(reinterpret_cast<simd_type const *>(data));
#elif defined TL_HAVE_SSE2
    return _mm_load_si128(reinterpret_cast<simd_type const *>(data));
#endif
}

template<typename T>
auto loadPackedUnaligned(const T *data) -> enableIfFloat<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_loadu_ps(data);
#elif defined TL_HAVE_SSE
    return _mm_loadu_ps(data);
#endif
}

template<typename T>
auto loadPackedUnaligned(const T *data) -> enableIfDouble<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_loadu_pd(data);
#elif defined TL_HAVE_SSE2
    return _mm_loadu_pd(data);
#endif
}

template<typename T>
auto loadPackedUnaligned(const T *data) -> enableIfIntegral<T, typename Packed<T>::simd_type>
{
    using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
    return _mm256_loadu_si256(reinterpret_cast<simd_type const *>(data));
#elif defined TL_HAVE_SSE2
    return _mm_loadu_si128(reinterpret_cast<simd_type const *>(data));
#endif
}


template<typename T, typename U>
auto storePackedAligned(T *data, U &result) -> enableIfFloat<T, void>
{
#ifdef TL_HAVE_AVX
    _mm256_store_ps(data, result);
#elif defined TL_HAVE_SSE
    _mm_store_ps(data, result);
#endif
}

template<typename T, typename U>
auto storePackedAligned(T *data, U &result) -> enableIfDouble<T, void>
{
#ifdef TL_HAVE_AVX
    _mm256_store_pd(data, result);
#elif defined TL_HAVE_SSE2
    _mm_store_pd(data, result);
#endif
}

template<typename T, typename U>
auto storePackedAligned(T *data, U &result) -> enableIfIntegral<T, void>
{
    using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
    _mm256_store_si256(reinterpret_cast<simd_type *>(data), result);
#elif defined TL_HAVE_SSE2
    _mm_store_si128(reinterpret_cast<simd_type *>(data), result);
#endif
}

template<typename T, typename U>
auto storePackedUnaligned(T *data, U &result) -> enableIfFloat<T, void>
{
#ifdef TL_HAVE_AVX
    _mm256_storeu_ps(data, result);
#elif defined TL_HAVE_SSE
    _mm_storeu_ps(data, result);
#endif
}

template<typename T, typename U>
auto storePackedUnaligned(T *data, U &result) -> enableIfDouble<T, void>
{
#ifdef TL_HAVE_AVX
    _mm256_storeu_pd(data, result);
#elif defined TL_HAVE_SSE2
    _mm_storeu_pd(data, result);
#endif
}

template<typename T, typename U>
auto storePackedUnaligned(T *data, U &result) -> enableIfIntegral<T, void>
{
    using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
    _mm256_storeu_si256(reinterpret_cast<simd_type *>(data), result);
#elif defined TL_HAVE_SSE2
    _mm_storeu_si128(reinterpret_cast<simd_type *>(data), result);
#endif
}


template<typename T>
auto set(T data) -> enableIfFloat<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_set1_ps(data);
#elif defined TL_HAVE_SSE
    return _mm_set1_ps(data);
#endif
}

template<typename T>
auto set(T data) -> enableIfDouble<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_set1_pd(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_pd(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi8(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi8(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi16(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi16(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi32(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi32(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi64x(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi64x(data);
#endif
}


template<typename T>
auto setZero() -> enableIfFloat<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_setzero_ps();
#elif defined TL_HAVE_SSE
    return _mm_setzero_ps();
#endif
}

template<typename T>
auto setZero() -> enableIfDouble<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_setzero_pd();
#elif defined TL_HAVE_SSE2
    return _mm_setzero_pd();
#else
    //...
#endif
}


template<typename T>
auto setZero() -> enableIfIntegral<T, void>
{

#ifdef TL_HAVE_AVX
    return _mm256_setzero_si256();
#elif defined TL_HAVE_SSE2
    return _mm_setzero_si128();
#endif
}

/// Addition

template<typename T>
auto add(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_add_ps(packed1, packed2);
#elif defined TL_HAVE_SSE
    return _mm_add_ps(packed1, packed2);
#endif
}

template<typename T>
auto add(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_add_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
    return _mm_add_pd(packed1, packed2);
#endif
}

template<typename T>
auto add(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX2
    return _mm256_add_epi8(packed1, packed2);
#elif defined TL_HAVE_SSE2
    return _mm_add_epi8(packed1, packed2);
#endif
}

template<typename T>
auto add(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX2
    return _mm256_add_epi16(packed1, packed2);
#elif defined TL_HAVE_SSE2
    return _mm_add_epi16(packed1, packed2);
#endif
}

template<typename T>
auto add(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX2
    return _mm256_add_epi32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    return _mm_add_epi32(packed1, packed2);
#endif
}

template<typename T>
auto add(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX2
    return _mm256_add_epi64(packed1, packed2);
#elif defined TL_HAVE_SSE2
    return _mm_add_epi64(packed1, packed2);
#endif
}

/// Subtract

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_sub_ps(packed1, packed2);
#elif defined TL_HAVE_SSE
    packed = _mm_sub_ps(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_sub_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_pd(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi8(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi8(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi16(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi16(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi32(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi64(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi64(packed1, packed2);
#endif

    return packed;
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

template<typename T>
auto mul(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_mul_ps(packed1, packed2);
#elif defined TL_HAVE_SSE
    packed = _mm_mul_ps(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto mul(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_mul_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_mul_pd(packed1, packed2);
#endif

    return packed;
}


template<typename T>
auto mul(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
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
    packed = _mm256_blendv_epi8(mulodd, muleven, mask);             // interleave even and odd
#elif defined TL_HAVE_SSE2
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
    packed = _mm_blendv_epi8(mulodd, muleven, mask);
#else
    packed = _mm_or_si128(_mm_and_si128(mask, muleven), _mm_andnot_si128(mask, mulodd));
#endif
#endif

    return packed;
}

template<typename T>
auto mul(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_mullo_epi16(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_mullo_epi16(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto mul(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_mullo_epi32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/d1e06dd3fa86a3ac052dde8f711f722f6d5c9762/vectori128.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.
#  if defined TL_HAVE_SSE4_1
    packed = _mm_mullo_epi32(packed1, packed2);
#  else
    __m128i a13 = _mm_shuffle_epi32(packed1, 0xF5);
    __m128i b13 = _mm_shuffle_epi32(packed2, 0xF5);
    __m128i prod02 = _mm_mul_epu32(packed1, packed2);
    __m128i prod13 = _mm_mul_epu32(a13, b13);
    __m128i prod01 = _mm_unpacklo_epi32(prod02, prod13);
    __m128i prod23 = _mm_unpackhi_epi32(prod02, prod13);
    packed = _mm_unpacklo_epi64(prod01, prod23);
#  endif
#endif

    return packed;
}

template<typename T>
auto mul(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/d1e06dd3fa86a3ac052dde8f711f722f6d5c9762/vectori256.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.
    // Split into 32-bit multiplies
    __m256i bswap = _mm256_shuffle_epi32(packed2, 0xB1);  // swap H<->L
    __m256i prodlh = _mm256_mullo_epi32(packed1, bswap);   // 32 bit L*H products
    __m256i zero = _mm256_setzero_si256();              // 0
    __m256i prodlh2 = _mm256_hadd_epi32(prodlh, zero);      // a0Lb0H+a0Hb0L,a1Lb1H+a1Hb1L,0,0
    __m256i prodlh3 = _mm256_shuffle_epi32(prodlh2, 0x73);  // 0, a0Lb0H+a0Hb0L, 0, a1Lb1H+a1Hb1L
    __m256i prodll = _mm256_mul_epu32(packed1, packed2);   // a0Lb0L,a1Lb1L, 64 bit unsigned products
    __m256i prod = _mm256_add_epi64(prodll, prodlh3);    // a0Lb0L+(a0Lb0H+a0Hb0L)<<32, a1Lb1L+(a1Lb1H+a1Hb1L)<<32
    packed = prod;
#elif defined TL_HAVE_SSE2
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
    packed = prod;
#  else
    ///TODO: Error
    int64_t aa[2], bb[2];
    packed1.storeUnaligned(&aa[0]);                            // split into elements
    packed2.storeUnaligned(&bb[0]);
    packed = _mm_set_epi64x(aa[1] * bb[1], aa[0] * bb[0]);     // multiply elements separetely
#  endif
#endif

    return packed;
}


template<typename T>
auto div(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_div_ps(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_div_ps(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto div(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_div_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_div_pd(packed1, packed2);
#endif

    return packed;
}

/// División entre enteros no permitida

/// Suma de todos los elementos de un vector
template<typename T>
auto horizontal_sum(const Packed<T> &packed) -> enableIfFloat<T, T>
{
    T sum{};

    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/master/vectorf256.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX
    __m128 sum1 = _mm_add_ps(_mm256_castps256_ps128(packed), _mm256_extractf128_ps(packed, 1));
    __m128 t1 = _mm_hadd_ps(sum1, sum1);
    __m128 t2 = _mm_hadd_ps(t1, t1);
    sum = _mm_cvtss_f32(t2);
#elif defined TL_HAVE_SSE3
  // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128 t1 = _mm_hadd_ps(packed, packed);
    __m128 t2 = _mm_hadd_ps(t1, t1);
    sum = _mm_cvtss_f32(t2);
#else
    __m128 t1 = _mm_movehl_ps(packed, packed);
    __m128 t2 = _mm_add_ps(packed, t1);
    __m128 t3 = _mm_shuffle_ps(t2, t2, 1);
    __m128 t4 = _mm_add_ss(t2, t3);
    sum = _mm_cvtss_f32(t4);
#endif

    return sum;
}


template<typename T>
auto horizontal_sum(const Packed<T> &packed) -> enableIfDouble<T, T>
{
    T sum{};

    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/master/vectorf256.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX
    __m128d sum1 = _mm_add_pd(_mm256_castpd256_pd128(packed), _mm256_extractf128_pd(packed, 1));
    __m128d t1 = _mm_unpackhi_pd(sum1, sum1);
    __m128d t2 = _mm_add_pd(sum1, t1);
    sum = _mm_cvtsd_f64(t2);
#elif defined TL_HAVE_SSE2
    __m128d t1 = _mm_unpackhi_pd(packed, packed);
    __m128d t2 = _mm_add_pd(packed, t1);
    sum = _mm_cvtsd_f64(t2);
#else
    __m128  t0 = _mm_castpd_ps(packed);
    __m128d t1 = _mm_castps_pd(_mm_movehl_ps(t0, t0));
    __m128d t2 = _mm_add_sd(packed, t1);
    sum = _mm_cvtsd_f64(t2);
#endif

    return sum;
}

template<typename T>
auto horizontal_sum(const Packed<T> &packed) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    T>
{
    T sum{};

    /// Copy from 'vector class library':
    /// https://github.com/vectorclass/version2/blob/master/vectori256.h
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX2
    __m256i sum1 = _mm256_sad_epu8(packed, _mm256_setzero_si256());
    __m256i sum2 = _mm256_shuffle_epi32(sum1, 2);
    __m256i sum3 = _mm256_add_epi16(sum1, sum2);
    __m128i sum4 = _mm256_extracti128_si256(sum3, 1);
    __m128i sum5 = _mm_add_epi16(_mm256_castsi256_si128(sum3), sum4);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum5));
#elif defined TL_HAVE_SSE2
    __m128i sum1 = _mm_sad_epu8(packed, _mm_setzero_si128());
    __m128i sum2 = _mm_unpackhi_epi64(sum1, sum1);
    __m128i sum3 = _mm_add_epi16(sum1, sum2);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum3));
#endif

    return sum;
}

template<typename T>
auto horizontal_sum(const Packed<T> &packed) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    T>
{
    T sum{};

    /// Copy from 'vector class library':
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX2
    // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128i sum1 = _mm_add_epi16(_mm256_extracti128_si256(packed, 1), _mm256_castsi256_si128(packed));
    __m128i sum2 = _mm_add_epi16(sum1, _mm_unpackhi_epi64(sum1, sum1));
    __m128i sum3 = _mm_add_epi16(sum2, _mm_shuffle_epi32(sum2, 1));
    __m128i sum4 = _mm_add_epi16(sum3, _mm_shufflelo_epi16(sum3, 1));
    sum = static_cast<T>(_mm_cvtsi128_si32(sum4));               // truncate to 16 bits
#elif defined TL_HAVE_SSE2
#  if defined TL_HAVE_SSSE3
    __m128i sum1 = _mm_hadd_epi16(packed, packed);                   // horizontally add 8 elements in 3 steps
    __m128i sum2 = _mm_hadd_epi16(sum1, sum1);
    __m128i sum3 = _mm_hadd_epi16(sum2, sum2);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum3));       // 16 bit sum
#  else
    __m128i sum1 = _mm_unpackhi_epi64(packed, packed);     // 4 high elements
    __m128i sum2 = _mm_add_epi16(packed, sum1);            // 4 sums
    __m128i sum3 = _mm_shuffle_epi32(sum2, 0x01);          // 2 high elements
    __m128i sum4 = _mm_add_epi16(sum2, sum3);              // 2 sums
    __m128i sum5 = _mm_shufflelo_epi16(sum4, 0x01);        // 1 high element
    __m128i sum6 = _mm_add_epi16(sum4, sum5);              // 1 sum
    sum = static_cast<T>(_mm_cvtsi128_si32(sum6));         // 16 bit sum
#  endif
#endif

    return sum;
}

template<typename T>
auto horizontal_sum(const Packed<T> &packed) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    T>
{
    T sum{};

    /// Copy from 'vector class library':
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX2
  // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128i sum1 = _mm_add_epi32(_mm256_extracti128_si256(packed, 1), _mm256_castsi256_si128(packed));
    __m128i sum2 = _mm_add_epi32(sum1, _mm_unpackhi_epi64(sum1, sum1));
    __m128i sum3 = _mm_add_epi32(sum2, _mm_shuffle_epi32(sum2, 1));
    sum = static_cast<T>(_mm_cvtsi128_si32(sum3));
#elif defined TL_HAVE_SSE2
#  if defined TL_HAVE_SSSE3
  // The hadd instruction is inefficient, and may be split into two instructions for faster decoding
    __m128i sum1 = _mm_hadd_epi32(packed, packed);                   // horizontally add 4 elements in 2 steps
    __m128i sum2 = _mm_hadd_epi32(sum1, sum1);
    sum = static_cast<T>(_mm_cvtsi128_si32(sum2));             // 32 bit sum
#  else
    __m128i sum1 = _mm_unpackhi_epi64(packed, packed);     // 2 high elements
    __m128i sum2 = _mm_add_epi32(packed, sum1);            // 2 sums
    __m128i sum3 = _mm_shuffle_epi32(sum2, 0x01);          // 1 high element
    __m128i sum4 = _mm_add_epi32(sum2, sum3);              // 2 sums
    sum = static_cast<T>(_mm_cvtsi128_si32(sum4));         // 32 bit sum
#  endif
#endif

    return sum;
}

template<typename T>
auto horizontal_sum(const Packed<T>& packed) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    T>
{
    T sum{};

    /// Copy from 'vector class library':
    /// (c) Copyright 2012-2021 Agner Fog.
    /// Apache License version 2.0 or later.

#ifdef TL_HAVE_AVX2
    __m256i sum1 = _mm256_shuffle_epi32(packed, 0x0E);                // high element
    __m256i sum2 = _mm256_add_epi64(packed, sum1);                    // sum
    __m128i sum3 = _mm256_extracti128_si256(sum2, 1);                 // get high part
    __m128i sum4 = _mm_add_epi64(_mm256_castsi256_si128(sum2), sum3); // add low and high parts
#  ifdef __x86_64__
    sum = static_cast<T>(_mm_cvtsi128_si64(sum4));
#  else
    // 64 bit registers not available
    union
    {
        __m128i m;
        int64_t y;
    } u;
    _mm_storel_epi64(&u.m, sum4);
    sum = static_cast<T>(u.y);
#  endif
#elif defined TL_HAVE_SSE2
    __m128i sum1 = _mm_unpackhi_epi64(packed, packed);          // high element
    __m128i sum2 = _mm_add_epi64(packed, sum1);                 // sum
#  ifdef __x86_64__
    sum = _mm_cvtsi128_si64(sum2);
#  else
    // 64 bit registers not available
    union
    {
        __m128i m;
        int64_t y;
    } u;
    _mm_storel_epi64(&u.m, sum2);
    sum = static_cast<T>(u.y);
#  endif
#endif

    return sum;
}



template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_min_ps(packed1, packed2);
#else
    packed = _mm_min_ps(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_min_pd(packed1, packed2);
#else
    packed = _mm_min_pd(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX2
   packed = _mm256_min_epi8(packed1, packed2);
#elif TL_HAVE_SSE4_1
   packed = _mm_min_epi8(packed1, packed2);
#else  // SSE2
    __m128i signbit = _mm_set1_epi32(0x80808080);
    __m128i a1 = _mm_xor_si128(packed1, signbit);          // add 0x80
    __m128i b1 = _mm_xor_si128(packed2, signbit);          // add 0x80
    __m128i m1 = _mm_min_epu8(a1, b1);                     // unsigned min
    return  _mm_xor_si128(m1, signbit);                    // sub 0x80
#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX2
   packed = _mm256_min_epu8(packed1, packed2);
#else
    packed = _mm_min_epu8(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX2
   packed = _mm256_min_epi16(packed1, packed2);
#else
    packed = _mm_min_epi16(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX2
   packed = _mm256_min_epu16(packed1, packed2);
#elif TL_HAVE_SSE4_1
    packed = _mm_min_epu16(packed1, packed2);
#else  // SSE2
    __m128i signbit = _mm_set1_epi32(0x80008000);
    __m128i a1 = _mm_xor_si128(packed1, signbit);          // add 0x8000
    __m128i b1 = _mm_xor_si128(packed2, signbit);          // add 0x8000
    __m128i m1 = _mm_min_epi16(a1, b1);                    // signed min
    packed = _mm_xor_si128(m1, signbit);                   // sub 0x8000
#endif

    return packed;
}

// Compare packed signed 32-bit integers in packed1 and packed2, and return packed minimum values
template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_min_epi32(packed1, packed2);
#elif TL_HAVE_SSE4_1
    packed = _mm_min_epi32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    // Compares the 4 signed 32-bit integers in packed1 and the 4 signed 32-bit integers in packed2 for greater than.
    __m128i greater = _mm_cmpgt_epi32(packed1, packed2);
    packed = _mm_or_si128(_mm_and_si128(greater, packed2), _mm_andnot_si128(greater, packed1));
#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_min_epu32(packed1, packed2);
#elif TL_HAVE_SSE4_1
    packed = _mm_min_epu32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    __m128i signbit = _mm_set1_epi32(0x80000000);
    __m128i greater = _mm_cmpgt_epi32(_mm_xor_si128(packed1, signbit), _mm_xor_si128(packed2, signbit));
    packed = _mm_or_si128(_mm_and_si128(greater, packed2), _mm_andnot_si128(greater, packed1));
#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_blendv_epi8(packed2, packed1, _mm256_cmpgt_epi64(packed1, packed2));
#else

#endif

    return packed;
}

template<typename T>
auto min(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    __m256i offset = _mm256_set1_epi64x(0x8000000000000000);
    packed = _mm256_blendv_epi8(packed2, packed1, _mm256_cmpgt_epi64(_mm256_xor_si256(packed2, offset), _mm256_xor_si256(packed1, offset)));
#else

#endif

    return packed;
}

/// Unary minus

template<typename T>
auto changeSign(const Packed<T> &packet) -> enableIfFloat<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_xor_ps(packet, Packed<T>(-0.0f));
#elif defined TL_HAVE_SSE2
    __m128 a = packet;
    return _mm_xor_ps(a, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
#endif
}

template<typename T>
auto changeSign(const Packed<T> &packet) -> enableIfDouble<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_xor_pd(packet, Packed<T>(-0.0));
#elif defined TL_HAVE_SSE2
    __m128d a = packet;
    return _mm_xor_pd(a, _mm_castsi128_pd(_mm_setr_epi32(0, 0x80000000, 0, 0x80000000)));
#endif
}

template<typename T>
auto changeSign(const Packed<T> &packet) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX2
    return _mm256_sub_epi8(_mm256_setzero_si256(), packet);
#elif defined TL_HAVE_SSE2
    return _mm_sub_epi8(_mm_setzero_si128(), packet);
#endif
}

template<typename T>
auto changeSign(const Packed<T> &packet) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_sub_epi16(_mm256_setzero_si256(), packet);
#elif defined TL_HAVE_SSE2
    return _mm_sub_epi16(_mm_setzero_si128(), packet);
#endif
}

template<typename T>
auto changeSign(const Packed<T> &packet) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_sub_epi32(_mm256_setzero_si256(), packet);
#elif defined TL_HAVE_SSE2
    return _mm_sub_epi32(_mm_setzero_si128(), packet);
#endif
}

template<typename T>
auto changeSign(const Packed<T> &packet) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_sub_epi64(_mm256_setzero_si256(), packet);
#elif defined TL_HAVE_SSE2
    return _mm_sub_epi64(_mm_setzero_si128(), packet);
#endif
}

template<typename T>
auto equalTo(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<float, T>::value,
    bool>
{
#ifdef TL_HAVE_AVX
    __m256 compare_result = _mm256_cmp_ps(packed1, packed2, _CMP_EQ_OQ);
    return _mm256_movemask_ps(compare_result) == 0b11111111;
#elif defined TL_HAVE_SSE
    __m128 compare_result = _mm_cmpeq_ps(packed1, packed2);
    return _mm_movemask_ps(compare_result) == 0b1111;
#endif
}

template<typename T>
auto equalTo(const Packed<T>& packed1, const Packed<T>& packed2) -> std::enable_if_t<
    std::is_same<double, T>::value,
    bool>
{
#ifdef TL_HAVE_AVX
    __m256d compare_result = _mm256_cmp_pd(packed1, packed2, _CMP_EQ_OQ);
    return _mm256_movemask_pd(compare_result) == 0b1111;
#elif defined TL_HAVE_SSE2
    __m128d compare_result = _mm_cmpeq_pd(packed1, packed2);
    return _mm_movemask_pd(compare_result) == 0b11;
#endif
}

template<typename T>
auto equalTo(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    bool>
{
#ifdef TL_HAVE_AVX2
    __m256i compare_result = _mm256_cmpeq_epi8(packed1, packed2);
    return _mm256_movemask_epi8(compare_result) == 0xffffffff;
#elif defined TL_HAVE_SSE2
    __m128i compare_result = _mm_cmpeq_epi8(packed1, packed2);
    return _mm_movemask_epi8(compare_result) == 0xffff;
#endif
}

template<typename T>
auto equalTo(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    bool>
{
#ifdef TL_HAVE_AVX2
    __m256i compare_result = _mm256_cmpeq_epi16(packed1, packed2);
    return _mm256_movemask_epi8(compare_result) == 0xffffffff;
#elif defined TL_HAVE_SSE2
    __m128i compare_result = _mm_cmpeq_epi16(packed1, packed2);
    return _mm_movemask_epi8(compare_result) == 0xffff;
#endif
}

template<typename T>
auto equalTo(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    bool>
{
#ifdef TL_HAVE_AVX2
    __m256i compare_result = _mm256_cmpeq_epi32(packed1, packed2);
    return _mm256_movemask_epi8(compare_result) == 0xffffffff;
#elif defined TL_HAVE_SSE2
    __m128i compare_result = _mm_cmpeq_epi32(packed1, packed2);
    return _mm_movemask_epi8(compare_result) == 0xffff;
#endif
}

template<typename T>
auto equalTo(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    bool>
{
#ifdef TL_HAVE_AVX2
    __m256i compare_result = _mm256_cmpeq_epi64(packed1, packed2);
    return _mm256_movemask_epi8(compare_result) == 0xffffffff;
#elif defined TL_HAVE_SSE4_1
    __m128i compare_result = _mm_cmpeq_epi64(packed1, packed2);
    return _mm_movemask_epi8(compare_result) == 0xffff;
#else
    __m128i com32 = _mm_cmpeq_epi32(packed1, packed2);                 // 32 bit compares
    __m128i com32s = _mm_shuffle_epi32(com32, 0xB1);       // swap low and high dwords
    __m128i test = _mm_and_si128(com32, com32s);           // low  &high
    __m128i teste = _mm_srai_epi32(test, 31);              // extend sign bit to 32 bits
    __m128i compare_result = _mm_shuffle_epi32(teste, 0xF5);       // extend sign bit to 64 bits
    return _mm_movemask_epi8(compare_result) == 0xffff;
#endif
}


template<typename T>
auto notEqual(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, bool>
{
#ifdef TL_HAVE_AVX
    __m256 compare_result = _mm256_cmp_ps(packed1, packed2, _CMP_NEQ_UQ);
    return _mm256_movemask_ps(compare_result) != 0;
#elif defined TL_HAVE_SSE
    __m128 compare_result = _mm_cmpneq_ps(packed1, packed2);
    return _mm_movemask_ps(compare_result) != 0;
#endif
}

template<typename T>
auto notEqual(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, bool>
{
#ifdef TL_HAVE_AVX
    __m256d compare_result = _mm256_cmp_pd(packed1, packed2, _CMP_NEQ_UQ);
    return _mm256_movemask_pd(compare_result) != 0;
#elif defined TL_HAVE_SSE2
    __m128d compare_result = _mm_cmpneq_pd(packed1, packed2);
    return _mm_movemask_pd(compare_result) != 0;
#endif
}

template<typename T>
auto notEqual(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfIntegral<T, bool>
{
#ifdef TL_HAVE_AVX2
    __m256i compare_result = _mm256_cmpeq_epi64(packed1, packed2);
    __m256i xor_value = _mm256_xor_si256(compare_result, _mm256_set1_epi32(-1));
    return _mm256_movemask_epi8(xor_value) != 0;
#elif defined TL_HAVE_SSE2
    __m128i compare_result = _mm_cmpeq_epi32(packed1, packed2);
    __m128i xor_value = _mm_xor_si128(compare_result, _mm_set1_epi32(-1));
    return _mm_movemask_epi8(xor_value) != 0;
#endif
}


// Greater than

template<typename T>
auto greaterThan(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_ps(packed2, packed1, 1);
#elif defined TL_HAVE_SSE
    return _mm_cmplt_ps(packed2, packed1);
#endif
}

template<typename T>
auto greaterThan(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_pd(packed2, packed1, 1);
#elif defined TL_HAVE_SSE2
    return _mm_cmplt_pd(packed2, packed1);
#endif
}


// Less than

template<typename T>
auto lessThan(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_ps(packed1, packed2, 1);
#elif defined TL_HAVE_SSE
    return _mm_cmplt_ps(packed1, packed2);
#endif
}

template<typename T>
auto lessThan(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_pd(packed1, packed2, 1);
#elif defined TL_HAVE_SSE2
    return _mm_cmplt_pd(packed1, packed2);
#endif
}


// Greater than or equal to

template<typename T>
auto greaterThanOrEqualTo(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_ps(packed2, packed1, 2);
#elif defined TL_HAVE_SSE
    return _mm_cmple_ps(packed2, packed1);
#endif
}

template<typename T>
auto greaterThanOrEqualTo(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_pd(packed2, packed1, 2);
#elif defined TL_HAVE_SSE2
    return _mm_cmple_pd(packed2, packed1);
#endif
}


// Less than or equal to

template<typename T>
auto lessThanOrEqualTo(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_ps(packed1, packed2, 2);
#elif defined TL_HAVE_SSE
    return _mm_cmple_ps(packed1, packed2);
#endif
}

template<typename T>
auto lessThanOrEqualTo(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
#ifdef TL_HAVE_AVX
    return _mm256_cmp_pd(packed1, packed2, 2);
#elif defined TL_HAVE_SSE2
    return _mm_cmple_pd(packed1, packed2);
#endif
}

} // namespace internal 

/// \endcond


/* Packed Implementation */



/* Packed overload operators */

template<typename T>
auto operator -(const Packed<T> &packet) -> Packed<T>
{
    return internal::changeSign(packet);
}

template<typename T>
auto operator+(const Packed<T> &packed1,
               const Packed<T> &packed2) -> Packed<T>
{
    return internal::add(packed1, packed2);
}

template<typename T>
auto operator+(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed + Packed<T>(scalar);
}

template<typename T>
auto operator+(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) + packed;
}

template<typename T>
auto operator-(const Packed<T> &packed1,
               const Packed<T> &packed2) -> Packed<T>
{
    return internal::sub(packed1, packed2);
}

template<typename T>
auto operator-(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed - Packed<T>(scalar);
}

template<typename T>
auto operator-(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) - packed;
}

template<typename T>
auto operator*(const Packed<T> &packed1,
               const Packed<T> &packed2) -> Packed<T>
{
    return internal::mul(packed1, packed2);
}

template<typename T>
auto operator*(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed * Packed<T>(scalar);
}

template<typename T>
auto operator*(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) * packed;
}

template<typename T>
auto operator/(const Packed<T> &packed1,
               const Packed<T> &packed2) -> Packed<T>
{
    return internal::div(packed1, packed2);
}

template<typename T>
auto operator/(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed / Packed<T>(scalar);
}

template<typename T>
auto operator/(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) / packed;
}

/* Comparison Operators */


template<typename T> 
static auto operator ==(const Packed<T> &packed1,
                        const Packed<T> &packed2) -> bool
{
    return internal::equalTo(packed1, packed2);
}

template<typename T> 
static auto operator !=(const Packed<T> &packed1,
                        const Packed<T> &packed2) -> bool
{
    return internal::notEqual(packed1, packed2);
}

/*!
 * \brief Compare packed in packed1 and packed2 for greater-than, and return the results.
 */
template<typename T> 
static auto operator > (const Packed<T> &packed1, 
                        const Packed<T> &packed2) -> Packed<T>
{
    return internal::greaterThan(packed1, packed2);
}


/*!
 * \brief Compare packed elements in packed1 and packed2 for 
 */
template<typename T> 
static auto operator < (const Packed<T> &packed1, 
                        const Packed<T> &packed2) -> Packed<T>
{
    return internal::lessThan(packed1, packed2);
}

template<typename T> 
static auto operator >= (const Packed<T> &packed1, 
                         const Packed<T> &packed2) -> Packed<T>
{
    return internal::greaterThanOrEqualTo(packed1, packed2);
}

template<typename T> 
static auto operator <= (const Packed<T> &packed1, 
                         const Packed<T> &packed2) -> Packed<T>
{
    return internal::lessThanOrEqualTo(packed1, packed2);
}



template<typename T>
Packed<T>::Packed(const Packed &packed)
  : mValue(packed.mValue)
{
}

template<typename T>
Packed<T>::Packed(Packed &&packed) TL_NOEXCEPT
  : mValue(std::move(packed.mValue))
{
}

template<typename T>
Packed<T>::Packed(simd_type packed)
  : mValue(packed)
{
}

template<typename T>
Packed<T>::Packed(value_type scalar)
    : mValue(internal::set(scalar))
{
}

template<typename T>
void Packed<T>::loadAligned(const value_type *src)
{
    mValue = internal::loadPackedAligned(src);
}

template<typename T>
void Packed<T>::loadUnaligned(const value_type *src)
{
    mValue = internal::loadPackedUnaligned(src);
}

template<typename T>
void Packed<T>::storeAligned(value_type *dst) const
{
    internal::storePackedAligned(dst, mValue);
}

template<typename T>
void Packed<T>::storeUnaligned(value_type *dst) const
{
    internal::storePackedUnaligned(dst, mValue);
}

template<typename T>
void Packed<T>::setScalar(value_type value)
{
    mValue = internal::set(value);
}

template<typename T>
auto Packed<T>::operator=(simd_type packed) -> Packed<T>&
{
    mValue = packed;
    return *this;
}

template<typename T>
constexpr auto Packed<T>::size() -> size_t
{
    return PackedTraits<Packed<T>>::size;
}

template<typename T>
Packed<T>::operator simd_type() const
{
    return mValue;
}

template<typename T>
auto Packed<T>::operator=(const Packed<T> &packed) -> Packed<T>&
{
    if (this != &packed) {
        mValue = packed;
    }

    return *this;
}

template<typename T>
auto Packed<T>::operator=(Packed<T> &&packed) TL_NOEXCEPT -> Packed<T>&
{
    if (this != &packed) {
        mValue = std::move(packed);
    }

    return *this;
}

template<typename T>
auto Packed<T>::operator+=(const Packed<T> &packed) -> Packed<T>&
{
    *this = *this + packed;
    return *this;
}

template<typename T>
auto Packed<T>::operator-=(const Packed<T>& packed) -> Packed<T>&
{
    *this = *this - packed;
    return *this;
}

template<typename T>
auto Packed<T>::operator*=(const Packed<T>& packed) -> Packed<T>&
{
    *this = *this * packed;
    return *this;
}

template<typename T>
auto Packed<T>::operator/=(const Packed<T>& packed) -> Packed<T>&
{
    *this = *this / packed;
    return *this;
}

template<typename T>
auto Packed<T>::operator++(int) -> Packed<T>
{
    Packed<T> packet = *this;
    *this += consts::one<T>;
    return *packet;
}

template<typename T>
auto Packed<T>::operator++() -> Packed<T>&
{
    *this += consts::one<T>;
    return *this;
}

template<typename T>
auto Packed<T>::operator--(int) -> Packed<T>
{
    Packed<T> packet = *this;
    *this -= consts::one<T>;
    return *packet;
}

template<typename T>
auto Packed<T>::operator--() -> Packed<T>&
{
    *this -= consts::one<T>;
    return *this;
}

template<typename T>
auto Packed<T>::sum() -> T
{
    return internal::horizontal_sum(*this);
}

template<typename T> 
auto Packed<T>::zero() -> Packed
{
    return internal::setZero<T>();
}


/* Transpose */

template<typename T>
auto transposeMatrix4x4(Packed<T> &r1,
                        Packed<T> &r2,
                        Packed<T> &r3,
                        Packed<T> &r4) -> std::enable_if_t<
    std::is_same<float, std::remove_cv_t<T>>::value,
    void>
{
    __m128 tmp1 = _mm_unpacklo_ps(r1, r2);
    __m128 tmp2 = _mm_unpackhi_ps(r1, r2);
    __m128 tmp3 = _mm_unpacklo_ps(r3, r4);
    __m128 tmp4 = _mm_unpackhi_ps(r3, r4);

    r1 = _mm_movelh_ps(tmp1, tmp3);
    r2 = _mm_movehl_ps(tmp3, tmp1);
    r3 = _mm_movelh_ps(tmp2, tmp4);
    r4 = _mm_movehl_ps(tmp4, tmp2);


/// _MM_TRANSPOSE4_PS lo hace:
//    __m128 _Tmp3, _Tmp2, _Tmp1, _Tmp0;                          
//        
//        _Tmp0   = _mm_shuffle_ps((row0), (row1), 0x44);          
//        _Tmp2   = _mm_shuffle_ps((row0), (row1), 0xEE);          
//        _Tmp1   = _mm_shuffle_ps((row2), (row3), 0x44);          
//        _Tmp3   = _mm_shuffle_ps((row2), (row3), 0xEE);          
//        
//        (row0) = _mm_shuffle_ps(_Tmp0, _Tmp1, 0x88);              
//        (row1) = _mm_shuffle_ps(_Tmp0, _Tmp1, 0xDD);              
//        (row2) = _mm_shuffle_ps(_Tmp2, _Tmp3, 0x88);              
//        (row3) = _mm_shuffle_ps(_Tmp2, _Tmp3, 0xDD); 
}


/*! \} */ // end of Math

} // End namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS

