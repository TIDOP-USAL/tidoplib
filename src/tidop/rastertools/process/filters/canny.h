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
 * \brief Canny Edge Detector
 *
 * The Canny edge detector is a widely used edge detection algorithm designed to identify 
 * edges in an image with precision. It employs a multi-step process that includes noise 
 * reduction, gradient calculation, non-maximum suppression, and hysteresis thresholding 
 * to produce clean and accurate edge maps.
 *
 * ### Threshold Calculation:
 * If the thresholds `threshold1` and `threshold2` are not explicitly provided during initialization, 
 * they are automatically calculated based on the mean and standard deviation of the pixel intensities 
 * in the input image. The formula for this calculation is:
 * \code
 * threshold1 = mean - stdDev;
 * threshold2 = mean + stdDev;
 * \endcode
 * where `mean` and `stdDev` are the mean and standard deviation of the pixel intensities, respectively.
 *
 * ### Applications:
 * - Object boundary detection.
 * - Feature extraction for computer vision tasks.
 * - Preprocessing for image segmentation.
 */
class TL_EXPORT Canny
  : public ImageProcess
{

private:

    double mThreshold1;
    double mThreshold2;

public:

    /*!
     * \brief Constructor for the Canny Edge Detector
     *
     * Initializes the Canny edge detector with user-defined thresholds or calculates them
     * automatically if they are set to zero.
     *
     * \param[in] threshold1 Lower threshold for hysteresis (optional). If set to 0.0, it will
     *            be calculated based on the image's mean and standard deviation.
     * \param[in] threshold2 Upper threshold for hysteresis (optional). If set to 0.0, it will
     *            be calculated based on the image's mean and standard deviation.
     */
    Canny(double threshold1 = 0.0,
          double threshold2 = 0.0);

    /*!
     * \brief Executes the Canny edge detection process
     *
     * Detects edges in the input image using the Canny edge detection algorithm. The output
     * image is a binary image where edges are represented as white pixels on a black background.
     *
     * \param[in] matIn Input image to process.
     * \param[out] matOut Output image with detected edges.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */ 

} // End namespace tl
