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

#include <tidop/core/base/defs.h>

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

 /*!
  * \brief Calculates the mode of a range of values.
  * \tparam It Type of the iterator.
  * \param[in] first Iterator pointing to the beginning of the range.
  * \param[in] last Iterator pointing to the end of the range.
  * \return The mode value, which is the most frequently occurring element in the range.
  *
  * The mode is the value that appears most frequently in a data set.
  * If multiple values have the same highest frequency, the function returns the first one encountered.
  * This function is useful in statistics for identifying the most common value in a sample.
  *
  * ### Example Usage
  * \code{.cpp}
  * std::vector<int> data = {1, 2, 2, 3, 4, 4, 4, 5};
  * int modeValue = mode(data.begin(), data.end());
  * // modeValue is 4
  * \endcode
  */
template<typename It>
auto mode(It first, It last) -> iteratorValueType<It>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    std::map<T, int> h;
    while (first != last) {
        h[*first++]++;
    }

    auto max = std::max_element(h.begin(), h.end(),
                                [](const std::pair<T, int> &p1,
                                const std::pair<T, int> &p2) {
                                    return p1.second < p2.second;
                                });

    return max->first;
}


/*! \} */

} // End namespace tl
