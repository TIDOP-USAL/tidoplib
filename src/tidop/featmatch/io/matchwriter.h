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

#include "tidop/core/base/path.h"

namespace tl
{

/*! \addtogroup FeatureIO
 * 
 *  \{
 */
 
/*!
 * \brief Writes feature matches to a file.
 *
 * This abstract class provides an interface for saving feature matches,
 * including correct matches (`goodMatches`) and incorrect matches (`wrongMatches`),
 * in OpenCV (.xml and .yml) and binary (.bin) formats.
 *
 * #### Example Usage
 *
 * The following example shows how to write feature matches to a file:
 *
 * \code{.cpp}
 * #include <TidopLib/Features/MatchesWriter.h>
 *
 * int main()
 * {
 *     try {
 *
 *         std::vector<cv::DMatch> goodMatches; // Assume matches are computed
 *         std::vector<cv::DMatch> wrongMatches; // Assume incorrect matches are identified
 *         
 *         tl::Path file("matches.json");
 *         auto writer = MatchesWriterFactory::create(file);
 *         
 *         writer->setGoodMatches(goodMatches);
 *         writer->setWrongMatches(wrongMatches);
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
class TL_EXPORT MatchesWriter
{

private:

    tl::Path mFilePath;
    std::vector<cv::DMatch> mGoodMatches;
    std::vector<cv::DMatch> mWrongMatches;

public:

    MatchesWriter(tl::Path file) : mFilePath(std::move(file)) {}
    virtual ~MatchesWriter() = default;

    virtual void write() = 0;

    /*!
     * \brief Set the good matches.
     * \param[in] goodMatches Vector of correctly matched features.
     */
    void setGoodMatches(const std::vector<cv::DMatch> &goodMatches);

    /*!
     * \brief Set the wrong matches.
     * \param[in] wrongMatches Vector of incorrectly matched features.
     */
    void setWrongMatches(const std::vector<cv::DMatch> &wrongMatches);

protected:

    auto filePath() const -> const tl::Path&;
    auto goodMatches() const -> const std::vector<cv::DMatch>&;
    auto wrongMatches() const -> const std::vector<cv::DMatch>&;

};


/*!
 * \brief Factory class to create match writers based on file format.
 *
 * This factory selects the appropriate `MatchesWriter` implementation depending
 * on the file extension.
 *
 */
class TL_EXPORT MatchesWriterFactory
{

private:

    MatchesWriterFactory() {}

public:

    /*!
     * \brief Create a match writer based on file format.
     * \param[in] file Path to the output matches file.
     * \return A unique pointer to a `MatchesWriter` object.
     * \exception Exception if no writer is found.
     */
    static auto create(const Path &file) -> std::unique_ptr<MatchesWriter>;
};

/*! \} */

/* Inline methods */

inline void MatchesWriter::setGoodMatches(const std::vector<cv::DMatch> &goodMatches)
{
    mGoodMatches = goodMatches;
}

inline void MatchesWriter::setWrongMatches(const std::vector<cv::DMatch> &wrongMatches)
{
    mWrongMatches = wrongMatches;
}

inline auto MatchesWriter::filePath() const -> const tl::Path &
{
    return mFilePath;
}

inline auto MatchesWriter::goodMatches() const -> const std::vector<cv::DMatch> &
{
    return mGoodMatches;
}

inline auto MatchesWriter::wrongMatches() const -> const std::vector<cv::DMatch> &
{
    return mWrongMatches;
}


} // namespace tl
