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

#include "tidop/core/base/defs.h"
#include "tidop/rastertools/process/imgprocess.h"

namespace tl
{

/*! \addtogroup Filters
 *
 *  \{
 */



/*!
 * \brief Box Filter
 *
 * The box filter is a linear filter that applies a rectangular kernel to the image 
 * for averaging pixel values. It can be used for smoothing, reducing noise, and 
 * general pre-processing in image analysis. Unlike the simple blur filter, the box 
 * filter provides more control over the output depth and normalization of the kernel.
 *
 * ### Parameters:
 * - \b Kernel Size (\a mKernelSize): Specifies the dimensions of the kernel. A larger kernel 
 *   results in stronger smoothing effects.
 * - \b Output Depth (\a mDepth): Determines the depth of the output image. Use -1 to maintain 
 *   the same depth as the input image.
 * - \b Anchor Point (\a mAnchor): Specifies the position of the anchor point within the kernel. 
 *   The default is (-1, -1), meaning the anchor is centered.
 * - \b Normalize (\a mNormalize): Indicates whether the kernel values should be normalized 
 *   (i.e., sum of kernel values equals 1). If set to false, the filter performs a simple 
 *   summation of pixel values.
 * - \b Border Type (\a mBorderType): Defines the method for border extrapolation.
 *
 * ### Applications:
 * - General image smoothing.
 * - Removing noise from images.
 * - Pre-processing step for edge detection or object detection.
 * - Enhancing low-frequency content in images.
 */
class TL_EXPORT BoxFilter
  : public ImageProcess
{

private:

    int mDepth;           //!< Depth of the output image.
    cv::Size mKernelSize; //!< Size of the averaging kernel.
    cv::Point mAnchor;    //!< Anchor point within the kernel.
    bool mNormalize;      //!< Flag to indicate whether kernel values should be normalized.
    int mBorderType;      //!< Method for border extrapolation.

public:

    /*!
     * \brief Constructor for the BoxFilter class
     *
     * Initializes the box filter with specified kernel size, depth, anchor point,
     * normalization, and border handling options.
     *
     * \param[in] ddepth Depth of the output image. Use -1 to keep the input image's depth.
     * \param[in] ksize Size of the kernel (width and height).
     * \param[in] anchor Anchor point within the kernel. Default is (-1, -1), meaning the kernel is centered.
     * \param[in] normalize Whether to normalize the kernel values (default: true).
     * \param[in] borderType Method for border extrapolation (default: cv::BORDER_DEFAULT).
     */
    BoxFilter(int ddepth,
              const cv::Size &ksize,
              const cv::Point &anchor = cv::Point(-1, -1),
              bool normalize = true,
              int borderType = cv::BORDER_DEFAULT);

    /*!
     * \brief Applies the box filter to an input image
     *
     * Processes the input image by averaging pixel intensities within the kernel
     * window. The result is an image that is smoothed or filtered based on the
     * chosen parameters.
     *
     * \param[in] matIn Input image to be processed.
     * \param[out] matOut Output image after applying the box filter.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */

} // End namespace tl