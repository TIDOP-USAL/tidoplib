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

#include <memory>

#include <opencv2/objdetect.hpp>

#include "tidop/featmatch/features/features.h"

namespace tl
{

/*! \addtogroup FeatureExtraction
 *
 *  \{
 */

/*!
 * \brief HOG (Histogram of Oriented Gradients) properties class.
 *
 * Stores configuration parameters for the HOG (Histogram of Oriented Gradients) feature descriptor.
 * The HOG descriptor is used for object detection by capturing gradient orientation distributions
 * in localized image regions.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"WinSize"`: Window size for the detection.
 * - `"BlockSize"`: Block size for normalization.
 * - `"BlockStride"`: Step size between adjacent blocks.
 * - `"CellSize"`: Size of each cell within a block.
 * - `"Nbins"`: Number of orientation bins.
 * - `"DerivAperture"`: Aperture parameter for derivative computation.
 */
class TL_EXPORT HogProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes HOG properties with default values.
     */
    HogProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `HogProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    HogProperties(const HogProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `HogProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    HogProperties(HogProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~HogProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `HogProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const HogProperties &properties) -> HogProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `HogProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(HogProperties &&properties) TL_NOEXCEPT -> HogProperties &;

    /*!
     * \brief Gets the detection window size.
     *
     * Defines the size of the window used for feature extraction.
     *
     * \return Window size.
     *
     * \note Property name: `"WinSize"`.
     */
    auto winSize() const->Size<int>;

    /*!
     * \brief Gets the block size for normalization.
     *
     * A block consists of multiple cells and is used for local normalization.
     *
     * \return Block size.
     *
     * \note Property name: `"BlockSize"`.
     */
    auto blockSize() const->Size<int>;

    /*!
     * \brief Gets the block stride.
     *
     * Defines the step size for moving between adjacent blocks.
     *
     * \return Block stride.
     *
     * \note Property name: `"BlockStride"`.
     */
    auto blockStride() const->Size<int>;

    /*!
     * \brief Gets the cell size.
     *
     * A cell is the smallest unit where gradients are computed.
     *
     * \return Cell size.
     *
     * \note Property name: `"CellSize"`.
     */
    auto cellSize() const->Size<int>;

    /*!
     * \brief Gets the number of orientation bins.
     *
     * Defines how many bins are used to represent the gradient orientations.
     *
     * \return Number of bins.
     *
     * \note Property name: `"Nbins"`.
     */
    auto nbins() const -> int;

    /*!
     * \brief Gets the aperture parameter for derivative computation.
     *
     * Used to calculate image gradients.
     *
     * \return Derivative aperture size.
     *
     * \note Property name: `"DerivAperture"`.
     */
    auto derivAperture() const -> int;

    /*!
     * \brief Sets the detection window size.
     *
     * \param[in] winSize Window size.
     */
    void setWinSize(const Size<int> &winSize);

    /*!
     * \brief Sets the block size for normalization.
     *
     * \param[in] blockSize Block size.
     */
    void setBlockSize(const Size<int> &blockSize);

    /*!
     * \brief Sets the block stride.
     *
     * \param[in] blockStride Block stride.
     */
    void setBlockStride(const Size<int> &blockStride);

    /*!
     * \brief Sets the cell size.
     *
     * \param[in] cellSize Cell size.
     */
    void setCellSize(const Size<int> &cellSize);

    /*!
     * \brief Sets the number of orientation bins.
     *
     * \param[in] nbins Number of bins.
     */
    void setNbins(int nbins);

    /*!
     * \brief Sets the aperture parameter for derivative computation.
     *
     * \param[in] derivAperture Derivative aperture size.
     */
    void setDerivAperture(int derivAperture);

// Feature interface

public:

    /*!
     * \brief Resets HOG properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief HOG (Histogram of Oriented Gradients) feature descriptor.
 *
 * Implements the HOG descriptor for feature extraction.
 * The HOG descriptor captures gradient orientation distributions in localized image regions
 * and is widely used for object detection, particularly in pedestrian detection.
 */
class TL_EXPORT HogDescriptor
  : public FeatureDescriptor
{

private:

    std::shared_ptr<cv::HOGDescriptor> mHOG;
    HogProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the HOG descriptor with default properties.
     */
    HogDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes HOG using the provided properties.
     *
     * \param[in] properties Configuration properties for HOG.
     */
    explicit HogDescriptor(const HogProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `HogDescriptor` instance.
     *
     * \param[in] hog The object to copy.
     */
    HogDescriptor(const HogDescriptor &hog);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `HogDescriptor` instance.
     *
     * \param[in] hog The object to move.
     */
    HogDescriptor(HogDescriptor &&hog) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~HogDescriptor() override = default;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `HogDescriptor` instance.
     *
     * \param[in] hog The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const HogDescriptor &hog) -> HogDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `HogDescriptor` instance.
     *
     * \param[in] hog The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(HogDescriptor &&hog) TL_NOEXCEPT -> HogDescriptor &;

    /*!
     * \brief Get a reference to the HOG descriptor properties.
     *
     * This allows modifying the properties of the HOG descriptor properties.
     *
     * \return Reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"WinSize"`: Window size for the detection.
     * - `"BlockSize"`: Block size for normalization.
     * - `"BlockStride"`: Step size between adjacent blocks.
     * - `"CellSize"`: Size of each cell within a block.
     * - `"Nbins"`: Number of orientation bins.
     * - `"DerivAperture"`: Aperture parameter for derivative computation.
     */
    auto properties() -> HogProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the HOG descriptor properties.
     *
     * This allows read-only access to the properties of the HOG descriptor properties.
     *
     * \return Constant Reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"WinSize"`: Window size for the detection.
     * - `"BlockSize"`: Block size for normalization.
     * - `"BlockStride"`: Step size between adjacent blocks.
     * - `"CellSize"`: Size of each cell within a block.
     * - `"Nbins"`: Number of orientation bins.
     * - `"DerivAperture"`: Aperture parameter for derivative computation.
     */
    auto properties() const -> const HogProperties &{ return mProperties; }

private:

    void init();
    void normalizepatch(const cv::Mat &gray,
                        const cv::KeyPoint &keypoint,
                        cv::Mat &output);

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the HOG descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
