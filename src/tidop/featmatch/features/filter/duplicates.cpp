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

#include "tidop/featmatch/features/filter/duplicates.h"

#include "tidop/core/base/exception.h"

namespace tl
{


KeyPointsFilterRemoveDuplicated::KeyPointsFilterRemoveDuplicated()
{

}

auto KeyPointsFilterRemoveDuplicated::filter(const std::vector<cv::KeyPoint> &keypoints) -> std::vector<cv::KeyPoint>
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



} // namespace tl
