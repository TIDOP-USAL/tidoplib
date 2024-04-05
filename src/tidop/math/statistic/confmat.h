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


#include <tidop/core/defs.h>


namespace tl
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

private:

    std::vector<std::pair<T, int>> mData;
    size_t mPositives;
    size_t mNegatives;

public:

    explicit ConfusionMatrix(const std::vector<std::pair<T, int>> &data);
    virtual ~ConfusionMatrix() = default;

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

    static auto truePositiveRate(size_t tp, size_t fn) -> double;
    static auto falsePositiveRate(size_t fp, size_t tn) -> double;
    static auto trueNegativeRate(size_t tn, size_t fp) -> double;
    static auto falseNegativeRate(size_t fn, size_t tp) -> double;
    static auto positivePredictiveValue(size_t tp, size_t fp) -> double;
    static auto negativePredictiveValue(size_t fn, size_t tn) -> double;
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



/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl



