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
#include <opencv2/core.hpp>

#include "tidop/rastertools/process/imgprocess.h"
#include "tidop/math/geometry/affine.h"

namespace tl
{

/*! \addtogroup ImgTransf
 *
 *  \{
 */

 /*!
  * \brief Applies an affine transformation to an input image.
  *
  */
class AffineTransform
  : public ImageProcess
{

private:

    Affine<float, 2> mAffine;

public:

    /*!
     * \brief Constructs an \c Affine object with a given transformation matrix.
     * \param[in] affine The affine transformation matrix.
     */
    explicit AffineTransform(const Affine<float, 2> &affine);

    ~AffineTransform() override = default;

    /*!
     * \brief Executes the affine transformation on the input image.
     *
     * This method takes an input image, applies the affine transformation matrix
     * and outputs the transformed image.
     *
     * \param[in] matIn The input image
     * \param[out] matOut The output imageafter applying the affine transformation.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};


/*! \} */

} // End namespace tl