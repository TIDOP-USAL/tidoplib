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

#include <memory>

#include <opencv2/objdetect.hpp>

#include "tidop/featmatch/base/features.h"

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


class TL_EXPORT HogProperties
    : public Hog
{

private:

    Size<int> mWinSize;
    Size<int> mBlockSize;
    Size<int> mBlockStride;
    Size<int> mCellSize;
    int mNbins{9};
    int mDerivAperture{1};

public:

    HogProperties();
    HogProperties(const HogProperties &hogProperties);
    ~HogProperties() override = default;

// Hog interface

public:

    auto winSize() const -> Size<int> override;
    auto blockSize() const -> Size<int> override;
    auto blockStride() const -> Size<int> override;
    auto cellSize() const -> Size<int> override;
    auto nbins() const -> int override;
    auto derivAperture() const -> int override;
    void setWinSize(const Size<int> &winSize) override;
    void setBlockSize(const Size<int> &blockSize) override;
    void setBlockStride(const Size<int> &blockStride) override;
    void setCellSize(const Size<int> &cellSize) override;
    void setNbins(int nbins) override;
    void setDerivAperture(int derivAperture) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT HogDescriptor
  : public HogProperties,
    public FeatureDescriptor
{

private:

    std::shared_ptr<cv::HOGDescriptor> mHOG;

public:

    HogDescriptor();
    HogDescriptor(const HogDescriptor &hogDescriptor);
    HogDescriptor(const Size<int> &winSize,
                  const Size<int> &blockSize,
                  const Size<int> &blockStride,
                  const Size<int> &cellSize,
                  int nbins,
                  int derivAperture);
    ~HogDescriptor() override = default;

private:

    void update();
    void normalizepatch(const cv::Mat &gray,
                        const cv::KeyPoint &keypoint,
                        cv::Mat &output);

// FeatureDescriptor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Hog interface

public:

    void setWinSize(const Size<int> &winSize) override;
    void setBlockSize(const Size<int> &blockSize) override;
    void setBlockStride(const Size<int> &blockStride) override;
    void setCellSize(const Size<int> &cellSize) override;
    void setNbins(int nbins) override;
    void setDerivAperture(int derivAperture) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
