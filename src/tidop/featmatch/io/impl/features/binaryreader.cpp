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

#include "tidop/featmatch/io/impl/features/binaryreader.h"
#include "tidop/core/base/exception.h"


namespace tl
{


FeaturesReaderBinary::FeaturesReaderBinary(tl::Path file)
  : FeaturesReader(std::move(file)),
    stream(new std::fstream())
{

}

FeaturesReaderBinary::~FeaturesReaderBinary()
{
    if (stream) {
        delete stream;
        stream = nullptr;
    }
}

void FeaturesReaderBinary::read()
{
    try {

        open();
        if (isOpen()) {
            readHeader();
            readBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::open()
{
    try {
        stream->open(mFilePath.toString(), std::ios_base::in | std::ios_base::binary);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto FeaturesReaderBinary::isOpen() -> bool
{
    return stream->is_open();
}

void FeaturesReaderBinary::readHeader()
{
    try {

        std::array<char, 24> header{};
        stream->read(header.data(), sizeof(char)*24);
        stream->read(reinterpret_cast<char *>(&size), sizeof(int32_t));
        stream->read(reinterpret_cast<char *>(&rows), sizeof(int32_t));
        stream->read(reinterpret_cast<char *>(&cols), sizeof(int32_t));
        stream->read(reinterpret_cast<char *>(&type), sizeof(int32_t));
        std::array<char, 200> extra_head{}; // Reserva de espacio para futuros usos
        stream->read(extra_head.data(), sizeof(char)*200);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::readBody()
{
    readKeypoints();
    readDescriptors();
}

void FeaturesReaderBinary::readKeypoints()
{
    try {

        mKeyPoints.resize(static_cast<size_t>(size));
        for (auto &keypoint : mKeyPoints) {
            stream->read(reinterpret_cast<char *>(&keypoint.pt.x), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.pt.y), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.size), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.angle), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.response), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.octave), sizeof(float));
            stream->read(reinterpret_cast<char *>(&keypoint.class_id), sizeof(float));
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::readDescriptors()
{
    try {

        cv::Mat aux(static_cast<int>(rows), static_cast<int>(cols), type);
        stream->read(reinterpret_cast<char *>(aux.data), sizeof(float) * static_cast<size_t>(rows * cols));
        aux.copyTo(mDescriptors);
        aux.release();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderBinary::close()
{
    if (stream->is_open()) stream->close();
}


} // namespace tl

