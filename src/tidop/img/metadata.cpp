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





ImageMetadata::ImageMetadata()
{
}

ImageMetadata::~ImageMetadata() = default;

auto ImageMetadata::existMetadata(const std::string &name) -> bool
{
    auto metadata = mMetadata.find(name);
    if (metadata != mMetadata.end()) {
        return true;
    }
    return false;
}

auto ImageMetadata::metadata(const std::string &name, bool &active) const -> std::string
{
    std::string value;
    active = false;
    auto metadata = mMetadata.find(name);
    if (metadata != mMetadata.end()) {
        value = metadata->second;
        active = true;
    }

    return value;
}

void ImageMetadata::setMetadata(const std::string &name, const std::string &value)
{
    mMetadata[name] = value;
}

auto ImageMetadata::begin() TL_NOEXCEPT -> iterator
{
    return mMetadata.begin();
}

auto ImageMetadata::begin() const TL_NOEXCEPT -> const_iterator
{
    return mMetadata.cbegin();
}

auto ImageMetadata::end() TL_NOEXCEPT -> iterator
{
    return mMetadata.end();
}

auto ImageMetadata::end() const TL_NOEXCEPT -> const_iterator
{
    return mMetadata.cend();
}

auto ImageMetadata::empty() const -> bool
{
    return mMetadata.empty();
}

auto ImageMetadata::size() const -> size_t
{
    return mMetadata.size();
}

void ImageMetadata::clear()
{
    mMetadata.clear();
}



} // End namespace tl
