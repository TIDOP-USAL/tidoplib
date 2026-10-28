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
auto sqrt_impl(const Packed<T> &packed) -> enableIfFloat<T, Packed<T>>
{
    Packed<T> result;

#if defined(TL_HAVE_AVX512)
    result = _mm512_sqrt_ps(packed);
#elif defined(TL_HAVE_AVX)
    result = _mm256_sqrt_ps(packed);
#elif defined(TL_HAVE_SSE2)
    result = _mm_sqrt_ps(packed);
#endif

    return result;
}

template<typename T>
[[nodiscard]]
auto sqrt_impl(const Packed<T> &packed) -> enableIfDouble<T, Packed<T>>
{
    Packed<T> result;

#if defined(TL_HAVE_AVX512)
    result = _mm512_sqrt_pd(packed);
#elif defined(TL_HAVE_AVX)
    result = _mm256_sqrt_pd(packed);
#elif defined(TL_HAVE_SSE2)
    result = _mm_sqrt_pd(packed);
#endif

    return result;
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
