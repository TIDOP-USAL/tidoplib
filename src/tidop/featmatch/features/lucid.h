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


/*!
 * \brief LUCID feature descriptor properties.
 *
 * Stores configuration parameters for the LUCID (Locally Uniform Comparison Image Descriptor) feature descriptor.
 *
 * The following property names are valid when accessed via the `Properties` class:
 * - `"LucidKernel"`: Kernel size for descriptor construction. (`1 = 3x3`, `2 = 5x5`, `3 = 7x7`, etc.). Default = `1`.
 * - `"BlurKernel"`: Kernel size for blurring the image prior to descriptor construction. (`1 = 3x3`, `2 = 5x5`, `3 = 7x7`, etc.). Default = `2`.
 *
 * Reference:
 * Christiansen, E., Kriegman, D., Ziegler, A., & Belongie, S. J.
 * Locally Uniform Comparison Image Descriptor.
 */
class TL_EXPORT LucidProperties
  : public Feature
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes LUCID properties with default values.
     */
    LucidProperties();

    /*!
     * \brief Copy constructor.
     *
     * Copies the properties from another `LucidProperties` instance.
     *
     * \param[in] properties The object to copy.
     */
    LucidProperties(const LucidProperties &properties);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties from another `LucidProperties` instance.
     *
     * \param[in] properties The object to move.
     */
    LucidProperties(LucidProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~LucidProperties() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties from another `LucidProperties` instance.
     *
     * \param[in] properties The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const LucidProperties &properties) -> LucidProperties &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties from another `LucidProperties` instance.
     *
     * \param[in] properties The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(LucidProperties &&properties) TL_NOEXCEPT -> LucidProperties &;

    /*!
     * \brief Gets the kernel size for descriptor construction.
     *
     * \return The kernel size (`1 = 3x3`, `2 = 5x5`, `3 = 7x7`, etc.).
     *
     * \note Property name: `"LucidKernel"`.
     */
    auto lucidKernel() const -> int;

    /*!
     * \brief Gets the kernel size for blurring the image prior to descriptor construction.
     *
     * \return The blur kernel size (`1 = 3x3`, `2 = 5x5`, `3 = 7x7`, etc.).
     *
     * \note Property name: `"BlurKernel"`.
     */
    auto blurKernel() const -> int;

    /*!
     * \brief Sets the kernel size for descriptor construction.
     *
     * \param[in] lucidKernel Kernel size (`1 = 3x3`, `2 = 5x5`, `3 = 7x7`, etc.).
     */
    void setLucidKernel(int lucidKernel);

    /*!
     * \brief Sets the kernel size for blurring the image prior to descriptor construction.
     *
     * \param[in] blurKernel Kernel size (`1 = 3x3`, `2 = 5x5`, `3 = 7x7`, etc.).
     */
    void setBlurKernel(int blurKernel);

// Feature interface

public:

    /*!
     * \brief Resets LUCID properties to their default values.
     */
    void reset() override;

};



/*!
 * \brief LUCID feature descriptor.
 *
 * Implements the LUCID (Locally Uniform Comparison Image Descriptor) for keypoint feature description.
 * LUCID is designed for robustness against image deformations and variations.
 *
 * \note This class requires OpenCV's `xfeatures2d` module.
 */
class TL_EXPORT LucidDescriptor
  : public FeatureDescriptor
{

private:

#ifdef HAVE_OPENCV_XFEATURES2D 
    cv::Ptr<cv::xfeatures2d::LUCID> mLUCID;
#endif // HAVE_OPENCV_XFEATURES2D
    LucidProperties mProperties;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the LUCID descriptor with default properties.
     */
    LucidDescriptor();

    /*!
     * \brief Constructor with custom properties.
     *
     * Initializes LUCID using the provided properties.
     *
     * \param[in] properties Configuration properties for LUCID.
     */
    explicit LucidDescriptor(const LucidProperties &properties);

    /*!
     * \brief Copy constructor.
     *
     * Creates a copy of the given `LucidDescriptor` instance.
     *
     * \param[in] lucid The object to copy.
     */
    LucidDescriptor(const LucidDescriptor &lucid);

    /*!
     * \brief Move constructor.
     *
     * Moves the properties and internal state from another `LucidDescriptor` instance.
     *
     * \param[in] lucid The object to move.
     */
    LucidDescriptor(LucidDescriptor &&lucid) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~LucidDescriptor() override;

    /*!
     * \brief Copy assignment operator.
     *
     * Copies the properties and internal state from another `LucidDescriptor` instance.
     *
     * \param[in] lucid The object to copy.
     * \return Reference to the updated object.
     */
    auto operator =(const LucidDescriptor &lucid) -> LucidDescriptor &;

    /*!
     * \brief Move assignment operator.
     *
     * Moves the properties and internal state from another `LucidDescriptor` instance.
     *
     * \param[in] lucid The object to move.
     * \return Reference to the updated object.
     */
    auto operator =(LucidDescriptor &&lucid) TL_NOEXCEPT -> LucidDescriptor &;

    /*!
     * \brief Get a reference to the LUCID descriptor properties.
     *
     * This allows modifying the properties of the LUCID descriptor.
     *
     * \return Reference to the properties object.
     *
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"LucidKernel"`
     * - `"BlurKernel"`
     */
    auto properties() -> LucidProperties &{ return mProperties; }

    /*!
     * \brief Get a constant reference to the LUCID descriptor properties.
     *
     * This allows read-only access to the properties of the LUCID descriptor.
     *
     * \return Constant reference to the properties object.
     * 
     * \note The following property names are valid when accessed through the `Properties` class:
     * - `"LucidKernel"`
     * - `"BlurKernel"`
     */
    auto properties() const -> const LucidProperties &{ return mProperties; }

private:

    void init();

// FeatureDescriptor interface

public:

    /*!
     * \brief Extract descriptors from an image.
     *
     * Extracts binary descriptors for the given keypoints using the LUCID descriptor.
     *
     * \param[in] img The input grayscale image.
     * \param[in,out] keyPoints Detected keypoints (modified if necessary).
     * \return %Matrix containing the computed descriptors.
     */
    auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat override;

};

/*! \} */

} // namespace tl
