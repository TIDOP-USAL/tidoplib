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

#include "tidop/rastertools/io/Reader.h"

#include "tidop/rastertools/io/impl/ImageReader.h"

#include <utility>

namespace tl
{


RasterReader::RasterReader()
  : mReader(nullptr)
{
}

RasterReader::RasterReader(tl::Path file, Mode mode)
{
    open(std::move(file), mode);
}

RasterReader::~RasterReader()
{
    close();
}

void RasterReader::open(const tl::Path &file, Mode mode)
{
    try {
        mReader = ImageReaderFactory::create(file, static_cast<ImageReader::Mode>(static_cast<std::underlying_type<Mode>::type>(mode)));
        if (mReader) {
            mReader->open();
        }
    } catch (...) {
        close();
    }
}

auto RasterReader::isOpen() const -> bool
{
    return mReader && mReader->isOpen();
}

void RasterReader::close()
{
    if (mReader) {
        mReader->close();
        mReader.reset();
    }
}

auto RasterReader::read(const Rect<int> &rect, const Size<int> &size) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(rect, size);
}

auto RasterReader::read(double scaleX, double scaleY, const Rect<int> &rect) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(scaleX, scaleY, rect);
}

auto RasterReader::read(const BoundingBox2i &window, double scaleX, double scaleY) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(window, scaleX, scaleY);
}

auto RasterReader::read(const BoundingBox2d &terrainWindow, double scaleX, double scaleY, Affine<double, 2> *affine) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(terrainWindow, scaleX, scaleY, affine);
}

void RasterReader::update(const cv::Mat &image, const Rect<int> &rect)
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->update(image, rect);
}

void RasterReader::update(const cv::Mat &image, const BoundingBox2i &window)
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->update(image, window);
}

void RasterReader::copy(const tl::Path &outputPath,
                        const ImageOptions &options, 
                        const ImageMetadata &metadata, 
                        const std::string &epsgCode) const
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->copy(outputPath.toString(), options, metadata, epsgCode);
}

void RasterReader::addOverviews(int levels, const ImageOptions &options)
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->addOverviews(levels, options);
}

auto RasterReader::rows() const -> int
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->rows();
}

auto RasterReader::cols() const -> int
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->cols();
}

auto RasterReader::channels() const -> int
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->channels();
}

auto RasterReader::dataType() const -> DataType
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->dataType();
}

auto RasterReader::depth() const -> int
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->depth();
}

auto RasterReader::metadata() const -> ImageMetadata
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->metadata();
}

auto RasterReader::isGeoreferenced() const -> bool
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->isGeoreferenced();
}

auto RasterReader::georeference() const -> Affine<double, 2>
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->georeference();
}

auto RasterReader::crsWkt() const -> std::string
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->crsWkt();
}

auto RasterReader::window() const -> BoundingBox2d
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->window();
}

auto RasterReader::noDataValue(bool *exist) const -> double
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->noDataValue(exist);
}


} // End namespace tl