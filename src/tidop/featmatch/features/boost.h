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

#include "tidop/featmatch/features/features.h"

#ifdef HAVE_OPENCV_XFEATURES2D 
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D

namespace tl
{


/*! \addtogroup FeatureExtraction
 * 
 *  \{
 */


constexpr auto boost_default_value_use_orientation = true;
constexpr auto boost_default_value_scale_factor = 6.25f;
constexpr auto boost_default_value_descriptor_type{"BINBOOST_256"};


/*!
 * \brief %Properties for the Boost Descriptor
 *
 * This class stores configuration settings for the Boost Descriptor,
 * which is part of OpenCV's extended features (`xfeatures2d`).
 *
 * The Boost Descriptor provides efficient binary descriptors optimized for
 * fast and accurate keypoint matching. It supports various descriptor types,
 * including BINBOOST, BGM, and LBGM.
 * 
 * The following property names are valid when accessed via the `Properties` class:
 * - `"DescriptorType"`
 * - `"UseOrientation"`
 * - `"ScaleFactor"`
 *
 * \note Requires OpenCV with `xfeatures2d` enabled.
 */
class TL_EXPORT BoostProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes BOOST properties with default values.
     */
    BoostProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the configuration properties from another `BoostProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    BoostProperties(const BoostProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `BoostProperties` instance.
     *
     * \param[in] boostProperties The object to move.
     */
    BoostProperties(BoostProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BoostProperties() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `BoostProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const BoostProperties &properties) -> BoostProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `BoostProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(BoostProperties &&properties) TL_NOEXCEPT -> BoostProperties &;

    /*!
     * \brief Get the type of descriptor used.
     *
     * Supported descriptor types:
     * - BINBOOST_256 (default)
     * - BGM
     * - BGM_HARD
     * - BGM_BILINEAR
     * - LBGM
     * - BINBOOST_64
     * - BINBOOST_128
     * - BINBOOST_256
     *
     * \return Descriptor type as a string.
     *
     * \note Property name: `"DescriptorType"`.
     */
    auto descriptorType() const -> std::string;

    /*!
     * \brief Check if keypoint orientation is used for sampling patterns.
     *
     * \return `true` if keypoints orientation is used; otherwise, `false`.
     *
     * \note Property name: `"UseOrientation"`.
     */
    auto useOrientation() const -> bool;

    /*!
     * \brief Get the scale factor for adjusting the sampling window of detected keypoints.
     *
     * \return Scale factor.
     *
     * \note Property name: `"ScaleFactor"`.
     */
    auto scaleFactor() const -> float;

    /*!
     * \brief Set the descriptor type.
     *
     * \param[in] descriptorType Type of descriptor to use.
     */
    void setDescriptorType(const std::string &descriptorType);

    /*!
     * \brief Enable or disable keypoint orientation for sampling patterns.
     *
     * \param[in] useOrientation `true` to use keypoint orientation; otherwise, `false`.
     */
    void setUseOrientation(bool useOrientation);

    /*!
     * \brief Set the scale factor for adjusting the sampling window of detected keypoints.
     *
     * \param[in] scaleFactor Scale factor.
     */
    void setScaleFactor(float scaleFactor);

// Feature interface

public:

    /*!
     * \brief Reset BOOST properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief Boost Descriptor Extractor
 *
 * This class implements the Boost Descriptor, a binary descriptor designed
 * for efficient and accurate feature matching. It supports multiple descriptor
 * types, including BINBOOST and BGM variants.
 *
 * The descriptor provides a compact binary representation of image features,
 * making it suitable for real-time applications and large-scale feature matching.
 *
 * \note Requires OpenCV with `xfeatures2d` enabled.
 */
class TL_EXPORT BoostDescriptor
  : public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
    cv::Ptr<cv::xfeatures2d::BoostDesc> mBoost;
#endif
#endif // HAVE_OPENCV_XFEATURES2D
    BoostProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the Boost Descriptor with default properties.
     */
    BoostDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes the Boost Descriptor using the provided properties.
     *
     * \param[in] properties Configuration properties for the descriptor.
     */
    explicit BoostDescriptor(const BoostProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `BoostDescriptor` instance.
     *
     * \param[in] boostDescriptor The object to copy.
     */
    BoostDescriptor(const BoostDescriptor &boostDescriptor);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `BoostDescriptor` instance.
     *
     * \param[in] boostDescriptor The object to move.
     */
    BoostDescriptor(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BoostDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `BoostDescriptor` instance.
     *
     * \param[in] boostDescriptor The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const BoostDescriptor &boostDescriptor) -> BoostDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `BoostDescriptor` instance.
     *
     * \param[in] boostDescriptor The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(BoostDescriptor &&boostDescriptor) TL_NOEXCEPT -> BoostDescriptor &;

    /*!
     * \brief Get the Boost Descriptor properties.
     *
     * Provides access to the `BoostProperties` used by this detector.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> BoostProperties &{ return mProperties; }

    /*!
     * \brief Get the Boost Descriptor properties (const version).
     *
     * Provides read-only access to the `BoostProperties` used by this detector.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const BoostProperties &{ return mProperties; }

private:

    void update();

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the Boost Descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
