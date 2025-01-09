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

#include "tidop/rastertools/process/morph/gradient.h"

#include "tidop/core/base/exception.h"

#include <opencv2/highgui.hpp>

#include <cstdarg>
#include <cstdio>


namespace tl
{

Gradient::Gradient(int size,
                   cv::MorphShapes shapes,
                   const cv::Point &anchor,
                   int iterations,
                   int borderType,
                   const cv::Scalar &borderValue)
  : MorphologicalOperation(ImageProcess::ProcessType::morph_gradient,
                           size, shapes, anchor, iterations,
                           borderType, borderValue)
{
}

void Gradient::execute(const cv::Mat &matIn, cv::Mat &matOut) const
{
    cv::morphologyEx(matIn, matOut, 
                     cv::MORPH_GRADIENT,
                     mStructuringElement,
                     mAnchor,
                     mIterations,
                     mBorderType,
                     mBorderValue);
}



} // End namespace tl
