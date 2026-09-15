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


auto ImageMetadata::contains(std::string_view key) -> bool
{
    return mMetadata.contains(key);
}

auto ImageMetadata::value(std::string_view key) const -> std::optional<std::string>
{
    if (auto it = mMetadata.find(key); it != mMetadata.end()) {
        return it->second;
    }
    return std::nullopt;
}

auto ImageMetadata::value(std::span<const std::string> keys) const -> std::optional<std::string>
{
    for (const auto &key : keys) {
        if (auto it = mMetadata.find(key); it != mMetadata.end()) {
            return it->second;
        }
    }
    return std::nullopt;
}

void ImageMetadata::set(std::string key, std::string value)
{
    mMetadata[std::move(key)] = std::move(value);
}

auto ImageMetadata::begin() noexcept -> iterator
{
    return mMetadata.begin();
}

auto ImageMetadata::begin() const noexcept -> const_iterator
{
    return mMetadata.cbegin();
}

auto ImageMetadata::end() noexcept -> iterator
{
    return mMetadata.end();
}

auto ImageMetadata::end() const noexcept -> const_iterator
{
    return mMetadata.cend();
}

auto ImageMetadata::empty() const noexcept -> bool
{
    return mMetadata.empty();
}

auto ImageMetadata::size() const noexcept -> size_t
{
    return mMetadata.size();
}

void ImageMetadata::clear()
{
    mMetadata.clear();
}



} // End namespace tl
