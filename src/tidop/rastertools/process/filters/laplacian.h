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
 * \brief Laplacian Filter
 *
 * This class computes the Laplacian of an image, a commonly used operator in image processing
 * for edge detection and highlighting regions of rapid intensity change. The Laplacian is
 * a second-order derivative operator defined as:
 * \f[
 * \nabla^2 f = \frac{\partial^2 f}{\partial x^2} + \frac{\partial^2 f}{\partial y^2}
 * \f]
 * It calculates the rate of change of gradients in the image, making it highly sensitive to
 * noise but effective for detecting edges.
 *
 * ### Key Features:
 * - **Kernel Size:** Configurable filter aperture that defines the size of the kernel used.
 * - **Depth Control:** Allows specifying the desired depth of the output image.
 * - **Scale and Offset:** Optional parameters for scaling and offsetting the Laplacian result.
 * - **Border Handling:** Various extrapolation methods to handle image borders during processing.
 *
 * ### Applications:
 * - **Edge Detection:** Identifies regions of rapid intensity change for segmentation or feature extraction.
 * - **Feature Enhancement:** Highlights fine details and structures in images.
 * - **Preprocessing for Blob Detection:** Enhances features for further analysis.
 */
class TL_EXPORT Laplacian
  : public ImageProcess
{

private:

    /*!
     * \brief Desired depth of the destination image
     *
     * Specifies the pixel depth of the output image. Use -1 to preserve the same depth as the input image.
     */
    int mDepth;

    /*!
     * \brief Filter aperture size
     *
     * Defines the size of the kernel used for computing the Laplacian. Must be a positive odd number, such as 1, 3, or 5.
     */
    int mKernelsize;

    /*!
     * \brief Scale factor for the Laplacian values
     *
     * Optional parameter that multiplies the computed Laplacian values to adjust the intensity. Default value is 1 (no scaling).
     */
    double mScale;

    /*!
     * \brief Offset added to the Laplacian result
     *
     * Optional parameter for adding a constant value to the filtered pixels. Default value is 0.
     */
    double mDelta;

    /*!
     * \brief Border extrapolation method
     *
     * Specifies how to handle the borders of the image. Options include:
     * - `cv::BORDER_CONSTANT`: Pads the border with a constant value.
     * - `cv::BORDER_REPLICATE`: Repeats the edge pixels.
     * - `cv::BORDER_REFLECT`: Mirrors the border.
     * - `cv::BORDER_WRAP`: Wraps around the image.
     */
    int mBorderType;

public:

    /*!
     * \brief Constructor for the Laplacian class
     *
     * Initializes the Laplacian filter with specified parameters.
     *
     * \param[in] ddepth Desired depth of the destination image. Use -1 to match the input image depth.
     * \param[in] ksize Filter aperture size. Must be a positive odd number.
     * \param[in] scale Optional scale factor for the computed Laplacian values. Default is 1.
     * \param[in] delta Optional constant offset added to the filtered pixels. Default is 0.
     * \param[in] bordertype Extrapolation method for the image borders (cv::BorderTypes).
     */
    Laplacian(int ddepth,
              int ksize,
              double scale = 1,
              double delta = 0,
              int bordertype = cv::BORDER_DEFAULT);

    /*!
     * \brief Executes the Laplacian filter process
     *
     * Applies the Laplacian operator to the input image and produces the resulting output image.
     *
     * \param[in] matIn Input image to be processed.
     * \param[out] matOut Output image containing the Laplacian result.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};


/*! \} */ 

} // End namespace tl
