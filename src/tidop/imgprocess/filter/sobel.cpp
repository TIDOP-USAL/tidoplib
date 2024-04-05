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

#include "tidop/imgprocess/filter/sobel.h"
#include "tidop/core/exception.h"

#ifdef TL_HAVE_OPENCV

#include <opencv2/imgproc.hpp>

namespace tl
{


Sobel::Sobel(int dx,
             int dy,
             int ksize,
             double scale,
             double delta,
             int ddepth,
             double thresh,
             double maxval,
             int bordertype)
  : ImageProcess(ProcessType::sobel),
    mDx(dx),
    mDy(dy),
    mKernelSize(ksize),
    mScale(scale),
    mDelta(delta),
    mDepth(ddepth),
    mThresh(thresh),
    mMaxVal(maxval),
    mBorderType(bordertype)
{
}

void Sobel::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;

        cv::Sobel(matIn, grad_x, mDepth, mDx, mDy, mKernelSize, mScale, mDelta, mBorderType);

        TL_TODO("No tiene mucho sentido añadir esto dentro del filtro Sobel")
        convertScaleAbs(grad_x, abs_grad_x);
        threshold(abs_grad_x, matOut, mThresh, mMaxVal, cv::THRESH_BINARY);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

}

void Sobel::setParameters(int dx, int dy, int ksize, double scale, double delta, int ddepth, double thresh, double maxval, int bordertype)
{
    mDx = dx;
    mDy = dy;
    mKernelSize = ksize;
    mScale = scale;
    mDelta = delta;
    mDepth = ddepth;
    mThresh = thresh;
    mMaxVal = maxval;
    mBorderType = bordertype;
}



}

#endif // TL_HAVE_OPENCV
