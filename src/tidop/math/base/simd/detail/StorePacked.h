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

template<Arithmetic T, typename U>
[[nodiscard]]
void store_aligned_impl(T *data, U &result)
{
    if constexpr (Floating<T>) {
#ifdef TL_HAVE_AVX
        if constexpr (std::same_as<T, float>)
            _mm256_store_ps(data, result);
        else 
            _mm256_store_pd(data, result);
#elif defined TL_HAVE_SSE
        if constexpr (std::same_as<T, float>)
            _mm_store_ps(data, result);
        else
            _mm_store_pd(data, result);
#endif
    } else {
        using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
        _mm256_store_si256(reinterpret_cast<simd_type *>(data), result);
#elif defined TL_HAVE_SSE2
        _mm_store_si128(reinterpret_cast<simd_type *>(data), result);
#endif
    }
}

template<Arithmetic T, typename U>
[[nodiscard]]
void store_unaligned_impl(T *data, U &result)
{
    if constexpr (Floating<T>) {
#ifdef TL_HAVE_AVX
        if constexpr (std::same_as<T, float>)
            _mm256_storeu_ps(data, result);
        else
            _mm256_storeu_pd(data, result);
#elif defined TL_HAVE_SSE
        if constexpr (std::same_as<T, float>)
            _mm_storeu_ps(data, result);
        else
            _mm_storeu_pd(data, result);
#endif
    } else {
        using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
        _mm256_storeu_si256(reinterpret_cast<simd_type *>(data), result);
#elif defined TL_HAVE_SSE2
        _mm_storeu_si128(reinterpret_cast<simd_type *>(data), result);
#endif
    }
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
