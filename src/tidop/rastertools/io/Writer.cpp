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

#include "tidop/rastertools/io/Writer.h"

#include "tidop/rastertools/io/ImageWriter.h"
#include "tidop/rastertools/io/Formats.h"
#include "tidop/rastertools/io/Metadata.h"
#include "tidop/core/base/exception.h"

namespace tl
{

RasterWriter::RasterWriter()
  : mWriter(nullptr)
{
}

RasterWriter::RasterWriter(tl::Path file)
{
    open(std::move(file));
}

RasterWriter::~RasterWriter()
{
    close();
}

void RasterWriter::open(const tl::Path &file)
{
    mWriter = ImageWriterFactory::create(file);
    mWriter->open();
}

void RasterWriter::create(int rows, int cols, int bands, DataType type, const std::shared_ptr<ImageOptions> &imageOptions)
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    mWriter->create(rows, cols, bands, type, imageOptions);
}

auto RasterWriter::isOpen() const -> bool
{
    return mWriter && mWriter->isOpen();
}

void RasterWriter::close()
{
    if (mWriter) {
        mWriter->close();
        mWriter.reset();
    }
}

void RasterWriter::setMetadata(const std::shared_ptr<ImageMetadata> &imageMetadata)
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    mWriter->setMetadata(imageMetadata);
}

void RasterWriter::write(const cv::Mat &image, const Rect<int> &rect)
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    mWriter->write(image, rect);
}

void RasterWriter::write(const cv::Mat &image, const WindowI &window)
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    mWriter->write(image, window);
}

auto RasterWriter::rows() const -> int
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    return mWriter->rows();
}

auto RasterWriter::cols() const -> int
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    return mWriter->cols();
}

auto RasterWriter::channels() const -> int
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    return mWriter->channels();
}

auto RasterWriter::dataType() const -> DataType
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    return mWriter->dataType();
}

auto RasterWriter::depth() const -> int
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    return mWriter->depth();
}

void RasterWriter::setGeoreference(const Affine<double, 2> &georeference)
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    mWriter->setGeoreference(georeference);
}

void RasterWriter::setCRS(const std::string &crs)
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    mWriter->setCRS(crs);
}

void RasterWriter::setNoDataValue(double nodata)
{
    TL_ASSERT(mWriter, "RasterWriter is not open");
    mWriter->setNoDataValue(nodata);
}


} // End namespace tl
