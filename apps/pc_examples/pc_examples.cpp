
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

#include <pdal/stage.hpp>
#include <pdal/stagefactory.hpp>
#include <pdal/pipelinemanager.hpp>
#include <pdal/util/fileutils.hpp>

//using namespace pdal;
using namespace tl;


int main(int argc, char* argv[])
{
    Path app_path(argv[0]);
    std::string cmd_name = app_path.baseName().toString();

#ifdef TL_OS_WINDOWS
#if defined _DEBUG
    Path gdal_data_path("D:/dev/libs/gdal/3.7.0/vc16/share/gdal");
    CPLSetConfigOption("GDAL_DATA", gdal_data_path.toString().c_str());
    Path proj_data_path("D:/dev/libs/proj/9.2/vc16/share/proj");
#   if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,7,0)
    CPLSetConfigOption("PROJ_DATA", proj_data_path.toString().c_str());
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

    try {
        GeoTools* ptrGeoTools = GeoTools::getInstance();
        bool ignoreDeprecatedCRSs = true;
        ptrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);
    }
    catch (const std::exception& e) {
        printException(e);
        return 1;
    }
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