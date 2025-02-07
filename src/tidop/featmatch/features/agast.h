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
#include "tidop/core/base/property.h"

namespace tl
{


/*! \addtogroup FeatureExtraction
 * 
 *  \{
 */


constexpr auto agast_default_value_threshold{10};
constexpr auto agast_default_value_nonmax_suppression{true};
constexpr auto agast_default_value_detector_type{"OAST_9_16"};


/*!
 * \brief Agast properties
 *
 * AGAST: Adaptive and Generic Corner Detection Based on the Accelerated %Segment Test.
 *
 * AGAST is an improvement over the original FAST corner detector, introducing an adaptive
 * decision tree to enhance performance and robustness. It enables efficient and accurate
 * corner detection, especially for real-time applications.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"Threshold"`
 * - `"NonmaxSuppression"`
 * - `"DetectorType"`
 * 
 * Reference:
 * - Mair E., Hager G.D., Burschka D., Suppa M., Hirzinger G. (2010) Adaptive and Generic
 *   Corner Detection Based on the Accelerated %Segment Test. In: Daniilidis K., Maragos P.,
 *   Paragios N. (eds) Computer Vision – ECCV 2010. ECCV 2010.
 *   Lecture Notes in Computer Science, vol 6312. Springer, Berlin, Heidelberg.
 *   [https://mediatum.ub.tum.de/doc/1287456/1287456.pdf](https://mediatum.ub.tum.de/doc/1287456/1287456.pdf)
 */
class TL_EXPORT AgastProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     * 
     * Initializes AGAST properties with default values.
     */
    AgastProperties();

    /*!
     * \brief Copy constructor.
     * 
     * Copies the properties from another `AgastProperties` instance.
     * 
     * \param[in] properties The object to copy.
     */
    AgastProperties(const AgastProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `AgastProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    AgastProperties(AgastProperties &&properties) TL_NOEXCEPT;
    
    /*!
     * \brief Destructor.
     */
    ~AgastProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `AgastProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const AgastProperties &properties) -> AgastProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `AgastProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(AgastProperties &&properties) TL_NOEXCEPT -> AgastProperties &;

    /*!
     * \brief Get the threshold value used by AGAST.
     *
     * The Adaptive %Segment Test (AST) applies a minimum difference threshold when comparing
     * the intensity of a pixel in the pattern with the central pixel (nucleus).
     * This threshold determines the sensitivity of the corner detector:
     * - A higher threshold results in fewer but stronger corners.
     * - A lower threshold detects more corners, including those with softer gradients.
     *
     * \return The threshold value.
     *
     * \note Property name: `"Threshold"`.
     */
    auto threshold() const -> int;

    /*!
     * \brief Check if non-maximal suppression is enabled.
     *
     * Non-maximal suppression (NMS) is a post-processing step that removes adjacent
     * corners to retain only the strongest responses, improving localization accuracy.
     *
     * \return `true` if NMS is enabled, `false` otherwise.
     *
     * \note Property name: `"NonmaxSuppression"`.
     */
    auto nonmaxSuppression() const -> bool;

    /*!
     * \brief Get the current AGAST detector type.
     *
     * AGAST supports multiple decision tree configurations for different performance
     * and accuracy trade-offs. The available types are:
     * - `"AGAST_5_8"`: AGAST-5 with an 8-pixel mask.
     * - `"AGAST_7_12d"`: AGAST-7 with a 12-pixel diamond-shaped mask.
     * - `"AGAST_7_12s"`: AGAST-7 with a 12-pixel squared mask.
     * - `"OAST_9_16"`: OAST-9 (Optimal AST) with a 16-pixel mask.
     *
     * \return The current detector type as a string.
     *
     * \note Property name: `"DetectorType"`.
     */
    auto detectorType() const -> std::string;

    /*!
     * \brief Set the AGAST detection threshold.
     *
     * The threshold determines the minimum intensity difference required to classify
     * a point as a corner.
     *
     * \param[in] threshold The threshold value (Default = 10).
     */
    void setThreshold(int threshold);

    /*!
     * \brief Enable or disable non-maximal suppression.
     *
     * If enabled, AGAST applies a non-maximal suppression step to remove weaker,
     * adjacent corner responses.
     *
     * \param[in] nonmaxSuppression `true` to enable, `false` to disable (Default = true).
     */
    void setNonmaxSuppression(bool nonmaxSuppression);

    /*!
     * \brief Set the AGAST detector type.
     *
     * The selected detector type determines the decision tree and pixel mask used
     * for corner detection. Supported types:
     * - `"AGAST_5_8"`: AGAST-5 with an 8-pixel mask.
     * - `"AGAST_7_12d"`: AGAST-7 with a 12-pixel diamond-shaped mask.
     * - `"AGAST_7_12s"`: AGAST-7 with a 12-pixel squared mask.
     * - `"OAST_9_16"`: OAST-9 (Optimal AST) with a 16-pixel mask.
     *
     * \param[in] detectorType The desired detector type (Default = `"OAST_9_16"`).
     */
    void setDetectorType(const std::string &detectorType);

// Feature interface

public:

    /*!
     * \brief Reset AGAST properties to their default values.
     */
    void reset() override;
};



/*!
 * \brief AGAST Feature Detector
 *
 * AGAST (Adaptive and Generic Accelerated %Segment Test) is a high-speed corner detection
 * algorithm designed for real-time applications. It improves upon the FAST algorithm by
 * introducing adaptive decision trees that enhance both speed and robustness.
 *
 * This class wraps OpenCV's `cv::AgastFeatureDetector`, providing an interface to set
 * detection properties via `AgastProperties`.
 *
 * \note Requires OpenCV with the AGAST feature detector enabled.
 *
 * Reference:
 * - Mair E., Hager G.D., Burschka D., Suppa M., Hirzinger G. (2010) Adaptive and Generic
 *   Corner Detection Based on the Accelerated %Segment Test. In: Daniilidis K., Maragos P.,
 *   Paragios N. (eds) Computer Vision – ECCV 2010. ECCV 2010.
 *   Lecture Notes in Computer Science, vol 6312. Springer, Berlin, Heidelberg.
 *   [https://mediatum.ub.tum.de/doc/1287456/1287456.pdf](https://mediatum.ub.tum.de/doc/1287456/1287456.pdf)
 */
class TL_EXPORT AgastDetector
  : public FeatureDetector
{

private:

    cv::Ptr<cv::AgastFeatureDetector> mAgast;
    AgastProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes AGAST with default properties.
     */
    AgastDetector();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes AGAST Detector using the provided properties.
     *
     * \param[in] properties The AGAST detection properties.
     */
    explicit AgastDetector(const AgastProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `AgastDetector` instance.
     *
     * \param[in] agastDetector The object to copy.
     */
    AgastDetector(const AgastDetector &agastDetector);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `AgastDetector` instance.
     *
     * \param[in] agastDetector The object to move.
     */
    AgastDetector(AgastDetector &&agastDetector) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~AgastDetector() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal detector from another `AgastDetector` instance.
     *
     * \param[in] agastDetector The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const AgastDetector &agastDetector) -> AgastDetector &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `AgastDetector` instance.
     *
     * \param[in] agastDetector The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(AgastDetector &&agastDetector) TL_NOEXCEPT -> AgastDetector &;

    /*!
     * \brief Get the AGAST properties.
     *
     * Provides access to the `AgastProperties`.
     *
     * \return Reference to the AGAST properties.
     * 
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Threshold"`
     * - `"NonmaxSuppression"`
     * - `"DetectorType"`
     */
    auto properties() -> AgastProperties &{ return mProperties; }

    /*!
     * \brief Get the AGAST properties (const version).
     *
     * Provides read-only access to the `AgastProperties`.
     *
     * \return Constant reference to the AGAST properties.
     * 
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Threshold"`
     * - `"NonmaxSuppression"`
     * - `"DetectorType"`
     */
    auto properties() const -> const AgastProperties &{ return mProperties; }

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDetectorType(const std::string &detectorType) -> cv::AgastFeatureDetector::DetectorType;
#else
    auto convertDetectorType(const std::string &detectorType) -> int;
#endif

    void initAgastFromProperties();

// FeatureDetector interface

public:

    /*!
     * \brief Detects keypoints in an image using AGAST.
     *
     * Processes the given image and extracts keypoints using the AGAST algorithm.
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

