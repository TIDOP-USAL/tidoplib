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
#include "tidop/core/base/common.h"
#include "tidop/math/base/simd.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Calculates the mean (average) of a range of values.
 * 
 *
 * This function computes the mean of a range of values. If the container
 * is contiguous and SIMD intrinsics are available, it uses SIMD instructions for
 * faster computation.
 * 
 * Formula for the mean:
 * \f[ \overline{x} = \frac{\sum_{i=1}^n x_i}{n} \f]
 * 
 * ### Example usage:
 * \code{.cpp}
 * std::vector<int> data = {1, 2, 3, 4, 5};
 * double meanValue = mean(data.begin(), data.end());
 * // meanValue is 3.0
 * \endcode
 * 
 * \tparam It Type of the iterator.
 * \param[in] first Iterator pointing to the beginning of the range.
 * \param[in] last Iterator pointing to the end of the range.
 * \param[in] isContiguousContainer Flag indicating if the container is contiguous (e.g., `std::vector`, `std::array`).
 * \return The mean value
 */

template<typename It>
auto mean(It first, It last, bool isContiguousContainer = false) -> enableIfIntegral<iteratorValueType<It>, double>
{
    double x{0};
    double i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    using T = std::remove_cv_t<iteratorValueType<It>>;

    if (isContiguousContainer) {

        Packed<T> packed_a;

        constexpr size_t packed_size = packed_a.size();
        size_t dist = std::distance(first, last);
        i = static_cast<double>(dist - dist % packed_size);

        while (std::distance(first, last) >= packed_size) {

            packed_a.loadUnaligned(&(*first));
            x += static_cast<double>(packed_a.sum()) / i;
            first = std::next(first, packed_size);
        }
    }

#endif

    while (first != last) {
        x += (static_cast<double>(*first++) - x) / ++i;
    }

    return x;

}

template<typename It>
auto mean(It first, It last, bool isContiguousContainer = false) -> enableIfFloating<iteratorValueType<It>, 
                                                                    std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;
    T x{0};
    T i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    if (isContiguousContainer) {

        Packed<T> packed_a;

        constexpr size_t packed_size = packed_a.size();
        size_t dist = std::distance(first, last);
        i = static_cast<T>(dist - dist % packed_size);

        while (std::distance(first, last) >= packed_size) {

            packed_a.loadUnaligned(&(*first));
            x += (packed_a / i).sum();
            first = std::next(first, packed_size);
        }
    }

#endif

    while (first != last) {
        x += (*first++ - x) / ++i;
    }

    return x;
} 
 

/*! \} */

} // End namespace tl
