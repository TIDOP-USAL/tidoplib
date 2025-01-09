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

#include <opencv2/core.hpp>
#ifdef HAVE_OPENCV_XPHOTO
#include <opencv2/xphoto/white_balance.hpp>
#endif // HAVE_OPENCV_XPHOTO

#include "tidop/core/base/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/rastertools/process/imgprocess.h"

namespace tl
{

/*! \addtogroup WhiteBalance
 * \{
 */

/// \cond
#ifdef HAVE_OPENCV_XPHOTO
#if defined CV_VERSION_MAJOR && CV_VERSION_MAJOR >= 3
#if defined CV_VERSION_MINOR && CV_VERSION_MINOR >= 2
/// \endcond

/*!
 * \class Grayworld
 * \brief Implements the Gray World White Balance algorithm.
 *
 * The Gray World algorithm assumes that the average color of a scene is neutral gray (i.e., R=G=B). 
 * It adjusts the colors in the image to achieve this balance, correcting for overall color casts 
 * caused by lighting conditions.
 *
 * ### Key Features:
 * - Simple and effective for images where the assumption of a neutral average holds true.
 * - Automatically adapts to the input image without requiring user-defined parameters.
 *
 * This class uses OpenCV's `GrayworldWB` implementation for its internal processing.
 */
class TL_EXPORT Grayworld
  : public ImageProcess
{

private:

    /*!
     * \brief Kernel size
     */
    cv::Ptr<cv::xphoto::GrayworldWB> mGrayworld;

public:

    /*!
     * \brief Default constructor for the Gray World White Balance processor.
     */
    Grayworld();

    /*!
     * \brief Executes the Gray World White Balance process on an image.
     * \param[in] matIn The input image.
     * \param[out] matOut The output image after applying the Gray World White Balance algorithm.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;


};

/// \cond
#endif // CV_VERSION_MINOR
#endif // CV_VERSION_MAJOR
#endif // HAVE_OPENCV_XPHOTO
/// \endcond

/*! \} */

} // End namespace tl