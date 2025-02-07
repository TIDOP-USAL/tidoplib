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


constexpr auto brief_default_value_bytes = "32";
constexpr auto brief_default_value_use_orientation = false;

/*!
 * \brief BRIEF %Properties.
 *
 * The BRIEF (Binary Robust Independent Elementary Features) descriptor is a
 * fast and efficient binary descriptor used for keypoint description in
 * computer vision applications.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"Bytes"`
 * - `"UseOrientation"`
 * 
 * Reference:
 * Michael Calonder, Vincent Lepetit, Christoph Strecha, and Pascal Fua.
 * "Brief: Binary robust independent elementary features." In
 * Computer Vision–ECCV 2010, pages 778–792. Springer, 2010.
 * [Link](https://www.cs.ubc.ca/~lowe/525/papers/calonder_eccv10.pdf)
 */
class TL_EXPORT BriefProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes BRIEF properties with default values.
     */
    BriefProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the configuration properties from another `BriefProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    BriefProperties(const BriefProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `BriefProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    BriefProperties(BriefProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BriefProperties() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `BriefProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const BriefProperties &properties) -> BriefProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `BriefProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(BriefProperties &&properties) TL_NOEXCEPT -> BriefProperties &;

    /*!
     * \brief Get the length of the descriptor in bytes.
     *
     * Valid values:
     * - `16`
     * - `32` (default)
     * - `64`
     *
     * \return Descriptor length as a string.
     *
     * \note Property name: `"Bytes"`.
     */
    auto bytes() const -> std::string;

    /*!
     * \brief Check if keypoint orientation is used.
     *
     * If enabled, the descriptor accounts for the keypoint's orientation.
     *
     * \return `true` if orientation is used, `false` otherwise.
     *
     * \note Property name: `"UseOrientation"`.
     */
    auto useOrientation() const -> bool;

    /*!
     * \brief Set the length of the descriptor in bytes.
     *
     * \param[in] bytes The length of the descriptor. Valid values are: 16, 32, or 64.
     */
    void setBytes(const std::string &bytes);

    /*!
     * \brief Enable or disable keypoint orientation usage.
     *
     * \param[in] useOrientation `true` to enable orientation, `false` to disable it.
     */
    void setUseOrientation(bool useOrientation);

// Feature interface

public:

    /*!
     * \brief Reset BRIEF properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief BRIEF Descriptor Extractor.
 *
 * The BRIEF (Binary Robust Independent Elementary Features) descriptor
 * is a lightweight and fast keypoint descriptor that relies on simple
 * binary tests to characterize keypoints.
 *
 * This class wraps OpenCV's `BriefDescriptorExtractor` and provides
 * an interface for extracting BRIEF descriptors from an image.
 *
 * \note Requires OpenCV with `xfeatures2d` enabled.
 */
class TL_EXPORT BriefDescriptor
  : public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::BriefDescriptorExtractor> mBrief;
#endif // HAVE_OPENCV_XFEATURES2D
    BriefProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the BRIEF descriptor with default properties.
     */
    BriefDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes the BRIEF descriptor using the provided properties.
     *
     * \param[in] properties The BRIEF properties for the descriptor.
     */
    explicit BriefDescriptor(const BriefProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `BriefDescriptor` instance.
     *
     * \param[in] briefDescriptor The object to copy.
     */
    BriefDescriptor(const BriefDescriptor &briefDescriptor);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `BriefDescriptor` instance.
     *
     * \param[in] briefDescriptor The object to move.
     */
    BriefDescriptor(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BriefDescriptor() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `BriefDescriptor` instance.
     *
     * \param[in] briefDescriptor The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const BriefDescriptor &briefDescriptor) -> BriefDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `BriefDescriptor` instance.
     *
     * \param[in] briefDescriptor The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(BriefDescriptor &&briefDescriptor) TL_NOEXCEPT -> BriefDescriptor &;

    /*!
     * \brief Get the BRIEF descriptor properties.
     *
     * Provides access to the `BriefProperties`.
     * 
     * \return Reference to the properties object.
     */
    auto properties() -> BriefProperties &{ return mProperties; }

    /*!
     * \brief Get the BRIEF descriptor properties (const version).
     *
     * Provides read-only access to the `BriefProperties`.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const BriefProperties &{ return mProperties; }

private:

    void init();

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the BRIEF Descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
