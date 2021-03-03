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

#ifndef TL_FEATMATCH_FLANN_MATCHER_H
#define TL_FEATMATCH_FLANN_MATCHER_H

#include "tidop/featmatch/matcher.h"

namespace tl
{

class TL_EXPORT FlannMatcherProperties
  : public FlannMatcher
{

public:

  FlannMatcherProperties();
  ~FlannMatcherProperties() override;

// Match interface

public:

  void reset() override;
  std::string name() const final;

// FlannMatcher interface

public:

  Index index() const override;
  virtual void setIndex(Index index) override;

private:

  Index mIndex;
};


/*----------------------------------------------------------------*/


class TL_EXPORT FlannMatcherImp
  : public FlannMatcherProperties,
    public DescriptorMatcher
{

public:

  FlannMatcherImp();
  explicit FlannMatcherImp(FlannMatcher::Index index);
  ~FlannMatcherImp() override = default;

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

// FlannMatcher interface

public:

  void setIndex(Index index) override;

protected:

  cv::Ptr<cv::FlannBasedMatcher> mFlannBasedMatcher;

};

} // namespace tl

#endif // TL_FEATMATCH_FLANN_MATCHER_H
