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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_FEATMATCH_FEAT_IO_H
#define TL_FEATMATCH_FEAT_IO_H

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
 
 
/*!
 * \brief FeaturesWriter class allows the writing of the detected features in different formats
 */
class TL_EXPORT FeaturesWriter
{

public:

  FeaturesWriter(std::string fileName);
  virtual ~FeaturesWriter() = default;

  virtual bool write() = 0;

  void setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints);
  void setDescriptors(const cv::Mat &descriptors);

protected:

  std::string mFileName;
  std::vector<cv::KeyPoint> mKeyPoints;
  cv::Mat mDescriptors;
};


/*----------------------------------------------------------------*/


/*!
 * \brief The FeaturesWriter class allows the reading of the different formats of features files
 */
class TL_EXPORT FeaturesReader
{

public:

  FeaturesReader(std::string fileName);
  virtual ~FeaturesReader() = default;

  virtual bool read() = 0;

  std::vector<cv::KeyPoint> keyPoints() const;
  cv::Mat descriptors() const;
  std::string fileName() const;

protected:

  std::string mFileName;
  std::vector<cv::KeyPoint> mKeyPoints;
  cv::Mat mDescriptors;
};



/*----------------------------------------------------------------*/



/*!
 * \brief Factory class to create different reading formats
 */
class TL_EXPORT FeaturesReaderFactory
{

private:

  FeaturesReaderFactory() {}

public:

  static std::unique_ptr<FeaturesReader> createReader(const std::string &fileName);
};

/*!
 * \brief Factory class to create different writing formats
 */
class TL_EXPORT FeaturesWriterFactory
{
public:

private:

  FeaturesWriterFactory() {}

public:

  static std::unique_ptr<FeaturesWriter> createWriter(const std::string &fileName);
};



/*----------------------------------------------------------------*/



class TL_EXPORT FeaturesIOHandler
{

public:

  FeaturesIOHandler();
  virtual ~FeaturesIOHandler() = default;

  bool read(const std::string &file);
  bool write(const std::string &file);
//  std::vector<cv::KeyPoint> keyPoints() const;
//  cv::Mat descriptors() const;
//  void setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints);
//  void setDescriptors(const cv::Mat &descriptors);

private:

  std::unique_ptr<FeaturesReader> mReader;
  std::unique_ptr<FeaturesWriter> mWriter;
};

/*! \} */ // end of Features

} // namespace tl


#endif // TL_FEATMATCH_FEAT_IO_H
