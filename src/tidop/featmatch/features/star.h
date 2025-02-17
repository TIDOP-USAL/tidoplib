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

constexpr auto star_default_value_max_size{45};
constexpr auto star_default_value_response_threshold{30};
constexpr auto star_default_value_line_threshold_projected{10};
constexpr auto star_default_value_line_threshold_binarized{8};
constexpr auto star_default_value_suppress_nonmax_size{5};


/*!
 * \class StarProperties
 * \brief Stores properties for the STAR feature detector.
 *
 * Properties can be accessed directly or by name using \ref getProperty and \ref setProperty.
 * Available property names:
 * - "MaxSize"
 * - "ResponseThreshold"
 * - "LineThresholdProjected"
 * - "LineThresholdBinarized"
 * - "SuppressNonmaxSize"
 */
class TL_EXPORT StarProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes STAR properties with default values.
     */
    StarProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `StarProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    StarProperties(const StarProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `StarProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    StarProperties(StarProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~StarProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `StarProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const StarProperties &properties) -> StarProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `StarProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(StarProperties &&properties) TL_NOEXCEPT -> StarProperties &;

    /*!
     * \brief Get the maximum size of the features.
     *
     * Defines the maximum diameter of the detected keypoints.
     * \return Maximum feature size.
     */
    auto maxSize() const -> int;

    /*!
     * \brief Get the response threshold.
     *
     * A higher threshold reduces the number of detected features by filtering
     * out weaker responses.
     * \return Response threshold.
     */
    auto responseThreshold() const -> int;

    /*!
     * \brief Get the projected line threshold.
     *
     * A higher value allows longer line segments in keypoints.
     * \return Line threshold (projected).
     */
    auto lineThresholdProjected() const -> int;

    /*!
     * \brief Get the binarized line threshold.
     *
     * This threshold determines the minimum contrast for a keypoint to be
     * considered.
     * \return Line threshold (binarized).
     */
    auto lineThresholdBinarized() const -> int;

    /*!
     * \brief Get the suppression window size for non-maximum suppression.
     *
     * A larger value leads to fewer but more significant keypoints.
     * \return Suppression non-maximum size.
     */
    auto suppressNonmaxSize() const -> int;

    /*!
     * \brief Set the maximum size of the features.
     * \param[in] maxSize Maximum feature size.
     */
    void setMaxSize(int maxSize);

    /*!
     * \brief Set the response threshold.
     * \param[in] responseThreshold Response threshold value.
     */
    void setResponseThreshold(int responseThreshold);

    /*!
     * \brief Set the projected line threshold.
     * \param[in] lineThresholdProjected Line threshold (projected).
     */
    void setLineThresholdProjected(int lineThresholdProjected);

    /*!
     * \brief Set the binarized line threshold.
     * \param[in] lineThresholdBinarized Line threshold (binarized).
     */
    void setLineThresholdBinarized(int lineThresholdBinarized);

    /*!
     * \brief Set the suppression window size for non-maximum suppression.
     * \param[in] suppressNonmaxSize Suppression non-maximum size.
     */
    void setSuppressNonmaxSize(int suppressNonmaxSize);

// Feature interface

public:

    /*!
     * \brief Reset STAR properties to their default values.
     */
    void reset() override;

};




/*!
 * \class StarDetector
 * \brief STAR feature detector implementation.
 *
 * Uses the STAR detector to find keypoints in an image.
 */
class TL_EXPORT StarDetector
  : public FeatureDetector
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::StarDetector> mSTAR;
#endif // HAVE_OPENCV_XFEATURES2D
    StarProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the STAR detector with default properties.
     */
    StarDetector();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes STAR using the provided properties.
     *
     * \param[in] properties Configuration properties for STAR.
     */
    explicit StarDetector(const StarProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `StarDetector` instance.
     *
     * \param[in] star The object to copy.
     */
    StarDetector(const StarDetector &star);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `StarDetector` instance.
     *
     * \param[in] star The object to move.
     */
    StarDetector(StarDetector &&star) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~StarDetector() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `StarDetector` instance.
     *
     * \param[in] star The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const StarDetector &star)->StarDetector &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `StarDetector` instance.
     *
     * \param[in] star The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(StarDetector &&star) TL_NOEXCEPT->StarDetector &;

    /*!
     * \brief Get a reference to the STAR properties.
     *
     * This allows modifying the properties of the STAR detector.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> StarProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the STAR properties.
     *
     * This allows read-only access to the properties of the STAR detector.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const StarProperties &{ return mProperties; }

private:

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using STAR.
     *
     * Processes the given image and extracts keypoints using the STAR algorithm.
     * If a mask is provided, keypoints will only be detected in unmasked regions.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] mask Optional mask to specify regions of interest (default = no mask).
     * \return A vector of detected keypoints.
     */
    auto detect(const cv::Mat &img,
                cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

};


/*! \} */ 

} // namespace tl
