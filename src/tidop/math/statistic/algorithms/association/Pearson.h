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

#include "tidop/math/statistic/algorithms/association/Covariance.h"
#include "tidop/math/statistic/algorithms/descriptive/StandardDeviation.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

template<NumericRange R1, NumericRange R2>
auto pearsonCorrelationCoefficient(R1 &&rangeX, R2 &&rangeY)
{
    using T1 = std::remove_cvref_t<std::ranges::range_value_t<R1>>;
    using T2 = std::remove_cvref_t<std::ranges::range_value_t<R2>>;
    using ResultType = CovarianceResultType<T1, T2>;

    auto n_x = std::ranges::distance(rangeX);
    auto n_y = std::ranges::distance(rangeY);
    if (n_x != n_y || n_x <= 1) return consts::zero<ResultType>;

    return covariance(rangeX, rangeY) /
           (standardDeviation(rangeX) * tl::standardDeviation(rangeY));
}

template<typename ItX, typename ItY>
auto pearsonCorrelationCoefficient(ItX firstX, ItX lastX, ItY firstY, ItY lastY)
{
    return pearsonCorrelationCoefficient(std::ranges::subrange<ItX, ItX>(firstX, lastX),
                                         std::ranges::subrange<ItY, ItY>(firstY, lastY));
}

/*! \} */

} // End namespace tl
