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
#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/xfeatures2d/cuda.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */

constexpr auto surf_default_value_hessian_threshold{100};
constexpr auto surf_default_value_octaves{4};
constexpr auto surf_default_value_octave_layers{3};
constexpr auto surf_default_value_extended_descriptor{false};
constexpr auto surf_default_value_upright{false};


/*!
 * \class SurfProperties
 * \brief Stores properties for the SURF feature detector and descriptor.
 *
 * Properties can be accessed directly or by name using \ref getProperty and \ref setProperty.
 * Available property names:
 * - "HessianThreshold"
 * - "Octaves"
 * - "OctaveLayers"
 * - "ExtendedDescriptor"
 * - "Upright"
 */
class TL_EXPORT SurfProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes SURF properties with default values.
     */
    SurfProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `SurfProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    SurfProperties(const SurfProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `SurfProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    SurfProperties(SurfProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~SurfProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `SurfProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const SurfProperties &properties) -> SurfProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `SurfProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(SurfProperties &&properties) TL_NOEXCEPT -> SurfProperties &;

    /*!
     * \brief Get the Hessian threshold for the keypoint detector.
     *
     * The threshold controls the number of detected keypoints. A higher value results
     * in fewer keypoints.
     * \return Hessian threshold.
     */
    auto hessianThreshold() const -> double;

    /*!
     * \brief Get the number of Gaussian pyramid octaves.
     *
     * Increasing the number of octaves detects larger features.
     * \return Number of octaves.
     */
    auto octaves() const -> int;

    /*!
     * \brief Get the number of octave layers within each octave.
     *
     * A higher number provides better scale resolution.
     * \return Number of octave layers.
     */
    auto octaveLayers() const -> int;

    /*!
     * \brief Check if the extended descriptor is enabled.
     *
     * \return True (128-element descriptor) or false (64-element descriptor).
     */
    auto extendedDescriptor() const -> bool;

    /*!
     * \brief Check if the detector is operating in upright mode.
     *
     * If enabled, orientation computation is disabled.
     * \return True (no orientation computation) or false (orientation computed).
     */
    auto upright() const -> bool;

    /*!
     * \brief Set the Hessian threshold.
     * \param[in] hessianThreshold Threshold for Hessian-based keypoint detection.
     */
    void setHessianThreshold(double hessianThreshold);

    /*!
     * \brief Set the number of Gaussian pyramid octaves.
     * \param[in] octaves Number of octaves.
     */
    void setOctaves(int octaves);

    /*!
     * \brief Set the number of octave layers per octave.
     * \param[in] octaveLayers Number of octave layers.
     */
    void setOctaveLayers(int octaveLayers);

    /*!
     * \brief Enable or disable the extended descriptor.
     * \param[in] extendedDescriptor True for 128-element descriptors, false for 64-element descriptors.
     */
    void setExtendedDescriptor(bool extendedDescriptor);

    /*!
     * \brief Enable or disable upright mode.
     * \param[in] upright True to disable orientation computation.
     */
    void setUpright(bool upright);

// Feature interface

public:

    /*!
     * \brief Reset SURF properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief SURF feature detector and descriptor implementation.
 *
 * This class detects keypoints and computes descriptors using the SURF algorithm.
 */
class TL_EXPORT SurfDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

private:

#if defined HAVE_OPENCV_XFEATURES2D  && defined OPENCV_ENABLE_NONFREE
    cv::Ptr<cv::xfeatures2d::SURF> mSurf;
#endif // HAVE_OPENCV_XFEATURES2D
    SurfProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the SURF detector and descriptor with default properties.
     */
    SurfDetectorDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes SURF using the provided properties.
     *
     * \param[in] properties Configuration properties for SURF.
     */
    explicit SurfDetectorDescriptor(const SurfProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `SurfDetectorDescriptor` instance.
     *
     * \param[in] surf The object to copy.
     */
    SurfDetectorDescriptor(const SurfDetectorDescriptor &surf);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `SurfDetectorDescriptor` instance.
     *
     * \param[in] surf The object to move.
     */
    SurfDetectorDescriptor(SurfDetectorDescriptor &&surf) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~SurfDetectorDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `SurfDetectorDescriptor` instance.
     *
     * \param[in] surf The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const SurfDetectorDescriptor &surf) -> SurfDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `SurfDetectorDescriptor` instance.
     *
     * \param[in] surf The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(SurfDetectorDescriptor &&surf) TL_NOEXCEPT -> SurfDetectorDescriptor &;

    /*!
     * \brief Get a reference to the SURF properties.
     *
     * This allows modifying the properties of the SURF detector/descriptor.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> SurfProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the SURF properties.
     *
     * This allows read-only access to the properties of the SURF detector/descriptor.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const SurfProperties &{ return mProperties; }

private:

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using SURF.
     *
     * Processes the given image and extracts keypoints using the SURF algorithm.
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
     * Extracts binary descriptors for the given keypoints using the SURF descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img,
                 std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};




/*!
 * \class SurfCudaDetectorDescriptor
 * \brief CUDA-accelerated SURF detector and descriptor.
 *
 * This class uses OpenCV's CUDA implementation of SURF to provide faster keypoint
 * detection and descriptor extraction.
 */
class TL_EXPORT SurfCudaDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

protected:

#if defined HAVE_OPENCV_XFEATURES2D && defined HAVE_OPENCV_CUDAFEATURES2D && defined OPENCV_ENABLE_NONFREE
    std::unique_ptr<cv::cuda::SURF_CUDA> mSurf;
#endif
    SurfProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     */
    SurfCudaDetectorDescriptor();

    /*!
     * \brief Constructor with properties.
     * \param[in] properties SURF CUDA detector and descriptor properties.
     */
    explicit SurfCudaDetectorDescriptor(const SurfProperties &properties);

    /*!
     * \brief Copy constructor.
     */
    SurfCudaDetectorDescriptor(const SurfCudaDetectorDescriptor &surf);

    /*!
     * \brief Move constructor.
     */
    SurfCudaDetectorDescriptor(SurfCudaDetectorDescriptor &&surf) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~SurfCudaDetectorDescriptor() override = default;

    /*!
     * \brief Assignment operator.
     */
    auto operator=(const SurfCudaDetectorDescriptor &surf) -> SurfCudaDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     */
    auto operator=(SurfCudaDetectorDescriptor &&surf) TL_NOEXCEPT -> SurfCudaDetectorDescriptor &;

    /*!
     * \brief Get the properties of the SURF detector and descriptor.
     * \return Reference to the properties.
     */
    auto properties() -> SurfProperties &{ return mProperties; }

    /*!
     * \brief Get the properties of the SURF detector and descriptor (const).
     * \return Const reference to the properties.
     */
    auto properties() const -> const SurfProperties &{ return mProperties; }

private:

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detects keypoints in the input image using CUDA.
     * \param[in] img Input image.
     * \param[in] mask Optional mask to specify valid keypoint regions.
     * \return Vector of detected keypoints.
     */
    std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                     cv::InputArray &mask = cv::noArray()) override;

// FeatureDescriptor interface

public:

    /*!
     * \brief Extracts descriptors using CUDA.
     * \param[in] img Input image.
     * \param[in,out] keyPoints Keypoints for which descriptors will be computed.
     * \return Descriptor matrix.
     */
    cv::Mat extract(const cv::Mat &img,
                    std::vector<cv::KeyPoint> &keyPoints) override;

};


/*! \} */

} // namespace tl

