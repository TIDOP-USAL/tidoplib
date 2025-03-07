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

#include "tidop/math/statistic/quantile.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Calculates the median of a range of values.
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the range.
 * \param[in] last Iterator pointing to the end of the range.
 * \return The median value.
 * 
 * The median is the value that separates the higher half from the lower half of a data sample.
 * It is the value that ensures that 50% of the data points are greater than or equal to it,
 * and the other 50% are smaller. The median is often used with a measure of dispersion,
 * such as the interquartile range, to give a better understanding of the data distribution.
 * 
 * ### Example Usage
 * \code{.cpp}
 * std::vector<int> data = {1, 2, 3, 4, 5};
 * double medianValue = median(data.begin(), data.end());
 * // medianValue is 3.0
 * \endcode
 */

template<typename It>
auto median(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
{
    return tl::quantile(first, last, 0.5);
}

template<typename It>
auto median(It first, It last) -> enableIfFloating<iteratorValueType<It>, std::remove_cv_t<iteratorValueType<It>>>
{
    return tl::quantile(first, last, 0.5);
}


/*! \} */

} // End namespace tl
