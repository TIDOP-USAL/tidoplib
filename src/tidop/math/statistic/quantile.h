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
#include "tidop/math/math.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */
 
/*!
 * \brief Quantile
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \param[in] p [0,1]
 * \return
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
quantile(It first, It last, double p)
{
    double q;

    auto n = std::distance(first, last);
    std::vector<typename std::iterator_traits<It>::value_type> sort_vector(n);
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

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
quantile(It first, It last, double p)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

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
 * \brief Quartile coefficient of dispersion
 * \f[ \frac{Q_3-Q_1}{Q_3+Q_1} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
quartileCoefficientOfDispersion(It first, It last)
{
    double q1 = quantile(first, last, 0.25);
    double q3 = quantile(first, last, 0.75);

    return (q3 - q1) / (q3 + q1);
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
quartileCoefficientOfDispersion(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    T q1 = quantile(first, last, 0.25);
    T q3 = quantile(first, last, 0.75);

    return (q3 - q1) / (q3 + q1);
}


/*!
 * \brief Quartile Deviation
 * \f[ \frac{Q_3-Q_1}{2} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
quartileDeviation(It first, It last)
{
    double q1 = quantile(first, last, 0.25);
    double q3 = quantile(first, last, 0.75);

    return (q3 - q1) / consts::two<double>;
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
quartileDeviation(It first, It last)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    T q1 = quantile(first, last, 0.25);
    T q3 = quantile(first, last, 0.75);

    return (q3 - q1) / consts::two<T>;
}



/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl