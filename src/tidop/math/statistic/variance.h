﻿/**************************************************************************
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


namespace tl
{

/*! \addtogrop Statistics
 * \{
 */


/*!
 * \brief Sample Variance
 *
 * The sample variance is a measure of the dispersion or spread in a dataset. It is calculated as:
 * \f[ \sigma^2 = \frac{\sum_{i=1}^n (x_i - \overline{x})^2}{n-1} \f]
 * where \( x_i \) are the data points, \( \overline{x} \) is the mean of the data, and \( n \) is the number of data points.
 *
 * \param[in] first Iterator to the beginning of the dataset.
 * \param[in] last Iterator to the end of the dataset.
 * \return Sample variance of the dataset.
 */
template<typename It>
auto variance(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
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

template<typename It>
auto variance(It first, It last) -> enableIfFloating<iteratorValueType<It>, std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

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
 * \brief Population Variance
 *
 * The population variance is a measure of the dispersion or spread in a dataset for an entire population.
 * It is calculated as:
 * \f[ \sigma^2 = \frac{\sum_{i=1}^n (x_i - \overline{x})^2}{n} \f]
 * where \( x_i \) are the data points, \( \overline{x} \) is the mean of the data, and \( n \) is the number of data points.
 *
 * \param[in] first Iterator to the beginning of the dataset.
 * \param[in] last Iterator to the end of the dataset.
 * \return Population variance of the dataset.
 */
template<typename It>
auto populationVariance(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
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

template<typename It>
auto populationVariance(It first, It last) -> enableIfFloating<iteratorValueType<It>, std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

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

/*! \} */

} // End namespace tl
