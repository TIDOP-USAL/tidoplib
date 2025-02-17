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


constexpr auto akaze_default_value_descriptor_type{"MLDB"};
constexpr auto akaze_default_value_descriptor_size{0};
constexpr auto akaze_default_value_descriptor_channels{3};
constexpr auto akaze_default_value_threshold{0.001};
constexpr auto akaze_default_value_octaves{4};
constexpr auto akaze_default_value_octave_layers{4};
constexpr auto akaze_default_value_diffusivity{"DIFF_PM_G2"};


/*!
 * \brief A-KAZE properties
 *
 * This class defines the properties for the A-KAZE (Accelerated-KAZE) feature detector and descriptor.
 * A-KAZE is an efficient variant of KAZE that uses a modified nonlinear diffusion approach to enhance
 * speed while maintaining robustness in feature detection and description.
 *
 * The properties defined in this class control various parameters for A-KAZE keypoint detection and
 * descriptor extraction.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"DescriptorType"`
 * - `"DescriptorSize"`
 * - `"DescriptorChannels"`
 * - `"Threshold"`
 * - `"Octaves"`
 * - `"OctaveLayers"`
 * - `"Diffusivity"`
 * 
 * \note Requires OpenCV with A-KAZE support.
 *
 * Reference:
 * - Pablo F. Alcantarilla, Jesús Nuevo, Adrien Bartoli. (2013). Fast Explicit Diffusion
 *   for Accelerated Features in Nonlinear Scale Spaces. British Machine Vision Conference (BMVC).
 *   [Link](https://projet.liris.cnrs.fr/imagine/pub/proceedings/BMVC-2013/Papers/paper0013/paper0013.pdf)
 */
class TL_EXPORT AkazeProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes A-KAZE properties with default values.
     */
    AkazeProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `AkazeProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    AkazeProperties(const AkazeProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `AkazeProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    AkazeProperties(AkazeProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~AkazeProperties() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `AkazeProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const AkazeProperties &properties) -> AkazeProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `AkazeProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(AkazeProperties &&properties) TL_NOEXCEPT -> AkazeProperties &;

    /*!
     * \brief Get the type of the extracted descriptor.
     *
     * Available descriptor types:
     * - **KAZE**: Standard KAZE descriptor.
     * - **KAZE_UPRIGHT**: KAZE descriptor without rotation invariance.
     * - **MLDB**: M-LDB binary descriptor (default).
     * - **MLDB_UPRIGHT**: M-LDB descriptor without rotation invariance.
     *
     * \return Descriptor type as a string.
     * 
     * \note Property name: `"DescriptorType"`.
     */
    auto descriptorType() const -> std::string;

    /*!
     * \brief Get the size of the descriptor in bits.
     *
     * The descriptor size determines the number of bits used for the feature description.
     * A value of 0 results in the full descriptor size.
     *
     * \return Descriptor size in bits.
     *
     * \note Property name: `"DescriptorSize"`.
     */
    auto descriptorSize() const -> int;

    /*!
     * \brief Get the number of channels in the descriptor.
     *
     * This value determines how many channels are used for descriptor computation.
     * Possible values:
     * - **1**: Intensity only.
     * - **2**: Intensity and gradient magnitude.
     * - **3**: Intensity, gradient magnitude, and gradient orientation (default).
     *
     * \return Number of descriptor channels.
     *
     * \note Property name: `"DescriptorChannels"`.
     */
    auto descriptorChannels() const -> int;

    /*!
     * \brief Get the detector response threshold.
     *
     * This threshold determines the minimum response required to accept a keypoint.
     * Higher values result in fewer keypoints being detected.
     *
     * \return Threshold value.
     *
     * \note Property name: `"Threshold"`.
     */
    auto threshold() const -> double;

    /*!
     * \brief Get the number of octaves used in the scale space.
     *
     * Defines the number of scale levels used for detecting features across different
     * resolutions of the image.
     *
     * \return Number of octaves.
     *
     * \note Property name: `"Octaves"`.
     */
    auto octaves() const -> int;

    /*!
     * \brief Get the number of sublevels per octave.
     *
     * Determines how many intermediate scale levels are computed within each octave.
     *
     * \return Number of sublevels per octave.
     *
     * \note Property name: `"OctaveLayers"`.
     */
    auto octaveLayers() const -> int;

    /*!
     * \brief Get the type of diffusion process used in the scale space.
     *
     * Available diffusivity types:
     * - **DIFF_PM_G1**: Perona-Malik equation with \( g1 \) diffusivity.
     * - **DIFF_PM_G2**: Perona-Malik equation with \( g2 \) diffusivity (default).
     * - **DIFF_WEICKERT**: Weickert diffusivity function.
     * - **DIFF_CHARBONNIER**: Charbonnier diffusivity function.
     *
     * \return Diffusivity type as a string.
     *
     * \note Property name: `"Diffusivity"`.
     */
    auto diffusivity() const -> std::string;

    /*!
     * \brief Set the descriptor type.
     *
     * \param[in] descriptorType Descriptor type (KAZE, KAZE_UPRIGHT, MLDB, MLDB_UPRIGHT).
     */
    void setDescriptorType(const std::string &descriptorType);

    /*!
     * \brief Set the descriptor size in bits.
     *
     * A value of 0 results in the full descriptor size.
     *
     * \param[in] descriptorSize Descriptor size in bits.
     */
    void setDescriptorSize(int descriptorSize);

    /*!
     * \brief Set the number of descriptor channels.
     *
     * Available values:
     * - **1**: Intensity only.
     * - **2**: Intensity and gradient magnitude.
     * - **3**: Intensity, gradient magnitude, and gradient orientation (default).
     *
     * \param[in] channels Number of descriptor channels.
     */
    void setDescriptorChannels(int channels);

    /*!
     * \brief Set the detector response threshold.
     *
     * Higher values result in fewer detected keypoints.
     *
     * \param[in] threshold Threshold value.
     */
    void setThreshold(double threshold);

    /*!
     * \brief Set the number of octaves for scale space computation.
     *
     * \param[in] octaves Number of octaves.
     */
    void setOctaves(int octaves);

    /*!
     * \brief Set the number of sublevels per octave.
     *
     * \param[in] octaveLayers Number of sublevels.
     */
    void setOctaveLayers(int octaveLayers);

    /*!
     * \brief Set the diffusivity type.
     *
     * Available values:
     * - **DIFF_PM_G1**
     * - **DIFF_PM_G2** (default)
     * - **DIFF_WEICKERT**
     * - **DIFF_CHARBONNIER**
     *
     * \param[in] diffusivity Diffusivity type as a string.
     */
    void setDiffusivity(const std::string &diffusivity);

// Feature interface

public:

    /*!
     * \brief Reset A-KAZE properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief A-KAZE Detector and Descriptor
 *
 * This class implements the A-KAZE (Accelerated-KAZE) algorithm for both feature detection
 * and descriptor extraction. A-KAZE is an efficient variant of KAZE that uses a modified
 * nonlinear diffusion approach to enhance speed while maintaining robustness in keypoint
 * detection and description.
 *
 * It inherits from both `FeatureDetector` and `FeatureDescriptor`, allowing it to be used
 * for detecting keypoints and extracting descriptors within the same framework.
 *
 * \note Requires OpenCV with A-KAZE support.
 *
 * Reference:
 * - Pablo F. Alcantarilla, Jesús Nuevo, Adrien Bartoli. (2013). Fast Explicit Diffusion
 *   for Accelerated Features in Nonlinear Scale Spaces. British Machine Vision Conference (BMVC).
 *   [Link](https://projet.liris.cnrs.fr/imagine/pub/proceedings/BMVC-2013/Papers/paper0013/paper0013.pdf)
 */
class TL_EXPORT AkazeDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

private:

    cv::Ptr<cv::AKAZE> mAkaze;
    AkazeProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the A-KAZE detector and descriptor with default properties.
     */
    AkazeDetectorDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes A-KAZE using the provided properties.
     *
     * \param[in] properties Configuration properties for A-KAZE.
     */
    explicit AkazeDetectorDescriptor(const AkazeProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `AkazeDetectorDescriptor` instance.
     *
     * \param[in] akaze The object to copy.
     */
    AkazeDetectorDescriptor(const AkazeDetectorDescriptor &akaze);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `AkazeDetectorDescriptor` instance.
     *
     * \param[in] akaze The object to move.
     */
    AkazeDetectorDescriptor(AkazeDetectorDescriptor &&akaze) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~AkazeDetectorDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `AkazeDetectorDescriptor` instance.
     *
     * \param[in] akaze The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const AkazeDetectorDescriptor &akaze) -> AkazeDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `AkazeDetectorDescriptor` instance.
     *
     * \param[in] akaze The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(AkazeDetectorDescriptor &&akaze) TL_NOEXCEPT -> AkazeDetectorDescriptor &;

    /*!
     * \brief Get a reference to the A-KAZE properties.
     * 
     * This allows modifying the properties of the A-KAZE detector/descriptor.
     * 
     * \return Reference to the properties object.
     */
    auto properties() -> AkazeProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the A-KAZE properties.
     *
     * This allows read-only access to the properties of the A-KAZE detector/descriptor.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const AkazeProperties &{ return mProperties; }

private:

#if CV_VERSION_MAJOR >= 4
    auto convertDescriptorType(const std::string &descriptorType) -> cv::AKAZE::DescriptorType;
    auto convertDiffusivity(const std::string &diffusivity) -> cv::KAZE::DiffusivityType;
#else
    auto convertDescriptorType(const std::string &descriptorType) -> int;
    auto convertDiffusivity(const std::string &diffusivity) -> int;
#endif

    void initAkazeFromProperties();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using A-KAZE.
     *
     * Processes the given image and extracts keypoints using the A-KAZE algorithm.
     * If a mask is provided, keypoints will only be detected in unmasked regions.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] mask Optional mask to specify regions of interest (default = no mask).
     * \return A vector of detected keypoints.
     */
    auto detect(const cv::Mat &img,
                cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the A-KAZE descriptor.
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

