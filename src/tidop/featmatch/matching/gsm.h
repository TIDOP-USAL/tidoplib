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

#include "tidop/featmatch/matching/matching.h"

namespace tl
{

/*! \addtogroup FeatureMatching
 *
 *  \{
 */

constexpr auto gms_default_value_rotation{true};
constexpr auto gms_default_value_scale{true};
constexpr auto gms_default_value_threshold{6.};


class TL_EXPORT GmsProperties
  : public MatchingStrategy
{

public:

    /*!
     * \brief Default constructor. Initializes the properties with default values.
     */
    GmsProperties();

    /*!
     * \brief Copy constructor.
     * \param[in] properties Another instance to copy properties from.
     */
    GmsProperties(const GmsProperties &properties);

    /*!
     * \brief Move constructor.
     * \param[in] properties Another instance to move properties from.
     */
    GmsProperties(GmsProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~GmsProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] properties Another instance to copy properties from.
     * \return Reference to this instance.
     */
    auto operator=(const GmsProperties &properties) -> GmsProperties &;

    /*!
     * \brief Move assignment operator.
     * \param[in] properties Another instance to move properties from.
     * \return Reference to this instance.
     */
    auto operator=(GmsProperties &&properties) TL_NOEXCEPT -> GmsProperties &;

    auto rotation() const -> bool;
    void setRotation(bool rotation);
    auto scale() const -> bool;
    void setScale(bool scale);
    auto threshold() const -> double;
    void setThreshold(double threshold);

// MatchingStrategy interface

public:

    void reset() override;

};





class TL_EXPORT Gms
  : public MatchingAlgorithm
{

private:

    std::shared_ptr<DescriptorMatcher> mDescriptorMatcher;
    GmsProperties mProperties;

public:

    explicit Gms(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher);
    Gms(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher,
        const GmsProperties &properties);
    ~Gms() override = default;

    /*!
     * \brief Get the GMS properties.
     * \return Reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Rotation"`
     * - `"Scale"`
     * - `"Threshold"`
     */
    auto properties() -> GmsProperties &{ return mProperties; }

    /*!
     * \brief Get the GMS properties (const version).
     * \return Const reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Rotation"`
     * - `"Scale"`
     * - `"Threshold"`
     */
    auto properties() const -> const GmsProperties &{ return mProperties; }

// MatchingAlgorithm interface

public:

    auto compute(const cv::Mat &queryDescriptor,
                 const cv::Mat &trainDescriptor,
                 const std::vector<cv::KeyPoint> &keypoints1,
                 const std::vector<cv::KeyPoint> &keypoints2,
                 std::vector<cv::DMatch> *wrongMatches = nullptr,
                 const cv::Size &queryImageSize = cv::Size(),
                 const cv::Size &trainImageSize = cv::Size()) -> std::vector<cv::DMatch> override;

};

/*! \} */

} // namespace tl

