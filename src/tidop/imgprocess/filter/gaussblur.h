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
 * \brief Gaussian blur
 */
class TL_EXPORT GaussianBlur
  : public ImageProcess
{

private:

    /*!
     * \brief Kernel size
     */
    cv::Size mKernelSize;

    /*!
     * \brief Standard deviation of the kernel in the X direction.
     */
    double mSigmaX;

    /*!
     * \brief Standard deviation of the kernel in the Y direction.
     */
    double mSigmaY;

    /*!
     * \brief Extrapolation method (cv::BorderTypes)
     */
    int mBorderType;

public:

    /*!
     * \brief GaussianBlur
     * \param[in] kernelSize Kernel size
     * \param[in] sigmaX Standard deviation of the kernel in the X direction
     * \param[in] sigmaY Standard deviation of the kernel in the Y direction
     * \param[in] borderType Extrapolation method (cv::BorderTypes)
     */
    GaussianBlur(const cv::Size &kernelSize,
                 double sigmaX,
                 double sigmaY = 0.,
                 int borderType = 4);

    /*!
     * \brief Executes the process
     * \param[in] matIn Input image
     * \param[out] matOut Output image
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the parameters
     * \param[in] kernelSize Kernel size
     * \param[in] sigmaX Standard deviation of the kernel in the X direction
     * \param[in] sigmaY Standard deviation of the kernel in the Y direction
     * \param[in] borderType Extrapolation method (cv::BorderTypes)
     */
    void setParameters(const cv::Size &kernelSize,
                       double sigmaX,
                       double sigmaY = 0,
                       int borderType = 4);

};



/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV