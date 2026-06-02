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
#include "tidop/math/base/Concepts.h"

/// \cond

namespace tl
{

template<typename T>
class Packed;

namespace detail
{

template<typename T>
struct simd_bias;

template<>
struct simd_bias<uint8_t>
{
    static constexpr uint8_t value = static_cast<uint8_t>(0x80);
};

template<>
struct simd_bias<uint16_t>
{
    static constexpr uint16_t value = static_cast<uint16_t>(0x8000);
};

template<>
struct simd_bias<uint32_t>
{
    static constexpr uint32_t value = static_cast<uint32_t>(0x80000000);
};

template<>
struct simd_bias<uint64_t>
{
    static constexpr uint64_t value = static_cast<uint64_t>(0x8000000000000000ULL);
};


template<UnsignedIntegral T>
[[nodiscard]]
auto signBias() noexcept -> typename Packed<T>
{
    auto bit_mask = simd_bias<T>::value;

#ifdef TL_HAVE_AVX2

    if constexpr (sizeof(T) == 1) {
        return _mm256_set1_epi8(bit_mask);
    } else if constexpr (sizeof(T) == 2) {
        return _mm256_set1_epi16(bit_mask);
    } else if constexpr (sizeof(T) == 4) {
        return _mm256_set1_epi32(bit_mask);
    } else {
        return _mm256_set1_epi64x(bit_mask);
    }

#elif defined(TL_HAVE_SSE2)

    if constexpr (sizeof(T) == 1) {
        return _mm_set1_epi8(bit_mask);
    } else if constexpr (sizeof(T) == 2) {
        return _mm_set1_epi16(bit_mask);
    } else if constexpr (sizeof(T) == 4) {
        return _mm_set1_epi32(bit_mask);
    } else {
        return _mm_set1_epi64x(bit_mask);
    }

#endif
}

} // namespace detail 

} // namespace tl

#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
