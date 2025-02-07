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


constexpr auto vgg_default_value_descriptor_type{"VGG_120"};
constexpr auto vgg_default_value_scale_factor{6.25f};
constexpr auto vgg_default_value_sigma{1.4f};
constexpr auto vgg_default_value_use_normalize_descriptor{false};
constexpr auto vgg_default_value_use_normalize_image{true};
constexpr auto vgg_default_value_useScale_orientation{true};


/*!
 * \class VggProperties
 * \brief Stores the parameters for the VGG feature descriptor.
 *
 * The VGG descriptor is a feature descriptor inspired by the VGG network.
 * It is designed to capture image features using Gaussian blurring and
 * normalization techniques. The descriptor can operate in different modes
 * depending on the type chosen (VGG_120, VGG_80, VGG_64, VGG_48).
 */
class TL_EXPORT VggProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     * 
     * Initializes the properties with their default values.
     */
    VggProperties();

    /*!
     * \brief Copy constructor.
     * 
     * Copies the properties from another `VggProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    VggProperties(const VggProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `VggProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    VggProperties(VggProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Default destructor.
     */
    ~VggProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `VggProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const VggProperties &properties) -> VggProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `VggProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(VggProperties &&properties) TL_NOEXCEPT -> VggProperties &;

    /*!
     * \brief Gets the type of descriptor used.
     * \return The descriptor type as a string. Available types:
     * - "VGG_120" (default)
     * - "VGG_80"
     * - "VGG_64"
     * - "VGG_48"
     */
    auto descriptorType() const -> std::string;

    /*!
     * \brief Sets the type of descriptor to use.
     * \param[in] descriptorType The desired descriptor type.
     * Must be one of "VGG_120", "VGG_80", "VGG_64", or "VGG_48".
     */
    void setDescriptorType(const std::string &descriptorType);

    /*!
     * \brief Gets the scale factor applied to the descriptor.
     * \return The scale factor (default: 6.25).
     */
    auto scaleFactor() const -> float;

    /*!
     * \brief Sets the scale factor for the descriptor.
     * \param[in] scaleFactor Scale factor to apply.
     */
    void setScaleFactor(float scaleFactor);

    /*!
     * \brief Gets the Gaussian kernel value used for image blurring.
     * \return The Gaussian sigma value (default: 1.4).
     */
    auto sigma() const -> float;

    /*!
     * \brief Sets the Gaussian kernel value for image blurring.
     * \param[in] sigma The sigma value for the Gaussian kernel.
     */
    void setSigma(float sigma);

    /*!
     * \brief Checks whether the descriptor normalization is enabled.
     * \return True if the descriptor is normalized, false otherwise (default: false).
     */
    auto useNormalizeDescriptor() const -> bool;

    /*!
     * \brief Enables or disables descriptor normalization.
     * \param[in] useNormalizeDescriptor Set to true to enable normalization.
     */
    void setUseNormalizeDescriptor(bool useNormalizeDescriptor);

    /*!
     * \brief Checks whether the image is normalized before feature extraction.
     * \return True if image normalization is enabled, false otherwise (default: true).
     */
    auto useNormalizeImage() const -> bool;

    /*!
     * \brief Enables or disables image normalization before feature extraction.
     * \param[in] useNormalizeImage Set to true to enable normalization.
     */
    void setUseNormalizeImage(bool useNormalizeImage);

    /*!
     * \brief Checks whether scale orientation is used in feature extraction.
     * \return True if scale orientation is enabled, false otherwise (default: true).
     */
    auto useScaleOrientation() const -> bool;

    /*!
     * \brief Enables or disables the use of scale orientation in feature extraction.
     * \param[in] useScaleOrientation Set to true to use scale orientation.
     */
    void setUseScaleOrientation(bool useScaleOrientation);

// Feature interface

public:

    /*!
     * \brief Resets VGG properties to their default values.
     */
    void reset() override;

};




/*!
 * \class VggDescriptor
 * \brief Implements the VGG feature descriptor using OpenCV.
 *
 * The VGG descriptor extracts feature descriptors from an image using
 * a Gaussian blurring and normalization pipeline. It is particularly
 * useful for applications in image matching and recognition.
 *
 * This class requires OpenCV's `xfeatures2d` module to be available.
 */
class TL_EXPORT VggDescriptor
  : public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
    cv::Ptr<cv::xfeatures2d::VGG> mVGG;
#endif
#endif // HAVE_OPENCV_XFEATURES2D
    VggProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the VGG detector and descriptor with default properties.
     */
    VggDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes VGG using the provided properties.
     *
     * \param[in] properties Configuration properties for VGG.
     */
    explicit VggDescriptor(const VggProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `VggDescriptor` instance.
     *
     * \param[in] vgg The object to copy.
     */
    VggDescriptor(const VggDescriptor &vgg);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `VggDescriptor` instance.
     *
     * \param[in] vgg The object to move.
     */
    VggDescriptor(VggDescriptor &&vgg) TL_NOEXCEPT;

    /*!
     * \brief Default destructor.
     */
    ~VggDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `VggDescriptor` instance.
     *
     * \param[in] vgg The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const VggDescriptor &vgg) -> VggDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `VggDescriptor` instance.
     *
     * \param[in] vgg The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(VggDescriptor &&vgg) TL_NOEXCEPT -> VggDescriptor &;

    /*!
     * \brief Get a reference to the VGG properties.
     *
     * This allows modifying the properties of the VGG descriptor.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> VggProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the VGG properties.
     *
     * This allows read-only access to the properties of the VGG descriptor.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const VggProperties &{ return mProperties; }

private:

    void init();

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the VGG descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img,
                 std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
