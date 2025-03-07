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

#include "tidop/featmatch/io/featreader.h"
#include "tidop/featmatch/io/impl/features/binaryreader.h"
#include "tidop/featmatch/io/impl/features/opencvreader.h"
#include "tidop/featmatch/io/impl/features/txtreader.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/base/string_utils.h"


namespace tl
{



FeaturesReader::FeaturesReader(tl::Path file)
    : mFilePath(std::move(file))
{

}

auto FeaturesReader::keyPoints() const -> std::vector<cv::KeyPoint>
{
    return mKeyPoints;
}

auto FeaturesReader::descriptors() const -> cv::Mat
{
    return mDescriptors;
}

auto FeaturesReader::file() const -> tl::Path
{
    return mFilePath;
}




auto FeaturesReaderFactory::create(const tl::Path &file) -> std::unique_ptr<FeaturesReader>
{
    std::unique_ptr<FeaturesReader> features_reader;

    try {

        std::string ext = file.extension().toString();
        if (compareInsensitiveCase(ext, ".bin")) {
            features_reader = std::make_unique<FeaturesReaderBinary>(file);
        } else if (compareInsensitiveCase(ext, ".xml")) {
            features_reader = std::make_unique<FeaturesReaderOpenCV>(file);
        } else if (compareInsensitiveCase(ext, ".yml")) {
            features_reader = std::make_unique<FeaturesReaderOpenCV>(file);
        } else if (compareInsensitiveCase(ext, ".txt")) {
            features_reader = std::make_unique<FeaturesReaderTxt>(file);
        } else {
            TL_THROW_EXCEPTION("No FeaturesReader found for: {}", file.fileName().toString());
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return features_reader;
}


} // namespace tl

