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
#ifdef HAVE_OPENCV_XPHOTO
#include <opencv2/xphoto/white_balance.hpp>
#endif // HAVE_OPENCV_XPHOTO

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/imgprocess/imgprocess.h"

namespace tl
{

// Balance de blancos
// https://courses.cs.washington.edu/courses/cse467/08au/labs/l5/whiteBalance.pdf


/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup WhiteBalance White Balance
 *
 *  \{
 */


#ifdef HAVE_OPENCV_XPHOTO
#if defined CV_VERSION_MAJOR && CV_VERSION_MAJOR >= 3
#if defined CV_VERSION_MINOR && CV_VERSION_MINOR >= 2

/*!
 * \brief Gray World White Balance
 */
class TL_EXPORT Grayworld
  : public ImageProcess
{

private:

    /*!
     * \brief Kernel size
     */
    cv::Ptr<cv::xphoto::GrayworldWB> mGrayworld;

public:

    /*!
     * \brief Constructor Gray World
     */
    Grayworld();

    /*!
     * \brief Executes the process
     * \param[in] matIn Input image
     * \param[out] matOut Output image
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;


};

#endif // CV_VERSION_MINOR
#endif // CV_VERSION_MAJOR
#endif // HAVE_OPENCV_XPHOTO



/*!
 * \brief White Patch white balance
 *
 * Assumes that the maximum color values in the three channels of the image represent white
 * under the scene lighting, then removes the effect of that light and imposes white light.
 * Estimation of the scene light color from the image:
 * Rmax(I): Maximum value of the red channel of image I
 * Gmax(I): Maximum value of the green channel of image I
 * Bmax(I): Maximum value of the blue channel of image I
 * For a white light of (255,255,255), the scene light is removed and white light is imposed:
 * (R, G, B) -> ((255/Rmax(I))*R, (255/Gmax(I))*G, (255/Bmax(I))*B)
 */
class TL_EXPORT WhitePatch
  : public ImageProcess
{

private:

    Color mWhite;

public:

    /*!
     * \brief WhitePatch constructor
     * \param[in] white White light. Default is (255, 255, 255)
     */
    WhitePatch(Color white = Color(Color::Name::white));

    /*!
     * \brief Executes the WhitePatch process
     * \param[in] matIn Input image
     * \param[out] matOut Output image
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the white light
     * \param[in] white White light. Default is (255, 255, 255)
     */
    void setWhite(const Color &white = Color(Color::Name::white));

private:

    auto scaleRed(const cv::Mat& red) const -> double;
    auto scaleGreen(const cv::Mat& green) const -> double;
    auto scaleBlue(const cv::Mat& blue) const -> double;

};

/*! \} */ // end of WhiteBalance

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV
