/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_FEATMATCH_BRUTE_FORCE_MATCHER_H
#define TL_FEATMATCH_BRUTE_FORCE_MATCHER_H

#include "tidop/featmatch/matcher.h"

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

#include <vector>

namespace tl
{

class TL_EXPORT BruteForceMatcherProperties
  : public BruteForceMatcher
{

public:

  BruteForceMatcherProperties();
  ~BruteForceMatcherProperties() override = default;

// Match interface

public:

  void reset() override;
  std::string name() const final;

// BruteForceMatcher interface

public:

  Norm normType() const override;
  void setNormType(Norm normType) override;

private:

  Norm mNormType;

};


/*----------------------------------------------------------------*/


class TL_EXPORT BruteForceMatcherImp
  : public BruteForceMatcherProperties,
    public DescriptorMatcher
{

public:

  BruteForceMatcherImp();
  explicit BruteForceMatcherImp(Norm normType);
  ~BruteForceMatcherImp() override = default;

private:

  void update();

// DescriptorMatcher interface

public:

  bool match(cv::InputArray &queryDescriptors,
             cv::InputArray &trainDescriptors,
             std::vector<cv::DMatch> &matches,
             cv::InputArray mask = cv::noArray()) override;

  bool match(cv::InputArray &queryDescriptors,
             cv::InputArray &trainDescriptors,
             std::vector<std::vector<cv::DMatch>> &matches,
             cv::InputArray mask = cv::noArray()) override;

// Match interface

public:

  void reset() override;

// BruteForceMatcher interface

public:

  void setNormType(Norm normType) override;

protected:

  cv::Ptr<cv::BFMatcher> mBFMatcher;
};


/*----------------------------------------------------------------*/


#ifdef HAVE_OPENCV_CUDAFEATURES2D

class TL_EXPORT BruteForceMatcherCuda
  : public BruteForceMatcherProperties,
    public DescriptorMatcher
{

public:

  BruteForceMatcherCuda();
  explicit BruteForceMatcherCuda(Norm normType);
  ~BruteForceMatcherCuda() override = default;

private:

  void update();

// DescriptorMatcher interface

public:

  bool match(cv::InputArray &queryDescriptors,
             cv::InputArray &trainDescriptors,
             std::vector<cv::DMatch> &matches,
             cv::InputArray mask = cv::noArray()) override;

  bool match(cv::InputArray &queryDescriptors,
             cv::InputArray &trainDescriptors,
             std::vector<std::vector<cv::DMatch>> &matches,
             cv::InputArray mask = cv::noArray()) override;

// Match interface

public:

  void reset() override;

// IBruteForceMatcher interface

public:

  void setNormType(Norm normType) override;

protected:

  cv::Ptr<cv::cuda::DescriptorMatcher> mBFMatcher;
};

#endif // HAVE_OPENCV_CUDAFEATURES2D

} // namespace tl

#endif // TL_FEATMATCH_BRUTE_FORCE_MATCHER_H
