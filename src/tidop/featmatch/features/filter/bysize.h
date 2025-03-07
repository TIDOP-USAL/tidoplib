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
 * \brief Properties for filtering keypoints based on their size.
 *
 * This class stores the parameters used to filter keypoints based on their size.
 * It allows setting and retrieving the minimum and maximum allowed keypoint sizes.
 */
class TL_EXPORT KeyPointsFilterBySizeProperties
  : public KeyPointsFilterProperties
{

private:

    double mMinSize;
    double mMaxSize;

public:

    KeyPointsFilterBySizeProperties();
    ~KeyPointsFilterBySizeProperties() override = default;

    /*!
     * \brief Get the minimum size for keypoints.
     * \return The minimum allowed keypoint size.
     */
    virtual auto minSize() const -> double;

    /*!
     * \brief Set the minimum size for keypoints.
     * \param[in] minSize The minimum allowed keypoint size.
     */
    virtual void setMinSize(double minSize);

    /*!
     * \brief Get the maximum size for keypoints.
     * \return The maximum allowed keypoint size.
     */
    virtual auto maxSize() const -> double;

    /*!
     * \brief Set the maximum size for keypoints.
     * \param[in] maxSize The maximum allowed keypoint size.
     */
    virtual void setMaxSize(double maxSize);

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
 * \brief Filters keypoints based on their size.
 *
 * This class removes keypoints that are outside a specified size range.
 * It ensures that only keypoints within the defined minimum and maximum size limits
 * are retained.
 */
class TL_EXPORT KeyPointsFilterBySize
  : public KeyPointsFilter
{

private:

    KeyPointsFilterBySizeProperties mProperties;

public:

    /*!
     * \brief Default constructor. Initializes the filter with default size limits.
     */
    KeyPointsFilterBySize();

    /*!
     * \brief Constructor with size limits.
     * \param[in] minSize Minimum allowed keypoint size.
     * \param[in] maxSize Maximum allowed keypoint size.
     */
    KeyPointsFilterBySize(double minSize, double maxSize);

    /*!
     * \brief Destructor.
     */
    ~KeyPointsFilterBySize() override = default;

// KeyPointsFilter interface

public:

    /*!
     * \brief Filters keypoints based on their size.
     * \param[in] keypoints The original set of keypoints.
     * \return A vector of keypoints that are within the specified size range.
     */
    auto filter(const std::vector<cv::KeyPoint> &keypoints) -> std::vector<cv::KeyPoint> override;

};

/*! \} */

} // namespace tl
