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

#ifndef TL_FEATMATCH_BOOST_DESCRIPTOR_H
#define TL_FEATMATCH_BOOST_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#ifdef HAVE_OPENCV_XFEATURES2D 
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D

namespace tl
{

constexpr auto boost_default_value_use_orientation = true;
constexpr auto boost_default_value_scale_factor = 6.25;

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */


class TL_EXPORT BoostProperties
  : public Boost
{

public:

  BoostProperties();
  BoostProperties(const BoostProperties &boostProperties);
  BoostProperties(BoostProperties &&boostProperties) TL_NOEXCEPT;
  ~BoostProperties() override;

  BoostProperties &operator =(const BoostProperties &boostProperties);
  BoostProperties &operator =(BoostProperties &&boostProperties) TL_NOEXCEPT;

// Boost interface

public:

  std::string descriptorType() const override;
  bool useOrientation() const override;
  double scaleFactor() const override;
  void setDescriptorType(const std::string &descriptorType) override;
  void setUseOrientation(bool useOrientation) override;
  void setScaleFactor(double scaleFactor) override;

// Feature interface

public:

  void reset() override;
  std::string name() const override;

private:

  std::string mDescriptorType;
  bool bUseOrientation{boost_default_value_use_orientation};
  double mScaleFactor{boost_default_value_scale_factor};

};


/*----------------------------------------------------------------*/

class TL_EXPORT BoostDescriptor
  : public BoostProperties,
    public DescriptorExtractor
{

public:

  BoostDescriptor();
  BoostDescriptor(const BoostDescriptor &boostDescriptor);
  BoostDescriptor(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT;
  BoostDescriptor(const std::string &descriptorType,
                  bool useOrientation,
                  double scaleFactor);
  ~BoostDescriptor() override = default;

  BoostDescriptor &operator =(const BoostDescriptor &boostDescriptor);
  BoostDescriptor &operator =(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT;

private:

  void update();

// Feature interface

public:

  void reset() override;

// Boost interface

public:

  void setDescriptorType(const std::string &descriptorType) override;
  void setUseOrientation(bool useOrientation) override;
  void setScaleFactor(double scaleFactor) override;

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  cv::Ptr<cv::xfeatures2d::BoostDesc> mBoost;
#endif
#endif // HAVE_OPENCV_XFEATURES2D

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_BOOST_DESCRIPTOR_H
