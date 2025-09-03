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

#include "tidop/rastertools/io/ImageReader.h"

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
    mReader = ImageReaderFactory::create(file);
    mReader->open();
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

auto RasterReader::read(const Rect<int> &rect, const Size<int> &size, Affine<int, 2> *affine) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(rect, size, affine);
}

auto RasterReader::read(double scaleX, double scaleY, const Rect<int> &rect, Affine<int, 2> *affine) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(scaleX, scaleY, rect, affine);
}

auto RasterReader::read(const WindowI &window, double scaleX, double scaleY, Affine<int, 2> *affine) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(window, scaleX, scaleY, affine);
}

auto RasterReader::read(const Window<Point<double>> &terrainWindow, double scaleX, double scaleY, Affine<int, 2> *affine) -> cv::Mat
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->read(terrainWindow, scaleX, scaleY, affine);
}

void RasterReader::update(const cv::Mat &image, const Rect<int> &rect)
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->update(image, rect);
}

void RasterReader::update(const cv::Mat &image, const WindowI &window)
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->update(image, window);
}

void RasterReader::copy(const tl::Path &outputPath,
                        const std::shared_ptr<ImageOptions> &options, 
                        const std::shared_ptr<ImageMetadata> &metadata, 
                        const std::string &epsgCode) const
{
    TL_ASSERT(isOpen(), "RasterReader is not open");
    return mReader->copy(outputPath.toString(), options, metadata, epsgCode);
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

auto RasterReader::metadata() const -> std::shared_ptr<ImageMetadata>
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

auto RasterReader::window() const -> WindowD
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