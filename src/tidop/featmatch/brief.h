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

#ifndef TL_FEATMATCH_BRIEF_DESCRIPTOR_H
#define TL_FEATMATCH_BRIEF_DESCRIPTOR_H

#include "config_tl.h"

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


constexpr auto brief_default_value_bytes = "32";
constexpr auto brief_default_value_use_orientation = false;


class TL_EXPORT BriefProperties
  : public Brief
{
public:

  BriefProperties();
  BriefProperties(const BriefProperties &briefProperties);
  BriefProperties(BriefProperties &&briefProperties) TL_NOEXCEPT;
  ~BriefProperties() override;

  BriefProperties &operator =(const BriefProperties &briefProperties);
  BriefProperties &operator =(BriefProperties &&briefProperties) TL_NOEXCEPT;

// Brief interface

public:

  std::string bytes() const override;
  bool useOrientation() const override;
  void setBytes(const std::string &bytes) override;
  void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  std::string mBytes;
  bool mUseOrientation{brief_default_value_use_orientation};
};


/*----------------------------------------------------------------*/


class TL_EXPORT BriefDescriptor
    : public BriefProperties,
      public DescriptorExtractor
{

public:

  BriefDescriptor();
  BriefDescriptor(const BriefDescriptor &briefDescriptor);
  BriefDescriptor(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT;
  BriefDescriptor(const std::string &bytes,
                  bool useOrientation);
  ~BriefDescriptor() override;

  BriefDescriptor &operator =(const BriefDescriptor &briefDescriptor);
  BriefDescriptor &operator =(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT;

private:

  void update();

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;


// Brief interface

public:

  void setBytes(const std::string &bytes) override;
  void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  void reset() override;

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
  cv::Ptr<cv::xfeatures2d::BriefDescriptorExtractor> mBrief;
#endif // HAVE_OPENCV_XFEATURES2D
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_BRIEF_DESCRIPTOR_H
