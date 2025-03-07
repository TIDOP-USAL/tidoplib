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

/*!
 * \ingroup MorphOper
 * \brief Morphological gradient operation.
 */


#pragma once

#include "tidop/config.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "tidop/core/base/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/rastertools/process/imgprocess.h"

namespace tl
{


/*! \addtogroup MorphOper
 *
 * \{
 */


class TL_EXPORT MorphologicalOperation
  : public ImageProcess
{

protected:

    cv::Mat mStructuringElement;
    cv::Point mAnchor;
    int mIterations;
    int mBorderType;
    cv::Scalar mBorderValue;

public:

    MorphologicalOperation(ImageProcess::ProcessType type,
                           int size,
                           cv::MorphShapes shapes = cv::MORPH_RECT,
                           const cv::Point &anchor = cv::Point(-1, -1),
                           int iterations = 1,
                           int borderType = cv::BORDER_CONSTANT,
                           const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());
    
    ~MorphologicalOperation() override = default;

    void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

protected:

    virtual void execute(const cv::Mat &matIn, cv::Mat &matOut) const = 0;
};

/*! \} */

} // End namespace tl
