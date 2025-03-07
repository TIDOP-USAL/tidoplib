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
 * \brief Sobel Operator
 *
 * The Sobel operator is an edge detection operator that calculates the gradient of image intensity
 * at each pixel, giving the direction of the greatest increase in intensity. It is primarily used 
 * for edge detection in images, allowing you to highlight regions of high spatial frequency.
 *
 * ### Key Features:
 * - **Gradient Calculation:** Computes the gradient of the image intensity using Sobel kernels.
 * - **Edge Detection:** Highlights edges in an image, useful for feature extraction and object detection.
 * - **Directional Information:** The Sobel operator can compute gradients in both horizontal (x-direction)
 *   and vertical (y-direction) directions.
 *
 * ### Parameters:
 * - **dx, dy:** Determines the order of the derivative in the x and y directions.
 * - **Kernel Size (ksize):** Defines the size of the Sobel kernel used for gradient calculation.
 * - **Scale:** Scales the result by a constant factor to adjust gradient magnitude.
 * - **Delta:** Adds a constant value to the computed gradient value.
 * - **Thresholding:** After computing the gradient, the magnitude can be thresholded to focus on significant edges.
 *
 * ### Applications:
 * - **Edge Detection:** Used in many image processing applications to detect boundaries of objects.
 * - **Feature Detection:** Helps in identifying edges for feature extraction in tasks like object recognition.
 * - **Image Segmentation:** The Sobel operator can be used to delineate different regions in an image.
 */
class TL_EXPORT Sobel
    : public ImageProcess
{

private:

    /*!
     * \brief Derivative in the x-direction
     *
     * Specifies the order of the derivative in the x-direction. A value of 1 calculates the first derivative,
     * which highlights horizontal edges.
     */
    int mDx;

    /*!
     * \brief Derivative in the y-direction
     *
     * Specifies the order of the derivative in the y-direction. A value of 1 calculates the first derivative,
     * which highlights vertical edges.
     */
    int mDy;

    /*!
     * \brief Kernel size
     *
     * Specifies the size of the Sobel kernel, which is typically 3x3 or 5x5. Larger kernels capture edges
     * at a broader scale but may lose fine details.
     */
    int mKernelSize;

    /*!
     * \brief Scale factor
     *
     * A scaling factor to adjust the magnitude of the computed gradients.
     */
    double mScale;

    /*!
     * \brief Delta value
     *
     * An optional constant value added to the computed gradients.
     */
    double mDelta;

    /*!
     * \brief Desired depth of the destination image
     *
     * Specifies the depth of the output image, commonly `CV_16S` or `CV_32F`, depending on the desired
     * range of gradient values.
     */
    int mDepth;

    /*!
     * \brief Threshold value
     *
     * A threshold applied to the computed gradient magnitude to filter out weaker edges.
     */
    double mThresh;

    /*!
     * \brief Maximum threshold value
     *
     * The maximum value that an edge magnitude can take, used for scaling the gradients.
     */
    double mMaxVal;

    /*!
     * \brief Border type
     *
     * Defines the method used to extrapolate image borders when applying the Sobel operator.
     */
    int mBorderType;

public:

    /*!
     * \brief Constructor for the Sobel operator
     *
     * Initializes the Sobel operator with the specified parameters.
     *
     * \param[in] dx Order of the derivative in the x-direction (1 for the first derivative).
     * \param[in] dy Order of the derivative in the y-direction (1 for the first derivative).
     * \param[in] ksize Size of the Sobel kernel (3x3 or 5x5 typically).
     * \param[in] scale Scale factor for the gradient values.
     * \param[in] delta Value added to the gradient result.
     * \param[in] ddepth Desired depth of the output image (e.g., `CV_16S`).
     * \param[in] thresh Threshold applied to the computed gradient magnitude.
     * \param[in] maxval Maximum value of the computed gradient.
     * \param[in] bordertype Border handling method (e.g., `cv::BORDER_DEFAULT`).
     */
    Sobel(int dx,
          int dy,
          int ksize = 3,
          double scale = 1.,
          double delta = 0.,
          int ddepth = CV_16S,
          double thresh = 50.,
          double maxval = 200.,
          int bordertype = cv::BORDER_DEFAULT);

    /*!
     * \brief Executes the Sobel operator on the input image
     *
     * Applies the Sobel operator to the input image to detect edges.
     * The result is stored in the output image, where edges are enhanced based on the gradient magnitude.
     *
     * \param[in] matIn Input image to be processed by the Sobel operator.
     * \param[out] matOut Output image with the detected edges.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};


/*! \} */


} // End namespace tl
