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

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Interquartile range (IQR)
 * The IQR is the difference between the third quartile (75th percentile) and the
 * first quartile (25th percentile).
 * \f[ IQR = Q_3 - Q_1 \f]
 * \param[in] first Iterator to the beginning
 * \param[in] last Iterator to the end
 * \return Value of the interquartile range for the dataset
 */
template<typename It>
auto interquartileRange(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
{
    double q1 = tl::quantile(first, last, 0.25);
    double q3 = tl::quantile(first, last, 0.75);

    return q3 - q1;
}

template<typename It>
auto interquartileRange(It first, It last) -> enableIfFloating<iteratorValueType<It>,
                                                               std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    T q1 = tl::quantile(first, last, 0.25);
    T q3 = tl::quantile(first, last, 0.75);

    return q3 - q1;
}


/*! \} */

} // End namespace tl
