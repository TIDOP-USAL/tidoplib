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

#include "tidop/rastertools/impl/io/metadata/metadata.h"

#include "tidop/core/app/message.h"

namespace tl
{

auto ImageMetadataBase::metadata(const std::string& name, bool& active) const -> std::string
{
    std::string value;
    active = false;

    auto metadata = mMetadata.find(name);
    if (metadata != mMetadata.end()) {
        value = metadata->second.first;
        active = metadata->second.second;
    } else {
        Message::warning("Metadata '{}' not supported", name);
    }

    return value;
}

void ImageMetadataBase::setMetadata(const std::string &name, const std::string &value)
{
    auto metadata = mMetadata.find(name);
    if (metadata != mMetadata.end()) {
        metadata->second.first = value;
        metadata->second.second = true;
    } /*else {
      msgWarning("Metadata '%s' not supported", name.c_str());
    }*/
}

auto ImageMetadataBase::metadata() const -> std::map<std::string, std::string>
{
    return this->metadata(true);
}

auto ImageMetadataBase::activeMetadata() const -> std::map<std::string, std::string>
{
    return this->metadata(false);
}

void ImageMetadataBase::reset()
{
    this->init();
}

auto ImageMetadataBase::metadata(bool all) const -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> metadata;

    for (auto &pair : mMetadata) {
        if (all || pair.second.second == true)
            metadata[pair.first] = pair.second.first;
    }

    return metadata;
}


} // End namespace tl
