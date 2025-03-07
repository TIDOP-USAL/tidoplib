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

#include "tidop/featmatch/io/impl/matches/binarywriter.h"

#include "tidop/core/base/exception.h"

namespace tl
{


MatchesWriterBinary::MatchesWriterBinary(Path file)
  : MatchesWriter(std::move(file)),
    stream(new std::fstream())
{
}

void MatchesWriterBinary::write()
{
    try {

        open();
        if (isOpen()) {
            writeHeader();
            writeGoodMatches();
            writeWrongMatches();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::open()
{
    try {

        stream->open(filePath().toString(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool MatchesWriterBinary::isOpen() const
{
    return stream->is_open();
}

void MatchesWriterBinary::writeHeader() const
{
    try {

        uint64_t size = goodMatches().size();
        uint64_t size_wm = wrongMatches().size();

        stream->write("TIDOPLIB-Matching-#01", sizeof("TIDOPLIB-Matching-#01"));
        stream->write(reinterpret_cast<char *>(&size), sizeof(uint64_t));
        stream->write(reinterpret_cast<char *>(&size_wm), sizeof(uint64_t));
        std::array<char, 100> extra_head{}; 
        stream->write(extra_head.data(), sizeof(char) * 100);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::writeGoodMatches() const
{
    try {

        writeMatches(goodMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::writeWrongMatches() const
{
    try {

        writeMatches(wrongMatches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::writeMatches(const std::vector<cv::DMatch> &matches) const
{
    try {

        for (size_t i = 0; i < matches.size(); i++) {
            stream->write(reinterpret_cast<const char *>(&matches[i].queryIdx), sizeof(int32_t));
            stream->write(reinterpret_cast<const char *>(&matches[i].trainIdx), sizeof(int32_t));
            stream->write(reinterpret_cast<const char *>(&matches[i].imgIdx), sizeof(int32_t));
            stream->write(reinterpret_cast<const char *>(&matches[i].distance), sizeof(float));
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesWriterBinary::close() const
{
    stream->close();
}


} // namespace tl
