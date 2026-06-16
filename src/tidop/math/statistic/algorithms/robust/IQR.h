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

#include "tidop/math/statistic/algorithms/descriptive/Quantile.h"
#include "tidop/core/base/Type.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Interquartile range (IQR)
 * The IQR is the difference between the third quartile (75th percentile) and the
 * first quartile (25th percentile).
 * \f[ IQR = Q_3 - Q_1 \f]
 * \param[in] range The numeric range.
 * \return Value of the interquartile range for the dataset
 */
template<NumericRange R>
auto interquartileRange(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using ResultType = AccumulateType<T>;

    ResultType q1 = quantile(range, 0.25);
    ResultType q3 = quantile(range, 0.75);

    return q3 - q1;
}

template<typename It>
auto interquartileRange(It first, It last)
{
    return interquartileRange(std::ranges::subrange<It, It>(first, last));
}

/*! \} */

} // End namespace tl
