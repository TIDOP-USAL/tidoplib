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

#include "tidop/pctools/io/Reader.h"
#include "tidop/core/base/Exception.h"
//#include "tidop/geotools/GeoTools.h"
//#include "tidop/geotools/CRSsTools.h"
#include "tidop/pctools/io/impl/PointCloudReader.h"
#include "tidop/pctools/io/impl/PointCloudReaderFactory.h"

//#include <fstream>
//#include <iostream>
//#include <algorithm>
//#include <string>
//
//
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


PointCloudReader::PointCloudReader()
  : mReader(nullptr)
{
}

PointCloudReader::PointCloudReader(tl::Path file)
{
	open(std::move(file));
}

PointCloudReader::~PointCloudReader()
{
    close();
}

void PointCloudReader::open(const tl::Path &file)
{
    try {
        mReader = PointCloudReaderFactory::create(file);
        if (mReader) {
            mReader->open();
        }
    } catch (...) {
        close();
    }
}

auto PointCloudReader::isOpen() const -> bool
{
    return mReader && mReader->isOpen();
}

void PointCloudReader::close()
{
    if (mReader) {
        mReader->close();
        mReader.reset();
    }
}

void PointCloudReader::copcDumpBoundingBoxToCsv(const std::string &fileName, std::string crsId)
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    mReader->copcDumpBoundingBoxToCsv(fileName, crsId);
}

void PointCloudReader::copcGetResolutionByLevel(std::map<int, double> &resolutionByLevel)
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    mReader->copcGetResolutionByLevel(resolutionByLevel);
}

void PointCloudReader::getBoundingBox(double &x_min, double &y_min, double &z_min,
                                      double &x_max, double &y_max, double &z_max, std::string crsId)
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    mReader->getBoundingBox(x_min, y_min, z_min, x_max, y_max, z_max, crsId);
}

auto PointCloudReader::getBoundingBox(std::string crsId) const -> BoundingBox<Point3d>
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->getBoundingBox(crsId);
}

auto PointCloudReader::getDimensionsNames() const -> std::vector<std::string>
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->getDimensionsNames();
}

bool PointCloudReader::getIsCopc() const
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->getIsCopc();
}

void PointCloudReader::getPoints(double &x_o, double &y_o, double &z_o,
                                 std::vector<std::vector<float>> &coordinates,
                                 std::vector<std::string> dimensionsNames,
                                 std::vector<std::vector<float>> &dimensionsValues,
                                 std::string crsId)
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    mReader->getPoints(x_o, y_o, z_o, coordinates, dimensionsNames, dimensionsValues, crsId);
}

void PointCloudReader::getPoints(double &x_o, double &y_o, double &z_o,
                                 std::vector<std::vector<float>> &coordinates,
                                 std::vector<std::string> dimensionsNames,
                                 std::vector<std::vector<float>> &dimensionsValues,
                                 double x_min, double y_min, double z_min,
                                 double x_max, double y_max, double z_max,
                                 std::string crsId)
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    mReader->getPoints(x_o, y_o, z_o, coordinates, dimensionsNames, dimensionsValues,
                       x_min, y_min, z_min, x_max, y_max, z_max, crsId);
}

void PointCloudReader::getPoints(double &x_o, double &y_o, double &z_o,
                                 std::vector<std::vector<float>> &coordinates,
                                 std::vector<std::string> dimensionsNames,
                                 std::vector<std::vector<float>> &dimensionsValues,
                                 double resolution,
                                 std::string crsId)
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    mReader->getPoints(x_o, y_o, z_o, coordinates, dimensionsNames, dimensionsValues, resolution, crsId);
}

void PointCloudReader::getPoints(double &x_o, double &y_o, double &z_o,
                                 std::vector<std::vector<float>> &coordinates,
                                 std::vector<std::string> dimensionsNames,
                                 std::vector<std::vector<float>> &dimensionsValues,
                                 double x_min, double y_min, double z_min,
                                 double x_max, double y_max, double z_max,
                                 double resolution,
                                 std::string crsId)
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    mReader->getPoints(x_o, y_o, z_o, coordinates, dimensionsNames, dimensionsValues,
                       x_min, y_min, z_min, x_max, y_max, z_max, resolution, crsId);
}

auto PointCloudReader::getOffset() const -> Point3<double>
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->getOffset();
}

auto PointCloudReader::getCoordinates(int index) const -> Point3<double>
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->getCoordinates(index);
}

auto PointCloudReader::getField(int index, const std::string &name) const -> double
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->getField(index, name);
}

auto PointCloudReader::hasColors() const -> bool
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->hasColors();
}

auto PointCloudReader::hasNormals() const -> bool
{
    TL_ASSERT(isOpen(), "PointCloudReader is not open");
    return mReader->hasNormals();
}

}
