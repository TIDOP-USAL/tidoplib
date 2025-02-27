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
#include <iostream>
#include <iomanip>

#include "tidop/config.h"
#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/base/ptr.h"
#include "tidop/core/base/exception.h"

#include <proj.h>
#include <copc-lib/las/header.hpp>
#include <lazperf/readers.hpp>
#include <copc-lib/io/copc_reader.hpp>
#include <copc-lib/laz/decompressor.hpp>

#include <io/CopcReader.hpp>
#include <io/LasReader.hpp>
#include <io/LasHeader.hpp>
#include <io/CopcWriter.hpp>
#include <filters/CropFilter.hpp>
#include <filters/ReprojectionFilter.hpp>
#include <filters/SortFilter.hpp>
#include <pdal/SrsBounds.hpp>
#include <pdal/util/FileUtils.hpp>
#include <pdal/private/gdal/GDALUtils.hpp>

#include "tidop/geotools/GeoTools.h"
#include "tidop/geotools/CRSsTools.h"
#include "PointCloudReaderPDAL.h"

using namespace tl;

PointCloudReaderPDAL::PointCloudReaderPDAL(tl::Path file)
    : PointCloudReader(std::move(file)),
    mPtrCopcFile(nullptr),
    mPtrLasReader(nullptr)
{
    try {
        initialize();
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

PointCloudReaderPDAL::~PointCloudReaderPDAL()
{
    PointCloudReaderPDAL::close();
}
        
void PointCloudReaderPDAL::close()
{
    if (mPtrCopcFile) {
        delete(mPtrCopcFile);
        mPtrCopcFile = nullptr;
    }
    if (mPtrLasReader) {
        delete(mPtrLasReader);
        mPtrLasReader = nullptr;
    }
}

void PointCloudReaderPDAL::copcDumpBoundingBoxToCsv(std::string fileName, std::string crsId)
{
    try {
        if (mPtrCopcFile == nullptr
            && mPtrLasReader == nullptr)
        {
            TL_ASSERT(false, "Reader is NULL");
        }
        // only COPC
        if (mPtrCopcFile == nullptr)
            return;
        std::ofstream ofs;
        ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);
        if (!ofs.is_open())
        {
            TL_ASSERT(false, "Error opening file : {}",fileName);
        }
        ofs << "id;level;point_count;resolution;wkt" << std::endl;
        int number_of_nodes = 0;
        for (const auto& node : mPtrCopcFile->GetAllChildrenOfPage(copc::VoxelKey::RootKey()))
            //for (const auto &node : reader.GetAllNodes())
        {
            copc::Box nodeBox(node.key, mPtrCopcFile->CopcConfig().LasHeader());
            double x_min = nodeBox.x_min;
            double y_min = nodeBox.y_min;
            double z_min = nodeBox.z_min;
            double x_max = nodeBox.x_max;
            double y_max = nodeBox.y_max;
            double z_max = nodeBox.z_max;
            if (!crsId.empty()
                && crsId != mCrsId)
            {
                std::vector<std::vector<double> > points;
                std::vector<double> ptoSW_minZ = { x_min, y_min, z_min };
                std::vector<double> ptoNW_minZ = { x_min, y_max, z_min };
                std::vector<double> ptoNE_minZ = { x_max, y_max, z_min };
                std::vector<double> ptoSE_minZ = { x_max, y_min, z_min };
                std::vector<double> ptoSW_maxZ = { x_min, y_min, z_max };
                std::vector<double> ptoNW_maxZ = { x_min, y_max, z_max };
                std::vector<double> ptoNE_maxZ = { x_max, y_max, z_max };
                std::vector<double> ptoSE_maxZ = { x_max, y_min, z_max };
                points.push_back(ptoSW_minZ);
                points.push_back(ptoNW_minZ);
                points.push_back(ptoNE_minZ);
                points.push_back(ptoSE_minZ);
                points.push_back(ptoSW_maxZ);
                points.push_back(ptoNW_maxZ);
                points.push_back(ptoNE_maxZ);
                points.push_back(ptoSE_maxZ);
                mPtrGeoTools->ptrCRSsTools()->crsOperation(mCrsId, crsId, points, true);
                double new_x_min = 10000000000.;
                double new_y_min = 10000000000.;
                double new_z_min = 10000000000.;
                double new_x_max = -10000000000.;
                double new_y_max = -10000000000.;
                double new_z_max = -10000000000.;
                for (auto const& pto : points)
                {
                    if (pto[0] < new_x_min) new_x_min = pto[0];
                    if (pto[0] > new_x_max) new_x_max = pto[0];
                    if (pto[1] < new_y_min) new_y_min = pto[1];
                    if (pto[1] > new_y_max) new_y_max = pto[1];
                    if (pto[2] < new_z_min) new_z_min = pto[2];
                    if (pto[2] > new_z_max) new_z_max = pto[2];
                }
                x_min = new_x_min;
                y_min = new_y_min;
                z_min = new_z_min;
                x_max = new_x_max;
                y_max = new_y_max;
                z_max = new_z_max;
            }
            std::ostringstream streamObj_xmin;
            streamObj_xmin << std::fixed << std::showpoint << std::setprecision(3) << x_min;
            std::ostringstream streamObj_xmax;
            streamObj_xmax << std::fixed << std::showpoint << std::setprecision(3) << x_max;
            std::ostringstream streamObj_ymin;
            streamObj_ymin << std::fixed << std::showpoint << std::setprecision(3) << y_min;
            std::ostringstream streamObj_ymax;
            streamObj_ymax << std::fixed << std::showpoint << std::setprecision(3) << y_max;

            std::string wkt = "POLYGON((";
            wkt += streamObj_xmin.str();
            wkt += " ";
            wkt += streamObj_ymin.str();
            wkt += ",";
            wkt += streamObj_xmin.str();
            wkt += " ";
            wkt += streamObj_ymax.str();
            wkt += ",";
            wkt += streamObj_xmax.str();
            wkt += " ";
            wkt += streamObj_ymax.str();
            wkt += ",";
            wkt += streamObj_xmax.str();
            wkt += " ";
            wkt += streamObj_ymin.str();
            wkt += ",";
            wkt += streamObj_xmin.str();
            wkt += " ";
            wkt += streamObj_ymin.str();
            wkt += "))";

            ofs << node.key.d << "-" << node.key.x << "-" << node.key.y << "-" << node.key.z << ";";
            ofs << node.key.d << ";";
            ofs << node.point_count << ";";
            double nodeResolution = node.key.Resolution(mPtrCopcFile->CopcConfig().LasHeader(), 
                mPtrCopcFile->CopcConfig().CopcInfo());
            ofs << std::fixed << std::setprecision(3) << nodeResolution << ";";
            ofs << wkt << std::endl;

            // In this example, we'll only save up to depth level 3.
            //if (node.key.d > 3)
            //    continue;
            //cout << node.key << ", resolution: " << node.key.Resolution(las_header, copc_info) << endl;
            //las::Points node_points = reader.GetPoints(node);
            //cout << node_points.ToString() << endl;
            //cout << endl << "First 5 points: " << endl;
            //for (int i = 0; i < 5; i++)
            //{
            //    cout << node_points.Get(i)->ToString() << endl;
            //}
            number_of_nodes++;
        }
        ofs.close();
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

void PointCloudReaderPDAL::copcGetResolutionByLevel(std::map<int, double>& resolutionByLevel)
{
    resolutionByLevel.clear();
    if (mPtrCopcFile == nullptr
        && mPtrLasReader == nullptr)
    {
        TL_ASSERT(false, "Reader is NULL");
    }
    // only COPC
    if (mPtrCopcFile == nullptr)
        return;
    for (auto value : mResolutionByLevel)
        resolutionByLevel[value.first] = value.second;
}

void PointCloudReaderPDAL::getBoundingBox(double& x_min, double& y_min, double& z_min,
    double& x_max, double& y_max, double& z_max, std::string crsId)
{
    if (mPtrCopcFile == nullptr
        && mPtrLasReader == nullptr)
    {
        TL_ASSERT(false, "Reader is NULL");
    }
    try {
        if (mPtrCopcFile)
        {
            auto las_header = mPtrCopcFile->CopcConfig().LasHeader();
            copc::Box box = las_header.Bounds();
            x_min = box.x_min;
            y_min = box.y_min;
            z_min = box.z_min;
            x_max = box.x_max;
            y_max = box.y_max;
            z_max = box.z_max;
        }
        else if (mPtrLasReader)
        {
            const pdal::LasHeader& h = mPtrLasReader->header();
            x_min = h.minX();
            y_min = h.minY();
            z_min = h.minZ();
            x_max = h.maxX();
            y_max = h.maxY();
            z_max = h.maxZ();
        }
        if (!crsId.empty()
            && crsId != mCrsId)
        {
            std::vector<std::vector<double> > points;
            std::vector<double> ptoSW_minZ = { x_min, y_min, z_min };
            std::vector<double> ptoNW_minZ = { x_min, y_max, z_min };
            std::vector<double> ptoNE_minZ = { x_max, y_max, z_min };
            std::vector<double> ptoSE_minZ = { x_max, y_min, z_min };
            std::vector<double> ptoSW_maxZ = { x_min, y_min, z_max };
            std::vector<double> ptoNW_maxZ = { x_min, y_max, z_max };
            std::vector<double> ptoNE_maxZ = { x_max, y_max, z_max };
            std::vector<double> ptoSE_maxZ = { x_max, y_min, z_max };
            points.push_back(ptoSW_minZ);
            points.push_back(ptoNW_minZ);
            points.push_back(ptoNE_minZ);
            points.push_back(ptoSE_minZ);
            points.push_back(ptoSW_maxZ);
            points.push_back(ptoNW_maxZ);
            points.push_back(ptoNE_maxZ);
            points.push_back(ptoSE_maxZ);
            mPtrGeoTools->ptrCRSsTools()->crsOperation(mCrsId, crsId, points, true);
            double new_x_min = 10000000000.;
            double new_y_min = 10000000000.;
            double new_z_min = 10000000000.;
            double new_x_max = -10000000000.;
            double new_y_max = -10000000000.;
            double new_z_max = -10000000000.;
            for (auto const& pto : points)
            {
                if (pto[0] < new_x_min) new_x_min = pto[0];
                if (pto[0] > new_x_max) new_x_max = pto[0];
                if (pto[1] < new_y_min) new_y_min = pto[1];
                if (pto[1] > new_y_max) new_y_max = pto[1];
                if (pto[2] < new_z_min) new_z_min = pto[2];
                if (pto[2] > new_z_max) new_z_max = pto[2];
            }
            x_min = new_x_min;
            y_min = new_y_min;
            z_min = new_z_min;
            x_max = new_x_max;
            y_max = new_y_max;
            z_max = new_z_max;
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

BoundingBoxd tl::PointCloudReaderPDAL::getBoundingBox(std::string crsId)
{
    BoundingBoxd bounding_box;

    try {

        TL_ASSERT(mPtrCopcFile || mPtrLasReader, "Reader is NULL");

        if (mPtrCopcFile)
        {
            auto las_header = mPtrCopcFile->CopcConfig().LasHeader();
            copc::Box box = las_header.Bounds();
            bounding_box.pt1.x = box.x_min;
            bounding_box.pt1.y = box.y_min;
            bounding_box.pt1.z = box.z_min;
            bounding_box.pt2.x = box.x_max;
            bounding_box.pt2.y = box.y_max;
            bounding_box.pt2.z = box.z_max;
        }
        else if (mPtrLasReader)
        {
            const pdal::LasHeader& h = mPtrLasReader->header();
            bounding_box.pt1.x = h.minX();
            bounding_box.pt1.y = h.minY();
            bounding_box.pt1.z = h.minZ();
            bounding_box.pt2.x = h.maxX();
            bounding_box.pt2.y = h.maxY();
            bounding_box.pt2.z = h.maxZ();
        }

        if (!crsId.empty()
            && crsId != mCrsId)
        {
            auto vertices = bounding_box.vertices();
            mPtrGeoTools->ptrCRSsTools()->crsOperation(mCrsId, crsId, vertices, true);
            bounding_box = BoundingBoxd(vertices);
        }
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return bounding_box;
}

void PointCloudReaderPDAL::getDimensionsNames(std::vector<std::string>& values)
{
    if (mPtrCopcFile == nullptr
        && mPtrLasReader == nullptr)
    {
        TL_ASSERT(false, "Reader is NULL");
    }
    values.clear();
    for (auto value : mDimensionIdByName)
    {
        values.push_back(value.first);
    }
}

bool PointCloudReaderPDAL::getIsCopc()
{
    if (mPtrCopcFile == nullptr
        && mPtrLasReader == nullptr)
    {
        TL_ASSERT(false, "Reader is NULL");
    }
    return(mIsCopc);
}

void PointCloudReaderPDAL::getPoints(double& x_o, double& y_o, double& z_o, 
    std::vector<std::vector<float> >& coordinates,
    std::vector<std::string> dimensionsNames,
    std::vector<std::vector<float> >& dimensionsValues,
    std::string crsId)
{
    double resolution = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double x_min = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double y_min = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double z_min = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double x_max = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double y_max = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double z_max = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    return(getPoints(x_o, y_o, z_o, coordinates, dimensionsNames, dimensionsValues,
        x_min, y_min, z_min, x_max, y_max, z_max, resolution, crsId));
}

void PointCloudReaderPDAL::getPoints(double& x_o, double& y_o, double& z_o, 
    std::vector<std::vector<float> >& coordinates,
    std::vector<std::string> dimensionsNames,
    std::vector<std::vector<float> >& dimensionsValues,
    double x_min, double y_min, double z_min,
    double x_max, double y_max, double z_max,
    std::string crsId)
{
    double resolution = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    return(getPoints(x_o, y_o, z_o, coordinates, dimensionsNames, dimensionsValues,
        x_min, y_min, z_min, x_max, y_max, z_max, resolution, crsId));
}

void PointCloudReaderPDAL::getPoints(double& x_o, double& y_o, double& z_o, 
    std::vector<std::vector<float> >& coordinates,
    std::vector<std::string> dimensionsNames,
    std::vector<std::vector<float> >& dimensionsValues,
    double resolution,
    std::string crsId)
{
    double x_min = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double y_min = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double z_min = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double x_max = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double y_max = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    double z_max = PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE;
    return(getPoints(x_o, y_o, z_o, coordinates, dimensionsNames, dimensionsValues,
        x_min, y_min, z_min, x_max, y_max, z_max, resolution, crsId));
}

void PointCloudReaderPDAL::getPoints(double& x_o, double& y_o, double& z_o, 
    std::vector<std::vector<float> >& coordinates,
    std::vector<std::string> dimensionsNames,
    std::vector<std::vector<float> >& dimensionsValues,
    double x_min, double y_min, double z_min,
    double x_max, double y_max, double z_max,
    double resolution,
    std::string crsId)
{
    coordinates.clear();
    dimensionsValues.clear();
    if (mPtrCopcFile == nullptr
        && mPtrLasReader == nullptr)
    {
        TL_ASSERT(false, "Reader is NULL");
    }
    std::vector<pdal::Dimension::Id> dimensionsIds;
    for (auto dimensionName : dimensionsNames)
    {
        if (compareInsensitiveCase(dimensionName, "X")
            || compareInsensitiveCase(dimensionName, "Y")
            || compareInsensitiveCase(dimensionName, "Z"))
            continue;
        if (mDimensionIdByName.find(dimensionName) == mDimensionIdByName.end())
        {
            TL_ASSERT(false, "Not found dimension: {}", dimensionName);
        }
        dimensionsIds.push_back(mDimensionIdByName[dimensionName]);
    }
    if (fabs(resolution - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) > 0.001
        && mPtrCopcFile == nullptr)
    {
        TL_ASSERT(false, "Get points using resolution is only valid for COPC format");
    }
    try
    {
        if (fabs(x_min - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) > 0.001
            && fabs(y_min - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) > 0.001
            && fabs(z_min - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) > 0.001
            && fabs(x_max - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) > 0.001
            && fabs(y_max - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) > 0.001
            && fabs(z_max - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) > 0.001)
        {
            if (!crsId.empty()
                && crsId != mCrsId)
            {
                std::vector<std::vector<double> > pointsBounds;
                std::vector<double> ptoSW_minZ = { x_min, y_min, z_min };
                std::vector<double> ptoNW_minZ = { x_min, y_max, z_min };
                std::vector<double> ptoNE_minZ = { x_max, y_max, z_min };
                std::vector<double> ptoSE_minZ = { x_max, y_min, z_min };
                std::vector<double> ptoSW_maxZ = { x_min, y_min, z_max };
                std::vector<double> ptoNW_maxZ = { x_min, y_max, z_max };
                std::vector<double> ptoNE_maxZ = { x_max, y_max, z_max };
                std::vector<double> ptoSE_maxZ = { x_max, y_min, z_max };
                pointsBounds.push_back(ptoSW_minZ);
                pointsBounds.push_back(ptoNW_minZ);
                pointsBounds.push_back(ptoNE_minZ);
                pointsBounds.push_back(ptoSE_minZ);
                pointsBounds.push_back(ptoSW_maxZ);
                pointsBounds.push_back(ptoNW_maxZ);
                pointsBounds.push_back(ptoNE_maxZ);
                pointsBounds.push_back(ptoSE_maxZ);
                mPtrGeoTools->ptrCRSsTools()->crsOperation(crsId, mCrsId, pointsBounds, true);
                double new_x_min = 10000000000.;
                double new_y_min = 10000000000.;
                double new_z_min = 10000000000.;
                double new_x_max = -10000000000.;
                double new_y_max = -10000000000.;
                double new_z_max = -10000000000.;
                for (auto const& pto : pointsBounds)
                {
                    if (pto[0] < new_x_min) new_x_min = pto[0];
                    if (pto[0] > new_x_max) new_x_max = pto[0];
                    if (pto[1] < new_y_min) new_y_min = pto[1];
                    if (pto[1] > new_y_max) new_y_max = pto[1];
                    if (pto[2] < new_z_min) new_z_min = pto[2];
                    if (pto[2] > new_z_max) new_z_max = pto[2];
                }
                x_min = new_x_min;
                y_min = new_y_min;
                z_min = new_z_min;
                x_max = new_x_max;
                y_max = new_y_max;
                z_max = new_z_max;
            }
        }
        else
        {
            getBoundingBox(x_min, y_min, z_min, x_max, y_max, z_max);
        }
        x_o = x_min;
        y_o = y_min;
        z_o = z_min;
        if (!crsId.empty()
            && crsId != mCrsId)
        {
            mPtrGeoTools->ptrCRSsTools()->crsOperation(mCrsId, crsId, x_o, y_o, z_o);
        }
        pdal::BOX3D bounds(x_min, y_min, z_min, x_max, y_max, z_max);
        pdal::PointViewSet set;
        pdal::PointTable pointTable;
        if (mPtrLasReader)
        {
            pdal::LasReader reader;
            {
                pdal::Options options;
                options.add("filename", file().toString());
                //options.add("bounds", bounds); // invalid no copc
                reader.setOptions(options);
            }
            reader.prepare(pointTable);
            //const auto set(reader.execute(pointTable));
            set=pdal::PointViewSet(reader.execute(pointTable));
        }
        else if (mPtrCopcFile)
        {
            if (fabs(resolution - PCTOOLS_DEFINITIONS_NO_DOUBLE_VALUE) < 0.001)
                resolution = mResolutionLastLevel;
            if(resolution > mResolutionFirstLevel)
                resolution = mResolutionFirstLevel + 0.01;
            else if (resolution < mResolutionLastLevel)
                resolution = mResolutionLastLevel - 0.01;
            else if (fabs(resolution - mResolutionLastLevel) < 0.001)
                resolution = mResolutionLastLevel - 0.01;
            else if (fabs(resolution - mResolutionFirstLevel) < 0.001)
                resolution = mResolutionFirstLevel + 0.01;
            pdal::CopcReader reader;
            {
                pdal::Options options;
                options.add("filename", file().toString());
                options.add("bounds", bounds);
                options.add("resolution", resolution);
                reader.setOptions(options);
            }
            reader.prepare(pointTable);
            //const auto set(reader.execute(pointTable));
            set = pdal::PointViewSet(reader.execute(pointTable));
        }
        uint64_t np(0);
        for (const pdal::PointViewPtr& view : set)
        {
            for (pdal::point_count_t i(0); i < view->size(); ++i)
            {
                ++np;
                uint64_t o = view->getFieldAs<uint64_t>(pdal::Dimension::Id::OriginId, i);
                double x = view->getFieldAs<double>(pdal::Dimension::Id::X, i);
                double y = view->getFieldAs<double>(pdal::Dimension::Id::Y, i);
                double z = view->getFieldAs<double>(pdal::Dimension::Id::Z, i);
                if (mPtrLasReader
                    && !bounds.contains(x, y, z))
                    continue;
                std::vector<float> values;
                for (auto dimensionId : dimensionsIds)
                {
                    double value = view->getFieldAs<double>(dimensionId, i);
                    values.push_back((float)value);
                }
                if (!crsId.empty()
                    && crsId != mCrsId)
                {
                    mPtrGeoTools->ptrCRSsTools()->crsOperation(mCrsId, crsId,x, y, z);
                }
                float x_f = x - x_o;
                float y_f = y - y_o;
                float z_f = z - z_o;
                std::vector<float> ptoCoordinates{x_f,y_f,z_f};
                coordinates.push_back(ptoCoordinates);
                dimensionsValues.push_back(values);
            }
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

void PointCloudReaderPDAL::initialize()
{
    try {
        mPtrGeoTools = GeoTools::getInstance();
        if (mPtrGeoTools->ptrCRSsTools() == NULL)
            TL_ASSERT(false, "CRSsTools is not initialized");
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

void PointCloudReaderPDAL::open()
{
    mResolutionByLevel.clear();
    mVoxelsByLevel.clear();
    mDimensionIdByName.clear();
    std::string extension = file().extension().toString();
    std::string crsWKT;
    pdal::StringList dimensionsName;
    pdal::PointTable pointTable;
    std::string crsId;
    if (compareInsensitiveCase(extension, ".las")
        || compareInsensitiveCase(extension, ".laz"))
    {
        try
        {
            copc::FileReader copcFile(file().toString());
            mIsCopc = true;
            mPtrCopcFile = new copc::FileReader(file().toString());
            auto las_header = mPtrCopcFile->CopcConfig().LasHeader();
            auto copc_info = mPtrCopcFile->CopcConfig().CopcInfo();
            crsWKT = mPtrCopcFile->CopcConfig().Wkt();
            pdal::CopcReader copcReader;
            pdal::Options options;
            options.add("filename", file().toString());
            copcReader.setOptions(options);
            copcReader.prepare(pointTable);
            pdal::SpatialReference crs = copcReader.getSpatialReference();
            std::string horizontalCrsEPSG = crs.identifyHorizontalEPSG();
            std::string verticalCrsEPSG = crs.identifyVerticalEPSG();
            if (!horizontalCrsEPSG.empty() || !verticalCrsEPSG.empty())
            {
                crsId = "EPSG:";
                if (!horizontalCrsEPSG.empty())
                {
                    crsId += horizontalCrsEPSG;
                    if (!verticalCrsEPSG.empty())
                        crsId += "+";
                }
                if (!verticalCrsEPSG.empty())
                    crsId += verticalCrsEPSG;
            }
            crsWKT = crs.getWKT1();
            mResolutionLastLevel = 100000000.;
            mResolutionFirstLevel = -100000000;
            for (const auto& node : mPtrCopcFile->GetAllChildrenOfPage(copc::VoxelKey::RootKey()))
                //for (const auto &node : reader.GetAllNodes())
            {
                copc::Box nodeBox(node.key, mPtrCopcFile->CopcConfig().LasHeader());
                std::string nodeId;
                std::vector<int> nodeKeys{ node.key.d, node.key.x, node.key.y, node.key.z };
                if (mVoxelsByLevel.find(node.key.d) == mVoxelsByLevel.end())
                {
                    std::vector<std::vector<int> > aux;
                    mVoxelsByLevel[node.key.d] = aux;
                }
                mVoxelsByLevel[node.key.d].push_back(nodeKeys);
                if (mResolutionByLevel.find(node.key.d) == mResolutionByLevel.end())
                {
                    double nodeResolution = node.key.Resolution(mPtrCopcFile->CopcConfig().LasHeader(),
                        mPtrCopcFile->CopcConfig().CopcInfo());
                    if (nodeResolution < mResolutionLastLevel)
                        mResolutionLastLevel = nodeResolution;
                    if (nodeResolution > mResolutionFirstLevel)
                        mResolutionFirstLevel = nodeResolution;
                    mResolutionByLevel[node.key.d] = nodeResolution;
                }
            }
            pdal::CopcReader reader;
            {
                pdal::Options options;
                options.add("filename", file().toString());
                //options.add("bounds", bounds);
                reader.setOptions(options);
            }
            reader.prepare(pointTable);
            const pdal::QuickInfo qi(reader.preview());
            dimensionsName=pdal::StringList(qi.m_dimNames.begin(), qi.m_dimNames.end());
        }
        catch (...)
        {
            mIsCopc = false;
            mPtrLasReader = new pdal::LasReader;
            {
                pdal::Options options;
                options.add("filename", file().toString());
                //options.add("bounds", bounds);
                mPtrLasReader->setOptions(options);
            }
            mPtrLasReader->prepare(pointTable);
            const pdal::LasHeader& h = mPtrLasReader->header();
            const pdal::QuickInfo qi(mPtrLasReader->preview());
            //const pdal::BOX3D pointBounds = qi.m_bounds;
            //const pdal::point_count_t pointsCount = qi.m_pointCount;
            dimensionsName=pdal::StringList(qi.m_dimNames.begin(), qi.m_dimNames.end());
            pdal::SpatialReference crs = h.srs();
            std::string horizontalCrsEPSG = crs.identifyHorizontalEPSG();
            std::string verticalCrsEPSG = crs.identifyVerticalEPSG();
            if (!horizontalCrsEPSG.empty() || !verticalCrsEPSG.empty())
            {
                crsId = "EPSG:";
                if (!horizontalCrsEPSG.empty())
                {
                    crsId += horizontalCrsEPSG;
                    if (!verticalCrsEPSG.empty())
                        crsId += "+";
                }
                if (!verticalCrsEPSG.empty())
                    crsId += verticalCrsEPSG;
            }
            crsWKT = crs.getWKT1();
            //TL_THROW_EXCEPTION("Error while opening COPC file: {}", file.fileName().toString());
            //auto f_stream = new std::fstream;
            //f_stream->open(file.toString().c_str(), std::ios::in | std::ios::binary);
            //if (!f_stream->good())
            //    TL_THROW_EXCEPTION("Error while opening file: {}", file.fileName().toString());
            //std::unique_ptr<lazperf::reader::generic_file> reader_;
            //reader_ = std::make_unique<lazperf::reader::generic_file>(*f_stream);
            //const base_header& header = reader_->header();
        }
    }
    if (crsWKT.empty())
        TL_ASSERT(false, "Not exists CRS WKT in file: {}", file().toString());
    try {
        pdal::Dimension::IdList dimensionsIdInFile = pointTable.layout()->dims();
        pdal::PointTable pointTableNoPredefinedDimensions;
        pdal::PointLayoutPtr layoutNoPredefinidedDimensions(pointTableNoPredefinedDimensions.layout());
        for (auto dimensionName : dimensionsName)
        {
            pdal::Dimension::Id dimensionId = pdal::Dimension::id(dimensionName);
            if (dimensionId == pdal::Dimension::Id::Unknown)
            {
                dimensionId = layoutNoPredefinidedDimensions->assignDim(dimensionName,
                    pdal::Dimension::Type::Float);
            }
            std::string name = dimensionName;
            mDimensionIdByName[name] = dimensionId;
        }
        if (crsId.empty())
        {
            if (!crsWKT.empty())
            {
                mPtrGeoTools->ptrCRSsTools()->setCRSFromWkt(crsWKT, crsId);
            }
        }
        if (crsId.empty())
            TL_ASSERT(false, "Not exists CRS WKT in file: {}", file().toString());
        mCrsId = crsId;
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}

