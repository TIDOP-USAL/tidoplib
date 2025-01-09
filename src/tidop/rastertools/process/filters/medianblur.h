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
 * \brief Median Filter
 *
 * This class applies a median filter to an image, an effective method for reducing noise
 * while preserving edges. The median filter replaces each pixel's value with the median
 * value of the intensities within a neighborhood defined by the filter size.
 *
 * ### Key Features:
 * - **Noise Reduction:** Particularly effective for removing salt-and-pepper noise from images.
 * - **Edge Preservation:** Unlike linear filters, the median filter preserves edges by considering
 *   the median of the neighborhood rather than averaging pixel values.
 * - **Non-Linear Operation:** The filter relies on sorting pixel intensities within the kernel,
 *   making it robust against outliers.
 *
 * ### Filter Behavior:
 * - The size of the filter is defined by the kernel size, which must be an odd integer.
 * - Larger kernel sizes result in stronger smoothing but may introduce distortions in finer details.
 *
 * ### Applications:
 * - **Noise Removal:** Especially useful for reducing impulsive noise (salt-and-pepper noise).
 * - **Preprocessing for Image Segmentation:** Enhances image quality for better segmentation results.
 * - **Edge Preservation Smoothing:** Ideal for scenarios where edge information is critical.
 */
class TL_EXPORT MedianBlur : public ImageProcess
{
private:

    /*!
     * \brief Filter aperture size
     *
     * Specifies the size of the neighborhood used to calculate the median value.
     * Must be a positive odd integer (e.g., 3, 5, 7).
     */
    int mKernelSize;

public:

    /*!
     * \brief Constructor for the MedianBlur class
     *
     * Initializes the median filter with a specified kernel size.
     *
     * \param ksize Size of the filter kernel. Must be an odd positive integer.
     * Larger values result in stronger smoothing effects.
     */
    MedianBlur(int ksize);

    /*!
     * \brief Executes the median filter process
     *
     * Applies the median filter to the input image and produces the smoothed output image.
     *
     * \param[in] matIn Input image to be processed.
     * \param[out] matOut Output image containing the result of the median filter.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */ 

} // End namespace tl