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

#include "tidop/math/statistic/variance.h"


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
 * \brief Standard Deviation
 *
 * \f[ \sigma = +\sqrt{\frac{\sum_{i=1}^n (x_i - \overline{x})^2}{n}}  \f]
 *
 * \param[in] first Iterator to the beginning
 * \param[in] last Iterator to the end
 * \return Standard deviation of the dataset
 */
template<typename It>
auto standarDeviation(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
{
    return sqrt(variance(first, last));
}

template<typename It>
auto standarDeviation(It first, It last) -> enableIfFloating<iteratorValueType<It>, iteratorValueType<It>>
{
    return sqrt(variance(first, last));
}


/*! \} */ // end of dispersion

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
