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
 * \brief Bilateral Filter
 *
 * The bilateral filter is a non-linear, edge-preserving, and noise-reducing smoothing filter.
 * It replaces the intensity value at each pixel with a weighted average of the intensities of
 * nearby pixels, where the weights depend on both the spatial distance and the intensity difference.
 *
 * This filter is particularly effective for:
 * - Removing noise while preserving sharp edges in images.
 * - Applications such as denoising photographs and pre-processing for edge-detection algorithms.
 *
 * ### Parameters:
 * - \b Diameter (\a mDiameter): Specifies the diameter of the pixel neighborhood used during filtering.
 *   A larger diameter leads to stronger smoothing.
 * - \b SigmaColor (\a mSigmaColor): Controls how much influence intensity differences between pixels
 *   have on the smoothing. A higher value allows more differences in intensity to be smoothed.
 * - \b SigmaSpace (\a mSigmaSpace): Controls how much influence spatial distance between pixels has
 *   on the smoothing. A higher value results in wider smoothing in terms of distance.
 * - \b BorderType (\a mBorderType): Defines the method used for extrapolating image borders.
 *
 * The bilateral filter is computationally expensive compared to other smoothing filters like Gaussian
 * or median filters, but its ability to preserve edges makes it a preferred choice in scenarios where
 * edge information is critical.
 */
class TL_EXPORT BilateralFilter
  : public ImageProcess
{

private:

    int mDiameter;       //!< Diameter of the pixel neighborhood.
    double mSigmaColor;  //!< Filter sigma in the color space.
    double mSigmaSpace;  //!< Filter sigma in the coordinate space.
    int mBorderType;     //!< Border type for extrapolation.

public:

    /*!
     * \brief Constructor of BilateralFilter class
     *
     * Initializes the bilateral filter with the specified parameters.
     *
     * \param[in] diameter Diameter of the pixel neighborhood.
     * \param[in] sigmaColor Controls the filter's sensitivity to intensity differences.
     * \param[in] sigmaSpace Controls the filter's sensitivity to spatial distance.
     * \param[in] borderType Method for border extrapolation. Default is cv::BORDER_DEFAULT.
     */
    BilateralFilter(int diameter,
                    double sigmaColor,
                    double sigmaSpace,
                    int borderType = cv::BORDER_DEFAULT);

    /*!
     * \brief Applies the bilateral filter to an image
     *
     * \param[in] matIn Input image to be processed.
     * \param[out] matOut Output image after applying the filter.
     *
     * This function processes the input image and produces a filtered output image
     * where noise is reduced while preserving edge details.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */

} // End namespace tl