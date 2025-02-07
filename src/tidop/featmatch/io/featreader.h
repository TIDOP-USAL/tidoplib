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
 * \brief Reads feature files in different formats.
 *
 * This abstract class defines an interface for reading keypoints and descriptors
 * from various feature file formats (XML (OpenCV), YML (OpenCV), binary (TidopLib) and text).
 *
 * #### Example Usage
 *
 * The following example demonstrates how to read keypoints and descriptors from a file:
 *
 * \code{.cpp}
 * #include <tidop/featmatch/io/featreader.h>
 *
 * int main()
 * {
 *     try {
 * 
 *         tl::Path file("features.xml");
 *         auto reader = FeaturesReaderFactory::create(file);
 *         reader->read();
 *         auto keypoints = reader->keyPoints();
 *         auto descriptors = reader->descriptors();
 * 
 *     } catch (std::exception &e) {
 *         printException(e);
 *     }
 * 
 *     return 0;
 * }
 * \endcode
 */
class TL_EXPORT FeaturesReader
{

protected:

    tl::Path mFilePath;
    std::vector<cv::KeyPoint> mKeyPoints;
    cv::Mat mDescriptors;

public:

    FeaturesReader(tl::Path file);
    virtual ~FeaturesReader() = default;

    /*!
     * \brief Reads the feature file.
     * \note This is a pure virtual function that must be implemented in derived classes.
     */
    virtual void read() = 0;

    /*!
     * \brief Get the extracted keypoints.
     * \return A vector of OpenCV keypoints.
     */
    auto keyPoints() const -> std::vector<cv::KeyPoint>;

    /*!
     * \brief Get the extracted descriptors.
     * \return A matrix of descriptors.
     */
    auto descriptors() const -> cv::Mat;

    /*!
     * \brief Get the file path.
     * \return The path of the feature file.
     */
    auto file() const -> tl::Path;

};


/*!
 * \brief Factory class to create feature readers based on file format.
 *
 * This factory creates the appropriate `FeaturesReader` implementation depending
 * on the file extension.
 */
class TL_EXPORT FeaturesReaderFactory
{

private:

    FeaturesReaderFactory() {}

public:

    /*!
     * \brief Create a feature reader based on file format.
     * \param[in] file Path to the feature file.
     * \return A unique pointer to a `FeaturesReader` object.
     * \exception Exception if no reader is found.
     */
    static auto create(const tl::Path &file) -> std::unique_ptr<FeaturesReader>;

};


/*! \} */ // end of Features

} // namespace tl
