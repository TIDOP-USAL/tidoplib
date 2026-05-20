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

#include "tidop/rastertools/io/Metadata.h"
#include "tidop/core/app/Message.h"

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

auto ImageMetadata::existMetadata(const std::string &key) -> bool
{
    auto metadata = mMetadata.find(key);
    if (metadata != mMetadata.end()) {
        return true;
    }
    return false;
}

auto ImageMetadata::metadata(const std::string &key, bool &active) const -> std::string
{
    active = false;
    auto metadata = mMetadata.find(key);
    if (metadata != mMetadata.end()) {
        active = true;
        return metadata->second;
    }

    return std::string();
}

auto ImageMetadata::metadata(const std::vector<std::string> &keys, bool &active) const -> std::string
{
    for (const auto &key : keys) {
        auto value = metadata(key, active);
        if (active)
            return value;
    }

    return std::string();
}

void ImageMetadata::setMetadata(const std::string &key, const std::string &value)
{
    mMetadata[key] = value;
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
