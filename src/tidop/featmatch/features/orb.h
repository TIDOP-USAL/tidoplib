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

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D


namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */


/*!
 * \class OrbProperties
 * \brief Stores properties for the ORB feature detector and descriptor.
 *
 * Properties can be accessed directly or by name using \ref getProperty and \ref setProperty.
 * Available property names:
 * - "FeaturesNumber"
 * - "ScaleFactor"
 * - "LevelsNumber"
 * - "EdgeThreshold"
 * - "FirstLevel"
 * - "WTA_K"
 * - "ScoreType"
 * - "PatchSize"
 * - "FastThreshold"
 */
class TL_EXPORT OrbProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes ORB properties with default values.
     */
    OrbProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `OrbProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    OrbProperties(const OrbProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `OrbProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    OrbProperties(OrbProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~OrbProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `OrbProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator=(const OrbProperties &properties) -> OrbProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `OrbProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator=(OrbProperties &&properties) TL_NOEXCEPT -> OrbProperties &;

    /*!
     * \brief Get the maximum number of features to retain.
     */
    auto featuresNumber() const -> int;

    /*!
     * \brief Get the pyramid decimation ratio.
     *
     * A scale factor of 2 means a classical pyramid, where each level
     * has 4x fewer pixels than the previous one. However, large scale
     * factors degrade feature matching quality.
     */
    auto scaleFactor() const -> float;

    /*!
     * \brief Get the number of pyramid levels.
     */
    auto levelsNumber() const -> int;

    /*!
     * \brief Get the size of the border where features are not detected.
     */
    auto edgeThreshold() const -> int;

    /*!
     * \brief Get the pyramid level at which the source image is placed.
     */
    auto firstLevel() const -> int;

    /*!
     * \brief Get the number of points that produce each ORB descriptor element.
     */
    auto wta_k() const -> int;

    /*!
     * \brief Get the scoring type.
     * \return Either "HARRIS_SCORE" or "FAST_SCORE".
     */
    auto scoreType() const->std::string;

    /*!
     * \brief Get the patch size used for the descriptor.
     */
    auto patchSize() const -> int;

    /*!
     * \brief Get the fast threshold.
     */
    auto fastThreshold() const -> int;

    /*!
     * \brief Set the number of features to retain.
     */
    void setFeaturesNumber(int featuresNumber);

    /*!
     * \brief Set the pyramid scale factor.
     */
    void setScaleFactor(float scaleFactor);

    /*!
     * \brief Set the number of pyramid levels.
     */
    void setLevelsNumber(int levelsNumber);

    /*!
     * \brief Set the edge threshold.
     */
    void setEdgeThreshold(int edgeThreshold);

    /*!
     * \brief Set the first level of the pyramid.
     */
    void setFirstLevel(int firstLevel);

    /*!
     * \brief Set the number of points used in the descriptor.
     * The default is 2, but it can also be 3 or 4.
     */
    void setWTA_K(int WTA_K);

    /*!
     * \brief Set the scoring type.
     * \param[in] scoreType Possible values: "HARRIS_SCORE", "FAST_SCORE".
     */
    void setScoreType(const std::string &scoreType);

    /*!
     * \brief Set the patch size.
     */
    void setPatchSize(int patchSize);

    /*!
     * \brief Set the fast threshold.
     */
    void setFastThreshold(int fastThreshold);

// Feature interface

public:

    /*!
     * \brief Reset ORB properties to default values.
     */
    void reset() override;

};



/*!
 * \brief ORB (Oriented FAST and Rotated BRIEF) feature detector and descriptor.
 *
 * The ORB detector and descriptor provide a fast and efficient alternative to SIFT and SURF.
 * ORB is rotation-invariant and uses a combination of FAST keypoint detection and BRIEF descriptors.
 *
 * This class encapsulates OpenCV's ORB implementation, allowing keypoint detection and descriptor extraction.
 */
class TL_EXPORT OrbDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

protected:

    cv::Ptr<cv::ORB> mOrb;
    OrbProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the ORB detector and descriptor with default properties.
     */
    OrbDetectorDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes ORB using the provided properties.
     *
     * \param[in] properties Configuration properties for ORB.
     */
    explicit OrbDetectorDescriptor(const OrbProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `OrbDetectorDescriptor` instance.
     *
     * \param[in] orb The object to copy.
     */
    OrbDetectorDescriptor(const OrbDetectorDescriptor &orb);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `OrbDetectorDescriptor` instance.
     *
     * \param[in] orb The object to move.
     */
    OrbDetectorDescriptor(OrbDetectorDescriptor &&orb) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~OrbDetectorDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `OrbDetectorDescriptor` instance.
     *
     * \param[in] orb The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const OrbDetectorDescriptor &orb)->OrbDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `OrbDetectorDescriptor` instance.
     *
     * \param[in] orb The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(OrbDetectorDescriptor &&orb) TL_NOEXCEPT->OrbDetectorDescriptor &;

    /*!
     * \brief Get a reference to the ORB properties.
     *
     * This allows modifying the properties of the ORB detector/descriptor.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> OrbProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the ORB properties.
     *
     * This allows read-only access to the properties of the ORB detector/descriptor.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const OrbProperties &{ return mProperties; }

private:

    void init();

#if CV_VERSION_MAJOR >= 4
    auto convertScoreType(const std::string &scoreType) -> cv::ORB::ScoreType;
#else
    auto convertScoreType(const std::string &scoreType) -> int;
#endif

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using ORB.
     *
     * Processes the given image and extracts keypoints using the ORB algorithm.
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
     * Extracts binary descriptors for the given keypoints using the ORB descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img,
                 std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;


};




/*!
 * \brief ORB feature detector and descriptor using CUDA acceleration.
 *
 * This class uses OpenCV's CUDA-accelerated ORB implementation to improve performance
 * on compatible hardware. It is particularly useful for processing large images or
 * real-time applications.
 *
 * \note Requires OpenCV compiled with CUDA support.
 */
class TL_EXPORT OrbCudaDetectorDescriptor
  : public FeatureDetector,
    public FeatureDescriptor
{

protected:

#ifdef HAVE_OPENCV_CUDAFEATURES2D  
    cv::Ptr<cv::cuda::ORB> mOrb;
#endif // HAVE_OPENCV_CUDAFEATURES2D
    OrbProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the ORB detector and descriptor with default properties.
     */
    OrbCudaDetectorDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes ORB using the provided properties.
     *
     * \param[in] properties Configuration properties for ORB.
     */
    explicit OrbCudaDetectorDescriptor(const OrbProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `OrbCudaDetectorDescriptor` instance.
     *
     * \param[in] orb The object to copy.
     */
    OrbCudaDetectorDescriptor(const OrbCudaDetectorDescriptor &orb);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `OrbCudaDetectorDescriptor` instance.
     *
     * \param[in] orb The object to move.
     */
    OrbCudaDetectorDescriptor(OrbCudaDetectorDescriptor &&orb) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~OrbCudaDetectorDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `OrbCudaDetectorDescriptor` instance.
     *
     * \param[in] orb The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const OrbCudaDetectorDescriptor &orb) -> OrbCudaDetectorDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `OrbCudaDetectorDescriptor` instance.
     *
     * \param[in] orb The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(OrbCudaDetectorDescriptor &&orb) TL_NOEXCEPT -> OrbCudaDetectorDescriptor &;

    /*!
     * \brief Get a reference to the ORB properties.
     *
     * This allows modifying the properties of the ORB detector/descriptor.
     *
     * \return Reference to the properties object.
     */
    auto properties() -> OrbProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the ORB properties.
     *
     * This allows read-only access to the properties of the ORB detector/descriptor.
     *
     * \return Constant reference to the properties object.
     */
    auto properties() const -> const OrbProperties &{ return mProperties; }

private:

    int convertScoreType(const std::string &scoreType);

    void init();

// FeatureDetector interface

public:

    /*!
     * \brief Detect keypoints in an image using ORB.
     *
     * Processes the given image and extracts keypoints using the ORB algorithm.
     * If a mask is provided, keypoints will only be detected in unmasked regions.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] mask Optional mask to specify regions of interest (default = no mask).
     * \return A vector of detected keypoints.
     */
    std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                     cv::InputArray &mask = cv::noArray()) override;

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the ORB descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    cv::Mat extract(const cv::Mat &img,
                    std::vector<cv::KeyPoint> &keyPoints) override;

};


/*! \} */ 

} // namespace tl

