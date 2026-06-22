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

 #include "tidop/pctools/io/impl/PointCloudReader.h"
 
//#include <fstream>
//#include <iostream>
//#include <algorithm>
//#include <string>
//
//#include "tidop/core/base/Exception.h"
//
//#include "tidop/geotools/GeoTools.h"
//#include "tidop/geotools/CRSsTools.h"
//
//#include "tidop/pctools/PointCloudReader.h"
//
//TL_DISABLE_WARNINGS
//#include <proj.h>
//
//#include <copc-lib/las/header.hpp>
//#include <lazperf/readers.hpp>
//#include <copc-lib/io/copc_reader.hpp>
//#include <copc-lib/laz/decompressor.hpp>
//TL_DEFAULT_WARNINGS

namespace tl
{

PointCloudReaderBase::PointCloudReaderBase(Path file)
  : mPtrGeoTools(nullptr),
    mFile(std::move(file))
{
}

auto PointCloudReaderBase::file() const -> Path
{
    return mFile;
}


}
