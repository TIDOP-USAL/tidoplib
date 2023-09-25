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

#ifndef TL_FEATMATCH_DAISY_DESCRIPTOR_H
#define TL_FEATMATCH_DAISY_DESCRIPTOR_H

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

constexpr auto daisy_default_value_radius{15.};
constexpr auto daisy_default_value_qradius{3};
constexpr auto daisy_default_value_qtheta{8};
constexpr auto daisy_default_value_qhist{8};
constexpr auto daisy_default_value_norm{"NRM_NONE"};
constexpr auto daisy_default_value_interpolation{true};
constexpr auto daisy_default_value_use_orientation{false};

class TL_EXPORT DaisyProperties
  : public Daisy
{
public:

  DaisyProperties();
  DaisyProperties(const DaisyProperties &daisyProperties);
  ~DaisyProperties() override = default;

// Daisy interface

public:

  double radius() const override;
  int qRadius() const override;
  int qTheta() const override;
  int qHist() const override;
  std::string norm() const override;
  bool interpolation() const override;
  bool useOrientation() const override;
  void setRadius(double radius) override;
  void setQRadius(int qRadius) override;
  void setQTheta(int qTheta) override;
  void setQHist(int qHist) override;
  void setNorm(const std::string &norm) override;
  void setInterpolation(bool interpolation) override;
  void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  double mRadius{daisy_default_value_radius};
  int mQRadius{daisy_default_value_qradius};
  int mQTheta{daisy_default_value_qtheta};
  int mQHist{daisy_default_value_qhist};
  std::string mNorm;
  bool mInterpolation{daisy_default_value_interpolation};
  bool mUseOrientation{daisy_default_value_use_orientation};
};


/*----------------------------------------------------------------*/


class TL_EXPORT DaisyDescriptor
    : public DaisyProperties,
      public DescriptorExtractor
{

public:

  DaisyDescriptor();
  DaisyDescriptor(const DaisyDescriptor &daisyDescriptor);
  DaisyDescriptor(double radius,
                  int qRadius,
                  int qTheta,
                  int qHist,
                  const std::string &norm,
                  bool interpolation,
                  bool useOrientation);
  ~DaisyDescriptor() override = default;

private:

  void update();

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;


// Daisy interface

public:

  void setRadius(double radius) override;
  void setQRadius(int qRadius) override;
  void setQTheta(int qTheta) override;
  void setQHist(int qHist) override;
  void setNorm(const std::string &norm) override;
  void setInterpolation(bool interpolation) override;
  void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  void reset() override;

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
  cv::Ptr<cv::xfeatures2d::DAISY> mDAISY;
#endif // HAVE_OPENCV_XFEATURES2D
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_DAISY_DESCRIPTOR_H
