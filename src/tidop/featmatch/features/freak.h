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

#include "tidop/featmatch/base/features.h"

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
 * \brief FREAK (Fast Retina Keypoint) properties class.
 *
 * Implements the configuration properties for the FREAK descriptor, 
 * as described in the following paper:
 *
 * Alexandre Alahi, Raphael Ortiz, and Pierre Vandergheynst.
 * "FREAK: Fast Retina Keypoint." 
 * In *Computer Vision and Pattern Recognition (CVPR)*, 
 * 2012 IEEE Conference on, pages 510–517. IEEE, 2012.
 */
class TL_EXPORT FreakProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes FREAK properties with default values.
     */
    FreakProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `FreakProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    FreakProperties(const FreakProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `FreakProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    FreakProperties(FreakProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~FreakProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `FreakProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const FreakProperties &properties) -> FreakProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `FreakProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(FreakProperties &&properties) TL_NOEXCEPT -> FreakProperties &;

    /*!
     * \brief Checks if orientation normalization is enabled.
     *
     * Orientation normalization ensures that the descriptor is invariant to rotation.
     *
     * \return `true` if orientation normalization is enabled, `false` otherwise. (Default = `true`)
     *
     * \note The corresponding property name when accessed via `Properties` class is `"OrientationNormalized"`.
     */
    auto orientationNormalized() const -> bool;

    /*!
     * \brief Checks if scale normalization is enabled.
     *
     * Scale normalization ensures that the descriptor is invariant to changes in scale.
     *
     * \return `true` if scale normalization is enabled, `false` otherwise. (Default = `true`)
     *
     * \note The corresponding property name when accessed via `Properties` class is `"ScaleNormalized"`.
     */
    auto scaleNormalized() const -> bool;

    /*!
     * \brief Gets the scaling factor of the descriptor pattern.
     *
     * Controls the size of the sampling pattern used to generate the descriptor.
     *
     * \return The pattern scale factor. (Default = `22.0`)
     *
     * \note The corresponding property name when accessed via `Properties` class is `"PatternScale"`.
     */
    auto patternScale() const -> float;

    /*!
     * \brief Gets the number of octaves covered by detected keypoints.
     *
     * Higher values allow detecting keypoints at multiple scales.
     *
     * \return Number of octaves. (Default = `4`)
     *
     * \note The corresponding property name when accessed via `Properties` class is `"Octaves"`.
     */
    auto octaves() const -> int;

    /*!
     * \brief Enables or disables orientation normalization.
     *
     * \param[in] orientationNormalized `true` to enable, `false` to disable.
     */
    void setOrientationNormalized(bool orientationNormalized);

    /*!
     * \brief Enables or disables scale normalization.
     *
     * \param[in] scaleNormalized `true` to enable, `false` to disable.
     */
    void setScaleNormalized(bool scaleNormalized);

    /*!
     * \brief Sets the scaling factor of the descriptor pattern.
     *
     * \param[in] patternScale The new pattern scale value.
     */
    void setPatternScale(float patternScale);

    /*!
     * \brief Sets the number of octaves covered by detected keypoints.
     *
     * \param[in] octaves Number of octaves.
     */
    void setOctaves(int octaves);

// Feature interface

public:

    /*!
     * \brief Resets FREAK properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief FREAK descriptor extractor.
 *
 * Implements the FREAK (Fast Retina Keypoint) descriptor,
 * a binary descriptor inspired by the human visual system.
 *
 * This descriptor is computationally efficient, requires low memory,
 * and is robust against various image transformations.
 */
class TL_EXPORT FreakDescriptor
  : public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::FREAK> mFREAK;
#endif // HAVE_OPENCV_XFEATURES2D
    FreakProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the FREAK descriptor with default properties.
     */
    FreakDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes FREAK using the provided properties.
     *
     * \param[in] properties Configuration properties for FREAK.
     */
    explicit FreakDescriptor(const FreakProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `FreakDescriptor` instance.
     *
     * \param[in] freak The object to copy.
     */
    FreakDescriptor(const FreakDescriptor &freak);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `FreakDescriptor` instance.
     *
     * \param[in] freak The object to move.
     */
    FreakDescriptor(FreakDescriptor &&freak) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~FreakDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `FreakDescriptor` instance.
     *
     * \param[in] freak The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const FreakDescriptor &freak) -> FreakDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `FreakDescriptor` instance.
     *
     * \param[in] freak The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(FreakDescriptor &&freak) TL_NOEXCEPT -> FreakDescriptor &;

    /*!
     * \brief Get a reference to the FREAK descriptor properties.
     *
     * This allows modifying the properties of the descriptor.
     *
     * \return Reference to the `FreakProperties` object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"OrientationNormalized"`: Enables or disables orientation normalization.
     * - `"ScaleNormalized"`: Enables or disables scale normalization.
     * - `"PatternScale"`: Defines the scaling factor of the descriptor pattern.
     * - `"Octaves"`: Defines the number of octaves covered by detected keypoints.
     */
    auto properties() -> FreakProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the FREAK descriptor properties.
     *
     * This allows read-only access to the properties of the descriptor.
     *
     * \return Constant reference to the `FreakProperties` object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"OrientationNormalized"`: Enables or disables orientation normalization.
     * - `"ScaleNormalized"`: Enables or disables scale normalization.
     * - `"PatternScale"`: Defines the scaling factor of the descriptor pattern.
     * - `"Octaves"`: Defines the number of octaves covered by detected keypoints.
     */
    auto properties() const -> const FreakProperties &{ return mProperties; }

private:

    void init();

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the FREAK descriptor.
     * 
     * \param[in] img Input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     *
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
