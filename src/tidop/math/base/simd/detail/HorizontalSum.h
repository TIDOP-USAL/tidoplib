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
[[nodiscard]]
auto horizontal_sum_impl(const Packed<T> &packed) -> enableIfFloat<T, T>
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
[[nodiscard]]
auto horizontal_sum_impl(const Packed<T> &packed) -> enableIfDouble<T, T>
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
[[nodiscard]]
auto horizontal_sum_impl(const Packed<T> &packed) -> std::enable_if_t<
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
[[nodiscard]]
auto horizontal_sum_impl(const Packed<T> &packed) -> std::enable_if_t<
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
[[nodiscard]]
auto horizontal_sum_impl(const Packed<T> &packed) -> std::enable_if_t<
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
[[nodiscard]]
auto horizontal_sum_impl(const Packed<T>& packed) -> std::enable_if_t<
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

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
