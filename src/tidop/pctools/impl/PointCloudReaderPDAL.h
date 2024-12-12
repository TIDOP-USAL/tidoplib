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

#pragma once

#ifndef PCTOOLS_POINTCLOUDREADERCOPC_H
#define PCTOOLS_POINTCLOUDREADERCOPC_H

#include <map>
#include <string>
#include <memory>

#include "tidop/config.h"
#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"

#include <proj.h>

#include "tidop/geotools/GeoTools.h"
#include "./PointCloudReader.h"

#include <pdal/Dimension.hpp>

namespace copc
{
    class FileReader;
}

namespace pdal
{
    class LasReader;
}

//namespace pdal
//{
//    Dimension::id;
//    namespace Dimension
//    {
//        class Id;
//    }
//}

namespace tl
{
class PointCloudReaderPDAL final
    : public PointCloudReader
{
    GENERATE_UNIQUE_PTR(PointCloudReaderPDAL)
public:
    PointCloudReaderPDAL(tl::Path file);
    ~PointCloudReaderPDAL() override;
    auto isOpen() const -> bool override
    {
        return mPtrCopcFile != nullptr;
    }
    void close() override;
    void copcDumpBoundingBoxToCsv(std::string fileName, std::string crsId = "") override;
    void copcGetResolutionByLevel(std::map<int, double>& resolutionByLevel) override;
    void getBoundingBox(double& x_min, double& y_min, double& z_min,
        double& x_max, double& y_max, double& z_max, std::string crsId = "") override;
    BoundingBoxd getBoundingBox(std::string crsId="") override;
    void getDimensionsNames(std::vector<std::string>& values) override;
    bool getIsCopc() override;
    void getPoints(double& x_o, double& y_o, double& z_o,
        std::vector<std::vector<float> >& coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> >& dimensionsValues,
        std::string crsId = "") override;
    void getPoints(double& x_o, double& y_o, double& z_o,
        std::vector<std::vector<float> >& coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> >& dimensionsValues,
        double x_min, double y_min, double z_min,
        double x_max, double y_max, double z_max,
        std::string crsId = "") override;
    void getPoints(double& x_o, double& y_o, double& z_o,
        std::vector<std::vector<float> >& coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> >& dimensionsValues,
        double resolution,
        std::string crsId = "") override;
    void getPoints(double& x_o, double& y_o, double& z_o,
        std::vector<std::vector<float> >& coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> >& dimensionsValues,
        double x_min, double y_min, double z_min,
        double x_max, double y_max, double z_max,
        double resolution,
        std::string crsId = "") override;
    void open() override;
private:
    void initialize();
    std::string mCrsId;
    std::map<std::string, pdal::Dimension::Id> mDimensionIdByName;
    bool mIsCopc{ false };
    // COPC
    copc::FileReader* mPtrCopcFile = nullptr;
    std::map<int, double> mResolutionByLevel;
    std::map<int, std::vector<std::vector<int> > > mVoxelsByLevel;
    double mResolutionFirstLevel;
    double mResolutionLastLevel;
    // LAS/LAZ no COPC
    pdal::LasReader* mPtrLasReader = nullptr;
};
}
#endif