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

#include <opencv2/imgproc.hpp>

#include "tidop/rastertools/process/morph/morphoper.h"

namespace tl
{

/*! \addtogroup MorphOper
 *
 *  \{
 */


/*!
 * \brief Morphological Black Hat operation
 *
 * This operation highlights dark regions in an image that are surrounded by lighter areas.
 * It is performed by applying a morphological closing operation (dilation followed by erosion)
 * to the input image and then subtracting the result from the original image.
 *
 * The Black Hat operation is commonly used in image processing to extract specific features,
 * such as dark objects, shadows, or imperfections in brighter regions, making it useful
 * for tasks like document analysis or defect detection.
 */
class TL_EXPORT BlackHat
  : public MorphologicalOperation
{

public:

    /*!
     * \brief Constructor for the BlackHat class
     * \param[in] size Size of the structuring element.
     * \param[in] shapes Type of the structuring element.
     * \param[in] anchor Anchor point. Default is the center of the kernel.
     * \param[in] iterations Number of iterations.
     * \param[in] borderType Method for extrapolation.
     * \param[in] borderValue Border value to be used.
     */
    BlackHat(int size,
             cv::MorphShapes shapes = cv::MORPH_RECT,
             const cv::Point &anchor = cv::Point(-1, -1),
             int iterations = 1,
             int borderType = cv::BORDER_CONSTANT,
             const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

private:

    void execute(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */ 

} // End namespace tl
