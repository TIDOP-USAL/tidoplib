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

#include "tidop/featmatch/io/impl/matches/binaryreader.h"

#include "tidop/core/base/exception.h"

namespace tl
{


MatchesReaderBinary::MatchesReaderBinary(tl::Path file)
  : MatchesReader(std::move(file)),
    stream(new std::fstream())
{

}

void MatchesReaderBinary::read()
{
    try {

        open();
        if (isOpen()) {
            readHeader();
            readGoodMatches();
            readWrongMatches();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::open()
{
    try {
        stream->open(filePath().toString(), std::ios_base::in | std::ios_base::binary);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool MatchesReaderBinary::isOpen() const
{
    return stream->is_open();
}

void MatchesReaderBinary::readHeader()
{
    try {

        std::array<char, 22> header{};
        std::array<char, 100> extraHead{};
        stream->read(header.data(), sizeof(char) * 22);
        stream->read(reinterpret_cast<char *>(&goodMatchesCount), sizeof(uint64_t));
        stream->read(reinterpret_cast<char *>(&wrongMatchesCount), sizeof(uint64_t));
        stream->read(extraHead.data(), sizeof(char) * 100);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::readMatches(std::vector<cv::DMatch> *matches)
{
    try {

        for (auto &match : *matches) {
            stream->read(reinterpret_cast<char *>(&match.queryIdx), sizeof(int32_t));
            stream->read(reinterpret_cast<char *>(&match.trainIdx), sizeof(int32_t));
            stream->read(reinterpret_cast<char *>(&match.imgIdx), sizeof(int32_t));
            stream->read(reinterpret_cast<char *>(&match.distance), sizeof(float));
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::readGoodMatches()
{
    try {

        good_matches().resize(goodMatchesCount);
        readMatches(&good_matches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::readWrongMatches()
{
    try {

        wrong_matches().resize(wrongMatchesCount);
        readMatches(&wrong_matches());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void MatchesReaderBinary::close()
{
    stream->close();
}


} // namespace tl
