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

#ifndef TL_MATH_STATISTIC_CONFUSSION_MATRIX_H
#define TL_MATH_STATISTIC_CONFUSSION_MATRIX_H

#include <tidop/core/defs.h>
#include <tidop/core/messages.h>

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
class ConfusionMatrix
{

public:

  enum class Classification
  {
    true_positives,
    false_positives,
    true_negatives,
    false_negatives
  };

public:

  ConfusionMatrix(const std::vector<std::pair<T, int>> &data);
  virtual ~ConfusionMatrix() = default;

  /*!
   * \brief positives
   * positives = TP + FN
   * \return
   */
  size_t positives() const;

  /*!
   * \brief negatives
   * negatives = FP + TN
   * \return
   */
  size_t negatives() const;

  /*!
   * \brief True Positives
   * \param[in] threshold
   * \return
   */
  size_t truePositives(T threshold) const;

  /*!
   * \brief False Positives
   * \param[in] threshold
   * \return
   */
  size_t falsePositives(T threshold) const;

  /*!
   * \brief True Negatives
   * \param[in] threshold
   * \return
   */
  size_t trueNegatives(T threshold) const;

  /*!
   * \brief False Negatives
   * \param[in] threshold
   * \return
   */
  size_t falseNegatives(T threshold) const;

  /*!
   * \brief accuracy
   * \param[in] threshold
   * \return
   */
  double accuracy(T threshold) const;

  /*!
   * \brief Precision or Positive Predictive Value
   * \param[in] threshold
   * \return
   */
  double positivePredictiveValue(T threshold) const;

  /*!
   * \brief Negative Predictive Value
   * \param[in] threshold
   * \return
   */
  double negativePredictiveValue(T threshold) const;

  /*!
   * \brief True Positive Rate, Recall or Sensitivity
   * \param[in] threshold
   * \return
   */
  double truePositiveRate(T threshold) const;

  /*!
   * \brief False Positive Rate or Fall-out
   * \param[in] threshold
   * \return
   */
  double falsePositiveRate(T threshold) const;

  /*!
   * \brief True Negative Rate or Specificity
   * \param[in] threshold
   * \return
   */
  double trueNegativeRate(T threshold) const;

  /*!
   * \brief False Negative Rate
   * \param[in] threshold
   * \return
   */
  double falseNegativeRate(T threshold) const;

  static double truePositiveRate(size_t tp, size_t fn);
  static double falsePositiveRate(size_t fp, size_t tn);
  static double trueNegativeRate(size_t tn, size_t fp);
  static double falseNegativeRate(size_t fn, size_t tp);
  static double positivePredictiveValue(size_t tp, size_t fp);
  static double negativePredictiveValue(size_t fn, size_t tn);
  static double accuracy(size_t tp, size_t tn, size_t positives, size_t negatives);

private:

  /*!
   * \brief Confussion matrix for the specified threshold
   * \param[in] threshold
   * \return Confussion Matrix
   */
  std::map<Classification, size_t> compute(T threshold) const;

private:

  std::vector<std::pair<T, int>> mData;
  size_t mPositives;
  size_t mNegatives;

};


template<typename T>
ConfusionMatrix<T>::ConfusionMatrix(const std::vector<std::pair<T, int>> &data)
  : mData(data),
    mPositives(0),
    mNegatives(0)
{
  std::sort(mData.begin(), mData.end(),
            [](const std::pair<T, int> &data1,
               const std::pair<T, int> &data2) {
              return data1.first < data2.first; });

  for (const auto &data : mData) {
    if (data.second == 1) {
      mPositives++;
    } else {
      mNegatives++;
    }
  }

}

template<typename T> inline
size_t ConfusionMatrix<T>::positives() const
{
  return mPositives;
}

template<typename T> inline
size_t ConfusionMatrix<T>::negatives() const
{
  return mNegatives;
}

template<typename T> inline
size_t ConfusionMatrix<T>::truePositives(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  return confussionMatrix[Classification::true_positives];
}

template<typename T> inline
size_t ConfusionMatrix<T>::falsePositives(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  return confussionMatrix[Classification::false_positives];
}

template<typename T> inline
size_t ConfusionMatrix<T>::trueNegatives(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  return confussionMatrix[Classification::true_negatives];
}

template<typename T> inline
size_t ConfusionMatrix<T>::falseNegatives(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  return confussionMatrix[Classification::false_negatives];
}

template<typename T> inline
double ConfusionMatrix<T>::accuracy(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);

  return (mPositives + mNegatives) > consts::zero<size_t> ?
    static_cast<double>(confussionMatrix[Classification::true_positives] + confussionMatrix[Classification::true_negatives])
    / static_cast<double>(mPositives + mNegatives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::positivePredictiveValue(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  double tp = static_cast<double>(confussionMatrix[Classification::true_positives]);
  double fp = static_cast<double>(confussionMatrix[Classification::false_positives]);
  return ((tp + fp) > consts::zero<double>) ? tp / (tp + fp) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::negativePredictiveValue(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  double fn = static_cast<double>(confussionMatrix[Classification::false_negatives]);
  double tn = static_cast<double>(confussionMatrix[Classification::true_negatives]);
  return (fn + tn > consts::zero<double>) ? tn / (fn + tn) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::truePositiveRate(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  double tp = static_cast<double>(confussionMatrix[Classification::true_positives]);
  return mPositives > consts::zero<double> ? tp / static_cast<double>(mPositives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::falsePositiveRate(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  double fp = static_cast<double>(confussionMatrix[Classification::false_positives]);
  return mNegatives > consts::zero<double> ? static_cast<double>(fp) / static_cast<double>(mNegatives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::trueNegativeRate(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  double tn = static_cast<double>(confussionMatrix[Classification::true_negatives]);
  return mNegatives > consts::zero<double> ? static_cast<double>(tn) / static_cast<double>(mNegatives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::falseNegativeRate(T threshold) const
{
  std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
  double fn = static_cast<double>(confussionMatrix[Classification::false_negatives]);
  return mPositives > consts::zero<double> ? static_cast<double>(fn) / static_cast<double>(mPositives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::truePositiveRate(size_t tp, size_t fn)
{
  size_t positives = tp + fn;
  return positives > consts::zero<size_t> ? static_cast<double>(tp) / static_cast<double>(positives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::falsePositiveRate(size_t fp, size_t tn)
{
  size_t negatives = fp + tn;
  return negatives > consts::zero<size_t> ? static_cast<double>(fp) / static_cast<double>(negatives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::trueNegativeRate(size_t tn, size_t fp)
{
  size_t negatives = fp + tn;
  return negatives > consts::zero<size_t> ? static_cast<double>(tn) / static_cast<double>(negatives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::falseNegativeRate(size_t fn, size_t tp)
{
  size_t positives = fn + tp;
  return positives > consts::zero<size_t> ? static_cast<double>(fn) / static_cast<double>(positives) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::positivePredictiveValue(size_t tp, size_t fp)
{
  return ( (tp + fp) > consts::zero<size_t>) ? tp / static_cast<double>(tp + fp) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::negativePredictiveValue(size_t fn, size_t tn)
{
  return (fn + tn > consts::zero<size_t>) ? tn / static_cast<double>(fn + tn) : -consts::one<double>;
}

template<typename T> inline
double ConfusionMatrix<T>::accuracy(size_t tp, size_t tn, size_t positives, size_t negatives)
{
  return (positives + negatives) > consts::zero<size_t> ?
    static_cast<double>(tp+tn) / static_cast<double>(positives + negatives) :
    -consts::one<double>;
}

template<typename T> inline
std::map<typename ConfusionMatrix<T>::Classification, size_t> ConfusionMatrix<T>::compute(T threshold) const
{
  size_t true_positives = 0;
  size_t false_positives = 0;
  size_t true_negatives = 0;
  size_t false_negatives = 0;

  for (size_t j = 0; j < mData.size(); j++) {

    if (mData[j].first < threshold) {
      if (mData[j].second == 1)
        false_negatives++;
      else
        true_negatives++;
    } else {
      if (mData[j].second == 0)
        false_positives++;
      else
        true_positives++;
    }

  }

  std::map<Classification, size_t> confussionMatrix;
  confussionMatrix[Classification::true_positives] = true_positives;
  confussionMatrix[Classification::false_positives] = false_positives;
  confussionMatrix[Classification::true_negatives] = true_negatives;
  confussionMatrix[Classification::false_negatives] = false_negatives;

  return confussionMatrix;
}

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace math

} // End namespace tl

#endif TL_MATH_STATISTIC_CONFUSSION_MATRIX_H


