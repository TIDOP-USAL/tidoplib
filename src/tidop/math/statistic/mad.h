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

#include "tidop/core/defs.h"
#include "tidop/math/statistic/mean.h"
#include "tidop/math/statistic/median.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */

/*!
 * \brief Mean Absolute Deviation
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
meanAbsoluteDeviation(It first, It last)
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

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
meanAbsoluteDeviation(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

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
 * \brief Median Absolute Deviation
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
medianAbsoluteDeviation(It first, It last)
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

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
medianAbsoluteDeviation(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

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


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
