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

#include <opencv2/core.hpp>

namespace tl
{

/*! \addtogroup ImgProc
 *  \{
 */

/*!
 * \class StereoImage
 * \brief A class for constructing stereo images in various visualization modes.
 *
 * This class uses predefined transformation matrices to generate stereo images
 * from two input images (left and right views). The transformations are based on
 * the selected visualization mode, which determines how the left and right images
 * are combined. Supported modes include grayscale, color-optimized anaglyphs,
 * and side-by-side formats.
 *
 * ## Visualization Modes and Transformations
 *
 * Each mode utilizes specific transformation matrices applied to the left and
 * right input images:
 *
 * - **normal**:
 *   - Left image: Unmodified.
 *   - Right image: Unmodified.
 *
 * - **gray**:
 *   - Left image: Grayscale transformation.
 *   - Right image: Grayscale transformation.
 *
 * - **dubois**:
 *   - Left image: Applies the Dubois transformation for reduced crosstalk and
 *     enhanced color reproduction.
 *   - Right image: Applies an adjusted Dubois matrix for optimized 3D effect.
 *
 * - **half_color**:
 *   - Left image: Grayscale transformation.
 *   - Right image: Original color retained.
 *
 * - **optimized**:
 *   - Left image: Slightly saturated grayscale transformation for reduced ghosting.
 *   - Right image: Original color retained.
 *
 * - **red_blue**:
 *   - Left image: Grayscale transformation.
 *   - Right image: Red channel retained for red-blue anaglyphs.
 *
 * ## Example Usage
 * ```cpp
 * #include <StereoImage.h>
 * #include <opencv2/opencv.hpp>
 *
 * int main() 
 * {
 *     // Load left and right images
 *     cv::Mat left = cv::imread("left_image.jpg");
 *     cv::Mat right = cv::imread("right_image.jpg");
 *
 *     // Create a StereoImage instance with Dubois mode
 *     StereoImage stereo(StereoImage::Mode::dubois);
 *
 *     // Generate the stereo image
 *     cv::Mat result = stereo.run(left, right);
 *
 *     // Save the result
 *     cv::imwrite("stereo_anaglyph.jpg", result);
 *
 *     return 0;
 * }
 * ```
 */
class StereoImage
{

public:

    /*!
     * \enum Mode
     * \brief Enum for stereo visualization modes.
     *
     * Defines the different modes for generating stereo images:
     * - **normal**: Combines left and right images without adjustments.
     * - **gray**: Converts input images to grayscale for monochrome anaglyphs.
     * - **dubois**: Uses the Dubois algorithm for color-optimized anaglyph generation.
     * - **half_color**: Creates anaglyphs with reduced color saturation to minimize crosstalk.
     * - **optimized**: Provides vibrant anaglyphs with improved ghosting reduction.
     * - **red_blue**: Standard red-blue anaglyph format.
     * - **side_by_side**: Outputs left and right images placed adjacently.
     */
    enum class Mode
    {
        normal,
        gray,
        dubois,
        half_color,
        optimized,
        red_blue,
        side_by_side
    };

private:

    Mode mMode;

public:

    /*!
     * \brief Constructor
     *
     * Initializes the `StereoImage` object with the specified visualization mode.
     *
     * \param[in] mode Visualization mode to be used (default: `Mode::normal`).
     * \see Mode
     */
    StereoImage(Mode mode = Mode::normal);
    ~StereoImage();

    /*!
     * \brief Generates a stereo image from input images.
     *
     * Processes the left and right images based on the current visualization mode
     * and produces the resulting stereo image.
     *
     * \param[in] left Left input image (OpenCV `cv::Mat` format).
     * \param[in] right Right input image (OpenCV `cv::Mat` format).
     * \return The resulting stereo image as an OpenCV `cv::Mat`.
     */
    auto run(const cv::Mat &left,
             const cv::Mat &right) -> cv::Mat;

    /*!
     * \brief Gets the current visualization mode.
     *
     * \return The current `Mode` used for stereo image generation.
     */
    auto mode() const -> Mode;

    /*!
     * \brief Sets the visualization mode.
     *
     * Updates the mode used for subsequent stereo image processing.
     *
     * \param[in] mode New visualization mode.
     */
    void setMode(Mode mode);

};

/*! \} */

} // End namespace tl
