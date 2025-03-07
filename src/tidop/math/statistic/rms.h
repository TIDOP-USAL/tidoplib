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

#include "tidop/core/base/defs.h"


namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Computes the Root Mean Square (RMS) of a range of values.
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the data range.
 * \param[in] last Iterator pointing to the end of the data range.
 * \return The Root Mean Square (RMS) of the values in the range.
 * 
 * The Root Mean Square (RMS) is a measure of the average magnitude of the values in a dataset. It is calculated as:
 * \f[ \text{RMS} = \sqrt{\frac{1}{n} \sum_{i=1}^n x_i^2} \f]
 * where \( x_i \) represents each individual value and \( n \) is the number of elements.
 * 
 * ### Example Usage
 * \code{.cpp}
 * std::vector<double> data = {8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};
 * double rms = rootMeanSquare(data.begin(), data.end()); // rms is approximately 7.77147562
 * \endcode
 */
template<typename It>
auto rootMeanSquare(It first, It last) -> double
{
    double sum{};
    double i{1};
    double x{};

    while (first != last) {
        x = *first++;
        sum += (x * x - sum) / i++;
    }

    return std::sqrt(sum);
}

/*! \} */

} // End namespace tl
