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

#ifdef TL_HAVE_OPENCV
#include <opencv2/core.hpp>

#include "tidop/core/defs.h"
#include "tidop/imgprocess/imgprocess.h"

namespace tl
{

/*! \addtogroup ImgProc
 *  \{
 */


/*! \addtogroup Filters
 *
 *  \{
 */


/*!
 * \brief Bilateral filter
 * The bilateral filter is a non-linear filter that is very effective in noise removal
 * while maintaining sharp edges. The intensity value at each pixel of the image is replaced
 * by a weighted average of the intensity values of nearby pixels.
 */
class TL_EXPORT BilateralFilter
    : public ImageProcess
{

private:

    int mDiameter;
    double mSigmaColor;
    double mSigmaSpace;
    int mBorderType;

public:

    /*!
     * \brief Constructor of BilateralFilter class
     * \param[in] diameter
     * \param[in] sigmaColor Gamma core for smoothing differences in intensities
     * \param[in] sigmaSpace Spatial core for smoothing coordinate differences
     * \param[in] borderType
     */
    BilateralFilter(int diameter,
                    double sigmaColor,
                    double sigmaSpace,
                    int borderType = cv::BORDER_DEFAULT);

    /*!
     * \brief Applies bilateral filter to an image
     * \param[in] matIn Input image.
     * \param[out] matOut Output image.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the parameters of the bilateral filter
     * \param[in] diameter
     * \param[in] sigmaColor Kernel size to smooth intensity differences
     * \param[in] sigmaSpace Spatial kernel to smooth coordinate differences
     * \param[in] borderType
     */
    void setParameters(int diameter,
                       double sigmaColor,
                       double sigmaSpace,
                       int borderType = cv::BORDER_DEFAULT);

};


/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV