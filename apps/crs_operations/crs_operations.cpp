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
 
#include <stdio.h>
#include <memory>
#include <fstream>
#include <map>
#include <vector>

#include <iostream>
#include <iomanip>

#include <algorithm>
#include <string>

#include <cpl_conv.h>
#include <ogr_core.h>
#include <gdal.h>
#include <ogr_srs_api.h>

#include <tidop/core/app/app.h>
#include <tidop/core/console.h>
#include <tidop/core/app/message.h>
#include <tidop/core/base/path.h>
#include <tidop/core/app/log.h>
// #include <tidop/geometry/entities/point.h>
// #include <tidop/geospatial/crs.h>
// #include <tidop/geospatial/crstransf.h>
#include <tidop/geotools/GeoTools.h>
#include <tidop/geotools/CRSsTools.h>

using namespace tl;

bool test_1(GeoTools* ptrGeoTools, std::string outputFileName, std::string& strError);
bool test_2(GeoTools* ptrGeoTools, std::string outputFileName, std::string& strError);

int main(int argc, char **argv)
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

            //Command cmd(cmd_name, "Ejemplo de transformación de coordenadas");
            //cmd.addArgument<std::string>("epsg_in", 'i', "Sistema de referencia de entrada");
            //cmd.addArgument<std::string>("epsg_out", 'o', "Sistema de referencia de salida");
            //cmd.addArgument<std::string>("coord", 'c', "Fichero de texto con las coordenadas separadas por comas o cadena de texto con las coordenadas de un punto");
            //cmd.addArgument<char>("separator", 's', "Caracter separador de coordenadas. Por defecto ';'", ';');
            //cmd.addArgument<Path>("coord_trf", 't', "Fichero de texto con las coordenadas transformadas", Path{});
            //cmd.addArgument<Path>("log", 'l', "Fichero de log", Path{});
            //cmd.addArgument<int>("skip","Skip lines", 0);

            //cmd.addExample(cmd_name + " --epsg_in EPSG:25830 --epsg_out EPSG:4258 --coord 281815.044;4827675.243;123.35");
            //cmd.addExample(cmd_name + " -iEPSG:25830 -oEPSG:4258 --coord utm.txt");

            //Command::Status status = cmd.parse(argc, argv);

            //if(status == Command::Status::parse_error) {
            //    return 1;
            //} else if(status == Command::Status::show_help || 
            //          status == Command::Status::show_licence || 
            //          status == Command::Status::show_version) {
            //    return 0;
            //}
    std::string strError;
    try {
        GeoTools* ptrGeoTools = GeoTools::getInstance();
        bool ignoreDeprecatedCRSs = true;
        ptrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);
        std::map<std::string, CRSInfo> mapCRSsInfo;
        ptrGeoTools->ptrCRSsTools()->getCRSsInfo(mapCRSsInfo);
        std::string crssInfoFileName = "D:/dev/sources/tidoplib/apps/crs_operations/CRSsInfo.csv";
        ptrGeoTools->ptrCRSsTools()->dumpCRSsInfoToFile(crssInfoFileName);
        //// test getENUCrs
        {
            std::string crsEnuBaseId2D = "EPSG:4258";
            std::string crsEnuId = "ENU:4937;-4.495021180808;36.756413127079;142.1590";
            std::string crs25830Id = "EPSG:25830";
            double fc4258 = -4.495021180808;
            double sc4258 = 36.756413127079;
            double tcElip = 142.1590;
            double tcHOrth = 94.2172;
            double fc25830 = fc4258;
            double sc25830 = sc4258;
            double tc25830 = tcElip;
            ptrGeoTools->ptrCRSsTools()->crsOperation(crsEnuBaseId2D, crs25830Id, fc25830, sc25830, tc25830);
            std::string crsEnuFrom25830 = ptrGeoTools->ptrCRSsTools()->getCRSEnu(crs25830Id, fc25830, sc25830, tc25830);
            std::string crs25830_5782Id = "EPSG:25830+5782";
            std::string crsEnuFrom25830_5782 = ptrGeoTools->ptrCRSsTools()->getCRSEnu(crs25830_5782Id, fc25830, sc25830, tcHOrth);
            double fc4258FromEnuHElip = 0.;
            double sc4258FromEnuHElip = 0.;
            double tc4258FromEnuHElip = 0.;
            ptrGeoTools->ptrCRSsTools()->crsOperation(crsEnuFrom25830, crsEnuBaseId2D, 
                fc4258FromEnuHElip, sc4258FromEnuHElip, tc4258FromEnuHElip);
            double fc4258FromEnuHOrth = 0.;
            double sc4258FromEnuHOrth = 0.;
            double tc4258FromEnuHOrth = 0.;
            ptrGeoTools->ptrCRSsTools()->crsOperation(crsEnuFrom25830, crsEnuBaseId2D,
                fc4258FromEnuHOrth, sc4258FromEnuHOrth, tc4258FromEnuHOrth);
            int yo = 1;
        }
        
        // test CRSsVertical
        {
            std::map<std::string, CRSInfo> crssFor2dApplications;
            ptrGeoTools->ptrCRSsTools()->getCRSsFor2dApplications(crssFor2dApplications);
            std::string crsId_1 = "EPSG:25830";
            std::map<std::string, CRSInfo> crssVertical_1;
            ptrGeoTools->ptrCRSsTools()->getCRSsVertical(crsId_1, crssVertical_1);
        }

        // test 1
        {
            std::string outputFileName_1 = "D:/dev/sources/tidoplib/apps/crs_operations/CrsOperationsTest1.txt";
            if (!test_1(ptrGeoTools, outputFileName_1, strError))
            {

            }
        }

        // test 2
        {
            std::string outputFileName_2 = "D:/dev/sources/tidoplib/apps/crs_operations/CrsOperationsTest2.txt";
            if (!test_2(ptrGeoTools, outputFileName_2, strError))
            {

            }
        }
        int yo = 1;
    }
    catch (const std::exception& e) {
        printException(e);
        return 1;
    }
    Message::success("Ended process");
    // Message::info("{};{};{} -> {};{};{}", point[0], point[1], point[2], pt_out.x, pt_out.y, pt_out.z);


    // auto epsg_in = cmd.value<std::string>("epsg_in");
    // auto epsg_out = cmd.value<std::string>("epsg_out");
    // auto coord = cmd.value<std::string>("coord");
    // auto separator = cmd.value<char>("separator");
    // auto coord_trf = cmd.value<Path>("coord_trf");
    // auto log_file = cmd.value<Path>("log");
    // auto skip_lines = cmd.value<int>("skip");

    // if(!log_file.empty()) {
        // Log &log = App::log();
        // log.setMessageLevel(MessageLevel::all);
        // log.open(log_file);
        // Message::addMessageHandler(&log);
    // }

    // try {

        // std::shared_ptr<Crs> epsgIn(new Crs(epsg_in));
        // std::shared_ptr<Crs> epsgOut(new Crs(epsg_out));
        // CrsTransform crs(epsgIn, epsgOut);

        // std::ofstream ofs;
        // ofs.open(coord_trf.toString(), std::ofstream::out | std::ofstream::trunc);

        // if(Path::exists(coord)) {

            // std::ifstream ifs;
            // ifs.open(coord, std::ifstream::in);
            // if(ifs.is_open()) {

                // std::string line;
                // while(std::getline(ifs, line)) {

                    // if (skip_lines > 0) {
                        // if (ofs.is_open())
                            // ofs << line << std::endl;
                        // skip_lines--;
                        // continue;
                    // }

                    // std::vector<double> vector = split<double>(line, separator);
                    // TL_ASSERT(vector.size() >= 3, "");
                    // Point3<double> pt_in(vector[0], vector[1], vector[2]);
                    // Point3<double> pt_out;
                    // crs.transform(pt_in, pt_out);
                    // //Message::info("{};{};{} -> {};{};{}", vector[0], vector[1], vector[2], pt_out.x, pt_out.y, pt_out.z);

                    // if(ofs.is_open()) {
                        // ofs << std::fixed << pt_out.x << separator << pt_out.y << separator << pt_out.z << separator;
                        // for (size_t i = 3; i < vector.size(); i++)
                            // ofs << vector[i] << separator;
                        // ofs << std::endl;
                    // }
                // }
                // ifs.close();
            // }

        // } else {

            // std::vector<double> point = split<double>(coord, separator);
            // Point3<double> pt_in(point[0], point[1], point[2]);
            // Point3<double> pt_out;
            // crs.transform(pt_in, pt_out);
            // Message::info("{};{};{} -> {};{};{}", point[0], point[1], point[2], pt_out.x, pt_out.y, pt_out.z);

            // if(ofs.is_open()) {
                // ofs << point[0] << separator << point[1] << separator << point[2] << separator << " -> "
                    // << pt_out.x << separator << pt_out.y << separator << pt_out.z << std::endl;
            // }

        // }

        // if(ofs.is_open()) ofs.close();

    // } catch(const std::exception &e) {
        // printException(e);
        // return 1;
    // }

    return 0;
}

bool test_1(GeoTools* ptrGeoTools, std::string outputFileName, std::string& strError)
{
    std::string functionName = "test_1";
    std::map<std::string, std::vector<double> > coor_4937, coor_4346, coor_4258_msl, coor_25830, coor_25830_msl;
    std::map<std::string, std::vector<double> > coor_4230, coor_4230_msl, coor_23030, coor_23030_msl;

    coor_4937["1001"]={ -8.380627694 , 43.326214300 , 70.097 };
    coor_4937["1003"] = { -1.955086196 , 39.003867259 , 750.786 };
    coor_4937["1012"] = { -4.443571654 , 36.684036053 , 117.310 };
    coor_4937["1013"] = { 2.683869762 , 39.541883089 , 62.554 };
    coor_4937["1015"] = { -5.443845659 , 40.913368096 , 861.337 };

    coor_4346["1001"] = { 4597588.151 , -677324.441 , 4353984.396 };
    coor_4346["1003"] = { 4960751.135 , -169339.909 , 3993123.187 };
    coor_4346["1012"] = { 5105718.475 , -396769.583 , 3789401.844 };
    coor_4346["1013"] = { 4919892.431 , 230628.099 , 4038930.385 };
    coor_4346["1015"] = { 4805774.310 , -457990.177 , 4155721.672 };

    coor_4258_msl["1001"] = { -8.380627694 , 43.326214300 , 15.485 };//54.612
    coor_4258_msl["1003"] = { -1.955086196 , 39.003867259 , 698.709 };//52.077
    coor_4258_msl["1012"] = { -4.443571654 , 36.684036053 , 69.568 };//47.743
    coor_4258_msl["1013"] = { 2.683869762 , 39.541883089 , 13.618 };//48.936
    coor_4258_msl["1015"] = { -5.443845659 , 40.913368096 , 806.420 };//54.917

    coor_25830["1001"] = { 63723.324 , 4811118.823 , 70.097 };
    coor_25830["1003"] = { 590477.047 , 4317725.016 , 750.786 };
    coor_25830["1012"] = { 371023.221 , 4060793.167 , 117.310 };
    coor_25830["1013"] = { 988552.725 , 4392368.299 , 62.554 };
    coor_25830["1015"] = { 294190.970 , 4532015.675 , 861.337 };

    coor_25830_msl["1001"] = { 63723.324 , 4811118.823 , 15.485 };
    coor_25830_msl["1003"] = { 590477.047 , 4317725.016 , 698.709 };
    coor_25830_msl["1012"] = { 371023.221 , 4060793.167 , 69.568 };
    coor_25830_msl["1013"] = { 988552.725 , 4392368.299 , 13.618 };
    coor_25830_msl["1015"] = { 294190.970 , 4532015.675 , 806.420 };

    coor_4230["1001"] = { -8.379095832 , 43.327352788 };
    coor_4230["1003"] = { -1.953844430 , 39.005072992 };
    coor_4230["1012"] = { -4.442269164 , 36.685310149 };
    coor_4230["1013"] = { 2.684970898 , 39.543059997 };
    coor_4230["1015"] = { -5.442477238 , 40.914547851 };

    coor_4230_msl["1001"] = { -8.379095832 , 43.327352788 , 15.485 };
    coor_4230_msl["1003"] = { -1.953844430 , 39.005072992 , 698.709 };
    coor_4230_msl["1012"] = { -4.442269164 , 36.685310149 , 69.568 };
    coor_4230_msl["1013"] = { 2.684970898 , 39.543059997 , 13.618 };
    coor_4230_msl["1015"] = { -5.442477238 , 40.914547851 , 806.420 };

    coor_23030["1001"] = { 63835.641 , 4811324.773 };
    coor_23030["1003"] = { 590587.107 , 4317932.878 };
    coor_23030["1012"] = { 371136.003 , 4060998.681 };
    coor_23030["1013"] = { 988661.192 , 4392580.200 };
    coor_23030["1015"] = { 294300.534 , 4532222.386 };

    coor_23030_msl["1001"] = { 63835.641 , 4811324.773 , 15.485 };
    coor_23030_msl["1003"] = { 590587.107 , 4317932.878 , 698.709 };
    coor_23030_msl["1012"] = { 371136.003 , 4060998.681 , 69.568 };
    coor_23030_msl["1013"] = { 988661.192 , 4392580.200 , 13.618 };
    coor_23030_msl["1015"] = { 294300.534 , 4532222.386 , 806.420 };

    std::ofstream ofs;
    ofs.open(outputFileName, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open())
    {
        strError = functionName;
        strError += ("\nError opening file:\n" + outputFileName);
        return(false);
    }
    ofs << "TEST DE CRS_DESIGN" << "\n";
    ofs << "==================" << "\n\n";

    std::vector<std::string> sourceCrsIds;
    std::vector<std::map<std::string, std::vector<double> > > sourcePoints;
    std::vector<std::string> targetCrsIds;
    std::vector<std::map<std::string, std::vector<double> > > targetPoints;

    std::string epsgString4346 = "EPSG:4346";
    //std::string epsgString4258="EPSG:4258";
    std::string epsgString4937 = "EPSG:4937";
    std::string epsgString4258MSL = "EPSG:4258+5782";
    std::string epsgString25830 = "EPSG:25830";
    std::string epsgString25830MSL = "EPSG:25830+5782";
    std::string epsgString25830EH = "EPSG:25830+4258";// no by Promote3D

    std::string epsgString4230 = "EPSG:4230";
    std::string epsgString4230MSL = "EPSG:4230+5782";
    std::string epsgString23030 = "EPSG:23030";
    std::string epsgString23030MSL = "EPSG:23030+5782";

    sourceCrsIds.push_back(epsgString4937);
    sourcePoints.push_back(coor_4937);
    targetCrsIds.push_back(epsgString4258MSL);
    targetPoints.push_back(coor_4258_msl);

    sourceCrsIds.push_back(epsgString4258MSL);
    sourcePoints.push_back(coor_4258_msl);
    targetCrsIds.push_back(epsgString4937);
    targetPoints.push_back(coor_4937);

    sourceCrsIds.push_back(epsgString4258MSL);
    sourcePoints.push_back(coor_4258_msl);
    targetCrsIds.push_back(epsgString25830MSL);
    targetPoints.push_back(coor_25830_msl);

    sourceCrsIds.push_back(epsgString25830MSL);
    sourcePoints.push_back(coor_25830_msl);
    targetCrsIds.push_back(epsgString4258MSL);
    targetPoints.push_back(coor_4258_msl);

    sourceCrsIds.push_back(epsgString4937);
    sourcePoints.push_back(coor_4937);
    targetCrsIds.push_back(epsgString25830);
    //targetCrsIds.push_back(epsgString25830EH);
    targetPoints.push_back(coor_25830);

    //sourceCrsIds.push_back(epsgString25830EH);
    sourceCrsIds.push_back(epsgString25830);
    sourcePoints.push_back(coor_25830);
    targetCrsIds.push_back(epsgString4937);
    targetPoints.push_back(coor_4937);

    // no PROJ 9.2
    //sourceCrsIds.push_back(epsgString23030MSL);
    //sourcePoints.push_back(coor_23030_msl);
    //targetCrsIds.push_back(epsgString4346);
    //targetPoints.push_back(coor_4346);

    //sourceCrsIds.push_back(epsgString4346);
    //sourcePoints.push_back(coor_4346);
    //targetCrsIds.push_back(epsgString23030MSL);
    //targetPoints.push_back(coor_23030_msl);

    sourceCrsIds.push_back(epsgString23030MSL);
    sourcePoints.push_back(coor_23030_msl);
    //targetCrsIds.push_back(epsgString25830EH);
    targetCrsIds.push_back(epsgString25830);
    targetPoints.push_back(coor_25830);

    //sourceCrsIds.push_back(epsgString25830EH);
    sourceCrsIds.push_back(epsgString25830);
    sourcePoints.push_back(coor_25830);
    targetCrsIds.push_back(epsgString23030MSL);
    targetPoints.push_back(coor_23030_msl);

    // Operaciones de conversion en ETRS89

    sourceCrsIds.push_back(epsgString4937);
    sourcePoints.push_back(coor_4937);
    //targetCrsIds.push_back(epsgString25830EH);
    targetCrsIds.push_back(epsgString25830);
    targetPoints.push_back(coor_25830);

    //sourceCrsIds.push_back(epsgString25830EH);
    sourceCrsIds.push_back(epsgString25830);
    sourcePoints.push_back(coor_25830);
    targetCrsIds.push_back(epsgString4937);
    targetPoints.push_back(coor_4937);

    //sourceCrsIds.push_back(epsgString4346);
    //sourcePoints.push_back(coor_4346);
    //targetCrsIds.push_back(epsgString4258MSL);
    //targetPoints.push_back(coor_4258_msl);

    //sourceCrsIds.push_back(epsgString4258MSL);
    //sourcePoints.push_back(coor_4258_msl);
    //targetCrsIds.push_back(epsgString4346);
    //targetPoints.push_back(coor_4346);

    //sourceCrsIds.push_back(epsgString4937);
    //sourcePoints.push_back(coor_4937);
    //targetCrsIds.push_back(epsgString4346);
    //targetPoints.push_back(coor_4346);

    //sourceCrsIds.push_back(epsgString4346);
    //sourcePoints.push_back(coor_4346);
    //targetCrsIds.push_back(epsgString4937);
    //targetPoints.push_back(coor_4937);

    sourceCrsIds.push_back(epsgString4258MSL);
    sourcePoints.push_back(coor_4258_msl);
    targetCrsIds.push_back(epsgString25830MSL);
    targetPoints.push_back(coor_25830_msl);

    sourceCrsIds.push_back(epsgString25830MSL);
    sourcePoints.push_back(coor_25830_msl);
    targetCrsIds.push_back(epsgString4258MSL);
    targetPoints.push_back(coor_4258_msl);

    //sourceCrsIds.push_back(epsgString25830EH);
    sourceCrsIds.push_back(epsgString25830);
    sourcePoints.push_back(coor_25830);
    targetCrsIds.push_back(epsgString4258MSL);
    targetPoints.push_back(coor_4258_msl);

    sourceCrsIds.push_back(epsgString4258MSL);
    sourcePoints.push_back(coor_4258_msl);
    //targetCrsIds.push_back(epsgString25830EH);
    targetCrsIds.push_back(epsgString25830);
    targetPoints.push_back(coor_25830);

    //std::string data = "Abc";
    //std::transform(data.begin(), data.end(),
    //    data.begin(), ::tolower);

    std::string strAuxError;
    for (int nt = 0; nt < sourceCrsIds.size(); nt++)
    //for (int nt = 0; nt < 1; nt++)
    {
        std::string sourceCrsId = sourceCrsIds.at(nt);
        std::string targetCrsId = targetCrsIds.at(nt);
        std::map<std::string, std::vector<double> > points = sourcePoints.at(nt);
        if (sourceCrsId == targetCrsId)
        {
            continue;
        }
        ptrGeoTools->ptrCRSsTools()->crsOperation(sourceCrsId, targetCrsId, points, true);
        if (!strAuxError.empty())
        {
            strError = functionName;
            strError += ("\n" + strAuxError);
            ofs.close();
            return(false);
        }
        int sourceCrsPrecision, targetCrsPrecision;
        int sourceVerticalPrecision, targetVerticalPrecision;
        ptrGeoTools->ptrCRSsTools()->getCRSPrecision(sourceCrsId,
            sourceCrsPrecision,
            sourceVerticalPrecision);
        if (!strAuxError.empty())
        {
            strError = functionName;
            strError += ("\n" + strAuxError);
            ofs.close();
            return(false);
        }
        ptrGeoTools->ptrCRSsTools()->getCRSPrecision(targetCrsId,
            targetCrsPrecision,
            targetVerticalPrecision);
        if (!strAuxError.empty())
        {
            strError = functionName;
            strError += ("\n" + strAuxError);
            ofs.close();
            return(false);
        }
        ofs << "- Test: " << (nt + 1) << "\n";
        ofs << "----------" << "\n";
        ofs << "  - CRS source ....................................:" << sourceCrsId << "\n";
        ofs << "  - CRS target ....................................:" << targetCrsId << "\n";
        ofs << "  Point.Id  Fc.Crs.Source  Sc.Crs.Source  Tc.Crs.Source  Fc.Crs.Target  Sc.Crs.Target  Tc.Crs.Target          Fc.Error          Sc.Error       Tc.Error" << "\n";
        std::string strNoCoordinateValue = "----------";
        for (auto const& x : points)
        {
            std::string pointId = x.first;
            std::vector<double> point = x.second;
            int pointSourceDimension = sourcePoints.at(nt)[pointId].size();
            double fcCrsSource = sourcePoints.at(nt)[pointId][0];
            double scCrsSource = sourcePoints.at(nt)[pointId][1];
            double tcCrsSource;
            if (pointSourceDimension == 3)
                tcCrsSource = sourcePoints.at(nt)[pointId][2];

            int pointTargetDimension = targetPoints.at(nt)[pointId].size();
            //            if(pointTargetDimension<pointSourceDimension)
            //            {
            //                pointTargetDimension=pointSourceDimension;
            //            }
            double fcCrsTarget = targetPoints.at(nt)[pointId][0];
            double scCrsTarget = targetPoints.at(nt)[pointId][1];
            double tcCrsTarget;
            if (pointTargetDimension == 3)
                tcCrsTarget = targetPoints.at(nt)[pointId][2];
            double fcCrsTargetCalc = point[0];
            double scCrsTargetCalc = point[1];
            double tcCrsTargetCalc;
            if (pointSourceDimension == 3
                && pointTargetDimension == 3)
                tcCrsTargetCalc = point[2];
            double fcError = fcCrsTarget - fcCrsTargetCalc;
            double scError = scCrsTarget - scCrsTargetCalc;
            double tcError;
            if (pointSourceDimension == 3
                && pointTargetDimension == 3)
                tcError = tcCrsTarget - tcCrsTargetCalc;
            ofs << std::setw(10) << std::right << pointId;
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(sourceCrsPrecision) << fcCrsSource;
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(sourceCrsPrecision) << scCrsSource;
            if (pointSourceDimension == 3)
                ofs << std::setw(15) << std::right << std::fixed << std::setprecision(sourceVerticalPrecision) << tcCrsSource;
            else
                ofs << std::setw(15) << std::right << " ";
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << fcCrsTarget;
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << scCrsTarget;
            if (pointTargetDimension == 3)
                ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetVerticalPrecision) << tcCrsTarget;
            else
                ofs << std::setw(15) << std::right << " ";
            ofs << std::setw(18) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << fcError;
            ofs << std::setw(18) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << scError;
            if (pointSourceDimension == 3
                && pointTargetDimension == 3)
                ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetVerticalPrecision) << tcError;
            else
                ofs << std::setw(15) << std::right << " ";
            ofs << "\n";
        }
    }

    ofs.close();
    return(true);
}

bool test_2(GeoTools* ptrGeoTools, std::string outputFileName, std::string& strError)
{
    std::string functionName = "test_2";

    std::map<std::string, std::vector<double> > coor_4937, coor_enu, coor_4258_msl;
    std::map<std::string, std::vector<double> > coor_4936, coor_25830, coor_25830_msl;

    coor_4937["1"] = { -4.497217703013,36.760517184373,122.7080 };
    coor_4937["2"] = { -4.496583599572,36.760661300696,125.8910 };
    coor_4937["3"] = { -4.497858961246,36.760281988169,117.7570 };
    coor_4937["4"] = { -4.496492325708,36.759341982626,128.3690 };
    coor_4937["5"] = { -4.495912663451,36.759291683360,138.0760 };
    coor_4937["6"] = { -4.497043975371,36.758978712618,124.5710 };
    coor_4937["7"] = { -4.497677442167,36.758305440089,124.3000 };
    coor_4937["8"] = { -4.497546059323,36.758426447336,122.5720 };
    coor_4937["9"] = { -4.494443893294,36.756906095656,147.1410 };
    coor_4937["10"] = { -4.495021180808,36.756413127079,142.1590 };
    coor_4937["11"] = { -4.495589168685,36.756069290791,139.6970 };
    coor_4937["12"] = { -4.493296655644,36.753637477440,139.6020 };
    coor_4937["13"] = { -4.493404118283,36.751686911571,135.5490 };
    coor_4937["14"] = { -4.492964197726,36.751889711901,136.5890 };
    coor_4937["15"] = { -4.492368477211,36.752332775397,138.8460 };
    coor_4937["16"] = { -4.491570794052,36.752535749293,141.0070 };

    coor_4258_msl["1"] = { -4.497217703013,36.760517184373,74.7560 };
    coor_4258_msl["2"] = { -4.496583599572,36.760661300696,77.9366 };
    coor_4258_msl["3"] = { -4.497858961246,36.760281988169,69.8078 };
    coor_4258_msl["4"] = { -4.496492325708,36.759341982626,80.4196 };
    coor_4258_msl["5"] = { -4.495912663451,36.759291683360,90.1252 };
    coor_4258_msl["6"] = { -4.497043975371,36.758978712618,76.6246 };
    coor_4258_msl["7"] = { -4.497677442167,36.758305440089,76.3581 };
    coor_4258_msl["8"] = { -4.497546059323,36.758426447336,74.6292 };
    coor_4258_msl["9"] = { -4.494443893294,36.756906095656,99.1956 };
    coor_4258_msl["10"] = { -4.495021180808,36.756413127079,94.2172 };
    coor_4258_msl["11"] = { -4.495589168685,36.756069290791,91.7581 };
    coor_4258_msl["12"] = { -4.493296655644,36.753637477440,91.6666 };
    coor_4258_msl["13"] = { -4.493404118283,36.751686911571,87.6218 };
    coor_4258_msl["14"] = { -4.492964197726,36.751889711901,88.6598 };
    coor_4258_msl["15"] = { -4.492368477211,36.752332775397,90.9134 };
    coor_4258_msl["16"] = { -4.491570794052,36.752535749293,93.0714 };

    coor_4936["1"] = { 5100289.7163,-401152.3016,3796208.1361 };
    coor_4936["2"] = { 5100287.1557,-401095.3052,3796222.8540 };
    coor_4936["3"] = { 5100296.8446,-401210.2983,3796184.2624 };
    coor_4936["4"] = { 5100377.1264,-401094.2053,3796107.0393 };
    coor_4936["5"] = { 5100392.2672,-401043.4762,3796108.3765 };
    coor_4936["6"] = { 5100394.2827,-401144.9653,3796072.4685 };
    coor_4936["7"] = { 5100434.2070,-401204.8450,3796012.4461 };
    coor_4936["8"] = { 5100425.7352,-401192.4107,3796022.1707 };
    coor_4936["9"] = { 5100567.7309,-400925.7123,3795901.6981 };
    coor_4936["10"] = { 5100592.3491,-400979.3563,3795854.8860 };
    coor_4936["11"] = { 5100609.1710,-401031.5550,3795822.8414 };
    coor_4936["12"] = { 5100786.1299,-400840.1151,3795606.5626 };
    coor_4936["13"] = { 5100911.2669,-400859.5751,3795430.7001 };
    coor_4936["14"] = { 5100901.7503,-400819.4203,3795449.3549 };
    coor_4936["15"] = { 5100878.3901,-400764.2220,3795490.1013 };
    coor_4936["16"] = { 5100872.2586,-400692.2866,3795509.4421 };

    coor_25830["1"] = { 366362.4770,4069351.4080,122.7080 };
    coor_25830["2"] = { 366419.3290,4069366.5110,125.8910 };
    coor_25830["3"] = { 366304.8280,4069326.2110,117.7570 };
    coor_25830["4"] = { 366425.1880,4069220.0190,128.3690 };
    coor_25830["5"] = { 366476.8440,4069213.6300,138.0760 };
    coor_25830["6"] = { 366375.3150,4069180.4880,124.5710 };
    coor_25830["7"] = { 366317.6000,4069106.6800,124.3000 };
    coor_25830["8"] = { 366329.5380,4069119.9210,122.5720 };
    coor_25830["9"] = { 366603.8210,4068946.9260,147.1410 };
    coor_25830["10"] = { 366551.4340,4068893.0410,142.1590 };
    coor_25830["11"] = { 366500.1350,4068855.6880,139.6970 };
    coor_25830["12"] = { 366700.5750,4068582.7090,139.6020 };
    coor_25830["13"] = { 366687.6060,4068366.4640,135.5490 };
    coor_25830["14"] = { 366727.2300,4068388.3500,136.5890 };
    coor_25830["15"] = { 366781.1780,4068436.6740,138.8460 };
    coor_25830["16"] = { 366852.7400,4068458.0820,141.0070 };

    coor_25830_msl["1"] = { 366362.4770,4069351.4080,74.7560 };
    coor_25830_msl["2"] = { 366419.3290,4069366.5110,77.9366 };
    coor_25830_msl["3"] = { 366304.8280,4069326.2110,69.8078 };
    coor_25830_msl["4"] = { 366425.1880,4069220.0190,80.4196 };
    coor_25830_msl["5"] = { 366476.8440,4069213.6300,90.1252 };
    coor_25830_msl["6"] = { 366375.3150,4069180.4880,76.6246 };
    coor_25830_msl["7"] = { 366317.6000,4069106.6800,76.3581 };
    coor_25830_msl["8"] = { 366329.5380,4069119.9210,74.6292 };
    coor_25830_msl["9"] = { 366603.8210,4068946.9260,99.1956 };
    coor_25830_msl["10"] = { 366551.4340,4068893.0410,94.2172 };
    coor_25830_msl["11"] = { 366500.1350,4068855.6880,91.7581 };
    coor_25830_msl["12"] = { 366700.5750,4068582.7090,91.6666 };
    coor_25830_msl["13"] = { 366687.6060,4068366.4640,87.6218 };
    coor_25830_msl["14"] = { 366727.2300,4068388.3500,88.6598 };
    coor_25830_msl["15"] = { 366781.1780,4068436.6740,90.9134 };
    coor_25830_msl["16"] = { 366852.7400,4068458.0820,93.0714 };

    coor_enu["1"] = { -196.13132302189277,455.45109522528946,-19.470324153080583 };
    coor_enu["2"] = { -139.51091540633385,471.44356962200254,-16.287001827731729 };
    coor_enu["3"] = { -253.39098083541617,429.35129995690659,-24.421523525379598 };
    coor_enu["4"] = { -131.36322985739466,325.03178641013801,-13.799658909440041 };
    coor_enu["5"] = { -79.603501571634808,319.44965302804485,-4.0915210228413343 };
    coor_enu["6"] = { -180.62254326252304,284.7184922657907,-17.596929216757417 };
    coor_enu["7"] = { -237.18912405822388,210.00324825244024,-17.866873009130359 };
    coor_enu["8"] = { -225.4569625078974,223.43168371357024,-19.594905724748969 };
    coor_enu["9"] = { 51.54963717055422,54.707663941197097,4.9815565422177315 };
    coor_enu["10"] = { 2.6258051288863271e-011,0,0 };
    coor_enu["11"] = { -50.719705063075743,-38.157257246319205,-2.4623158797621727 };
    coor_enu["12"] = { 154.00004008122855,-308.02770824125037,-2.5663177091628313 };
    coor_enu["13"] = { 144.40718281931035,-524.49246384715661,-6.6332644838839769 };
    coor_enu["14"] = { 183.69259476135963,-501.98595407092944,-5.5924570299685001 };
    coor_enu["15"] = { 236.89032750360101,-452.81566643668339,-3.333517100661993 };
    coor_enu["16"] = { 308.12390523432583,-430.28843003278598,-1.1739924941211939 };

    std::ofstream ofs;
    ofs.open(outputFileName, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open())
    {
        strError = functionName;
        strError += ("\nError opening file:\n" + outputFileName);
        return(false);
    }
    ofs << "TEST DE CRS_DESIGN" << "\n";
    ofs << "==================" << "\n\n";

    std::string epsgString4936 = "EPSG:4936";
    std::string epsgString4937 = "EPSG:4937";
    std::string epsgString4258MSL = "EPSG:4258+5782";
    std::string epsgString25830 = "EPSG:25830";
    std::string epsgString25830MSL = "EPSG:25830+5782";
    std::string enuString = "ENU:4937;-4.495021180808;36.756413127079;142.1590";

    std::vector<std::string> sourceCrsIds;
    std::vector<std::map<std::string, std::vector<double> > > sourcePoints;
    std::vector<std::string> targetCrsIds;
    std::vector<std::map<std::string, std::vector<double> > > targetPoints;

    // Operaciones de conversion ETRS89 con ENU con destino en ENU
    sourceCrsIds.push_back(epsgString4937);
    sourcePoints.push_back(coor_4937);
    targetCrsIds.push_back(enuString);
    targetPoints.push_back(coor_enu);

    sourceCrsIds.push_back(epsgString4258MSL);
    sourcePoints.push_back(coor_4258_msl);
    targetCrsIds.push_back(enuString);
    targetPoints.push_back(coor_enu);

    sourceCrsIds.push_back(epsgString4936);
    sourcePoints.push_back(coor_4936);
    targetCrsIds.push_back(enuString);
    targetPoints.push_back(coor_enu);

    sourceCrsIds.push_back(epsgString25830MSL);
    sourcePoints.push_back(coor_25830_msl);
    targetCrsIds.push_back(enuString);
    targetPoints.push_back(coor_enu);

    sourceCrsIds.push_back(epsgString25830);
    sourcePoints.push_back(coor_25830);
    targetCrsIds.push_back(enuString);
    targetPoints.push_back(coor_enu);

    // Operaciones de conversion ETRS89 con ENU con origen en ENU
    sourceCrsIds.push_back(enuString);
    sourcePoints.push_back(coor_enu);
    targetCrsIds.push_back(epsgString4937);
    targetPoints.push_back(coor_4937);

    sourceCrsIds.push_back(enuString);
    sourcePoints.push_back(coor_enu);
    targetCrsIds.push_back(epsgString4258MSL);
    targetPoints.push_back(coor_4258_msl);

    sourceCrsIds.push_back(enuString);
    sourcePoints.push_back(coor_enu);
    targetCrsIds.push_back(epsgString4936);
    targetPoints.push_back(coor_4936);

    sourceCrsIds.push_back(enuString);
    sourcePoints.push_back(coor_enu);
    targetCrsIds.push_back(epsgString25830MSL);
    targetPoints.push_back(coor_25830_msl);

    sourceCrsIds.push_back(enuString);
    sourcePoints.push_back(coor_enu);
    targetCrsIds.push_back(epsgString25830);
    targetPoints.push_back(coor_25830);

    std::string strAuxError;
    for (int nt = 0; nt < sourceCrsIds.size(); nt++)
        //for (int nt = 0; nt < 1; nt++)
    {
        std::string sourceCrsId = sourceCrsIds.at(nt);
        std::string targetCrsId = targetCrsIds.at(nt);
        std::map<std::string, std::vector<double> > points = sourcePoints.at(nt);
        if (sourceCrsId == targetCrsId)
        {
            continue;
        }
        if (nt == 4)
        {
            int yo = 1;
            yo++;
        }
        ptrGeoTools->ptrCRSsTools()->crsOperation(sourceCrsId, targetCrsId, points, true);
        if (!strAuxError.empty())
        {
            strError = functionName;
            strError += ("\n" + strAuxError);
            ofs.close();
            return(false);
        }
        int sourceCrsPrecision, targetCrsPrecision;
        int sourceVerticalPrecision, targetVerticalPrecision;
        ptrGeoTools->ptrCRSsTools()->getCRSPrecision(sourceCrsId,
            sourceCrsPrecision,
            sourceVerticalPrecision);
        if (!strAuxError.empty())
        {
            strError = functionName;
            strError += ("\n" + strAuxError);
            ofs.close();
            return(false);
        }
        ptrGeoTools->ptrCRSsTools()->getCRSPrecision(targetCrsId,
            targetCrsPrecision,
            targetVerticalPrecision);
        if (!strAuxError.empty())
        {
            strError = functionName;
            strError += ("\n" + strAuxError);
            ofs.close();
            return(false);
        }
        ofs << "- Test: " << (nt + 1) << "\n";
        ofs << "----------" << "\n";
        ofs << "  - CRS source ....................................:" << sourceCrsId << "\n";
        ofs << "  - CRS target ....................................:" << targetCrsId << "\n";
        ofs << "  Point.Id  Fc.Crs.Source  Sc.Crs.Source  Tc.Crs.Source  Fc.Crs.Target  Sc.Crs.Target  Tc.Crs.Target          Fc.Error          Sc.Error       Tc.Error" << "\n";
        std::string strNoCoordinateValue = "----------";
        for (auto const& x : points)
        {
            std::string pointId = x.first;
            std::vector<double> point = x.second;
            int pointSourceDimension = sourcePoints.at(nt)[pointId].size();
            double fcCrsSource = sourcePoints.at(nt)[pointId][0];
            double scCrsSource = sourcePoints.at(nt)[pointId][1];
            double tcCrsSource;
            if (pointSourceDimension == 3)
                tcCrsSource = sourcePoints.at(nt)[pointId][2];

            int pointTargetDimension = targetPoints.at(nt)[pointId].size();
            //            if(pointTargetDimension<pointSourceDimension)
            //            {
            //                pointTargetDimension=pointSourceDimension;
            //            }
            double fcCrsTarget = targetPoints.at(nt)[pointId][0];
            double scCrsTarget = targetPoints.at(nt)[pointId][1];
            double tcCrsTarget;
            if (pointTargetDimension == 3)
                tcCrsTarget = targetPoints.at(nt)[pointId][2];
            double fcCrsTargetCalc = point[0];
            double scCrsTargetCalc = point[1];
            double tcCrsTargetCalc;
            if (pointSourceDimension == 3
                && pointTargetDimension == 3)
                tcCrsTargetCalc = point[2];
            double fcError = fcCrsTarget - fcCrsTargetCalc;
            double scError = scCrsTarget - scCrsTargetCalc;
            double tcError;
            if (pointSourceDimension == 3
                && pointTargetDimension == 3)
                tcError = tcCrsTarget - tcCrsTargetCalc;
            ofs << std::setw(10) << std::right << pointId;
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(sourceCrsPrecision) << fcCrsSource;
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(sourceCrsPrecision) << scCrsSource;
            if (pointSourceDimension == 3)
                ofs << std::setw(15) << std::right << std::fixed << std::setprecision(sourceVerticalPrecision) << tcCrsSource;
            else
                ofs << std::setw(15) << std::right << " ";
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << fcCrsTarget;
            ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << scCrsTarget;
            if (pointTargetDimension == 3)
                ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetVerticalPrecision) << tcCrsTarget;
            else
                ofs << std::setw(15) << std::right << " ";
            ofs << std::setw(18) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << fcError;
            ofs << std::setw(18) << std::right << std::fixed << std::setprecision(targetCrsPrecision) << scError;
            if (pointSourceDimension == 3
                && pointTargetDimension == 3)
                ofs << std::setw(15) << std::right << std::fixed << std::setprecision(targetVerticalPrecision) << tcError;
            else
                ofs << std::setw(15) << std::right << " ";
            ofs << "\n";
        }
    }

    ofs.close();

    return(true);
}
