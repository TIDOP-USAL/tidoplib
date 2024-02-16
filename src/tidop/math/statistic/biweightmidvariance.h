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
#include "tidop/math/statistic/mad.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */


/*!
 * \brief Biweight Midvariance
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
biweightMidvariance(It first, It last)
{
    size_t n = std::distance(first, last);
    if (n <= 2) return consts::zero<double>;

    double _median = median(first, last);
    double mad = medianAbsoluteDeviation(first, last);

    double num{};
    double den{};
    while (first != last) {
        double x = *first++ - _median;
        double u = x / (9 * mad);
        if (std::abs(u) < consts::one<double>) {
            double u2 = u * u;
            double y = consts::one<double> -u2;
            double y2 = y * y;
            num += x * x * y2 * y2;
            den += y * (consts::one<double> -static_cast<double>(5) * u2);
        }
    }

    if (den == consts::zero<double>)
        return consts::zero<double>;

    return n * num / (den * den);
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
biweightMidvariance(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    size_t n = std::distance(first, last);
    if (n <= 2) return consts::zero<T>;

    T _median = median(first, last);
    T mad = medianAbsoluteDeviation(first, last);

    T num{};
    T den{};
    while (first != last) {
        T x = *first++ - _median;
        T u = x / (9 * mad);
        if (std::abs(u) < consts::one<T>) {
            T u2 = u * u;
            T y = consts::one<T> -u2;
            T y2 = y * y;
            num += x * x * y2 * y2;
            den += y * (consts::one<T> -static_cast<T>(5) * u2);
        }
    }

    if (den == consts::zero<T>)
        return consts::zero<T>;

    return n * num / (den * den);
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
