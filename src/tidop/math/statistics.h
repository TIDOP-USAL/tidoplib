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

#include <vector>
#include <map>

#include "tidop/math/math.h"
#include "tidop/math/statistic/mad.h"
#include "tidop/math/statistic/mode.h"
#include "tidop/math/statistic/covariance.h"
#include "tidop/math/statistic/stddev.h"
#include "tidop/math/statistic/variance.h"
#include "tidop/math/statistic/range.h"
#include "tidop/math/statistic/iqr.h"
#include "tidop/math/statistic/pearson.h"
#include "tidop/math/statistic/biweightmidvariance.h"

namespace tl
{

/*! \addtogrop Statistics
 * \{
 */

/*!
 * \brief Coefficient of variation (CV) or Relative Standard Deviation (RSD)
 *
 * The coefficient of variation describes the dispersion of a dataset relative to its mean.
 * It is the ratio of the standard deviation to the absolute value of the mean:
 *
 * \f[ C_V = \frac{\sigma}{|\bar{x}|} \f]
 *
 * \param[in] first Iterator to the first element in the data range
 * \param[in] last Iterator to the last element in the data range
 * \return Coefficient of variation for the given dataset
 *
 */
template<typename It>
auto coefficientOfVariation(It first, It last) -> enableIfIntegral<iteratorValueType<It>, double>
{
    return standarDeviation(first, last) / std::abs(mean(first, last));
}

/*!
 * \brief Coefficient of variation (CV) or Relative Standard Deviation (RSD)
 *
 * The coefficient of variation describes the dispersion of a dataset relative to its mean.
 * It is the ratio of the standard deviation to the absolute value of the mean:
 *
 * \f[ C_V = \frac{\sigma}{|\bar{x}|} \f]
 *
 * \param[in] first Iterator to the first element in the data range
 * \param[in] last Iterator to the last element in the data range
 * \return Coefficient of variation for the given dataset
 *
 * \ingroup dispersion
 */
template<typename It>
auto coefficientOfVariation(It first, It last) -> enableIfFloating<iteratorValueType<It>, iteratorValueType<It>>
{
    return standarDeviation(first, last) / std::abs(mean(first, last));
}


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
