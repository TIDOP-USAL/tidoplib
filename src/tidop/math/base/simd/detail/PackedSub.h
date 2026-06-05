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
auto sub_impl(const Packed<T> &a, const Packed<T> &b) noexcept -> Packed<T>
{
    if constexpr (Floating<T>) {

#ifdef TL_HAVE_AVX

        if constexpr (std::same_as<T, float>) {
            return _mm256_sub_ps(a, b);
        } else {
            return _mm256_sub_pd(a, b);
        }

#elif defined(TL_HAVE_SSE2)

        if constexpr (std::same_as<T, float>) {
            return _mm_sub_ps(a, b);
        } else {
            return _mm_sub_pd(a, b);
        }
#endif

    } else {

#ifdef TL_HAVE_AVX2

        if constexpr (sizeof(T) == 1) {
            return _mm256_sub_epi8(a, b);
        } else if constexpr (sizeof(T) == 2) {
            return _mm256_sub_epi16(a, b);
        } else if constexpr (sizeof(T) == 4) {
            return _mm256_sub_epi32(a, b);
        } else {
            return _mm256_sub_epi64(a, b);
        }

#elif defined(TL_HAVE_SSE2)

        if constexpr (sizeof(T) == 1) {
            return _mm_sub_epi8(a, b);
        } else if constexpr (sizeof(T) == 2) {
            return _mm_sub_epi16(a, b);
        } else if constexpr (sizeof(T) == 4) {
            return _mm_sub_epi32(a, b);
        } else {
            return _mm_sub_epi64(a, b);
        }

#endif
    }
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
