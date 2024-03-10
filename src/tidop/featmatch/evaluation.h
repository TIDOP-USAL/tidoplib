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

#include "tidop/config.h"

#include <map>

#include <opencv2/features2d.hpp>

#include "tidop/geometry/entities/point.h"

namespace tl
{

///TODO: No esta preparado para trabajar con enteros 
///TODO: ConfusionMatrix movido a statistic. Duplicado por ahora

template<typename T>
class Curve
{

protected:

    std::vector<std::pair<T, int>> mData;
    std::vector<Point<double>> mCurve;
    size_t mPositives;
    size_t mNegatives;
    double mAuc;

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
    auto confusionMatrix(T threshold) const -> std::map<Classification, size_t>;

    /*!
     * \brief positives
     * positives = TP + FN
     * \return
     */
    auto positives() const -> size_t;
    
    /*!
     * \brief negatives
     * negatives = FP + TN
     * \return
     */
    auto negatives() const -> size_t;
    
    /*!
     * \brief True Positives
     * \param[in] threshold
     * \return
     */
    auto truePositives(T threshold) const -> size_t;
    
    /*!
     * \brief False Positives
     * \param[in] threshold
     * \return
     */
    auto falsePositives(T threshold) const -> size_t;
    
    /*!
     * \brief True Negatives
     * \param[in] threshold
     * \return
     */
    auto trueNegatives(T threshold) const -> size_t;
    
    /*!
     * \brief False Negatives
     * \param[in] threshold
     * \return
     */
    auto falseNegatives(T threshold) const -> size_t;
    
    /*!
     * \brief accuracy
     * \param[in] threshold
     * \return
     */
    auto accuracy(T threshold) const -> double;
    
    /*!
     * \brief Precision or Positive Predictive Value
     * \param[in] threshold
     * \return
     */
    auto positivePredictiveValue(T threshold) const -> double;

    /*!
     * \brief Negative Predictive Value
     * \param[in] threshold
     * \return
     */
    auto negativePredictiveValue(T threshold) const -> double;
    
    /*!
     * \brief True Positive Rate, Recall or Sensitivity
     * \param[in] threshold
     * \return
     */
    auto truePositiveRate(T threshold) const -> double;
    
    /*!
     * \brief False Positive Rate or Fall-out
     * \param[in] threshold
     * \return
     */
    auto falsePositiveRate(T threshold) const -> double;
    
    /*!
     * \brief True Negative Rate or Specificity
     * \param[in] threshold
     * \return
     */
    auto trueNegativeRate(T threshold) const -> double;
    
    /*!
     * \brief False Negative Rate
     * \param[in] threshold
     * \return
     */    
    auto falseNegativeRate(T threshold) const -> double;
    
    /*!
     * \brief Compute curve
     * \param[in] steeps number of steeps
     */
    virtual void compute(size_t steeps) = 0;
    virtual void compute() = 0;
    
    auto curve() const -> std::vector<Point<double>>;
    
    /*!
     * \brief auc
     * \return
     */
    auto auc() const -> double;

    static auto truePositiveRate(size_t tp, size_t fn) -> double;
    static auto falsePositiveRate(size_t fp, size_t tn) -> double;
    static auto trueNegativeRate(size_t tn, size_t fp) -> double;
    static auto falseNegativeRate(size_t fn, size_t tp) -> double;
    static auto positivePredictiveValue(size_t tp, size_t fp) -> double;
    static auto negativePredictiveValue(size_t fn, size_t tn) -> double;
    static auto accuracy(size_t tp, size_t tn, size_t positives, size_t negatives) -> double;

};



/*------------------------------------------------------------------------------------*/



template<typename T>
class PRCurve
    : public Curve<T>
{

public:

    PRCurve(const std::vector<std::pair<T, int>> &data);
    ~PRCurve() override = default;

    void compute(size_t steeps) override;
    void compute() override;
};



/*------------------------------------------------------------------------------------*/



template<typename T>
class ROCCurve
  : public Curve<T>
{

public:

    ROCCurve(const std::vector<std::pair<T, int>> &data);
    ~ROCCurve() override = default;

    void compute(size_t steeps) override;
    void compute() override;
};



/*------------------------------------------------------------------------------------*/



template<typename T>
class DETCurve
  : public Curve<T>
{

public:

    explicit DETCurve(const std::vector<std::pair<T, int>> &data);
    ~DETCurve() override = default;

    void compute(size_t steeps) override;
    void compute() override;
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
              const std::pair<T, int> &data2) {
                  return data1.first > data2.first; }); // Ordenado al reves al utilizar la distancia de los matches

    for (size_t i = 0; i < mData.size(); i++) {
        if (mData[i].second == 1) {
            mPositives++;
        } else {
            mNegatives++;
        }
    }

}

template<typename T> 
auto Curve<T>::confusionMatrix(T threshold) const -> std::map<Curve<T>::Classification, size_t>
{
    size_t true_positives = 0;
    size_t false_positives = 0;
    size_t true_negatives = 0;
    size_t false_negatives = 0;

    for (size_t j = 0; j < mData.size(); j++) {

        if (mData[j].first > threshold) {
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

    std::map<Classification, size_t> confussion_matrix;
    confussion_matrix[Classification::true_positives] = true_positives;
    confussion_matrix[Classification::false_positives] = false_positives;
    confussion_matrix[Classification::true_negatives] = true_negatives;
    confussion_matrix[Classification::false_negatives] = false_negatives;

    return confussion_matrix;
}

template<typename T> 
auto Curve<T>::positives() const -> size_t
{
    return mPositives;
}

template<typename T>
auto Curve<T>::negatives() const -> size_t
{
    return mNegatives;
}

template<typename T>
auto Curve<T>::truePositives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    return confussionMatrix[Classification::true_positives];
}

template<typename T>
auto Curve<T>::falsePositives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    return confussionMatrix[Classification::false_positives];
}

template<typename T>
auto Curve<T>::trueNegatives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    return confussionMatrix[Classification::true_negatives];
}

template<typename T>
auto Curve<T>::falseNegatives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    return confussionMatrix[Classification::false_negatives];
}

template<typename T>
auto Curve<T>::accuracy(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);

    return (mPositives + mNegatives) ?
        static_cast<double>(confussionMatrix[Classification::true_positives] + confussionMatrix[Classification::true_negatives])
        / static_cast<double>(mPositives + mNegatives) : -1;
}

template<typename T> 
auto Curve<T>::positivePredictiveValue(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    double tp = static_cast<double>(confussionMatrix[Classification::true_positives]);
    double fp = static_cast<double>(confussionMatrix[Classification::false_positives]);
    return ((tp + fp) > 0.) ? tp / (tp + fp) : -1.;
}

template<typename T>
auto Curve<T>::negativePredictiveValue(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    double fn = static_cast<double>(confussionMatrix[Classification::false_negatives]);
    double tn = static_cast<double>(confussionMatrix[Classification::true_negatives]);
    return (fn + tn > 0.) ? tn / (fn + tn) : -1.;
}

template<typename T>
auto Curve<T>::truePositiveRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    size_t tp = confussionMatrix[Classification::true_positives];
    size_t fn = confussionMatrix[Classification::false_negatives];
    size_t positives = tp + fn;
    return positives > 0 ? tp / static_cast<double>(positives) : -1;
}

template<typename T>
auto Curve<T>::falsePositiveRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    size_t fp = confussionMatrix[Classification::false_positives];
    return mNegatives ? static_cast<double>(fp) / static_cast<double>(mNegatives) : -1;
}

template<typename T>
auto Curve<T>::trueNegativeRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    size_t tn = confussionMatrix[Classification::true_negatives];
    return mNegatives ? static_cast<double>(tn) / static_cast<double>(mNegatives) : -1;
}

template<typename T>
auto Curve<T>::falseNegativeRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->confusionMatrix(threshold);
    size_t fn = confussionMatrix[Classification::false_negatives];
    return mPositives ? static_cast<double>(fn) / static_cast<double>(mPositives) : -1;
}

template<typename T>
auto Curve<T>::curve() const -> std::vector<Point<double>>
{
    return mCurve;
}

template<typename T>
auto Curve<T>::auc() const -> double
{
    return mAuc;
}

template<typename T>
auto Curve<T>::truePositiveRate(size_t tp, size_t fn) -> double
{
    size_t positives = tp + fn;
    return positives > 0 ? static_cast<double>(tp) / static_cast<double>(positives) : -1;
}

template<typename T>
auto Curve<T>::falsePositiveRate(size_t fp, size_t tn) -> double
{
    size_t negatives = fp + tn;
    return negatives > 0 ? static_cast<double>(fp) / static_cast<double>(negatives) : -1;
}

template<typename T>
auto Curve<T>::trueNegativeRate(size_t tn, size_t fp) -> double
{
    size_t negatives = fp + tn;
    return negatives > 0. ? static_cast<double>(tn) / static_cast<double>(negatives) : -1;
}

template<typename T>
auto Curve<T>::falseNegativeRate(size_t fn, size_t tp) -> double
{
    size_t positives = fn + tp;
    return positives > 0 ? static_cast<double>(fn) / static_cast<double>(positives) : -1;
}

template<typename T>
auto Curve<T>::positivePredictiveValue(size_t tp, size_t fp) -> double
{
    return ((tp + fp) > static_cast<size_t>(0)) ? tp / static_cast<double>(tp + fp) : -1;
}

template<typename T>
auto Curve<T>::negativePredictiveValue(size_t fn, size_t tn) -> double
{
    return (fn + tn > static_cast<size_t>(0)) ? tn / static_cast<double>(fn + tn) : -1;
}

template<typename T>
auto Curve<T>::accuracy(size_t tp, size_t tn, size_t positives, size_t negatives) -> double
{

    return (positives + negatives) ?
        static_cast<double>(tp + tn)
        / static_cast<double>(positives + negatives) : -1;
}



/*------------------------------------------------------------------------------------*/


/* PR Curve class implementation */

template<typename T>
PRCurve<T>::PRCurve(const std::vector<std::pair<T, int>> &data)
  : Curve<T>(data)
{
}

template<typename T>
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

        auto confussion_matrix = this->confusionMatrix(threshold);

        double recall = this->truePositiveRate(confussion_matrix[PRCurve<T>::Classification::true_positives],
                                               confussion_matrix[PRCurve<T>::Classification::false_negatives]);
        double precision = this->positivePredictiveValue(confussion_matrix[PRCurve<T>::Classification::true_positives],
                                                         confussion_matrix[PRCurve<T>::Classification::false_positives]);
        this->mCurve.emplace_back(recall, precision);
        threshold += step;
    }

    /// AUC
    size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    if (size > 2) {
        Point<double> point1 = this->mCurve[0];
        Point<double> point2;

        for (size_t i = 1; i < size; i++) {
            point2 = this->mCurve[i];
            this->mAuc += std::abs(point1.x - point2.x) * (point1.y + point2.y) / 2.;
            point1 = point2;
        }
    }

}

template<typename T>
void PRCurve<T>::compute()
{
    if (this->mData.empty()) return;

    this->mCurve.resize(0);

    for (auto &data : this->mData) {

        auto confussionMatrix = this->confusionMatrix(data.first);

        double recall = this->truePositiveRate(confussionMatrix[PRCurve<T>::Classification::true_positives],
                                               confussionMatrix[PRCurve<T>::Classification::false_negatives]);
        double precision = this->positivePredictiveValue(confussionMatrix[PRCurve<T>::Classification::true_positives],
                                                         confussionMatrix[PRCurve<T>::Classification::false_positives]);
        this->mCurve.emplace_back(recall, precision);
    }

    /// AUC
    size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    if (size > 2) {
        Point<double> point1 = this->mCurve[0];
        Point<double> point2;

        for (size_t i = 1; i < size; i++) {
            point2 = this->mCurve[i];
            this->mAuc += std::abs(point1.x - point2.x) * (point1.y + point2.y) / 2.;
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

template<typename T>
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

        auto confussion_matrix = this->confusionMatrix(threshold);
        double fpr = this->falsePositiveRate(confussion_matrix[ROCCurve<T>::Classification::false_positives],
                                             confussion_matrix[ROCCurve<T>::Classification::true_negatives]);
        double tpr = this->truePositiveRate(confussion_matrix[ROCCurve<T>::Classification::true_positives],
                                            confussion_matrix[ROCCurve<T>::Classification::false_negatives]);
        this->mCurve.emplace_back(fpr, tpr);
        threshold += step;
    }

    /// AUC
    size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    if (size > 2) {
        Point<double> point1 = this->mCurve[0];
        Point<double> point2;

        for (size_t i = 1; i < size; i++) {
            point2 = this->mCurve[i];
            this->mAuc += std::abs(point1.x - point2.x) * (point1.y + point2.y) / 2.;
            point1 = point2;
        }

    }
}

template<typename T>
void ROCCurve<T>::compute()
{
    if (this->mData.empty()) return;

    this->mCurve.resize(0);

    for (auto &data : this->mData) {

        auto confussion_matrix = this->confusionMatrix(data.first);
        double fpr = this->falsePositiveRate(confussion_matrix[ROCCurve<T>::Classification::false_positives],
                                             confussion_matrix[ROCCurve<T>::Classification::true_negatives]);
        double tpr = this->truePositiveRate(confussion_matrix[ROCCurve<T>::Classification::true_positives],
                                            confussion_matrix[ROCCurve<T>::Classification::false_negatives]);
        this->mCurve.emplace_back(fpr, tpr);
    }

    /// AUC
    size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    if (size > 2) {
        Point<double> point1 = this->mCurve[0];
        Point<double> point2;

        for (size_t i = 1; i < size; i++) {
            point2 = this->mCurve[i];
            this->mAuc += std::abs(point1.x - point2.x) * (point1.y + point2.y) / 2.;
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

template<typename T>
void DETCurve<T>::compute(size_t steeps)
{
    TL_TODO("para enteros habría que especializar la plantilla")

    if (this->mData.empty()) return;

    this->mCurve.resize(0);

    T min = this->mData.front().first;
    T max = this->mData.back().first;

    T step = (max - min) / static_cast<double>(steeps);

    T threshold = min;
    for (size_t i = 0; i < steeps; i++) {

        auto confussion_matrix = this->confusionMatrix(threshold);
        double fpr = this->falsePositiveRate(confussion_matrix[DETCurve<T>::Classification::false_positives],
                                             confussion_matrix[DETCurve<T>::Classification::true_negatives]);
        double fnr = this->falseNegativeRate(confussion_matrix[DETCurve<T>::Classification::false_negatives],
                                             confussion_matrix[DETCurve<T>::Classification::true_positives]);
        this->mCurve.emplace_back(fpr, fnr);
        threshold += step;
    }

    /// AUC
    //size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    //if (size > 2) {
    //  Point<double> point1 = this->mCurve[0];
    //  Point<double> point2;

    //  for(size_t i = 1; i < size; i++){
    //    point2 = this->mCurve[i];
    //    this->mAuc += std::abs((1 - point1.x + 1 - point2.x) / 2. * (point1.y - point2.y));
    //    point1 = point2;
    //  }
    //}
}

template<typename T>
void DETCurve<T>::compute()
{
    if (this->mData.empty()) return;

    this->mCurve.resize(0);

    for (auto &data : this->mData) {
        auto confussion_matrix = this->confusionMatrix(data.first);
        double fpr = this->falsePositiveRate(confussion_matrix[DETCurve<T>::Classification::false_positives],
                                             confussion_matrix[DETCurve<T>::Classification::true_negatives]);
        double fnr = this->falseNegativeRate(confussion_matrix[DETCurve<T>::Classification::false_negatives],
                                             confussion_matrix[DETCurve<T>::Classification::true_positives]);
        this->mCurve.emplace_back(fpr, fnr);
    }

    /// AUC
    //size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    //if (size > 2) {
    //  Point<double> point1 = this->mCurve[0];
    //  Point<double> point2;

    //  for (size_t i = 1; i < size; i++) {
    //    point2 = this->mCurve[i];
    //    this->mAuc += std::abs((1 - point1.x + 1 - point2.x) / 2. * (point1.y - point2.y));
    //    point1 = point2;
    //  }
    //}
}

} // namespace tl
