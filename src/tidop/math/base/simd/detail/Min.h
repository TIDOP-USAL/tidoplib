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
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX512
   packed = _mm512_min_epi8(packed1, packed2);
#elif defined TL_HAVE_AVX2
   packed = _mm256_min_epi8(packed1, packed2);
#elif defined TL_HAVE_SSE4_1
   packed = _mm_min_epi8(packed1, packed2);
#else  // SSE2
    __m128i signbit = _mm_set1_epi32(0x80808080);
    __m128i a1 = _mm_xor_si128(packed1, signbit);
    __m128i b1 = _mm_xor_si128(packed2, signbit);
    __m128i m1 = _mm_min_epu8(a1, b1);
    packed = _mm_xor_si128(m1, signbit);
#endif

    return packed;
}

template<typename T>
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX512
   packed = _mm512_min_epu8(packed1, packed2);
#elif defined TL_HAVE_AVX2
   packed = _mm256_min_epu8(packed1, packed2);
#else
    packed = _mm_min_epu8(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX512F
    packed = _mm512_min_epi16(packed1, packed2);
#elif defined TL_HAVE_AVX2
   packed = _mm256_min_epi16(packed1, packed2);
#else
    packed = _mm_min_epi16(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    Packed<T>>
{
   Packed<T> packed;

#ifdef TL_HAVE_AVX512F
    packed = _mm512_min_epu16(packed1, packed2);
#elif defined TL_HAVE_AVX2
   packed = _mm256_min_epu16(packed1, packed2);
#elif defined TL_HAVE_SSE4_1
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
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX512F
    packed = _mm512_min_epi32(packed1, packed2);
#elif defined TL_HAVE_AVX2
    packed = _mm256_min_epi32(packed1, packed2);
#elif defined TL_HAVE_SSE4_1
    packed = _mm_min_epi32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    // Compares the 4 signed 32-bit integers in packed1 and the 4 signed 32-bit integers in packed2 for greater than.
    __m128i greater = _mm_cmpgt_epi32(packed1, packed2);
    packed = _mm_or_si128(_mm_and_si128(greater, packed2), _mm_andnot_si128(greater, packed1));
#endif

    return packed;
}

template<typename T>
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX512F
    packed = _mm512_min_epu32(packed1, packed2);
#elif defined TL_HAVE_AVX2
    packed = _mm256_min_epu32(packed1, packed2);
#elif defined TL_HAVE_SSE4_1
    packed = _mm_min_epu32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    __m128i signbit = _mm_set1_epi32(0x80000000);
    __m128i greater = _mm_cmpgt_epi32(_mm_xor_si128(packed1, signbit), _mm_xor_si128(packed2, signbit));
    packed = _mm_or_si128(_mm_and_si128(greater, packed2), _mm_andnot_si128(greater, packed1));
#endif

    return packed;
}

template<typename T>
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX512F
    packed = _mm512_min_epi64(packed1, packed2);
#elif defined TL_HAVE_AVX2
    __m256i mask = _mm256_cmpgt_epi64(packed1, packed2);
    packed = _mm256_blendv_epi8(packed1, packed2, mask);
#elif defined(TL_HAVE_SSE4_2)
    __m128i mask = _mm_cmpgt_epi64(packed1, packed2);
    packed = _mm_blendv_epi8(packed1, packed2, mask);
#else
    // Emulación 64‑bit signed con operaciones de 32 bits
    __m128i hi1 = _mm_shuffle_epi32(packed1, 0xB1);
    __m128i hi2 = _mm_shuffle_epi32(packed2, 0xB1);
    __m128i cmp_hi = _mm_cmpgt_epi32(hi1, hi2);
    __m128i eq_hi = _mm_cmpeq_epi32(hi1, hi2);
    __m128i mask = _mm_or_si128(cmp_hi,
        _mm_and_si128(eq_hi, _mm_cmpgt_epi32(packed1, packed2)));
    packed = _mm_or_si128(_mm_and_si128(mask, packed2),
        _mm_andnot_si128(mask, packed1));
#endif

    return packed;
}

template<typename T>
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX512F
    packed = _mm512_min_epu64(packed1, packed2);
#elif defined TL_HAVE_AVX2
    __m256i offset = _mm256_set1_epi64x(0x8000000000000000);
    __m256i a = _mm256_xor_si256(packed1, offset);
    __m256i b = _mm256_xor_si256(packed2, offset);
    __m256i mask = _mm256_cmpgt_epi64(a, b);
    packed = _mm256_blendv_epi8(packed1, packed2, mask);
#elif defined(TL_HAVE_SSE4_2)
    const __m128i offset = _mm_set1_epi64x(0x8000000000000000ULL);
    __m128i a = _mm_xor_si128(packed1, offset);
    __m128i b = _mm_xor_si128(packed2, offset);
    __m128i mask = _mm_cmpgt_epi64(a, b);
    packed = _mm_blendv_epi8(packed1, packed2, mask);
#else
    //// Convertir a signed y reutilizar min<int64_t>
    //const __m128i offset = _mm_set1_epi64x(0x8000000000000000ULL);
    //__m128i a = _mm_xor_si128(packed1, offset);
    //__m128i b = _mm_xor_si128(packed2, offset);
    //packed = _mm_xor_si128(
    //    min<int64_t>(*reinterpret_cast<const Packed<int64_t>*>(&a),
    //                 *reinterpret_cast<const Packed<int64_t>*>(&b)),
    //    offset);
// SSE2 puro: No hay cmpgt_epi64 ni blendv_epi8
    const __m128i offset = _mm_set1_epi64x(0x8000000000000000ULL);
    __m128i a_s = _mm_xor_si128(packed1, offset);
    __m128i b_s = _mm_xor_si128(packed2, offset);

    // 1. Emulación de comparación > (64-bit signed)
    __m128i hi_a = _mm_shuffle_epi32(a_s, 0xFD);
    __m128i hi_b = _mm_shuffle_epi32(b_s, 0xFD);
    __m128i gt_hi = _mm_cmpgt_epi32(hi_a, hi_b);
    __m128i eq_hi = _mm_cmpeq_epi32(hi_a, hi_b);

    __m128i bias32 = _mm_set1_epi32(0x80000000);
    __m128i lo_a = _mm_xor_si128(_mm_shuffle_epi32(a_s, 0xA0), bias32);
    __m128i lo_b = _mm_xor_si128(_mm_shuffle_epi32(b_s, 0xA0), bias32);
    __m128i gt_lo = _mm_cmpgt_epi32(lo_a, lo_b);

    // mask es true si packed1 > packed2
    __m128i mask = _mm_or_si128(gt_hi, _mm_and_si128(eq_hi, gt_lo));
    mask = _mm_shuffle_epi32(mask, 0xF5); // Propagar máscara a los 64 bits

    // 2. Selección bitwise (Simulando blendv):
    // Si mask (p1 > p2) es true, elegimos p2. Si es false (p1 <= p2), elegimos p1.
    // Fórmula: (mask & packed2) | (~mask & packed1)
    packed = _mm_or_si128(
        _mm_and_si128(mask, packed2),
        _mm_andnot_si128(mask, packed1)
    );
#endif

    return packed;
}

template<Floating T>
auto min_impl(const Packed<T> &packed1, const Packed<T> &packed2) -> Packed<T>
{
#if defined(TL_HAVE_AVX512)
    if constexpr (std::same_as<T, float>)
        return _mm512_min_ps(packed1, packed2);
    else
        return _mm512_min_pd(packed1, packed2);
#elif defined(TL_HAVE_AVX)
    if constexpr (std::same_as<T, float>)
        return _mm256_min_ps(packed1, packed2);
    else 
        return _mm256_min_pd(packed1, packed2);
#else
    if constexpr (std::same_as<T, float>)
        return _mm_min_ps(packed1, packed2);
    else 
        return _mm_min_pd(packed1, packed2);
#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
