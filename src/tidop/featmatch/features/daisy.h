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

constexpr auto daisy_default_value_radius{15.f};
constexpr auto daisy_default_value_qradius{3};
constexpr auto daisy_default_value_qtheta{8};
constexpr auto daisy_default_value_qhist{8};
constexpr auto daisy_default_value_norm{"NRM_NONE"};
constexpr auto daisy_default_value_interpolation{true};
constexpr auto daisy_default_value_use_orientation{false};


/*!
 * \brief DAISY Properties
 *
 * %Properties of the DAISY descriptor based on the following publication:
 * E. Tola, V. Lepetit, and P. Fua.
 * "DAISY: An Efficient Dense Descriptor Applied to Wide Baseline Stereo."
 * IEEE Transactions on Pattern Analysis and Machine Intelligence, 32(5):815–830, May 2010.
 */
class TL_EXPORT DaisyProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes DAISY properties with default values.
     */
    DaisyProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `DaisyProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    DaisyProperties(const DaisyProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `DaisyProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    DaisyProperties(DaisyProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~DaisyProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `DaisyProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const DaisyProperties &properties) -> DaisyProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `DaisyProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(DaisyProperties &&properties) TL_NOEXCEPT -> DaisyProperties &;

    /*!
     * \brief Get the radius of the descriptor at the initial scale.
     * \return The descriptor radius (default: 15.0f).
     */
    auto radius() const -> float;

    /*!
     * \brief Get the number of radial divisions.
     * \return The number of radial divisions (default: 3).
     */
    auto radialDivisions() const -> int;

    /*!
     * \brief Get the number of angular divisions.
     * \return The number of angular divisions (default: 8).
     */
    auto angularDivision() const -> int;

    /*!
     * \brief Get the number of gradient orientation divisions.
     * \return The number of gradient orientation divisions (default: 8).
     */
    auto gradientOrientationsDivision() const -> int;

    /*!
     * \brief Get the descriptor normalization type.
     *
     * Possible values:
     * - `"NRM_NONE"`: No normalization (default).
     * - `"NRM_PARTIAL"`: Each histogram is independently normalized to an L2 norm of 1.0.
     * - `"NRM_FULL"`: The entire descriptor is normalized to an L2 norm of 1.0.
     * - `"NRM_SIFT"`: Similar to `"NRM_FULL"`, but individual values are clamped to a maximum of 0.154 (as in SIFT).
     *
     * \return The normalization type as a string.
     */
    auto normalization() const -> std::string;

    /*!
     * \brief Check whether interpolation is enabled.
     * \return True if interpolation is enabled (default: true), otherwise false.
     */
    auto interpolation() const -> bool;

    /*!
     * \brief Check whether keypoint orientation is used.
     * \return True if orientation is used (default: false), otherwise false.
     */
    auto useOrientation() const -> bool;

    /*!
     * \brief Set the descriptor radius at the initial scale.
     * \param[in] radius The new radius value.
     */
    void setRadius(float radius);

    /*!
     * \brief Set the number of radial divisions.
     * \param[in] radialDivisions The new number of radial divisions.
     */
    void setRadialDivisions(int radialDivisions);

    /*!
     * \brief Set the number of angular divisions.
     * \param[in] angularDivisions The new number of angular divisions.
     */
    void setAngularDivision(int angularDivision);

    /*!
     * \brief Set the number of gradient orientation divisions.
     * \param[in] gradientOrientationDivisions The new number of gradient orientation divisions.
     */
    void setGradientOrientationsDivision(int gradientOrientationsDivision);

    /*!
     * \brief Set normalization type
     * 
     * Possible values:
     * - `"NRM_NONE"`: No normalization (default).
     * - `"NRM_PARTIAL"`: Each histogram is independently normalized to an L2 norm of 1.0.
     * - `"NRM_FULL"`: The entire descriptor is normalized to an L2 norm of 1.0.
     * - `"NRM_SIFT"`: Similar to `"NRM_FULL"`, but individual values are clamped to a maximum of 0.154 (as in SIFT).
     * 
     * \param[in] normalization The normalization type as a string.
     */
    void setNormalization(const std::string &normalization);

    /*!
     * \brief Enable or disable interpolation.
     * \param[in] interpolation True to enable interpolation, false to disable.
     */
    void setInterpolation(bool interpolation);

    /*!
     * \brief Enable or disable the use of keypoint orientation.
     * \param[in] useOrientation True to enable orientation, false to disable.
     */
    void setUseOrientation(bool useOrientation);

// Feature interface

public:

    /*!
     * \brief Reset DAISY properties to their default values.
     */
    void reset() override;

};




/*!
 * \brief DAISY Descriptor Extractor
 *
 * This class provides an implementation of the DAISY descriptor extractor using OpenCV's
 * xfeatures2d module. The descriptor parameters are configured via the `DaisyProperties` class.
 *
 * \note Requires OpenCV with the xfeatures2d module enabled.
 */
class TL_EXPORT DaisyDescriptor
  : public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::DAISY> mDAISY;
#endif // HAVE_OPENCV_XFEATURES2D
    DaisyProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the DAISY descriptor extractor with default properties.
     */
    DaisyDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes DAISY using the provided properties.
     *
     * \param[in] properties The DAISY configuration properties.
     */
    explicit DaisyDescriptor(const DaisyProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `DaisyProperties` instance.
     *
     * \param[in] daisy The object to copy.
     */
    DaisyDescriptor(const DaisyDescriptor &daisy);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `DaisyDescriptor` instance.
     *
     * \param[in] daisy The object to move.
     */
    DaisyDescriptor(DaisyDescriptor &&daisy) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~DaisyDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `DaisyDescriptor` instance.
     *
     * \param[in] daisy The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const DaisyDescriptor &daisy) -> DaisyDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `DaisyDescriptor` instance.
     *
     * \param[in] daisy The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(DaisyDescriptor &&daisy) TL_NOEXCEPT -> DaisyDescriptor &;

    /*!
     * \brief Get the DAISY properties.
     *
     * Provides access to the `DaisyProperties`.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> DaisyProperties &{ return mProperties; }

    /*!
     * \brief Get the DAISY properties (const version).
     *
     * Provides read-only access to the `DaisyProperties`.
     *
     * \return Const reference to the properties object.
     */
    auto properties() const -> const DaisyProperties &{ return mProperties; }

private:

    void init();

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the DAISY descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
