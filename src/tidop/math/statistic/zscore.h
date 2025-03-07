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

#include "tidop/math/statistic/mean.h"


namespace tl
{

/*! \addtogrop Statistics
 * \{
 */


/*!
 * \brief Z-Score normalization
 *
 * The Z-score normalization is a statistical method that transforms a dataset by subtracting
 * the mean and dividing by the standard deviation for each data point. The Z-score represents
 * the number of standard deviations a value is from the mean.
 *
 * \param[in] inFirst Iterator to the first element in the data range
 * \param[in] inLast Iterator to the last element in the data range
 * \param[out] outFirst Iterator to store the normalized Z-scores
 */
template<typename itIn, typename itOut>
void zScore(itIn inFirst, itIn inLast, itOut outFirst)
{
    double _mean = mean(inFirst, inLast);
    double standar_deviation = standarDeviation(inFirst, inLast);

    while (inFirst != inLast) {
        *outFirst++ = (*inFirst++ - _mean) / standar_deviation;
    }
}

/*! \} */

} // End namespace tl
