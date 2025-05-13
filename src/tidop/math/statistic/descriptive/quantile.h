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

#include "tidop/math/math.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */
 
/*!
 * \brief Computes the quantile of a sorted range of data.
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the data range.
 * \param[in] last Iterator pointing to the end of the data range.
 * \param[in] p A double value in the range [0,1] representing the desired quantile (e.g., 0.5 for the median).
 * \return The value of the quantile.
 * 
 * The quantile is a statistical measure that indicates the value below which a given percentage of observations fall.
 * For example, the median is the 0.5 quantile. This function sorts the data and interpolates between values if necessary
 * to compute the quantile for non-integer positions.
 * 
 * This function supports both integral and floating-point data types.
 * 
 * ### Example Usage
 * \code{.cpp}
 * std::vector<int> data = {1, 2, 3, 4, 5};
 * double q = quantile(data.begin(), data.end(), 0.25); // q is 1.5
 * \endcode
 */
template<typename It>
auto quantile(It first, It last, double p) -> enableIfIntegral<iteratorValueType<It>, double>
{
    double q;

    auto n = std::distance(first, last);
    std::vector<iteratorValueType<It>> sort_vector(n);
    std::copy(first, last, sort_vector.begin());
    std::sort(sort_vector.begin(), sort_vector.end());

    double idx = static_cast<double>(n + 1) * p - 1.;
    size_t idx_1 = static_cast<size_t>(std::floor(idx));
    size_t idx_2 = static_cast<size_t>(std::ceil(idx));

    if (idx_1 == idx_2) {
        q = static_cast<double>(sort_vector[idx_1]);
    } else {
        q = static_cast<double>(sort_vector[idx_1]) + static_cast<double>(sort_vector[idx_2] - sort_vector[idx_1]) * fabs(idx - static_cast<int>(idx));
    }

    return q;
}

template<typename It>
auto quantile(It first, It last, double p) -> enableIfFloating<iteratorValueType<It>, std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    T q;

    auto n = std::distance(first, last);
    std::vector<T> sort_vector(n);
    std::copy(first, last, sort_vector.begin());
    std::sort(sort_vector.begin(), sort_vector.end());

    double idx = static_cast<double>(n + 1) * p - 1.;
    size_t idx_1 = static_cast<size_t>(std::floor(idx));
    size_t idx_2 = static_cast<size_t>(std::ceil(idx));

    if (idx_1 == idx_2) {
        q = sort_vector[idx_1];
    } else {
        q = sort_vector[idx_1] + (sort_vector[idx_2] - sort_vector[idx_1]) * static_cast<T>(fabs(idx - static_cast<int>(idx)));
    }

    return q;
}


/*!
 * \brief Computes the quartile coefficient of dispersion.
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the data range.
 * \param[in] last Iterator pointing to the end of the data range.
 * \return The quartile coefficient of dispersion.
 *
 * The quartile coefficient of dispersion is a measure of relative dispersion based on the interquartile range.
 * It is calculated as:
 * \f[ \frac{Q_3-Q_1}{Q_3+Q_1} \f]
 * where \( Q_1 \) is the first quartile and \( Q_3 \) is the third quartile.
 *
 * ### Example Usage
 * \code{.cpp}
 * std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
 * double qcd = quartileCoefficientOfDispersion(data.begin(), data.end()); // qcd is 0.2
 * \endcode
 */
template<typename It>
auto quartileCoefficientOfDispersion(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
{
    double q1 = quantile(first, last, 0.25);
    double q3 = quantile(first, last, 0.75);

    return (q3 - q1) / (q3 + q1);
}

template<typename It>
auto quartileCoefficientOfDispersion(It first, It last) -> enableIfFloating<iteratorValueType<It>, std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    T q1 = quantile(first, last, 0.25);
    T q3 = quantile(first, last, 0.75);

    return (q3 - q1) / (q3 + q1);
}


/*!
 * \brief Computes the quartile deviation.
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the data range.
 * \param[in] last Iterator pointing to the end of the data range.
 * \return The quartile deviation.
 *
 * The quartile deviation, also known as the semi-interquartile range, is calculated as:
 * \f[ \frac{Q_3-Q_1}{2} \f]
 * where \( Q_1 \) is the first quartile and \( Q_3 \) is the third quartile.
 *
 * ### Example Usage
 * \code{.cpp}
 * std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
 * double qd = quartileDeviation(data.begin(), data.end()); // qd is 2.0
 * \endcode
 */
template<typename It>
auto quartileDeviation(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
{
    double q1 = quantile(first, last, 0.25);
    double q3 = quantile(first, last, 0.75);

    return (q3 - q1) / consts::two<double>;
}

template<typename It>
auto quartileDeviation(It first, It last) -> enableIfFloating<iteratorValueType<It>, std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    T q1 = quantile(first, last, 0.25);
    T q3 = quantile(first, last, 0.75);

    return (q3 - q1) / consts::two<T>;
}



/*! \} */

} // End namespace tl