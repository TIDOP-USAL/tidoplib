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

#include "tidop/featmatch/io/impl/features/binarywriter.h"
#include "tidop/core/base/exception.h"

namespace tl
{


FeaturesWriterBinary::FeaturesWriterBinary(tl::Path file)
  : FeaturesWriter(std::move(file)),
    stream(new std::fstream())
{

}

FeaturesWriterBinary::~FeaturesWriterBinary()
{
    if (stream) {
        delete stream;
        stream = nullptr;
    }
}

void FeaturesWriterBinary::write()
{
    try {

        open();
        if (isOpen()) {
            writeHeader();
            writeBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterBinary::open()
{
    try {
        stream->open(filePath().toString(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesWriterBinary::isOpen()
{
    return stream->is_open();
}

void FeaturesWriterBinary::writeHeader()
{
    try {

        int32_t size = static_cast<int32_t>(keyPoints().size());
        int32_t rows = static_cast<int32_t>(descriptors().rows);
        int32_t cols = static_cast<int32_t>(descriptors().cols);
        int32_t type = descriptors().type();

        stream->write("TIDOPLIB-Features2D-#01", sizeof("TIDOPLIB-Features2D-#01"));
        stream->write(reinterpret_cast<char *>(&size), sizeof(int32_t));
        stream->write(reinterpret_cast<char *>(&rows), sizeof(int32_t));
        stream->write(reinterpret_cast<char *>(&cols), sizeof(int32_t));
        stream->write(reinterpret_cast<char *>(&type), sizeof(int32_t));
        std::array<char, 200> extra_head{};  // Reserva de espacio para futuros usos
        stream->write(reinterpret_cast<char *>(extra_head.data()), sizeof(char)*200);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterBinary::writeBody()
{
    try {

        for (auto &keyPoint : keyPoints()) {
            stream->write(reinterpret_cast<const char *>(&keyPoint.pt.x), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.pt.y), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.size), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.angle), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.response), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.octave), sizeof(float));
            stream->write(reinterpret_cast<const char *>(&keyPoint.class_id), sizeof(float));
        }

        size_t rows = static_cast<size_t>(descriptors().rows);
        size_t cols = static_cast<size_t>(descriptors().cols);
        stream->write(reinterpret_cast<char *>(descriptors().data), sizeof(float) * rows * cols);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterBinary::close()
{
    stream->close();
}


} // namespace tl

