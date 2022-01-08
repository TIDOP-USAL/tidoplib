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

#ifndef TL_FEATMATCH_GFTT_DETECTOR_H
#define TL_FEATMATCH_GFTT_DETECTOR_H

#include "config_tl.h"

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


class TL_EXPORT GfttProperties
  : public Gftt
{
public:

  GfttProperties();
  GfttProperties(const GfttProperties &gfttProperties);
  ~GfttProperties() override = default;

// Gftt interface

public:

  int maxFeatures() const override;
  double qualityLevel() const override;
  double minDistance() const override;
  int blockSize() const override;
  bool harrisDetector() const override;
  double k() const override;
  void setMaxFeatures(int maxFeatures) override;
  void setQualityLevel(double qlevel) override;
  void setMinDistance(double minDistance) override;
  void setBlockSize(int blockSize) override;
  void setHarrisDetector(bool value) override;
  void setK(double k) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mMaxFeatures{1000};
  double mQualityLevel{0.01};
  double mMinDistance{1};
  int mBlockSize{3};
  bool mHarrisDetector{false};
  double mK{0.04};

};


/*----------------------------------------------------------------*/


class TL_EXPORT GfttDetector
  : public GfttProperties,
    public KeypointDetector
{

public:

  GfttDetector();
  GfttDetector(const GfttDetector &gfttDetector);
  GfttDetector(int maxFeatures,
               double qualityLevel,
               double minDistance,
               int blockSize,
               bool harrisDetector,
               double k);
  ~GfttDetector() override = default;

// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// Gftt interface

public:

  void setMaxFeatures(int maxFeatures) override;
  void setQualityLevel(double qlevel) override;
  void setMinDistance(double minDistance) override;
  void setBlockSize(int blockSize) override;
  void setHarrisDetector(bool value) override;
  void setK(double k) override;

// Feature interface

public:

  void reset() override;

private:

  cv::Ptr<cv::GFTTDetector> mGFTT;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_GFTT_DETECTOR_H
