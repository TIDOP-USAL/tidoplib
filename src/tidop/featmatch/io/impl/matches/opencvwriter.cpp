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

#include "tidop/featmatch/io/impl/matches/opencvwriter.h"

#include "tidop/core/base/exception.h"
#include "tidop/core/base/string_utils.h"

namespace tl
{


MatchesWriterOpenCV::MatchesWriterOpenCV(Path file)
  : MatchesWriter(std::move(file)),
    mFileStorage(nullptr)
{
}

MatchesWriterOpenCV::~MatchesWriterOpenCV()
{
    if (mFileStorage) {
        delete mFileStorage;
        mFileStorage = nullptr;
    }
}

void MatchesWriterOpenCV::write()
{
    try {

        open();
        if (isOpen()) {
            writeGoodMatches();
            writeWrongMatches();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterOpenCV::open()
{
    try {

        int mode{};
        std::string ext = filePath().extension().toString();
        if (compareInsensitiveCase(ext, ".xml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
        } else if (compareInsensitiveCase(ext, ".yml")) {
            mode = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
        }

        mFileStorage = new cv::FileStorage(filePath().toString(), mode);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool MatchesWriterOpenCV::isOpen() const
{
    if (mFileStorage == nullptr) return false;
    return mFileStorage->isOpened();
}

void MatchesWriterOpenCV::writeGoodMatches()
{
    try {

        if (mFileStorage)
            cv::write(*mFileStorage, "matches", goodMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterOpenCV::writeWrongMatches()
{
    try {

        if (mFileStorage)
            cv::write(*mFileStorage, "wrong_matches", wrongMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterOpenCV::close()
{
    if (mFileStorage)
        mFileStorage->release();
}


} // namespace tl
