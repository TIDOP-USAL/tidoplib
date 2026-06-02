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
auto set(T data) -> enableIfFloat<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_set1_ps(data);
#elif defined TL_HAVE_SSE
    return _mm_set1_ps(data);
#endif
}

template<typename T>
auto set(T data) -> enableIfDouble<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_set1_pd(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_pd(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint8_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi8(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi8(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint16_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi16(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi16(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint32_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi32(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi32(data);
#endif
}

template<typename T>
auto set(T data) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value ||
    std::is_same<std::remove_cv_t<T>, uint64_t>::value,
    typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX2
    return _mm256_set1_epi64x(data);
#elif defined TL_HAVE_SSE2
    return _mm_set1_epi64x(data);
#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
