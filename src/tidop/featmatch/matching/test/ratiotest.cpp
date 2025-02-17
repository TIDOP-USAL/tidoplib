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

#include "tidop/featmatch/matching/test/ratiotest.h"

namespace tl
{

auto nearestNeighbourDistanceRatioTest(const std::vector<std::vector<cv::DMatch>> &matches,
                                       double ratio, 
                                       std::vector<std::vector<cv::DMatch>> *wrongMatches) -> std::vector<std::vector<cv::DMatch>>
{
    std::vector<std::vector<cv::DMatch>> goodMatches;

    for (size_t i = 0; i < matches.size(); i++) {

        if (matches[i].size() > 1) {
            // check distance ratio
            if (matches[i][0].distance / matches[i][1].distance <= static_cast<float>(ratio)) {
                goodMatches.push_back(matches[i]);
            } else {
                if (wrongMatches) {
                    wrongMatches->push_back(matches[i]);
                }
            }
        }

    }

    return goodMatches;
}

} // namespace tl
