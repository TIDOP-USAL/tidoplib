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

#ifndef TL_FEATMATCH_GSM_H
#define TL_FEATMATCH_GSM_H

#include "tidop/featmatch/matcher.h"

namespace tl
{

constexpr auto gms_default_value_rotation{true};
constexpr auto gms_default_value_scale{true};
constexpr auto gms_default_value_threshold{6.};


class TL_EXPORT GmsProperties
  : public Gms
{

public:

  GmsProperties();
  ~GmsProperties() override = default;


// MatchingStrategy interface

public:

  void reset() override;
  std::string name() const override;

// IGms interface

public:

  bool rotation() const override;
  void setRotation(bool rotation) override;
  bool scale() const override;
  void setScale(bool scale) override;
  double threshold() const override;
  void setThreshold(double threshold) override;

private:

  bool mRotation{gms_default_value_rotation};
  bool mScale{gms_default_value_scale};
  double mThreshold{gms_default_value_threshold};
};


/*----------------------------------------------------------------*/


class TL_EXPORT GsmImp
  : public GmsProperties,
    public MatchingAlgorithm
{

public:

  explicit GsmImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher);
  GsmImp(std::shared_ptr<DescriptorMatcher> descriptorMatcher,
         bool rotation,
         bool scale,
         double threshold);
  ~GsmImp() override = default;

// MatchingAlgorithm interface

public:

  bool compute(const cv::Mat &queryDescriptor,
               const cv::Mat &trainDescriptor,
               const std::vector<cv::KeyPoint> &keypoints1,
               const std::vector<cv::KeyPoint> &keypoints2,
               std::vector<cv::DMatch> *goodMatches,
               std::vector<cv::DMatch> *wrongMatches = nullptr,
               const cv::Size &queryImageSize = cv::Size(),
               const cv::Size &trainImageSize = cv::Size()) override;

private:

  std::shared_ptr<DescriptorMatcher> mDescriptorMatcher;

};

} // namespace tl

#endif // TL_FEATMATCH_GSM_H
