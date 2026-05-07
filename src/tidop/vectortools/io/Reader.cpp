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

#include "tidop/vectortools/io/Reader.h"
#include "tidop/core/base/exception.h"
#include "tidop/vectortools/io/impl/GdalReader.h"

namespace tl
{

VectorReader::VectorReader()
  : mReader(nullptr)
{
}

VectorReader::VectorReader(Path file)
{
    open(std::move(file));
}

VectorReader::~VectorReader()
{
    close();
}

void VectorReader::open(const tl::Path &file)
{
    mReader = VectorReaderFactory::create(file);
    mReader->open();
}

auto VectorReader::isOpen() const -> bool
{
    return mReader && mReader->isOpen();
}

void VectorReader::close()
{
    if (mReader) {
        mReader->close();
        mReader.reset();
    }
}

auto VectorReader::layersCount() const -> int
{
    TL_ASSERT(isOpen(), "VectorReader is not open");
    return mReader->layersCount();
}

auto VectorReader::read(int layerId) -> std::shared_ptr<GLayer>
{
    TL_ASSERT(isOpen(), "VectorReader is not open");
    return mReader->read(layerId);
}

auto VectorReader::read(const std::string &layerName) -> std::shared_ptr<GLayer>
{
    TL_ASSERT(isOpen(), "VectorReader is not open");
    return mReader->read(layerName);
}

//void VectorReader::copy(const tl::Path &outputPath, const std::string &targetEpsg) const
//{
//    TL_ASSERT(isOpen(), "VectorReader is not open");
//    return mReader->copy(outputPath.toString(), targetEpsg);
//}

auto VectorReader::crsWkt() const -> std::string
{
    TL_ASSERT(isOpen(), "VectorReader is not open");
    return mReader->crsWkt();
}

} // End namespace tl
