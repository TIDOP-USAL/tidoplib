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


#include "tidop/pctools/impl/ply/ply.h"


namespace tl
{


Ply::Ply()
  : stream(nullptr),
    mIsBinary(false),
    mIsLittleEndian(false),
    mHasColors(false),
    mHasNormals(false),
    mHasScalarFields(false),
    mSize(0)
{
    init();
}

Ply::Ply(tl::Path file, OpenMode mode)
  : _file(std::move(file)),
    stream(nullptr),
    mIsBinary(false),
    mIsLittleEndian(false),
    mHasColors(false),
    mHasNormals(false),
    mHasScalarFields(false)
{
    init();
    open(_file, mode);
}

Ply::~Ply()
{
    if (stream) {
        delete stream;
        stream = nullptr;
    }
}

void Ply::open(const tl::Path& file, OpenMode mode)
{
    _file = file;
    flags = mode;

    if (flags.isEnabled(OpenMode::in)) {

        TL_ASSERT(tl::Path::exists(file), "File does not exist");

        stream = new std::fstream(_file.toString(), std::ios_base::in | std::ios_base::binary);

        readHeader();
    }
}

void Ply::read()
{
    if (flags.isEnabled(OpenMode::in) && stream->is_open()) {

        TL_ASSERT(stream != nullptr, "File not open");

        for (auto &property : mProperties) {
            property.second->resize(mSize);
        }

        readBody();

        stream->close();
    }
}

void Ply::save(bool binary, bool littleEndian)
{
    mIsBinary = binary;
    mIsLittleEndian = littleEndian;
    std::ios_base::openmode open_mode = std::ios_base::out;

    stream = new std::fstream(_file.toString(), open_mode | std::ios_base::trunc);

    writeHeader();

    if (mIsBinary) {
        stream->close();
        stream->open(_file.toString(), open_mode | std::ios_base::app | std::ios_base::binary);
    }

    writeBody();

    stream->close();
}

void Ply::close() const
{
    if (stream && stream->is_open())
        stream->close();
}

void Ply::resize(size_t size)
{
    mSize = size;

    for (auto &property : mProperties){
        property.second->resize(size);
    }
}

void Ply::reserve(size_t size) const
{
    for (auto &property : mProperties) {
        property.second->reserve(size);
    }
}

void Ply::setProperty(const std::string& name, PlyProperty::Type type)
{
    mPropertiesOrder.push_back(name);
    mProperties[name] = createProperty(name, type);
}

auto Ply::color(size_t index) const -> tl::Color
{
    return tl::ColorRGB(property<int>(index, "red"),
                        property<int>(index, "green"),
                        property<int>(index, "blue")).toColor();
}

void Ply::addColor(const tl::Color& color)
{
    mHasColors = true; // Esto se tiene que definir antes
    addProperty("red", color.red());
    addProperty("green", color.green());
    addProperty("blue", color.blue());
}

auto Ply::property(const std::string& propertyName) const -> std::shared_ptr<PlyProperty>
{
    return mProperties.at(propertyName);
}

auto Ply::propertyNames() const -> std::vector<std::string>
{
    return mPropertiesOrder;
}

void Ply::init()
{
    mStringTypes["unknown"] = PlyProperty::Type::ply_unknown;
    mStringTypes["int8"] = PlyProperty::Type::ply_int8;
    mStringTypes["uint8"] = PlyProperty::Type::ply_uint8;
    mStringTypes["int16"] = PlyProperty::Type::ply_int16;
    mStringTypes["uint16"] = PlyProperty::Type::ply_uint16;
    mStringTypes["int32"] = PlyProperty::Type::ply_int32;
    mStringTypes["uint32"] = PlyProperty::Type::ply_uin32;
    mStringTypes["float32"] = PlyProperty::Type::ply_float32;
    mStringTypes["float64"] = PlyProperty::Type::ply_float64;
    mStringTypes["char"] = PlyProperty::Type::ply_char;
    mStringTypes["uchar"] = PlyProperty::Type::ply_uchar;
    mStringTypes["short"] = PlyProperty::Type::ply_short;
    mStringTypes["ushort"] = PlyProperty::Type::ply_ushort;
    mStringTypes["int"] = PlyProperty::Type::ply_int;
    mStringTypes["uint"] = PlyProperty::Type::ply_uint;
    mStringTypes["float"] = PlyProperty::Type::ply_float;
    mStringTypes["double"] = PlyProperty::Type::ply_double;
}

void Ply::readHeader()
{
    std::string line;

    std::getline(*stream, line);

    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    TL_ASSERT(line == "ply", "Invalid Ply file");

    std::string name;
    std::string type;
    std::string value;

    int coordinates_dimension{};
    int normals_dimension{};
    int color_dimension{};

    while (std::getline(*stream, line)) {

        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

        if (line == "end_header") break;

        std::istringstream isstream(line);
        isstream >> name >> type >> value;

        if (name == "format") {

            if (type == "binary_little_endian") {
                mIsBinary = true;
                mIsLittleEndian = true;
            } else if (type == "binary_big_endian") {
                mIsBinary = true;
                mIsLittleEndian = false;
            } else if (type == "ascii") {
                mIsBinary = false;
            } else {
                /// TODO: devolver error
            }

        } else if (name == "element") {

            if (type == "vertex") {
                mSize = tl::convertStringTo<size_t>(value);
            } /*else if (type == "vertex") {
                  mFaceSize = tl::convertStringTo<size_t>(value);
                }*/ else {
                tl::Message::error("Error reading Ply: Only vertex elements supported");
            }

        } else if (name == "property") {

            setProperty(value, findType(type));

            if (tl::compareInsensitiveCase(value, "x") ||
                tl::compareInsensitiveCase(value, "y") ||
                tl::compareInsensitiveCase(value, "z")) {

                coordinates_dimension++;

            } else if (tl::compareInsensitiveCase(value, "red") || 
                       tl::compareInsensitiveCase(value, "green") || 
                       tl::compareInsensitiveCase(value, "blue")) {

                color_dimension++;

            } else if (tl::compareInsensitiveCase(value, "nx") ||
                       tl::compareInsensitiveCase(value, "ny") ||
                       tl::compareInsensitiveCase(value, "nz") ||
                       tl::compareInsensitiveCase(value, "normal_x") ||
                       tl::compareInsensitiveCase(value, "normal_y") ||
                       tl::compareInsensitiveCase(value, "normal_z") ||
                       tl::compareInsensitiveCase(value, "normalx") ||
                       tl::compareInsensitiveCase(value, "normaly") ||
                       tl::compareInsensitiveCase(value, "normalz") ||
                       tl::compareInsensitiveCase(value, "normalx_8knn") ||
                       tl::compareInsensitiveCase(value, "normaly_8knn") ||
                       tl::compareInsensitiveCase(value, "normalz_8knn")) {

                normals_dimension++;

            } else {

                mHasScalarFields = true;

            }

        } else {
            // Comment
        }
    }

    TL_ASSERT(coordinates_dimension == 3, "Invalid Ply file");
    mHasColors = color_dimension == 3;
    mHasNormals = normals_dimension == 3;
}

void Ply::readBody()
{
    if (mIsBinary) {
        readBinaryBody();
    } else {
        readTextBody();
    }
}

void Ply::readTextBody()
{
    std::string line;

    size_t id{};

    while (std::getline(*stream, line)) {

        while (line.empty()) {
            std::getline(*stream, line);
        }

        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

        std::stringstream line_stream(line);

        std::string item;
        std::vector<std::string> items;
        auto property_order = mPropertiesOrder.begin();

        while (!line_stream.eof()) {

            std::getline(line_stream, item, ' ');
            if (item.empty()) continue;
            auto property = mProperties[*property_order];
            property->setValue(id, item);
            ++property_order;
        }

        id++;
    }
}

void Ply::readBinaryBody()
{
    for (size_t i = 0; i < mSize; ++i) {
        for (auto &property_name : mPropertiesOrder) {
            auto property = mProperties[property_name];
            property->read(stream, i);
        }
    }
}

void Ply::writeHeader()
{
    TL_ASSERT(stream->is_open(), "File not open");

    std::string endianness;
    if (mIsBinary && mIsLittleEndian)
        endianness = "binary_little_endian";
    else if (mIsBinary && !mIsLittleEndian)
        endianness = "binary_big_endian";
    else
        endianness = "ascii";

    *stream << "ply\n"
        << "format " << endianness << " 1.0\n"
        << "element vertex " << mSize << std::endl;

    *stream << "comment Created by GRAPHOS" << std::endl;

    for (auto &property_name : mPropertiesOrder) {

        auto property = mProperties[property_name];
        std::string property_type = findStringType(property->type());
        *stream << "property " << property_type << " " << property_name << "\n";
    }

    *stream << "end_header" << std::endl;
}

void Ply::writeBody()
{
    if (mIsBinary) {
        writeBinaryBody();
    } else {
        writeTextBody();
    }
}

void Ply::writeTextBody()
{
    *stream << std::fixed;

    for (size_t i = 0; i < mSize; ++i) {

        for (size_t j = 0; j < mPropertiesOrder.size(); j++) {
            auto property_name = mPropertiesOrder[j];
            auto property = mProperties[property_name];
            property->write(stream, i);
            if (j +1 < mPropertiesOrder.size()) *stream << " ";
        }

        *stream << "\n";
    }

    *stream << std::endl;
}

void Ply::writeBinaryBody()
{
    for (size_t i = 0; i < mSize; ++i) {
        for (auto &property_name : mPropertiesOrder) {
            auto property = mProperties[property_name];
            property->write(stream, i, mIsLittleEndian);
        }
    }
}

auto Ply::findType(const std::string& type) const -> PlyProperty::Type
{
    PlyProperty::Type _type = PlyProperty::Type::ply_unknown;

    auto string_type = mStringTypes.find(type);
    if (string_type != mStringTypes.end()) {
        _type = string_type->second;
    }

    return _type;
}

auto Ply::findStringType(PlyProperty::Type type) const -> std::string
{
    std::string string_type = "unknown";

    for (auto &_type : mStringTypes) {
        if (_type.second == type) {
            string_type = _type.first;
            break;
        }
    }

    return string_type;
}
auto Ply::createProperty(const std::string& name, PlyProperty::Type type) -> std::shared_ptr<PlyProperty>
{
    std::shared_ptr<PlyProperty> ply_property;

    switch (type) {
    case PlyProperty::ply_unknown:
        //TODO: Devolver excepción
        break;
    case PlyProperty::ply_char:
        ply_property = std::make_shared<PlyPropertyImp<int8_t>>(name, type);
        break;
    case PlyProperty::ply_uchar:
        ply_property = std::make_shared<PlyPropertyImp<uint8_t>>(name, type);
        break;
    case PlyProperty::ply_short:
        ply_property = std::make_shared<PlyPropertyImp<int16_t>>(name, type);
        break;
    case PlyProperty::ply_ushort:
        ply_property = std::make_shared<PlyPropertyImp<uint16_t>>(name, type);
        break;
    case PlyProperty::ply_int:
        ply_property = std::make_shared<PlyPropertyImp<int32_t>>(name, type);
        break;
    case PlyProperty::ply_uint:
        ply_property = std::make_shared<PlyPropertyImp<uint32_t>>(name, type);
        break;
    case PlyProperty::ply_float:
        ply_property = std::make_shared<PlyPropertyImp<float>>(name, type);
        break;
    case PlyProperty::ply_double:
        ply_property = std::make_shared<PlyPropertyImp<double>>(name, type);
        break;
    default:
        break;
    }

    return ply_property;
}

} // end namespace graphos
