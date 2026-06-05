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

#include "tidop/core/base/Type.h"
#include "tidop/math/base/simd/Intrinsics.h"
#include "tidop/math/base/simd/detail/SignBias.h"
#include "tidop/math/base/simd/detail/BitwiseXor.h"

/// \cond

namespace tl
{

template<typename T>
class Packed;

namespace detail
{

// Greater than

//template<Arithmetic T>
//[[nodiscard]]
//auto greaterThan(const Packed<T> &packed1, const Packed<T> &packed2) noexcept -> Packed<T>
//{
//    if constexpr (Floating<T>) {
//
//        if constexpr (std::same_as<T, float>) {
//
//#ifdef TL_HAVE_AVX512
//            return _mm512_cmp_ps_mask(packed1, packed2, 14);
//#elif defined TL_HAVE_AVX
//            return _mm256_cmp_ps(packed2, packed1, 1);
//#elif defined TL_HAVE_SSE
//            return _mm_cmplt_ps(packed2, packed1);
//#endif
//
//        } else {
//
//#ifdef TL_HAVE_AVX512
//            return _mm512_cmp_pd_mask(packed1, packed2, 14);
//#elif defined TL_HAVE_AVX
//            return _mm256_cmp_pd(packed2, packed1, 1);
//#elif defined TL_HAVE_SSE2
//            return _mm_cmplt_pd(packed2, packed1);
//#endif
//
//        }
//
//    } else {
//
//    }
//}

template<typename T>
[[nodiscard]]
auto greaterThan(const Packed<T> &a, const Packed<T> &b) -> enableIfFloat<T, Packed<T>>
{
#ifdef TL_HAVE_AVX512
    __mmask16 mask = _mm512_cmp_ps_mask(a, b, _CMP_GT_OQ);
    return _mm512_castsi512_ps(_mm512_maskz_set1_epi32(mask, -1));
#elif defined TL_HAVE_AVX
    return _mm256_cmp_ps(a, b, _CMP_GT_OQ);
#elif defined TL_HAVE_SSE
    return _mm_cmplt_ps(b, a);
#endif
}

template<typename T>
[[nodiscard]]
auto greaterThan(const Packed<T> &a, const Packed<T> &b) -> enableIfDouble<T, Packed<T>>
{
#ifdef TL_HAVE_AVX512
    __mmask8 mask = _mm512_cmp_pd_mask(a, b, _CMP_GT_OQ);
    return _mm512_castsi512_pd(_mm512_maskz_set1_epi64(mask, -1LL));
#elif defined TL_HAVE_AVX
    return _mm256_cmp_pd(a, b, _CMP_GT_OQ);
#elif defined TL_HAVE_SSE2
    return _mm_cmplt_pd(b, a);
#endif
}

template<typename T>
[[nodiscard]]
auto greaterThan(const Packed<T> &a, const Packed<T> &b) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX512
    __mmask64 mask; // 64 lanes en un registro de 512 bits
    if constexpr (SignedIntegral<T>)
        mask = _mm512_cmp_epi8_mask(a, b, _MM_CMPINT_GT);
    else
        mask = _mm512_cmp_epu8_mask(a, b, _MM_CMPINT_GT);
    return _mm512_maskz_set1_epi8(mask, -1);
#elif defined TL_HAVE_AVX2
    if constexpr (SignedIntegral<T>) {
        return _mm256_cmpgt_epi8(a, b);
    } else {
        auto bias = signBias<T>();
        auto a_s = bitwise_xor_impl(a, bias);
        auto b_s = bitwise_xor_impl(b, bias);
        return _mm256_cmpgt_epi8(a_s, b_s);
    }
#else // TL_HAVE_SSE2
    if constexpr (SignedIntegral<T>) {
        return _mm_cmpgt_epi8(a, b);
    } else {
        const __m128i bias = _mm_set1_epi8(static_cast<char>(0x80));
        __m128i a_s = _mm_xor_si128(a, bias);
        __m128i b_s = _mm_xor_si128(b, bias);
        return _mm_cmpgt_epi8(a_s, b_s);
    }
#endif
}

template<typename T>
[[nodiscard]]
auto greaterThan(const Packed<T> &a, const Packed<T> &b) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX512
    __mmask32 mask; // 32 lanes en un registro de 512 bits
    if constexpr (SignedIntegral<T>)
        mask = _mm512_cmp_epi16_mask(a, b, _MM_CMPINT_GT);
    else
        mask = _mm512_cmp_epu16_mask(a, b, _MM_CMPINT_GT);
    return _mm512_maskz_set1_epi16(mask, -1);
#elif defined TL_HAVE_AVX2
    if constexpr (SignedIntegral<T>) {
        return _mm256_cmpgt_epi16(a, b);
    } else {
        auto bias = signBias<T>();
        auto a_s = bitwise_xor_impl(a, bias);
        auto b_s = bitwise_xor_impl(b, bias);
        return _mm256_cmpgt_epi16(a_s, b_s);
    }
#else // TL_HAVE_SSE2
    if constexpr (SignedIntegral<T>) {
        return _mm_cmpgt_epi16(a, b);
    } else {
        const __m128i bias = _mm_set1_epi16(static_cast<short>(0x8000));
        __m128i a_s = _mm_xor_si128(a, bias);
        __m128i b_s = _mm_xor_si128(b, bias);
        return _mm_cmpgt_epi16(a_s, b_s);
    }
#endif
}

template<typename T>
[[nodiscard]]
auto greaterThan(const Packed<T> &a, const Packed<T> &b) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX512
    __mmask16 mask;
    if constexpr (SignedIntegral<T>)
        mask = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_GT);
    else
        mask = _mm512_cmp_epu32_mask(a, b, _MM_CMPINT_GT);
    return _mm512_maskz_set1_epi32(mask, -1);
#elif defined  TL_HAVE_AVX2
    if constexpr (SignedIntegral<T>) {
        return _mm256_cmpgt_epi32(a, b);
    } else {
        auto bias = signBias<T>();
        auto a_s = bitwise_xor_impl(a, bias);
        auto b_s = bitwise_xor_impl(b, bias);
        return _mm256_cmpgt_epi32(a_s, b_s);
    }
#else // TL_HAVE_SSE2
    if constexpr (SignedIntegral<T>) {
        return _mm_cmpgt_epi32(a, b);
    } else {
        const __m128i bias = _mm_set1_epi32(static_cast<int>(0x80000000));
        __m128i a_s = _mm_xor_si128(a, bias);
        __m128i b_s = _mm_xor_si128(b, bias);
        return _mm_cmpgt_epi32(a_s, b_s);
    }
#endif
}

template<typename T>
[[nodiscard]]
auto greaterThan(const Packed<T> &a, const Packed<T> &b) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    Packed<T>>
{
#ifdef TL_HAVE_AVX512
    __mmask8 mask;
    if constexpr (SignedIntegral<T>)
        mask = _mm512_cmp_epi64_mask(a, b, _MM_CMPINT_GT);
    else
        mask = _mm512_cmp_epu64_mask(a, b, _MM_CMPINT_GT);
    return _mm512_maskz_set1_epi64(mask, -1);
#elif defined  TL_HAVE_AVX2
    if constexpr (SignedIntegral<T>) {
        return _mm256_cmpgt_epi64(a, b);
    } else {
        auto bias = signBias<T>();
        auto a_s = bitwise_xor_impl(a, bias);
        auto b_s = bitwise_xor_impl(b, bias);
        return _mm256_cmpgt_epi64(a_s, b_s);
    }
#else
    auto get_gt64 = [](__m128i a, __m128i b) {
        __m128i sign_bit = _mm_set1_epi32(0x80000000);
        // Parte alta (signed)
        __m128i hi_a = _mm_shuffle_epi32(a, 0xFD);
        __m128i hi_b = _mm_shuffle_epi32(b, 0xFD);
        __m128i gt_hi = _mm_cmpgt_epi32(hi_a, hi_b);
        __m128i eq_hi = _mm_cmpeq_epi32(hi_a, hi_b);

        // Parte baja (unsigned -> bias)
        __m128i lo_a = _mm_xor_si128(_mm_shuffle_epi32(a, 0xA0), sign_bit);
        __m128i lo_b = _mm_xor_si128(_mm_shuffle_epi32(b, 0xA0), sign_bit);
        __m128i gt_lo = _mm_cmpgt_epi32(lo_a, lo_b);

        __m128i res = _mm_or_si128(gt_hi, _mm_and_si128(eq_hi, gt_lo));
        return _mm_shuffle_epi32(res, 0xF5); // Duplicar máscara a 64 bits
        };

    if constexpr (SignedIntegral<T>) {
        return get_gt64(a, b);
    } else {
        const __m128i bias = _mm_set1_epi64x(0x8000000000000000ULL);
        return get_gt64(_mm_xor_si128(a, bias), _mm_xor_si128(b, bias));
    }
#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
