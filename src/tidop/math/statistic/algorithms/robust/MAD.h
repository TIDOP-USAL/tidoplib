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

#include "tidop/math/statistic/algorithms/descriptive/Mean.h"
#include "tidop/math/statistic/algorithms/descriptive/Median.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Computes the Mean Absolute Deviation (MAD) for a range of values.
 * \param[in] range The input numeric range.
 * \return The mean absolute deviation of the values in the range.
 */
template<NumericRange R>
auto meanAbsoluteDeviation(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using ResultType = std::conditional_t<std::is_floating_point_v<T>, T, double>;

    auto n = std::ranges::distance(range);
    if (n <= 1) return consts::zero<ResultType>;

    ResultType _mean = tl::mean(range);
    ResultType sum{};

    for (auto &&value : range) {
        sum += std::abs(static_cast<ResultType>(value) - _mean);
    }
    return sum / n;
}

template<typename It>
auto meanAbsoluteDeviation(It first, It last)
{
    return meanAbsoluteDeviation(std::ranges::subrange<It, It>(first, last));
}

/*!
 * \brief Calculates the Median Absolute Deviation (MAD) of a range of values.
 * \param[in] range The input numeric range.
 * \return The median absolute deviation.
 */
template<NumericRange R>
auto medianAbsoluteDeviation(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using ResultType = std::conditional_t<std::is_floating_point_v<T>, T, double>;

    auto n = std::ranges::distance(range);
    if (n <= 1) return consts::zero<ResultType>;

    ResultType _median = tl::median(range);

    std::vector<ResultType> x;
    x.reserve(n);

    for (auto &&value : range) {
        x.push_back(std::abs(static_cast<ResultType>(value) - _median));
    }

    return tl::median(x);
}

template<typename It>
auto medianAbsoluteDeviation(It first, It last)
{
    return medianAbsoluteDeviation(std::ranges::subrange<It, It>(first, last));
}

/*! \} */

} // End namespace tl
