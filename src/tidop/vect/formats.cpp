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

/* --------------------------------------------------------------------------- */


VectorOptions::VectorOptions(Format format) 
  : FileOptions(),
    mFormat(format)
{}

VectorOptions::~VectorOptions()
{}

VectorOptions::Format VectorOptions::getFormat()
{
  return mFormat;
}


/* --------------------------------------------------------------------------- */


ShapeOptions::ShapeOptions() 
  : VectorOptions(VectorOptions::Format::shp),
    mEncoding(""),
    mDbfDateLastUpdate(""),
    bAdjustType(false),
    mAdjustGeomType(ADJUST_GEOM_TYPE::FIRST_SHAPE),
    bAutoRepack(true),
    bDbfEofChar(true)
{
}

ShapeOptions::~ShapeOptions()
{}

const char *ShapeOptions::getOptions()
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


std::string ShapeOptions::getEncoding() const
{
  return mEncoding;
}

void ShapeOptions::setEncoding(const std::string &encoding)
{
  mEncoding = encoding;
}

std::string ShapeOptions::getDbfDateLastUpdate() const
{
  return mDbfDateLastUpdate;
}

void ShapeOptions::setDbfDateLastUpdate(const std::string &date)
{
  mDbfDateLastUpdate = date;
}

ShapeOptions::ADJUST_GEOM_TYPE ShapeOptions::getAdjustGeomType() const
{
  return mAdjustGeomType;
}

void ShapeOptions::setAdjustGeomType(ADJUST_GEOM_TYPE type)
{
  mAdjustGeomType = type;
}

}
