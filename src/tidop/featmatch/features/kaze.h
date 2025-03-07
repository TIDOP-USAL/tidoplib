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

#include "tidop/config.h"

#include "tidop/featmatch/features/features.h"

namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */


/*!
 * \brief KAZE feature properties.
 *
 * Stores configuration parameters for the KAZE feature detection and description algorithm.
 * KAZE is a multiscale 2D feature detection and description method based on nonlinear scale spaces.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"ExtendedDescriptor"`: Use extended 128-element descriptors (true) or 64-element descriptors (false, default).
 * - `"UprightDescriptor"`: Use upright descriptors (non-rotation-invariant).
 * - `"Threshold"`: Detector response threshold to accept a keypoint.
 * - `"Octaves"`: Maximum number of image octaves.
 * - `"OctaveLayers"`: Number of sublevels per scale level.
 * - `"Diffusivity"`: Diffusivity type (`DIFF_PM_G1`, `DIFF_PM_G2` (default), `DIFF_WEICKERT`, `DIFF_CHARBONNIER`).
 *
 * Reference:
 * Alcantarilla P.F., Bartoli A., Davison A.J. (2012) KAZE Features.
 * In: Fitzgibbon A., Lazebnik S., Perona P., Sato Y., Schmid C. (eds)
 * Computer Vision – ECCV 2012. ECCV 2012. Lecture Notes in Computer Science, vol 7577. Springer, Berlin, Heidelberg.
 *
 * \see https://www.doc.ic.ac.uk/~ajd/Publications/alcantarilla_etal_eccv2012.pdf
 */
class TL_EXPORT KazeProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes KAZE properties with default values.
     */
    KazeProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `KazeProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    KazeProperties(const KazeProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `KazeProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    KazeProperties(KazeProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~KazeProperties() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `KazeProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const KazeProperties &properties)->KazeProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `KazeProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(KazeProperties &&properties) TL_NOEXCEPT->KazeProperties &;

    /*!
     * \brief Checks if the extended descriptor is enabled.
     *
     * \return `true` if using extended 128-element descriptors, `false` if using 64-element descriptors (default).
     *
     * \note Property name: `"ExtendedDescriptor"`.
     */
    auto extendedDescriptor() const -> bool;

    /*!
     * \brief Checks if upright descriptors are used (non-rotation-invariant).
     *
     * \return `true` if using upright descriptors, `false` otherwise (default).
     *
     * \note Property name: `"UprightDescriptor"`.
     */
    auto uprightDescriptor() const -> bool;

    /*!
     * \brief Gets the detector response threshold for keypoint acceptance.
     *
     * \return The threshold value.
     *
     * \note Property name: `"Threshold"`.
     */
    auto threshold() const -> float;

    /*!
     * \brief Gets the maximum number of octaves for image evolution.
     *
     * \return The number of octaves.
     *
     * \note Property name: `"Octaves"`.
     */
    auto octaves() const -> int;

    /*!
     * \brief Gets the number of sublevels per scale level.
     *
     * \return The number of octave layers.
     *
     * \note Property name: `"OctaveLayers"`.
     */
    auto octaveLayers() const -> int;

    /*!
     * \brief Gets the diffusivity type.
     *
     * \return One of the following diffusivity types:
     * - `"DIFF_PM_G1"`
     * - `"DIFF_PM_G2"` (default)
     * - `"DIFF_WEICKERT"`
     * - `"DIFF_CHARBONNIER"`
     *
     * \note Property name: `"Diffusivity"`.
     */
    auto diffusivity() const->std::string;

    /*!
     * \brief Enables or disables the extended descriptor.
     *
     * \param[in] extended `true` for 128-element descriptors, `false` for 64-element descriptors (default).
     */
    void setExtendedDescriptor(bool extended);

    /*!
     * \brief Enables or disables upright descriptors.
     *
     * \param[in] upright `true` to use upright descriptors, `false` otherwise.
     */
    void setUprightDescriptor(bool uprightDescriptor);

    /*!
     * \brief Sets the detector response threshold for keypoint acceptance.
     *
     * \param[in] threshold The threshold value.
     */
    void setThreshold(float threshold);

    /*!
     * \brief Sets the maximum number of octaves for image evolution.
     *
     * \param[in] octaves The number of octaves.
     */
    void setOctaves(int octaves);

    /*!
     * \brief Sets the number of sublevels per scale level.
     *
     * \param[in] octaveLayers The number of octave layers.
     */
    void setOctaveLayers(int octaveLayers);

    /*!
     * \brief Sets the diffusivity type.
     *
     * \param[in] diffusivity One of `"DIFF_PM_G1"`, `"DIFF_PM_G2"`, `"DIFF_WEICKERT"`, or `"DIFF_CHARBONNIER"`.
     */
    void setDiffusivity(const std::string &diffusivity);

// Feature interface

public:

    /*!
     * \brief Resets KAZE properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief KAZE feature detector and descriptor.
 *
 * Implements the KAZE algorithm for keypoint detection and descriptor extraction.
 * KAZE is a nonlinear scale-space-based method that provides robust and distinctive features.
 */
class TL_EXPORT KazeDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

protected:

    cv::Ptr<cv::KAZE> mKaze;
    KazeProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the KAZE detector and descriptor with default properties.
     */
    KazeDetectorDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes KAZE using the provided properties.
     *
     * \param[in] properties Configuration properties for KAZE.
     */
    explicit KazeDetectorDescriptor(const KazeProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `KazeDetectorDescriptor` instance.
     *
     * \param[in] kaze The object to copy.
     */
    KazeDetectorDescriptor(const KazeDetectorDescriptor &kaze);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `KazeDetectorDescriptor` instance.
     *
     * \param[in] kaze The object to move.
     */
    KazeDetectorDescriptor(KazeDetectorDescriptor &&kaze) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~KazeDetectorDescriptor() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `KazeDetectorDescriptor` instance.
     *
     * \param[in] kaze The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const KazeDetectorDescriptor &kaze) -> KazeDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `KazeDetectorDescriptor` instance.
     *
     * \param[in] kaze The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(KazeDetectorDescriptor &&kaze) TL_NOEXCEPT -> KazeDetectorDescriptor &;

    /*!
     * \brief Get a reference to the KAZE properties.
     *
     * This allows modifying the properties of the KAZE detector/descriptor.
     *
     * \return Reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"ExtendedDescriptor"`
     * - `"UprightDescriptor"`
     * - `"Threshold"`
     * - `"Octaves"`
     * - `"OctaveLayers"`
     * - `"Diffusivity"`
     */
    auto properties() -> KazeProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the KAZE properties.
     *
     * This allows read-only access to the properties of the KAZE detector/descriptor.
     *
     * \return Constant reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"ExtendedDescriptor"`
     * - `"UprightDescriptor"`
     * - `"Threshold"`
     * - `"Octaves"`
     * - `"OctaveLayers"`
     * - `"Diffusivity"`
     */
    auto properties() const -> const KazeProperties &{ return mProperties; }

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDiffusivity(const std::string &diffusivity) -> cv::KAZE::DiffusivityType;
#else
    auto convertDiffusivity(const std::string &diffusivity) -> int;
#endif

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detects keypoints in an image using KAZE.
     *
     * Processes the given image and extracts keypoints using the KAZE algorithm.
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
     * \brief Extracts KAZE descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the A-KAZE descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
