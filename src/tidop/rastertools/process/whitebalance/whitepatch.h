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
#ifdef HAVE_OPENCV_XPHOTO
#include <opencv2/xphoto/white_balance.hpp>
#endif // HAVE_OPENCV_XPHOTO

#include "tidop/core/base/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/rastertools/process/imgprocess.h"

namespace tl
{

/*! \addtogroup WhiteBalance
 * \{
 */

/*!
 * \class WhitePatch
 * \brief Implements the White Patch White Balance algorithm.
 *
 * The White Patch algorithm assumes that the brightest pixels in each color channel (red, green, blue) represent the 
 * scene's lighting color. It scales the intensity of each channel so that the brightest pixel corresponds to white 
 * light (255, 255, 255). This effectively removes color casts caused by the lighting and enhances color accuracy.
 *
 * ### Key Features:
 * - Simple and effective for scenes where the assumption of bright white patches holds true.
 * - Particularly useful in scenes with distinct highlights or strong illumination sources.
 *
 * ### Formula:
 * Given:
 * - `Rmax(I)`: Maximum value of the red channel in the image.
 * - `Gmax(I)`: Maximum value of the green channel in the image.
 * - `Bmax(I)`: Maximum value of the blue channel in the image.
 *
 * The White Patch algorithm transforms each pixel `(R, G, B)` as:
 * \f[
 * R' = \frac{255}{Rmax(I)} \cdot R, \quad
 * G' = \frac{255}{Gmax(I)} \cdot G, \quad
 * B' = \frac{255}{Bmax(I)} \cdot B
 * \f]
 */
class TL_EXPORT WhitePatch
  : public ImageProcess
{

private:

    Color mWhite;

public:

    /*!
     * \brief Constructor for the White Patch White Balance processor.
     * \param[in] white The target white color to normalize to. Default is `(255, 255, 255)`.
     */
    WhitePatch(Color white = Color(Color::Name::white));

    /*!
     * \brief Executes the White Patch White Balance process on an image.
     * \param[in] matIn The input image.
     * \param[out] matOut The output image after applying the White Patch White Balance algorithm.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

private:

    auto scaleRed(const cv::Mat& red) const -> double;
    auto scaleGreen(const cv::Mat& green) const -> double;
    auto scaleBlue(const cv::Mat& blue) const -> double;

};

/*! \} */

} // End namespace tl