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

namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */


/*!
 * \brief MSER Properties
 * 
 * Properties class for the MSER (Maximally Stable Extremal Regions) keypoint detector.
 * This class holds the configuration settings for the detector, which are used to adjust
 * the behavior of the MSER keypoint detection algorithm.
 * 
 * Properties can be accessed and modified using their respective names:
 * - **"Delta"**: Step size between intensity thresholds for region detection.
 * - **"MinArea"**: Minimum area of detected regions.
 * - **"MaxArea"**: Maximum area of detected regions.
 * - **"MaxVariation"**: Maximum variation of the region size relative to its initial size.
 * - **"MinDiversity"**: Minimum diversity of region stability.
 * - **"MaxEvolution"**: Maximum number of evolution steps.
 * - **"AreaThreshold"**: Area threshold used for region filtering.
 * - **"MinMargin"**: Minimum margin for detected regions.
 * - **"EdgeBlurSize"**: Size of the Gaussian kernel for edge blurring.
 * 
 */
class TL_EXPORT MserProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes MSER properties with default values.
     */
    MserProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `MserProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    MserProperties(const MserProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `MserProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    MserProperties(MserProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~MserProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `MserProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const MserProperties &properties) -> MserProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `MserProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(MserProperties &&properties) TL_NOEXCEPT -> MserProperties &;

    /*!
     * \brief Step size between intensity thresholds for region detection.
     * \return Delta value.
     */
    auto delta() const -> int;

    /*!
     * \brief Minimum area of detected regions.
     * \return Minimum area.
     */
    auto minArea() const -> int;

    /*!
     * \brief Maximum area of detected regions.
     * \return Maximum area.
     */
    auto maxArea() const -> int;

    /*!
     * \brief Maximum variation of the region size relative to its initial size.
     * \return Maximum variation.
     */
    auto maxVariation() const -> double;

    /*!
     * \brief Minimum diversity of region stability.
     * \return Minimum diversity.
     */
    auto minDiversity() const -> double;

    /*!
     * \brief Maximum number of evolution steps.
     * \return Maximum evolution steps.
     */
    auto maxEvolution() const -> int;

    /*!
     * \brief Area threshold used for region filtering.
     * \return Area threshold.
     */
    auto areaThreshold() const -> double;

    /*!
     * \brief Minimum margin for detected regions.
     * \return Minimum margin.
     */
    auto minMargin() const -> double;

    /*!
     * \brief Size of the Gaussian kernel for edge blurring.
     * \return Edge blur size.
     */
    auto edgeBlurSize() const -> int;

    /*!
     * \brief Set the step size between intensity thresholds for region detection.
     * \param[in] delta Delta value.
     */
    void setDelta(int delta);

    /*!
     * \brief Set the minimum area of detected regions.
     * \param[in] minArea Minimum area.
     */
    void setMinArea(int minArea);

    /*!
     * \brief Set the maximum area of detected regions.
     * \param[in] maxArea Maximum area.
     */
    void setMaxArea(int maxArea);

    /*!
     * \brief Set the maximum variation of the region size relative to its initial size.
     * \param[in] maxVariation Maximum variation.
     */
    void setMaxVariation(double maxVariation);

    /*!
     * \brief Set the minimum diversity of region stability.
     * \param[in] minDiversity Minimum diversity.
     */
    void setMinDiversity(double minDiversity);

    /*!
     * \brief Set the maximum number of evolution steps.
     * \param[in] maxEvolution Maximum evolution steps.
     */
    void setMaxEvolution(int maxEvolution);

    /*!
     * \brief Set the area threshold used for region filtering.
     * \param[in] areaThreshold Area threshold.
     */
    void setAreaThreshold(double areaThreshold);

    /*!
     * \brief Set the minimum margin for detected regions.
     * \param[in] minMargin Minimum margin.
     */
    void setMinMargin(double minMargin);

    /*!
     * \brief Set the size of the Gaussian kernel for edge blurring.
     * \param[in] edgeBlurSize Edge blur size.
     */
    void setEdgeBlurSize(int edgeBlurSize);

// Feature interface

public:

    /*!
     * \brief Reset MSER properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief MSER Detector
 *
 * This class implements the MSER (*Maximally Stable Extremal Regions*) keypoint detector.
 * The algorithm detects blobs in an image by identifying regions that remain stable
 * across multiple intensity thresholds.
 *
 * The detector's behavior is controlled via \ref MserProperties, which allows modifying
 * parameters through property names.
 *
 * ### Example Usage
 * \code{.cpp}
 * MserDetector detector;
 * detector.properties().setProperty("MaxArea", 500);
 * int maxArea = detector.properties().property<int>("MaxArea");
 * \endcode
 */
class TL_EXPORT MserDetector
  : public FeatureDetector
{

protected:

    cv::Ptr<cv::MSER> mMSER;
    MserProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the MSER detector with default properties.
     */
    MserDetector();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes MSER using the provided properties.
     *
     * \param[in] properties Configuration properties for MSER.
     */
    explicit MserDetector(const MserProperties &mser);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `MserDetector` instance.
     *
     * \param[in] mser The object to copy.
     */
    MserDetector(const MserDetector &mser);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `MserDetector` instance.
     *
     * \param[in] mser The object to move.
     */
    MserDetector(MserDetector &&mser) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~MserDetector() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `MserDetector` instance.
     *
     * \param[in] mser The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const MserDetector &mser) -> MserDetector &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `MserDetector` instance.
     *
     * \param[in] mser The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(MserDetector &&mser) TL_NOEXCEPT -> MserDetector &;

    /*!
     * \brief Get a reference to the MSER properties.
     *
     * This allows modifying the properties of the MSER detector.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> MserProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the MSER properties.
     *
     * This allows read-only access to the properties of the MSER detector.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const MserProperties &{ return mProperties; }


private:

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using MSER.
     *
     * Processes the given image and extracts keypoints using the MSER algorithm.
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
