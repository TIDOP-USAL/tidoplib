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

#include "tidop/vectortools/io/formats.h"


namespace tl
{



VectorOptionsBase::VectorOptionsBase(Format format)
  : mFormat(format)
{
}

VectorOptionsBase::~VectorOptionsBase()
{
}

auto VectorOptionsBase::format() const -> Format
{
    return mFormat;
}

auto VectorOptionsBase::options() const -> std::map<std::string, std::string>
{
    return this->options(true);
}

auto VectorOptionsBase::activeOptions() const -> std::map<std::string, std::string>
{
    return this->options(false);
}



//ShapeOptions::ShapeOptions()
//  : VectorOptionsBase(Format::shp)
//{
//    this->init();
//}
//
//ShapeOptions::~ShapeOptions() = default;
//
//void ShapeOptions::reset()
//{
//    this->init();
//}
//
//void ShapeOptions::enableAdjustType(bool value)
//{
//#if  GDAL_VERSION_MAJOR >= 2
//    bAdjustType.second = value;
//#endif
//}
//
//void ShapeOptions::enableAutoRepac(bool value)
//{
//    bAutoRepack.second = value;
//}
//
//void ShapeOptions::enableDbfEofChar(bool value)
//{
//    bDbfEofChar.second = value;
//}
//
//
//auto ShapeOptions::encoding() const -> std::string
//{
//    return mEncoding.second;
//}
//
//void ShapeOptions::setEncoding(const std::string &encoding)
//{
//    mEncoding.second = encoding;
//}
//
//auto ShapeOptions::dbfDateLastUpdate() const -> std::string
//{
//    return mDbfDateLastUpdate.second;
//}
//
//void ShapeOptions::setDbfDateLastUpdate(const std::string &date)
//{
//    mDbfDateLastUpdate.second = date;
//}
//
//auto ShapeOptions::adjustGeomType() const -> AdjustGeomType
//{
//    return mAdjustGeomType.second;
//}
//
//void ShapeOptions::setAdjustGeomType(AdjustGeomType type)
//{
//    mAdjustGeomType.second = type;
//}
//
//void ShapeOptions::init()
//{
//    mEncoding = std::make_pair("", "");
//    mDbfDateLastUpdate = std::make_pair("", "");
//    bAdjustType = std::make_pair(false, false);
//    mAdjustGeomType = std::make_pair(AdjustGeomType::first_shape, AdjustGeomType::first_shape);
//    bAutoRepack = std::make_pair(true, true);
//    bDbfEofChar = std::make_pair(true, true);
//}
//
//auto ShapeOptions::options(bool all) const -> std::map<std::string, std::string>
//{
//    std::map<std::string, std::string> options;
//
//    if (all || mEncoding.first != mEncoding.second) {
//        options["SOURCE_ICC_PROFILE"] = mEncoding.second;
//    }
//
//    if (all || mDbfDateLastUpdate.first != mDbfDateLastUpdate.second) {
//        options["COMMENT"] = mDbfDateLastUpdate.second;
//    }
//
//    if (all || bAdjustType.first != bAdjustType.second) {
//        options["ADJUST_TYPE"] = bAdjustType.second ? "YES" : "NO";
//    }
//
//    if (all || mAdjustGeomType.first != mAdjustGeomType.second) {
//        switch (mAdjustGeomType.second) {
//        case AdjustGeomType::no:
//            options["ADJUST_GEOM_TYPE"] = "NO";
//            break;
//        case AdjustGeomType::first_shape:
//            options["ADJUST_GEOM_TYPE"] = "FIRST_SHAPE";
//            break;
//        case AdjustGeomType::all_shapes:
//            options["ADJUST_GEOM_TYPE"] = "ALL_SHAPES";
//            break;
//        }
//    }
//
//    if (all || bAutoRepack.first != bAutoRepack.second) {
//        options["AUTO_REPACK"] = bAutoRepack.second ? "YES" : "NO";
//    }
//
//    if (all || bDbfEofChar.first != bDbfEofChar.second) {
//        options["DBF_EOF_CHAR"] = bDbfEofChar.second ? "YES" : "NO";
//    }
//
//    return options;
//}


} // namespace tl
