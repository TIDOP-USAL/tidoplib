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

#include "tidop/rastertools/process/whitebalance/grayworld.h"

#include "tidop/core/base/exception.h"
#include "tidop/core/concurrency.h"

#ifdef TL_HAVE_OPENCV
#include <opencv2/highgui.hpp>

#include <cstdarg>
#include <cstdio>

namespace tl
{

#ifdef  HAVE_OPENCV_XPHOTO
#if defined CV_VERSION_MAJOR && CV_VERSION_MAJOR >= 3
#if defined CV_VERSION_MINOR && CV_VERSION_MINOR >= 2

Grayworld::Grayworld()
  : ImageProcess(ProcessType::grayworld),
    mGrayworld(cv::xphoto::createGrayworldWB())
{
}

void Grayworld::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");
        TL_ASSERT(matIn.channels() == 3, "Invalid image type. Incorrect number of channels");

        mGrayworld->balanceWhite(matIn, matOut);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("Grayworld::run() failed"));
    }
}

#endif // CV_VERSION_MINOR
#endif // CV_VERSION_MAJOR
#endif // HAVE_OPENCV_XPHOTO

} // End namespace tl

#endif // TL_HAVE_OPENCV
