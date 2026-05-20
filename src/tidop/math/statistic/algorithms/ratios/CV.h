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

#include "tidop/math/statistic/algorithms/descriptive/Mean.h"
#include "tidop/math/statistic/algorithms/descriptive/StandardDeviation.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

/*!
 * \brief Coefficient of variation (CV) or Relative Standard Deviation (RSD)
 *
 * The coefficient of variation describes the dispersion of a dataset relative to its mean.
 * It is the ratio of the standard deviation to the absolute value of the mean:
 *
 * \f[ C_V = \frac{\sigma}{|\bar{x}|} \f]
 *
 * \param[in] range The numeric range.
 * \return Coefficient of variation for the given dataset
 */
template<NumericRange R>
auto coefficientOfVariation(R &&range)
{
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using ResultType = std::conditional_t<std::is_floating_point_v<T>, T, double>;

    return standardDeviation(std::forward<R>(range)) / std::abs(mean(std::forward<R>(range)));
}

template<typename It>
auto coefficientOfVariation(It first, It last)
{
    return coefficientOfVariation(std::ranges::subrange<It, It>(first, last));
}

/*! \} */

} // End namespace tl
