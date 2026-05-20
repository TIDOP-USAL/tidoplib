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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <ranges>
#include "tidop/math/base/Concepts.h"
#include "tidop/core/base/exception.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

template<NumericRange R>
auto quantile(R &&range, double p)
{
    using T = std::ranges::range_value_t<R>;
    using ResultType = std::conditional_t<std::integral<T>, double, T>;

    TL_ASSERT(!std::ranges::empty(range), "quantile: empty range");
    TL_ASSERT(std::isfinite(p), "quantile: p must be finite");
    TL_ASSERT(p >= 0. && p <= 1., "quantile: p must be in the range [0, 1]");

    std::vector<T> sort_vector(std::ranges::begin(range), std::ranges::end(range));
    std::sort(sort_vector.begin(), sort_vector.end());
    size_t n = sort_vector.size();

    // Boundary cases
    if (p == 0.0) return static_cast<ResultType>(sort_vector.front());
    if (p == 1.0) return static_cast<ResultType>(sort_vector.back());

    // Hyndman & Fan type 7
    double h = (static_cast<double>(n) - 1.0) * p;
    size_t i = static_cast<size_t>(std::floor(h));
    double f = h - static_cast<double>(i);

    return (i + 1 < n)
        ? static_cast<ResultType>(sort_vector[i] + f * (static_cast<double>(sort_vector[i + 1]) - static_cast<double>(sort_vector[i])))
        : static_cast<ResultType>(sort_vector[i]);
}

template<NumericRange R>
auto quartileCoefficientOfDispersion(R &&range)
{
    auto q1 = tl::quantile(range, 0.25);
    auto q3 = tl::quantile(range, 0.75);

    return (q3 - q1) / (q3 + q1);
}

template<NumericRange R>
auto quartileDeviation(R &&range)
{
    using T = std::ranges::range_value_t<R>;
    using ResultType = std::conditional_t<std::integral<T>, double, T>;

    auto q1 = tl::quantile(range, 0.25);
    auto q3 = tl::quantile(range, 0.75);

    return (q3 - q1) / consts::two<ResultType>;
}

template<typename It>
auto quantile(It first, It last, double p)
{
    return tl::quantile(std::ranges::subrange(first, last), p);
}

template<typename It>
auto quartileCoefficientOfDispersion(It first, It last)
{
    return tl::quartileCoefficientOfDispersion(std::ranges::subrange(first, last));
}

template<typename It>
auto quartileDeviation(It first, It last)
{
    return tl::quartileDeviation(std::ranges::subrange(first, last));
}

/*! \} */

} // End namespace tl
