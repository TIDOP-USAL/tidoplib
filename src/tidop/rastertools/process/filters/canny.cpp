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


#include "tidop/rastertools/process/filters.h"
#include "tidop/core/base/exception.h"

#include <opencv2/imgproc.hpp>


namespace tl
{



Canny::Canny(double threshold1, double threshold2)
  : ImageProcess(ProcessType::canny),
    mThreshold1(threshold1),
    mThreshold2(threshold2)
{
}

void Canny::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        double th1 = mThreshold1;
        double th2 = mThreshold2;

        if (th1 == 0.0 && th2 == 0.0) {
            cv::Scalar mean;
            cv::Scalar stdv;
            cv::meanStdDev(matIn, mean, stdv);
            th1 = mean[0] - stdv[0];
            th2 = mean[0] + stdv[0];
        }

        cv::Canny(matIn, matOut, th1, th2, 3);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}


} // End namespace tl
