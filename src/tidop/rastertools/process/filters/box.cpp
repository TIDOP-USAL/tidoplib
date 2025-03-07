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

#include "tidop/rastertools/process/filters/box.h"
#include "tidop/core/base/exception.h"

#include <opencv2/imgproc.hpp>


namespace tl
{



BoxFilter::BoxFilter(int ddepth,
                     const cv::Size &ksize,
                     const cv::Point &anchor,
                     bool normalize,
                     int borderType)
  : ImageProcess(ProcessType::box_filter),
    mDepth(ddepth),
    mKernelSize(ksize),
    mAnchor(anchor),
    mNormalize(normalize),
    mBorderType(borderType)
{
}

void BoxFilter::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        cv::boxFilter(matIn, matOut, mDepth, mKernelSize, mAnchor, mNormalize, mBorderType);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}


} // End namespace tl
