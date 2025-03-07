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

#include "tidop/featmatch/features/filter/keypointsfilter.h"

namespace tl
{

/*! \addtogroup KeyPointsFilters
 *
 *  \{
 */


/*!
 * \class KeyPointsFilterRemoveDuplicated
 * \brief A keypoint filter that removes duplicate keypoints.
 *
 * This filter eliminates duplicate keypoints based on their position.
 * It is useful when keypoint detection algorithms produce redundant keypoints
 * that could affect further processing steps, such as feature matching.
 */
class TL_EXPORT KeyPointsFilterRemoveDuplicated
  : public KeyPointsFilter
{

public:

    KeyPointsFilterRemoveDuplicated();
    ~KeyPointsFilterRemoveDuplicated() override = default;

// KeyPointsFilter interface

public:

    /*!
     * \brief Filters out duplicate keypoints.
     * \param[in] keypoints The original set of keypoints.
     * \return A vector containing only unique keypoints.
     *
     * This function removes keypoints that are too close to each other in position.
     * It ensures that only one keypoint is kept per distinct location.
     */
    auto filter(const std::vector<cv::KeyPoint> &keypoints) -> std::vector<cv::KeyPoint> override;

};

/*! \} */

} // namespace tl
