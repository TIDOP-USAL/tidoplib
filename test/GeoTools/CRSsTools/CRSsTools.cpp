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
#include "tidop/core/base/path.h"

#include <gdal.h>
#include <cpl_conv.h>

#include "tidop/core/app/app.h"
#include "tidop/geometry/entities/point.h"

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
        tl::Path _path = App::instance().path().parentPath().parentPath();
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

        ptrGeoTools = GeoTools::getInstance();
        bool ignoreDeprecatedCRSs = true;
        ptrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);

        coor_4937["1001"] = {-8.380627694 , 43.326214300 , 70.097};
        coor_4937["1003"] = {-1.955086196 , 39.003867259 , 750.786};
        coor_4937["1012"] = {-4.443571654 , 36.684036053 , 117.310};
        coor_4937["1013"] = {2.683869762 , 39.541883089 , 62.554};
        coor_4937["1015"] = {-5.443845659 , 40.913368096 , 861.337};

    }

    virtual void teardown()
    {

    }

    GeoTools *ptrGeoTools;
    std::string crs25830;
    std::string crs4258;
    std::string crs25830_5782;
    std::map<std::string, std::vector<double> > coor_4937;
};


BOOST_FIXTURE_TEST_CASE(transform, CRSsToolsTest)
{
    tl::Point3d point(281815.044, 4827675.243, 0.);
    ptrGeoTools->ptrCRSsTools()->crsOperation(crs25830, crs4258, point.x, point.y, point.z);
    BOOST_CHECK_CLOSE(-5.701905, point.x, 0.1);
    BOOST_CHECK_CLOSE(43.570113, point.y, 0.1);
    BOOST_CHECK_CLOSE(0., point.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(getCRSEnu, CRSsToolsTest)
{
    double tcElip = 142.1590;
    double tcHOrth = 94.2172;
    tl::Point3d point(-4.495021180808, 36.756413127079, tcElip);
    ptrGeoTools->ptrCRSsTools()->crsOperation(crs4258, crs25830, point.x, point.y, point.z);

    std::string crsEnuFrom25830 = ptrGeoTools->ptrCRSsTools()->getCRSEnu(crs25830, point.x, point.y, point.z);
    BOOST_CHECK_EQUAL("ENU:4258;-4.495021181;36.756413127;142.1590", crsEnuFrom25830);

    std::string crsEnuFrom25830_5782 = ptrGeoTools->ptrCRSsTools()->getCRSEnu(crs25830_5782, point.x, point.y, tcHOrth);
    BOOST_CHECK_EQUAL("ENU:4258;-4.495021181;36.756413127;94.2172", crsEnuFrom25830_5782);

    double fc4258FromEnuHElip = 0.;
    double sc4258FromEnuHElip = 0.;
    double tc4258FromEnuHElip = 0.;
    ptrGeoTools->ptrCRSsTools()->crsOperation(crsEnuFrom25830, crs4258,
                                              fc4258FromEnuHElip, sc4258FromEnuHElip, tc4258FromEnuHElip);
    BOOST_CHECK_CLOSE(-4.495021181, fc4258FromEnuHElip, 0.01);
    BOOST_CHECK_CLOSE(36.756413127, sc4258FromEnuHElip, 0.01);
    BOOST_CHECK_CLOSE(142.1590, tc4258FromEnuHElip, 0.01);

    double fc4258FromEnuHOrth = 0.;
    double sc4258FromEnuHOrth = 0.;
    double tc4258FromEnuHOrth = 0.;
    ptrGeoTools->ptrCRSsTools()->crsOperation(crsEnuFrom25830_5782, crs4258,
                                              fc4258FromEnuHOrth, sc4258FromEnuHOrth, tc4258FromEnuHOrth);

    BOOST_CHECK_CLOSE(-4.495021181, fc4258FromEnuHOrth, 0.01);
    BOOST_CHECK_CLOSE(36.756413127, sc4258FromEnuHOrth, 0.01);
    BOOST_CHECK_CLOSE(94.2172, tc4258FromEnuHOrth, 0.01);
}

// test CRSsVertical

BOOST_FIXTURE_TEST_CASE(getCRSsVertical, CRSsToolsTest)
{
    std::map<std::string, CRSInfo> crssFor2dApplications;
    ptrGeoTools->ptrCRSsTools()->getCRSsFor2dApplications(crssFor2dApplications);
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