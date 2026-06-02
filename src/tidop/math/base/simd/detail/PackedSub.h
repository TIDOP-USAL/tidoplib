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
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfFloat<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_sub_ps(packed1, packed2);
#elif defined TL_HAVE_SSE
    packed = _mm_sub_ps(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> enableIfDouble<T, Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX
    packed = _mm256_sub_pd(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_pd(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int8_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi8(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi8(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int16_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi16(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi16(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int32_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi32(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi32(packed1, packed2);
#endif

    return packed;
}

template<typename T>
auto sub(const Packed<T> &packed1, const Packed<T> &packed2) -> std::enable_if_t<
    std::is_same<std::remove_cv_t<T>, int64_t>::value,
    Packed<T>>
{
    Packed<T> packed;

#ifdef TL_HAVE_AVX2
    packed = _mm256_sub_epi64(packed1, packed2);
#elif defined TL_HAVE_SSE2
    packed = _mm_sub_epi64(packed1, packed2);
#endif

    return packed;
}


} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
