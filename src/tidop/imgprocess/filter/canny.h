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
 * \brief Canny edge detector
 */
class TL_EXPORT Canny
  : public ImageProcess
{

private:

    /*!
     * \brief First threshold for the hysteresis procedure
     */
    double mThreshold1;

    /*!
     * \brief Second threshold for the hysteresis procedure
     */
    double mThreshold2;

public:

    /*!
     * \brief Constructor
     * \param[in] threshold1 First threshold for the hysteresis procedure
     * \param[in] threshold2 Second threshold for the hysteresis procedure
     */
    Canny(double threshold1 = 0.0,
          double threshold2 = 0.0);

    /*!
     * \brief Executes the process
     * \param[in] matIn Input image
     * \param[out] matOut Output image
     */
    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

    /*!
     * \brief Sets the parameters for the Canny edge detector
     * \param[in] threshold1 First threshold for the hysteresis procedure
     * \param[in] threshold2 Second threshold for the hysteresis procedure
     */
    void setParameters(double threshold1,
                       double threshold2);

};



/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // TL_HAVE_OPENCV