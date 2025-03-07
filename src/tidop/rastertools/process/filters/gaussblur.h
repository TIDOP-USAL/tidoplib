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
  * \brief Gaussian Blur Filter
  *
  * This class applies a Gaussian blur to an image, a widely-used technique in image processing
  * for reducing noise and detail. The Gaussian blur works by convolving the image with a Gaussian kernel,
  * resulting in a smooth, natural blur effect.
  *
  * ### How It Works:
  * The Gaussian blur filter replaces each pixel value with a weighted average of its neighboring pixels,
  * where the weights are determined by the Gaussian function:
  * \f[
  * G(x, y) = \frac{1}{2\pi\sigma^2} e^{-\frac{x^2 + y^2}{2\sigma^2}}
  * \f]
  * Here, \f$\sigma\f$ represents the standard deviation, which determines the spread of the Gaussian
  * kernel and the extent of the blur.
  *
  * ### Key Features:
  * - **Custom Kernel Size:** The size of the kernel determines the area of influence for the blur.
  * - **Adjustable Sigma Values:** The standard deviations (\f$\sigma_x\f$ and \f$\sigma_y\f$) control
  *   the intensity of the blur along the X and Y axes, respectively. If \f$\sigma_y\f$ is set to 0,
  *   it will be automatically computed as equal to \f$\sigma_x\f$.
  * - **Border Handling:** Supports various extrapolation methods to manage image borders effectively,
  *   such as constant padding, replication, and mirroring.
  *
  * ### Applications:
  * - Preprocessing images to remove noise before applying edge detection or segmentation.
  * - Creating a natural depth-of-field effect in images.
  * - Smoothing textures in computer vision tasks.
  */
class TL_EXPORT GaussianBlur
  : public ImageProcess
{

private:

    /*!
     * \brief Kernel size
     *
     * Defines the dimensions of the Gaussian kernel. The kernel size must be positive
     * and odd (e.g., 3x3, 5x5).
     */
    cv::Size mKernelSize;

    /*!
     * \brief Standard deviation of the kernel in the X direction
     *
     * Determines the amount of blur along the X-axis. A larger value results in more blur.
     */
    double mSigmaX;

    /*!
     * \brief Standard deviation of the kernel in the Y direction
     *
     * Determines the amount of blur along the Y-axis. If set to 0, it will be computed
     * automatically as equal to \f$\sigma_x\f$.
     */
    double mSigmaY;

    /*!
     * \brief Border extrapolation method
     *
     * Specifies how to handle image borders. Available options include:
     * - `cv::BORDER_CONSTANT`: Pads with a constant value.
     * - `cv::BORDER_REPLICATE`: Repeats the border pixels.
     * - `cv::BORDER_REFLECT`: Mirrors the border pixels.
     * - `cv::BORDER_WRAP`: Wraps around the image.
     */
    int mBorderType;

public:

    /*!
     * \brief Constructor of the GaussianBlur class
     *
     * Initializes the Gaussian blur filter with the specified parameters.
     *
     * \param[in] kernelSize Kernel size, defining the dimensions of the Gaussian kernel.
     * \param[in] sigmaX Standard deviation of the kernel in the X direction.
     * \param[in] sigmaY Standard deviation of the kernel in the Y direction. If set to 0, it
     *                   will be automatically calculated as equal to \f$\sigma_x\f$.
     * \param[in] borderType Extrapolation method for image borders (cv::BorderTypes).
     */
    GaussianBlur(const cv::Size &kernelSize,
                 double sigmaX,
                 double sigmaY = 0.,
                 int borderType = 4);

    /*!
     * \brief Executes the Gaussian blur process
     *
     * Applies the Gaussian blur to the input image using the specified parameters, producing
     * a smoothed output image.
     *
     * \param[in] matIn Input image to be filtered.
     * \param[out] matOut Output image containing the filtered result.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};


/*! \} */

} // End namespace tl