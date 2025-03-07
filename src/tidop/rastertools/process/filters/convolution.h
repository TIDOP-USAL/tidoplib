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
 * \brief Convolution Filter
 *
 * This class applies a convolution filter to an image using a user-defined kernel. 
 * Convolution is a fundamental operation in image processing and computer vision, 
 * commonly used for tasks such as blurring, sharpening, edge detection, and feature extraction.
 *
 * ### How it Works:
 * The convolution operation involves sliding the kernel (a small matrix) over the image and 
 * computing the weighted sum of the pixel intensities covered by the kernel at each position. 
 * The result is stored in the corresponding pixel of the output image.
 *
 * ### Key Features:
 * - **Custom Kernel:** The user provides a kernel matrix that defines the convolution operation.
 * - **Anchor Point:** Specifies the reference point in the kernel relative to which the convolution is applied. 
 *   If not specified, the anchor defaults to the center of the kernel.
 * - **Delta Adjustment:** Allows an optional constant value to be added to each filtered pixel, useful 
 *   for biasing the result.
 * - **Depth Control:** The user can define the depth of the output image. By default, the output depth 
 *   matches the input image.
 * - **Border Handling:** Supports various extrapolation methods to deal with image borders, such as 
 *   constant, replicate, reflect, or wrap.
 *
 * ### Applications:
 * - Image smoothing (e.g., Gaussian blur).
 * - Edge detection (e.g., Sobel or Prewitt filters).
 * - Feature extraction (e.g., Gabor filters).
 * - Image enhancement (e.g., sharpening).
 */
class TL_EXPORT Convolution
  : public ImageProcess
{

private:

    /*!
     * \brief Depth of the destination image
     *
     * If set to -1, the output image will have the same depth as the source image.
     */
    int mDepth;

    /*!
     * \brief Convolution kernel matrix
     *
     * Defines the weights used in the convolution operation. The kernel matrix should
     * be of odd size (e.g., 3x3, 5x5) to ensure proper alignment with the image pixels.
     */
    cv::Mat mKernel;

    /*!
     * \brief Anchor point
     *
     * The reference point in the kernel relative to which the convolution is applied.
     * By default, the anchor is set to (-1, -1), which corresponds to the kernel center.
     */
    cv::Point mAnchor;

    /*!
     * \brief Optional bias added to filtered pixels
     *
     * A constant value added to the result of the convolution, allowing adjustments to
     * the pixel intensity.
     */
    double mDelta;

    /*!
     * \brief Border extrapolation method
     *
     * Specifies how to handle image borders. Options include:
     * - `cv::BORDER_CONSTANT`: Pads with a constant value.
     * - `cv::BORDER_REPLICATE`: Repeats the border pixels.
     * - `cv::BORDER_REFLECT`: Mirrors the border pixels.
     * - `cv::BORDER_WRAP`: Wraps around the image.
     */
    int mBorderType;

public:

    /*!
     * \brief Constructor of the Convolution class
     *
     * Initializes the convolution filter with the specified parameters.
     *
     * \param[in] ddepth Depth of the destination image. If set to -1, the output image
     *                   will have the same depth as the input image.
     * \param[in] kernel Convolution kernel matrix that defines the operation.
     * \param[in] anchor Anchor point relative to the kernel. Defaults to the kernel's center.
     * \param[in] delta Optional constant value added to each filtered pixel.
     * \param[in] borderType Method for border extrapolation.
     */
    Convolution(int ddepth,
                const cv::Mat &kernel,
                const cv::Point &anchor = cv::Point(-1, -1),
                double delta = 0,
                int borderType = cv::BORDER_CONSTANT);

    /*!
     * \brief Executes the convolution process
     *
     * Processes the input image using the specified convolution kernel and parameters,
     * producing a filtered output image.
     *
     * \param[in] matIn Input image to be filtered.
     * \param[out] matOut Output image containing the filtered result.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */

} // End namespace tl
