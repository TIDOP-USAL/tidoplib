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

#include "keypointsfilter.h"

#include "tidop/core/base/exception.h"

namespace tl
{


KeyPointsFilterNBestProperties::KeyPointsFilterNBestProperties()
  : KeyPointsFilterBase(Type::n_best),
    mPointsNumber(5000)
{
}

int KeyPointsFilterNBestProperties::nPoints() const
{
    return mPointsNumber;
}

void KeyPointsFilterNBestProperties::setNPoints(int nPoints)
{
    mPointsNumber = nPoints;
}

void KeyPointsFilterNBestProperties::reset()
{
    mPointsNumber = 5000;
}

auto KeyPointsFilterNBestProperties::name() const -> std::string
{
    return std::string("N Best");
}



/*----------------------------------------------------------------*/



KeyPointsFilterNBest::KeyPointsFilterNBest() = default;

KeyPointsFilterNBest::KeyPointsFilterNBest(int nPoints)
{
    KeyPointsFilterNBestProperties::setNPoints(nPoints);
}

auto KeyPointsFilterNBest::filter(const std::vector<cv::KeyPoint>& keypoints) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> filteredKeypoints = keypoints;

    try {
        size_t n_points = static_cast<size_t>(KeyPointsFilterNBestProperties::nPoints());

        if (filteredKeypoints.size() > n_points) {
            cv::KeyPointsFilter::retainBest(filteredKeypoints,
                                            KeyPointsFilterNBestProperties::nPoints());

            if (filteredKeypoints.size() != n_points) {
                /// Los keypoints no tienen 'response'. Por ahora...
                filteredKeypoints.resize(static_cast<size_t>(KeyPointsFilterNBestProperties::nPoints()));
            }
        }

        Message::info("Filtered retaining {} best keypoints", filteredKeypoints.size());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return filteredKeypoints;
}

void KeyPointsFilterNBest::reset()
{
    KeyPointsFilterNBestProperties::reset();
}


/*----------------------------------------------------------------*/



KeyPointsFilterBySizeProperties::KeyPointsFilterBySizeProperties()
    : KeyPointsFilterBase(KeyPointsFilter::Type::size),
    mMinSize(0.),
    mMaxSize(TL_DOUBLE_MAX)
{
}

double KeyPointsFilterBySizeProperties::minSize() const
{
    return mMinSize;
}

void KeyPointsFilterBySizeProperties::setMinSize(double minSize)
{
    mMinSize = minSize;
}

double KeyPointsFilterBySizeProperties::maxSize() const
{
    return mMaxSize;
}

void KeyPointsFilterBySizeProperties::setMaxSize(double maxSize)
{
    mMaxSize = maxSize;
}

void KeyPointsFilterBySizeProperties::reset()
{
    mMinSize = 0.;
    mMaxSize = TL_DOUBLE_MAX;
}

auto KeyPointsFilterBySizeProperties::name() const -> std::string
{
    return std::string("Filter By Size");
}


/*----------------------------------------------------------------*/


KeyPointsFilterBySize::KeyPointsFilterBySize() = default;

KeyPointsFilterBySize::KeyPointsFilterBySize(double minSize, double maxSize)
{
    KeyPointsFilterBySizeProperties::setMinSize(minSize);
    KeyPointsFilterBySizeProperties::setMaxSize(maxSize);
}

auto KeyPointsFilterBySize::filter(const std::vector<cv::KeyPoint>& keypoints) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> filteredKeypoints = keypoints;

    try {

        size_t size = keypoints.size();
        double min_size = KeyPointsFilterBySizeProperties::minSize();
        double max_size = KeyPointsFilterBySizeProperties::maxSize();
        cv::KeyPointsFilter::runByKeypointSize(filteredKeypoints,
                                               static_cast<float>(min_size),
                                               static_cast<float>(max_size));
        size_t new_size = filteredKeypoints.size();
        Message::info("Filtered keypoints by size (min={},max={}): {}", min_size, max_size, size - new_size);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return filteredKeypoints;
}

void KeyPointsFilterBySize::reset()
{
    KeyPointsFilterBySizeProperties::reset();
}



/*----------------------------------------------------------------*/


KeyPointsFilterRemoveDuplicatedProperties::KeyPointsFilterRemoveDuplicatedProperties()
    : KeyPointsFilterBase(Type::remove_duplicated)
{
}

void KeyPointsFilterRemoveDuplicatedProperties::reset()
{
}

std::string KeyPointsFilterRemoveDuplicatedProperties::name() const
{
    return std::string("Remove Duplicated");
}



/*----------------------------------------------------------------*/



KeyPointsFilterRemoveDuplicated::KeyPointsFilterRemoveDuplicated()
{

}

std::vector<cv::KeyPoint> KeyPointsFilterRemoveDuplicated::filter(const std::vector<cv::KeyPoint> &keypoints)
{
    std::vector<cv::KeyPoint> filteredKeypoints = keypoints;

    try {

        int size = static_cast<int>(keypoints.size());
        cv::KeyPointsFilter::removeDuplicated(filteredKeypoints);
        int new_size = static_cast<int>(filteredKeypoints.size());
        Message::info("Remove duplicated keypoints: {}", size - new_size);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return filteredKeypoints;
}

void KeyPointsFilterRemoveDuplicated::reset()
{
    KeyPointsFilterRemoveDuplicatedProperties::reset();
}



} // namespace tl
