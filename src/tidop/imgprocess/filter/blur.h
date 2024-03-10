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
 * \brief Blur filter
 * Causes a smoothing effect on the resulting image
 */
class TL_EXPORT Blur
    : public ImageProcess
{

private:

    /*!
     * \brief Kernel size
     */
    cv::Size mKernelSize;

    /*!
     * \brief Anchor point.
     * By default, it is the center of the kernel.
     */
    cv::Point mAnchor;

    /*!
     * \brief Border type
     * Border type cv::BorderTypes
     */
    int mBorderType;

public:

    /*!
     * \brief Constructor of the class
     * \param[in] ksize Kernel size
     * \param[in] anchor Anchor point
     * \param[in] borderType Border type
     */
    Blur(const cv::Size &ksize,
         const cv::Point &anchor = cv::Point(-1, -1),
         int borderType = cv::BORDER_DEFAULT);

    /*!
     * \brief Applies the blur filter.
     * \param[in] matIn Input image.
     * \param[out] matOut Output image.
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the filter parameters
     * \param[in] ksize Kernel size
     * \param[in] anchor Anchor point
     * \param[in] borderType Border type
     */
    void setParameters(const cv::Size ksize,
                       const cv::Point &anchor = cv::Point(-1, -1),
                       int borderType = cv::BORDER_DEFAULT);

};



/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV