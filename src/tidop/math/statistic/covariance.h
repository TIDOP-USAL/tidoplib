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

#include "tidop/core/defs.h"
#include "tidop/math/statistic/descriptive.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */



/*! \addtogroup statistics Statistics
 *  \{
 */


/*!
 * \brief covariance
 * \f[ \S_{xy} = \frac{\sum_{i=1}^n (x_i - \overline{x})(y_i - \overline{y})}{n}  \f]
 * \param first
 * \param last
 * \return
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
covariance(It first_x, It last_x, It first_y, It last_y)
{
    auto n_x = std::distance(first_x, last_x);
    auto n_y = std::distance(first_y, last_y);
    if (n_x != n_y || n_x <= 1) return consts::zero<double>;

    double mean_x = mean(first_x, last_x);
    double mean_y = mean(first_y, last_y);
    double sum{};
    double x{};
    double y{};

    while (first_x != last_x) {
        x = *first_x++ - mean_x;
        y = *first_y++ - mean_y;
        sum += x * y;
    }

    return sum / n_x;
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
covariance(It first_x, It last_x, It first_y, It last_y)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    auto n_x = std::distance(first_x, last_x);
    auto n_y = std::distance(first_y, last_y);
    if (n_x != n_y || n_x <= 1) return consts::zero<T>;

    T mean_x = mean(first_x, last_x);
    T mean_y = mean(first_y, last_y);
    T sum{};
    T x{};
    T y{};

    while (first_x != last_x) {
        x = *first_x++ - mean_x;
        y = *first_y++ - mean_y;
        sum += x * y;
    }

    return sum / n_x;
}


/*!
 * /brief Covariance
 * \f[ \S_{xy} = \frac{\sum_{i=1}^n (x_i - \overline{x})(y_i - \overline{y})}{n}  \f]
 */
template<typename T>
class Covariance
{

public:

    Covariance();
    ~Covariance();

    double eval(const Series<T> &data1,
                const Series<T> &data2);

};


/* Implementation */

template<typename T>
Covariance<T>::Covariance()
{
}

template<typename T>
Covariance<T>::~Covariance()
{
}

template<typename T> inline
double Covariance<T>::eval(const Series<T> &series1,
                           const Series<T> &series2)
{
    DescriptiveStatistics<T> stat1(series1);
    DescriptiveStatistics<T> stat2(series2);

    auto n_x = stat1.size();
    auto n_y = stat2.size();
    if (n_x != n_y || n_x <= 1) return consts::zero<double>;

    double mean_x = stat1.mean();
    double mean_y = stat2.mean();
    double sum{};
    double x{};
    double y{};

    auto it1 = series1.begin();
    auto it2 = series2.begin();
    while (it1 != series1.end()) {
        x = static_cast<double>(*it1++) - mean_x;
        y = static_cast<double>(*it2++) - mean_y;
        sum += x * y;
    }

    return sum / n_x;
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
