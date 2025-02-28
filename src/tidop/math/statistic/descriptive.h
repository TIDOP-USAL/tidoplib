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


#include "tidop/core/base/defs.h"
#include "tidop/core/base/flags.h"
#include "tidop/math/statistic/mean.h"
#include "tidop/math/statistic/median.h"
#include "tidop/math/statistic/mode.h"
#include "tidop/math/statistic/quantile.h"
#include "tidop/math/statistic/rms.h"
#include "tidop/math/statistic/series.h"
#include "tidop/math/statistic/skewness.h"
#include "tidop/math/statistic/kurtosis.h"

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

public:

    //enum class Config
    //{
    //  sample,
    //  population
    //};

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
        min = (1 << 0),               ///< Minimum value computed
        max = (1 << 1),               ///< Maximum value computed
        mean = (1 << 2),              ///< Mean computed
        median = (1 << 3),            ///< Median computed
        mode = (1 << 4),              ///< Mode computed
        range = (1 << 5),             ///< Range computed
        first_quartile = (1 << 6),    ///< First quartile computed
        second_quartile = (1 << 7),   ///< Second quartile computed
        third_quartile = (1 << 8),    ///< Third quartile computed
        sum_of_squares = (1 << 9),    ///< Sum of squares computed
        rms = (1 << 10),              ///< Root mean square computed
        variance = (1 << 11),         ///< Variance computed
        standar_deviation = (1 << 12) ///< Standard deviation computed
    };

    mutable tl::EnumFlags<InternalStatus> mStatus;
    Series<T> mData;
    Config mConfig;
    std::shared_ptr<Skewness<T>> mSkewnessMethod;
    std::shared_ptr<Kurtosis<T>> mKurtosisMethod;
    mutable T mMin{};
    mutable T mMax{};
    mutable double mMean{};
    mutable T mMedian{};
    mutable T mMode{};
    mutable T mRange{};
    mutable double mVariance{};
    mutable double mStandarDeviation{};
    mutable double mQ1{};
    mutable double mQ2{};
    mutable double mQ3{};
    mutable double mSumOfSquares{};
    mutable double mRootMeanSquare{};


public:

    /*!
     * \brief Default constructor
     * \param config Configuration settings
     */
    DescriptiveStatistics(Config config = Config());

    /*!
     * \brief Constructor with data
     * \param[in] data A series of data values
     * \param[in] config Configuration settings
     */
    DescriptiveStatistics(Series<T> data, Config config = Config());

    /*!
     * \brief Copy constructor
     * \param object An existing DescriptiveStatistics object
     */
    DescriptiveStatistics(const DescriptiveStatistics<T> &object);

    /*!
     * \brief Destructor
     */
    ~DescriptiveStatistics();

    /*!
     * \brief Get the dataset
     * \return The series of data
     */
    auto data() const -> Series<T>;
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
     * \f[ \mu = \frac{\sum_{i=1}^{n}x_i}{n} \f]
     * \return Mean of the dataset
     */
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
    auto median() const -> T;

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
    auto standarDeviation() const -> double;

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
     * \brief thirdQuartile
     * \return
     */
    auto thirdQuartile() const -> double;

    /*!
     * \brief Return the third quartile (Q3)
     * \return The third quartile value
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

    auto octiles() const->std::array<double, 7>;

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

    void configure();

    void computeMinMax() const;
    void computeMean() const;
    void computeSumOfSquares() const;
    void computeRootMeanSquare() const;
    void computeVariance() const;
    void computeStandarDeviation() const;
    void computeMode() const;
    void computeRange() const;
    void computeFirstQuartile() const;
    void computeSecondQuartile() const;
    void computeThirdQuartile() const;
    template<typename It>
    void quantile(It &first, It &last) const;
};

/*! \} */


/* Implementation */

template<typename T>
DescriptiveStatistics<T>::DescriptiveStatistics(Config config)
  : mConfig(std::move(config))
{
    this->configure();
}

template<typename T>
DescriptiveStatistics<T>::DescriptiveStatistics(Series<T> data,
                                                Config config)
  : mData(std::move(data)),
    mConfig(std::move(config))
{
    this->configure();
}

template<typename T>
DescriptiveStatistics<T>::DescriptiveStatistics(const DescriptiveStatistics<T> &object)
  : mData(object.mData),
    mConfig(object.mConfig),
    mSkewnessMethod(object.mSkewnessMethod),
    mKurtosisMethod(object.mKurtosisMethod)
{
    this->configure();
}

template<typename T>
DescriptiveStatistics<T>::~DescriptiveStatistics()
{
}

template<typename T>
auto DescriptiveStatistics<T>::data() const -> Series<T>
{
    return mData;
}

template<typename T>
auto DescriptiveStatistics<T>::min() const -> T
{
    if (!mStatus.isEnabled(InternalStatus::min)) {
        computeMinMax();
    }

    return mMin;
}

template<typename T>
auto DescriptiveStatistics<T>::max() const -> T
{
    if (!mStatus.isEnabled(InternalStatus::max)) {
        computeMinMax();
    }
    return mMax;
}

template<typename T>
auto DescriptiveStatistics<T>::sum() const -> T
{
    T summation{};

    //#ifdef TL_HAVE_OPENMP
    //
    //  T aux{};

    TL_TODO("Hacer prueba de rendimiento")
        //#pragma omp parallel for reduction(+:summation) private(aux) 
        //  for (long long i = 0; i < static_cast<long long>(size()); i++) {
        //    aux = mData[static_cast<size_t>(i)];
        //    summation += aux;
        //  }
        //#else
        for (const auto &data : mData) {
            summation += data;
        }
    //#endif

    return summation;
}

template<typename T>
auto DescriptiveStatistics<T>::mean() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::mean)) {
        computeMean();
    }

    return mMean;
}

template<typename T>
auto DescriptiveStatistics<T>::median() const -> T
{
    if (!mStatus.isEnabled(InternalStatus::median)) {
        computeSecondQuartile();
    }

    return mMedian;
}

template<typename T>
auto DescriptiveStatistics<T>::variance() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::variance)) {
        computeVariance();
    }

    return mVariance;
}

template<typename T>
auto DescriptiveStatistics<T>::standarDeviation() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::standar_deviation)) {
        computeStandarDeviation();
    }

    return mStandarDeviation;
}

template<typename T>
auto DescriptiveStatistics<T>::mode() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::mode)) {
        computeMode();
    }

    return mMode;
}

template<typename T>
auto DescriptiveStatistics<T>::range() const -> T
{
    if (!mStatus.isEnabled(InternalStatus::range)) {
        computeRange();
    }

    return mRange;
}

template<typename T>
auto DescriptiveStatistics<T>::quantile(double p) const -> double
{
    return tl::quantile(mData.begin(), mData.end(), p);
}

template<typename T>
auto DescriptiveStatistics<T>::firstQuartile() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::first_quartile)) {
        computeFirstQuartile();
    }

    return mQ1;
}

template<typename T>
auto DescriptiveStatistics<T>::secondQuartile() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::second_quartile)) {
        computeSecondQuartile();
    }

    return mQ2;
}

template<typename T>
auto DescriptiveStatistics<T>::thirdQuartile() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::third_quartile)) {
        computeThirdQuartile();
    }

    return mQ3;
}

template<typename T>
auto DescriptiveStatistics<T>::quartiles() const -> std::array<double, 3>
{
    return std::array<double, 3> {firstQuartile(), secondQuartile(), thirdQuartile()};
}

template<typename T>
auto DescriptiveStatistics<T>::quintiles() const -> std::array<double, 4>
{
    double q1 = quantile(0.2);
    double q2 = quantile(0.4);
    double q3 = quantile(0.6);
    double q4 = quantile(0.8);
    
    return std::array<double, 4>{q1, q2, q3, q4};
}

template<typename T>
auto DescriptiveStatistics<T>::octiles() const -> std::array<double, 7>
{
    double q1 = quantile(0.125);
    double q2 = quantile(0.250);
    double q3 = quantile(0.375);
    double q4 = quantile(0.500);
    double q5 = quantile(0.625);
    double q6 = quantile(0.750);
    double q7 = quantile(0.875);

    return std::array<double, 7>{q1, q2, q3, q4, q5, q6, q7};
}

template<typename T>
auto DescriptiveStatistics<T>::deciles() const -> std::array<double, 9>
{
    std::array<double, 9> _deciles{};

    quantile(_deciles.begin(), _deciles.end());

    return _deciles;
}

template<typename T>
auto DescriptiveStatistics<T>::percentiles() const -> std::array<double, 99>
{
    std::array<double, 99> _percentiles{};

    quantile(_percentiles.begin(), _percentiles.end());

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
    size_t n = size();
    if (n <= 1) return consts::zero<double>;

    double _mean = mean();
    double sum{};

    for (const auto &data : mData) {
        sum += std::abs(static_cast<double>(data) - _mean);
    }

    return sum / static_cast<T>(n);
}

template<typename T>
auto DescriptiveStatistics<T>::medianAbsoluteDeviation() const -> double
{
    size_t n = size();
    if (n <= 1) return consts::zero<double>;

    double _median = median();

    std::vector<double> x(n);
    auto x_it = x.begin();

    for (const auto &data : mData) {
        *x_it++ = std::abs(static_cast<double>(data) - _median);
    }

    return tl::median(x.begin(), x.end());
}

template<typename T>
auto DescriptiveStatistics<T>::sumOfSquares() const -> double
{
    if (!mStatus.isEnabled(InternalStatus::sum_of_squares)) {
        computeSumOfSquares();
    }

    return mSumOfSquares;
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
    return mSkewnessMethod->eval(*this);
}

template<typename T>
auto DescriptiveStatistics<T>::kurtosis() const -> double
{
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
    return this->standarDeviation() / std::abs(this->mean());
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
    size_t n = this->size();
    if (n <= 2) return consts::zero<double>;

    double median = this->median();
    double mad = this->medianAbsoluteDeviation();

    double num{};
    double den{};
    for (const auto data : mData) {
        double x = static_cast<double>(data) - median;
        double u = x / (9 * mad);
        if (std::abs(u) < consts::one<double>) {
            double u2 = u * u;
            double y = consts::one<double> -u2;
            double y2 = y * y;
            num += x * x * y2 * y2;
            den += y * (consts::one<double> -5. * u2);
        }
    }

    if (den == consts::zero<double>)
        return consts::zero<double>;

    return n * num / (den * den);
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
void DescriptiveStatistics<T>::configure()
{

    //if (mConfig.sample) {

    //} else {

    //}

    mSkewnessMethod = SkewnessFactory<T>::create(mConfig.skewness_method);
    mKurtosisMethod = KurtosisFactory<T>::create(mConfig.kurtosis_method);
}

template<typename T>
void DescriptiveStatistics<T>::computeMinMax() const
{
    auto min_max = std::minmax_element(mData.begin(), mData.end());
    mMin = *min_max.first;
    mMax = *min_max.second;
    mStatus.enable(InternalStatus::min);
    mStatus.enable(InternalStatus::max);
}

template<typename T>
void DescriptiveStatistics<T>::computeMean() const
{
#ifdef TL_HAVE_SIMD_INTRINSICS
    mMean = tl::mean(mData.begin(), mData.end(), true);
#else
    mMean = tl::mean(mData.begin(), mData.end());
#endif

    mStatus.enable(InternalStatus::mean);
}

template<typename T>
void DescriptiveStatistics<T>::computeSumOfSquares() const
{   
    double _mean = mean();
    size_t i{0};

//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    Packed<T> packed;
//    Packed<T> packed_mean(_mean);
//
//    constexpr size_t packed_size = packed.size();
//    size_t max_vector = (mData.size() / packed_size) * packed_size;
//
//    for (i = 0; i < max_vector; i += packed_size) {
//        packed.loadUnaligned(&mData[i]);
//        auto dif = packed - packed_mean;
//        mSumOfSquares += (dif * dif).sum();
//    }
//
//#endif

    for (; i < mData.size(); i++) {
        double dif = static_cast<double>(mData[i]) - _mean;
        mSumOfSquares += dif * dif;
    }

    //for (const auto &data : mData) {
    //    double dif = static_cast<double>(data) - _mean;
    //    mSumOfSquares += dif * dif;
    //}

    mStatus.enable(InternalStatus::sum_of_squares);
}

template<typename T>
void DescriptiveStatistics<T>::computeRootMeanSquare() const
{
    mRootMeanSquare = tl::rootMeanSquare(mData.begin(), mData.end());

    mStatus.enable(InternalStatus::rms);
}

template<typename T>
void DescriptiveStatistics<T>::computeVariance() const
{
    size_t n = mData.size();
    TL_TODO("¿Devolver error?")
    //if (n <= 1) return consts::one<T>;

    double sum{};
    double ep{};
    double aux{};

    double _mean = mean();
    for (const auto &data : mData) {
        aux = data - _mean;
        ep += aux;
        sum += aux * aux;
    }

    size_t div = mConfig.sample ? n - 1 : n;

    mVariance = (sum - ep * ep / static_cast<double>(n)) / static_cast<double>(div);

    mStatus.enable(InternalStatus::variance);
}

template<typename T>
void DescriptiveStatistics<T>::computeStandarDeviation() const
{
    mStandarDeviation = sqrt(variance());
    mStatus.enable(InternalStatus::standar_deviation);
}

template<typename T>
void DescriptiveStatistics<T>::computeMode() const
{
    mMode = tl::mode(mData.begin(), mData.end());
    mStatus.enable(InternalStatus::mode);
}

template<typename T>
void DescriptiveStatistics<T>::computeRange() const
{
    mRange = max() - min();
    mStatus.enable(InternalStatus::range);
}

template<typename T>
void DescriptiveStatistics<T>::computeFirstQuartile() const
{
    mQ1 = tl::quantile(mData.begin(), mData.end(), 0.25);
    mStatus.enable(InternalStatus::first_quartile);
}

template<typename T>
void DescriptiveStatistics<T>::computeSecondQuartile() const
{
    mQ2 = tl::quantile(mData.begin(), mData.end(), 0.5);
    mMedian = static_cast<T>(mQ2);
    mStatus.enable(InternalStatus::second_quartile);
    mStatus.enable(InternalStatus::median);
}

template<typename T>
void DescriptiveStatistics<T>::computeThirdQuartile() const
{
    mQ3 = tl::quantile(mData.begin(), mData.end(), 0.75);
    mStatus.enable(InternalStatus::third_quartile);
}


template<typename T>
template<typename It>
void DescriptiveStatistics<T>::quantile(It &first, It &last) const
{
    auto n = std::distance(first, last);

    double step = (static_cast<double>(n) + 1.) / 10.;
    double p = 0.;
    for (size_t i = 0; i < n; i++) {
        p += step;
        *first++ = quantile(p);
    }
}


} // End namespace tl
