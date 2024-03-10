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

#include "tidop/imgprocess/filter/bilateral.h"
#include "tidop/core/exception.h"

#ifdef TL_HAVE_OPENCV

#include <opencv2/imgproc.hpp>

namespace tl
{


/* Bilateral filter */

BilateralFilter::BilateralFilter(int diameter, double sigmaColor, double sigmaSpace, int borderType)
  : ImageProcess(ProcessType::bilateral),
    mDiameter(diameter),
    mSigmaColor(sigmaColor),
    mSigmaSpace(sigmaSpace),
    mBorderType(borderType)
{
}

void BilateralFilter::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        cv::Mat mat_aux = cv::Mat::zeros(matIn.size(), CV_8UC1);
        cv::bilateralFilter(matIn, mat_aux, mDiameter, mSigmaColor, mSigmaSpace, mBorderType);
        mat_aux.copyTo(matOut);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

void BilateralFilter::setParameters(int diameter,
                                    double sigmaColor,
                                    double sigmaSpace,
                                    int borderType)
{
    mDiameter = diameter;
    mSigmaColor = sigmaColor;
    mSigmaSpace = sigmaSpace;
    mBorderType = borderType;
}

} // End namespace tl

#endif // TL_HAVE_OPENCV
