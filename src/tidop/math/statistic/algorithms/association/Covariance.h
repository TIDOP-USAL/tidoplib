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
#include "tidop/math/base/Concepts.h"
#include "tidop/math/statistic/base/Series.h"
#include "tidop/core/base/Exception.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

template<typename T1, typename T2>
using CovarianceResultType = std::conditional_t<
    std::is_floating_point_v<T1> || std::is_floating_point_v<T2>,
    std::common_type_t<T1, T2>,
    double
>;

/*!
 * \brief Covariance of two datasets
 */
template<NumericRange R1, NumericRange R2>
auto covariance(R1 &&rangeX, R2 &&rangeY)
{
    using T1 = std::remove_cvref_t<std::ranges::range_value_t<R1>>;
    using T2 = std::remove_cvref_t<std::ranges::range_value_t<R2>>;
    using ResultType = CovarianceResultType<T1, T2>;

    auto n_x = std::ranges::distance(rangeX);
    auto n_y = std::ranges::distance(rangeY);
    if (n_x != n_y || n_x <= 1) return consts::zero<ResultType>;

    ResultType mean_x = mean(rangeX);
    ResultType mean_y = mean(rangeY);
    ResultType sum{};

    auto itX = std::ranges::begin(rangeX);
    auto itY = std::ranges::begin(rangeY);
    auto endX = std::ranges::end(rangeX);

    while (itX != endX) {
        sum += (static_cast<ResultType>(*itX++) - mean_x) * (static_cast<ResultType>(*itY++) - mean_y);
    }

    return sum / n_x;
}

template<typename ItX, typename ItY>
auto covariance(ItX firstX, ItX lastX, ItY firstY, ItY lastY)
{
    return covariance(std::ranges::subrange<ItX, ItX>(firstX, lastX),
                      std::ranges::subrange<ItY, ItY>(firstY, lastY));
}


/*! \} */

} // End namespace tl
