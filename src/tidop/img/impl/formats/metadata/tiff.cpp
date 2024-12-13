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

#include "tidop/img/impl/formats/metadata/tiff.h"

namespace tl
{

TiffMetadata::TiffMetadata()
  : ExifMetadata(Format::tiff)
{
    TiffMetadata::init();
}

TiffMetadata::~TiffMetadata() = default;

void TiffMetadata::init()
{
    mMetadata["TIFFTAG_DOCUMENTNAME"] = std::make_pair("", false);
    mMetadata["TIFFTAG_IMAGEDESCRIPTION"] = std::make_pair("", false);
    mMetadata["TIFFTAG_SOFTWARE"] = std::make_pair("", false);
    mMetadata["TIFFTAG_DATETIME"] = std::make_pair("", false);
    mMetadata["TIFFTAG_ARTIST"] = std::make_pair("", false);
    mMetadata["TIFFTAG_HOSTCOMPUTER"] = std::make_pair("", false);
    mMetadata["TIFFTAG_COPYRIGHT"] = std::make_pair("", false);
    mMetadata["TIFFTAG_XRESOLUTION"] = std::make_pair("", false);
    mMetadata["TIFFTAG_YRESOLUTION"] = std::make_pair("", false);
    mMetadata["TIFFTAG_RESOLUTIONUNIT"] = std::make_pair("", false);
    mMetadata["TIFFTAG_MINSAMPLEVALUE"] = std::make_pair("", false);
    mMetadata["TIFFTAG_MAXSAMPLEVALUE"] = std::make_pair("", false);
    mMetadata["GEO_METADATA"] = std::make_pair("", false);
    mMetadata["TIFF_RSID"] = std::make_pair("", false);
}


} // End namespace tl
