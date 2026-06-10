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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses> *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <vector>
#include <ranges>

#include "tidop/math/statistic/algorithms/descriptive/Quantile.h"
#include "tidop/math/base/Concepts.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \enum TukeyFencesK
 * \brief Enum to specify the type of fence to use.
 * \var outlier Standard outlier detection with \( k = 1.5 \).
 * \var far_out Detection of far out points with \( k = 3 \).
 */
enum class TukeyFencesK
{
    outlier, /*!< \( k = 1.5 \) */
    far_out  /*!< \( k = 3 \) */
};

/*!
 * \brief Tukey's fences
 *
 * Tukey's fences are a statistical method used to identify outliers in a dataset.
 * They utilize the interquartile range (IQR) to establish cut-off points for determining
 * outliers and far out points.
 *
 * The fences are defined as:
 * \f[ \text{Lower Fence} = Q_1 - k \times \text{IQR} \]
 * \f[ \text{Upper Fence} = Q_3 + k \times \text{IQR} \]
 * where \( Q_1 \) is the first quartile, \( Q_3 \) is the third quartile, and \( k \) is a multiplier
 * that typically takes the values 1.5 for outliers and 3 for far out points.
 *
 * \param[in] range The numeric range to evaluate.
 * \param[in] k The type of fence to apply (outlier or far_out).
 * \return A vector of booleans indicating whether each element is within the fences.
 */
template<NumericRange R>
auto tukeyFences(R &&range, TukeyFencesK k = TukeyFencesK::outlier) -> std::vector<bool>
{
    using T = std::ranges::range_value_t<R>;
    using value_type = std::conditional_t<std::integral<T>, double, T>;

    value_type _k{};

    switch (k) {
    case TukeyFencesK::outlier:
        _k = static_cast<value_type>(1.5);
        break;
    case TukeyFencesK::far_out:
        _k = static_cast<value_type>(3.);
        break;
    }

    value_type q1 = quantile(range, 0.25);
    value_type q3 = quantile(range, 0.75);
    value_type iqr = q3 - q1;

    value_type el1 = q1 - iqr * _k;
    value_type el2 = q3 + iqr * _k;

    std::vector<bool> inliers(std::ranges::size(range), false);

    auto out = inliers.begin();
    for (const auto &data : range) {
        *out++ = el1 < static_cast<value_type>(data) && static_cast<value_type>(data) < el2;
    }

    return inliers;
}

/*!
 * \brief Evaluate a dataset to determine if each element is within the Tukey fences.
 * \param[in] first Iterator to the beginning of the dataset.
 * \param[in] last Iterator to the end of the dataset.
 * \param[in] k The type of fence to apply (outlier or far_out).
 * \return A vector of booleans indicating whether each element is within the fences.
 */
template<typename It>
auto tukeyFences(It first, It last, TukeyFencesK k = TukeyFencesK::outlier) -> std::vector<bool>
{
    return tukeyFences(std::ranges::subrange(first, last), k);
}

/*! \} */

} // End namespace tl
