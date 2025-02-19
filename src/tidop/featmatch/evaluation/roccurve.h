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
#include "tidop/featmatch/evaluation/curve.h"

namespace tl
{

/*! \addtogroup Curves
 *
 *  \{
 */

/*!
 * \class ROCCurve
 * \brief Receiver Operating Characteristic (ROC) curve.
 *
 * The ROC curve is a widely used evaluation metric that plots the true positive rate (TPR) 
 * against the false positive rate (FPR) at various threshold settings. It is useful for 
 * assessing the trade-off between sensitivity and specificity.
 *
 * \tparam T The type of the score values used in the curve computation.
 */
template<typename T>
class ROCCurve
  : public Curve<T>
{

public:

    /*!
     * \brief Constructor.
     * \param[in] data Vector of score-label pairs (T, int) used to compute the ROC curve.
     */
    ROCCurve(const std::vector<std::pair<T, int>> &data);

    /*!
     * \brief Virtual destructor.
     */
    ~ROCCurve() override = default;

    /*!
     * \brief Compute the ROC curve with a given number of steps.
     * \param[in] steps Number of discrete points used to compute the curve.
     */
    void compute(size_t steps) override;

    /*!
     * \brief Compute the ROC curve using an automatic step calculation.
     */
    void compute() override;
};



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
        double fpr = this->mConfusionMatrix.falsePositiveRate(threshold);
        double tpr = this->mConfusionMatrix.truePositiveRate(threshold);

        this->mCurve.emplace_back(fpr, tpr);
        threshold += step;
    }

    /// AUC
    size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    if (size > 1) {

        if (this->mCurve.front().x > this->mCurve.back().x)
            std::reverse(this->mCurve.begin(), this->mCurve.end());

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
        double fpr = this->mConfusionMatrix.falsePositiveRate(data.first);
        double tpr = this->mConfusionMatrix.truePositiveRate(data.first);
        this->mCurve.emplace_back(fpr, tpr);
    }

    /// AUC
    size_t size = this->mCurve.size();
    this->mAuc = 0.0;

    if (size > 1) {

        if (this->mCurve.front().x > this->mCurve.back().x)
            std::reverse(this->mCurve.begin(), this->mCurve.end());

        Point<double> point1 = this->mCurve[0];
        Point<double> point2;

        for (size_t i = 1; i < size; i++) {
            point2 = this->mCurve[i];
            this->mAuc += std::abs(point1.x - point2.x) * (point1.y + point2.y) / 2.;
            point1 = point2;
        }

    }
}

/*! \} */

} // namespace tl
