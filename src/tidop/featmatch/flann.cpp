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

#include "tidop/core/exception.h"

namespace tl
{

FlannMatcherProperties::FlannMatcherProperties()
  : mIndex(Index::kdtree)
{
}

FlannMatcherProperties::~FlannMatcherProperties() = default;

void FlannMatcherProperties::reset()
{
    mIndex = FlannMatcherProperties::Index::kdtree;
}

auto FlannMatcherProperties::name() const -> std::string
{
    return std::string("Flann Based Matching");
}

auto FlannMatcherProperties::index() const -> FlannMatcher::Index
{
    return mIndex;
}

void FlannMatcherProperties::setIndex(FlannMatcher::Index index)
{
    mIndex = index;
}


/*----------------------------------------------------------------*/


FlannMatcherImp::FlannMatcherImp()
{
    update();
}

FlannMatcherImp::FlannMatcherImp(Index index)
{
    FlannMatcherProperties::setIndex(index);
    update();
}

void FlannMatcherImp::update()
{
    cv::Ptr<cv::flann::IndexParams> indexParams;
    if (FlannMatcherProperties::index() == FlannMatcherProperties::Index::kdtree) {
        indexParams = cv::makePtr<cv::flann::KDTreeIndexParams>();
    } else if (FlannMatcherProperties::index() == FlannMatcherProperties::Index::lsh) {
        indexParams = cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2);
    }
    mFlannBasedMatcher = cv::Ptr<cv::FlannBasedMatcher>(new cv::FlannBasedMatcher(indexParams));
}

void FlannMatcherImp::match(const cv::Mat &queryDescriptors,
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

void FlannMatcherImp::match(const cv::Mat &queryDescriptors,
                            const cv::Mat &trainDescriptors,
                            std::vector<std::vector<cv::DMatch>> &matches,
                            const cv::Mat mask)
{
    try {

        mFlannBasedMatcher->knnMatch(queryDescriptors, trainDescriptors, matches, 2, mask);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FlannMatcherImp::reset()
{
    FlannMatcherProperties::reset();
    update();
}

void FlannMatcherImp::setIndex(FlannMatcher::Index index)
{
    FlannMatcherProperties::setIndex(index);
}


} // namespace tl
