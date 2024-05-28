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

#include "tidop/vect/formats.h"


namespace tl
{



VectorOptions::VectorOptions(Format format)
  : mFormat(format)
{
}

VectorOptions::~VectorOptions() = default;

auto VectorOptions::format() const -> Format
{
    return mFormat;
}




ShapeOptions::ShapeOptions()
  : VectorOptions(Format::shp),
    bAdjustType(false),
    mAdjustGeomType(AdjustGeomType::first_shape),
    bAutoRepack(true),
    bDbfEofChar(true)
{
}

ShapeOptions::~ShapeOptions() = default;

auto ShapeOptions::options() const -> const char*
{
    return nullptr;
}

void ShapeOptions::enableAdjustType(bool value)
{
#if  GDAL_VERSION_MAJOR >= 2
    bAdjustType = value;
#endif
}

void ShapeOptions::enableAutoRepac(bool value)
{
    bAutoRepack = value;
}

void ShapeOptions::enableDbfEofChar(bool value)
{
    bDbfEofChar = value;
}


auto ShapeOptions::encoding() const -> std::string
{
    return mEncoding;
}

void ShapeOptions::setEncoding(const std::string &encoding)
{
    mEncoding = encoding;
}

auto ShapeOptions::dbfDateLastUpdate() const -> std::string
{
    return mDbfDateLastUpdate;
}

void ShapeOptions::setDbfDateLastUpdate(const std::string &date)
{
    mDbfDateLastUpdate = date;
}

auto ShapeOptions::adjustGeomType() const -> AdjustGeomType
{
    return mAdjustGeomType;
}

void ShapeOptions::setAdjustGeomType(AdjustGeomType type)
{
    mAdjustGeomType = type;
}

}
