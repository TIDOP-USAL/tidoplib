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
#include "tidop/math/base/simd/detail/BitwiseNot.h"
#include "tidop/math/base/simd/detail/GreaterThan.h"
#include "tidop/math/base/simd/detail/EqualTo.h"

/// \cond

namespace tl
{

template<typename T>
class Packed;

enum class CompareOp
{
    lt,
    le,
    gt,
    ge,
    eq,
    ne
};


namespace detail
{

template<CompareOp Op>
constexpr int getPredicate()
{
    if constexpr (Op == CompareOp::lt) return _CMP_LT_OQ;
    else if constexpr (Op == CompareOp::le) return _CMP_LE_OQ;
    else if constexpr (Op == CompareOp::gt) return _CMP_GT_OQ;
    else if constexpr (Op == CompareOp::ge) return _CMP_GE_OQ;
    else if constexpr (Op == CompareOp::eq) return _CMP_EQ_OQ;
    else if constexpr (Op == CompareOp::ne) return _CMP_NEQ_UQ; // Se usa Unordered Quiet ya NaN nunca es igual a nada. Si un operando es NaN devuelve true
    else static_assert(always_false_v<Op>, "Unsupported comparison operation");
};

template<CompareOp Op, Floating T>
auto compare_float(const Packed<T> &a, const Packed<T> &b) -> Packed<T>
{
    constexpr int predicate = getPredicate<Op>();

#if defined(TL_HAVE_AVX512)
    if constexpr (std::is_same_v<T, float>) {
        __mmask16 mask = _mm512_cmp_ps_mask(a, b, predicate);
        return _mm512_castsi512_ps(_mm512_maskz_set1_epi32(mask, -1));
    } else {
        __mmask8 mask = _mm512_cmp_pd_mask(a, b, predicate);
        return _mm512_castsi512_pd(_mm512_maskz_set1_epi64(mask, -1LL));
    }
#elif defined(TL_HAVE_AVX)
    if constexpr (std::is_same_v<T, float>)
        return _mm256_cmp_ps(a, b, predicate);
    else
        return _mm256_cmp_pd(a, b, predicate);
#else
    if constexpr (std::is_same_v<T, float>) {

        if constexpr (Op == CompareOp::lt)
            return _mm_cmplt_ps(a, b);
        else if constexpr (Op == CompareOp::le)
            return _mm_cmple_ps(a, b);
        else if constexpr (Op == CompareOp::gt)
            return _mm_cmplt_ps(b, a); // a > b ⇔ b < a
        else if constexpr (Op == CompareOp::ge)
            return _mm_cmple_ps(b, a);
        else if constexpr (Op == CompareOp::eq)
            return _mm_cmpeq_ps(a, b);
        else if constexpr (Op == CompareOp::ne)
            return _mm_cmpneq_ps(a, b);
        else
            static_assert(always_false_v<Op>, "Unsupported comparison operation");

    } else {

        // double: map predicate to appropriate SSE intrinsic
        // SSE2 has _mm_cmplt_pd, _mm_cmple_pd, etc.
        if constexpr (Op == CompareOp::lt)
            return _mm_cmplt_pd(a, b);
        else if constexpr (Op == CompareOp::le)
            return _mm_cmple_pd(a, b);
        else if constexpr (Op == CompareOp::gt)
            return _mm_cmplt_pd(b, a); // a > b ⇔ b < a
        else if constexpr (Op == CompareOp::ge)
            return _mm_cmple_pd(b, a);
        else if constexpr (Op == CompareOp::eq)
            return _mm_cmpeq_pd(a, b);
        else if constexpr (Op == CompareOp::ne)
            return _mm_cmpneq_pd(a, b);
        else
            static_assert(always_false_v<Op>, "Unsupported comparison operation");

    }
#endif
}

template<CompareOp Op, Integral T>
auto compare_integral(const Packed<T> &a, const Packed<T> &b) -> Packed<T>
{
#if defined(TL_HAVE_AVX512)

    constexpr int predicate = getPredicate<Op>();

    if constexpr (sizeof(T) == 1) {
        __mmask64 mask; // 64 lanes en un registro de 512 bits
        if constexpr (SignedIntegral<T>)
            mask = _mm512_cmp_epi8_mask(a, b, predicate);
        else
            mask = _mm512_cmp_epu8_mask(a, b, predicate);
        return _mm512_maskz_set1_epi8(mask, -1);
    } else if constexpr (sizeof(T) == 2) {
        __mmask32 mask; // 32 lanes en un registro de 512 bits
        if constexpr (SignedIntegral<T>)
            mask = _mm512_cmp_epi16_mask(a, b, predicate);
        else
            mask = _mm512_cmp_epu16_mask(a, b, predicate);
        return _mm512_maskz_set1_epi16(mask, -1);
    } else if constexpr (sizeof(T) == 4) {
        __mmask16 mask;
        if constexpr (SignedIntegral<T>)
            mask = _mm512_cmp_epi32_mask(a, b, predicate);
        else
            mask = _mm512_cmp_epu32_mask(a, b, predicate);
        return _mm512_maskz_set1_epi32(mask, -1);
    } else {
        __mmask8 mask;
        if constexpr (SignedIntegral<T>)
            mask = _mm512_cmp_epi64_mask(a, b, predicate);
        else
            mask = _mm512_cmp_epu64_mask(a, b, predicate);
        return _mm512_maskz_set1_epi64(mask, -1);
    }

#else

    if constexpr (Op == CompareOp::lt) {
        return detail::greaterThan(b, a);       // a < b  ⇔ b > a
    } else if constexpr (Op == CompareOp::le) {
        // a <= b  ⇔ !(a > b)
        return bitwiseNot(detail::greaterThan(a, b));
    } else if constexpr (Op == CompareOp::gt) {
        return detail::greaterThan(a, b);
    } else if constexpr (Op == CompareOp::ge) {
        // a >= b  ⇔ !(b > a)
        return bitwiseNot(detail::greaterThan(b, a));
    } else if constexpr (Op == CompareOp::eq) {
        return detail::equalTo(a, b);
    } else if constexpr (Op == CompareOp::ne) {
        return bitwiseNot(detail::equalTo(a, b));
    } else {
        static_assert(always_false_v<Op>, "Unsupported comparison operation");
    }
#endif
}

} // namespace detail 

template<CompareOp Op, Arithmetic T>
[[nodiscard]]
auto compare(const Packed<T> &a, const Packed<T> &b) -> Packed<T>
{
    if constexpr (Floating<T>) {
        return detail::compare_float<Op, T>(a, b);
    } else {
        return detail::compare_integral<Op, T>(a, b);
    }
}


} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
