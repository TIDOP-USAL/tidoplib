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

#include "tidop/featmatch/matcher.h"

namespace tl
{

class TL_EXPORT FlannMatcherProperties
  : public FlannMatcher
{

private:

    Index mIndex;

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

};


/*----------------------------------------------------------------*/


class TL_EXPORT FlannMatcherImp
  : public FlannMatcherProperties,
    public DescriptorMatcher
{

private:

    cv::Ptr<cv::FlannBasedMatcher> mFlannBasedMatcher;

public:

    FlannMatcherImp();
    explicit FlannMatcherImp(FlannMatcher::Index index);
    ~FlannMatcherImp() override = default;

private:

    void update();

// DescriptorMatcher interface

public:

    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<cv::DMatch> &matches,
               const cv::Mat mask = cv::Mat()) override;

    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<std::vector<cv::DMatch>> &matches,
               const cv::Mat mask = cv::Mat()) override;

// Match interface

public:

    void reset() override;

// FlannMatcher interface

public:

    void setIndex(Index index) override;

};

} // namespace tl
