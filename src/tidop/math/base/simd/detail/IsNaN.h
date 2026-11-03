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
#include "tidop/core/base/Concepts.h"

#ifdef TL_HAVE_SIMD_INTRINSICS

#include "tidop/math/base/simd/Intrinsics.h"

/// \cond

namespace tl
{

template<typename T>
class Packed;

namespace detail
{

template<Floating T>
auto is_nan_mask_impl(const Packed<T> &packed) noexcept -> Packed<T>
{
//#if defined(TL_HAVE_AVX512F)
//    if constexpr (std::same_as<T, float>) {
//        return _mm512_castsi512_ps(
//            _mm512_maskz_set1_epi32(
//                _mm512_cmp_ps_mask(packed, packed, _CMP_UNORD_Q),
//                -1));
//    } else {
//        return _mm512_castsi512_pd(
//            _mm512_maskz_set1_epi64(
//                _mm512_cmp_pd_mask(packed, packed, _CMP_UNORD_Q),
//                -1));
//    }
//#el
#if defined(TL_HAVE_AVX)
    if constexpr (std::same_as<T, float>) {
        return _mm256_cmp_ps(packed, packed, _CMP_UNORD_Q);
    } else {
        return _mm256_cmp_pd(packed, packed, _CMP_UNORD_Q);
    }
#elif defined(TL_HAVE_SSE2)
    if constexpr (std::same_as<T, float>) {
        return _mm_cmpunord_ps(packed, packed);
    } else {
        return _mm_cmpunord_pd(packed, packed);
    }
#endif
}

template<Integral T>
[[nodiscard]]
auto is_nan_mask_impl(const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>::zero();
}

} // namespace detail

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
