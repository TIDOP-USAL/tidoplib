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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses> *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <ranges>

#include "tidop/math/statistic/algorithms/descriptive/Mean.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/core/base/Type.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

template<NumericRange R>
auto variance(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using Accumulator = AccumulateType<T>;

    Accumulator mean = 0;
    Accumulator M2 = 0;
    size_t n = 0;

    for (auto &&value : range) {
        ++n;
        Accumulator delta = static_cast<Accumulator>(value) - mean;
        mean += delta / n;
        Accumulator delta2 = static_cast<Accumulator>(value) - mean;
        M2 += delta * delta2;
    }

    if (n <= 1) return static_cast<Accumulator>(1.0);

    return M2 / (n - 1);
}

template<NumericRange R>
auto populationVariance(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using Accumulator = AccumulateType<T>;

    Accumulator mean = 0;
    Accumulator M2 = 0;
    size_t n = 0;

    for (auto &&value : range) {
        ++n;
        Accumulator delta = static_cast<Accumulator>(value) - mean;
        mean += delta / n;
        Accumulator delta2 = static_cast<Accumulator>(value) - mean;
        M2 += delta * delta2;
    }

    if (n <= 1) return static_cast<Accumulator>(1.0);

    return M2 / n;
}

template<typename It>
auto variance(It first, It last)
{
    return variance(std::ranges::subrange(first, last));
}

template<typename It>
auto populationVariance(It first, It last)
{
    return populationVariance(std::ranges::subrange(first, last));
}

/*! \} */

} // End namespace tl
