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
  * \param[in] firstX
  * \param[in] lastX
  * \param[in] firstY
  * \param[in] lastY
  * \return
  */
template<typename It>
auto pearsonCorrelationCoefficient(It firstX, It lastX, It firstY, It lastY) -> enableIfIntegral<iteratorValueType<It>,double>
{
    auto n_x = std::distance(firstX, lastX);
    auto n_y = std::distance(firstY, lastY);
    if (n_x != n_y || n_x <= 1) return consts::zero<double>;
    return covariance(firstX, lastX, firstY, lastY) /
        (standarDeviation(firstX, lastX) * standarDeviation(firstY, lastY));
}

template<typename It>
auto pearsonCorrelationCoefficient(It firstX, It lastX, It firstY, It lastY) -> enableIfFloating<iteratorValueType<It>,
                                                                                std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    auto n_x = std::distance(firstX, lastX);
    auto n_y = std::distance(firstY, lastY);
    if (n_x != n_y || n_x <= 1) return consts::zero<T>;
    return covariance(firstX, lastX, firstY, lastY) /
        (standarDeviation(firstX, lastX) * standarDeviation(firstY, lastY));
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
