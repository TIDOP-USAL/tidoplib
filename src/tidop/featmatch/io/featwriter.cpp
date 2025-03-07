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

#include "tidop/featmatch/io/featwriter.h"
#include "tidop/featmatch/io/impl/features/binarywriter.h"
#include "tidop/featmatch/io/impl/features/opencvwriter.h"
#include "tidop/featmatch/io/impl/features/txtwriter.h"
#include "tidop/core/base/string_utils.h"


namespace tl
{

FeaturesWriter::FeaturesWriter(tl::Path file)
    : mFilePath(std::move(file))
{

}

void FeaturesWriter::setKeyPoints(const std::vector<cv::KeyPoint> &keyPoints)
{
    mKeyPoints = keyPoints;
}

void FeaturesWriter::setDescriptors(const cv::Mat &descriptors)
{
    mDescriptors = descriptors;
}

auto FeaturesWriter::filePath() const -> const tl::Path&
{
    return mFilePath;
}

auto FeaturesWriter::keyPoints() const -> const std::vector<cv::KeyPoint>&
{
    return mKeyPoints;
}

auto FeaturesWriter::descriptors() const -> const cv::Mat&
{
    return mDescriptors;
}






auto FeaturesWriterFactory::create(const tl::Path &file) -> std::unique_ptr<FeaturesWriter>
{
    std::unique_ptr<FeaturesWriter> features_writer;

    try {

        std::string ext = file.extension().toString();
        if (compareInsensitiveCase(ext, ".bin")) {
            features_writer = std::make_unique<FeaturesWriterBinary>(file);
        } else if (compareInsensitiveCase(ext, ".txt")) {
            features_writer = std::make_unique<FeaturesWriterTxt>(file);
        } else if (compareInsensitiveCase(ext, ".xml") || compareInsensitiveCase(ext, ".yml")) {
            features_writer = std::make_unique<FeaturesWriterOpenCV>(file);
        } else {
            TL_THROW_EXCEPTION("No FeaturesWriter found for: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return features_writer;
}



} // namespace tl

