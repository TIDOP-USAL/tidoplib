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

namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */

/*! \addtogroup CentralTendency Measures of central tendency
 *  \{
 */

 /*!
  * \brief The mean is the sum of all the data divided by the number of data points.
  * It is represented as:
  * \f[ \overline{x} = \frac{\sum_{i=1}^n x_i}{n} \f]
  * The most commonly used measure of dispersion for the mean is the standard deviation,
  * but it can also be another measure such as the range, coefficient of variation, etc.
  * It is very sensitive to outliers.
  * \param[in] first Iterator to the beginning
  * \param[in] last Iterator to the end
  * \return Value of the mean
  */
template<typename It>
auto mean(It first, It last) -> std::enable_if_t<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>
{
    double x{0};
    double i{1};

    while (first != last) {
        x += (static_cast<double>(*first++) - x) / i++;
    }

    return x;
}

template<typename It>
auto mean(It first, It last) -> std::enable_if_t<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    std::remove_cv_t<typename std::iterator_traits<It>::value_type>>
{
    using T = std::remove_cv_t<typename std::iterator_traits<It>::value_type>;

    T x{0};
    T i{1};

    while (first != last) {
        x += (*first++ - x) / i++;
    }

    return x;
} 
 


/*! \} */ // end of CentralTendency

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
