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

#include "tidop/math/statistic/algorithms/robust/MAD.h"
#include "tidop/core/base/Type.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Biweight Midvariance
 *
 * The biweight midvariance is a robust measure of the spread of data that reduces the influence
 * of outliers. It is calculated by weighting the squared deviations from the median and adjusting
 * them using a biweight function.
 *
 * \param[in] range The numeric range.
 * \return The biweight midvariance for the dataset.
 */
template<NumericRange R>
auto biweightMidvariance(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using ResultType = AccumulateType<T>;

    auto n = std::ranges::distance(range);
    if (n <= 2) return consts::zero<ResultType>;

    ResultType _median = tl::median(range);
    ResultType mad = tl::medianAbsoluteDeviation(range);

    ResultType num{};
    ResultType den{};
    for (auto &&value : range) {
        ResultType x = static_cast<ResultType>(value) - _median;
        ResultType u = x / (9 * mad);
        if (std::abs(u) < consts::one<ResultType>) {
            ResultType u2 = u * u;
            ResultType y = consts::one<ResultType> - u2;
            ResultType y2 = y * y;
            num += x * x * y2 * y2;
            den += y * (consts::one<ResultType> - static_cast<ResultType>(5) * u2);
        }
    }

    if (den == consts::zero<ResultType>)
        return consts::zero<ResultType>;

    return n * num / (den * den);
}

template<typename It>
auto biweightMidvariance(It first, It last)
{
    return biweightMidvariance(std::ranges::subrange<It, It>(first, last));
}

/*! \} */

} // End namespace tl
