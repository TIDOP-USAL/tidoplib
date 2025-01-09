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
 * \brief Morphological Top Hat operation.
 *
 * The Top Hat operation computes the difference between an image and the result
 * of applying a morphological opening operation to the same image:
 *
 * \f[
 * TopHat = Image - Opening(Image)
 * \f]
 *
 * This operation is useful for highlighting small, bright elements or regions in an
 * image that are smaller than the structuring element used in the opening. It can be
 * applied to tasks such as enhancing fine details, detecting small objects, or
 * preprocessing images for further analysis.
 */
class TL_EXPORT TopHat
  : public MorphologicalOperation
{
public:

    /*!
     * \brief Constructor for the TopHat class.
     *
     * Initializes the Top Hat operation with the specified parameters.
     *
     * \param[in] size Size of the structuring element. Determines the scale of elements
     *                 to be highlighted in the image.
     * \param[in] type Type of the structuring element (e.g., rectangle, ellipse, cross).
     *                 Defines the shape of the kernel used in the operation.
     * \param[in] anchor Anchor point within the kernel. By default, it is set to the
     *                   center of the structuring element (-1, -1).
     * \param[in] iterations Number of times the Top Hat operation is applied. Increasing
     *                       iterations can accentuate the highlighting effect.
     * \param[in] borderType Method for extrapolating border pixels outside the image boundary.
     *                       Common options include constant borders or reflected edges.
     * \param[in] borderValue Value assigned to border pixels if the border type is set to constant.
     */
    TopHat(int size,
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
