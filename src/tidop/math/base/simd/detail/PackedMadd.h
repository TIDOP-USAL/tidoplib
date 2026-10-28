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
auto madd_impl(const Packed<T> &a, const Packed<T> &b, const Packed<T> &c) -> enableIfFloat<T, Packed<T>>
{
#if defined(__FMA__) || defined(__AVX2__)
    // Use FMA instruction: a * b + c
#if defined(TL_HAVE_AVX512)
    return _mm512_fmadd_ps(a, b, c);
#elif defined(TL_HAVE_AVX)
    return _mm256_fmadd_ps(a, b, c);
#else
    return _mm_fmadd_ps(a, b, c);
#endif
#else
    // Fallback to multiply then add
    return a * b + c;
#endif
}

template<typename T>
[[nodiscard]]
auto madd_impl(const Packed<T> &a, const Packed<T> &b, const Packed<T> &c) -> enableIfDouble<T, Packed<T>>
{
#if defined(__FMA__) || defined(__AVX2__)
    // Use FMA instruction: a * b + c
#if defined(TL_HAVE_AVX512)
    return _mm512_fmadd_pd(a, b, c);
#elif defined(TL_HAVE_AVX)
    return _mm256_fmadd_pd(a, b, c);
#else
    return _mm_fmadd_pd(a, b, c);
#endif
#else
    // Fallback to multiply then add
    return a * b + c;
#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
