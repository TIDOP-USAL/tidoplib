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

#include "tidop/math/statistic/quantile.h"

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
 * \brief The median is the value of the variable that occupies the central position in the distribution.
 * It is the value that verifies that 50% of the variables are greater than or equal to it,
 * and the other 50% are smaller.
 * The median should be accompanied by a measure of dispersion (usually the interquartile range).
 * \param[in] first Iterator to the beginning
 * \param[in] last Iterator to the end
 * \return Value of the median
 */
template<typename It>
auto median(It first, It last) -> std::enable_if_t<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>
{
    return tl::quantile(first, last, 0.5);
}

template<typename It>
auto median(It first, It last) -> std::enable_if_t<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    std::remove_cv_t<typename std::iterator_traits<It>::value_type>>
{
    return tl::quantile(first, last, 0.5);
}


/*! \} */ // end of CentralTendency

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
