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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_EVALUATION_H
#define TL_EVALUATION_H

#include "config_tl.h"

#include <map>

#include <opencv2/features2d.hpp>

#include "tidop/geometry/entities/point.h"


namespace tl
{

template<typename T>
class Curve
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

  Curve(const std::vector<std::pair<T, int>> &data);

  virtual ~Curve() = default;
  /*!
   * \brief Confussion matrix for the specified threshold
   * \param[in] threshold
   * \return Confussion Matrix
   */
  std::map<Classification, size_t> confusionMatrix(T threshold) const;

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

  /*!
   * \brief Compute curve
   * \param[in] steeps number of steeps
   */
  virtual void compute(size_t steeps) = 0;

  std::vector<PointF> curve() const;

  /*!
   * \brief auc
   * \return
   */
  double auc() const;

  static double truePositiveRate(size_t tp, size_t positives);
  static double falsePositiveRate(size_t fp, size_t negatives);
  static double trueNegativeRate(size_t tn, size_t negatives);
  static double falseNegativeRate(size_t fn, size_t positives);
  static double positivePredictiveValue(size_t tp, size_t fp);
  static double negativePredictiveValue(size_t fn, size_t tn);
  static double accuracy(size_t tp, size_t tn, size_t positives, size_t negatives);

protected:

  std::vector<std::pair<T, int>> mData;
  std::vector<PointF> mCurve;
  size_t mPositives;
  size_t mNegatives;
  double mAuc;

};



/*------------------------------------------------------------------------------------*/



template<typename T>
class PRCurve
  : public Curve<T>
{

public:

  PRCurve(const std::vector<std::pair<T, int>> &data);
  ~PRCurve() override = default;               

  void compute(size_t steeps = 100) override;
};



/*------------------------------------------------------------------------------------*/



template<typename T>
class ROCCurve
  : public Curve<T>
{

public:

  ROCCurve(const std::vector<std::pair<T, int>> &data);
  ~ROCCurve() override = default;                   

  void compute(size_t steeps = 100) override;

};



/*------------------------------------------------------------------------------------*/



template<typename T>
class DETCurve
  : public Curve<T>
{

public:

  explicit DETCurve(const std::vector<std::pair<T, int>> &data);
  ~DETCurve() override = default;
  void compute(size_t steeps = 100) override;

};



/*------------------------------------------------------------------------------------*/


/* Curve class implementation */

template<typename T> 
Curve<T>::Curve(const std::vector<std::pair<T, int>> &data)
  : mData(data),
    mCurve(0),
    mPositives(0),
    mNegatives(0),
    mAuc(0.0)
{
  std::sort(mData.begin(), mData.end(),
            [](const std::pair<T, int> &data1,
               const std::pair<T, int> &data2){
    return data1.first < data2.first;});

  for(size_t i = 0; i < mData.size(); i++) {
    if (mData[i].second == 1){
      mPositives++;
    } else {
      mNegatives++;
    }
  }

}

template<typename T> inline
std::map<typename Curve<T>::Classification,size_t> Curve<T>::confusionMatrix(T threshold) const
{

  size_t true_positives = 0;
  size_t false_positives = 0;
  size_t true_negatives = 0;
  size_t false_negatives = 0;
  
  for(size_t j = 0; j < mData.size(); j++ ) {

    if (mData[j].first <= threshold){
      if (mData[j].second == 1)
        true_positives++;
      else
        false_positives++;
    } else {
      if (mData[j].second == 0)
        true_negatives++;
      else
        false_negatives++;
    }
  }
  std::map<Classification,size_t> confussionMatrix;
  confussionMatrix[Classification::true_positives] = true_positives;
  confussionMatrix[Classification::false_positives] = false_positives;
  confussionMatrix[Classification::true_negatives] = true_negatives;
  confussionMatrix[Classification::false_negatives] = false_negatives;

  return confussionMatrix;
}

template<typename T> inline
size_t Curve<T>::positives() const
{
  return mPositives;
}

template<typename T> inline
size_t Curve<T>::negatives() const
{
  return mNegatives;
}

template<typename T> inline
size_t Curve<T>::truePositives(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  return confussionMatrix[Classification::true_positives];
}

template<typename T> inline
size_t Curve<T>::falsePositives(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  return confussionMatrix[Classification::false_positives];
}

template<typename T> inline
size_t Curve<T>::trueNegatives(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  return confussionMatrix[Classification::true_negatives];
}

template<typename T> inline
size_t Curve<T>::falseNegatives(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  return confussionMatrix[Classification::false_negatives];
}

template<typename T> inline
double Curve<T>::accuracy(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);

  return (mPositives + mNegatives) ?
    static_cast<double>(confussionMatrix[Classification::true_positives]+confussionMatrix[Classification::true_negatives])
      / static_cast<double>(mPositives + mNegatives) : -1;
}

template<typename T> inline
double Curve<T>::positivePredictiveValue(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  double tp = confussionMatrix[Classification::true_positives];
  double fp = confussionMatrix[Classification::false_positives];
  return ( (tp + fp) > 0.) ? tp / (tp + fp) : -1;
}

template<typename T> inline
double Curve<T>::negativePredictiveValue(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  double fn = confussionMatrix[Classification::false_negatives];
  double tn = confussionMatrix[Classification::true_negatives];
  return (fn + tn > 0.) ? tn / (fn + tn) : -1;
}

template<typename T> inline
double Curve<T>::truePositiveRate(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  double tp = confussionMatrix[Classification::true_positives];
  return mPositives ? tp / static_cast<double>(mPositives) : -1;
}

template<typename T> inline
double Curve<T>::falsePositiveRate(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  double fp = confussionMatrix[Classification::false_positives];
  return mNegatives ? static_cast<double>(fp) / static_cast<double>(mNegatives) : -1;
}

template<typename T> inline
double Curve<T>::trueNegativeRate(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  double tn = confussionMatrix[Classification::true_negatives];
  return mNegatives ? static_cast<double>(tn) / static_cast<double>(mNegatives) : -1;
}

template<typename T> inline
double Curve<T>::falseNegativeRate(T threshold) const
{
  std::map<Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
  double fn = confussionMatrix[Classification::false_negatives];
  return mPositives ? static_cast<double>(fn) / static_cast<double>(mPositives) : -1;
}

template<typename T>
std::vector<PointF> Curve<T>::curve() const
{
  return mCurve;
}

template<typename T>
double Curve<T>::auc() const
{
  return mAuc;
}

template<typename T> inline
double Curve<T>::truePositiveRate(size_t tp, size_t positives)
{
  return positives ? static_cast<double>(tp) / static_cast<double>(positives) : -1;
}

template<typename T> inline
double Curve<T>::falsePositiveRate(size_t fp, size_t negatives)
{
  return negatives ? static_cast<double>(fp) / static_cast<double>(negatives) : -1;
}

template<typename T> inline
double Curve<T>::trueNegativeRate(size_t tn, size_t negatives)
{
  return negatives ? static_cast<double>(tn) / static_cast<double>(negatives) : -1;
}

template<typename T> inline
double Curve<T>::falseNegativeRate(size_t fn, size_t positives)
{
  return positives ? static_cast<double>(fn) / static_cast<double>(positives) : -1;
}

template<typename T> inline
double Curve<T>::positivePredictiveValue(size_t tp, size_t fp)
{
  return ( (tp + fp) > static_cast<size_t>(0)) ? tp / (tp + fp) : -1;
}

template<typename T> inline
double Curve<T>::negativePredictiveValue(size_t fn, size_t tn)
{
  return (fn + tn > static_cast<size_t>(0)) ? tn / (fn + tn) : -1;
}

template<typename T> inline
double Curve<T>::accuracy(size_t tp, size_t tn, size_t positives, size_t negatives)
{

  return (positives + negatives) ?
    static_cast<double>(tp+tn)
      / static_cast<double>(positives + negatives) : -1;
}



/*------------------------------------------------------------------------------------*/


/* PR Curve class implementation */

template<typename T>
PRCurve<T>::PRCurve(const std::vector<std::pair<T, int>> &data)
  : Curve<T>(data)
{
}

template<typename T> inline
void PRCurve<T>::compute(size_t steeps)
{
  TL_TODO("para enteros habria que especializar la plantilla")

  if (this->mData.empty()) return;                 
  
  this->mCurve.resize(0);


  T min = this->mData.front().first;
  T max = this->mData.back().first;

  T step = (max - min) / static_cast<double>(steeps);

  T threshold = min;

  for (size_t i = 0; i < steeps; i++) {

    std::map<typename PRCurve<T>::Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);

    double recall = truePositiveRate(confussionMatrix[PRCurve<T>::Classification::true_positives], this->mPositives);
    double precision = positivePredictiveValue(confussionMatrix[PRCurve<T>::Classification::true_positives],
                                               confussionMatrix[PRCurve<T>::Classification::true_positives] + confussionMatrix[PRCurve<T>::Classification::false_positives]);
    this->mCurve.push_back(PointF(recall, 1-precision));
    threshold += step;
  }

  /// AUC
  size_t size = this->mCurve.size();
  this->mAuc = 0.0;

  if (size > 2) {
    PointF point1 = this->mCurve[0];
    PointF point2;

    for(size_t i = 1; i < size; i++){
      point2 = this->mCurve[i];
      this->mAuc += sqrt(pow((1 - point1.x + 1 - point2.x) / 2. * (point1.y - point2.y), 2));
      point1 = point2;
    }
  }

}



/*------------------------------------------------------------------------------------*/


/* ROC Curve class implementation */


template<typename T>
ROCCurve<T>::ROCCurve(const std::vector<std::pair<T, int>> &data)
  : Curve<T>(data)
{
}

template<typename T> inline
void ROCCurve<T>::compute(size_t steeps)
{
  TL_TODO("para enteros habria que especializar la plantilla")
  if (this->mData.empty()) return;

  this->mCurve.resize(0);
  T min = this->mData.front().first;
  T max = this->mData.back().first;

  T step = (max - min) / static_cast<double>(steeps);

  T threshold = min;
  for (size_t i = 0; i < steeps; i++) {

    std::map<typename PRCurve<T>::Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
double fpr = falsePositiveRate(confussionMatrix[PRCurve<T>::Classification::false_positives], this->mNegatives);
    double tpr = truePositiveRate(confussionMatrix[PRCurve<T>::Classification::true_positives], this->mPositives);
    this->mCurve.push_back(PointF(fpr, tpr));
    threshold += step;
  }

  /// AUC
  size_t size = this->mCurve.size();
  this->mAuc = 0.0;

  if (size > 2) {
    PointF point1 = this->mCurve[0];
    PointF point2;

    for(size_t i = 1; i < size; i++){
      point2 = this->mCurve[i];
      this->mAuc += sqrt(pow( (1-point1.x + 1-point2.x) / 2. * (point1.y - point2.y), 2));
      point1 = point2;
    }
  }
}

/*------------------------------------------------------------------------------------*/


/* DET Curve class implementation */

template<typename T>
DETCurve<T>::DETCurve(const std::vector<std::pair<T, int>> &data)
  : Curve<T>(data)
{
}

template<typename T> inline
void DETCurve<T>::compute(size_t steeps)
{
  TL_TODO("para enteros habria que especializar la plantilla")

  if (this->mData.empty()) return;   
  
  this->mCurve.resize(0);
  
  T min = this->mData.front().first;
  T max = this->mData.back().first;

  T step = (max - min) / static_cast<double>(steeps);

  T threshold = min;
  for (size_t i = 0; i < steeps; i++) {

    std::map<typename DETCurve<T>::Classification,size_t> confussionMatrix = this->confusionMatrix(threshold);
    double fpr = falsePositiveRate(confussionMatrix[DETCurve<T>::Classification::false_positives], this->mNegatives);
    double fnr = falseNegativeRate(confussionMatrix[DETCurve<T>::Classification::false_negatives], this->mPositives);
    this->mCurve.push_back(PointF(fpr, fnr));
    threshold += step;
  }

  /// AUC
  size_t size = this->mCurve.size();
  this->mAuc = 0.0;

  if (size > 2) {
    PointF point1 = this->mCurve[0];
    PointF point2;

    for(size_t i = 1; i < size; i++){
      point2 = this->mCurve[i];
      this->mAuc += sqrt(pow( (1-point1.x + 1-point2.x) / 2. * (point1.y - point2.y), 2));
      point1 = point2;
    }
  }
}


} // namespace tl


#endif // EVALUATION_H
