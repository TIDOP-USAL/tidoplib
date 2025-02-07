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

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */


constexpr auto fast_default_value_threshold{10};
constexpr auto fast_default_value_nonmax_suppression{true};
constexpr auto fast_default_value_detector_type{"TYPE_9_16"};


/*!
 * \brief FAST properties
 *
 * This class defines the properties for the FAST (Features from Accelerated Segment Test)
 * keypoint detector, based on the paper:
 *
 * Rosten, E., & Drummond, T. (2006). Machine Learning for High-Speed Corner Detection.
 * In: Leonardis, A., Bischof, H., & Pinz, A. (eds) *Computer Vision – ECCV 2006*.
 * Lecture Notes in Computer Science, vol 3951. Springer, Berlin, Heidelberg.
 *
 * The FAST detector is designed to efficiently identify corner-like features in an image
 * using a decision tree approach. The key parameters include:
 * - **Threshold**: Determines the intensity difference for corner detection.
 * - **Non-maximal suppression**: Eliminates weaker adjacent corners to improve feature quality.
 * - **Detector type**: Specifies the neighborhood used for the detection.
 */
class TL_EXPORT FastProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes AGAST properties with default values.
     */
    FastProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `FastProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    FastProperties(const FastProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `FastProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    FastProperties(FastProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~FastProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `FastProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const FastProperties &properties) -> FastProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `FastProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(FastProperties &&properties) TL_NOEXCEPT -> FastProperties &;

    /*!
     * \brief Get the intensity threshold for corner detection.
     *
     * The threshold determines the minimum difference in intensity between the central pixel
     * and the surrounding pixels in the neighborhood. Higher values make the detector less
     * sensitive to small variations, reducing the number of detected keypoints.
     *
     * \return The current threshold value (Default = 10).
     */
    auto threshold() const -> int;

    /*!
     * \brief Check if non-maximal suppression is enabled.
     *
     * Non-maximal suppression removes weaker adjacent corners, keeping only the strongest
     * responses. This helps to refine the set of detected keypoints and avoid redundant detections.
     *
     * \return True if non-maximal suppression is enabled, false otherwise (Default = true).
     */
    auto nonmaxSuppression() const -> bool;

    /*!
     * \brief Get the current FAST detector type.
     *
     * The detector type defines the neighborhood size used to evaluate intensity changes:
     * - **TYPE_5_8**: Uses an 8-pixel mask (FAST-5 decision tree).
     * - **TYPE_7_12**: Uses a 12-pixel mask (FAST-7 decision tree).
     * - **TYPE_9_16**: Uses a 16-pixel mask (FAST-9 decision tree, Default).
     *
     * \return The detector type as a string.
     */
    auto detectorType() const -> std::string;

    /*!
     * \brief Set the intensity threshold for corner detection.
     *
     * \param[in] threshold New threshold value.
     */
    void setThreshold(int threshold);

    /*!
     * \brief Enable or disable non-maximal suppression.
     *
     * \param[in] nonmaxSuppression Set to true to enable non-maximal suppression, false to disable it.
     */
    void setNonmaxSuppression(bool nonmaxSuppression);

    /*!
     * \brief Set the FAST detector type.
     *
     * Supported types:
     * - **TYPE_5_8**: Uses an 8-pixel mask (FAST-5 decision tree).
     * - **TYPE_7_12**: Uses a 12-pixel mask (FAST-7 decision tree).
     * - **TYPE_9_16**: Uses a 16-pixel mask (FAST-9 decision tree, Default).
     *
     * \param[in] detectorType The new detector type.
     */
    void setDetectorType(const std::string &detectorType);

// Feature interface

public:

    /*!
     * \brief Reset FAST properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief FAST Feature Detector
 *
 * This class implements the FAST keypoint detector using OpenCV.
 * It allows detecting corners in an image based on intensity variations.
 *
 * The detector uses the `cv::FastFeatureDetector` class internally.
 */
class TL_EXPORT FastDetector
  : public FeatureDetector
{

private:

    cv::Ptr<cv::FastFeatureDetector> mFast;
    FastProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes FAST with default properties.
     */
    FastDetector();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes FAST Detector using the provided properties.
     *
     * \param[in] properties The FAST detection properties.
     */
    explicit FastDetector(const FastProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `FastDetector` instance.
     *
     * \param[in] fast The object to copy.
     */
    FastDetector(const FastDetector &fast);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `FastDetector` instance.
     *
     * \param[in] fast The object to move.
     */
    FastDetector(FastDetector &&fast) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~FastDetector() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `FastDetector` instance.
     *
     * \param[in] fast The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const FastDetector &fast) -> FastDetector &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `FastDetector` instance.
     *
     * \param[in] fast The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(FastDetector &&fast) TL_NOEXCEPT -> FastDetector &;

    /*!
     * \brief Get a reference to the FAST detector properties.
     *
     * This allows modifying the properties of the FAST detector.
     *
     * \return Reference to the `FastProperties` object.
     *
     * \note The following property names are valid when accessed through the `Property` class:
     * - `"Threshold"`: Controls the intensity difference threshold.
     * - `"NonmaxSuppression"`: Enables or disables non-maximum suppression.
     * - `"DetectorType"`: Defines the FAST detector type (`TYPE_5_8`, `TYPE_7_12`, or `TYPE_9_16`).
     */
    auto properties() -> FastProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the FAST detector properties.
     *
     * This allows read-only access to the properties of the FAST detector.
     *
     * \return Constant reference to the `FastProperties` object.
     *
     * \note The following property names are valid when accessed through the `Property` class:
     * - `"Threshold"`: Controls the intensity difference threshold.
     * - `"NonmaxSuppression"`: Enables or disables non-maximum suppression.
     * - `"DetectorType"`: Defines the FAST detector type (`TYPE_5_8`, `TYPE_7_12`, or `TYPE_9_16`).
     */
    auto properties() const -> const FastProperties &{ return mProperties; }

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDetectorType(const std::string &detectorType) -> cv::FastFeatureDetector::DetectorType;
#else
    auto convertDetectorType(const std::string &detectorType) -> int;
#endif

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using the FAST algorithm.
     *
     * \param[in] img The input grayscale image.
     * \param[in] mask Optional mask to specify regions of interest (default = no mask).
     * \return A vector of detected keypoints.
     */
    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

};




/*!
 * \brief FAST Feature Detector (Cuda)
 *
 * This class implements the FAST keypoint detector using OpenCV.
 * It allows detecting corners in an image based on intensity variations.
 *
 * The detector uses the `cv::cuda::FastFeatureDetector` class internally.
 */
class TL_EXPORT FastDetectorCuda
  : public FeatureDetector
{

private:

#ifdef HAVE_OPENCV_CUDAFEATURES2D
    cv::Ptr<cv::cuda::FastFeatureDetector> mFast;
#endif // HAVE_OPENCV_CUDAFEATURES2D
    FastProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes FAST with default properties.
     */
    FastDetectorCuda();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes FAST Detector using the provided properties.
     *
     * \param[in] properties The FAST detection properties.
     */
    explicit FastDetectorCuda(const FastProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `FastDetectorCuda` instance.
     *
     * \param[in] fast The object to copy.
     */
    FastDetectorCuda(const FastDetectorCuda &fast);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `FastDetectorCuda` instance.
     *
     * \param[in] fast The object to move.
     */
    FastDetectorCuda(FastDetectorCuda &&fast) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~FastDetectorCuda() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `FastDetectorCuda` instance.
     *
     * \param[in] fast The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const FastDetectorCuda &fast) -> FastDetectorCuda &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `FastDetectorCuda` instance.
     *
     * \param[in] fast The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(FastDetectorCuda &&fast) TL_NOEXCEPT -> FastDetectorCuda &;

    /*!
     * \brief Get a reference to the FAST detector properties.
     *
     * This allows modifying the properties of the FAST detector.
     *
     * \return Reference to the `FastProperties` object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"Threshold"`: Controls the intensity difference threshold.
     * - `"NonmaxSuppression"`: Enables or disables non-maximum suppression.
     * - `"DetectorType"`: Defines the FAST detector type (`TYPE_5_8`, `TYPE_7_12`, or `TYPE_9_16`).
     */
    auto properties() -> FastProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the FAST detector properties.
     *
     * This allows read-only access to the properties of the FAST detector.
     *
     * \return Constant reference to the `FastProperties` object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"Threshold"`: Controls the intensity difference threshold.
     * - `"NonmaxSuppression"`: Enables or disables non-maximum suppression.
     * - `"DetectorType"`: Defines the FAST detector type (`TYPE_5_8`, `TYPE_7_12`, or `TYPE_9_16`).
     */
    auto properties() const -> const FastProperties &{ return mProperties; }

private:

    auto convertDetectorType(const std::string &detectorType) -> int;

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using the FAST algorithm.
     *
     * \param[in] img The input grayscale image.
     * \param[in] mask Optional mask to specify regions of interest (default = no mask).
     * \return A vector of detected keypoints.
     */
    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

};


/*! \} */

} // namespace tl
