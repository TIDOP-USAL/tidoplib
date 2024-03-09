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


/*! \addtogroup Filters Image Filters
 *
 *  \{
 */


/*!
 * \brief Laplacian
 * Calculates the Laplacian of an image
 */
class TL_EXPORT Laplacian
  : public ImageProcess
{

private:

    /*!
     * \brief Desired depth of the destination image
     */
    int mDepth;

    /*!
     * \brief Filter aperture
     */
    int mKernelsize;

    /*!
     * \brief Optional scale factor for the values calculated by the Laplacian.
     * By default, no scale is applied.
     */
    double mScale;

    /*!
     * \brief Optional value added to the filtered pixels
     */
    double mDelta;

    /*!
     * \brief Extrapolation method (cv::BorderTypes)
     */
    int mBorderType;



public:

    /*!
     * \brief Laplacian
     * \param[in] ddepth Desired depth of the destination image
     * \param[in] ksize Filter aperture
     * \param[in] scale Optional scale factor for the values calculated by the Laplacian. By default, no scale is applied.
     * \param[in] delta Optional value added to the filtered pixels
     * \param[in] bordertype Extrapolation method (cv::BorderTypes)
     */
    Laplacian(int ddepth,
              int ksize,
              double scale = 1,
              double delta = 0,
              int bordertype = cv::BORDER_DEFAULT);

    /*!
     * \brief Executes the process
     * \param[in] matIn Input image
     * \param[out] matOut Output image
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the parameters
     * \param[in] ddepth Desired depth of the destination image
     * \param[in] ksize Filter aperture
     * \param[in] scale Optional scale factor for the values calculated by the Laplacian. By default, no scale is applied.
     * \param[in] delta Optional value added to the filtered pixels
     * \param[in] bordertype Extrapolation method (cv::BorderTypes)
     */
    void setParameters(int ddepth,
                       int ksize,
                       double scale = 1,
                       double delta = 0,
                       int bordertype = cv::BORDER_DEFAULT);

};


/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV