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

/*!
 * \brief LATCH feature descriptor properties.
 *
 * Stores configuration parameters for the LATCH binary descriptor.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"Bytes"`: Descriptor size in bytes (valid values: `"32"`, `"64"`, `"128"`).
 * - `"RotationInvariance"`: Enables or disables rotation invariance.
 * - `"HalfSsdSize"`: Radius for computing descriptor similarity.
 *
 * Reference:
 * Levi, G., & Hassner, T. (2016). LATCH: Learned Arrangements of Three Patch Codes.
 * In: Winter Conference on Applications of Computer Vision (WACV).
 */
class TL_EXPORT LatchProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes LATCH properties with default values.
     */
    LatchProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `LatchProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    LatchProperties(const LatchProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `LatchProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    LatchProperties(LatchProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~LatchProperties() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `LatchProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const LatchProperties &properties) -> LatchProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `LatchProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(LatchProperties &&properties) TL_NOEXCEPT -> LatchProperties &;

    /*!
     * \brief Gets the descriptor size in bytes.
     *
     * \return Descriptor size as a string (`"32"`, `"64"`, `"128"`).
     *
     * \note Property name: `"Bytes"`.
     */
    auto bytes() const->std::string;

    /*!
     * \brief Checks if rotation invariance is enabled.
     *
     * \return `true` if rotation invariance is enabled, `false` otherwise.
     *
     * \note Property name: `"RotationInvariance"`.
     */
    auto rotationInvariance() const -> bool;

    /*!
     * \brief Gets the Half SSD (Sum of Squared Differences) size.
     *
     * \return The Half SSD size (radius for descriptor similarity computation).
     *
     * \note Property name: `"HalfSsdSize"`.
     */
    auto halfSsdSize() const -> int;

    /*!
     * \brief Sets the descriptor size in bytes.
     *
     * \param[in] bytes Descriptor size as a string (`"32"`, `"64"`, `"128"`).
     */
    void setBytes(const std::string &bytes);

    /*!
     * \brief Enables or disables rotation invariance.
     *
     * \param[in] rotationInvariance `true` to enable, `false` to disable.
     */
    void setRotationInvariance(bool rotationInvariance);

    /*!
     * \brief Sets the Half SSD (Sum of Squared Differences) size.
     *
     * \param[in] halfSsdSize The Half SSD size (radius for descriptor similarity computation).
     */
    void setHalfSsdSize(int halfSsdSize);

// Feature interface

public:

    /*!
     * \brief Resets LATCH properties to their default values.
     */
    void reset() override;

};


/*!
 * \brief LATCH binary descriptor.
 *
 * Implements the LATCH descriptor for keypoint feature description.
 * LATCH is a binary descriptor that uses learned patch arrangements for robust matching.
 *
 * \note This class requires OpenCV's `xfeatures2d` module.
 */
class TL_EXPORT LatchDescriptor
  : public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::LATCH> mLATCH;
#endif // HAVE_OPENCV_XFEATURES2D
    LatchProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the LATCH descriptor with default properties.
     */
    LatchDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes LATCH using the provided properties.
     *
     * \param[in] properties Configuration properties for LATCH.
     */
    explicit LatchDescriptor(const LatchProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `LatchDescriptor` instance.
     *
     * \param[in] latch The object to copy.
     */
    LatchDescriptor(const LatchDescriptor &latch);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `LatchDescriptor` instance.
     *
     * \param[in] latch The object to move.
     */
    LatchDescriptor(LatchDescriptor &&latch) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~LatchDescriptor() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `LatchDescriptor` instance.
     *
     * \param[in] latch The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const LatchDescriptor &latch) -> LatchDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `LatchDescriptor` instance.
     *
     * \param[in] latch The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(LatchDescriptor &&latch) TL_NOEXCEPT -> LatchDescriptor &;

    /*!
     * \brief Get a reference to the LATCH descriptor properties.
     *
     * This allows modifying the properties of the LATCH descriptor.
     *
     * \return Reference to the `LatchProperties` object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"Bytes"`
     * - `"RotationInvariance"`
     * - `"HalfSsdSize"`
     */
    auto properties() -> LatchProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the LATCH descriptor properties.
     *
     * This allows read-only access to the properties of the LATCH descriptor.
     *
     * \return Constant reference to the `LatchProperties` object.
     */
    auto properties() const -> const LatchProperties &{ return mProperties; }


private:

    void init();

 // FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the LATCH descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
