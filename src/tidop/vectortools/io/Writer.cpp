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

#include "tidop/vectortools/io/Writer.h"
#include "tidop/vectortools/io/impl/GdalWriter.h"
#include "tidop/core/base/StringUtils.h"
#include "tidop/core/base/Exception.h"

namespace tl
{

VectorWriter::VectorWriter()
  : mWriter(nullptr)
{
}

VectorWriter::VectorWriter(Path file)
{
    open(std::move(file));
}

VectorWriter::~VectorWriter()
{
    close();
}

void VectorWriter::open(const Path &file)
{
    mWriter = VectorWriterFactory::create(file);
    mWriter->open();
    mWriter->create();
}

auto VectorWriter::isOpen() const -> bool
{
    return mWriter && mWriter->isOpen();
}

void VectorWriter::close()
{
    if (mWriter) {
        mWriter->close();
        mWriter.reset();
    }
}

void VectorWriter::write(const GLayer &layer)
{
    TL_ASSERT(mWriter, "VectorWriter is not open");
    mWriter->write(layer);
}

void VectorWriter::setCRS(const std::string &epsgCode)
{
    TL_ASSERT(mWriter, "VectorWriter is not open");
    mWriter->setCRS(epsgCode);
}


} // End namespace tl
