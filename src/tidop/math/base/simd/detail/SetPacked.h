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

template<Arithmetic T>
[[nodiscard]]
auto set_impl(T data) noexcept -> Packed<T>
{
    if constexpr (Floating<T>) {

#ifdef TL_HAVE_AVX

        if constexpr (std::same_as<T, float>) {
            return _mm256_set1_ps(data);
        } else {
            return _mm256_set1_pd(data);
        }

#elif defined(TL_HAVE_SSE2)

        if constexpr (std::same_as<T, float>) {
            return _mm_set1_ps(data);
        } else {
            return _mm_set1_pd(data);
        }
#endif

    } else {

#ifdef TL_HAVE_AVX2

        if constexpr (sizeof(T) == 1) {
            return _mm256_set1_epi8(data);
        } else if constexpr (sizeof(T) == 2) {
            return _mm256_set1_epi16(data);
        } else if constexpr (sizeof(T) == 4) {
            return _mm256_set1_epi32(data);
        } else {
            return _mm256_set1_epi64x(data);
        }

#elif defined(TL_HAVE_SSE2)

        if constexpr (sizeof(T) == 1) {
            return _mm_set1_epi8(a);
        } else if constexpr (sizeof(T) == 2) {
            return _mm_set1_epi16(a);
        } else if constexpr (sizeof(T) == 4) {
            return _mm_set1_epi32(a);
        } else {
            return _mm_set1_epi64x(a);
        }

#endif
    }
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
