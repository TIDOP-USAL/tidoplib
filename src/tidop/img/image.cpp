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
    mChannels(channels)
{
    init();
}

Image::Image(int rows,
             int cols,
             DataType type,
             int channels,
             const Color &color)
  : mRows(rows),
    mCols(cols),
    mType(type),
    mChannels(channels)
{
    init();

    int pixelSize = depth() / 8; // depth() devuelve bits, convertir a bytes
    unsigned char r = static_cast<unsigned char>(color.red());
    unsigned char g = static_cast<unsigned char>(color.green());
    unsigned char b = static_cast<unsigned char>(color.blue());
    unsigned char a = color.opacity();

    for (int i = 0; i < mRows; ++i) {
        for (int j = 0; j < mCols; ++j) {
            int index = (i * mCols + j) * pixelSize * mChannels;
            if (channels == 1) {
                mData[index] = r;
            } else if (channels == 3) {
                mData[index] = r;
                mData[index + 1] = g;
                mData[index + 2] = b;
            } else if (channels == 4) {
                mData[index] = r;
                mData[index + 1] = g;
                mData[index + 2] = b;
                mData[index + 3] = a;
            }
        }
    }
}

Image::Image(int rows,
             int cols,
             DataType type,
             int channels,
             void *data)
  : mRows(rows),
    mCols(cols),
    mType(type),
    mChannels(channels)
{
    init();
    auto data_size = static_cast<size_t>(mRows) * mCols * mChannels * (depth() / 8);
    std::memcpy(mData, data, data_size);
}

Image::Image(const Size<int> &size,
             DataType type,
             int channels)
  : Image(size.height, size.width, type, channels)
{
}

Image::Image(const Size<int> &size,
             DataType type,
             int channels,
             const Color &color)
  : Image(size.height, size.width, type, channels, color)
{
}

Image::Image(const Size<int> &size,
             DataType type,
             int channels,
             void *data)
  : Image(size.height, size.width, type, channels, data)
{
}

Image::Image(const Image &image)
  : mRows(image.mRows),
    mCols(image.mCols),
    mType(image.mType),
    mChannels(image.mChannels)
{
    auto size = static_cast<size_t>(mRows) * mCols * mChannels * (depth() / 8);
    mData = static_cast<unsigned char *>(std::malloc(size));
    std::memcpy(mData, image.mData, size);
}

Image::Image(Image &&image) TL_NOEXCEPT
  : mRows(std::exchange(image.mRows,0)),
    mCols(std::exchange(image.mCols,0)),
    mType(std::exchange(image.mType, DataType::TL_8U)),
    mChannels(std::exchange(image.mChannels, 0)),
    mData(std::exchange(image.mData, nullptr))
{

}

Image::~Image()
{
    if (mData != nullptr) {
        delete[] mData;
        mData = nullptr;
    }
};

auto Image::operator=(const Image& image) -> Image&
{
    if (this != &image) {

        if (mData != nullptr) {
            delete[] mData;
            mData = nullptr;
        }

        mRows = image.mRows;
        mCols = image.mCols;
        mType = image.mType;
        mChannels = image.mChannels;
        auto size = static_cast<size_t>(mRows) * mCols * mChannels * (depth() / 8);
        mData = static_cast<unsigned char *>(std::malloc(size));
        memcpy(mData, image.mData, size);
    }

    return *this;
}

auto Image::operator=(Image&& image) noexcept -> Image&
{
    if (this != &image) {

        if (mData != nullptr) {
            delete[] mData;
            mData = nullptr;
        }

        mRows = std::exchange(image.mRows, 0);
        mCols = std::exchange(image.mCols, 0);
        mType = std::exchange(image.mType, DataType::TL_8U);
        mChannels = std::exchange(image.mChannels, 1);
        mData = std::exchange(image.mData, nullptr);
    }

    return *this;
}

auto Image::rows() const -> int
{
    return mRows;
}

auto Image::cols() const -> int
{
    return mCols;
}

auto Image::type() const -> DataType
{
    return mType;
}

auto Image::channels() const -> int
{
    return mChannels;
}

unsigned char *Image::data() const
{
    return mData;
}

auto Image::depth() const -> int
{
    int bits = 0;
    TL_TODO("Completar")
    switch (mType) {
    case tl::DataType::TL_8U:
    case tl::DataType::TL_8S:
        bits = 8;
        break;
    case tl::DataType::TL_16U:
    case tl::DataType::TL_16S:
        bits = 16;
        break;
    case tl::DataType::TL_32U:
    case tl::DataType::TL_32S:
    case tl::DataType::TL_32F:
        bits = 32;
        break;
    case tl::DataType::TL_64F:
        bits = 64;
        break;
    }

    return bits;
}

auto Image::isEmpty() const -> bool
{
    return (mData == nullptr) || (mRows == 0) || (mCols == 0);
}

void Image::init()
{
    //mData = static_cast<unsigned char *>(std::malloc(static_cast<size_t>(mRows) *
    //                                                 static_cast<size_t>(mCols) *
    //                                                 static_cast<size_t>(mChannels) *
    //                                                 static_cast<size_t>(this->depth())));
    auto size = static_cast<size_t>(mRows) * static_cast<size_t>(mCols) *
                static_cast<size_t>(mChannels) * static_cast<size_t>(depth() / 8);
    mData = static_cast<unsigned char *>(std::malloc(size));
    std::memset(mData, 0, size);
    if (mData == nullptr) {
        throw std::bad_alloc();
    }
}

} // End namespace tl
