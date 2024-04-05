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

#include "tidop/featmatch/features.h"

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


class TL_EXPORT KazeProperties
  : public Kaze
{

private:

    bool mExtended;
    bool mUpright;
    double mThreshold;
    int mOctaves;
    int mOctaveLayers;
    std::string mDiffusivity;

public:

    KazeProperties();
    KazeProperties(const KazeProperties &kazeProperties);
    ~KazeProperties() override = default;

// Kaze interface

public:

    auto extendedDescriptor() const -> bool override;
    auto uprightDescriptor() const -> bool  override;
    auto threshold() const -> double override;
    auto octaves() const -> int override;
    auto octaveLayers() const -> int override;
    auto diffusivity() const -> std::string override;
    void setExtendedDescriptor(bool extended) override;
    void setUprightDescriptor(bool uprightDescriptor) override;
    void setThreshold(double threshold) override;
    void setOctaves(int octaves) override;
    void setOctaveLayers(int octaveLayers) override;
    void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

    void reset() override;
    auto name() const -> std::string final;

};


/*----------------------------------------------------------------*/


class TL_EXPORT KazeDetectorDescriptor
  : public KazeProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

protected:

    cv::Ptr<cv::KAZE> mKaze;

public:

    KazeDetectorDescriptor();
    KazeDetectorDescriptor(const KazeDetectorDescriptor &kazeDetectorDescriptor);
    KazeDetectorDescriptor(bool extendedDescriptor,
                           bool uprightDescriptor,
                           double threshold,
                           int octaves,
                           int octaveLayers,
                           const std::string &diffusivity);
    ~KazeDetectorDescriptor() override = default;

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDiffusivity(const std::string &diffusivity) -> cv::KAZE::DiffusivityType;
#else
    auto convertDiffusivity(const std::string &diffusivity) -> int;
#endif
    void updateCvKaze();

// KeypointDetector interface

public:

    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// DescriptorExtractor interface

public:

    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

// Kaze interface

public:

    void setExtendedDescriptor(bool extended) override;
    void setUprightDescriptor(bool uprightDescriptor) override;
    void setThreshold(double threshold) override;
    void setOctaves(int octaves) override;
    void setOctaveLayers(int octaveLayers) override;
    void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

    void reset() override;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
