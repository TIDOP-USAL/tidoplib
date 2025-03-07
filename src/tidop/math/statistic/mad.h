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

#include "tidop/math/statistic/mean.h"
#include "tidop/math/statistic/median.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Computes the Mean Absolute Deviation (MAD) for a range of values.
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the range.
 * \param[in] last Iterator pointing to the end of the range.
 * \return The mean absolute deviation of the values in the range.
 * 
 * This function calculates the mean absolute deviation for a range of values
 * pointed to by the iterators `first` and `last`.
 * 
 * ### Example Usage
 * \code{.cpp}
 * std::vector<double> data = {8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};
 * double mad = meanAbsoluteDeviation(data.begin(), data.end());
 * // mad is 0.89506172839506
 * \endcode
 */

template<typename It>
auto meanAbsoluteDeviation(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
{
    size_t n = std::distance(first, last);
    if (n <= 1) return consts::zero<double>;

    double _mean = mean(first, last);
    double sum{};

    while (first != last) {
        sum += std::abs(static_cast<double>(*first++) - _mean);
    }
    return sum / n;
}

template<typename It>
auto meanAbsoluteDeviation(It first, It last) -> std::enable_if_t<
    std::is_floating_point<iteratorValueType<It>>::value,
    std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    size_t n = std::distance(first, last);
    if (n <= 1) return consts::zero<T>;

    T _mean = mean(first, last);
    T sum{};

    while (first != last) {
        sum += std::abs(*first++ - _mean);
    }
    return sum / n;
}


/*!
 * \brief Calculates the Median Absolute Deviation (MAD) of a range of values.
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the range.
 * \param[in] last Iterator pointing to the end of the range.
 * \return The median absolute deviation.
 *
 * This function computes the median absolute deviation, which is a robust measure
 * of statistical dispersion. It is the median of the absolute deviations from the
 * median of the data set.
 *
 * Formula for MAD:
 * \f[ \text{MAD} = \text{median}(|x_i - \text{median}(x)|) \f]
 *
 * ### Example Usage
 * \code{.cpp}
 * std::vector<int> data = {1, 2, 3, 4, 5};
 * double madValue = medianAbsoluteDeviation(data.begin(), data.end());
 * // madValue is 1.0
 * \endcode
 */
template<typename It>
auto medianAbsoluteDeviation(It first, It last) -> enableIfIntegral<iteratorValueType<It>,double>
{
    size_t n = std::distance(first, last);
    if (n <= 1) return consts::zero<double>;

    double _median = median(first, last);

    std::vector<double> x(n);
    auto x_it = x.begin();

    while (first != last) {
        *x_it++ = std::abs(static_cast<double>(*first++) - _median);
    }

    return median(x.begin(), x.end());
}

template<typename It>
auto medianAbsoluteDeviation(It first, It last) -> enableIfFloating<iteratorValueType<It>,
                                                   std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    size_t n = std::distance(first, last);
    if (n <= 1) return consts::zero<T>;

    T _median = median(first, last);
    T sum{};

    std::vector<T> x(n);
    auto x_it = x.begin();


    while (first != last) {
        *x_it++ = std::abs(*first++ - _median);
    }

    return median(x.begin(), x.end());
}


/*! \} */

} // End namespace tl
