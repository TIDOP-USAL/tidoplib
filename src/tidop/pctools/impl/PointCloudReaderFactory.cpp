/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by David Hernandez Lopez                            *
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

#include <map>
#include <string>
#include <memory>

#include "tidop/config.h"
#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"
#include "tidop/core/exception.h"
#include "tidop/core/utils.h"

#include <proj.h>
#include <copc-lib/las/header.hpp>
#include <lazperf/readers.hpp>
#include <copc-lib/io/copc_reader.hpp>
#include <copc-lib/laz/decompressor.hpp>


#include "tidop/geotools/GeoTools.h"
#include "tidop/pctools/PointCloudReader.h"
#include "tidop/pctools/impl/PointCloudReaderPDAL.h"
#include "tidop/pctools/impl/PlyReader.h"

namespace tl
{
auto PointCloudReaderFactory::create(const Path &file) -> PointCloudReader::Ptr
{
    PointCloudReader::Ptr pointCloudReader;

    TL_ASSERT(file.exists(), "File doesn't exist: {}", file.toString());
    std::string extension = file.extension().toString();
    if (compareInsensitiveCase(extension, ".las")
        || compareInsensitiveCase(extension, ".laz")) {
        pointCloudReader = PointCloudReaderPDAL::New(file);
    } else if (compareInsensitiveCase(extension, ".ply")) {
        pointCloudReader = PlyReader::New(file);
    } else {
        TL_THROW_EXCEPTION("Invalid Point Cloud Reader: {}", file.fileName().toString());
    }

    return pointCloudReader;
}

}