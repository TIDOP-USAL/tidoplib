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
#include <string> 

#include "tidop/core/defs.h"
#include "tidop/core/flags.h"
#include "tidop/math/statistic/mean.h"
#include "tidop/math/statistic/median.h"
#include "tidop/math/statistic/mode.h"
#include "tidop/math/statistic/quantile.h"
#include "tidop/math/statistic/rms.h"
#include "tidop/math/statistic/series.h"
#include "tidop/math/statistic/skewness.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */

/*! \defgroup statistics Statistics
 *  \{
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

    struct Config
    {
        bool sample = true;
        SkewnessMethod skewness_method = SkewnessMethod::fisher_pearson;
    };

private:

    enum class InternalStatus
    {
        min = (1 << 0),
        max = (1 << 1),
        mean = (1 << 2),
        median = (1 << 3),
        mode = (1 << 4),
        range = (1 << 5),
        first_quartile = (1 << 6),
        second_quartile = (1 << 7),
        third_quartile = (1 << 8),
        sum_of_squares = (1 << 9),
        rms = (1 << 10),
        variance = (1 << 11),
        standar_deviation = (1 << 12)
    };

    mutable tl::EnumFlags<InternalStatus> mStatus;
    Series<T> mData;
    Config mConfig;
    std::shared_ptr<Skewness<T>> mSkewnessMethod;
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

    DescriptiveStatistics(Config config = Config());
    DescriptiveStatistics(Series<T> data,
                          Config config = Config());
    DescriptiveStatistics(const DescriptiveStatistics<T> &object);
    ~DescriptiveStatistics();

    auto data() const -> Series<T>;

    /*!
     * \brief Return the smallest value
     * \f[ \text{min} = \text{min}(x_i)_{i=1}^{n} \f]
     * \return Minimun value
     */
    auto min() const -> T;

    /*!
     * \brief Returns the greatest value
     * \f[ \text{max} = \text{max}(x_i)_{i=1}^{n} \f]
     * \return Maximum value
     */
    auto max() const -> T;

    /*!
     * \brief Total of all data values
     * \f[ \text{sum} = \sum_{i=1}^{n}x_i \f]
     * \return
     */
    auto sum() const -> T;

    /*!
     * \brief The arithmetic mean or arithmetic average,
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
     * \brief The median is the value separating the higher half from
     * the lower half of a data sample, a population, or a probability
     * distribution.
     * \return
     */
    auto median() const -> T;

    /*!
     * \brief Variance measures dispersion of data from the mean.
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
     * \return
     */
    auto variance() const -> double;

    /*!
     * \brief The standard deviation is a measure of the amount of variation
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
     * \return
     */
    auto standarDeviation() const -> double;

    /*!
     * \brief mode
     * \return
     */
    auto mode() const -> double;

    /*!
     * \brief The range of a set of data is the difference between
     * the largest and smallest values.
     * \f[ \text{range} = x_n - x_1 \f]
     * \return Range
     * \see min, max
     */
    auto range() const -> T;

    /*!
     * \brief Quantile
     * \return
     */
    auto quantile(double p) const -> double;

    /*!
     * \brief firstQuartile
     * \return
     */
    auto firstQuartile() const -> double;

    /*!
     * \brief secondQuartile
     * \return
     */
    auto secondQuartile() const -> double;

    /*!
     * \brief thirdQuartile
     * \return
     */
    auto thirdQuartile() const -> double;

    auto quartiles() const -> std::array<double, 3>;
    auto quintiles() const -> std::array<double, 4>;
    auto deciles() const -> std::array<double, 9>;
    auto percentiles() const -> std::array<double, 99>;

    /*!
     * \brief Interquartile Range
     * \f[ IQR = Q_3 - Q_1 \f]
     * \return
     */
    auto interquartileRange() const -> double;

    /*!
     * \brief Mean Absolute Deviation
     *
     * For a Population:
     *
     * \f[ MAD = \frac{\sum_{i=1}^{n}|x_i - \mu|}{n} \f]
     *
     * For a Sample:
     *
     * \f[ MAD = \frac{\sum_{i=1}^{n}|x_i - \overline{x}|}{n}  \f]
     *
     * \return
     */
    auto meanAbsoluteDeviation() const -> double;

    /*!
     * \brief Median Absolute Deviation
     * \return
     */
    auto medianAbsoluteDeviation() const -> double;

    /*!
     * \brief Sum of Squares
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
     * \return
     */
    auto sumOfSquares() const -> double;

    /*!
     * \brief Root Mean Square
     *
     * The root mean square describes the magnitude of a set of numbers.
     * The formula for root mean square is the square root of the sum of
     * the squared data values divided by n.
     *
     * \f[ RMS = \sqrt{\frac{\sum_{i=1}^{n}x_i^{2}}{n}} \f]
     *
     * \return
     */
    auto rootMeanSquare() const -> double;

    /*!
     * \brief Skewness
     * \return
     */
    auto skewness() const -> double;

    /*!
     * \brief Kurtosis
     * \return
     */
    auto kurtosis() const -> double;

    auto kurtosisExcess() const -> double;

    /*!
     * \brief Coefficient of Variation (CV) or Relative Standard Deviation (RSD)
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
     * \return
     */
    auto coefficientOfVariation() const -> double;

    /*!
     * \brief Quartile coefficient of dispersion
     * \f[ \frac{Q_3-Q_1}{Q_3+Q_1} \f]
     * \return
     */
    auto quartileCoefficientOfDispersion() const -> double;

    /*!
     * \brief Quartile Deviation
     * \f[ \frac{Q_3-Q_1}{2} \f]
     */
    auto quartileDeviation() const -> double;

    auto biweightMidvariance() const -> double;

    auto size() const -> size_t;
    auto isSample() const -> bool;
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
    mSkewnessMethod(object.mSkewnessMethod)
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

    return sum / n;
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
    size_t n = size();

    if (n <= 1) return consts::zero<double>;

    double _mean = mean();
    double dif{};
    double dif2{};
    double _kurtosis{};

    for (const auto &data : mData) {
        dif = static_cast<double>(data) - _mean;
        dif2 = dif * dif;
        _kurtosis += dif2 * dif2;
    }

    double _variance = variance();

    if (_variance == consts::zero<double>) return consts::zero<double>;

    if (mConfig.sample) {
        _kurtosis = n * (n + 1) * _kurtosis /
            ((n - 1) * (n - 2) * (n - 3) * _variance * _variance);
    } else {
        _kurtosis = _kurtosis /
            (n * _variance * _variance);
    }

    return _kurtosis;
}

template<typename T>
auto DescriptiveStatistics<T>::kurtosisExcess() const -> double
{
    size_t n = size();
    double kurtosis_excess{};

    if (mConfig.sample) {
        kurtosis_excess = this->kurtosis() - 3. * (n - 1) * (n - 1) / ((n - 2) * (n - 3));
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

    if (mConfig.sample) {

    } else {

    }

    mSkewnessMethod = SkewnessFactory<T>::create(mConfig.skewness_method);

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


/*! \} */ // end of statistic

/*! \} */ // end of math



} // End namespace tl
