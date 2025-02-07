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
 * \brief Reads feature matches from a file.
 *
 * This abstract class provides an interface for reading feature matches,
 * including correct matches (`goodMatches`) and incorrect matches (`wrongMatches`),
 * from OpenCV (.xml and .yml) and binary (.bin) formats.
 *
 * #### Example Usage
 *
 * The following example demonstrates how to read feature matches from a file:
 *
 * \code{.cpp}
 * #include <TidopLib/Features/MatchesReader.h>
 *
 * int main()
 * {
 *     try {
 *
 *         tl::Path file("matches.xml");
 *         auto reader = MatchesReaderFactory::create(file);
 *         reader->read();
 *         std::vector<cv::DMatch> goodMatches = reader->goodMatches();
 *         std::vector<cv::DMatch> wrongMatches = reader->wrongMatches();
 *
 *     } catch (std::exception &e) {
 *         printException(e);
 *     }
 *
 *     return 0;
 * }
 * \endcode
 */
class TL_EXPORT MatchesReader
{

private:

    tl::Path mFilePath;
    std::vector<cv::DMatch> mGoodMatches;
    std::vector<cv::DMatch> mWrongMatches;

public:

    /*!
     * \brief Constructor.
     * \param[in] file Path to the input matches file.
     */
    MatchesReader(tl::Path file) : mFilePath(std::move(file)) {}
    
    /*!
     * \brief Destructor.
     */
    virtual ~MatchesReader() = default;

    /*!
     * \brief Reads matches from a file.
     * \note This is a pure virtual function that must be implemented in derived classes.
     */
    virtual void read() = 0;

    /*!
     * \brief Get the good matches.
     * \return Vector of correctly matched features.
     */
    auto goodMatches() const -> std::vector<cv::DMatch>;

    /*!
     * \brief Get the wrong matches.
     * \return Vector of incorrectly matched features.
     */
    auto wrongMatches() const -> std::vector<cv::DMatch>;

protected:

    auto filePath() const -> const tl::Path&;
    auto good_matches() -> std::vector<cv::DMatch>&;
    auto wrong_matches() -> std::vector<cv::DMatch>&;

};


/*!
 * \brief Factory class to create match readers based on file format.
 *
 * This factory selects the appropriate `MatchesReader` implementation depending
 * on the file extension.
 */
class TL_EXPORT MatchesReaderFactory
{

private:

    MatchesReaderFactory() {}

public:

    /*!
     * \brief Create a match reader based on file format.
     * \param[in] file Path to the input matches file.
     * \return A unique pointer to a `MatchesReader` object.
     * \exception Exception if no reader is found.
     */
    static auto create(const Path &file) -> std::unique_ptr<MatchesReader>;
};


/*! \} */



/* Inline methods */

inline auto MatchesReader::goodMatches() const -> std::vector<cv::DMatch>
{
    return mGoodMatches;
}

inline auto MatchesReader::wrongMatches() const -> std::vector<cv::DMatch>
{
    return mWrongMatches;
}

inline auto MatchesReader::filePath() const -> const Path &
{
    return mFilePath;
}

inline auto MatchesReader::good_matches() -> std::vector<cv::DMatch> &
{
    return mGoodMatches;
}

inline auto MatchesReader::wrong_matches() -> std::vector<cv::DMatch> &
{
    return mWrongMatches;
}



} // namespace tl
