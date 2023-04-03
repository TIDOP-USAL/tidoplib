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

#ifndef TL_FEATMATCH_LATCH_DESCRIPTOR_H
#define TL_FEATMATCH_LATCH_DESCRIPTOR_H

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


class TL_EXPORT LatchProperties
  : public Latch
{
public:

  LatchProperties();
  LatchProperties(const LatchProperties &latchProperties);
  ~LatchProperties() override = default;

// Latch interface

public:

  std::string bytes() const override;
  bool rotationInvariance() const override;
  int halfSsdSize() const override;
  void setBytes(const std::string &bytes) override;
  void setRotationInvariance(bool rotationInvariance) override;
  void setHalfSsdSize(int halfSsdSize) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  std::string mBytes;
  bool mRotationInvariance{true};
  int mHalfSsdSize{3};
};

/*----------------------------------------------------------------*/


class TL_EXPORT LatchDescriptor
    : public LatchProperties,
      public DescriptorExtractor
{

public:

  LatchDescriptor();
  LatchDescriptor(const LatchDescriptor &latchDescriptor);
  LatchDescriptor(const std::string &bytes,
                  bool rotationInvariance,
                  int halfSsdSize);
  ~LatchDescriptor() override = default;

private:

  void update();

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

// Latch interface

public:

  void setBytes(const std::string &bytes) override;
  void setRotationInvariance(bool rotationInvariance) override;
  void setHalfSsdSize(int halfSsdSize) override;

// Feature interface

public:

  void reset() override;

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
  cv::Ptr<cv::xfeatures2d::LATCH> mLATCH;
#endif // HAVE_OPENCV_XFEATURES2D

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_LATCH_DESCRIPTOR_H
