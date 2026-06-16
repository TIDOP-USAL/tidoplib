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

#include "tidop/math/base/Simd.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/statistic/base/CentralMoments.h"
#include "tidop/core/base/Type.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

namespace detail
{

template<NumericRange R>
auto momentsScalar(R &&range)
{
    using Value = std::ranges::range_value_t<R>;
    using T = AccumulateType<Value>;

    CentralMoments<T> moments;

    for (auto &&x : range) {
        moments.add(static_cast<T>(x));
    }

    return moments;
}

#ifdef TL_HAVE_SIMD_INTRINSICS

template<SimdRange R>
    requires std::is_floating_point_v<std::ranges::range_value_t<R>>
auto momentsSIMD(R &&range)
{
    using T = std::ranges::range_value_t<R>;

    const T *ptr = std::ranges::data(range);
    const size_t n = std::ranges::size(range);

    if (n == 0) {
        return CentralMoments<T>{};
    }

    constexpr size_t packed_size = PackedTraits<Packed<T>>::size;

    // SIMD REDUCTION PHASE
    Packed<T> sum = Packed<T>::zero();
    Packed<T> sum2 = Packed<T>::zero();

    size_t i = 0;

    for (; i + packed_size <= n; i += packed_size) {
        Packed<T> p;
        p.loadUnaligned(ptr + i);

        sum += p;
        sum2 += p * p;
    }

    T scalar_sum = sum.reduceSum();
    T scalar_sum2 = sum2.reduceSum();

    // process remainder
    T scalar_sum_rem = 0;
    T scalar_sum2_rem = 0;

    for (; i < n; ++i) {
        T v = ptr[i];
        scalar_sum += v;
        scalar_sum2 += v * v;
    }

    const T mean = scalar_sum / static_cast<T>(n);

    // SECOND PASS (central moments)

    i = 0;

    Packed<T> m2_acc = Packed<T>::zero();
    Packed<T> m3_acc = Packed<T>::zero();
    Packed<T> m4_acc = Packed<T>::zero();

    const Packed<T> mean_p(mean);

    for (; i + packed_size <= n; i += packed_size) {
        Packed<T> x;
        x.loadUnaligned(ptr + i);

        auto d = x - mean_p;

        auto d2 = d * d;
        auto d3 = d2 * d;
        auto d4 = d2 * d2;

        m2_acc += d2;
        m3_acc += d3;
        m4_acc += d4;
    }

    T m2 = m2_acc.reduceSum();
    T m3 = m3_acc.reduceSum();
    T m4 = m4_acc.reduceSum();

    for (; i < n; ++i) {
        T d = ptr[i] - mean;
        T d2 = d * d;

        m2 += d2;
        m3 += d2 * d;
        m4 += d2 * d2;
    }

    CentralMoments<T> result(n, mean, m2, m3, m4);

    return result;
}

#endif

} // namespace detail

template<NumericRange R>
auto moments(R &&range)
{

#ifdef TL_HAVE_SIMD_INTRINSICS
    if constexpr (std::ranges::contiguous_range<R> &&
                  std::is_floating_point_v<std::ranges::range_value_t<R>>) {
        return detail::momentsSIMD(std::forward<R>(range));
    } else
#endif
    {
        return detail::momentsScalar(std::forward<R>(range));
    }
}

/*! \} */

} // End namespace tl
