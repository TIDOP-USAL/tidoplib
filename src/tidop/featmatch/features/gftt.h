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

/*!
 * \brief GFTT (Good Features to Track) properties class.
 *
 * Stores configuration parameters for the GFTT (Good Features to Track) corner detector.
 * This detector is designed to identify strong corners in an image using the minimum eigenvalue method
 * or the Harris detector response.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"MaxFeatures"`: Maximum number of corners to detect.
 * - `"QualityLevel"`: Minimum accepted quality of image corners.
 * - `"MinDistance"`: Minimum possible Euclidean distance between detected corners.
 * - `"BlockSize"`: Size of the averaging block for derivative computation.
 * - `"HarrisDetector"`: Enables the use of the Harris detector instead of the minimum eigenvalue method.
 * - `"K"`: Free parameter for the Harris detector.
 */
class TL_EXPORT GfttProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes GFTT properties with default values.
     */
    GfttProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `GfttProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    GfttProperties(const GfttProperties &gfttProperties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `GfttProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    GfttProperties(GfttProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~GfttProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `GfttProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const GfttProperties &properties) -> GfttProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `GfttProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(GfttProperties &&properties) TL_NOEXCEPT -> GfttProperties &;

    /*!
     * \brief Gets the maximum number of corners to detect.
     *
     * If set to 0 or negative, there is no limit on the number of detected corners.
     *
     * \return Maximum number of corners.
     *
     * \note Property name: `"MaxFeatures"`.
     */
    auto maxFeatures() const -> int;

    /*!
     * \brief Gets the minimum accepted quality level of image corners.
     *
     * The value is multiplied by the best corner quality measure. Corners with a lower measure are rejected.
     *
     * \return Quality level.
     *
     * \note Property name: `"QualityLevel"`.
     */
    auto qualityLevel() const -> double;

    /*!
     * \brief Gets the minimum possible Euclidean distance between detected corners.
     *
     * Ensures that detected corners are sufficiently spaced apart.
     *
     * \return Minimum distance between corners.
     *
     * \note Property name: `"MinDistance"`.
     */
    auto minDistance() const -> double;

    /*!
     * \brief Gets the size of the averaging block for computing the derivative covariation matrix.
     *
     * A larger block size results in a smoother response.
     *
     * \return Block size.
     *
     * \note Property name: `"BlockSize"`.
     */
    auto blockSize() const -> int;

    /*!
     * \brief Checks if the Harris detector is enabled.
     *
     * If `true`, the Harris corner detector response is used instead of the minimum eigenvalue method.
     *
     * \return `true` if Harris detector is enabled, `false` otherwise.
     *
     * \note Property name: `"HarrisDetector"`.
     */
    auto harrisDetector() const -> bool;

    /*!
     * \brief Gets the free parameter for the Harris detector.
     *
     * This parameter is used when `HarrisDetector` is enabled.
     *
     * \return Harris detector parameter.
     *
     * \note Property name: `"K"`.
     */
    auto k() const -> double;

    /*!
     * \brief Sets the maximum number of corners to detect.
     *
     * \param[in] maxFeatures Maximum number of corners.
     */
    void setMaxFeatures(int maxFeatures);

    /*!
     * \brief Sets the minimum accepted quality level of image corners.
     *
     * \param[in] qlevel Quality level.
     */
    void setQualityLevel(double qlevel);

    /*!
     * \brief Sets the minimum possible Euclidean distance between detected corners.
     *
     * \param[in] minDistance Minimum distance between corners.
     */
    void setMinDistance(double minDistance);

    /*!
     * \brief Sets the size of the averaging block for computing the derivative covariation matrix.
     *
     * \param[in] blockSize Block size.
     */
    void setBlockSize(int blockSize);

    /*!
     * \brief Enables or disables the Harris detector.
     *
     * \param[in] value `true` to enable, `false` to disable.
     */
    void setHarrisDetector(bool value);

    /*!
     * \brief Sets the free parameter for the Harris detector.
     *
     * \param[in] k Harris detector parameter.
     */
    void setK(double k);

// Feature interface

public:

    /*!
     * \brief Resets all properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief GFTT (Good Features to Track) feature detector.
 *
 * Implements the GFTT detector, which finds the most prominent corners in an image using
 * the minimum eigenvalue method or the Harris detector response.
 *
 * This detector applies non-maximum suppression and filters weak corners based on the `QualityLevel` parameter.
 * It is commonly used for object tracking and feature extraction in computer vision applications.
 */
class TL_EXPORT GfttDetector
  : public FeatureDetector
{

private:

    cv::Ptr<cv::GFTTDetector> mGFTT;
    GfttProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes GFTT with default properties.
     */
    GfttDetector();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes GFTT Detector using the provided properties.
     *
     * \param[in] properties The GFTT detection properties.
     */
    explicit GfttDetector(const GfttProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `GfttDetector` instance.
     *
     * \param[in] gftt The object to copy.
     */
    GfttDetector(const GfttDetector &gftt);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `GfttDetector` instance.
     *
     * \param[in] gftt The object to move.
     */
    GfttDetector(GfttDetector &&gftt) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~GfttDetector() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal detector from another `GfttDetector` instance.
     *
     * \param[in] gftt The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const GfttDetector &gftt) -> GfttDetector &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `GfttDetector` instance.
     *
     * \param[in] gftt The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(GfttDetector &&gftt) TL_NOEXCEPT -> GfttDetector &;

    /*!
     * \brief Get a reference to the GFTT detector properties.
     *
     * This allows modifying the properties of the GFTT detector.
     *
     * \return Reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"MaxFeatures"`: Maximum number of corners to detect.
     * - `"QualityLevel"`: Minimum accepted quality of image corners.
     * - `"MinDistance"`: Minimum possible Euclidean distance between detected corners.
     * - `"BlockSize"`: Size of the averaging block for derivative computation.
     * - `"HarrisDetector"`: Enables the use of the Harris detector instead of the minimum eigenvalue method.
     * - `"K"`: Free parameter for the Harris detector.
     */
    auto properties() -> GfttProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the GFTT detector properties.
     *
     * This allows read-only access to the properties of the GFTT detector.
     *
     * \return Constant reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"MaxFeatures"`: Maximum number of corners to detect.
     * - `"QualityLevel"`: Minimum accepted quality of image corners.
     * - `"MinDistance"`: Minimum possible Euclidean distance between detected corners.
     * - `"BlockSize"`: Size of the averaging block for derivative computation.
     * - `"HarrisDetector"`: Enables the use of the Harris detector instead of the minimum eigenvalue method.
     * - `"K"`: Free parameter for the Harris detector.
     */
    auto properties() const -> const GfttProperties &{ return mProperties; }

private:

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using GFTT.
     *
     * Processes the given image and extracts keypoints using the GFTT algorithm.
     * If a mask is provided, keypoints will only be detected in unmasked regions.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] mask Optional mask to specify regions of interest (default = no mask).
     *
     * \return A vector of detected keypoints.
     */
    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

};

/*! \} */

} // namespace tl
