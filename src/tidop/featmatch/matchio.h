/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_FEATMATCH_MATCH_IO_H
#define TL_FEATMATCH_MATCH_IO_H

#include "config_tl.h"

#include <memory>

#include <opencv2/features2d.hpp>

#include "tidop/core/defs.h"

namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \defgroup FeatureMatching Correspondencia de caracteristicas (Feature Matching)
 * 
 *  \{
 */

class TL_EXPORT MatchesReader
{

public:

  MatchesReader(const std::string &fileName);
  virtual ~MatchesReader() = default;

  virtual bool read() = 0;

  std::vector<cv::DMatch> goodMatches() const;
  std::vector<cv::DMatch> wrongMatches() const;

protected:

  std::string mFileName;
  std::vector<cv::DMatch> mGoodMatches;
  std::vector<cv::DMatch> mWrongMatches;
};



/*----------------------------------------------------------------*/



class TL_EXPORT MatchesWriter
{

public:

  MatchesWriter(const std::string &fileName);
  virtual ~MatchesWriter() = default;

  virtual bool write() = 0;

  void setGoodMatches(const std::vector<cv::DMatch> &goodMatches);
  void setWrongMatches(const std::vector<cv::DMatch> &wrongMatches);

protected:

  std::string mFileName;
  std::vector<cv::DMatch> mGoodMatches;
  std::vector<cv::DMatch> mWrongMatches;
};



/*----------------------------------------------------------------*/



class TL_EXPORT MatchesReaderFactory
{

private:

  MatchesReaderFactory() {}

public:

  static std::unique_ptr<MatchesReader> createReader(const std::string &fileName);
};



/*----------------------------------------------------------------*/



class TL_EXPORT MatchesWriterFactory
{
public:

private:

  MatchesWriterFactory() {}

public:

  static std::unique_ptr<MatchesWriter> createWriter(const std::string &fileName);
};


/*----------------------------------------------------------------*/


/*!
 * \brief Pass Points write
 * \param[in] fname File name
 * \param[in] pass_points Pass Points
 */
TL_EXPORT void passPointsWrite(const std::string &fname,
                               const std::vector<std::vector<std::pair<std::string,int>>> &pass_points);

/*!
 * \brief Pass Points read
 * \param[in] fname File name
 * \param[out] pass_points Pass Points
 */
TL_EXPORT void passPointsRead(const std::string &fname,
                              std::vector<std::vector<std::pair<std::string,int>>> &pass_points);

/*! \} */ // end of FeatureMatching

/*! \} */ // end of Features


} // namespace tl


#endif // TL_FEATMATCH_MATCH_IO_H
