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

template<Integral T>
[[nodiscard]]
auto equalTo(const Packed<T> &a,
             const Packed<T> &b) noexcept -> Packed<T>
{
    if constexpr (sizeof(T) == 1) {

#ifdef TL_HAVE_AVX2
        return _mm256_cmpeq_epi8(a, b);
#else
        return _mm_cmpeq_epi8(a, b);
#endif

    } else if constexpr (sizeof(T) == 2) {

#ifdef TL_HAVE_AVX2
        return _mm256_cmpeq_epi16(a, b);
#else
        return _mm_cmpeq_epi16(a, b);
#endif

    } else if constexpr (sizeof(T) == 4) {

#ifdef TL_HAVE_AVX2
        return _mm256_cmpeq_epi32(a, b);
#else
        return _mm_cmpeq_epi32(a, b);
#endif

    } else {

#ifdef TL_HAVE_AVX2
        return _mm256_cmpeq_epi64(a, b);
#elif defined(TL_HAVE_SSE4_1)
        return _mm_cmpeq_epi64(a, b);
#else
        __m128i com32 = _mm_cmpeq_epi32(a, b);                         // 32 bit compares
        __m128i com32s = _mm_shuffle_epi32(com32, 0xB1);               // swap low and high dwords
        __m128i test = _mm_and_si128(com32, com32s);                   // low  &high
         __m128i teste = _mm_srai_epi32(test, 31);                     // extend sign bit to 32 bits
        return _mm_shuffle_epi32(teste, 0xF5);                         // extend sign bit to 64 bits
#endif
    }
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
