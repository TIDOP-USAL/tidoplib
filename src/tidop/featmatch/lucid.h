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

#include "tidop/featmatch/features.h"

#ifdef HAVE_OPENCV_XFEATURES2D 
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D


namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */


class TL_EXPORT LucidProperties
  : public Lucid
{

private:

    int mLucidKernel{1};
    int mBlurKernel{2};

public:

    LucidProperties();
    ~LucidProperties() override = default;

// Lucid interface

public:

    auto lucidKernel() const -> int override;
    auto blurKernel() const -> int override;
    void setLucidKernel(int lucidKernel) override;
    void setBlurKernel(int blurKernel) override;

// Feature interface

public:

    void reset() override;
    std::string name() const final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT LucidDescriptor
  : public LucidProperties,
    public DescriptorExtractor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::LUCID> mLUCID;
#endif // HAVE_OPENCV_XFEATURES2D

public:

    LucidDescriptor();
    LucidDescriptor(int lucidKernel, int blurKernel);
    ~LucidDescriptor() override = default;

private:

    void update();

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Lucid interface

public:

    void setLucidKernel(int lucidKernel) override;
    void setBlurKernel(int blurKernel) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
