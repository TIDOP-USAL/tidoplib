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
 * \brief Morphological erosion operation.
 *
 * This operation shrinks the bright regions (or foreground) in a binary or grayscale image.
 * Morphological erosion reduces the size of objects by convolving the image with a structuring
 * element (kernel). It removes small bright noise, disconnects connected objects, or emphasizes
 * darker features in the image.
 *
 * Erosion is particularly useful for tasks such as reducing noise, separating connected objects,
 * and shrinking features to analyze their structure. The extent of erosion depends on the size,
 * shape, and number of iterations of the structuring element.
 */
class TL_EXPORT Erotion
  : public MorphologicalOperation
{

public:

    /*!
     * \brief Constructor for the Erotion class.
     *
     * Initializes the erosion operation with the specified parameters.
     *
     * \param[in] size Size of the structuring element. Determines the area of influence for erosion.
     * \param[in] type Type of the structuring element (e.g., rectangle, ellipse, cross).
     *                 Defines the shape of the kernel.
     * \param[in] anchor Anchor point within the kernel. By default, it is set to the center
     *                   of the structuring element (-1, -1).
     * \param[in] iterations Number of times the erosion operation is applied.
     *                       Higher values lead to greater shrinking of features.
     * \param[in] borderType Method for extrapolating border pixels outside the image boundary.
     *                       Common options include constant borders or reflected edges.
     * \param[in] borderValue Value assigned to border pixels if the border type is set to constant.
     */
    Erotion(int size,
            cv::MorphShapes type = cv::MORPH_RECT,
            const cv::Point &anchor = cv::Point(-1, -1),
            int iterations = 1,
            int borderType = cv::BORDER_CONSTANT,
            const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

private:

    void execute(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/*! \} */ 

} // End namespace tl
