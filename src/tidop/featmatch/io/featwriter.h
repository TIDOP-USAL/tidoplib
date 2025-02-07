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

#include <opencv2/features2d.hpp>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"

namespace tl
{

/*! \addtogroup FeatureIO
 *
 *  \{
 */

 
/*!
 * \brief Writes detected features to files in different formats.
 *
 * This abstract class provides an interface for saving keypoints and descriptors
 * in various formats (XML (OpenCV), YML (OpenCV), binary (TidopLib) and text).
 *
 * #### Example Usage
 *
 * The following example shows how to write keypoints and descriptors to a file:
 *
 * \code{.cpp}
 * #include <tidop/featmatch/io/featwriter.h>
 *
 * int main()
 * {
 *     try {
 *
 *         // Load input image
 *         cv::Mat img = cv::imread("image.jpg", cv::IMREAD_GRAYSCALE);
 *         TL_ASSERT(!img.empty(), "Error loading image!");
 *
 *         // Create STAR detector and detect keypoints
 *         StarDetector detector;
 *         std::vector<cv::KeyPoint> keypoints = detector.detect(img);
 *
 *         // Create VGG descriptor and extract descriptors
 *         VggDescriptor descriptor;
 *         cv::Mat descriptors = descriptor.extract(img, keypoints);
 * 
 *         tl::Path file("output.bin");
 *         auto writer = FeaturesWriterFactory::create(file);
 *         writer->setKeyPoints(keypoints);
 *         writer->setDescriptors(descriptors);
 *         writer->write();
 *
 *     } catch (std::exception &e) {
 *         printException(e);
 *     }
 * 
 *     return 0;
 * }
 * \endcode
 */
class TL_EXPORT FeaturesWriter
{

private:

    tl::Path mFilePath;
    std::vector<cv::KeyPoint> mKeyPoints;
    cv::Mat mDescriptors;

public:

    FeaturesWriter(tl::Path file);
    virtual ~FeaturesWriter() = default;

    /*!
     * \brief Writes the keypoints and descriptors to a file.
     * \note This is a pure virtual function that must be implemented in derived classes.
     */
    virtual void write() = 0;

    /*!
     * \brief Set the keypoints to be written.
     * \param[in] keyPoints Vector of keypoints.
     */
    void setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints);

    /*!
     * \brief Set the descriptors to be written.
     * \param[in] descriptors OpenCV matrix of descriptors.
     */
    void setDescriptors(const cv::Mat &descriptors);

protected:

    auto filePath() const -> const tl::Path&;
    auto keyPoints() const -> const std::vector<cv::KeyPoint>&;
    auto descriptors() const -> const cv::Mat&;

};




/*!
 * \ingroup FeatureExtraction
 * \class FeaturesWriterFactory
 * \brief Factory class to create feature writers based on file format.
 *
 * This factory selects the appropriate `FeaturesWriter` implementation depending
 * on the file extension.
 */
class TL_EXPORT FeaturesWriterFactory
{

private:

    FeaturesWriterFactory() {}

public:

    /*!
     * \brief Create a feature writer based on file format.
     * \param[in] file Path to the output feature file.
     * \return A unique pointer to a `FeaturesWriter` object.
     * \exception Exception if no features writer is found.
     */
    static auto create(const tl::Path &file) -> std::unique_ptr<FeaturesWriter>;
};


/*! \} */ // end of Features

} // namespace tl
