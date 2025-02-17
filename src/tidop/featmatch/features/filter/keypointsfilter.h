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

#include <vector>

#include <opencv2/features2d.hpp>

#include <tidop/core/base/flags.h>

namespace tl
{

/*! \addtogroup KeyPointsFilters
 *
 *  \{
 */

/*!
 * \class KeyPointsFilterProperties
 * \brief Base class for defining properties of keypoint filtering.
 *
 * This class provides an interface for different keypoint filtering techniques,
 * such as selecting the N best keypoints, filtering by size, removing duplicates, or using a mask.
 */
class TL_EXPORT KeyPointsFilterProperties
{

public:

    /*!
     * \enum Type
     * \brief Enumeration of keypoint filtering methods.
     */
    enum class Type
    {
        n_best,           /*!< Selects the N best keypoints based on response value. */
        size,             /*!< Filters keypoints based on their size (scale). */
        remove_duplicated,/*!< Removes duplicate keypoints. */
        mask              /*!< Filters keypoints using a predefined mask. */
    };
	
protected:

	EnumFlags<Type> mFilterType;
	
public:

    /*!
     * \brief Constructor for setting the filter type.
     * \param[in] type The type of filtering to be applied.
     */
    KeyPointsFilterProperties(Type type) : mFilterType(type) {}

    /*!
     * \brief Virtual destructor.
     */
    virtual ~KeyPointsFilterProperties() = default;

    /*!
     * \brief Resets the filter properties to their default values.
     */
    virtual void reset() = 0;

    /*!
     * \brief Gets the type of keypoint filter.
     * \return The filter type.
     */
    auto type() const -> Type
    {
        return mFilterType.flags();
    }

    /*!
     * \brief Gets the name of the filter type.
     * \return A string representing the filter name.
     */
    virtual auto name() const -> std::string = 0;

};
ALLOW_BITWISE_FLAG_OPERATIONS(KeyPointsFilterProperties::Type)



/*!
 * \class KeyPointsFilter
 * \brief Abstract class for filtering keypoints.
 *
 * This class defines an interface for filtering keypoints based on various criteria.
 * Derived classes must implement the `filter` method.
 */
class TL_EXPORT KeyPointsFilter
{

public:

    KeyPointsFilter() {}
    virtual ~KeyPointsFilter() = default;

    /*!
     * \brief Filters the given keypoints according to the filter properties.
     * \param[in] keypoints The original set of keypoints.
     * \return A vector of filtered keypoints.
     *
     * This method must be implemented by derived classes to apply specific filtering criteria.
     */
    virtual auto filter(const std::vector<cv::KeyPoint> &keypoints) -> std::vector<cv::KeyPoint> = 0;

};


/*! \} */

} // namespace tl
