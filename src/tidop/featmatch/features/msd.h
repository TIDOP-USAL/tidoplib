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

#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR >= 3 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR >= 1))
#include <opencv2/xfeatures2d.hpp>
#else
class MsdDetector;
#endif


namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */



/*!
 * \brief MSD (Maximal Self-Dissimilarity) feature detector properties.
 *
 * Stores configuration parameters for the MSD (Maximal Self-Dissimilarity) detector.
 * The MSD detector is based on the concept of "contextual self-dissimilarity," which enhances repeatability,
 * distinctiveness, and localization accuracy by identifying highly dissimilar patches in a neighborhood.
 * This concept extends the local self-dissimilarity notion found in corner-like interest point detectors.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"ThresholdSaliency"`: Threshold for saliency.
 * - `"PatchRadius"`: Radius of the image patch used for local descriptor computation.
 * - `"KNN"`: Number of nearest neighbors for feature matching.
 * - `"SearchAreaRadius"`: Radius of the area to search for keypoints.
 * - `"ScaleFactor"`: Scale factor for feature computation.
 * - `"NMSRadius"`: Non-maximum suppression radius for keypoint filtering.
 * - `"NScales"`: Number of scales to use for multi-scale detection.
 * - `"NMSScaleRadius"`: Non-maximum suppression radius for different scales.
 * - `"ComputeOrientation"`: Whether to compute orientation for each keypoint.
 * - `"AffineMSD"`: Whether to use affine-invariant features.
 * - `"AffineTilts"`: Number of affine tilts to use.
 * 
 * \note The MSD algorithm is described in the paper:
 *       F. Tombari, L. Di Stefano, "Interest Points via Maximal Self-Dissimilarities",
 *       12th Asian Conference on Computer Vision (ACCV), 2014.
 */
class TL_EXPORT MsdProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes MSD properties with default values.
     */
    MsdProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `MsdProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    MsdProperties(const MsdProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `MsdProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    MsdProperties(MsdProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~MsdProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `MsdProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const MsdProperties &properties) -> MsdProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `MsdProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(MsdProperties &&properties) TL_NOEXCEPT -> MsdProperties &;

    /*!
     * \brief Gets the threshold for saliency.
     *
     * \return The saliency threshold value.
     * \note Property name: `"ThresholdSaliency"`.
     */
    auto thresholdSaliency() const -> float;

    /*!
     * \brief Gets the patch radius for keypoint extraction.
     *
     * \return The patch radius value.
     * \note Property name: `"PatchRadius"`.
     */
    auto patchRadius() const -> int;

    /*!
     * \brief Gets the number of nearest neighbors for feature matching.
     *
     * \return The KNN value.
     * \note Property name: `"KNN"`.
     */
    auto knn() const -> int;

    /*!
     * \brief Gets the radius of the area to search for keypoints.
     *
     * \return The search area radius value.
     * \note Property name: `"SearchAreaRadius"`.
     */
    auto searchAreaRadius() const -> int;

    /*!
     * \brief Gets the scale factor for feature computation.
     *
     * \return The scale factor.
     * \note Property name: `"ScaleFactor"`.
     */
    auto scaleFactor() const -> float;

    /*!
     * \brief Gets the radius for non-maximum suppression.
     *
     * \return The NMS radius value.
     * \note Property name: `"NMSRadius"`.
     */
    auto NMSRadius() const -> int;

    /*!
     * \brief Gets the number of scales to use for multi-scale detection.
     *
     * \return The number of scales.
     * \note Property name: `"NScales"`.
     */
    auto nScales() const -> int;

    /*!
     * \brief Gets the non-maximum suppression radius for different scales.
     *
     * \return The NMS scale radius.
     * \note Property name: `"NMSScaleRadius"`.
     */
    auto NMSScaleRadius() const -> int;

    /*!
     * \brief Gets whether to compute orientation for each keypoint.
     *
     * \return `true` if orientation is computed, `false` otherwise.
     * \note Property name: `"ComputeOrientation"`.
     */
    auto computeOrientation() const -> bool;

    /*!
     * \brief Gets whether to use affine-invariant features.
     *
     * \return `true` if affine-invariant features are used, `false` otherwise.
     * \note Property name: `"AffineMSD"`.
     */
    auto affineMSD() const -> bool;

    /*!
     * \brief Gets the number of affine tilts to use.
     *
     * \return The number of affine tilts.
     * \note Property name: `"AffineTilts"`.
     */
    auto affineTilts() const -> int;

    /*!
     * \brief Set the threshold for saliency detection
     * \param[in] thresholdSaliency Threshold value
     */
    void setThresholdSaliency(float thresholdSaliency);

    /*!
     * \brief Set the patch radius for saliency computation
     * \param[in] patchRadius Patch radius
     */
    void setPatchRadius(int patchRadius);

    /*!
     * \brief Set the number of nearest neighbors for KNN
     * \param[in] knn Number of nearest neighbors
     */
    void setKNN(int knn);

    /*!
     * \brief Set the search area radius for keypoint detection
     * \param[in] searchAreaRadius Search area radius
     */
    void setSearchAreaRadius(int searchAreaRadius);

    /*!
     * \brief Set the scale factor for multi-scale keypoint detection
     * \param[in] scaleFactor Scale factor
     */
    void setScaleFactor(float scaleFactor);

    /*!
     * \brief Set the non-maximal suppression radius
     * \param[in] NMSRadius NMS radius
     */
    void setNMSRadius(int NMSRadius);

    /*!
     * \brief Set the number of scales for keypoint detection
     * \param[in] nScales Number of scales
     */
    void setNScales(int nScales);

    /*!
     * \brief Set the NMS scale radius
     * \param[in] NMSScaleR NMS scale radius
     */
    void setNMSScaleRadius(int NMSScaleR);

    /*!
     * \brief Set whether to compute orientation of keypoints
     * \param[in] computeOrientation True if orientation is computed
     */
    void setComputeOrientation(bool computeOrientation);

    /*!
     * \brief Set whether to apply affine MSD transformation
     * \param[in] affineMSD True if affine MSD is applied
     */
    void setAffineMSD(bool affineMSD);

    /*!
     * \brief Set the number of affine tilts
     * \param[in] affineTilts Number of affine tilts
     */
    void setAffineTilts(int affineTilts);

// Feature interface

public:

    /*!
     * \brief Resets MSD properties to their default values.
     */
    void reset() override;

};

/*!
 * \brief MSD Detector
 *
 * This class implements the MSD (*Maximal Self-Dissimilarity*) keypoint detector.
 * The algorithm detects keypoints by identifying image patches that are highly dissimilar
 * over their surroundings. It enhances repeatability, distinctiveness, and localization accuracy
 * by utilizing the concept of "contextual self-dissimilarity".
 *
 * \note The MSD algorithm is described in the paper:
 *       F. Tombari, L. Di Stefano, "Interest Points via Maximal Self-Dissimilarities",
 *       12th Asian Conference on Computer Vision (ACCV), 2014.
 */
class MsdDetector
  : public FeatureDetector
{

protected:

#if defined HAVE_OPENCV_XFEATURES2D && (CV_VERSION_MAJOR >= 3 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR >= 1))
    cv::Ptr<cv::xfeatures2d::MSDDetector> mMSD;
#else
    std::shared_ptr<::MsdDetector> mMSD;
#endif
    MsdProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the MSD detector with default properties.
     */
    MsdDetector();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes MSD using the provided properties.
     *
     * \param[in] properties Configuration properties for MSD.
     */
    explicit MsdDetector(const MsdProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `MsdDetector` instance.
     *
     * \param[in] msd The object to copy.
     */
    MsdDetector(const MsdDetector &msd);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `MsdDetector` instance.
     *
     * \param[in] msd The object to move.
     */
    MsdDetector(MsdDetector &&msd) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~MsdDetector() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `MsdDetector` instance.
     *
     * \param[in] msd The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const MsdDetector &msd) -> MsdDetector &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `MsdDetector` instance.
     *
     * \param[in] msd The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(MsdDetector &&msd) TL_NOEXCEPT -> MsdDetector &;

    /*!
     * \brief Get a reference to the MSD detector properties.
     *
     * This allows modifying the properties of the MSD detector.
     *
     * \return Reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"ThresholdSaliency"`
     * - `"PatchRadius"`
     * - `"KNN"`
     * - `"SearchAreaRadius"`
     * - `"ScaleFactor"`
     * - `"NMSRadius"`
     * - `"NScales"`
     * - `"NMSScaleRadius"`
     * - `"ComputeOrientation"`
     * - `"AffineMSD"`
     * - `"AffineTilts"`
     */
    auto properties() -> MsdProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the MSD detector properties.
     *
     * This allows read-only access to the properties of the MSD detector.
     *
     * \return Constant reference to the properties object.
     * 
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"ThresholdSaliency"`
     * - `"PatchRadius"`
     * - `"KNN"`
     * - `"SearchAreaRadius"`
     * - `"ScaleFactor"`
     * - `"NMSRadius"`
     * - `"NScales"`
     * - `"NMSScaleRadius"`
     * - `"ComputeOrientation"`
     * - `"AffineMSD"`
     * - `"AffineTilts"`
     */
    auto properties() const -> const MsdProperties &{ return mProperties; }

private:

    void init();

#if CV_VERSION_MAJOR < 3 || (CV_VERSION_MAJOR == 3 && CV_VERSION_MINOR < 1) || !defined HAVE_OPENCV_XFEATURES2D

    bool pointIsAcceptable(const cv::KeyPoint &vl_keypoint, int width, int height);
    void compensate_affine_coor1(float *x0, float *y0, int w1, int h1, float t1, float t2, float Rtheta);
    void affineSkew(double tilt, double phi, cv::Mat &img, cv::Mat &mask, cv::Mat &Ai);

#endif

// FeatureDetector interface

public:

    /*!
     * \brief Detects keypoints in an image using the MSD detector.
     *
     * Processes the given image and extracts keypoints using the MSD algorithm.
     * If a mask is provided, keypoints will only be detected in unmasked regions.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] mask Optional mask to specify regions of interest (default = no mask).
     * \return A vector of detected keypoints.
     */
    auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> override;

};

/*! \} */

} // namespace tl

