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
 
#define BOOST_TEST_MODULE Tidop geospatial CRSsTools test

#include <boost/test/unit_test.hpp>
#include <tidop/geotools/GeoTools.h>
#include <tidop/geotools/CRSsTools.h>
#include "tidop/core/path.h"

#include <gdal.h>
#include <cpl_conv.h>

#include "tidop/core/app.h"
#include "tidop/geometry/entities/point.h"

//#ifdef TL_HAVE_VLD
//#include "vld.h"
//#endif // TL_HAVE_VLD

using namespace tl;

BOOST_AUTO_TEST_SUITE(CRSsToolsTestSuite)

struct CRSsToolsTest
{
    CRSsToolsTest()
      : ptrGeoTools(nullptr),
        crs25830("EPSG:25830"),
        crs4258("EPSG:4258"),
        crs25830_5782("EPSG:25830+5782")
    {
    }

    ~CRSsToolsTest()
    {
    }


    virtual void setup()
    {

        _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & ~_CRTDBG_LEAK_CHECK_DF);

        std::string gdal_data_path(CPLGetConfigOption("TL_GDAL_DATA", ""));
        std::string proj_data_path(CPLGetConfigOption("TL_PROJ_DATA", ""));

        CPLSetConfigOption("GDAL_DATA", gdal_data_path.c_str());
        CPLSetConfigOption("PROJ_LIB", proj_data_path.c_str());

#if GDAL_VERSION_NUM < GDAL_COMPUTE_VERSION(3,7,0)
        const char *proj_data[]{proj_data_path.c_str(), nullptr};
        OSRSetPROJSearchPaths(proj_data);
#endif

        ptrGeoTools = GeoTools::getInstance();
        bool ignoreDeprecatedCRSs = true;
        ptrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);

        coor_4937["1001"] = {-8.380627694 , 43.326214300 , 70.097};
        coor_4937["1003"] = {-1.955086196 , 39.003867259 , 750.786};
        coor_4937["1012"] = {-4.443571654 , 36.684036053 , 117.310};
        coor_4937["1013"] = {2.683869762 , 39.541883089 , 62.554};
        coor_4937["1015"] = {-5.443845659 , 40.913368096 , 861.337};

        coor_4346["1001"] = {4597588.151 , -677324.441 , 4353984.396};
        coor_4346["1003"] = {4960751.135 , -169339.909 , 3993123.187};
        coor_4346["1012"] = {5105718.475 , -396769.583 , 3789401.844};
        coor_4346["1013"] = {4919892.431 , 230628.099 , 4038930.385};
        coor_4346["1015"] = {4805774.310 , -457990.177 , 4155721.672};

        coor_4258_msl = {{-8.380627694 , 43.326214300 , 15.485},
                         {-1.955086196 , 39.003867259 , 698.709},
                         {-4.443571654 , 36.684036053 , 69.568},
                         {2.683869762 , 39.541883089 , 13.618},
                         {-5.443845659 , 40.913368096 , 806.420}};

        coor_25830 = {{63723.324 , 4811118.823 , 70.097},
                      {590477.047 , 4317725.016 , 750.786},
                      {371023.221 , 4060793.167 , 117.310},
                      {988552.725 , 4392368.299 , 62.554},
                      {294190.970 , 4532015.675 , 861.337}};

        coor_25830_msl = {{63723.324 , 4811118.823 , 15.485},
                          {590477.047 , 4317725.016 , 698.709},
                          {371023.221 , 4060793.167 , 69.568},
                          {988552.725 , 4392368.299 , 13.618},
                          {294190.970 , 4532015.675 , 806.420}};

        coor_4230 = {{-8.379095832 , 43.327352788},
                     {-1.953844430 , 39.005072992},
                     {-4.442269164 , 36.685310149},
                     {2.684970898 , 39.543059997},
                     {-5.442477238 , 40.914547851}};

        coor_4230_msl = {{-8.379095832 , 43.327352788 , 15.485},
                         {-1.953844430 , 39.005072992 , 698.709},
                         {-4.442269164 , 36.685310149 , 69.568},
                         {2.684970898 , 39.543059997 , 13.618},
                         {-5.442477238 , 40.914547851 , 806.420}};

        coor_23030 = {{63835.641 , 4811324.773},
                      {590587.107 , 4317932.878},
                      {371136.003 , 4060998.681},
                      {988661.192 , 4392580.200},
                      {294300.534 , 4532222.386}};

        coor_23030_msl = {{63835.641 , 4811324.773 , 15.485},
                          {590587.107 , 4317932.878 , 698.709},
                          {371136.003 , 4060998.681 , 69.568},
                          {988661.192 , 4392580.200 , 13.618},
                          {294300.534 , 4532222.386 , 806.420}};


    }

    virtual void teardown()
    {

    }

    GeoTools *ptrGeoTools;
    std::string crs25830;
    std::string crs4258;
    std::string crs25830_5782;
    std::map<std::string, std::vector<double> > coor_4937, coor_4346;
    std::vector<std::vector<double>> coor_4258_msl, coor_25830, coor_25830_msl;
    std::vector<std::vector<double> > coor_4230, coor_23030;
    std::vector<Point3<double>> coor_4230_msl,  coor_23030_msl;
};

BOOST_FIXTURE_TEST_CASE(crsOperation_coordinates, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();

    tl::Point3d point(281815.044, 4827675.243, 0.);
    crs_tools->crsOperation(crs25830, crs4258, point.x, point.y, point.z);
    BOOST_CHECK_CLOSE(-5.701905, point.x, 0.1);
    BOOST_CHECK_CLOSE(43.570113, point.y, 0.1);
    BOOST_CHECK_CLOSE(0., point.z, 0.1);

    double x = coor_4937["1001"][0];
    double y = coor_4937["1001"][1];
    double z = coor_4937["1001"][2];
    crs_tools->crsOperation("EPSG:4937", "EPSG:4258+5782", x, y, z);

    BOOST_CHECK_CLOSE(-8.380627694, x, 0.1);
    BOOST_CHECK_CLOSE(43.326214300, y, 0.1);
    BOOST_CHECK_CLOSE(15.4850, z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(crsOperation_point, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();

    tl::Point3d point(281815.044, 4827675.243, 0.);
    auto transform_point = crs_tools->crsOperation(crs25830, crs4258, point);
    BOOST_CHECK_CLOSE(-5.701905, transform_point.x, 0.1);
    BOOST_CHECK_CLOSE(43.570113, transform_point.y, 0.1);
    BOOST_CHECK_CLOSE(0., transform_point.z, 0.1);

    point.x = coor_4937["1001"][0];
    point.y = coor_4937["1001"][1];
    point.z = coor_4937["1001"][2];
    transform_point = crs_tools->crsOperation("EPSG:4937", "EPSG:4258+5782", point);

    BOOST_CHECK_CLOSE(-8.380627694, transform_point.x, 0.1);
    BOOST_CHECK_CLOSE(43.326214300, transform_point.y, 0.1);
    BOOST_CHECK_CLOSE(15.4850, transform_point.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(crsOperation_map, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();

    {
        auto points = coor_4937;
        crs_tools->crsOperation("EPSG:4937", "EPSG:4258+5782", points);

        BOOST_CHECK_CLOSE(-8.380627694, points["1001"][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points["1001"][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points["1001"][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points["1003"][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points["1003"][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points["1003"][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points["1012"][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points["1012"][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points["1012"][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points["1013"][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points["1013"][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points["1013"][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points["1015"][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points["1015"][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points["1015"][2], 0.1);

    }

    {
        auto points = coor_4937;
        crs_tools->crsOperation("EPSG:4937", "EPSG:25830", points);

        BOOST_CHECK_CLOSE(63723.3240, points["1001"][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points["1001"][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points["1001"][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points["1003"][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points["1003"][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points["1003"][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points["1012"][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points["1012"][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points["1012"][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points["1013"][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points["1013"][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points["1013"][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points["1015"][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points["1015"][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points["1015"][2], 0.1);
    }

    {
        auto points = coor_4937;
        crs_tools->crsOperation("EPSG:4937", "EPSG:25830", points);

        BOOST_CHECK_CLOSE(63723.3240, points["1001"][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points["1001"][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points["1001"][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points["1003"][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points["1003"][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points["1003"][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points["1012"][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points["1012"][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points["1012"][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points["1013"][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points["1013"][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points["1013"][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points["1015"][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points["1015"][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points["1015"][2], 0.1);
    }
}

BOOST_FIXTURE_TEST_CASE(crsOperation_map_by_point, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();

    {
        auto points = coor_4937;
        crs_tools->crsOperation("EPSG:4937", "EPSG:4258+5782", points, true);

        BOOST_CHECK_CLOSE(-8.380627694, points["1001"][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points["1001"][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points["1001"][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points["1003"][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points["1003"][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points["1003"][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points["1012"][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points["1012"][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points["1012"][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points["1013"][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points["1013"][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points["1013"][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points["1015"][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points["1015"][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points["1015"][2], 0.1);
    }

    {
        auto points = coor_4937;
        crs_tools->crsOperation("EPSG:4937", "EPSG:25830", points, true);

        BOOST_CHECK_CLOSE(63723.3240, points["1001"][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points["1001"][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points["1001"][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points["1003"][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points["1003"][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points["1003"][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points["1012"][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points["1012"][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points["1012"][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points["1013"][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points["1013"][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points["1013"][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points["1015"][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points["1015"][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points["1015"][2], 0.1);
    }

    {
        auto points = coor_4937;
        crs_tools->crsOperation("EPSG:4937", "EPSG:25830", points, true);

        BOOST_CHECK_CLOSE(63723.3240, points["1001"][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points["1001"][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points["1001"][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points["1003"][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points["1003"][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points["1003"][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points["1012"][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points["1012"][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points["1012"][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points["1013"][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points["1013"][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points["1013"][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points["1015"][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points["1015"][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points["1015"][2], 0.1);
    }
}

BOOST_FIXTURE_TEST_CASE(crsOperation_vector, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();

    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:4937", points);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);

    }

    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:25830+5782", points);

        BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);    
    }

    {
        auto points = coor_25830_msl;
        crs_tools->crsOperation("EPSG:25830+5782", "EPSG:4258+5782", points);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);
        
        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);
                      
    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:4937", points);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);
    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:23030+5782", points);

        BOOST_CHECK_CLOSE(63835.6410, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811324.7730, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590587.1070, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317932.8780, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371136.0030, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060998.6810, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988661.1920, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392580.2000, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294300.5340, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532222.3860, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);    
    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:4937", points);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);
    }

    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:25830+5782", points);

        BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);
    }

    {
        auto points = coor_25830_msl;
        crs_tools->crsOperation("EPSG:25830+5782", "EPSG:4258+5782", points);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);

    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:4258+5782", points);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);
    }
       
    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:25830", points);

        BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);

    }

}


BOOST_FIXTURE_TEST_CASE(crsOperation_vector_by_point, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();

    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:4937", points, true);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);

    }

    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:25830+5782", points, true);

        BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);
    }

    {
        auto points = coor_25830_msl;
        crs_tools->crsOperation("EPSG:25830+5782", "EPSG:4258+5782", points, true);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);

    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:4937", points, true);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);
    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:23030+5782", points, true);

        BOOST_CHECK_CLOSE(63835.6410, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811324.7730, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590587.1070, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317932.8780, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371136.0030, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060998.6810, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988661.1920, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392580.2000, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294300.5340, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532222.3860, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);
    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:4937", points, true);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);
    }

    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:25830+5782", points, true);

        BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);
    }

    {
        auto points = coor_25830_msl;
        crs_tools->crsOperation("EPSG:25830+5782", "EPSG:4258+5782", points, true);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);

    }

    {
        auto points = coor_25830;
        crs_tools->crsOperation("EPSG:25830", "EPSG:4258+5782", points, true);

        BOOST_CHECK_CLOSE(-8.380627694, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(43.326214300, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(15.4850, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(-1.955086196, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(39.003867259, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(698.7090, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(-4.443571654, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(36.684036053, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(69.5680, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(2.683869762, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(39.541883089, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(13.6180, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(-5.443845659, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(40.913368096, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(806.4200, points[4][2], 0.1);
    }

    {
        auto points = coor_4258_msl;
        crs_tools->crsOperation("EPSG:4258+5782", "EPSG:25830", points, true);

        BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
        BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);
        BOOST_CHECK_CLOSE(70.0970, points[0][2], 0.1);

        BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
        BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);
        BOOST_CHECK_CLOSE(750.7860, points[1][2], 0.1);

        BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
        BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);
        BOOST_CHECK_CLOSE(117.3100, points[2][2], 0.1);

        BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
        BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);
        BOOST_CHECK_CLOSE(62.5540, points[3][2], 0.1);

        BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
        BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
        BOOST_CHECK_CLOSE(861.3370, points[4][2], 0.1);

    }

}

BOOST_FIXTURE_TEST_CASE(crsOperation_points, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();
    auto points = crs_tools->crsOperation("EPSG:23030+5782", "EPSG:25830", coor_23030_msl);

    BOOST_CHECK_CLOSE(63723.3240, points[0].x, 0.1);
    BOOST_CHECK_CLOSE(4811118.8230, points[0].y, 0.1);
    BOOST_CHECK_CLOSE(70.0970, points[0].z, 0.1);

    BOOST_CHECK_CLOSE(590477.0470, points[1].x, 0.1);
    BOOST_CHECK_CLOSE(4317725.0160, points[1].y, 0.1);
    BOOST_CHECK_CLOSE(750.7860, points[1].z, 0.1);

    BOOST_CHECK_CLOSE(371023.2210, points[2].x, 0.1);
    BOOST_CHECK_CLOSE(4060793.1670, points[2].y, 0.1);
    BOOST_CHECK_CLOSE(117.3100, points[2].z, 0.1);

    BOOST_CHECK_CLOSE(988552.7250, points[3].x, 0.1);
    BOOST_CHECK_CLOSE(4392368.2990, points[3].y, 0.1);
    BOOST_CHECK_CLOSE(62.5540, points[3].z, 0.1);

    BOOST_CHECK_CLOSE(294190.9700, points[4].x, 0.1);
    BOOST_CHECK_CLOSE(4532015.6750, points[4].y, 0.1);
    BOOST_CHECK_CLOSE(861.3370, points[4].z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(crsOperation_points_by_point, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();
    auto points = crs_tools->crsOperation("EPSG:23030+5782", "EPSG:25830", coor_23030_msl, true);

    BOOST_CHECK_CLOSE(63723.3240, points[0].x, 0.1);
    BOOST_CHECK_CLOSE(4811118.8230, points[0].y, 0.1);
    BOOST_CHECK_CLOSE(70.0970, points[0].z, 0.1);

    BOOST_CHECK_CLOSE(590477.0470, points[1].x, 0.1);
    BOOST_CHECK_CLOSE(4317725.0160, points[1].y, 0.1);
    BOOST_CHECK_CLOSE(750.7860, points[1].z, 0.1);

    BOOST_CHECK_CLOSE(371023.2210, points[2].x, 0.1);
    BOOST_CHECK_CLOSE(4060793.1670, points[2].y, 0.1);
    BOOST_CHECK_CLOSE(117.3100, points[2].z, 0.1);

    BOOST_CHECK_CLOSE(988552.7250, points[3].x, 0.1);
    BOOST_CHECK_CLOSE(4392368.2990, points[3].y, 0.1);
    BOOST_CHECK_CLOSE(62.5540, points[3].z, 0.1);

    BOOST_CHECK_CLOSE(294190.9700, points[4].x, 0.1);
    BOOST_CHECK_CLOSE(4532015.6750, points[4].y, 0.1);
    BOOST_CHECK_CLOSE(861.3370, points[4].z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(crsOperation_2d_points, CRSsToolsTest)
{
    auto points = coor_4230;
    ptrGeoTools->ptrCRSsTools()->crsOperation("EPSG:4230", "EPSG:25830", points);

    BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
    BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);

    BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
    BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);

    BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
    BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);

    BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
    BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);

    BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
    BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
}

BOOST_FIXTURE_TEST_CASE(crsOperation_2d_points_by_point, CRSsToolsTest)
{
    auto points = coor_4230;
    ptrGeoTools->ptrCRSsTools()->crsOperation("EPSG:4230", "EPSG:25830", points, true);

    BOOST_CHECK_CLOSE(63723.3240, points[0][0], 0.1);
    BOOST_CHECK_CLOSE(4811118.8230, points[0][1], 0.1);

    BOOST_CHECK_CLOSE(590477.0470, points[1][0], 0.1);
    BOOST_CHECK_CLOSE(4317725.0160, points[1][1], 0.1);

    BOOST_CHECK_CLOSE(371023.2210, points[2][0], 0.1);
    BOOST_CHECK_CLOSE(4060793.1670, points[2][1], 0.1);

    BOOST_CHECK_CLOSE(988552.7250, points[3][0], 0.1);
    BOOST_CHECK_CLOSE(4392368.2990, points[3][1], 0.1);

    BOOST_CHECK_CLOSE(294190.9700, points[4][0], 0.1);
    BOOST_CHECK_CLOSE(4532015.6750, points[4][1], 0.1);
}

BOOST_FIXTURE_TEST_CASE(getCRSEnu, CRSsToolsTest)
{
    auto crs_tools = ptrGeoTools->ptrCRSsTools();

    double tcElip = 142.1590;
    double tcHOrth = 94.2172;
    tl::Point3d point(-4.495021180808, 36.756413127079, tcElip);
    crs_tools->crsOperation(crs4258, crs25830, point.x, point.y, point.z);

    std::string crsEnuFrom25830 = crs_tools->getCRSEnu(crs25830, point.x, point.y, point.z);
    BOOST_CHECK_EQUAL("ENU:4258;-4.495021181;36.756413127;142.1590", crsEnuFrom25830);

    std::string crsEnuFrom25830_5782 = crs_tools->getCRSEnu(crs25830_5782, point.x, point.y, tcHOrth);
    BOOST_CHECK_EQUAL("ENU:4258;-4.495021181;36.756413127;142.1591", crsEnuFrom25830_5782);

    double fc4258FromEnuHElip = 0.;
    double sc4258FromEnuHElip = 0.;
    double tc4258FromEnuHElip = 0.;
    crs_tools->crsOperation(crsEnuFrom25830, crs4258,
                            fc4258FromEnuHElip, sc4258FromEnuHElip, tc4258FromEnuHElip);
    BOOST_CHECK_CLOSE(-4.495021181, fc4258FromEnuHElip, 0.01);
    BOOST_CHECK_CLOSE(36.756413127, sc4258FromEnuHElip, 0.01);
    BOOST_CHECK_CLOSE(142.1590, tc4258FromEnuHElip, 0.01);

    double fc4258FromEnuHOrth = 0.;
    double sc4258FromEnuHOrth = 0.;
    double tc4258FromEnuHOrth = 0.;
    crs_tools->crsOperation(crsEnuFrom25830_5782, crs4258,
                            fc4258FromEnuHOrth, sc4258FromEnuHOrth, tc4258FromEnuHOrth);

    BOOST_CHECK_CLOSE(-4.495021181, fc4258FromEnuHOrth, 0.01);
    BOOST_CHECK_CLOSE(36.756413127, sc4258FromEnuHOrth, 0.01);
    BOOST_CHECK_CLOSE(142.1591, tc4258FromEnuHOrth, 0.01);

}

BOOST_FIXTURE_TEST_CASE(getCRSIdEllipsoidHeightsForPDAL, CRSsToolsTest)
{
    std::string crsIdEllipsoidHeightsForPDAL = ptrGeoTools->ptrCRSsTools()->getCRSIdEllipsoidHeightsForPDAL(crs25830);
    BOOST_CHECK_EQUAL("EPSG:25830+4258", crsIdEllipsoidHeightsForPDAL);
}

BOOST_FIXTURE_TEST_CASE(getCRSsInfo, CRSsToolsTest)
{
    auto mapCRSsInfo = ptrGeoTools->ptrCRSsTools()->getCRSsInfo();

    {
        auto &epsg_25830_info = mapCRSsInfo["EPSG:25830"];
        auto &area_name = epsg_25830_info.area_name;
        BOOST_CHECK_EQUAL(area_name, "Europe between 6°W and 0°W: Faroe Islands offshore; Ireland - offshore; Jan Mayen - offshore; Norway including Svalbard - offshore; Spain - onshore and offshore.");
        auto &auth_name = epsg_25830_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_25830_info.code;
        BOOST_CHECK_EQUAL(code, "25830");
        auto deprecated = epsg_25830_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_25830_info.name;
        BOOST_CHECK_EQUAL(name, "ETRS89 / UTM zone 30N");
        auto &projection_method_name = epsg_25830_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "Transverse Mercator");
        auto &type = epsg_25830_info.type;
        BOOST_CHECK_EQUAL(type, "Projected Coordinate Reference System");
    }

    {
        auto &epsg_4258_info = mapCRSsInfo["EPSG:4258"];
        auto &area_name = epsg_4258_info.area_name;
        BOOST_CHECK_EQUAL(area_name, "Europe - onshore and offshore: Albania; Andorra; Austria; Belgium; Bosnia and Herzegovina; Bulgaria; Croatia; Cyprus; Czechia; Denmark; Estonia; Faroe Islands; Finland; France; Germany; Gibraltar; Greece; Hungary; Ireland; Italy; Kosovo; Latvia; Liechtenstein; Lithuania; Luxembourg; Malta; Moldova; Monaco; Montenegro; Netherlands; North Macedonia; Norway including Svalbard and Jan Mayen; Poland; Portugal; Romania; San Marino; Serbia; Slovakia; Slovenia; Spain; Sweden; Switzerland; United Kingdom (UK) including Channel Islands and Isle of Man; Vatican City State.");
        auto &auth_name = epsg_4258_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_4258_info.code;
        BOOST_CHECK_EQUAL(code, "4258");
        auto deprecated = epsg_4258_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_4258_info.name;
        BOOST_CHECK_EQUAL(name, "ETRS89");
        auto &projection_method_name = epsg_4258_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "");
        auto &type = epsg_4258_info.type;
        BOOST_CHECK_EQUAL(type, "Geographic 2D Coordinate Reference System");
    }

    {
        auto &epsg_20000_info = mapCRSsInfo["EPSG:20000"];
        auto &area_name = epsg_20000_info.area_name;
        BOOST_CHECK_EQUAL(area_name, "Arctic (Norway (Svalbard) onshore and offshore) - between 81°10'N and 76°10'N.");
        auto &auth_name = epsg_20000_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_20000_info.code;
        BOOST_CHECK_EQUAL(code, "20000");
        auto deprecated = epsg_20000_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_20000_info.name;
        BOOST_CHECK_EQUAL(name, "SVD2006 height");
        auto &projection_method_name = epsg_20000_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "");
        auto &type = epsg_20000_info.type;
        BOOST_CHECK_EQUAL(type, "Vertical Coordinate Reference System");
    }

    {
        auto epsg_20037_info = mapCRSsInfo["EPSG:20037"];
        auto &auth_name = epsg_20037_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_20037_info.code;
        BOOST_CHECK_EQUAL(code, "20037");
        auto deprecated = epsg_20037_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_20037_info.name;
        BOOST_CHECK_EQUAL(name, "NAD83(CSRS)v4 + CGVD2013a(2002) height");
        auto &projection_method_name = epsg_20037_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "");
        auto &type = epsg_20037_info.type;
        BOOST_CHECK_EQUAL(type, "Compound Coordinate Reference System");
    }

    {
        auto epsg_20041_info = mapCRSsInfo["EPSG:20041"];
        auto &auth_name = epsg_20041_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_20041_info.code;
        BOOST_CHECK_EQUAL(code, "20041");
        auto deprecated = epsg_20041_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_20041_info.name;
        BOOST_CHECK_EQUAL(name, "SIRGAS-Chile 2021");
        auto &projection_method_name = epsg_20041_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "");
        auto &type = epsg_20041_info.type;
        BOOST_CHECK_EQUAL(type, "Geographic 2D Coordinate Reference System");
    }

    {
        auto epsg_3822_info = mapCRSsInfo["EPSG:3822"];
        auto &auth_name = epsg_3822_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_3822_info.code;
        BOOST_CHECK_EQUAL(code, "3822");
        auto deprecated = epsg_3822_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_3822_info.name;
        BOOST_CHECK_EQUAL(name, "TWD97");
        auto &projection_method_name = epsg_3822_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "");
        auto &type = epsg_3822_info.type;
        BOOST_CHECK_EQUAL(type, "Geocentric Coordinate Reference System");
    }

    {
        auto epsg_3823_info = mapCRSsInfo["EPSG:3823"];
        auto &area_name = epsg_3823_info.area_name;
        BOOST_CHECK_EQUAL(area_name, "Taiwan, Republic of China - onshore and offshore - Taiwan Island, Penghu (Pescadores) Islands.");
        auto &auth_name = epsg_3823_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_3823_info.code;
        BOOST_CHECK_EQUAL(code, "3823");
        auto deprecated = epsg_3823_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_3823_info.name;
        BOOST_CHECK_EQUAL(name, "TWD97");
        auto &projection_method_name = epsg_3823_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "");
        auto &type = epsg_3823_info.type;
        BOOST_CHECK_EQUAL(type, "Geographic 3D Coordinate Reference System");
    }
}

BOOST_FIXTURE_TEST_CASE(getCRSPrecision, CRSsToolsTest)
{
    int crsPrecision = 0;
    int crsVerticalPrecision = 0;
    ptrGeoTools->ptrCRSsTools()->getCRSPrecision(crs25830_5782, crsPrecision, crsVerticalPrecision);
    BOOST_CHECK_EQUAL(4, crsPrecision);
    BOOST_CHECK_EQUAL(4, crsVerticalPrecision);

    ptrGeoTools->ptrCRSsTools()->getCRSPrecision(crs25830, crsPrecision, crsVerticalPrecision);
    BOOST_CHECK_EQUAL(4, crsPrecision);
    BOOST_CHECK_EQUAL(4, crsVerticalPrecision);

    ptrGeoTools->ptrCRSsTools()->getCRSPrecision(crs4258, crsPrecision, crsVerticalPrecision);
    BOOST_CHECK_EQUAL(9, crsPrecision);
    BOOST_CHECK_EQUAL(4, crsVerticalPrecision);
}

BOOST_FIXTURE_TEST_CASE(getCRSsFor2dApplications, CRSsToolsTest)
{
    auto crssFor2dApplications = ptrGeoTools->ptrCRSsTools()->getCRSsFor2dApplications();

    {
        auto epsg_25830_info = crssFor2dApplications["EPSG:25830"];
        auto &auth_name = epsg_25830_info.auth_name;
        BOOST_CHECK_EQUAL(auth_name, "EPSG");
        auto &code = epsg_25830_info.code;
        BOOST_CHECK_EQUAL(code, "25830");
        auto deprecated = epsg_25830_info.deprecated;
        BOOST_CHECK_EQUAL(deprecated, false);
        auto &name = epsg_25830_info.name;
        BOOST_CHECK_EQUAL(name, "ETRS89 / UTM zone 30N");
        auto &projection_method_name = epsg_25830_info.projection_method_name;
        BOOST_CHECK_EQUAL(projection_method_name, "Transverse Mercator");
        auto &type = epsg_25830_info.type;
        BOOST_CHECK_EQUAL(type, "Projected Coordinate Reference System");
    }
}

BOOST_FIXTURE_TEST_CASE(getCRSsVertical, CRSsToolsTest)
{

    std::string crsId_1 = "EPSG:25830";
    std::map<std::string, CRSInfo> crssVertical_1;
    ptrGeoTools->ptrCRSsTools()->getCRSsVertical(crsId_1, crssVertical_1);
    
    auto &epsg5782 = crssVertical_1["EPSG:5782"];
    BOOST_CHECK_EQUAL(epsg5782.name, "Alicante height");

    auto &epsg9392 = crssVertical_1["EPSG:9392"];
    BOOST_CHECK_EQUAL(epsg9392.name, "Mallorca height");

    auto &epsg9393 = crssVertical_1["EPSG:9393"];
    BOOST_CHECK_EQUAL(epsg9393.name, "Menorca height");

}

BOOST_FIXTURE_TEST_CASE(isCRSEnu, CRSsToolsTest)
{
    BOOST_CHECK(!ptrGeoTools->ptrCRSsTools()->getIsCRSEnu("EPSG:25830"));
    BOOST_CHECK(ptrGeoTools->ptrCRSsTools()->getIsCRSEnu("ENU:4937;-4.495021180808;36.756413127079;142.1590"));
}

BOOST_FIXTURE_TEST_CASE(isCRSGeographic, CRSsToolsTest)
{
    BOOST_CHECK(!ptrGeoTools->ptrCRSsTools()->getIsCRSGeographic("EPSG:25830"));
    BOOST_CHECK(!ptrGeoTools->ptrCRSsTools()->getIsCRSGeographic("ENU:4937;-4.495021180808;36.756413127079;142.1590"));
    BOOST_CHECK(ptrGeoTools->ptrCRSsTools()->getIsCRSGeographic("EPSG:4258"));
}

BOOST_FIXTURE_TEST_CASE(isCRSValid, CRSsToolsTest)
{
    BOOST_CHECK(ptrGeoTools->ptrCRSsTools()->getIsCRSValid("EPSG:25830"));
    BOOST_CHECK(!ptrGeoTools->ptrCRSsTools()->getIsCRSValid("ENU:4937;-4.495021180808;36.756413127079;142.1590"));
    BOOST_CHECK(ptrGeoTools->ptrCRSsTools()->getIsCRSValid("EPSG:4258"));
    BOOST_CHECK(ptrGeoTools->ptrCRSsTools()->getIsCRSValid("EPSG:25830+5782"));
    BOOST_CHECK(!ptrGeoTools->ptrCRSsTools()->getIsCRSValid("25830"));
    BOOST_CHECK(!ptrGeoTools->ptrCRSsTools()->getIsCRSValid("EPSG:999999"));
}

BOOST_AUTO_TEST_SUITE_END()


//BOOST_AUTO_TEST_SUITE(EcefToEnuTestSuite)
//
//struct EcefToEnuTest
//{
//
//    EcefToEnuTest()
//    {
//    }
//
//    ~EcefToEnuTest()
//    {
//    }
//
//
//    virtual void setup()
//    {
//        coordinates_utm = {
//            {271931.214, 4338432.933, 294.395},
//            {271910.232, 4338364.968, 294.113},
//            {271918.286, 4338297.121, 294.199},
//            {272143.607, 4338498.725, 295.197},
//            {272152.641, 4338399.580, 294.870},
//            {271952.571, 4338365.355, 294.336},
//            {272098.641, 4338415.037, 294.745}
//        };
//
//        geocentric_center_coordinates = {4928049.918, -486570.363, 4006715.986};
//
//        coordinates_geocentric = {
//            {4928019.9290454974, -486652.93056170165, 4006742.4187911944},
//            {4928060.9183000689, -486676.0619335298, 4006689.1044401266},
//            {4928104.4371891106, -486670.28738667775, 4006636.7686339747},
//            {4927996.0070305113, -486439.197751494655, 4006798.6939182375},
//            {4928059.0346600218, -486433.45029953803, 4006721.8687072434},
//            {4928064.2300744383, -486633.88164300204, 4006690.5004094145},
//            {4928044.8706753952, -486486.7327698741, 4006732.5493164747}
//        };
//
//        coordinates_enu = {
//            {-85.114472, 34.218334, -0.155280},
//            {-104.106427, -34.315054, -0.437561},
//            {-94.083784, -101.885868, -0.352129},
//            {125.233590, 106.148289, 0.645265},
//            {137.146142, 7.327173, 0.318902},
//            {-61.804839, -32.696674, -0.214004},
//            {82.729790, 21.203800, 0.194807}
//        };
//
//        epsg_geographic = std::make_shared<Crs>("EPSG:4326");
//        epsg_geocentric = std::make_shared<Crs>("EPSG:4978");
//        epsg_utm = std::make_shared<Crs>("EPSG:25830");
//    }
//
//    virtual void teardown()
//    {
//
//    }
//
//    std::vector<Point3d> coordinates_utm;
//    std::vector<Point3d> coordinates_geocentric;
//    Point3d geocentric_center_coordinates;
//    std::vector<Point3d> coordinates_enu;
//    std::shared_ptr<Crs> epsg_geographic;
//    std::shared_ptr<Crs> epsg_geocentric;
//    std::shared_ptr<Crs> epsg_utm;
//    
//
//};
//
//
//BOOST_FIXTURE_TEST_CASE(transform_utm_to_enu, EcefToEnuTest)
//{
//    std::vector<Point3d>  _coordinates_geocentric;
//
//    tl::CrsTransform utm_to_geocentric(epsg_utm, epsg_geocentric);
//    for (size_t i = 0; i < coordinates_utm.size(); i++) {
//        auto point_geocentric = utm_to_geocentric.transform(coordinates_utm[i]);
//        BOOST_CHECK_CLOSE(coordinates_geocentric[i].x, point_geocentric.x, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_geocentric[i].y, point_geocentric.y, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_geocentric[i].z, point_geocentric.z, 0.1);
//        _coordinates_geocentric.push_back(point_geocentric);
//    }
//
//    /// Cálculo del centro
//    tl::Point3<double> ecef_center;
//    for (const auto &ecef : _coordinates_geocentric) {
//        ecef_center += ecef / static_cast<double>(_coordinates_geocentric.size());
//    }
//
//    tl::CrsTransform crs_transfom_geocentric_to_geographic(epsg_geocentric, epsg_geographic);
//    auto lla = crs_transfom_geocentric_to_geographic.transform(ecef_center);
//    auto rotation = tl::rotationEnuToEcef(lla.x, lla.y);
//    auto ecef_to_enu = std::make_shared<tl::EcefToEnu>(ecef_center, rotation);
//
//    for (size_t i = 0; i < _coordinates_geocentric.size(); i++) {
//        auto enu_point = ecef_to_enu->direct(_coordinates_geocentric[i]);
//        BOOST_CHECK_CLOSE(coordinates_enu[i].x, enu_point.x, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_enu[i].y, enu_point.y, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_enu[i].z, enu_point.z, 0.1);
//    }
//
//}
//
//
//BOOST_FIXTURE_TEST_CASE(transform_enu_to_utm, EcefToEnuTest)
//{
//    tl::CrsTransform crs_transfom_geocentric_to_geographic(epsg_geocentric, epsg_geographic);
//    auto lla = crs_transfom_geocentric_to_geographic.transform(geocentric_center_coordinates);
//    auto rotation = tl::rotationEnuToEcef(lla.x, lla.y);
//    auto ecef_to_enu = std::make_shared<tl::EcefToEnu>(geocentric_center_coordinates, rotation);
//
//    std::vector<Point3d>  _coordinates_geocentric;
//    for (size_t i = 0; i < coordinates_enu.size(); i++) {
//        auto ecef_point = ecef_to_enu->inverse(coordinates_enu[i]);
//        BOOST_CHECK_CLOSE(coordinates_geocentric[i].x, ecef_point.x, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_geocentric[i].y, ecef_point.y, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_geocentric[i].z, ecef_point.z, 0.1);
//
//        _coordinates_geocentric.push_back(ecef_point);
//    }
//
//    tl::CrsTransform geocentric_to_utm(epsg_geocentric, epsg_utm);
//    for (size_t i = 0; i < _coordinates_geocentric.size(); i++) {
//        auto point_utm = geocentric_to_utm.transform(_coordinates_geocentric[i]);
//        BOOST_CHECK_CLOSE(coordinates_utm[i].x, point_utm.x, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_utm[i].y, point_utm.y, 0.1);
//        BOOST_CHECK_CLOSE(coordinates_utm[i].z, point_utm.z, 0.1);
//    }
//
//}
//
//
//BOOST_AUTO_TEST_SUITE_END()