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
[[nodiscard]]
auto reduce_max_impl(const Packed<T> &packed) -> enableIfFloat<T, T>
{
#ifdef TL_HAVE_AVX
    __m128 v = _mm_max_ps(
        _mm256_castps256_ps128(packed),
        _mm256_extractf128_ps(packed, 1));

    v = _mm_max_ps(v, _mm_movehl_ps(v, v));
    v = _mm_max_ss(v, _mm_shuffle_ps(v, v, 1));

    return _mm_cvtss_f32(v);
#elif defined(TL_HAVE_SSE)
    __m128 v = _mm_max_ps(packed, _mm_movehl_ps(packed, packed));
    v = _mm_max_ss(v, _mm_shuffle_ps(v, v, 1));

    return _mm_cvtss_f32(v);
#endif
}


template<typename T>
[[nodiscard]]
auto reduce_max_impl(const Packed<T> &packed) -> enableIfDouble<T, T>
{
#ifdef TL_HAVE_AVX
    __m128d v = _mm_max_pd(_mm256_castpd256_pd128(packed),
        _mm256_extractf128_pd(packed, 1));
    v = _mm_max_sd(v, _mm_unpackhi_pd(v, v));
    return _mm_cvtsd_f64(v);
#elif defined TL_HAVE_SSE2
    __m128d v = _mm_max_sd(packed, _mm_unpackhi_pd(packed, packed));
    return _mm_cvtsd_f64(v);
#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
