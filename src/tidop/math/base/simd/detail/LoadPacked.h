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
auto load_aligned_impl(const T *data) -> typename Packed<T>::simd_type
{
    if constexpr (Floating<T>) {
#ifdef TL_HAVE_AVX
        if constexpr (std::same_as<T, float>)
            return _mm256_load_ps(data);
        else 
            return _mm256_load_pd(data);
#elif defined TL_HAVE_SSE
        if constexpr (std::same_as<T, float>)
            return _mm_load_ps(data);
        else
            return _mm_load_pd(data);
#endif 
    } else {
        using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
        return _mm256_load_si256(reinterpret_cast<simd_type const *>(data));
#elif defined TL_HAVE_SSE2
        return _mm_load_si128(reinterpret_cast<simd_type const *>(data));
#endif
    }
}

template<Arithmetic T>
[[nodiscard]]
auto load_unaligned_impl(const T *data) -> typename Packed<T>::simd_type
{
    if constexpr (Floating<T>){
#ifdef TL_HAVE_AVX
        if constexpr (std::same_as<T, float>)
            return _mm256_loadu_ps(data);
        else 
            return _mm256_loadu_pd(data);
#elif defined TL_HAVE_SSE
        if constexpr (std::same_as<T, float>)
            return _mm_loadu_ps(data);
        else
            return _mm_loadu_pd(data);
#endif
    } else {
        using simd_type = typename Packed<T>::simd_type;

#ifdef TL_HAVE_AVX2
        return _mm256_loadu_si256(reinterpret_cast<simd_type const *>(data));
#elif defined TL_HAVE_SSE2
        return _mm_loadu_si128(reinterpret_cast<simd_type const *>(data));
#endif
    }
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
