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

#ifdef TL_HAVE_SIMD_INTRINSICS

#include "tidop/math/base/simd/Intrinsics.h"

/// \cond

namespace tl
{

template<typename T>
class Packed;

namespace detail
{


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


} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
