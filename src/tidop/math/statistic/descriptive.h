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

#ifndef TL_MATH_DESCRIPTIVE_STATISTIC_H
#define TL_MATH_DESCRIPTIVE_STATISTIC_H

#include <vector> 
#include <string> 

#include <tidop/core/defs.h>
#include <tidop/core/flags.h>
#include <tidop/math/statistic/series.h>
#include <tidop/math/statistics.h>

namespace tl
{

namespace math
{

/*! \addtogroup math
 *  \{
 */


enum class SkewnessMethod
{
  fisher_pearson, //adjusted Fisher - Pearson standardized moment coefficient
  pearson_mode,
  bowley,
  momental
};

enum class KurtosisMethod
{
  
};

template<typename T> class DescriptiveStatistics;


/*! \defgroup statistics Statistics
 *  \{
 */

template<typename T>
class Algorithm
{

public:

  Algorithm() = default;
  virtual ~Algorithm() = default;

  virtual double eval(DescriptiveStatistics<T> *stat) = 0;
  ////virtual double eval(const Series<T> &data) = 0;
};


template<typename T>
class Skewness
  : public Algorithm<T>
{

public:

  Skewness()
  {
  }
  ~Skewness() override
  {
  }

};

template<typename T>
class SkewnessFactory
{

private:

  SkewnessFactory() = default;

public:

  /*!
   * \brief 
   * \param[in] method
   * \see SkewnessMethod
   * \return 
  */
  static std::shared_ptr<Skewness<T>> create(SkewnessMethod method);
};


//template<typename T>
//class Kurtosis
//  : public Algorithm<T>
//{
//
//public:
//
//  Kurtosis()
//  {
//  }
//  ~Kurtosis() override
//  {
//  }
//
//};
//
//template<typename T>
//class KurtosisFactory
//{
//
//private:
//
//  KurtosisFactory() = default;
//
//public:
//
//  static std::shared_ptr<Kurtosis<T>> create(KurtosisMethod method);
//};


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

public:

  DescriptiveStatistics(Config config = Config());
  DescriptiveStatistics(Series<T> data, 
                        Config config = Config());
  DescriptiveStatistics(const DescriptiveStatistics<T> &object);
  ~DescriptiveStatistics();

  Series<T> data();

  /*!
   * \brief Return the smallest value
   * \f[ \text{min} = \text{min}(x_i)_{i=1}^{n} \f]
   * \return Minimun value
   */
  T min();

  /*!
   * \brief Returns the greatest value
   * \f[ \text{max} = \text{max}(x_i)_{i=1}^{n} \f]
   * \return Maximum value
   */
  T max();

  /*!
   * \brief Total of all data values
   * \f[ \text{sum} = \sum_{i=1}^{n}x_i \f]
   * \return
   */
  T sum();

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
  double mean();

  /*!
   * \brief The median is the value separating the higher half from 
   * the lower half of a data sample, a population, or a probability
   * distribution.
   * \return
   */
  T median();

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
  double variance();

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
  double standarDeviation();

  /*!
   * \brief mode
   * \return
   */
  double mode();

  /*!
   * \brief The range of a set of data is the difference between
   * the largest and smallest values. 
   * \f[ \text{range} = x_n - x_1 \f]
   * \return Range
   * \see min, max
   */
  T range();

  /*!
   * \brief firstQuartile
   * \return
   */
  double firstQuartile();

  /*!
   * \brief secondQuartile
   * \return
   */
  double secondQuartile();

  /*!
   * \brief thirdQuartile
   * \return
   */
  double thirdQuartile();

  /*!
   * \brief Interquartile Range
   * \f[ IQR = Q_3 - Q_1 \f]
   * \return
   */
  double interquartileRange();

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
  double meanAbsoluteDeviation();

  /*!
   * \brief Median Absolute Deviation
   * \return
   */
  double medianAbsoluteDeviation();

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
  double sumOfSquares();

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
  double rootMeanSquare();

  /*!
   * \brief Skewness
   * \return
   */
  double skewness();

  /*!
   * \brief Kurtosis
   * \return
   */
  double kurtosis();

  double kurtosisExcess();

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
  double coefficientOfVariation();

  /*!
   * \brief Quartile coefficient of dispersion
   * \f[ \frac{Q_3-Q_1}{Q_3+Q_1} \f]
   * \return
   */
  double quartileCoefficientOfDispersion();

  /*!
   * \brief Quartile Deviation
   * \f[ \frac{Q_3-Q_1}{2} \f]
   */
  double quartileDeviation();

  double biweightMidvariance();

  size_t size() const;
  bool isSample() const;
  bool isPopulation() const;

private:

  void configure();

  void computeMinMax();
  void computeMean();
  void computeSumOfSquares();
  void computeRootMeanSquare();
  void computeVariance();
  void computeStandarDeviation();
  void computeMode();
  void computeRange();
  void computeFirstQuartile();
  void computeSecondQuartile();
  void computeThirdQuartile();

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

  tl::EnumFlags<InternalStatus> mStatus;
  Series<T> mData;
  Config mConfig;
  std::shared_ptr<Skewness<T>> mSkewnessMethod;
  T mMin{};
  T mMax{};
  double mMean{};
  T mMedian{};
  T mMode{};
  T mRange{};
  double mVariance{};
  double mStandarDeviation{};
  double mQ1{};
  double mQ2{};
  double mQ3{};
  double mSumOfSquares{};
  double mRootMeanSquare{};
  
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

template<typename T> inline
Series<T> DescriptiveStatistics<T>::data()
{
  return mData;
}

template<typename T> inline
T DescriptiveStatistics<T>::min()
{
  if (!mStatus.isActive(InternalStatus::min)) {
    computeMinMax();
  }
  return mMin;
}

template<typename T> inline
T DescriptiveStatistics<T>::max()
{
  if (!mStatus.isActive(InternalStatus::max)) {
    computeMinMax();
  }
  return mMax;
}

template<typename T> inline
T DescriptiveStatistics<T>::sum()
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

template<typename T> inline
double DescriptiveStatistics<T>::mean()
{
  if (!mStatus.isActive(InternalStatus::mean)) {
    computeMean();
  }
  return mMean;
}

template<typename T> inline
T DescriptiveStatistics<T>::median()
{
  if (!mStatus.isActive(InternalStatus::median)) {
    computeSecondQuartile();
  }
  return mMedian;
}

template<typename T> inline
double DescriptiveStatistics<T>::variance()
{
  if (!mStatus.isActive(InternalStatus::variance)) {
    computeVariance();
  }
  return mVariance;
}

template<typename T> inline
double DescriptiveStatistics<T>::standarDeviation()
{
  if (!mStatus.isActive(InternalStatus::standar_deviation)) {
    computeStandarDeviation();
  }
  return mStandarDeviation;
}

template<typename T> inline
double DescriptiveStatistics<T>::mode()
{
  if (!mStatus.isActive(InternalStatus::mode)) {
    computeMode();
  }
  return mMode;
}

template<typename T> inline
T DescriptiveStatistics<T>::range()
{
  if (!mStatus.isActive(InternalStatus::range)) {
    computeRange();
  }
  return mRange;
}

template<typename T> inline
double DescriptiveStatistics<T>::firstQuartile()
{
  if (!mStatus.isActive(InternalStatus::first_quartile)) {
    computeFirstQuartile();
  }
  return mQ1;
}

template<typename T> inline
double DescriptiveStatistics<T>::secondQuartile()
{
  if (!mStatus.isActive(InternalStatus::second_quartile)) {
    computeSecondQuartile();
  }
  return mQ2;
}

template<typename T> inline
double DescriptiveStatistics<T>::thirdQuartile()
{
  if (!mStatus.isActive(InternalStatus::third_quartile)) {
    computeThirdQuartile();
  }
  return mQ3;
}

template<typename T> inline
double DescriptiveStatistics<T>::interquartileRange()
{
  return thirdQuartile() - firstQuartile();
}

template<typename T> inline
double DescriptiveStatistics<T>::meanAbsoluteDeviation()
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

template<typename T> inline
double DescriptiveStatistics<T>::medianAbsoluteDeviation()
{
  size_t n = size();
  if (n <= 1) return consts::zero<double>;

  double _median = median();

  std::vector<double> x(n);
  auto x_it = x.begin();

  for (const auto &data : mData) {
    *x_it++ = std::abs(static_cast<double>(data) - _median);
  }

  return tl::math::median(x.begin(), x.end());
}

template<typename T> inline
double DescriptiveStatistics<T>::sumOfSquares()
{
  if (!mStatus.isActive(InternalStatus::sum_of_squares)) {
    computeSumOfSquares();
  }

  return mSumOfSquares;
}

template<typename T> inline
double DescriptiveStatistics<T>::rootMeanSquare()
{
  if (!mStatus.isActive(InternalStatus::rms)) {
    computeRootMeanSquare();
  }

  return mRootMeanSquare;
}

template<typename T> inline
double DescriptiveStatistics<T>::skewness()
{
  return mSkewnessMethod->eval(this);
}

template<typename T> inline
double DescriptiveStatistics<T>::kurtosis()
{
  size_t n = size();

  if(n <= 1) return consts::zero<double>;

  double _mean = mean();
  double dif{};
  double dif2{};
  double _kurtosis{};

  for(const auto &data : mData) {
    dif = static_cast<double>(data) - _mean;
    dif2 = dif * dif;
    _kurtosis += dif2 * dif2;
  }

  double _variance = variance();

  if(_variance == consts::zero<double>) return consts::zero<double>;

  if(mConfig.sample) {
    _kurtosis = n * (n + 1) * _kurtosis /
      ((n - 1) * (n - 2) * (n - 3) * _variance * _variance);
  } else {
    _kurtosis = _kurtosis /
      (n * _variance * _variance);
  }

  return _kurtosis;
}

template<typename T>
inline double DescriptiveStatistics<T>::kurtosisExcess()
{
  int n = size();
  double kurtosis_excess{};

  if (mConfig.sample) {
    kurtosis_excess = this->kurtosis() - 3. * (n - 1)*(n - 1) / ((n - 2) * (n - 3));
  } else {
    kurtosis_excess = this->kurtosis() - 3.;
  }

  return kurtosis_excess;
}

template<typename T> inline
double DescriptiveStatistics<T>::coefficientOfVariation()
{
  return this->standarDeviation() / std::abs(this->mean());
}

template<typename T> inline
double DescriptiveStatistics<T>::quartileCoefficientOfDispersion()
{
  double q1 = this->firstQuartile();
  double q3 = this->thirdQuartile();

  return (q3 - q1) / (q3 + q1);
}

template<typename T> inline
double DescriptiveStatistics<T>::quartileDeviation()
{
  return this->interquartileRange() / consts::two<double>;
}

template<typename T> inline
double DescriptiveStatistics<T>::biweightMidvariance()
{
  size_t n = this->size();
  if(n <= 2) return consts::zero<double>;

  double median = this->median();
  double mad = this->medianAbsoluteDeviation();

  double num{};
  double den{};
  for (const auto data : mData) {
    double x = static_cast<double>(data) - median;
    double u = x / (9 * mad);
    if(std::abs(u) < consts::one<double>) {
      double u2 = u * u;
      double y = consts::one<double> - u2;
      double y2 = y * y;
      num += x * x * y2 * y2;
      den += y * (consts::one<double> -static_cast<double>(5) * u2);
    }
  }

  if(den == consts::zero<double>)
    return consts::zero<double>;

  return n * num / (den * den);
}


template<typename T> inline
size_t DescriptiveStatistics<T>::size() const
{
  return mData.size();
}

template<typename T>
inline bool DescriptiveStatistics<T>::isSample() const
{
  return mConfig.sample;
}

template<typename T>
inline bool DescriptiveStatistics<T>::isPopulation() const
{
  return !mConfig.sample;
}

template<typename T>
void DescriptiveStatistics<T>::configure()
{

  if(mConfig.sample) {

  } else {

  }

  mSkewnessMethod =  SkewnessFactory<T>::create(mConfig.skewness_method);

}

template<typename T> inline
void DescriptiveStatistics<T>::computeMinMax()
{
  auto min_max = std::minmax_element(mData.begin(), mData.end());
  mMin = *min_max.first;
  mMax = *min_max.second;
  mStatus.flagOn(InternalStatus::min);
  mStatus.flagOn(InternalStatus::max);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeMean()
{
  mMean = tl::math::mean(mData.begin(), mData.end());

  mStatus.flagOn(InternalStatus::mean);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeSumOfSquares()
{
  mSumOfSquares = tl::math::sumOfSquares(mData.begin(), mData.end());

  mStatus.flagOn(InternalStatus::sum_of_squares);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeRootMeanSquare()
{
  mRootMeanSquare = tl::math::rootMeanSquare(mData.begin(), mData.end());

  mStatus.flagOn(InternalStatus::rms);
}

template<typename T>
void DescriptiveStatistics<T>::computeVariance()
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

  double div = mConfig.sample ? n - 1 : n;

  mVariance = (sum - ep * ep / n) / div;

  mStatus.flagOn(InternalStatus::variance);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeStandarDeviation()
{
  mStandarDeviation = sqrt(variance());
  mStatus.flagOn(InternalStatus::standar_deviation);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeMode()
{
  mMode = tl::math::mode(mData.begin(), mData.end());
  mStatus.flagOn(InternalStatus::mode);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeRange()
{
  mRange = max() - min();
  mStatus.flagOn(InternalStatus::range);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeFirstQuartile()
{
  mQ1 = tl::math::quantile(mData.begin(), mData.end(), 0.25);
  mStatus.flagOn(InternalStatus::first_quartile);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeSecondQuartile()
{
  mMedian = mQ2 = tl::math::quantile(mData.begin(), mData.end(), 0.5);
  mStatus.flagOn(InternalStatus::second_quartile);
  mStatus.flagOn(InternalStatus::median);
}

template<typename T> inline
void DescriptiveStatistics<T>::computeThirdQuartile()
{
  mQ3 = tl::math::quantile(mData.begin(), mData.end(), 0.75);
  mStatus.flagOn(InternalStatus::third_quartile);
}


/// \cond

namespace internal
{

template<typename T>
class SkewnessFisherPearson
  : public Skewness<T>
{

public:

  SkewnessFisherPearson() : Skewness<T>()
  {
  }
  ~SkewnessFisherPearson() override = default;

  double eval(DescriptiveStatistics<T> *stat) override
  {
    size_t n = stat->size();

    if(n <= 1) return consts::zero<double>;

    double _mean = stat->mean();
    double dif{};
    double skew{};

    for(const auto &data : stat->data()) {
      dif = static_cast<double>(data) - _mean;
      skew += dif * dif * dif;
    }

    double _variance = stat->variance();

    if(_variance == consts::zero<double>) return consts::zero<double>;

    if(stat->isSample()) {
      return skew * n / ((n - 1) * (n - 2) * _variance * stat->standarDeviation());
    } else {
      return skew / (n * _variance * stat->standarDeviation());
    }
  }

};


template<typename T>
class SkewnessPearson
  : public Skewness<T>
{

public:

  SkewnessPearson() : Skewness<T>() {}
  ~SkewnessPearson() override = default;

  double eval(DescriptiveStatistics<T> *stat) override
  {
    return (stat->mean() - stat->mode()) / stat->standarDeviation();
  }

};


template<typename T>
class SkewnessBowley
  : public Skewness<T>
{

public:

  SkewnessBowley() : Skewness<T>() {}
  ~SkewnessBowley() override = default;

  double eval(DescriptiveStatistics<T> *stat) override
  {
    return (stat->thirdQuartile() + stat->firstQuartile() - 2 * stat->secondQuartile()) /
      stat->interquartileRange();
  }

};


template<typename T>
class SkewnessMomental
  : public Skewness<T>
{

public:

  SkewnessMomental() : Skewness<T>()
  {
  }
  ~SkewnessMomental() override = default;

  double eval(DescriptiveStatistics<T> *stat) override
  {
    return 0.0;
  }

};


//template<typename T>
//class KurtosisB2
//  : public Kurtosis<T>
//{
//
//public:
//
//  KurtosisB2() : Kurtosis<T>()
//  {
//  }
//  ~KurtosisB2() override = default;
//
//  double eval(DescriptiveStatistics<T> *stat) override
//  {
//    return 0.0;
//  }
//
//};


} // internal

/// \endcond

template<typename T> inline
std::shared_ptr<Skewness<T>> SkewnessFactory<T>::create(SkewnessMethod method)
{
  std::shared_ptr<Skewness<T>> skew;

  switch(method) {
    case tl::math::SkewnessMethod::fisher_pearson:
      skew = std::make_shared<internal::SkewnessFisherPearson<T>>();
      break;
    case tl::math::SkewnessMethod::pearson_mode:
      skew = std::make_shared<internal::SkewnessPearson<T>>();
      break;
    case tl::math::SkewnessMethod::bowley:
      skew = std::make_shared<internal::SkewnessBowley<T>>();
      break;
    case tl::math::SkewnessMethod::momental:
      skew = std::make_shared<internal::SkewnessMomental<T>>();
      break;
  }

  return skew;
}


//template<typename T> inline
//std::shared_ptr<Kurtosis<T>> KurtosisFactory<T>::create(KurtosisMethod method)
//{
//  std::shared_ptr<Kurtosis<T>> skew;
//
//  return skew;
//}


/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace math

} // End namespace tl


#endif TL_MATH_DESCRIPTIVE_STATISTIC_H

