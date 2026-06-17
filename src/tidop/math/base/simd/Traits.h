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

/// \cond
#ifdef TL_HAVE_SIMD_INTRINSICS
/// \endcond

#include "tidop/math/base/simd/Intrinsics.h"


namespace tl
{

template<typename T>
class Packed;

/*! \addtogroup MathBase
 *  \{
 */


template<typename P>
struct PackedTraits;

// size == lanes
template<>
struct PackedTraits<Packed<float>>
{
    using value_type = float;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512;
    static constexpr size_t size = 16;
#elif defined TL_HAVE_AVX
    using simd_type = __m256;
    static constexpr size_t size = 8;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128;
    static constexpr size_t size = 4;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<double>>
{
    using value_type = double;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512;
    static constexpr size_t size = 8;
#elif defined TL_HAVE_AVX
    using simd_type = __m256d;
    static constexpr size_t size = 4;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128d;
    static constexpr size_t size = 2;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<int8_t>>
{
    using value_type = int8_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 64;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 32;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 16;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<uint8_t>>
{
    using value_type = uint8_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 64;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 32;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 16;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<int16_t>>
{
    using value_type = int16_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 32;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 16;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 8;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<uint16_t>>
{
    using value_type = uint16_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 32;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 16;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 8;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<int32_t>>
{
    using value_type = int32_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 16;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 8;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 4;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<uint32_t>>
{
    using value_type = uint32_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 16;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 8;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 4;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<int64_t>>
{
    using value_type = int64_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 8;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 4;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 2;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};

template<>
struct PackedTraits<Packed<uint64_t>>
{
    using value_type = uint64_t;
#ifdef TL_HAVE_AVX512
    using simd_type = __m512i;
    static constexpr size_t size = 8;
#elif defined TL_HAVE_AVX2
    using simd_type = __m256i;
    static constexpr size_t size = 4;
#elif defined TL_HAVE_SSE2
    using simd_type = __m128i;
    static constexpr size_t size = 2;
#endif
    static constexpr size_t alignment = sizeof(simd_type);
};


template<typename T>
struct is_packed : std::false_type {};

template<typename T>
struct is_packed<Packed<T>> : std::true_type {};


/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
