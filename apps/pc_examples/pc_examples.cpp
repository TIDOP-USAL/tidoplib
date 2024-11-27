
#include <stdio.h>
#include <memory>
#include <fstream>
#include <map>
#include <vector>
//
#include <iostream>
#include <iomanip>
//
#include <algorithm>
#include <string>
//
#include <cpl_conv.h>
#include <ogr_core.h>
#include <gdal.h>
#include <ogr_srs_api.h>
//
#include <tidop/core/app.h>
#include <tidop/core/console.h>
#include <tidop/core/msg/message.h>
#include <tidop/core/path.h>
#include <tidop/core/log.h>
#include <tidop/geotools/GeoTools.h>
#include <tidop/pctools/PointCloudTools.h>
//#include <tidop/pctools/PointCloudFileManager.h>
//#include <tidop/pctools/PointCloudFile.h>
#include <tidop/pctools/PointCloudReader.h>

#include <copc-lib/io/copc_reader.hpp>
#include <copc-lib/laz/decompressor.hpp>

#include <pdal/stage.hpp>
#include <pdal/stagefactory.hpp>
#include <pdal/pipelinemanager.hpp>
#include <pdal/util/fileutils.hpp>


//using namespace pdal;
using namespace tl;
using namespace copc;
using namespace std;

void ReaderExample();

int main(int argc, char* argv[])
{
    std::string inputFileName, outputFileName;
    
    //std::string pointCloudFileName = "D:/dev/pnoa_lidar_2009.laz";
    //std::string pointCloudFileName = "D:/dev/pnoa_lidar_2009_dhl_writer_crs.copc.laz";
    bool doNoCopc = false;
    std::string pointCloudFileNameNoCopc = "D:/dev/pnoa_lidar_2009_crs_features.laz";
    std::string pointCloudFileName = "D:/dev/pnoa_lidar_2009_crs_features.copc.laz";
    outputFileName = "D:/dev/pnoa_lidar_2009_dhl_writer_crs.copc.laz.csv";

    Path app_path(argv[0]);
    std::string cmd_name = app_path.baseName().toString();

#ifdef TL_OS_WINDOWS
#if defined _DEBUG
    Path gdal_data_path("D:/dev/libs/gdal/3.7.0/vc16/share/gdal");
    CPLSetConfigOption("GDAL_DATA", gdal_data_path.toString().c_str());
    Path proj_data_path("D:/dev/libs/proj/9.2/vc16/share/proj");
#   if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
    CPLSetConfigOption("PROJ_DATA", proj_data_path.toString().c_str());
    CPLSetConfigOption("PROJ_LIB", proj_data_path.toString().c_str());
#   else
    std::string s_proj = proj_data_path.toString();
    const char* proj_data[]{ s_proj.c_str(), nullptr };
    OSRSetPROJSearchPaths(proj_data);
#   endif
#else
    tl::Path _path = app_path.parentPath().parentPath();
    tl::Path gdal_data_path(_path);
    gdal_data_path.append("gdal\\data");
    tl::Path proj_data_path(_path);
    proj_data_path.append("proj");
    CPLSetConfigOption("GDAL_DATA", gdal_data_path.toString().c_str());
#   if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
    CPLSetConfigOption("PROJ_DATA", proj_data_path.toString().c_str());
    CPLSetConfigOption("PROJ_LIB", proj_data_path.toString().c_str());
#   else
    std::string s_proj = proj_data_path.toString();
    const char* proj_data[]{ s_proj.c_str(), nullptr };
    OSRSetPROJSearchPaths(proj_data);
#   endif
#   endif
#endif

    // Consola
    Console& console = App::console();
    console.setTitle(cmd_name);
    console.setConsoleUnicode();
    console.setMessageLevel(MessageLevel::all);
    Message::addMessageHandler(&console);

    GeoTools* ptrGeoTools = GeoTools::getInstance();
    try {
        bool ignoreDeprecatedCRSs = true;
        ptrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);
    }
    catch (const std::exception& e) {
        printException(e);
        return 1;
    }

    std::string sourceCrsId, targetCrsId;
    // ply to copc
    //inputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_clipped.ply";
    //outputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_25830_5782_bad_crs_25830.laz";
    //sourceCrsId = "ENU:4326;-4.344685328;36.717412795;146.7191";
    //targetCrsId = "EPSG:25830";// -> 25830+5782 por graphos
    //try
    //{
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    ptrPointCloudTools->formatFileConversionToCOPC(inputFileName,
    //        outputFileName, targetCrsId, sourceCrsId);
    //    int yo = 1;
    //}
    //catch (const std::exception& e) {
    //    printException(e);
    //    return 1;
    //}
    //std::string assignCrs_crsId = "EPSG:25830+5782";
    //try
    //{
    //    inputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_25830_5782_bad_crs_25830.copc.laz";
    //    outputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_25830_5782.copc.laz";
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    bool succesFull = ptrPointCloudTools->assignCRS(inputFileName,
    //        assignCrs_crsId, outputFileName);
    //}
    //catch (...) {
    //    TL_THROW_EXCEPTION_WITH_NESTED("");
    //};

    //inputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_25830_5782.copc.laz";
    //outputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_25830.laz";
    //sourceCrsId = "EPSG:25830+5782";
    //targetCrsId = "EPSG:25830";// -> 25830+5782 por c
    //try
    //{
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    ptrPointCloudTools->formatFileConversionToCOPC(inputFileName,
    //        outputFileName, targetCrsId, sourceCrsId);
    //    int yo = 1;
    //}
    //catch (const std::exception& e) {
    //    printException(e);
    //    return 1;
    //}

    //inputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_25830.copc.laz";
    //outputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_4258_5782.laz";
    //sourceCrsId = "EPSG:25830";
    //targetCrsId = "EPSG:4258+5782";
    //try
    //{
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    ptrPointCloudTools->formatFileConversionToCOPC(inputFileName,
    //        outputFileName, targetCrsId, sourceCrsId);
    //    int yo = 1;
    //}
    //catch (const std::exception& e) {
    //    printException(e);
    //    return 1;
    //}

    inputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_4258_5782.copc.laz";
    outputFileName = "D:/GraphosProjects/Candado/dense/mvs/model_dense_25830_5782_bis.laz";
    sourceCrsId = "EPSG:4258+5782";
    targetCrsId = "EPSG:25830+5782";
    try
    {
        PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
        ptrPointCloudTools->formatFileConversionToCOPC(inputFileName,
            outputFileName, targetCrsId, sourceCrsId);
        int yo = 1;
    }
    catch (const std::exception& e) {
        printException(e);
        return 1;
    }

    // assign CRS
    //std::string assignCrsInputFileName = "D:/dev/pnoa_lidar_2009_25830_5782.laz";
    std::string assignCrsInputFileName = "D:/dev/pnoa_lidar_2009.laz";
    std::string assignCrsOutputFileName = "D:/dev/pnoa_lidar_2009_test_25830_5782.laz";
    std::string assignCrs_crsId = "EPSG:25830+5782";
    //try
    //{
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    bool succesFull = ptrPointCloudTools->assignCRS(assignCrsInputFileName,
    //        assignCrs_crsId, assignCrsOutputFileName);
    //}
    //catch (...) {
    //    TL_THROW_EXCEPTION_WITH_NESTED("");
    //};
    //assignCrsInputFileName = "D:/dev/pnoa_lidar_2009_test_25830_5782.laz";
    //assignCrsOutputFileName = "D:/dev/pnoa_lidar_2009_test_25830.laz";
    //assignCrs_crsId = "EPSG:25830";
    //try
    //{
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    bool succesFull = ptrPointCloudTools->assignCRS(assignCrsInputFileName,
    //        assignCrs_crsId, assignCrsOutputFileName);
    //}
    //catch (...) {
    //    TL_THROW_EXCEPTION_WITH_NESTED("");
    //};

    //std::string toCopcInputFileName = "D:/dev/pnoa_lidar_2009.laz";
    //std::string toCopcOutputFileName = "D:/dev/pnoa_lidar_2009_test.copc.laz";
    //std::string toCopcCrs_crsId;
    //try
    //{
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    bool succesFull = ptrPointCloudTools->toCOPC(toCopcInputFileName,
    //        toCopcOutputFileName, toCopcCrs_crsId);
    //}
    //catch (...) {
    //    TL_THROW_EXCEPTION_WITH_NESTED("");
    //};

    //toCopcInputFileName = "D:/dev/pnoa_lidar_2009.laz";
    //toCopcOutputFileName = "D:/dev/pnoa_lidar_2009_test_25830_5782.copc.laz";
    //toCopcCrs_crsId = "EPSG:25830+5782";
    //try
    //{
    //    PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //    bool succesFull = ptrPointCloudTools->toCOPC(toCopcInputFileName,
    //        toCopcOutputFileName, toCopcCrs_crsId);
    //}
    //catch (...) {
    //    TL_THROW_EXCEPTION_WITH_NESTED("");
    //};

    assignCrsInputFileName = "D:/dev/pnoa_lidar_2009_test.copc.laz";
    assignCrsOutputFileName = "D:/dev/pnoa_lidar_2009_test_25830_5782.copc.laz";
    assignCrs_crsId = "EPSG:25830+5782";
    try
    {
        PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
        bool succesFull = ptrPointCloudTools->assignCRS(assignCrsInputFileName,
            assignCrs_crsId, assignCrsOutputFileName);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    };


    //try {
    //    ReaderExample();
    //}
    //catch (const std::exception& e) {
    //    printException(e);
    //    return 1;
    //}

    std::string crsId_25830_5782 = "EPSG:25830+5782";
    std::string crsId_4937 = "EPSG:4937";
    std::string crsId_4258_5782 = "EPSG:4258+5782";
    // 25830+5782
    double x_min = 598400.0;
    double y_min = 4264600.0;
    double z_min = 487.0;
    double x_max = 598800.0;
    double y_max = 4265000.0;
    double z_max = 781.0;
    // 4258+5782
    //double x_min = -1.87111208185624056;
    //double y_min = 38.52431347581596555;
    //double z_min = 487.0;
    //double x_max = -1.86652393733056021;
    //double y_max = 38.52796193315600703;
    //double z_max = 781.0;

    double x_o, y_o, z_o;
    try {
        PointCloudReader::Ptr pointCloudReader;
        pointCloudReader = PointCloudReaderFactory::create(pointCloudFileName);
        pointCloudReader->open();
        std::vector<std::string> dimensionsNames;
        pointCloudReader->getDimensionsNames(dimensionsNames);
        double xmin_25830_5782, ymin_25830_5782, zmin_25830_5782;
        double xmax_25830_5782, ymax_25830_5782, zmax_25830_5782;
        pointCloudReader->getBoundingBox(xmin_25830_5782, ymin_25830_5782, zmin_25830_5782,
            xmax_25830_5782, ymax_25830_5782, zmax_25830_5782, crsId_25830_5782);
        double xmin_4937, ymin_4937, zmin_4937;
        double xmax_4937, ymax_4937, zmax_4937;
        pointCloudReader->getBoundingBox(xmin_4937, ymin_4937, zmin_4937,
            xmax_4937, ymax_4937, zmax_4937, crsId_4937);
        std::map<int, double> resolutionByLevel;
        pointCloudReader->copcGetResolutionByLevel(resolutionByLevel);
        double resolution = 0.05;
        std::vector<std::vector<float> > coordinates;
        std::vector<std::vector<float> > dimensionsValues;
        pointCloudReader->getPoints(x_o, y_o, z_o, coordinates, 
            dimensionsNames, dimensionsValues,
            x_min, y_min, z_min, x_max, y_max, z_max, resolution, crsId_25830_5782);// crsId_25830_5782);

        // no copc
        if (doNoCopc)
        {
            PointCloudReader::Ptr pointCloudReaderNoCopc;
            pointCloudReaderNoCopc = PointCloudReaderFactory::create(pointCloudFileNameNoCopc);
            pointCloudReaderNoCopc->open();
            std::vector<std::string> dimensionsNamesNoCopc;
            pointCloudReaderNoCopc->getDimensionsNames(dimensionsNamesNoCopc);
            double xmin_25830_5782NoCopc, ymin_25830_5782NoCopc, zmin_25830_5782NoCopc;
            double xmax_25830_5782NoCopc, ymax_25830_5782NoCopc, zmax_25830_5782NoCopc;
            pointCloudReaderNoCopc->getBoundingBox(xmin_25830_5782NoCopc, ymin_25830_5782NoCopc, zmin_25830_5782NoCopc,
                xmax_25830_5782NoCopc, ymax_25830_5782NoCopc, zmax_25830_5782NoCopc, crsId_25830_5782);
            double xmin_4937NoCopc, ymin_4937NoCopc, zmin_4937NoCopc;
            double xmax_4937NoCopc, ymax_4937NoCopc, zmax_4937NoCopc;
            pointCloudReaderNoCopc->getBoundingBox(xmin_4937NoCopc, ymin_4937NoCopc, zmin_4937NoCopc,
                xmax_4937NoCopc, ymax_4937NoCopc, zmax_4937NoCopc, crsId_4937);
            std::vector<std::vector<float> > coordinatesNoCopc;
            std::vector<std::vector<float> > dimensionsValuesNoCopc;
            pointCloudReaderNoCopc->getPoints(x_o, y_o, z_o,coordinatesNoCopc, 
                dimensionsNamesNoCopc, dimensionsValuesNoCopc,
                x_min, y_min, z_min, x_max, y_max, z_max, crsId_25830_5782);// crsId_25830_5782);
        }
        int yo = 1;
    }
    catch (const std::exception& e) {
        printException(e);
        return 1;
    }

    //if (image_reader->isOpen()) {

    //PointCloudTools* ptrPointCloudTools = PointCloudTools::getInstance();
    //try {
    //    bool ignoreDeprecatedCRSs = true;
    //    PointCloudFile* ptrPointCloudFile=ptrPointCloudTools->ptrPointCloudFileManager()->ptrPointCloudFile(pointCloudFileName);
    //    double xmin, ymin, zmin, xmax, ymax, zmax;
    //    ptrPointCloudFile->getBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);
    //    int yo = 1;
    //}
    //catch (const std::exception& e) {
    //    printException(e);
    //    return 1;
    //}

    Message::success("Ended process");
    
    /*
    const std::string inputFileName = "D:/dev/sources/pdal_examples/data/dhl/pnoa_lidar_2009_dhl_writer.copc.laz";
	const std::string outputFileName = "D:/dev/sources/pdal_examples/data/dhl/pnoa_lidar_2009_dhl_writer_csf_dhl_host.copc.laz";
    const std::string outputDtmFileName = "D:/dev/sources/pdal_examples/data/dhl/pnoa_lidar_2009_dhl_writer_csf_dhl_host.tif";

    {
        PipelineManager mgr;
        Options optsR;
        optsR.add("filename", inputFileName);
        Stage& reader = mgr.addReader("readers.copc");
        reader.setOptions(optsR);

        Stage& filterElm = mgr.makeFilter("filters.elm", reader);
        Stage& filterOutlier = mgr.makeFilter("filters.outlier", filterElm);
        Stage& filterCsf = mgr.makeFilter("filters.csf", filterOutlier);
        Options optsFilterCsf;
        optsFilterCsf.add("resolution", 2.0);
        optsFilterCsf.add("where", "!(Classification == 7)");
        optsFilterCsf.add("step", 2.0);
        optsFilterCsf.add("threshold", 0.5);
        optsFilterCsf.add("rigidness", 1);
        filterCsf.setOptions(optsFilterCsf);

        Stage& filterCrsOperation = mgr.makeFilter("filters.reprojection", filterCsf);
        Options optsFilterCrsOperation;
        optsFilterCrsOperation.add("in_srs","EPSG:25830+5782");
        optsFilterCrsOperation.add("out_srs", "EPSG:25830+4258");
        filterCrsOperation.setOptions(optsFilterCrsOperation);

        //Stage& writer = mgr.makeWriter(outputFileName, "writers.copc", filterCsf);
        //Stage& writer = mgr.makeWriter(outputFileName, "writers.copc", filterCrsOperation);
        Options optsFilterWriter;
        optsFilterWriter.add("a_srs", "EPSG:25830");
        Stage& writer = mgr.makeWriter(outputFileName, "writers.copc", filterCrsOperation, optsFilterWriter);
        //optsFilterWriter.add("filename", outputFileName);
        //writer.setOptions(optsFilterWriter);

        //Stage& filterRange = mgr.makeFilter("filters.range", filterCsf);
        //Options optsFilterRange;
        //optsFilterRange.add("limits", "Classification[2:2]");
        //filterRange.setOptions(optsFilterRange);
        //Stage& writerDtm = mgr.makeWriter(outputDtmFileName, "writers.gdal", filterRange);
        //Options optsDtm;
        //optsDtm.add("gdaldriver", "GTiff");
        //optsDtm.add("output_type", "all");
        //optsDtm.add("resolution", "5.0");
        //writerDtm.setOptions(optsDtm);
        point_count_t np = mgr.execute();
        bool succesFull = !std::ifstream(outputFileName).fail();
    }

    //{
    //    PipelineManager mgrDtm;
    //    Stage& readerToDtm = mgrDtm.makeReader(outputFileName, "readers.copc");
    //    Stage& filterRange = mgrDtm.makeFilter("filters.range", readerToDtm);
    //    Options optsFilterRange;
    //    optsFilterRange.add("limits", "Classification[2:2]");
    //    filterRange.setOptions(optsFilterRange);
    //    Stage& writerDtm = mgrDtm.makeWriter(outputDtmFileName, "writers.gdal", filterRange);
    //    Options optsDtm;
    //    optsDtm.add("gdaldriver", "GTiff");
    //    optsDtm.add("output_type", "all");
    //    optsDtm.add("resolution", "5.0");
    //    writerDtm.setOptions(optsDtm);
    //    point_count_t np2 = mgrDtm.execute();
    //    bool succesFullDtm = !std::ifstream(outputDtmFileName).fail();
    //}

    int yo = 1;
    //Stage& r = mgr.makeReader("in.las", "readers.las");
    //Stage& f = mgr.makeFilter("filters.crop", r);
    //Stage& w = mgr.makeWriter("out.las", "writers.las", f);
    */
}

void ReaderExample()
{
    // Create a reader object
    //FileReader reader("D:/dev/sources/copc-lib-main/example/build/autzen-classified.copc.laz");
    //FileReader reader("D:/dev/pnoa_lidar_2009_dhl_writer_crs.copc.laz");
    //try {
    FileReader reader("D:/dev/pnoa_lidar_2009.laz");
    //}
    //catch (const std::exception& e) {
    //    printException(e);
    //    return;
    //}

    // Get the Las Header
    auto las_header = reader.CopcConfig().LasHeader();
    //cout << endl << "Las Header:" << endl;
    //cout << "\tPoint Format: " << (int)las_header.PointFormatId() << endl
    //     << "\tPoint Count: " << (int)las_header.PointCount() << endl;

    // Get the CopcInfo struct
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

    // Get the WKT string
    //cout << "WKT: " << reader.CopcConfig().Wkt() << endl;

    //cout << endl;

    //std::vector<double> resolutions = {50.0, 30.0, 20.0, 10.0, 5.0, 2.0, 1.0, 0.5, 0.2, 0.1};
    //for (double resolution: resolutions)
    //{
    //    auto target_depth = reader.GetDepthAtResolution(resolution);
    //    cout << "Resolution: " << resolution << " m"
    //         << "dept: "<< target_depth<< endl;
    //    std::vector<Node> nodesAtResolution = reader.GetNodesAtResolution(resolution);
    //    for (const auto &node : nodesAtResolution)
    //    {
    //        cout << node.key << "(" << node.key.Resolution(las_header, copc_info) << ")" << endl;
    //    }
    //}

    std::string outputFileName = "D:/dev/pnoa_lidar_2009_dhl_writer_crs.copc.laz.csv";
    std::ofstream ofs;
    ofs.open(outputFileName, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open())
    {
        cout << "Error opening file: " << outputFileName << endl;
        exit(1);
    }
    ofs << "id;level;point_count;resolution;wkt" << endl;
    int number_of_nodes = 0;
    for (const auto& node : reader.GetAllChildrenOfPage(VoxelKey::RootKey()))
        //for (const auto &node : reader.GetAllNodes())
    {
        Box nodeBox(node.key, las_header);
        std::ostringstream streamObj_xmin;
        streamObj_xmin << std::fixed << std::showpoint << std::setprecision(3) << nodeBox.x_min;
        std::ostringstream streamObj_xmax;
        streamObj_xmax << std::fixed << std::showpoint << std::setprecision(3) << nodeBox.x_max;
        std::ostringstream streamObj_ymin;
        streamObj_ymin << std::fixed << std::showpoint << std::setprecision(3) << nodeBox.y_min;
        std::ostringstream streamObj_ymax;
        streamObj_ymax << std::fixed << std::showpoint << std::setprecision(3) << nodeBox.y_max;

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
        ofs << std::fixed << std::setprecision(3) << node.key.Resolution(las_header, copc_info) << ";";
        ofs << wkt << endl;

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
    int yo = 1;
    //{
    //    VoxelKey load_key(4, 11, 9, 0);

    //    // FindNode will automatically load the minimum pages needed
    //    // to find the key you request
    //    Node node = reader.FindNode(load_key);
    //    // If FindNode can't find the node, it will return an "invalid" node:
    //    if (!node.IsValid())
    //        exit(123);

    //    // GetPoints returns a Points object, which provides helper functions
    //    // as well as a Get() function to access the underlying point vector
    //    las::Points node_points = reader.GetPoints(node);

    //    cout << node_points.ToString() << endl;

    //    cout << endl << "First 5 points: " << endl;
    //    for (int i = 0; i < 5; i++)
    //    {
    //        cout << node_points.Get(i)->ToString() << endl;
    //    }
    //}

    //// We can also get the raw compressed data if we want to decompress it ourselves:
    //{
    //    VoxelKey loadKey(4, 11, 9, 0);

    //    Node node = reader.FindNode(loadKey);
    //    if (!node.IsValid())
    //        exit(123);
    //    vector<char> compressed_data = reader.GetPointDataCompressed(node);

    //    // We can decompress `n` number of points, or we can decompress the entire node
    //    // by setting n=node.point_count
    //    int num_points_to_decompress = 5;
    //    vector<char> uncompressed_data =
    //        copc::laz::Decompressor::DecompressBytes(compressed_data, las_header, num_points_to_decompress);

    //    cout << endl
    //         << "Successfully decompressed " << uncompressed_data.size() / las_header.PointRecordLength() << "points!"
    //         << endl;
    //}
}
