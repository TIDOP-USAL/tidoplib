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

#include "tidop/rastertools/process/transform/affine.h"

namespace tl
{
/*!
* \brief Constructs an \c Affine object with a given transformation matrix.
* \param[in] affine The affine transformation matrix.
*/

AffineTransform::AffineTransform(const Affine<float, 2> &affine)
  : ImageProcess(ProcessType::affine), 
    mAffine(affine)
{
}

void AffineTransform::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    cv::Mat affMat(2, 3, CV_32FC1);

    affMat.at<float>(0, 0) = static_cast<float>(mAffine(0, 0));
    affMat.at<float>(0, 1) = static_cast<float>(mAffine(0, 1));
    affMat.at<float>(0, 2) = static_cast<float>(mAffine(0, 2));
    affMat.at<float>(1, 0) = static_cast<float>(mAffine(1, 0));
    affMat.at<float>(1, 1) = static_cast<float>(mAffine(1, 1));
    affMat.at<float>(1, 2) = static_cast<float>(mAffine(1, 2));

    cv::warpAffine(matIn, matOut, affMat, matIn.size(), cv::INTER_LINEAR);
}


} // End namespace tl
