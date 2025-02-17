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
#include "tidop/math/statistic/confmat.h"

namespace tl
{

/*! \addtogroup Curves
 *
 *  \{
 */

/*!
 * \class Curve
 * \brief Abstract base class for computing performance curves.
 *
 * This template class is designed to compute curves for performance evaluation,
 * such as ROC (Receiver Operating Characteristic) or Precision-Recall curves.
 * It processes a set of data points and evaluates performance metrics like the
 * Area Under the Curve (AUC).
 *
 * \tparam T Data type of the score values.
 */
template<typename T>
class Curve
{

protected:

    std::vector<std::pair<T, int>> mData;
    ConfusionMatrix<T> mConfusionMatrix;
    std::vector<Point<double>> mCurve;
    double mAuc;

public:

    /*!
     * \brief Constructor.
     * \param[in] data Vector of score-label pairs (T, int) used to compute the curve.
     *
     * The input data consists of pairs where the first element is the score and the second is
     * the ground truth label (1 for positive, 0 for negative). The data is sorted in ascending
     * order based on the score values.
     */
    Curve(const std::vector<std::pair<T, int>> &data);

    /*!
     * \brief Virtual destructor.
     */
    virtual ~Curve() = default;
    
    /*!
     * \brief Compute the evaluation curve with a given number of steps.
     * \param[in] steps Number of discrete points used to compute the curve.
     *
     * This function must be implemented in derived classes to generate specific types of curves.
     */
    virtual void compute(size_t steeps) = 0;

    /*!
     * \brief Compute the evaluation curve using an automatic step calculation.
     *
     * This function must be implemented in derived classes.
     */
    virtual void compute() = 0;

    /*!
     * \brief Get the computed curve points.
     * \return A vector of points representing the computed curve.
     */
    auto curve() const -> std::vector<Point<double>>;
    
    /*!
     * \brief Get the Area Under the Curve (AUC) value.
     * \return The computed AUC value.
     */
    auto auc() const -> double;

};



/* Curve class implementation */

template<typename T> 
Curve<T>::Curve(const std::vector<std::pair<T, int>> &data)
  : mData(data),
    mConfusionMatrix(data),
    mCurve(0),
    mAuc(0.0)
{
    std::sort(mData.begin(), mData.end(),
        [](const std::pair<T, int> &data1,
            const std::pair<T, int> &data2) {
                return data1.first < data2.first; });
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


/*! \} */

} // namespace tl
