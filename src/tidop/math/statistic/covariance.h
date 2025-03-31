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

#include "tidop/core/base/common.h"
#include "tidop/math/statistic/descriptive.h"

namespace tl
{


/*! \addtogroup Statistics
 *  \{
 */


/*!
 * \brief %Covariance of two datasets
 * 
 * The `covariance` function calculates the covariance between two datasets. %Covariance measures
 * the degree to which two random variables change together. It is a measure of the linear relationship 
 * between two variables. The formula for covariance is:
 * 
 * \f[ S_{xy} = \frac{\sum_{i=1}^n (x_i - \overline{x})(y_i - \overline{y})}{n} \f]
 * 
 * \param firstX Iterator to the beginning of the first dataset
 * \param lastX Iterator to the end of the first dataset
 * \param firstY Iterator to the beginning of the second dataset
 * \param lastY Iterator to the end of the second dataset
 * \return The covariance between the two datasets
 * 
 * \note The function assumes that the two datasets have the same length.
 * 
 * ### Example Usage
 * \code{.cpp}
 * #include <iostream>
 * #include <vector>
 * #include <tidop/math/statistic/covariance.h>
 *
 * int main() {
 *     std::vector<double> dataX = {2.3, 3.5, 4.1, 5.2, 6.3};
 *     std::vector<double> dataY = {1.1, 2.2, 3.4, 4.6, 5.0};
 * 
 *     double cov = covariance(dataX.begin(), dataX.end(), dataY.begin(), dataY.end());
 *     Message::info("Covariance: {}", cov)
 * 
 *     return 0;
 * }
 * \endcode
 */
template<typename It>
auto covariance(It firstX, It lastX, It firstY, It lastY) -> enableIfIntegral<iteratorValueType<It>,double>
{
    auto n_x = std::distance(firstX, lastX);
    auto n_y = std::distance(firstY, lastY);
    if (n_x != n_y || n_x <= 1) return consts::zero<double>;

    double mean_x = mean(firstX, lastX);
    double mean_y = mean(firstY, lastY);
    double sum{};
    double x{};
    double y{};

    while (firstX != lastX) {
        x = *firstX++ - mean_x;
        y = *firstY++ - mean_y;
        sum += x * y;
    }

    return sum / n_x;
}

template<typename It>
auto covariance(It firstX, It lastX, It firstY, It lastY) -> enableIfFloating<iteratorValueType<It>,
                                                             std::remove_cv_t<iteratorValueType<It>>>
{
    using T = std::remove_cv_t<iteratorValueType<It>>;

    auto n_x = std::distance(firstX, lastX);
    auto n_y = std::distance(firstY, lastY);
    if (n_x != n_y || n_x <= 1) return consts::zero<T>;

    T mean_x = mean(firstX, lastX);
    T mean_y = mean(firstY, lastY);
    T sum{};
    T x{};
    T y{};

    while (firstX != lastX) {
        x = *firstX++ - mean_x;
        y = *firstY++ - mean_y;
        sum += x * y;
    }

    return sum / n_x;
}


/*!
 * \brief %Covariance class
 *
 * The `Covariance` class provides an interface for calculating the covariance between two series of data.
 * It offers a method to evaluate the covariance between two Series objects. %Covariance measures how two
 * datasets change together, with a positive covariance indicating that the variables tend to increase together,
 * and a negative covariance indicating that one increases while the other decreases.
 *
 * \tparam T The type of the elements in the series (e.g., `double`, `int`).
 * 
 * ### Example Usage
 * \code{.cpp}
 * #include <iostream>
 * #include <vector>
 * #include <tidop/math/statistic/covariance.h>
 *
 * int main() {
 *     std::vector<double> dataX = {2.3, 3.5, 4.1, 5.2, 6.3};
 *     std::vector<double> dataY = {1.1, 2.2, 3.4, 4.6, 5.0};
 *
 *     Covariance<double> covarianceCalculator;
 *     Series<double> seriesX(dataX.begin(), dataX.end());
 *     Series<double> seriesY(dataY.begin(), dataY.end());
 *
 *     double cov = covarianceCalculator.eval(seriesX, seriesY);
 *     Message::info("Covariance: {}", cov)
 *
 *     return 0;
 * }
 * \endcode
 */
template<typename T>
class Covariance
{

public:

    /*!
     * \brief Default constructor
     */
    Covariance() = default;

    /*!
     * \brief Destructor
     */
    ~Covariance() = default;

    /*!
     * \brief Evaluates the covariance between two Series
     *
     * This method calculates the covariance between two series using the formula:
     *
     * \f[ S_{xy} = \frac{\sum_{i=1}^n (x_i - \overline{x})(y_i - \overline{y})}{n} \f]
     *
     * \param[in] series1 The first data series
     * \param[in] series2 The second data series
     * \return The covariance between the two series
     *
     * \note The two series must have the same length.
     */
    auto eval(const Series<T> &series1,
              const Series<T> &series2) -> double;

};


/* Implementation */

template<typename T>
auto Covariance<T>::eval(const Series<T> &series1,
                         const Series<T> &series2) -> double
{

    auto data1_size = series1.size();
    auto data2_size = series2.size();

    TL_ASSERT(data1_size == data2_size && data1_size >= 1, "Invalid data");

    DescriptiveStatistics<T> stat1(series1);
    DescriptiveStatistics<T> stat2(series2);

    double mean_x = stat1.mean();
    double mean_y = stat2.mean();
    double sum{};
    size_t i{0};

//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    Packed<T> packed_a;
//    Packed<T> packed_b;
//    Packed<double> packed_mean_x(mean_x); // mean_x y mean_y son double. Revisar como se hace la resta entre tipos diferentes
//    Packed<double> packed_mean_y(mean_y);
//
//    constexpr size_t packed_size = packed_a.size();
//    size_t max_vector = (data1_size / packed_size) * packed_size;
//    
//    for (i = 0; i < max_vector; i += packed_size) {
//
//        packed_a.loadUnaligned(&series1[i]);
//        packed_b.loadUnaligned(&series2[i]);
//
//        sum += ((packed_a - packed_mean_x) * (packed_b - packed_mean_y)).sum();
//
//    }
//
//#endif

    for (; i < data1_size; i++) {
        sum += (series1[i] - mean_x) * (series2[i] - mean_y);
    }

    return sum / data1_size;
}


/*! \} */

} // End namespace tl
