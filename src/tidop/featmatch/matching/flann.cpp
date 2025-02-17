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

#include "flann.h"

#include "tidop/core/base/exception.h"

namespace tl
{

/* Flann matcher properties */

FlannMatcherProperties::FlannMatcherProperties()
  : MatchingMethod("Flann Based Matching", Type::flann)
{
    reset();
}

FlannMatcherProperties::FlannMatcherProperties(const FlannMatcherProperties &properties)
  : MatchingMethod(properties)
{
}

FlannMatcherProperties::FlannMatcherProperties(FlannMatcherProperties &&properties) TL_NOEXCEPT
  : MatchingMethod(std::forward<MatchingMethod>(properties))
{
}

FlannMatcherProperties::~FlannMatcherProperties() = default;

auto FlannMatcherProperties::operator=(const FlannMatcherProperties &properties) -> FlannMatcherProperties &
{
    if (this != &properties) {
        MatchingMethod::operator=(properties);
    }
    return *this;
}

auto FlannMatcherProperties::operator=(FlannMatcherProperties &&properties) TL_NOEXCEPT -> FlannMatcherProperties &
{
    if (this != &properties) {
        MatchingMethod::operator=(std::forward<MatchingMethod>(properties));
    }
    return *this;
}

void FlannMatcherProperties::reset()
{
    setIndex("kdtree");
}

auto FlannMatcherProperties::index() const -> std::string
{
    return getProperty<std::string>("Index");
}

void FlannMatcherProperties::setIndex(const std::string &index)
{
    if (index == "kdtree" ||
        index == "lsh") {
        setProperty("Index", index);
    } else {
        Message::warning("'Index' value not valid: {}", index);
    }
}



/* Flann matcher */

FlannMatcher::FlannMatcher()
  : mProperties() 
{
    init();
}

FlannMatcher::FlannMatcher(const FlannMatcherProperties &properties)
  : mProperties(properties)
{
    init();
}

FlannMatcher::FlannMatcher(const FlannMatcher &flann)
  : mProperties(flann.mProperties)
{
    init();
}

FlannMatcher::FlannMatcher(FlannMatcher &&flann) TL_NOEXCEPT
  : mProperties(std::move(flann.mProperties)),
    mFlannBasedMatcher(std::move(flann.mFlannBasedMatcher))
{
}

auto FlannMatcher::operator =(const FlannMatcher &flann) -> FlannMatcher &
{
    if (this != &flann) {
        mProperties = flann.mProperties;
        init();
    }
    return *this;
}

auto FlannMatcher::operator =(FlannMatcher &&flann) TL_NOEXCEPT -> FlannMatcher &
{
    if (this != &flann) {
        mProperties = std::move(flann.mProperties);
        mFlannBasedMatcher = std::move(flann.mFlannBasedMatcher);
    }
    return *this;
}

void FlannMatcher::init()
{
    cv::Ptr<cv::flann::IndexParams> indexParams;
    if (mProperties.index() == "kdtree") {
        indexParams = cv::makePtr<cv::flann::KDTreeIndexParams>();
    } else {
        indexParams = cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2);
    }
    mFlannBasedMatcher = cv::Ptr<cv::FlannBasedMatcher>(new cv::FlannBasedMatcher(indexParams));
}

void FlannMatcher::match(const cv::Mat &queryDescriptors,
                         const cv::Mat &trainDescriptors,
                         std::vector<cv::DMatch> &matches,
                         const cv::Mat mask)
{
    try {

        mFlannBasedMatcher->match(queryDescriptors, trainDescriptors, matches, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FlannMatcher::match(const cv::Mat &queryDescriptors,
                         const cv::Mat &trainDescriptors,
                         std::vector<std::vector<cv::DMatch>> &matches,
                         int k,
                         const cv::Mat mask)
{
    try {

        mFlannBasedMatcher->knnMatch(queryDescriptors, trainDescriptors, matches, k, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}


} // namespace tl
