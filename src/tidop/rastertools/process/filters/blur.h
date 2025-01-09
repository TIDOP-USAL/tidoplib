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

#include "tidop/core/base/defs.h"

#include <opencv2/core.hpp>

#include "tidop/rastertools/process/imgprocess.h"

namespace tl
{

/*! \addtogroup Filters
 *
 *  \{
 */



/*!
 * \brief Blur Filter
 *
 * The blur filter applies a simple averaging filter to the input image,
 * causing a smoothing effect. This operation reduces noise and detail,
 * resulting in a softer appearance of the image. It is one of the simplest
 * linear filters and is commonly used as a pre-processing step in computer
 * vision tasks.
 *
 * ### Parameters:
 * - \b Kernel Size (\a mKernelSize): Specifies the size of the averaging kernel.
 *   Larger kernels produce stronger smoothing effects.
 * - \b Anchor Point (\a mAnchor): Defines the point relative to the kernel that
 *   is placed over the pixel being processed. By default, this is set to (-1, -1),
 *   which means the anchor is centered.
 * - \b Border Type (\a mBorderType): Determines the method used for border extrapolation.
 *
 * ### Applications:
 * - Smoothing images to reduce noise.
 * - Pre-processing for edge detection.
 * - Reducing high-frequency content in images.
 */
class TL_EXPORT Blur
    : public ImageProcess
{

private:

    cv::Size mKernelSize; //!< Size of the averaging kernel.
    cv::Point mAnchor;    //!< Anchor point within the kernel.
    int mBorderType;      //!< Border extrapolation method.

public:

    /*!
     * \brief Constructor for the Blur class
     *
     * Initializes the blur filter with the specified kernel size, anchor point, and border type.
     *
     * \param[in] ksize Size of the kernel (width and height).
     * \param[in] anchor Anchor point within the kernel. Default is (-1, -1), meaning the kernel is centered.
     * \param[in] borderType Method for border extrapolation. Default is cv::BORDER_DEFAULT.
     */
    Blur(const cv::Size &ksize,
         const cv::Point &anchor = cv::Point(-1, -1),
         int borderType = cv::BORDER_DEFAULT);

    /*!
     * \brief Applies the blur filter to the input image
     *
     * \param[in] matIn Input image to be processed.
     * \param[out] matOut Output image after applying the blur filter.
     *
     * This function processes the input image by averaging the pixel intensities within
     * the kernel window. The result is a smoother, less detailed output image.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */

} // End namespace tl
