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

#include "gsm.h"

#include <utility>

#include "tidop/core/base/exception.h"

namespace tl
{

GmsProperties::GmsProperties()
  : MatchingStrategy("GMS", Strategy::gms)
{
    reset();
}

GmsProperties::GmsProperties(const GmsProperties &properties)
    : MatchingStrategy(properties)
{
}

GmsProperties::GmsProperties(GmsProperties &&properties) TL_NOEXCEPT
    : MatchingStrategy(std::forward<MatchingStrategy>(properties))
{
}

auto GmsProperties::operator=(const GmsProperties &properties) -> GmsProperties &
{
    if (this != &properties) {
        MatchingStrategy::operator=(properties);
    }
    return *this;
}

auto GmsProperties::operator=(GmsProperties &&properties) TL_NOEXCEPT -> GmsProperties &
{
    if (this != &properties) {
        MatchingStrategy::operator=(std::forward<MatchingStrategy>(properties));
    }
    return *this;
}
auto GmsProperties::rotation() const -> bool
{
    return getProperty<bool>("Rotation");
}

void GmsProperties::setRotation(bool rotation)
{
    setProperty("Rotation", rotation);
}

auto GmsProperties::scale() const -> bool
{
    return getProperty<bool>("Scale");
}

void GmsProperties::setScale(bool scale)
{
    setProperty("Scale", scale);
}

auto GmsProperties::threshold() const -> double
{
    return getProperty<double>("Threshold");
}

void GmsProperties::setThreshold(double threshold)
{
    setProperty("Threshold", threshold);
}

void GmsProperties::reset()
{
    setRotation(gms_default_value_rotation);
    setScale(gms_default_value_scale);
    setThreshold(gms_default_value_threshold);
}





Gms::Gms(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher)
  : mDescriptorMatcher(descriptorMatcher)
{
}

Gms::Gms(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher,
         const GmsProperties &properties)
  : mDescriptorMatcher(descriptorMatcher),
    mProperties(properties)
{
}

auto Gms::compute(const cv::Mat &queryDescriptor,
                  const cv::Mat &trainDescriptor,
                  const std::vector<cv::KeyPoint> &keypoints1,
                  const std::vector<cv::KeyPoint> &keypoints2,
                  std::vector<cv::DMatch> *wrongMatches,
                  const cv::Size &queryImageSize,
                  const cv::Size &trainImageSize) -> std::vector<cv::DMatch>
{
    std::vector<cv::DMatch> goodMatches;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D

#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR >= 4 && CV_VERSION_REVISION >= 1 )

        std::vector<cv::DMatch> matches;
        mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches);

        cv::xfeatures2d::matchGMS(queryImageSize, trainImageSize, keypoints1, keypoints2, matches, goodMatches);

        for (size_t i = 0; i < matches.size(); i++) {
            bool wrong = true;
            for (size_t j = 0; j < goodMatches.size(); j++) {
                if (matches[i].queryIdx == goodMatches[j].queryIdx &&
                    matches[i].trainIdx == goodMatches[j].trainIdx) {
                    wrong = false;
                    break;
                }
            }
            if (wrong) {
                wrongMatches->push_back(matches[i]);
            }
        }

#  else
        TL_COMPILER_WARNING("'matchGMS' not supported in OpenCV versions < 3.3.1")
        throw TL_ERROR("'matchGMS' not supported in OpenCV versions < 3.3.1");
#endif
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. 'matchGMS' not supported")
        throw TL_ERROR("OpenCV not built with extra modules. 'matchGMS' not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return goodMatches;
}

} // namespace tl
