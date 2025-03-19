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

#include "tidop/core/exception.h"
#include "tidop/core/utils.h"

#include "tidop/geotools/GeoTools.h"
#include "tidop/geotools/CRSsTools.h"

#include <proj.h>
#include <copc-lib/las/header.hpp>
#include <lazperf/readers.hpp>
#include <copc-lib/io/copc_reader.hpp>
#include <copc-lib/laz/decompressor.hpp>


#include <io/CopcReader.hpp>
#include <io/LasReader.hpp>
#include <io/LasHeader.hpp>
#include <io/CopcWriter.hpp>
#include <io/PlyReader.hpp>
#include <io/PlyWriter.hpp>
#include <io/BufferReader.hpp>
#include <filters/CropFilter.hpp>
#include <filters/ReprojectionFilter.hpp>
#include <filters/SortFilter.hpp>
#include <pdal/SrsBounds.hpp>
#include <pdal/util/FileUtils.hpp>
#include <pdal/private/gdal/GDALUtils.hpp>

#include <pdal/Stage.hpp>
#include <pdal/StageFactory.hpp>
#include <pdal/PipelineManager.hpp>
#include <pdal/util/FileUtils.hpp>

#include "PointCloudTools.h"

using namespace tl;


PointCloudTools::PointCloudTools()
{
    mPtrGeoTools = GeoTools::getInstance();
    try {
        bool ignoreDeprecatedCRSs = true;
        mPtrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
};

PointCloudTools::~PointCloudTools()
{
    clear();
}

bool PointCloudTools::assignCRS(std::string fileName, std::string crsId, 
    std::string outputFileName, bool ignoreInternalCrs)
{
    bool succesFull = false;
    if (mPtrGeoTools == nullptr)
    {
        TL_ASSERT(false, "GeoTools is not defined");
    }
    if (!mPtrGeoTools->ptrCRSsTools()->getIsCRSValid(crsId))
    {
        TL_ASSERT(false, "Invalid CRS Id: {}", crsId);
    }
    std::string pdalCrsId = mPtrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(crsId);
    if (pdalCrsId.empty()) // "EPSG:25830" -> "EPSG:25830+4258", emtpy when is not needed
        pdalCrsId = crsId;
    tl::Path inputFile(fileName);
    if (!inputFile.exists())
    {
        TL_ASSERT(false, "Not exists input file: {}", fileName);
    }
    if (!inputFile.isFile())
    {
        TL_ASSERT(false, "Input is not a file: {}", fileName);
    }
    std::string extension = inputFile.extension().toString();
    tl::Path outputFile = Path(outputFileName);
    if (outputFile.exists())
    {
        if (!outputFile.isFile())
        {
            TL_ASSERT(false, "Output is not a file: {}", outputFileName);
        }
    }
    std::string outputExtension = outputFile.extension().toString();
    if (!compareInsensitiveCase(extension, outputExtension))
    {
        TL_ASSERT(false, "Extensions are different for input and output files");
    }
    if (outputFile.exists())
    {
        tl::Path::removeFile(outputFile);
        if (outputFile.exists())
        {
            TL_ASSERT(false, "Error removing existing output file: {}", outputFileName);
        }
    }
    try
    {
        std::string inputFileCrsId = getCRSId(fileName);
        pdal::StringList dimensionsName;
        pdal::PointTable pointTable;
        pdal::PipelineManager pdalPipeline;
        pdal::Options optsReader;
        optsReader.add("filename", fileName);
        if (compareInsensitiveCase(extension, ".las")
            || compareInsensitiveCase(extension, ".laz"))
        {
            bool isCopc = false;
            try
            {
                {// for close
                    copc::FileReader copcFile(fileName); // error if file is not copc
                    isCopc = true;
                    auto las_header = copcFile.CopcConfig().LasHeader();
                    auto copc_info = copcFile.CopcConfig().CopcInfo();
                }
                pdal::Stage& reader = pdalPipeline.addReader("readers.copc");
                reader.setOptions(optsReader);
                if (inputFileCrsId.empty()
                    ||ignoreInternalCrs)
                {
                    pdal::Options optsWriter;
                    optsWriter.add("a_srs", crsId);
                    optsWriter.add("extra_dims", "all");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                }
                else
                {
                    std::string  pdalInpuFileCrsId = mPtrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(inputFileCrsId);
                    if (pdalInpuFileCrsId.empty()) // "EPSG:25830" -> "EPSG:25830+4258", emtpy when is not needed
                        pdalInpuFileCrsId = inputFileCrsId;
                    pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                    pdal::Options optsFilterCrsOperation;
                    optsFilterCrsOperation.add("in_srs", pdalInpuFileCrsId);
                    optsFilterCrsOperation.add("out_srs", pdalCrsId);// crsId);
                    filterCrsOperation.setOptions(optsFilterCrsOperation);
                    pdal::Options optsWriter;
                    optsWriter.add("a_srs", crsId);
                    optsWriter.add("extra_dims", "all");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", filterCrsOperation, optsWriter);
                }
            }
            catch (...)
            {
                {// for close
                    pdal::LasReader lasReader;
                    pdal::Options options;
                    options.add("filename", fileName);
                    lasReader.setOptions(options);
                    lasReader.prepare(pointTable);
                    const pdal::LasHeader& h = lasReader.header();
                    pdal::SpatialReference crs = h.srs();
                }
                pdal::Stage& reader = pdalPipeline.addReader("readers.las");
                reader.setOptions(optsReader);
                if (inputFileCrsId.empty())
                {
                    pdal::Options optsWriter;
                    optsWriter.add("a_srs", crsId);
                    optsWriter.add("extra_dims", "all");
                    optsWriter.add("minor_version", "4");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.las", reader, optsWriter);
                }
                else
                {
                    std::string  pdalInpuFileCrsId = mPtrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(inputFileCrsId);
                    if (pdalInpuFileCrsId.empty()) // "EPSG:25830" -> "EPSG:25830+4258", emtpy when is not needed
                        pdalInpuFileCrsId = inputFileCrsId;
                    pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                    pdal::Options optsFilterCrsOperation;
                    optsFilterCrsOperation.add("in_srs", pdalInpuFileCrsId);
                    optsFilterCrsOperation.add("out_srs", pdalCrsId);// crsId);
                    filterCrsOperation.setOptions(optsFilterCrsOperation);
                    pdal::Options optsWriter;
                    optsWriter.add("a_srs", crsId);
                    optsWriter.add("extra_dims", "all");
                    optsWriter.add("minor_version", "4");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.las", filterCrsOperation, optsWriter);
                }
            }
            pdal::point_count_t np = pdalPipeline.execute();
            succesFull = !std::ifstream(outputFileName).fail();
        }
        else
        {
            TL_ASSERT(false, "Input file must be LAS/LAZ");
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    };
    return(succesFull);
}

void PointCloudTools::clear()
{
    // if (mPtrPointCloudFileManager != NULL)
        // delete(mPtrPointCloudFileManager);
}

void PointCloudTools::getCOPCStruct(std::string fileName,
    std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& x_min_by_node_keys,
    std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& x_max_by_node_keys,
    std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& y_min_by_node_keys,
    std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& y_max_by_node_keys,
    std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& resolution_by_node_keys,
    std::map<int, std::map<int, std::map<int, std::map<int, int > > > >& number_of_points_by_node_keys)
{
    x_min_by_node_keys.clear();
    x_max_by_node_keys.clear();
    y_min_by_node_keys.clear();
    y_max_by_node_keys.clear();
    resolution_by_node_keys.clear();
    number_of_points_by_node_keys.clear();
    tl::Path inputFile(fileName);
    if (!inputFile.exists())
    {
        TL_ASSERT(false, "Not exists input file: {}", fileName);
    }
    if (!inputFile.isFile())
    {
        TL_ASSERT(false, "Input is not a file: {}", fileName);
    }
    std::string extension = inputFile.extension().toString();
    if (!compareInsensitiveCase(extension, ".las")
        && !compareInsensitiveCase(extension, ".laz"))
    {
        TL_ASSERT(false, "Input is not a LAS/LAZ file: {}", fileName);
    }
    bool isCopc = false;
    std::map<int, std::map<int, std::map<int, std::map<int, std::vector<double> > > > > node_keys;
    try
    {
        copc::FileReader reader(fileName); // error if file is not copc
        isCopc = true;
        auto las_header = reader.CopcConfig().LasHeader();
        auto copc_info = reader.CopcConfig().CopcInfo();
        //cout << "Copc Info: " << endl;
        //cout << "\tSpacing: " << copc_info.spacing << endl
        //     << "\tRoot Offset: " << copc_info.root_hier_offset << endl
        //     << "\tRoot Size: " << copc_info.root_hier_size << endl;
        // Get the CopcInfo struct
        auto copc_extents = reader.CopcConfig().CopcExtents();
        //cout << "Copc Extents (Min/Max): " << endl;
        //cout << "\tIntensity : (" << copc_extents.Intensity()->minimum << "/" << copc_extents.Intensity()->maximum << ")"
        //     << endl
        //     << "\tClassification : (" << copc_extents.Classification()->minimum << "/"
        //     << copc_extents.Classification()->maximum << ")" << endl
        //     << "\tGpsTime : (" << copc_extents.GpsTime()->minimum << "/" << copc_extents.GpsTime()->maximum << ")" << endl;
        int number_of_nodes = 0;
        for (const auto& node : reader.GetAllChildrenOfPage(copc::VoxelKey::RootKey()))
            //for (const auto &node : reader.GetAllNodes())
        {
            copc::Box nodeBox(node.key, las_header);
            double x_min = nodeBox.x_min;
            double x_max = nodeBox.x_max;
            double y_min = nodeBox.y_min;
            double y_max = nodeBox.y_max;
            int key_d = node.key.d;
            int key_x = node.key.x;
            int key_y = node.key.y;
            int key_z = node.key.z;
            int nop = node.point_count;
            std::vector<double> bbox;
            bbox.push_back(x_min);
            bbox.push_back(x_max);
            bbox.push_back(y_min);
            bbox.push_back(y_max);
            x_min_by_node_keys[key_d][key_x][key_y][key_z] = node.key.d;
            x_max_by_node_keys[key_d][key_x][key_y][key_z] = node.key.x;
            y_min_by_node_keys[key_d][key_x][key_y][key_z] = node.key.y;
            y_max_by_node_keys[key_d][key_x][key_y][key_z] = node.key.z;
            resolution_by_node_keys[key_d][key_x][key_y][key_z] = node.key.Resolution(las_header, copc_info);
            number_of_points_by_node_keys[key_d][key_x][key_y][key_z] = node.point_count;
        }
    }
    catch (...)
    {
        TL_ASSERT(false, "Input is not a COPC file: {}", fileName);
    }
}

std::string PointCloudTools::getCRSId(std::string fileName)
{
    bool succesFull = false;
    if (mPtrGeoTools == nullptr)
    {
        TL_ASSERT(false, "GeoTools is not defined");
    }
    std::string crsId;
    tl::Path inputFile(fileName);
    if (!inputFile.exists())
    {
        TL_ASSERT(false, "Not exists input file: {}", fileName);
    }
    if (!inputFile.isFile())
    {
        TL_ASSERT(false, "Input is not a file: {}", fileName);
    }
    std::string extension = inputFile.extension().toString();
    std::string crsWKT;
    pdal::StringList dimensionsName;
    pdal::PointTable pointTable;
    pdal::PipelineManager pdalPipeline;
    pdal::Options optsReader;
    optsReader.add("filename", fileName);
    if (compareInsensitiveCase(extension, ".las")
        || compareInsensitiveCase(extension, ".laz"))
    {
        bool isCopc = false;
        try
        {
            copc::FileReader copcFile(fileName); // error if file is not copc
            isCopc = true;
            auto las_header = copcFile.CopcConfig().LasHeader();
            auto copc_info = copcFile.CopcConfig().CopcInfo();
            crsWKT = copcFile.CopcConfig().Wkt();
            pdal::CopcReader copcReader;
            pdal::Options options;
            options.add("filename", fileName);
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
                    if(!verticalCrsEPSG.empty())
                        crsId += "+";
                }
                if (!verticalCrsEPSG.empty())
                    crsId += verticalCrsEPSG;
            }
            crsWKT = crs.getWKT1();
        }
        catch (...)
        {
            pdal::LasReader lasReader;
            pdal::Options options;
            options.add("filename", fileName);
            lasReader.setOptions(options);
            lasReader.prepare(pointTable);
            const pdal::LasHeader& h = lasReader.header();
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
        }
        if (crsId.empty())
        {
            if (!crsWKT.empty())
            {
                mPtrGeoTools->ptrCRSsTools()->setCRSFromWkt(crsWKT, crsId);
            }
        }
    }
    else
    {
        TL_ASSERT(false, "File format: {} is not supported", extension);
    }
    return(crsId);
}

void PointCloudTools::formatFileConversionToCOPC(std::string fileName, std::string outputFileName,
    std::string targetCrsId, std::string sourceCrsId)
{
    try {
        std::string inputSourceCrsId = sourceCrsId;
        std::string inputTargetCrsId = targetCrsId;
        std::vector<std::string> filesToRemove;

        TL_ASSERT(mPtrGeoTools, "GeoTools is not defined");

        tl::Path inputFile(fileName);
        TL_ASSERT(inputFile.exists(), "Not exists input file: {}", fileName);
        TL_ASSERT(inputFile.isFile(), "Input is not a file: {}", fileName);

        std::string extension = inputFile.extension().toString();
        if (!compareInsensitiveCase(extension, ".ply")
            && !compareInsensitiveCase(extension, ".laz")
            && !compareInsensitiveCase(extension, ".las"))
        {
            TL_ASSERT(false, "Invalid input file extension: {}", extension);
        }
        tl::Path outputFile(outputFileName);
        if (outputFile.exists())
        {
            TL_ASSERT(outputFile.isFile(), "Output is not a file: {}", outputFileName);
            Path::removeFile(outputFile);
            TL_ASSERT(!outputFile.exists(), "Error removing existing output file: {}", outputFileName);
        }
        std::string outputExtension = outputFile.extension().toString();
        if (outputFileName.find(".copc") == std::string::npos)
        {
            outputFile.replaceExtension(".copc.laz");
        }
        else
        {
            outputFile.replaceExtension(".laz");
        }
        outputFileName = outputFile.toString();
        // if ply and sourceCrsId =="" then CRSs operation is not possible
        if (compareInsensitiveCase(extension, ".ply")
            && sourceCrsId.empty())
        {
            targetCrsId = "";
        }
        // equal source and target CRSs -> no CRSsOperation, but assignation using inputSourceCrsId
        if (compareInsensitiveCase(sourceCrsId, targetCrsId))
        {
            sourceCrsId = "";
            targetCrsId = "";
        }
        // if targetCrsId == "" then CRSs operation is not possible, but assignation using inputSourceCrsId
        if (targetCrsId.empty())
        {
            sourceCrsId = "";
        }
        if (!targetCrsId.empty()) // enu output is invalid
        {

            TL_ASSERT(mPtrGeoTools->ptrCRSsTools()->getIsCRSValid(targetCrsId), "Invalid target CRS Id: {}", targetCrsId);
            TL_ASSERT(!mPtrGeoTools->ptrCRSsTools()->getIsCRSEnu(targetCrsId), "ENU CRS is not a valid target CRS: {}", targetCrsId);
        }

        //TL_ASSERT(mPtrGeoTools->ptrCRSsTools()->getIsCRSValid(sourceCrsId), "Invalid source CRS Id: {}", sourceCrsId);

        // sourceCRS is ENU and different to targetCRS then CRSOperation using CRSsTools
        if (mPtrGeoTools->ptrCRSsTools()->getIsCRSEnu(sourceCrsId)
            && !compareInsensitiveCase(sourceCrsId, targetCrsId))
        {
            TL_ASSERT(compareInsensitiveCase(extension, ".ply"), "ENU CRS: {} is only valid for PLY input files", sourceCrsId);
            Path temporalPlyPath(fileName);
            temporalPlyPath.replaceExtension("_temp.ply");
            if (Path::exists(temporalPlyPath))
            {
                Path::removeFile(temporalPlyPath);
                if (Path::exists(temporalPlyPath))
                {
                    TL_ASSERT(false, "Error removing existing file: {}", temporalPlyPath.toString());
                }
            }
            pdal::Options options;
            options.add("filename", fileName);
            pdal::PointTable table;
            pdal::PlyReader reader;
            reader.setOptions(options);
            reader.prepare(table);
            pdal::PointViewSet viewSet = reader.execute(table);
            pdal::PointViewSet::iterator pi;
            uint64_t np_total(0);
            pdal::BufferReader r;
            for (auto si = viewSet.begin(); si != viewSet.end(); ++si)
            {
                pdal::PointViewPtr view = *si;
                uint64_t np(0);
                for (pdal::point_count_t i(0); i < view->size(); ++i)
                {
                    ++np;
                    double x = view->getFieldAs<double>(pdal::Dimension::Id::X, i);
                    double y = view->getFieldAs<double>(pdal::Dimension::Id::Y, i);
                    double z = view->getFieldAs<double>(pdal::Dimension::Id::Z, i);
                    mPtrGeoTools->ptrCRSsTools()->crsOperation(sourceCrsId,
                        targetCrsId, x, y, z);
                    view->setField(pdal::Dimension::Id::X, i, x);
                    view->setField(pdal::Dimension::Id::Y, i, y);
                    view->setField(pdal::Dimension::Id::Z, i, z);
                }
                np_total += np;
                r.addView(view);
            }
            pdal::PlyWriter writer;
            pdal::Options writerOptions;
            writerOptions.add("filename", temporalPlyPath.toString());
            //writerOptions.add("storage_mode", "little endian");
            writerOptions.add("storage_mode", "ascii");
            int precision = 3;
            if (mPtrGeoTools->ptrCRSsTools()->getIsCRSGeographic(inputTargetCrsId))
                precision = 9;
            writerOptions.add("precision", precision);
            writer.setInput(r);
            writer.setOptions(writerOptions);
            writer.prepare(table);
            writer.execute(table);
            if (!Path::exists(temporalPlyPath))
            {
                TL_ASSERT(false, "Error in CRSs operation from: {} to {}",
                    sourceCrsId, targetCrsId);
            }
            fileName = temporalPlyPath.toString();
            inputSourceCrsId = inputTargetCrsId;
            filesToRemove.push_back(fileName);
            sourceCrsId = "";
            targetCrsId = "";
        }
        std::string pdalSourceCrsId, pdalTargetCrsId;
        if (!sourceCrsId.empty())
        {
            pdalSourceCrsId = mPtrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(sourceCrsId);
            if (pdalSourceCrsId.empty()) // "EPSG:25830" -> "EPSG:25830+4258", emtpy when is not needed
                pdalSourceCrsId = sourceCrsId;
        }
        if (!targetCrsId.empty())
        {
            pdalTargetCrsId = mPtrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(targetCrsId);
            if (pdalTargetCrsId.empty()) // "EPSG:25830" -> "EPSG:25830+4258", emtpy when is not needed
                pdalTargetCrsId = targetCrsId;
        }
        pdal::StringList dimensionsName;
        pdal::PointTable pointTable;
        pdal::PipelineManager pdalPipeline;
        pdal::Options optsReader;
        optsReader.add("filename", fileName);
        if (compareInsensitiveCase(extension, ".las")
            || compareInsensitiveCase(extension, ".laz"))
        {
            std::string inputFileCrsId = getCRSId(fileName);
            if (!inputFileCrsId.empty()
                &&!pdalTargetCrsId.empty())
            {
                // exists a method to assing CRS then internal CRS is imperative
                if (!compareInsensitiveCase(pdalSourceCrsId, inputFileCrsId))
                {
                    pdalSourceCrsId = mPtrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(inputFileCrsId);
                    if (pdalSourceCrsId.empty()) // "EPSG:25830" -> "EPSG:25830+4258", emtpy when is not needed
                        pdalSourceCrsId = inputFileCrsId;
                }
            }
            bool isCopc = false;
            try
            {
                {
                    copc::FileReader copcFile(fileName); // error if file is not copc
                    isCopc = true;
                    //auto las_header = copcFile.CopcConfig().LasHeader();
                    //auto copc_info = copcFile.CopcConfig().CopcInfo();
                    //crsWKT = copcFile.CopcConfig().Wkt();
                }
                pdal::Stage& reader = pdalPipeline.addReader("readers.copc");
                reader.setOptions(optsReader);
                if (!pdalSourceCrsId.empty()
                    && !compareInsensitiveCase(pdalSourceCrsId, pdalTargetCrsId))
                {
                    pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                    pdal::Options optsFilterCrsOperation;
                    optsFilterCrsOperation.add("in_srs", pdalSourceCrsId);
                    optsFilterCrsOperation.add("out_srs", pdalTargetCrsId);// crsId);
                    filterCrsOperation.setOptions(optsFilterCrsOperation);
                    pdal::Options optsWriter;
                    if (!pdalTargetCrsId.empty())
                        optsWriter.add("a_srs", inputTargetCrsId);
                        //optsWriter.add("a_srs", pdalTargetCrsId);
                    if (mPtrGeoTools->ptrCRSsTools()->getIsCRSGeographic(inputTargetCrsId))
                    {
                        optsWriter.add("scale_x","0.0000001");
                        optsWriter.add("scale_y", "0.0000001");
                        optsWriter.add("scale_z", "0.01");
                        optsWriter.add("offset_x", "auto");
                        optsWriter.add("offset_y", "auto");
                        optsWriter.add("offset_z", "auto");
                        //optsWriter.add("out_axis_ordering", "2,1");
                    }
                    optsWriter.add("extra_dims", "all");
                    //pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc",
                        filterCrsOperation, optsWriter);
                }
                else
                {
                    pdal::Options optsWriter;
                    if (inputFileCrsId.empty()
                        && !inputSourceCrsId.empty())
                    {
                        optsWriter.add("a_srs", inputSourceCrsId);
                    }
                    if (mPtrGeoTools->ptrCRSsTools()->getIsCRSGeographic(inputSourceCrsId))
                    {
                        optsWriter.add("scale_x", "0.0000001");
                        optsWriter.add("scale_y", "0.0000001");
                        optsWriter.add("scale_z", "0.01");
                        optsWriter.add("offset_x", "auto");
                        optsWriter.add("offset_y", "auto");
                        optsWriter.add("offset_z", "auto");
                        //optsWriter.add("out_axis_ordering", "2,1");
                    }
                    optsWriter.add("extra_dims", "all");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                }
            }
            catch (...)
            {
                pdal::Stage& reader = pdalPipeline.addReader("readers.las");
                reader.setOptions(optsReader);
                if (!pdalSourceCrsId.empty()
                    && !compareInsensitiveCase(pdalSourceCrsId, pdalTargetCrsId))
                {
                    pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                    pdal::Options optsFilterCrsOperation;
                    optsFilterCrsOperation.add("in_srs", pdalSourceCrsId);
                    optsFilterCrsOperation.add("out_srs", pdalTargetCrsId);// crsId);
                    filterCrsOperation.setOptions(optsFilterCrsOperation);
                    pdal::Options optsWriter;
                    if (!pdalTargetCrsId.empty())
                        optsWriter.add("a_srs", inputTargetCrsId);
                        //optsWriter.add("a_srs", pdalTargetCrsId);
                    if (mPtrGeoTools->ptrCRSsTools()->getIsCRSGeographic(inputTargetCrsId))
                    {
                        optsWriter.add("scale_x", "0.0000001");
                        optsWriter.add("scale_y", "0.0000001");
                        optsWriter.add("scale_z", "0.01");
                        optsWriter.add("offset_x", "auto");
                        optsWriter.add("offset_y", "auto");
                        optsWriter.add("offset_z", "auto");
                        //optsWriter.add("out_axis_ordering", "2,1");
                    }
                    optsWriter.add("extra_dims", "all");
                    //pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", 
                        filterCrsOperation, optsWriter);
                }
                else
                {
                    pdal::Options optsWriter;
                    if (inputFileCrsId.empty()
                        && !inputSourceCrsId.empty())
                    {
                        optsWriter.add("a_srs", inputSourceCrsId);
                    }
                    if (mPtrGeoTools->ptrCRSsTools()->getIsCRSGeographic(inputSourceCrsId))
                    {
                        optsWriter.add("scale_x", "0.0000001");
                        optsWriter.add("scale_y", "0.0000001");
                        optsWriter.add("scale_z", "0.01");
                        optsWriter.add("offset_x", "auto");
                        optsWriter.add("offset_y", "auto");
                        optsWriter.add("offset_z", "auto");
                        //optsWriter.add("out_axis_ordering", "2,1");
                    }
                    optsWriter.add("extra_dims", "all");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                }
            }
        }
        else if (compareInsensitiveCase(extension, ".ply"))
        {
            pdal::Stage& reader = pdalPipeline.addReader("readers.ply");
            reader.setOptions(optsReader);
            if (!pdalSourceCrsId.empty()
                && !compareInsensitiveCase(pdalSourceCrsId, pdalTargetCrsId))
            {
                pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                pdal::Options optsFilterCrsOperation;
                optsFilterCrsOperation.add("in_srs", pdalSourceCrsId);
                optsFilterCrsOperation.add("out_srs", pdalTargetCrsId);// crsId);
                filterCrsOperation.setOptions(optsFilterCrsOperation);
                pdal::Options optsWriter;
                if (!inputTargetCrsId.empty())
                {
                    optsWriter.add("a_srs", inputTargetCrsId);
                }
                if (mPtrGeoTools->ptrCRSsTools()->getIsCRSGeographic(inputTargetCrsId))
                {
                    optsWriter.add("scale_x", "0.0000001");
                    optsWriter.add("scale_y", "0.0000001");
                    optsWriter.add("scale_z", "0.01");
                    optsWriter.add("offset_x", "auto");
                    optsWriter.add("offset_y", "auto");
                    optsWriter.add("offset_z", "auto");
                    //optsWriter.add("out_axis_ordering", "2,1");
                }
                optsWriter.add("extra_dims", "all");
                //pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", filterCrsOperation, optsWriter);
            }
            else
            {
                pdal::Options optsWriter;
                if (!inputSourceCrsId.empty())
                {
                    optsWriter.add("a_srs", inputSourceCrsId);
                }
                optsWriter.add("extra_dims", "all");
                if (mPtrGeoTools->ptrCRSsTools()->getIsCRSGeographic(inputSourceCrsId))
                {
                    optsWriter.add("scale_x", "0.0000001");
                    optsWriter.add("scale_y", "0.0000001");
                    optsWriter.add("scale_z", "0.01");
                    optsWriter.add("offset_x", "auto");
                    optsWriter.add("offset_y", "auto");
                    optsWriter.add("offset_z", "auto");
                    //optsWriter.add("out_axis_ordering", "2,1");
                }
                pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
            }
        }
        else
        {
            TL_ASSERT(false, "File format: {} is not supported", extension);
        }
        pdal::point_count_t np = pdalPipeline.execute();
        bool succesFull = !std::ifstream(outputFileName).fail();
        for (auto fileNameToRemove : filesToRemove)
        {
            tl::Path fileToRemove = Path(fileNameToRemove);
            if (fileToRemove.exists())
            {
                Path::removeFile(fileToRemove);
                if (fileToRemove.exists())
                {
                    TL_ASSERT(false, "Error removing temporal file: {}", fileNameToRemove);
                }
            }
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
}
/*
bool PointCloudTools::formatFileConversionToCOPC(std::string fileName, std::string outputFileName, std::string crsId)
{
    bool succesFull = false;
    if (mPtrGeoTools == nullptr)
    {
        TL_ASSERT(false, "GeoTools is not defined");
    }
    std::string pdalCrsId;
    if (!crsId.empty())
    {
        if (!mPtrGeoTools->ptrCRSsTools()->getIsCRSValid(crsId))
        {
            TL_ASSERT(false, "Invalid CRS Id: {}", crsId);
        }
        pdalCrsId = mPtrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(crsId);
        if (pdalCrsId.empty()) // "EPSG:25830" -> "EPSG:25830+4258", emtpy when is not needed
            pdalCrsId = crsId;
    }
    tl::Path inputFile(fileName);
    if (!inputFile.exists())
    {
        TL_ASSERT(false, "Not exists input file: {}", fileName);
    }
    if (!inputFile.isFile())
    {
        TL_ASSERT(false, "Input is not a file: {}", fileName);
    }
    std::string extension = inputFile.extension().toString();
    tl::Path outputFile = Path(outputFileName);
    if (outputFile.exists())
    {
        if (!outputFile.isFile())
        {
            TL_ASSERT(false, "Output is not a file: {}", outputFileName);
        }
    }
    std::string outputExtension = outputFile.extension().toString();
    if (!compareInsensitiveCase(outputExtension, ".laz"))
    {
        TL_ASSERT(false, "Output file extension for COPC format must be .laz");
    }
    if (outputFile.exists())
    {
        tl::Path::removeFile(outputFile);
        if (outputFile.exists())
        {
            TL_ASSERT(false, "Error removing existing output file: {}", outputFileName);
        }
    }
    try
    {
        std::string crsWKT;
        pdal::StringList dimensionsName;
        pdal::PointTable pointTable;
        pdal::PipelineManager pdalPipeline;
        pdal::Options optsReader;
        optsReader.add("filename", fileName);
        if (compareInsensitiveCase(extension, ".las")
            || compareInsensitiveCase(extension, ".laz"))
        {
            bool isCopc = false;
            try
            {
                {
                    copc::FileReader copcFile(fileName); // error if file is not copc
                    isCopc = true;
                    auto las_header = copcFile.CopcConfig().LasHeader();
                    auto copc_info = copcFile.CopcConfig().CopcInfo();
                    crsWKT = copcFile.CopcConfig().Wkt();
                }
                pdal::Stage& reader = pdalPipeline.addReader("readers.copc");
                reader.setOptions(optsReader);
                if (crsWKT.empty())
                {
                    pdal::Options optsWriter;
                    if(!crsId.empty())
                        optsWriter.add("a_srs", crsId);
                    optsWriter.add("extra_dims", "all");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                }
                else
                {
                    if (!crsId.empty())
                    {
                        std::string inputFileCrsId;
                        mPtrGeoTools->ptrCRSsTools()->setCRSFromWkt(crsWKT, inputFileCrsId);
                        pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                        pdal::Options optsFilterCrsOperation;
                        optsFilterCrsOperation.add("in_srs", inputFileCrsId);
                        optsFilterCrsOperation.add("out_srs", pdalCrsId);// crsId);
                        filterCrsOperation.setOptions(optsFilterCrsOperation);
                        pdal::Options optsWriter;
                        optsWriter.add("a_srs", crsId);
                        optsWriter.add("extra_dims", "all");
                        pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", filterCrsOperation, optsWriter);
                    }
                    else
                    {
                        pdal::Options optsWriter;
                        optsWriter.add("extra_dims", "all");
                        pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                    }
                }
            }
            catch (...)
            {
                {// for close
                    pdal::LasReader lasReader;
                    pdal::Options options;
                    options.add("filename", fileName);
                    lasReader.setOptions(options);
                    lasReader.prepare(pointTable);
                    const pdal::LasHeader& h = lasReader.header();
                    pdal::SpatialReference crs = h.srs();
                    crsWKT = crs.getWKT1();
                }
                pdal::Stage& reader = pdalPipeline.addReader("readers.las");
                reader.setOptions(optsReader);
                if (crsWKT.empty())
                {
                    pdal::Options optsWriter;
                    if (!crsId.empty())
                        optsWriter.add("a_srs", crsId);
                    optsWriter.add("extra_dims", "all");
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                }
                else
                {
                    if (!crsId.empty())
                    {
                        std::string inputFileCrsId;
                        mPtrGeoTools->ptrCRSsTools()->setCRSFromWkt(crsWKT, inputFileCrsId);
                        pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                        pdal::Options optsFilterCrsOperation;
                        optsFilterCrsOperation.add("in_srs", inputFileCrsId);
                        optsFilterCrsOperation.add("out_srs", pdalCrsId);// crsId);
                        filterCrsOperation.setOptions(optsFilterCrsOperation);
                        pdal::Options optsWriter;
                        optsWriter.add("a_srs", crsId);
                        optsWriter.add("extra_dims", "all");
                        pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", filterCrsOperation, optsWriter);
                    }
                    else
                    {
                        pdal::Options optsWriter;
                        optsWriter.add("extra_dims", "all");
                        pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
                    }
                }
            }
            pdal::point_count_t np = pdalPipeline.execute();
            succesFull = !std::ifstream(outputFileName).fail();
        }
        else
        {
            TL_ASSERT(false, "Input file must be LAS/LAZ");
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    };
    return(succesFull);
}
*/
// PointCloudFileManager* PointCloudTools::ptrPointCloudFileManager()
// {
    // if (mPtrPointCloudFileManager == NULL)
    // {
        // mPtrPointCloudFileManager = new PointCloudFileManagerImpl();
    // }
    // //return(dynamic_cast<CRSsTools*>(mPtrCRSsTools));
    // return(mPtrPointCloudFileManager);
// }
