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


/*! \defgroup statistics Statistics
 *  \{
 */


template<typename T>
class DescriptiveStatistics
{
public:

  enum
  {
    sample,
    population
  };

public:

  DescriptiveStatistics() = default;
  DescriptiveStatistics(Series<T> data);
  ~DescriptiveStatistics();

  /*!
   * \brief min
   * \return
   */
  T min();

  /*!
   * \brief max
   * \return
   */
  T max();

  /*!
   * \brief Summation
   * \return
   */
  T sum();

  /*!
   * \brief mean
   * \return
   */
  double mean();

  /*!
   * \brief median
   * \return
   */
  T median();

  /*!
   * \brief variance
   * \return
   */
  double variance();

  /*!
   * \brief standarDeviation
   * \return
   */
  double standarDeviation();

  /*!
   * \brief mode
   * \return
   */
  double mode();

  /*!
   * \brief range
   * \return
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
   * \return
   */
  double interquartileRange();

  /*!
   * \brief Mean Absolute Deviation
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
   * \return
   */
  double sumOfSquares();

  /*!
   * \brief Root Mean Square
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

  /*!
   * \brief Coefficient of Variation
   * \return
   */
  double coefficientOfVariation();

  size_t size() const;

private:

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
    min = (0 << 0),
    max = (1 << 0),
    mean = (1 << 1),
    median = (1 << 2),
    mode = (1 << 3),
    range = (1 << 4),
    first_quartile = (1 << 5),
    second_quartile = (1 << 6),
    third_quartile = (1 << 7),
    sum_of_squares = (1 << 8),
    rms = (1 << 9),
    variance = (1 << 10),
    standar_deviation = (1 << 11)
  };

  tl::EnumFlags<InternalStatus> mStatus;
  Series<T> mData;
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
DescriptiveStatistics<T>::DescriptiveStatistics(Series<T> data)
  : mData(std::move(data))
{}

template<typename T>
DescriptiveStatistics<T>::~DescriptiveStatistics()
{}

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

  for (const auto &data : mData) {
    summation += data;
  }

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

  for (const auto data : mData) {
    *x_it++ = std::abs(static_cast<double>(data) - _median);
  }

  return ::median(x.begin(), x.end());
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
  size_t n = size();

  if (n <= 1) return consts::zero<double>;

  double _mean = mean();
  double dif{};
  double skew{};

  for (const auto data : mData) {
    dif = static_cast<double>(data) - _mean;
    skew += dif * dif * dif;
  }

  double _variance = variance();

  if (_variance != consts::zero<double>)
    return skew / (n * _variance * standarDeviation());
  else
    return consts::zero<double>;
}

template<typename T> inline
double DescriptiveStatistics<T>::kurtosis()
{
  return 0.0;
}

template<typename T> inline
double DescriptiveStatistics<T>::coefficientOfVariation()
{
  return standarDeviation() / std::abs(mean());
}

template<typename T> inline
size_t DescriptiveStatistics<T>::size() const
{
  return mData.size();
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

  mVariance = (sum - ep * ep / n) / (n - 1);

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

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace math

} // End namespace tl


#endif TL_MATH_DESCRIPTIVE_STATISTIC_H

