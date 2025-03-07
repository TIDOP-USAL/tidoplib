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
 * \brief Properties for filtering keypoints by retaining the N best ones.
 *
 * This class allows setting and retrieving the number of keypoints to retain.
 * It ensures that only the N strongest keypoints (based on response values) are kept.
 */
class TL_EXPORT KeyPointsFilterNBestProperties
  : public KeyPointsFilterProperties
{

private:

    int mPointsNumber;

public:

    KeyPointsFilterNBestProperties();
    ~KeyPointsFilterNBestProperties() override = default;

    /*!
     * \brief Get the number of keypoints to retain.
     * \return The number of keypoints to keep.
     */
    virtual auto nPoints() const -> int;

    /*!
     * \brief Set the number of keypoints to retain.
     * \param[in] nPoints The number of keypoints to keep.
     */
    virtual void setNPoints(int nPoints);

// KeyPointsFilter interface

public:

    /*!
     * \brief Resets the properties to their default values.
     */
    void reset();

    /*!
     * \brief Get the name of the filter.
     * \return The filter name as a string.
     */
    auto name() const -> std::string;

};




/*!
 * \class KeyPointsFilterNBest
 * \brief Filters keypoints by selecting the N best ones based on response.
 *
 * This filter sorts the keypoints by their response value and retains only the top N.
 */
class TL_EXPORT KeyPointsFilterNBest
  : public KeyPointsFilter
{

private:

    KeyPointsFilterNBestProperties mProperties;

public:

    /*!
     * \brief Default constructor. Initializes the filter with default parameters.
     */
    KeyPointsFilterNBest();

    /*!
     * \brief Constructor with a specified number of keypoints.
     * \param[in] nPoints The number of keypoints to retain.
     */
    explicit KeyPointsFilterNBest(int nPoints);

    /*!
     * \brief Destructor.
     */
    ~KeyPointsFilterNBest() override = default;

// KeyPointsFilterProcess interface

public:

    /*!
     * \brief Filters keypoints by selecting the N best ones.
     * \param[in] keypoints The original set of keypoints.
     * \return A vector of the N best keypoints.
     */
    auto filter(const std::vector<cv::KeyPoint> &keypoints) -> std::vector<cv::KeyPoint> override;

};

/*! \} */

} // namespace tl
