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

#include <array>
#include <vector>
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <optional>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/flags.h"
#include "tidop/math/statistic/algorithms/descriptive/Mean.h"
#include "tidop/math/statistic/algorithms/descriptive/Median.h"
#include "tidop/math/statistic/algorithms/descriptive/Mode.h"
#include "tidop/math/statistic/algorithms/descriptive/Quantile.h"
#include "tidop/math/statistic/algorithms/descriptive/Variance.h"
#include "tidop/math/statistic/algorithms/descriptive/Moments.h"
#include "tidop/math/statistic/algorithms/ratios/RMS.h"
#include "tidop/math/statistic/algorithms/robust/MAD.h"
#include "tidop/math/statistic/algorithms/robust/BiweightMidvariance.h"
#include "tidop/math/statistic/base/Series.h"
#include "tidop/math/statistic/algorithms/shape/Skewness.h"
#include "tidop/math/statistic/algorithms/shape/Kurtosis.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Class for computing descriptive statistics
 *
 * The `DescriptiveStatistics` class provides a comprehensive set of methods for calculating
 * descriptive statistics on a dataset, including measures like mean, median, mode, variance,
 * standard deviation, quartiles, and more. The class can handle both sample and population
 * data configurations and allows flexibility in choosing the method for skewness calculation.
 * 
 * \tparam T The type of data in the series (e.g., `double`, `int`).
 *
 * ### Example Usage
 * \code{.cpp}
 * // Example usage of the DescriptiveStatistics class
 * 
 * // Creating a dataset of double values
 * std::vector<double> data = {1.5, 2.3, 3.1, 4.4, 5.5, 6.2, 7.7, 8.8, 9.0};
 *
 * // Initializing DescriptiveStatistics object with the dataset
 * DescriptiveStatistics<double> stats(data);
 *
 * // Computing and printing quintiles
 * auto quintiles = stats.quintiles();
 * std::cout << "Quintiles: ";
 * for (const auto& q : quintiles) {
 *     std::cout << q << " ";
 * }
 * std::cout << std::endl;
 *
 * // Computing and printing deciles
 * auto deciles = stats.deciles();
 * std::cout << "Deciles: ";
 * for (const auto& d : deciles) {
 *     std::cout << d << " ";
 * }
 * std::cout << std::endl;
 *
 * // Computing and printing percentiles
 * auto percentiles = stats.percentiles();
 * std::cout << "Percentiles: ";
 * for (const auto& p : percentiles) {
 *     std::cout << p << " ";
 * }
 * std::cout << std::endl;
 * \endcode
 */
template<typename T>
class DescriptiveStatistics
{

    static_assert(Arithmetic<T>, "DescriptiveStatistics requires an arithmetic type (integral or floating-point)");

public:

    using moments_type = CentralMoments<std::conditional_t<std::is_integral_v<T>, double, T>>;

    /*!
     * \brief Configuration struct
     *
     * This structure allows users to configure the behavior of the `DescriptiveStatistics`
     * class. Users can specify whether the data is considered a sample or a population,
     * and select the skewness calculation method.
     */
    struct Config
    {
        bool sample = true; ///< True for sample data, false for population data.
        SkewnessMethod skewness_method = SkewnessMethod::fisher_pearson; ///< Method for calculating skewness.
        KurtosisMethod kurtosis_method = KurtosisMethod::pearson; ///< Method for calculating kurtosis.
    };

private:

    /*!
     * \brief Internal status flags for computed statistics
     *
     * This enum is used internally to track which statistics have been computed.
     */
    enum class InternalStatus
    {
        rms             = (1 << 0),  ///< Root mean square computed
        central_moments = (1 << 1),  ///< Central moments computed
        sorted          = (1 << 2)   ///< Data sorted in cache
    };

    mutable tl::EnumFlags<InternalStatus> mStatus;
    std::vector<T> mData;
    Config mConfig;

    mutable std::shared_ptr<Skewness<T>> mSkewnessMethod;
    mutable std::shared_ptr<Kurtosis<T>> mKurtosisMethod;

    mutable std::optional<T> mMin;
    mutable std::optional<T> mMax;
    mutable std::optional<T> mMode;
    
    mutable double mRootMeanSquare{};
    mutable moments_type mCentralMoments;
    mutable std::vector<T> mSortedData;


public:

    /*!
     * \brief Default constructor
     * \param config Configuration settings
     */
    DescriptiveStatistics(Config config = Config());

    template <typename R>
        requires std::ranges::contiguous_range<R> && std::same_as<std::ranges::range_value_t<R>, T>
    DescriptiveStatistics(const R &container, Config config = Config())
        : mConfig(config)
    {
        mData.assign(std::ranges::begin(container), std::ranges::end(container));
    }

    /*!
     * \brief Destructor
     */
    ~DescriptiveStatistics() = default;

    /*!
     * \brief Get the dataset
     * \return The series of data
     */
    //auto data() const -> Series<T>;

    /*!
     * \brief Get computed central moments
     * \return CentralMoments object containing mean, second, third, and fourth central moments.
     */
    auto centralMoments() const -> const moments_type &;

    /*!
     * \brief Set the dataset
     * \param[in] data A series of data values
     */
    //void setData(Series<T> data);

    /*!
     * \brief Return the smallest value in the dataset
     * \f[ \text{min} = \text{min}(x_i)_{i=1}^{n} \f]
     * \return Minimum value
     */
    auto min() const -> T;

    /*!
     * \brief Return the largest value in the dataset
     * \f[ \text{max} = \text{max}(x_i)_{i=1}^{n} \f]
     * \return Maximum value
     */
    auto max() const -> T;

    /*!
     * \brief Return the sum of all values in the dataset
     * \f[ \text{sum} = \sum_{i=1}^{n}x_i \f]
     * \return Sum of the dataset
     */
    auto sum() const -> T;

    /*!
     * \brief Return the arithmetic mean of the dataset
     * 
     * The arithmetic mean or arithmetic average,
     * or simply just the mean or the average, is the sum
     * of a collection of numbers divided by the count of
     * numbers in the collection.
     *
     * For a population:
     *
     * \f[ \mu = \frac{\sum_{i=1}^{n}x_i}{n} \f]
     *
     * For a sample:
     *
     * \f[ \overline{x} = \frac{\sum_{i=1}^{n}x_i}{n} \f]
     *
     * \return
     */
    auto mean() const -> double;

    /*!
     * \brief Return the median of the dataset
     * 
     * The median is the value separating the higher half from
     * the lower half of a data sample, a population, or a probability
     * distribution.
     * \return Median value
     */
    auto median() const -> double;

    /*!
     * \brief Return the variance of the dataset
     * 
     * Variance measures dispersion of data from the mean.
     * The formula for variance is the sum of squared differences from the
     * mean divided by the size of the data set.
     *
     * For a population:
     *
     * \f[ \sigma^{2} = \frac{\sum_{i=1}^{n}(x_i - \mu)^{2}}{n} \f]
     *
     * For a sample:
     *
     * \f[ s^{2} = \frac{\sum_{i=1}^{n}(x_i - \overline{x})^{2}}{n - 1} \f]
     *
     * \return Variance of the dataset
     */
    auto variance() const -> double;

    /*!
     * \brief Return the standard deviation of the dataset
     * 
     * The standard deviation is a measure of the amount of variation
     * or dispersion of a set of values.
     *
     * For a population:
     *
     * \f[ \sigma = \sqrt{\frac{\sum_{i=1}^{n}(x_i - \mu)^{2}}{n}} \f]
     *
     * For a sample:
     *
     * \f[ s = \sqrt{\frac{\sum_{i=1}^{n}(x_i - \overline{x})^{2}}{n - 1}} \f]
     *
     * \return Standard deviation of the dataset
     */
    auto standardDeviation() const -> double;

    /*!
     * \brief Return the mode of the dataset
     * \return Mode of the dataset
     */
    auto mode() const -> double;

    /*!
     * \brief Return the range of the dataset
     * 
     * The range of a set of data is the difference between
     * the largest and smallest values.
     * \f[ \text{range} = x_n - x_1 \f]
     * \return Range of the dataset
     * \see min, max
     */
    auto range() const -> T;

    /*!
     * \brief Return the specified quantile of the dataset
     * \param[in] p The quantile percentage (between 0 and 1)
     * \return The quantile value
     */
    auto quantile(double p) const -> double;

    /*!
     * \brief Return the first quartile (Q1)
     * \return The first quartile value
     */
    auto firstQuartile() const -> double;

    /*!
     * \brief Return the second quartile (Q2, median)
     * \return The second quartile value
     */
    auto secondQuartile() const -> double;

    /*!
     * \brief Return the third quartile (Q3)
     * \return The third quartile value
     */
    auto thirdQuartile() const -> double;

    /*!
     * \brief Return the first, second and third quartile
     * \return An array containing Q1, Q2, Q3
     */
    auto quartiles() const -> std::array<double, 3>;

    /*!
     * \brief Compute the quintiles of the dataset
     *
     * The quintiles are the values that divide the data into five equal parts. This method computes
     * the 20th, 40th, 60th, and 80th percentiles, which represent the first, second, third, and fourth
     * quintiles of the data, respectively.
     *
     * \return An array containing the first, second, third, and fourth quintiles.
     */
    auto quintiles() const -> std::array<double, 4>;

    auto octiles() const -> std::array<double, 7>;

    /*!
     * \brief Compute the deciles of the dataset
     *
     * The deciles are the values that divide the data into ten equal parts. This method computes the
     * values corresponding to the 10th, 20th, ..., 90th percentiles, representing the deciles of the data.
     *
     * \return An array containing the deciles (9 values corresponding to 10th, 20th, ..., 90th percentiles).
     */
    auto deciles() const -> std::array<double, 9>;

    /*!
     * \brief Compute the percentiles of the dataset
     *
     * The percentiles are the values that divide the data into 100 equal parts. This method computes
     * the values corresponding to the 1st, 2nd, ..., 99th percentiles, representing the percentiles
     * of the data.
     *
     * \return An array containing the percentiles (99 values corresponding to 1st, 2nd, ..., 99th percentiles).
     */
    auto percentiles() const -> std::array<double, 99>;

    /*!
     * \brief Return the interquartile range (IQR)
     * \f[ IQR = Q_3 - Q_1 \f]
     * \return The interquartile range
     */
    auto interquartileRange() const -> double;

    /*!
     * \brief Return the mean absolute deviation (MAD)
     *
     * Mean Absolute Deviation
     *
     * For a Population:
     *
     * \f[ MAD = \frac{\sum_{i=1}^{n}|x_i - \mu|}{n} \f]
     *
     * For a Sample:
     *
     * \f[ MAD = \frac{\sum_{i=1}^{n}|x_i - \overline{x}|}{n}  \f]
     *
     * \return The mean absolute deviation
     */
    auto meanAbsoluteDeviation() const -> double;

    /*!
     * \brief Return the median absolute deviation (MAD)
     * \return The median absolute deviation
     */
    auto medianAbsoluteDeviation() const -> double;

    /*!
     * \brief Return the sum of squared differences from the mean
     * 
     * The sum of squares is the sum of the squared differences between
     * data values and the mean.
     *
     * For a Population:
     *
     * \f[ SS = \sum_{i=1}^{n}(x_i - \mu)^{2} \f]
     *
     * For a Sample:
     *
     * \f[ SS = \sum_{i=1}^{n}(x_i - \overline{x})^{2}  \f]
     *
     * \return The sum of squares
     */
    auto sumOfSquares() const -> double;

    /*!
     * \brief Return the root mean square (RMS) of the dataset
     *
     * The root mean square describes the magnitude of a set of numbers.
     * The formula for root mean square is the square root of the sum of
     * the squared data values divided by n.
     *
     * \f[ RMS = \sqrt{\frac{\sum_{i=1}^{n}x_i^{2}}{n}} \f]
     *
     * \return The root mean square
     */
    auto rootMeanSquare() const -> double;

    /*!
     * \brief Return the skewness of the dataset
     * \return The skewness value
     */
    auto skewness() const -> double;

    /*!
     * \brief Return the kurtosis of the dataset
     * \return The kurtosis value
     */
    auto kurtosis() const -> double;

    /*!
     * \brief Return the excess kurtosis of the dataset
     * \return The kurtosis excess
     */
    auto kurtosisExcess() const -> double;

    /*!
     * \brief Return the coefficient of variation (CV)
     *
     * Coefficient of Variation (CV) or Relative Standard Deviation (RSD)
     * The coefficient of variation describes dispersion of data around the mean.
     * It is the ratio of the standard deviation to the mean. The coefficient of
     * variation is calculated as the standard deviation divided by the mean.
     *
     * For a Population:
     *
     * \f[ C_V = \frac{\sigma}{\mu} \f]
     *
     * For a Sample:
     *
     * \f[ C_V = \frac{\sigma}{\bar{x}} \f]
     *
     * \return The coefficient of variation
     */
    auto coefficientOfVariation() const -> double;

    /*!
     * \brief Return the quartile coefficient of dispersion 
     * 
     * Quartile coefficient of dispersion
     * \f[ \frac{Q_3-Q_1}{Q_3+Q_1} \f]
     * \return The quartile coefficient of dispersion
     */
    auto quartileCoefficientOfDispersion() const -> double;

    /*!
     * \brief Return the quartile deviation
     * \f[ \frac{Q_3-Q_1}{2} \f]
     * \return The quartile deviation
     */
    auto quartileDeviation() const -> double;

    /*!
     * \brief Return the biweight midvariance
     * \return The biweight midvariance
     */
    auto biweightMidvariance() const -> double;

    /*!
     * \brief Return the size of the dataset
     * \return The number of elements in the dataset
     */
    auto size() const -> size_t;

    /*!
     * \brief Return whether the data is treated as a sample
     * \return True if the data is a sample
     */
    auto isSample() const -> bool;

    /*!
     * \brief Return whether the data is treated as a population
     * \return True if the data is a population
     */
    auto isPopulation() const -> bool;

private:

    void computeMinMax() const;
    void computeRootMeanSquare() const;
    void computeMode() const;

    void ensureSorted() const;
    auto sortedQuantile(double p) const -> double;

    template<typename It>
    void fillQuantiles(It first, It last) const;
};

/*! \} */


/* Implementation */

template<typename T>
DescriptiveStatistics<T>::DescriptiveStatistics(Config config)
  : mConfig(std::move(config))
{
}

template<typename T>
auto DescriptiveStatistics<T>::centralMoments() const -> const moments_type &
{
    if (!mStatus.isEnabled(InternalStatus::central_moments)) {
        mCentralMoments = tl::moments(mData);
        mStatus.enable(InternalStatus::central_moments);
    }

    return mCentralMoments;
}

template<typename T>
auto DescriptiveStatistics<T>::min() const -> T
{
    if (!mMin) computeMinMax();
    return *mMin;
}

template<typename T>
auto DescriptiveStatistics<T>::max() const -> T
{
    if (!mMax) computeMinMax();
    return *mMax;
}

template<typename T>
auto DescriptiveStatistics<T>::sum() const -> T
{
    T summation{};

    TL_TODO("Hacer prueba de rendimiento")
    TL_TODO("Utilizar SIMD")
    for (const auto &data : mData) {
        summation += data;
    }

    return summation;
}

template<typename T>
auto DescriptiveStatistics<T>::mean() const -> double
{
    TL_ASSERT(mData.size() > 0, "mean: empty dataset");
    try {
        return centralMoments().mean();
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

template<typename T>
auto DescriptiveStatistics<T>::median() const -> double
{
    return sortedQuantile(0.5);
}

template<typename T>
auto DescriptiveStatistics<T>::variance() const -> double
{
    try {
        size_t n = mData.size();
        TL_ASSERT(n > 0, "empty dataset");

        if (mConfig.sample && n < 2) TL_THROW_EXCEPTION("variance: sample variance undefined for n < 2");

        double sum_sq = centralMoments().secondMoment();
        size_t div = mConfig.sample ? n - 1 : n;

        double var = sum_sq / static_cast<double>(div);

        if (var < 0 && std::abs(var) < 1e-15) var = 0.0;

        return var;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

template<typename T>
auto DescriptiveStatistics<T>::standardDeviation() const -> double
{
    return std::sqrt(variance());
}

template<typename T>
auto DescriptiveStatistics<T>::mode() const -> double
{
    if (!mMode) computeMode();
    return static_cast<double>(*mMode);
}

template<typename T>
auto DescriptiveStatistics<T>::range() const -> T
{
    return max() - min();
}

template<typename T>
auto DescriptiveStatistics<T>::quantile(double p) const -> double
{
    return sortedQuantile(p);
}

template<typename T>
auto DescriptiveStatistics<T>::firstQuartile() const -> double
{
    return sortedQuantile(0.25);
}

template<typename T>
auto DescriptiveStatistics<T>::secondQuartile() const -> double
{
    return sortedQuantile(0.5);
}

template<typename T>
auto DescriptiveStatistics<T>::thirdQuartile() const -> double
{
    return sortedQuantile(0.75);
}

template<typename T>
auto DescriptiveStatistics<T>::quartiles() const -> std::array<double, 3>
{
    return std::array<double, 3> {firstQuartile(), secondQuartile(), thirdQuartile()};
}

template<typename T>
auto DescriptiveStatistics<T>::quintiles() const -> std::array<double, 4>
{
    return std::array<double, 4>{
        sortedQuantile(0.2),
        sortedQuantile(0.4),
        sortedQuantile(0.6),
        sortedQuantile(0.8)
    };
}

template<typename T>
auto DescriptiveStatistics<T>::octiles() const -> std::array<double, 7>
{
    return std::array<double, 7>{
        sortedQuantile(0.125),
        sortedQuantile(0.250),
        sortedQuantile(0.375),
        sortedQuantile(0.500),
        sortedQuantile(0.625),
        sortedQuantile(0.750),
        sortedQuantile(0.875)
    };
}

template<typename T>
auto DescriptiveStatistics<T>::deciles() const -> std::array<double, 9>
{
    std::array<double, 9> _deciles{};
    fillQuantiles(_deciles.begin(), _deciles.end());
    return _deciles;
}

template<typename T>
auto DescriptiveStatistics<T>::percentiles() const -> std::array<double, 99>
{
    std::array<double, 99> _percentiles{};
    fillQuantiles(_percentiles.begin(), _percentiles.end());
    return _percentiles;
}

template<typename T>
auto DescriptiveStatistics<T>::interquartileRange() const -> double
{
    return thirdQuartile() - firstQuartile();
}

template<typename T>
auto DescriptiveStatistics<T>::meanAbsoluteDeviation() const -> double
{
    return tl::meanAbsoluteDeviation(mData);
}

template<typename T>
auto DescriptiveStatistics<T>::medianAbsoluteDeviation() const -> double
{
    return tl::medianAbsoluteDeviation(mData);
}

template<typename T>
auto DescriptiveStatistics<T>::sumOfSquares() const -> double
{
    TL_ASSERT(mData.size() > 0, "sumOfSquares: empty dataset");
    return centralMoments().secondMoment();
}

template<typename T>
auto DescriptiveStatistics<T>::rootMeanSquare() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::rms)) {
        computeRootMeanSquare();
    }

    return mRootMeanSquare;
}

template<typename T>
auto DescriptiveStatistics<T>::skewness() const -> double
{
    if (!mSkewnessMethod)
        mSkewnessMethod = SkewnessFactory<T>::create(mConfig.skewness_method);

    return mSkewnessMethod->eval(*this);
}

template<typename T>
auto DescriptiveStatistics<T>::kurtosis() const -> double
{
    if (!mKurtosisMethod)
        mKurtosisMethod = KurtosisFactory<T>::create(mConfig.kurtosis_method);

    return mKurtosisMethod->eval(*this);
}

template<typename T>
auto DescriptiveStatistics<T>::kurtosisExcess() const -> double
{
    size_t n = size();

    if (n < (mConfig.sample ? 4 : 1)) return consts::zero<double>;

    double kurtosis_excess{};

    if (mConfig.sample) {
        kurtosis_excess = this->kurtosis() - 3. * static_cast<T>(n - 1) * static_cast<T>(n - 1) / (static_cast<T>(n - 2) * static_cast<T>(n - 3));
    } else {
        kurtosis_excess = this->kurtosis() - 3.;
    }

    return kurtosis_excess;
}

template<typename T>
auto DescriptiveStatistics<T>::coefficientOfVariation() const -> double
{
    return this->standardDeviation() / std::abs(this->mean());
}

template<typename T>
auto DescriptiveStatistics<T>::quartileCoefficientOfDispersion() const -> double
{
    double q1 = this->firstQuartile();
    double q3 = this->thirdQuartile();

    return (q3 - q1) / (q3 + q1);
}

template<typename T>
auto DescriptiveStatistics<T>::quartileDeviation() const -> double
{
    return this->interquartileRange() / consts::two<double>;
}

template<typename T>
auto DescriptiveStatistics<T>::biweightMidvariance() const -> double
{
    return tl::biweightMidvariance(mData);
}


template<typename T>
auto DescriptiveStatistics<T>::size() const -> size_t
{
    return mData.size();
}

template<typename T>
bool DescriptiveStatistics<T>::isSample() const
{
    return mConfig.sample;
}

template<typename T>
bool DescriptiveStatistics<T>::isPopulation() const
{
    return !mConfig.sample;
}

template<typename T>
void DescriptiveStatistics<T>::computeMinMax() const
{
    if (mStatus.isEnabled(InternalStatus::sorted)) {
        mMin = mSortedData.front();
        mMax = mSortedData.back();
    } else {
        auto [_min, _max] = std::minmax_element(mData.begin(), mData.end());
        mMin = *_min;
        mMax = *_max;
    }
}

template<typename T>
void DescriptiveStatistics<T>::computeRootMeanSquare() const
{
    mRootMeanSquare = tl::rootMeanSquare(mData);
    mStatus.enable(InternalStatus::rms);
}

template<typename T>
void DescriptiveStatistics<T>::computeMode() const
{
    mMode = tl::mode(mData.begin(), mData.end());
    //mStatus.enable(InternalStatus::mode);
}

template<typename T>
void DescriptiveStatistics<T>::ensureSorted() const
{
    if (!mStatus.isEnabled(InternalStatus::sorted)) {
        mSortedData.assign(mData.begin(), mData.end());
        std::sort(mSortedData.begin(), mSortedData.end());
        mStatus.enable(InternalStatus::sorted);
    }
}

template<typename T>
auto DescriptiveStatistics<T>::sortedQuantile(double p) const -> double
{
    ensureSorted();
    size_t n = mSortedData.size();
    TL_ASSERT(n > 0, "sortedQuantile: empty range");
    TL_ASSERT(std::isfinite(p), "sortedQuantile: p must be finite");
    TL_ASSERT(p >= 0. && p <= 1., "sortedQuantile: p must be in the range [0, 1]");

    // Boundary cases
    if (p == 0.0) return static_cast<double>(mSortedData.front());
    if (p == 1.0) return static_cast<double>(mSortedData.back());

    // Hyndman & Fan type 7
    double h = (static_cast<double>(n) - 1.0) * p;
    size_t i = static_cast<size_t>(std::floor(h));
    double f = h - static_cast<double>(i);

    return (i + 1 < n)
        ? static_cast<double>(mSortedData[i] + f * (static_cast<double>(mSortedData[i + 1]) - static_cast<double>(mSortedData[i])))
        : static_cast<double>(mSortedData[i]);
}

template<typename T>
template<typename It>
void DescriptiveStatistics<T>::fillQuantiles(It first, It last) const
{
    size_t n = std::distance(first, last);

    double step = 1.0 / static_cast<double>(n + 1);
    double p = 0.;
    for (size_t i = 0; i < n; i++) {
        p += step;
        *first++ = sortedQuantile(p);
    }
}


} // End namespace tl
