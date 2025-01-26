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
#include <string>
#include <map>

#include <opencv2/features2d.hpp>

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"

namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

///*!
// * \brief Features metadata
// * 
// */
//class FeaturesMetadata
//{
//
//public:
//
//  FeaturesMetadata();
//
//private:
//
//  std::string data;
//  double time; 
//  std::string detector;
//  std::map<std::string, std::string> detector_parameters;
//  std::string descriptor;
//  std::map<std::string, std::string> descriptor_parameters;
//};
 
/*!
 * \brief FeaturesWriter class allows the writing of the detected features in different formats
 */
class TL_EXPORT FeaturesWriter
{

private:

    tl::Path mFilePath;
    std::vector<cv::KeyPoint> mKeyPoints;
    cv::Mat mDescriptors;
    //FeaturesMetadata mMetadata;

public:

    FeaturesWriter(tl::Path file);
    virtual ~FeaturesWriter() = default;

    virtual void write() = 0;

    void setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints);
    void setDescriptors(const cv::Mat &descriptors);
    //void setMetadata(const FeaturesMetadata &metadata);

protected:

    auto filePath() const -> const tl::Path&;
    auto keyPoints() const -> const std::vector<cv::KeyPoint>&;
    auto descriptors() const -> const cv::Mat&;
    //const FeaturesMetadata &metadata() const;

};


/*----------------------------------------------------------------*/


/*!
 * \brief The FeaturesWriter class allows the reading of the different formats of features files
 */
class TL_EXPORT FeaturesReader
{

protected:

    tl::Path mFilePath;
    std::vector<cv::KeyPoint> mKeyPoints;
    cv::Mat mDescriptors;
    //FeaturesMetadata mMetadata;

public:

    FeaturesReader(tl::Path file);
    virtual ~FeaturesReader() = default;

    virtual void read() = 0;

    auto keyPoints() const -> std::vector<cv::KeyPoint>;
    auto descriptors() const -> cv::Mat;
    //FeaturesMetadata metadata() const;
    auto file() const -> tl::Path;

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

    static auto create(const tl::Path &file) -> std::unique_ptr<FeaturesReader>;
    TL_DEPRECATED("create", "2.1")
    static auto createReader(const tl::Path &file) -> std::unique_ptr<FeaturesReader>;
};

/*!
 * \brief Factory class to create different writing formats
 */
class TL_EXPORT FeaturesWriterFactory
{

private:

    FeaturesWriterFactory() {}

public:

    static auto create(const tl::Path &file) -> std::unique_ptr<FeaturesWriter>;
    TL_DEPRECATED("create", "2.1")
    static auto createWriter(const tl::Path &file) -> std::unique_ptr<FeaturesWriter>;
};



/*----------------------------------------------------------------*/



class TL_EXPORT FeaturesIOHandler
{

private:

    std::unique_ptr<FeaturesReader> mReader;
    std::unique_ptr<FeaturesWriter> mWriter;

public:

    FeaturesIOHandler();
    virtual ~FeaturesIOHandler() = default;

    void read(const tl::Path &file);
    void write(const tl::Path &file);
    //  std::vector<cv::KeyPoint> keyPoints() const;
    //  cv::Mat descriptors() const;
    //  void setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints);
    //  void setDescriptors(const cv::Mat &descriptors);

};

/*! \} */ // end of Features

} // namespace tl
