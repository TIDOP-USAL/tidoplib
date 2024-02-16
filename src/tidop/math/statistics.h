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

/*! \addtogroup math
 *  \{
 */


/*! \defgroup statistics Statistics
 *  \{
 */

/*! \defgroup CentralTendency Measure of central tendency
 *  \{
 * 
 * - Mean
 * - Median
 * - Mode
 */

/*! \} */ // end of Measure of Central Tendency


/*! \defgroup dispersion Statistical dispersion
 *  \{
 * Measures of dispersion
 * - Range
 * - Variance
 * - Standard deviation
 * - Coefficient of variation
 * - Inter-quantile range
 */


/*!
 * \brief Coefficient of variation (CV) or Relative Standard Deviation (RSD)
 * \f[ C_V = \frac{\sigma}{|\bar{x}|} \f]
 * \param[in] first Iterador al inicio
 * \param[in] last Iterador al final
 * \return Coeficiente de variación para el conjunto de datos
 */
template<typename It> inline
typename std::enable_if<
    std::is_integral<typename std::iterator_traits<It>::value_type>::value,
    double>::type
coefficientOfVariation(It first, It last)
{
    return standarDeviation(first, last) / std::abs(mean(first, last));
}

template<typename It> inline
typename std::enable_if<
    std::is_floating_point<typename std::iterator_traits<It>::value_type>::value,
    typename std::iterator_traits<It>::value_type>::type
coefficientOfVariation(It first, It last)
{
    return standarDeviation(first, last) / std::abs(mean(first, last));
}


/*! \} */ // end of Dispersion


template<typename itIn, typename itOut> inline
double rootMeanSquareError(itIn in_first, itIn in_last, itOut out_first)
{
    double rms = 0.;
    while (in_first != in_last) {
        //  rms
        //  trf->transform(*in_first++, &(*out_first++), trfOrder);
    }
    return rms;
}


template<typename itIn, typename itOut> inline
void zScore(itIn in_first, itIn in_last, itOut out_first)
{
    double _mean = mean(in_first, in_last);
    double standar_deviation = standarDeviation(in_first, in_last);

    while (in_first != in_last) {
        *out_first++ = (*in_first++ - _mean) / standar_deviation;
    }
}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
