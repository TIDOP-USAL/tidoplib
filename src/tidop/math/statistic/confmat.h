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


#include "tidop/core/base/defs.h"


namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \brief Confusion Matrix for classification problems
 * 
 * The `ConfusionMatrix` class provides methods to calculate various metrics related to the 
 * performance of a classification model, such as True Positives, False Positives, Accuracy, 
 * Precision, Recall, and more, based on a dataset of predicted and actual labels.
 * It also supports threshold-based classification, where you can adjust the threshold to
 * evaluate the performance at different points.
 * 
 * \tparam T The type of the predicted labels (e.g., `double`, `int`).
 * 
 * ### Example Usage
 * \code{.cpp}
 * 
 * #include <iostream>
 * #include <vector>
 * #include <tidop/math/statistic/confmat.h>
 * 
 * int main() {
 *     std::vector<std::pair<double, int>> data = {
 *         {0.9, 1}, {0.7, 1}, {0.6, 0}, {0.8, 1}, {0.4, 0}, {0.3, 0}, {0.5, 1}
 *     };
 * 
 *     ConfusionMatrix<double> cm(data);
 *     double threshold = 0.5;
 * 
 *     Message::info("Accuracy: {}", cm.accuracy(threshold));
 *     Message::info("Precision: {}", cm.positivePredictiveValue(threshold));
 *     Message::info("Recall: {}", cm.truePositiveRate(threshold));
 *     Message::info("Specificity: {}", cm.trueNegativeRate(threshold));
 * 
 *     return 0;
 * }
 * 
 * \endcode
 */
template<typename T>
class ConfusionMatrix
{

public:

    /*!
     * \brief Enumeration for confusion matrix elements
     *
     * Defines the four key elements of a confusion matrix:
     * - True Positives (TP)
     * - False Positives (FP)
     * - True Negatives (TN)
     * - False Negatives (FN)
     */
    enum class Classification
    {
        true_positives,  /*!< True positives */
        false_positives, /*!< False positives */
        true_negatives,  /*!< True negatives */
        false_negatives  /*!< False negatives */
    };

private:

    std::vector<std::pair<T, int>> mData;
    size_t mPositives;
    size_t mNegatives;

public:

    /*!
     * \brief Constructor to initialize the confusion matrix with data
     *
     * \param[in] data A vector of pairs where each pair contains the predicted label and the
     *                 actual label.
     */
    explicit ConfusionMatrix(const std::vector<std::pair<T, int>> &data);

    virtual ~ConfusionMatrix() = default;

    /*!
     * \brief Computes the total number of positive instances (TP + FN)
     * \return The number of positive instances
     */
    auto positives() const -> size_t;

    /*!
     * \brief Computes the total number of negative instances (FP + TN)
     * \return The number of negative instances
     */
    auto negatives() const -> size_t;

    /*!
     * \brief Computes the number of True Positives (TP) based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The number of True Positives
     */
    auto truePositives(T threshold) const -> size_t;

    /*!
     * \brief Computes the number of False Positives (FP) based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The number of False Positives
     */
    auto falsePositives(T threshold) const -> size_t;

    /*!
     * \brief Computes the number of True Negatives (TN) based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The number of True Negatives
     */
    auto trueNegatives(T threshold) const -> size_t;

    /*!
     * \brief Computes the number of False Negatives (FN) based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The number of False Negatives
     */
    auto falseNegatives(T threshold) const -> size_t;

    /*!
     * \brief Computes the accuracy of the classification model based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The accuracy of the model
     */
    auto accuracy(T threshold) const -> double;

    /*!
     * \brief Computes the Precision (Positive Predictive Value) of the classification model
     *        based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The Precision of the model
     */
    auto positivePredictiveValue(T threshold) const -> double;

    /*!
     * \brief Computes the Negative Predictive Value (NPV) of the classification model
     *        based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The Negative Predictive Value
     */
    auto negativePredictiveValue(T threshold) const -> double;

    /*!
     * \brief Computes the True Positive Rate (Recall or Sensitivity) based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The True Positive Rate
     */
    auto truePositiveRate(T threshold) const -> double;

    /*!
     * \brief Computes the False Positive Rate (Fall-out) based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The False Positive Rate
     */
    auto falsePositiveRate(T threshold) const -> double;

    /*!
     * \brief Computes the True Negative Rate (Specificity) based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The True Negative Rate
     */
    auto trueNegativeRate(T threshold) const -> double;

    /*!
     * \brief Computes the False Negative Rate based on the given threshold
     * \param[in] threshold The threshold for classification
     * \return The False Negative Rate
     */
    auto falseNegativeRate(T threshold) const -> double;

    /*!
     * \brief Static method to compute True Positive Rate
     * \param[in] tp The number of true positives
     * \param[in] fn The number of false negatives
     * \return The True Positive Rate
     */
    static auto truePositiveRate(size_t tp, size_t fn) -> double;

    /*!
     * \brief Static method to compute False Positive Rate
     * \param[in] fp The number of false positives
     * \param[in] tn The number of true negatives
     * \return The False Positive Rate
     */
    static auto falsePositiveRate(size_t fp, size_t tn) -> double;

    /*!
     * \brief Static method to compute True Negative Rate
     * \param[in] tn The number of true negatives
     * \param[in] fp The number of false positives
     * \return The True Negative Rate
     */
    static auto trueNegativeRate(size_t tn, size_t fp) -> double;

    /*!
     * \brief Static method to compute False Negative Rate
     * \param[in] fn The number of false negatives
     * \param[in] tp The number of true positives
     * \return The False Negative Rate
     */
    static auto falseNegativeRate(size_t fn, size_t tp) -> double;

    /*!
     * \brief Static method to compute Precision (Positive Predictive Value)
     * \param[in] tp The number of true positives
     * \param[in] fp The number of false positives
     * \return The Precision
     */
    static auto positivePredictiveValue(size_t tp, size_t fp) -> double;

    /*!
     * \brief Static method to compute Negative Predictive Value
     * \param[in] fn The number of false negatives
     * \param[in] tn The number of true negatives
     * \return The Negative Predictive Value
     */
    static auto negativePredictiveValue(size_t fn, size_t tn) -> double;

    /*!
     * \brief Static method to compute Accuracy
     * \param[in] tp The number of true positives
     * \param[in] tn The number of true negatives
     * \param[in] positives The number of positive instances
     * \param[in] negatives The number of negative instances
     * \return The accuracy
     */
    static auto accuracy(size_t tp, size_t tn, size_t positives, size_t negatives) -> double;

private:

    /*!
     * \brief Confussion matrix for the specified threshold
     * \param[in] threshold
     * \return Confussion Matrix
     */
    auto compute(T threshold) const -> std::map<Classification, size_t>;

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

    for (const auto &pair : mData) {
        if (pair.second == 1) {
            mPositives++;
        } else {
            mNegatives++;
        }
    }

}

template<typename T>
auto ConfusionMatrix<T>::positives() const -> size_t
{
    return mPositives;
}

template<typename T>
auto ConfusionMatrix<T>::negatives() const -> size_t
{
    return mNegatives;
}

template<typename T>
auto ConfusionMatrix<T>::truePositives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    return confussionMatrix[Classification::true_positives];
}

template<typename T>
auto ConfusionMatrix<T>::falsePositives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    return confussionMatrix[Classification::false_positives];
}

template<typename T>
auto ConfusionMatrix<T>::trueNegatives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    return confussionMatrix[Classification::true_negatives];
}

template<typename T>
auto ConfusionMatrix<T>::falseNegatives(T threshold) const -> size_t
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    return confussionMatrix[Classification::false_negatives];
}

template<typename T>
auto ConfusionMatrix<T>::accuracy(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);

    return (mPositives + mNegatives) > consts::zero<size_t> ?
        static_cast<double>(confussionMatrix[Classification::true_positives] + confussionMatrix[Classification::true_negatives])
        / static_cast<double>(mPositives + mNegatives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::positivePredictiveValue(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    double tp = static_cast<double>(confussionMatrix[Classification::true_positives]);
    double fp = static_cast<double>(confussionMatrix[Classification::false_positives]);
    return ((tp + fp) > consts::zero<double>) ? tp / (tp + fp) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::negativePredictiveValue(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    double fn = static_cast<double>(confussionMatrix[Classification::false_negatives]);
    double tn = static_cast<double>(confussionMatrix[Classification::true_negatives]);
    return (fn + tn > consts::zero<double>) ? tn / (fn + tn) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::truePositiveRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    double tp = static_cast<double>(confussionMatrix[Classification::true_positives]);
    return mPositives > consts::zero<size_t> ? tp / static_cast<double>(mPositives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::falsePositiveRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    double fp = static_cast<double>(confussionMatrix[Classification::false_positives]);
    return mNegatives > consts::zero<size_t> ? static_cast<double>(fp) / static_cast<double>(mNegatives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::trueNegativeRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    double tn = static_cast<double>(confussionMatrix[Classification::true_negatives]);
    return mNegatives > consts::zero<size_t> ? static_cast<double>(tn) / static_cast<double>(mNegatives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::falseNegativeRate(T threshold) const -> double
{
    std::map<Classification, size_t> confussionMatrix = this->compute(threshold);
    double fn = static_cast<double>(confussionMatrix[Classification::false_negatives]);
    return mPositives > consts::zero<size_t> ? static_cast<double>(fn) / static_cast<double>(mPositives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::truePositiveRate(size_t tp, size_t fn) -> double
{
    size_t positives = tp + fn;
    return positives > consts::zero<size_t> ? static_cast<double>(tp) / static_cast<double>(positives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::falsePositiveRate(size_t fp, size_t tn) -> double
{
    size_t negatives = fp + tn;
    return negatives > consts::zero<size_t> ? static_cast<double>(fp) / static_cast<double>(negatives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::trueNegativeRate(size_t tn, size_t fp) -> double
{
    size_t negatives = fp + tn;
    return negatives > consts::zero<size_t> ? static_cast<double>(tn) / static_cast<double>(negatives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::falseNegativeRate(size_t fn, size_t tp) -> double
{
    size_t positives = fn + tp;
    return positives > consts::zero<size_t> ? static_cast<double>(fn) / static_cast<double>(positives) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::positivePredictiveValue(size_t tp, size_t fp) -> double
{
    return ((tp + fp) > consts::zero<size_t>) ? static_cast<double>(tp) / static_cast<double>(tp + fp) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::negativePredictiveValue(size_t fn, size_t tn) -> double
{
    return (fn + tn > consts::zero<size_t>) ? static_cast<double>(tn) / static_cast<double>(fn + tn) : -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::accuracy(size_t tp, size_t tn, size_t positives, size_t negatives) -> double
{
    return (positives + negatives) > consts::zero<size_t> ?
        static_cast<double>(tp + tn) / static_cast<double>(positives + negatives) :
        -consts::one<double>;
}

template<typename T>
auto ConfusionMatrix<T>::compute(T threshold) const -> std::map<Classification, size_t>
{
    size_t true_positives = 0;
    size_t false_positives = 0;
    size_t true_negatives = 0;
    size_t false_negatives = 0;

#if CPP_VERSION >= 17
    for (const auto &[value, tag] : mData) {
#else
    for (const auto &pair : mData) {
        auto value = pair.first;
        auto tag = pair.second;
#endif

        if (value < threshold) {
            if (tag == 1)
                false_negatives++;
            else
                true_negatives++;
        } else {
            if (tag == 0)
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


/*! \} */

} // End namespace tl



