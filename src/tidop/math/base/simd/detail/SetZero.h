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
auto setZero() -> enableIfFloat<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_setzero_ps();
#elif defined TL_HAVE_SSE
    return _mm_setzero_ps();
#endif
}

template<typename T>
auto setZero() -> enableIfDouble<T, typename Packed<T>::simd_type>
{
#ifdef TL_HAVE_AVX
    return _mm256_setzero_pd();
#elif defined TL_HAVE_SSE2
    return _mm_setzero_pd();
#else
    //...
#endif
}


template<typename T>
auto setZero() -> enableIfIntegral<T, void>
{

#ifdef TL_HAVE_AVX
    return _mm256_setzero_si256();
#elif defined TL_HAVE_SSE2
    return _mm_setzero_si128();
#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
