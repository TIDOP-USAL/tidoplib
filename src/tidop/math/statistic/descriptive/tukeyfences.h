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


#include "tidop/math/statistic/descriptive.h"


namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */


/*!
 * \brief Tukey's fences
 *
 * Tukey's fences are a statistical method used to identify outliers in a dataset.
 * They utilize the interquartile range (IQR) to establish cut-off points for determining
 * outliers and far out points.
 *
 * The fences are defined as:
 * \f[
 * \text{Lower Fence} = Q_1 - k \times \text{IQR}
 * \]
 * \f[
 * \text{Upper Fence} = Q_3 + k \times \text{IQR}
 * \]
 * where \( Q_1 \) is the first quartile, \( Q_3 \) is the third quartile, and \( k \) is a multiplier
 * that typically takes the values 1.5 for outliers and 3 for far out points.
 *
 * \tparam T Type of the elements in the series.
 */
template<typename T>
class TukeyFences
{

public:

    /*!
     * \enum K
     * \brief Enum to specify the type of fence to use.
     * \var outlier Standard outlier detection with \( k = 1.5 \).
     * \var far_out Detection of far out points with \( k = 3 \).
     */
    enum class K
    {
        outlier, /*!< \( k = 1.5 \) */
        far_out  /*!< \( k = 3 \) */
    };

public:

    TukeyFences() = default;
    ~TukeyFences() = default;

    /*!
     * \brief Evaluate a series to determine if each element is within the Tukey fences.
     * \param[in] series The series of data points to evaluate.
     * \param[in] k The type of fence to apply (outlier or far_out).
     * \return A vector of booleans indicating whether each element is within the fences.
     */
    auto eval(const Series<T>& series, K k = K::outlier) -> std::vector<bool>;

};

/*! \} */


/* Implementation */

template<typename T>
auto TukeyFences<T>::eval(const Series<T> &series, TukeyFences<T>::K k) -> std::vector<bool>
{
    DescriptiveStatistics<T> stat(series);

    double _k{};

    switch (k) {
    case K::outlier:
        _k = 1.5;
        break;
    case K::far_out:
        _k = 3.;
        break;
    }

    T el1 = stat.firstQuartile() - stat.interquartileRange() * _k;
    T el2 = stat.thirdQuartile() + stat.interquartileRange() * _k;

    std::vector<bool> inliers(stat.size(), false);

    auto out = inliers.begin();
    for (const auto &data : series) {
        *out++ = el1 < data && data < el2;
    }

    return inliers;
}

} // End namespace tl