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
#include "tidop/math/statistic/algorithms/descriptive/StandardDeviation.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

/*!
 * \brief Z-Score normalization
 *
 * The Z-score normalization is a statistical method that transforms a dataset by subtracting
 * the mean and dividing by the standard deviation for each data point. The Z-score represents
 * the number of standard deviations a value is from the mean.
 *
 * \param[in] range The input numeric range.
 * \param[out] outFirst Output iterator to store the normalized Z-scores.
 */
template<NumericRange R, typename ItOut>
auto zScore(R &&range, ItOut outFirst) -> ItOut
{
    double _mean = mean(range);
    double standard_deviation = tl::standardDeviation(range);

    for (auto &&value : range) {
        *outFirst++ = (static_cast<double>(value) - _mean) / standard_deviation;
    }

    return outFirst;
}

template<typename ItIn, typename ItOut>
void zScore(ItIn inFirst, ItIn inLast, ItOut outFirst)
{
    zScore(std::ranges::subrange<ItIn, ItIn>(inFirst, inLast), outFirst);
}

/*! \} */

} // End namespace tl
