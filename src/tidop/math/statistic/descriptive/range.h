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
 * \brief Range
 * 
 * The range is the difference between the maximum and minimum values in the dataset.
 * It is calculated as:
 * 
 * \f[ R = x_{\text{max}} - x_{\text{min}} \f]
 * 
 * \param[in] first Iterator to the beginning of the dataset
 * \param[in] last Iterator to the end of the dataset
 * \return The value of the range for the dataset.
 * 
 * ### Example Usage
 * \code{.cpp}
 * std::vector<int> data = {5, 3, 9, 1, 7, 8};
 * auto result = range(data.begin(), data.end());
 * Message::info("Range of the dataset: {}", result);
 * 
 * // Output: Range of the dataset: 8
 * \endcode
 */
template<typename It>
auto range(It first, It last) -> iteratorValueType<It>
{
    iteratorValueType<It> r{0};

    if (first != last) {
        auto result = std::minmax_element(first, last);
        r = *result.second - *result.first;
    }

    return r;
}


/*! \} */

} // End namespace tl
