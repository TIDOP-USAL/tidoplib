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

#include "tidop/featmatch/matching/test/crosstest.h"

namespace tl
{

auto crossCheckTest(const std::vector<std::vector<cv::DMatch>> &matches12, 
                    const std::vector<std::vector<cv::DMatch>> &matches21,
                    std::vector<cv::DMatch> *wrongMatches) -> std::vector<cv::DMatch>
{
    std::vector<cv::DMatch> goodMatches;

    for (size_t i = 0; i < matches12.size(); i++) {

        if (matches12[i].empty() || matches12[i].size() < 2)
            continue;

        bool findGoodMatch = false;
        for (size_t j = 0; j < matches21.size(); j++) {

            if (matches21[j].empty() || matches21[j].size() < 2)
                continue;

            if (matches12[i][0].queryIdx == matches21[j][0].trainIdx &&
                matches21[j][0].queryIdx == matches12[i][0].trainIdx) {
                goodMatches.push_back(matches12[i][0]);
                findGoodMatch = true;
                break;
            }

        }

        if (findGoodMatch == false && wrongMatches)
            wrongMatches->push_back(matches12[i][0]);

    }

    return goodMatches;
}

} // namespace tl
