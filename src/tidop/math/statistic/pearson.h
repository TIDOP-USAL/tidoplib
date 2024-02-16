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
#include "tidop/math/statistic/covariance.h"
#include "tidop/math/statistic/stddev.h"


namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */

 /*!
  * \brief pearsonCorrelationCoefficient
  * \param[in] first_x
  * \param[in] last_x
  * \param[in] first_y
  * \param[in] last_y
  * \return
  */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
pearsonCorrelationCoefficient(It first_x, It last_x, It first_y, It last_y)
{
    auto n_x = std::distance(first_x, last_x);
    auto n_y = std::distance(first_y, last_y);
    if (n_x != n_y || n_x <= 1) return consts::zero<double>;
    return covariance(first_x, last_x, first_y, last_y) /
        (standarDeviation(first_x, last_x) * standarDeviation(first_y, last_y));
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type>::type
pearsonCorrelationCoefficient(It first_x, It last_x, It first_y, It last_y)
{
    using T = typename std::remove_cv<typename std::iterator_traits<It>::value_type>::type;

    auto n_x = std::distance(first_x, last_x);
    auto n_y = std::distance(first_y, last_y);
    if (n_x != n_y || n_x <= 1) return consts::zero<T>;
    return covariance(first_x, last_x, first_y, last_y) /
        (standarDeviation(first_x, last_x) * standarDeviation(first_y, last_y));
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
