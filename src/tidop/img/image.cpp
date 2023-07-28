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

#include "tidop/img/image.h"


namespace tl
{

Image::Image() = default;

Image::Image(int rows,
             int cols,
             DataType type,
             int channels)
  : mRows(rows),
    mCols(cols),
    mType(type),
    mChannels(channels),
    mData(nullptr)
{
    init();
}

Image::Image(int rows,
             int cols,
             DataType type,
             int channels,
             const graph::Color &color)
  : mRows(rows),
    mCols(cols),
    mType(type),
    mChannels(channels),
    mData(nullptr)
{
    unusedParameter(color); /// Para inicializar la imagen con ese color
    init();
}

Image::Image(int rows,
             int cols,
             DataType type,
             int channels,
             void *data)
  : mRows(rows),
    mCols(cols),
    mType(type),
    mChannels(channels),
    mData(static_cast<unsigned char *>(data))
{

}

Image::Image(const SizeI &size,
             DataType type,
             int channels)
  : mRows(size.height),
    mCols(size.width),
    mType(type),
    mChannels(channels),
    mData(nullptr)
{
    init();
}

Image::Image(const SizeI &size,
             DataType type,
             int channels,
             const graph::Color &color)
  : mRows(size.height),
    mCols(size.width),
    mType(type),
    mChannels(channels),
    mData(nullptr)
{
    unusedParameter(color); /// Para inicializar la imagen con ese color
    init();
}

Image::Image(const SizeI &size,
             DataType type,
             int channels,
             void *data)
  : mRows(size.height),
    mCols(size.width),
    mType(type),
    mChannels(channels),
    mData(static_cast<unsigned char *>(data))
{

}

Image::Image(const Image &image) = default;
Image::~Image() = default;

Image &Image::operator=(const Image &image)
{
    if (this != &image) {
        mRows = image.mRows;
        mCols = image.mCols;
        mType = image.mType;
        mChannels = image.mChannels;
        mData = image.mData;
    }
    return *this;
}

int Image::rows() const
{
    return mRows;
}

int Image::cols() const
{
    return mCols;
}

DataType Image::type() const
{
    return mType;
}

int Image::channels() const
{
    return mChannels;
}

unsigned char *Image::data()
{
    return mData;
}

int Image::depth()
{
    int bits = 0;
    TL_TODO("Completar")
    switch (mType) {
    case tl::DataType::TL_8U:
        bits = 8;
        break;
    case tl::DataType::TL_8S:
        bits = 8;
        break;
    case tl::DataType::TL_16U:
        bits = 16;
        break;
    case tl::DataType::TL_16S:
        bits = 16;
        break;
    case tl::DataType::TL_32U:
        break;
    case tl::DataType::TL_32S:
        break;
    case tl::DataType::TL_32F:
        bits = 32;
        break;
    case tl::DataType::TL_64F:
        bits = 64;
        break;
    }

    return bits;
}

bool Image::isEmpty()
{
    return false;
}

void Image::init()
{
    mData = static_cast<unsigned char *>(std::malloc(static_cast<size_t>(mRows) *
                                         static_cast<size_t>(mCols) *
                                         static_cast<size_t>(mChannels) *
                                         static_cast<size_t>(this->depth())));
}

} // End namespace tl
