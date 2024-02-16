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


namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */

/*! \addtogroup dispersion Statistical dispersion
 *  \{
 */


/*!
 * \brief Sample Variance
 *
 * \f[ \sigma² = \frac{\sum_{i=1}^n (x_i - \overline{x})²}{n}  \f]
 *
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Varianza del conjunto de datos
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
variance(It first, It last)
{
    auto n = std::distance(first, last);
    if (n <= 1) return consts::one<double>;

    double _mean = mean(first, last);
    double sum{};
    double ep{};
    double aux{};

    while (first != last) {
        aux = *first++ - _mean;
        ep += aux;
        sum += aux * aux;
    }

    return (sum - ep * ep / n) / (n - 1);
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
variance(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    auto n = std::distance(first, last);
    if (n <= 1) return consts::one<T>;

    T _mean = mean(first, last);
    T sum{};
    T ep{};
    T aux{};

    while (first != last) {
        aux = *first++ - _mean;
        ep += aux;
        sum += aux * aux;
    }

    return (sum - ep * ep / n) / (n - 1);
}


/*!
 * \brief Varianza
 *
 * \f[ \sigma² = \frac{\sum_{i=1}^n (x_i - \overline{x})²}{n}  \f]
 *
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Varianza del conjunto de datos
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
populationVariance(It first, It last)
{
    auto n = std::distance(first, last);
    if (n <= 1) return consts::one<double>;

    double _mean = mean(first, last);
    double sum{};
    double ep{};
    double aux{};

    while (first != last) {
        aux = *first++ - _mean;
        ep += aux;
        sum += aux * aux;
    }

    return (sum - ep * ep / n) / n;
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
populationVariance(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    auto n = std::distance(first, last);
    if (n <= 1) return consts::one<T>;

    T _mean = mean(first, last);
    T sum{};
    T ep{};
    T aux{};

    while (first != last) {
        aux = *first++ - _mean;
        ep += aux;
        sum += aux * aux;
    }

    return (sum - ep * ep / n) / n;
}


/*! \} */ // end of dispersion

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
