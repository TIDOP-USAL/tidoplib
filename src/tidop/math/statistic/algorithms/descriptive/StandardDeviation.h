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

#include <cmath>
#include "tidop/math/statistic/algorithms/descriptive/Variance.h"


namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

/*!
 * \brief Standard Deviation
 *
 * The standard deviation measures the amount of variation or dispersion in a set of values.
 * It is defined as the square root of the variance, providing insight into the average distance of the data points from the mean.
 *
 * The formula for the standard deviation (\f$\sigma\f$) is:
 * \f[
 * \sigma = +\sqrt{\frac{\sum_{i=1}^n (x_i - \overline{x})^2}{n}}
 * \f]
 *
 * \param[in] range The numeric range.
 * \return The standard deviation of the dataset.
 */
template<NumericRange R>
auto standardDeviation(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using ResultType = std::conditional_t<std::is_floating_point_v<T>, T, double>;
    return std::sqrt(tl::variance(std::forward<R>(range)));
}

template<typename It>
auto standardDeviation(It first, It last)
{
    return standardDeviation(std::ranges::subrange<It, It>(first, last));
}

/*! \} */

} // End namespace tl
