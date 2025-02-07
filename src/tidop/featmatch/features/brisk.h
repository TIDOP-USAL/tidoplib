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

namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */


constexpr auto brisk_default_value_threshold{30};
constexpr auto brisk_default_value_octaves{3};
constexpr auto brisk_default_value_pattern_scale{1.0f};


/*!
 * \brief BRISK properties.
 *
 * BRISK (Binary Robust Invariant Scalable Keypoints) is a keypoint detector and descriptor
 * designed for efficiency and robustness in computer vision applications. This class stores
 * the configurable parameters for the BRISK algorithm.
 *
 * Reference:
 * Stefan Leutenegger, Margarita Chli, and Roland Yves Siegwart.
 * "BRISK: Binary robust invariant scalable keypoints." In Computer Vision (ICCV), 2011 IEEE International Conference on,
 * pages 2548–2555. IEEE, 2011.
 * [Link to paper](http://margaritachli.com/papers/ICCV2011paper.pdf)
 */
class TL_EXPORT BriskProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes BRISK properties with default values.
     */
    BriskProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `BriskProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    BriskProperties(const BriskProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `BriskProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    BriskProperties(BriskProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BriskProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `BriskProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const BriskProperties &properties) -> BriskProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `BriskProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(BriskProperties &&properties) TL_NOEXCEPT -> BriskProperties &;

    /*!
     * \brief Get the detection threshold.
     *
     * The threshold value determines the sensitivity of the detector: a higher value results
     * in fewer, more robust keypoints, while a lower value yields more keypoints.
     *
     * \return The threshold value.
     */
    auto threshold() const -> int;

    /*!
     * \brief Get the number of octaves used in the detection.
     *
     * The number of octaves defines the number of scale-space layers over which keypoints
     * are detected.
     *
     * \return The number of octaves.
     */
    auto octaves() const -> int;

    /*!
     * \brief Get the pattern scale factor.
     *
     * The pattern scale factor adjusts the size of the sampling pattern used in the descriptor.
     * A default value of 1.0 corresponds to the standard scale.
     *
     * \return The pattern scale factor.
     */
    auto patternScale() const -> float;

    /*!
     * \brief Set the detection threshold.
     *
     * \param[in] threshold The new threshold value (default is typically 30).
     */
    void setThreshold(int threshold);

    /*!
     * \brief Set the number of octaves.
     *
     * \param[in] octaves The new number of octaves to use in the scale space.
     */
    void setOctaves(int octaves);

    /*!
     * \brief Set the pattern scale factor.
     *
     * \param[in] patternScale The new pattern scale factor.
     */
    void setPatternScale(float patternScale);

// Feature interface

public:

    /*!
     * \brief Reset BRISK properties to their default values.
     *
     * Resets:
     * - The detection threshold to the default value (30).
     * - The number of octaves to the default value (1).
     * - The pattern scale factor to the default value (1.0).
     */
    void reset() override;

};




/*!
 * \brief BRISK Descriptor Extractor.
 *
 * This class implements the BRISK algorithm for keypoint detection and descriptor extraction.
 * It uses OpenCV's BRISK implementation (from the xfeatures2d module) and applies the
 * configuration specified in a `BriskProperties` instance.
 *
 * \note Requires OpenCV with xfeatures2d support.
 */
class TL_EXPORT BriskDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

private:

    cv::Ptr<cv::BRISK> mBrisk;
    BriskProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the BRISK detector/descriptor with default properties.
     */
    BriskDetectorDescriptor();
    
    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes the BRISK detector/descriptor using the provided properties.
     *
     * \param[in] properties The AGAST properties.
     */
    explicit BriskDetectorDescriptor(const BriskProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `BriskDetectorDescriptor` instance.
     *
     * \param[in] brisk The object to copy.
     */
    BriskDetectorDescriptor(const BriskDetectorDescriptor &brisk);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `BriskDetectorDescriptor` instance.
     *
     * \param[in] brisk The object to move.
     */
    BriskDetectorDescriptor(BriskDetectorDescriptor &&brisk) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BriskDetectorDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal detector from another `BriskDetectorDescriptor` instance.
     *
     * \param[in] brisk The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const BriskDetectorDescriptor &brisk) -> BriskDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `BriskDetectorDescriptor` instance.
     *
     * \param[in] brisk The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(BriskDetectorDescriptor &&brisk) TL_NOEXCEPT -> BriskDetectorDescriptor &;

    /*!
     * \brief Get the current BRISK properties.
     *
     * Provides access to the `BriskProperties`.
     *
     * \return Reference to the `BriskProperties` object.
     */
    auto properties() -> BriskProperties &{ return mProperties; }

    /*!
     * \brief Get the current BRISK properties (const version).
     *
     * Provides read-only access to the `BriskProperties`.
     *
     * \return Constant reference to the `BriskProperties` object.
     */
    auto properties() const -> const BriskProperties &{ return mProperties; }

protected:

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using BRISK.
     *
     * Processes the given image and extracts keypoints using the BRISK algorithm.
     * If a mask is provided, keypoints will only be detected in unmasked regions.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] mask Optional mask to specify regions of interest (default = no mask).
     * \return A vector of detected keypoints.
     */
    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the BRISK descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (may be modified).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
