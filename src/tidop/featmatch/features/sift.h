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

#if defined OPENCV_ENABLE_NONFREE && defined HAVE_OPENCV_XFEATURES2D
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D

namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */


constexpr auto sift_default_value_features_number{5000};
constexpr auto sift_default_value_octave_layers{3};
constexpr auto sift_default_value_contrast_threshold{0.04};
constexpr auto sift_default_value_edge_threshold{10.};
constexpr auto sift_default_value_sigma{1.6};


/*!
 * \brief Stores properties for the SIFT feature detector and descriptor.
 *
 * Properties can be accessed directly or by name using \ref getProperty and \ref setProperty.
 * Available property names:
 * - "FeaturesNumber"
 * - "OctaveLayers"
 * - "ContrastThreshold"
 * - "EdgeThreshold"
 * - "Sigma"
 */
class TL_EXPORT SiftProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes SIFT properties with default values.
     */
    SiftProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `SiftProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    SiftProperties(const SiftProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `SiftProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    SiftProperties(SiftProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~SiftProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `SiftProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const SiftProperties &properties) -> SiftProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `SiftProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(SiftProperties &&properties) TL_NOEXCEPT -> SiftProperties &;

    /*!
     * \brief Get the number of best features to retain.
     *
     * Features are ranked by their scores, measured in the SIFT algorithm
     * as the local contrast. A higher value retains more features.
     *
     * \return Number of features.
     */
    auto featuresNumber() const -> int;

    /*!
     * \brief Get the number of layers in each octave.
     *
     * The default value is 3, as used in D. Lowe's original SIFT paper.
     * The number of octaves is computed automatically based on image resolution.
     *
     * \return Number of layers per octave.
     */
    auto octaveLayers() const -> int;

    /*!
     * \brief Get the contrast threshold used to filter out weak features.
     *
     * A larger threshold results in fewer features, as weak contrast regions
     * are ignored.
     *
     * \return Contrast threshold.
     */
    auto contrastThreshold() const -> double;

    /*!
     * \brief Get the threshold used to filter out edge-like features.
     *
     * Unlike contrastThreshold, a larger edgeThreshold retains more features
     * by reducing edge-based filtering.
     *
     * \return Edge threshold.
     */
    auto edgeThreshold() const -> double;

    /*!
     * \brief Get the sigma value for Gaussian blur at the first octave.
     *
     * If the input image is blurry (e.g., from a low-quality camera), lowering
     * this value might improve results.
     *
     * \return Gaussian sigma value.
     */
    auto sigma() const -> double;

    /*!
     * \brief Set the number of best features to retain.
     *
     * \param[in] featuresNumber Number of features.
     */
    void setFeaturesNumber(int featuresNumber);

    /*!
     * \brief Set the number of layers per octave.
     *
     * \param[in] octaveLayers The number of layers per octave (default: 3).
     */
    void setOctaveLayers(int octaveLayers);

    /*!
     * \brief Set the contrast threshold.
     *
     * \param[in] contrastThreshold Contrast threshold value.
     */
    void setContrastThreshold(double contrastThreshold);

    /*!
     * \brief Set the edge threshold.
     *
     * \param[in] edgeThreshold Edge threshold value.
     */
    void setEdgeThreshold(double edgeThreshold);

    /*!
     * \brief Set the sigma value for Gaussian blur at the first octave.
     *
     * \param[in] sigma Sigma value.
     */
    void setSigma(double sigma);

// Feature interface

public:

    /*!
     * \brief Reset SIFT properties to their default values.
     */
    void reset() override;

};




/*!
 * \brief SIFT feature detector and descriptor.
 *
 * Implements the Scale-Invariant Feature Transform (SIFT) algorithm for
 * detecting and describing keypoints.
 */
class TL_EXPORT SiftDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

private:

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 4))
    cv::Ptr <cv::SIFT>  mSift;
#elif defined OPENCV_ENABLE_NONFREE && defined HAVE_OPENCV_XFEATURES2D
    cv::Ptr<cv::xfeatures2d::SIFT> mSift;
#endif // HAVE_OPENCV_XFEATURES2D
    SiftProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the SIFT detector and descriptor with default properties.
     */
    SiftDetectorDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes SIFT using the provided properties.
     *
     * \param[in] properties Configuration properties for SIFT.
     */
    explicit SiftDetectorDescriptor(const SiftProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `SiftDetectorDescriptor` instance.
     *
     * \param[in] sift The object to copy.
     */
    SiftDetectorDescriptor(const SiftDetectorDescriptor &sift);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `SiftDetectorDescriptor` instance.
     *
     * \param[in] sift The object to move.
     */
    SiftDetectorDescriptor(SiftDetectorDescriptor &&sift) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~SiftDetectorDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `SiftDetectorDescriptor` instance.
     *
     * \param[in] sift The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const SiftDetectorDescriptor &sift) -> SiftDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `SiftDetectorDescriptor` instance.
     *
     * \param[in] sift The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(SiftDetectorDescriptor &&sift) TL_NOEXCEPT -> SiftDetectorDescriptor &;

    /*!
     * \brief Get a reference to the SIFT properties.
     *
     * This allows modifying the properties of the SIFT detector/descriptor.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> SiftProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the SIFT properties.
     *
     * This allows read-only access to the properties of the SIFT detector/descriptor.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const SiftProperties &{ return mProperties; }

private:

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using SIFT.
     *
     * Processes the given image and extracts keypoints using the SIFT algorithm.
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
     * Extracts binary descriptors for the given keypoints using the SIFT descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl

