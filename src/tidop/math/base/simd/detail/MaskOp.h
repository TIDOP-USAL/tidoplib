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


template<Integral T>
[[nodiscard]]
auto all_impl(const Packed<T> &mask) noexcept -> bool
{
#if defined(TL_HAVE_AVX512F)
    return _mm512_cmpneq_epi8_mask(mask, _mm512_setzero_si512()) == 0xFFFFFFFFFFFFFFFFULL;
#elif defined(TL_HAVE_AVX2)
    return static_cast<uint32_t>(_mm256_movemask_epi8(mask)) == 0xFFFFFFFFU;
#elif defined(TL_HAVE_SSE2)
    return static_cast<uint16_t>(_mm_movemask_epi8(mask)) == 0xFFFFU;
#endif
}

template<Floating T>
[[nodiscard]]
auto all_impl(const Packed<T> &mask) noexcept -> bool
{
#if defined(TL_HAVE_AVX512F)
    if constexpr (std::same_as<T, float>) {
        return _mm512_cmpneq_ps_mask(mask, _mm512_setzero_ps()) == 0xFFFFU;
    } else {
        return _mm512_cmpneq_pd_mask(mask, _mm512_setzero_pd()) == 0xFFU;
    }
#elif defined(TL_HAVE_AVX)
    if constexpr (std::same_as<T, float>) {
        return _mm256_movemask_ps(mask) == 0xFF;
    } else {
        return _mm256_movemask_pd(mask) == 0xF;
    }
#elif defined(TL_HAVE_SSE2)
    if constexpr (std::same_as<T, float>) {
        return _mm_movemask_ps(mask) == 0xF;
    } else {
        return _mm_movemask_pd(mask) == 0x3;
    }
#endif
}


template<Integral T>
[[nodiscard]]
auto none_impl(const Packed<T> &mask) noexcept -> bool
{
#if defined(TL_HAVE_AVX512)
    return _mm512_cmpeq_epi8_mask(mask, _mm512_setzero_si512()) == 0xFFFFFFFFFFFFFFFFULL;
#elif defined(TL_HAVE_AVX2)
    return _mm256_movemask_epi8(mask) == 0;
#elif defined(TL_HAVE_SSE2)
    return _mm_movemask_epi8(mask) == 0;
#endif
}

template<Floating T>
[[nodiscard]]
auto none_impl(const Packed<T> &mask) noexcept -> bool
{
#if defined(TL_HAVE_AVX512)
    if constexpr (std::same_as<T, float>) {
        return _mm512_cmpneq_ps_mask(mask, _mm512_setzero_ps()) == 0U;
    } else {
        return _mm512_cmpneq_pd_mask(mask, _mm512_setzero_pd()) == 0U;
    }
#elif defined(TL_HAVE_AVX)
    if constexpr (std::same_as<T, float>) {
        return _mm256_movemask_ps(mask) == 0;
    } else {
        return _mm256_movemask_pd(mask) == 0;
    }
#elif defined(TL_HAVE_SSE2)
    if constexpr (std::same_as<T, float>) {
        return _mm_movemask_ps(mask) == 0;
    } else {
        return _mm_movemask_pd(mask) == 0;
    }
#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
