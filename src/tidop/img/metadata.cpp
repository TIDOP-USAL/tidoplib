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

#include "tidop/img/metadata.h"
#include "tidop/img/impl/formats/metadata/bmp.h"
#include "tidop/img/impl/formats/metadata/exif.h"
#include "tidop/img/impl/formats/metadata/gif.h"
#include "tidop/img/impl/formats/metadata/jpeg.h"
#include "tidop/img/impl/formats/metadata/png.h"
#include "tidop/img/impl/formats/metadata/tiff.h"
#include "tidop/core/msg/message.h"

namespace tl
{


MetadataItemBase::MetadataItemBase(std::string name,
                                   std::string defValue)
  : MetadataItem(),
    mName(std::move(name)),
    mDefaultValue(std::move(defValue)),
    bActive(false)
{
}

auto MetadataItemBase::value() const -> std::string
{
    return mValue;
}

void MetadataItemBase::setValue(const std::string &value)
{
    bActive = true;
    mValue = value;
}

auto MetadataItemBase::defaultValue() const -> std::string
{
    return mDefaultValue;
}

void MetadataItemBase::setDefaultValue(const std::string &defValue)
{
    mDefaultValue = defValue;
}

auto MetadataItemBase::isActive() const -> bool
{
    return bActive;
}



MetadataItemNumber::MetadataItemNumber(const std::string &name,
                                       const std::string &defValue)
    : MetadataItemBase(name, defValue)
{
}

void MetadataItemNumber::parseValue(const std::string &value)
{
    size_t pos1 = value.find('(');
    size_t pos2 = value.find(')');

    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        setValue(value.substr(pos1 + 1, pos2 - pos1 + 1));
    }
}



MetadataItemText::MetadataItemText(const std::string &name,
                                   const std::string &defValue)
    : MetadataItemBase(name, defValue)
{
}

void MetadataItemText::parseValue(const std::string &value)
{
    setValue(value);
}





ImageMetadata::ImageMetadata(Format format)
    : mFormat(format)
{
}

ImageMetadata::~ImageMetadata() = default;

auto ImageMetadata::format() const -> Format
{
    return mFormat;
}



std::shared_ptr<ImageMetadata> ImageMetadataFactory::create(const std::string &format)
{
    std::shared_ptr<ImageMetadata> imageMetadata;

    if (format == "JPEG") {
        imageMetadata = std::make_shared<JpegMetadata>();
    } else if (format == "GTiff") {
        imageMetadata = std::make_shared<TiffMetadata>();
    } else if (format == "PNG") {
        imageMetadata = std::make_shared<PngMetadata>();
    } else if (format == "BMP") {
        imageMetadata = std::make_shared<BmpMetadata>();
    } else if (format == "GIF") {
        imageMetadata = std::make_shared<GifMetadata>();
    } else {
        throw std::runtime_error("Invalid Image Format");
    }

    return imageMetadata;
}


} // End namespace tl
