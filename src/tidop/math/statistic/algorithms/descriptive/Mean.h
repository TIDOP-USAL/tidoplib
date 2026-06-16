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

#include <ranges>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/meta.h"
#include "tidop/core/base/exception.h"
#include "tidop/math/base/Simd.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/core/base/Type.h"

namespace tl
{

namespace detail
{

template<NumericRange R>
auto meanScalar(R &&range)
{
    using T = std::ranges::range_value_t<R>;
    using Accumulator = AccumulateType<T>;

    TL_ASSERT(!std::ranges::empty(range), "mean: empty range");

    Accumulator mean = 0;
    size_t n = 0;

    for (auto &&value : range) {
        mean += (static_cast<Accumulator>(value) - mean) / ++n;
    }

    return mean;
}

#ifdef TL_HAVE_SIMD_INTRINSICS
template<ContiguousNumericRange R>
auto meanSimd(R &&range)
{
    using T = std::ranges::range_value_t<R>;
    using Accumulator = std::conditional_t<std::integral<T>, double, T>;

    TL_ASSERT(!std::ranges::empty(range), "mean: empty range");

    constexpr size_t packed_size = Packed<T>::size();
    const T *ptr = std::ranges::data(range);
    size_t n = std::ranges::size(range);
    size_t i = 0;
    Packed<T> packed_sum(T{0});

    for (; i + packed_size <= n; i += packed_size) {
        Packed<T> pack;
        pack.loadUnaligned(ptr + i);
        packed_sum += pack;
    }

    Accumulator sum = static_cast<Accumulator>(packed_sum.sum());

    for (; i < n; ++i) {
        sum += static_cast<Accumulator>(ptr[i]);
    }

    return sum / static_cast<Accumulator>(n);
}
#endif

}

/*! \addtogroup Statistics
 *  \{
 */

template<NumericRange R>
auto mean(R &&range)
{
#ifdef TL_HAVE_SIMD_INTRINSICS
    if constexpr (ContiguousNumericRange<R>) {
        return detail::meanSimd(std::forward<R>(range));
    } else
#endif
    {
        return detail::meanScalar(std::forward<R>(range));
    }
}

template<typename It>
auto mean(It first, It last)
{
    return mean(std::ranges::subrange(first, last));
}

/*! \} */

} // End namespace tl
