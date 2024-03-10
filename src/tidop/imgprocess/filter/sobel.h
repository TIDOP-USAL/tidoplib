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
 * \brief Sobel
 */
class TL_EXPORT Sobel
    : public ImageProcess
{

private:

    /*!
     * \brief dx
     */
    int mDx;

    /*!
     * \brief dy
     */
    int mDy;

    /*!
     * \brief mKernelSize
     */
    int mKernelSize;

    /*!
     * \brief scale
     */
    double mScale;

    /*!
     * \brief delta
     */
    double mDelta;

    /*!
     * \brief mDepth
     */
    int mDepth;

    /*!
     * \brief thresh
     */
    double mThresh;

    /*!
     * \brief maxval
     */
    double mMaxVal;

    /*!
     * \brief bordertype
     */
    int mBorderType;

public:

    /*!
     * \brief Constructor
     * \param[in] dx
     * \param[in] dy
     * \param[in] ksize
     * \param[in] scale
     * \param[in] delta
     * \param[in] ddepth
     * \param[in] thresh
     * \param[in] maxval
     * \param[in] bordertype
     */
    Sobel(int dx,
          int dy,
          int ksize = 3,
          double scale = 1.,
          double delta = 0.,
          int ddepth = CV_16S,
          double thresh = 50.,
          double maxval = 200.,
          int bordertype = cv::BORDER_DEFAULT);

    /*!
     * \brief Executes the process
     * \param[in] matIn Input image
     * \param[out] matOut Output image
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the parameters
     * \param[in] dx Order of the derivative in x
     * \param[in] dy Order of the derivative in y
     * \param[in] ksize Kernel size
     * \param[in] scale Optional scale factor for the calculated derivatives values
     * \param[in] delta
     * \param[in] ddepth
     * \param[in] thresh
     * \param[in] maxval
     * \param[in] bordertype
     */
    void setParameters(int dx,
                       int dy,
                       int ksize = 3,
                       double scale = 1.,
                       double delta = 0.,
                       int ddepth = CV_16S,
                       double thresh = 50.,
                       double maxval = 200.,
                       int bordertype = cv::BORDER_DEFAULT);

};


/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV