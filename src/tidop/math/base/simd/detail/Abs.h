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
#include "tidop/core/base/Concepts.h"

#ifdef TL_HAVE_SIMD_INTRINSICS

#include "tidop/math/base/simd/Intrinsics.h"

/// \cond

namespace tl
{

template<typename T>
class Packed;

namespace detail
{

template<Floating T>
[[nodiscard]]
auto abs_impl(const Packed<T> &packed) -> Packed<T>
{
#ifdef TL_HAVE_AVX
    if constexpr (std::same_as<T, float>) {
        static const __m256 mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF));
        return _mm256_and_ps(packed, mask);
    } else {
        static const __m256d mask = _mm256_castsi256_pd(_mm256_set1_epi64x(0x7FFFFFFFFFFFFFFFLL));
        return _mm256_and_pd(packed, mask);
    }
#else // SSE
    if constexpr (std::same_as<T, float>) {
        static const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
        return _mm_and_ps(packed, mask);
    } else {
        static const __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(0x7FFFFFFFFFFFFFFFLL));
        return _mm_and_pd(packed, mask);
    }
#endif
}

template<SignedIntegral T>
[[nodiscard]]
auto abs_impl(const Packed<T> &packed) -> Packed<T>
{
#ifdef TL_HAVE_AVX2
    if constexpr (sizeof(T) == 1) {
        return _mm256_abs_epi8(packed);
    } else if constexpr (sizeof(T) == 2) {
        return _mm256_abs_epi16(packed);
    } else if constexpr (sizeof(T) == 4) {
        return _mm256_abs_epi32(packed);
    } else {
        static const __m256i mask = _mm256_set1_epi64x(0x7FFFFFFFFFFFFFFFLL);
        return _mm256_and_si256(packed, mask);
    }
#elif defined(TL_HAVE_SSE2)
    if constexpr (sizeof(T) == 1) {
        return _mm_abs_epi8(packed);
    } else if constexpr (sizeof(T) == 2) {
        return _mm_abs_epi16(packed);
    } else if constexpr (sizeof(T) == 4) {
        return _mm_abs_epi32(packed);
    } else {
        static const __m128i mask = _mm_set1_epi64x(0x7FFFFFFFFFFFFFFFLL);
        return _mm_and_si128(packed, mask);
    }
#endif
}

template<UnsignedIntegral T>
[[nodiscard]]
auto abs_impl(const Packed<T> &packed) -> Packed<T>
{
    return packed;
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
