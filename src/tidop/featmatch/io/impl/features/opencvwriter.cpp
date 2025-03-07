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

#include "tidop/featmatch/io/impl/features/opencvwriter.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/base/string_utils.h"


namespace tl
{


FeaturesWriterOpenCV::FeaturesWriterOpenCV(tl::Path file)
    : FeaturesWriter(std::move(file))
{
}

void FeaturesWriterOpenCV::write()
{
    try {

        open();
        if (isOpen()) {
            writeKeypoints();
            writeDescriptors();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterOpenCV::open()
{
    try {
        int mode{};
        std::string ext = filePath().extension().toString();
        if (compareInsensitiveCase(ext, ".xml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
        } else if (compareInsensitiveCase(ext, ".yml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
        } else {
            TL_THROW_EXCEPTION("Unsupported format");
        }

        mFileStorage = cv::FileStorage(filePath().toString(), mode);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesWriterOpenCV::isOpen()
{
    return mFileStorage.isOpened();
}

void FeaturesWriterOpenCV::writeKeypoints()
{
    try {

        if (!keyPoints().empty())
            cv::write(mFileStorage, "keypoints", keyPoints());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterOpenCV::writeDescriptors()
{
    try {

        if (!descriptors().empty())
            cv::write(mFileStorage, "descriptors", descriptors());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterOpenCV::close()
{
    mFileStorage.release();
}


} // namespace tl

