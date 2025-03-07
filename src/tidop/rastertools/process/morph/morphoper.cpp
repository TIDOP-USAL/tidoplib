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

#include "tidop/rastertools/process/morph/morphoper.h"

#include "tidop/core/base/exception.h"

#include <opencv2/highgui.hpp>

#include <cstdarg>
#include <cstdio>


namespace tl
{

MorphologicalOperation::MorphologicalOperation(ImageProcess::ProcessType type,
                                               int size, 
                                               cv::MorphShapes shapes,
                                               const cv::Point &anchor,
                                               int iterations,
                                               int borderType,
                                               const cv::Scalar &borderValue)
  : ImageProcess(type),
    mAnchor(anchor),
    mIterations(iterations),
    mBorderType(borderType),
    mBorderValue(borderValue)
{
    mStructuringElement = getStructuringElement(shapes,
                                                cv::Size(2 * size + 1, 2 * size + 1),
                                                cv::Point(size, size));
}

void MorphologicalOperation::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        execute(matIn, matOut);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("MorphologicalOperation::run() failed"));
    }
}



} // End namespace tl

