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

/// \cond

namespace tl
{

template<typename T>
class Packed;

namespace detail
{


/// Unary minus

template<SignedArithmetic T>
[[nodiscard]]
auto changeSign(const Packed<T> &packet) noexcept -> Packed<T>
{
    if constexpr (Floating<T>) {

#ifdef TL_HAVE_AVX

        if constexpr (std::same_as<T, float>) {
            return _mm256_xor_ps(packet, Packed<T>(-0.0f));
        } else {
            return _mm256_xor_pd(packet, Packed<T>(-0.0));
        }

#elif defined(TL_HAVE_SSE2)

        if constexpr (std::same_as<T, float>) {
            return _mm_xor_ps(packet, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
        } else {
            return _mm_xor_pd(packet, _mm_castsi128_pd(_mm_setr_epi32(0, 0x80000000, 0, 0x80000000)));
        }
#endif

    } else {

#ifdef TL_HAVE_AVX2

        auto zero = _mm256_setzero_si256();

        if constexpr (sizeof(T) == 1) {
            return _mm256_sub_epi8(zero, packet);
        } else if constexpr (sizeof(T) == 2) {
            return _mm256_sub_epi16(zero, packet);
        } else if constexpr (sizeof(T) == 4) {
            return _mm256_sub_epi32(zero, packet);
        } else {
            return _mm256_sub_epi64(zero, packet);
        }

#elif defined(TL_HAVE_SSE2)

        auto zero = _mm_setzero_si128();

        if constexpr (sizeof(T) == 1) {
            return _mm_sub_epi8(zero, packet);
        } else if constexpr (sizeof(T) == 2) {
            return _mm_sub_epi16(zero, packet);
        } else if constexpr (sizeof(T) == 4) {
            return _mm_sub_epi32(zero, packet);
        } else {
            return _mm_sub_epi64(zero, packet);
        }

#endif
    }
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
