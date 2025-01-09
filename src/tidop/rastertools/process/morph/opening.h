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

#include "tidop/rastertools/process/morph/morphoper.h"

namespace tl
{

/*! \addtogroup MorphOper
 *
 *  \{
 */


/*!
 * \brief Morphological opening operation.
 *
 * The morphological opening operation involves performing an erosion
 * followed by a dilation on an image:
 *
 * \f[
 * Opening = Dilation(Erosion(Image))
 * \f]
 *
 * This operation is used to remove small objects or noise from an image
 * while preserving the shape and size of larger objects. It is particularly
 * effective for removing bright noise (foreground elements smaller than the
 * structuring element) from binary or grayscale images. Opening is often used
 * in preprocessing for object detection and segmentation tasks.
 */
class TL_EXPORT Opening
  : public MorphologicalOperation
{

public:

    /*!
     * \brief Constructor for the Opening class.
     *
     * Initializes the opening operation with the specified parameters.
     *
     * \param[in] size Size of the structuring element. Determines the scale of objects
     *                 that will be removed during erosion.
     * \param[in] type Type of the structuring element (e.g., rectangle, ellipse, cross).
     *                 Defines the shape of the kernel used in the operation.
     * \param[in] anchor Anchor point within the kernel. By default, it is set to the
     *                   center of the structuring element (-1, -1).
     * \param[in] iterations Number of times the opening operation is applied. Increasing
     *                       iterations can amplify the effect of noise removal.
     * \param[in] borderType Method for extrapolating border pixels outside the image boundary.
     *                       Common options include constant borders or reflected edges.
     * \param[in] borderValue Value assigned to border pixels if the border type is set to constant.
     */
    Opening(int size,
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
