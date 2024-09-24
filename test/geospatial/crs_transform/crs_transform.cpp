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
 
#define BOOST_TEST_MODULE Tidop geospatial crs_transform test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/crstransf.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;

/* CrsTransformTest */

#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ

BOOST_AUTO_TEST_SUITE(CrsTransformTestSuite)

struct CrsTransformTest
{
    CrsTransformTest()
    {
        epsg25830 = std::make_shared<Crs>("EPSG:25830");
        epsg4258 = std::make_shared<Crs>("EPSG:4258");
    }

    ~CrsTransformTest()
    {
    }


    virtual void setup()
    {

    }

    virtual void teardown()
    {

    }

    std::shared_ptr<Crs> epsg25830;
    std::shared_ptr<Crs> epsg4258;
};


BOOST_FIXTURE_TEST_CASE(transform, CrsTransformTest)
{
    CrsTransform trf(epsg25830, epsg4258);
    Point3d pt_utm(281815.044, 4827675.243, 0.);
    Point3d pt_geo;
    trf.transform(pt_utm, pt_geo);
    BOOST_CHECK_CLOSE(-5.701905, pt_geo.x, 0.1);
    BOOST_CHECK_CLOSE(43.570113, pt_geo.y, 0.1);
    BOOST_CHECK_CLOSE(0., pt_geo.z, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(EcefToEnuTestSuite)

struct EcefToEnuTest
{

    EcefToEnuTest()
    {
    }

    ~EcefToEnuTest()
    {
    }


    virtual void setup()
    {
        coordinates_utm = {
            {271931.214, 4338432.933, 294.395},
            {271910.232, 4338364.968, 294.113},
            {271918.286, 4338297.121, 294.199},
            {272143.607, 4338498.725, 295.197},
            {272152.641, 4338399.580, 294.870},
            {271952.571, 4338365.355, 294.336},
            {272098.641, 4338415.037, 294.745}
        };

        geocentric_center_coordinates = {4928049.918, -486570.363, 4006715.986};

        coordinates_geocentric = {
            {4928019.9290454974, -486652.93056170165, 4006742.4187911944},
            {4928060.9183000689, -486676.0619335298, 4006689.1044401266},
            {4928104.4371891106, -486670.28738667775, 4006636.7686339747},
            {4927996.0070305113, -486439.197751494655, 4006798.6939182375},
            {4928059.0346600218, -486433.45029953803, 4006721.8687072434},
            {4928064.2300744383, -486633.88164300204, 4006690.5004094145},
            {4928044.8706753952, -486486.7327698741, 4006732.5493164747}
        };

        coordinates_enu = {
            {-85.114472, 34.218334, -0.155280},
            {-104.106427, -34.315054, -0.437561},
            {-94.083784, -101.885868, -0.352129},
            {125.233590, 106.148289, 0.645265},
            {137.146142, 7.327173, 0.318902},
            {-61.804839, -32.696674, -0.214004},
            {82.729790, 21.203800, 0.194807}
        };

        epsg_geographic = std::make_shared<Crs>("EPSG:4326");
        epsg_geocentric = std::make_shared<Crs>("EPSG:4978");
        epsg_utm = std::make_shared<Crs>("EPSG:25830");
    }

    virtual void teardown()
    {

    }

    std::vector<Point3d> coordinates_utm;
    std::vector<Point3d> coordinates_geocentric;
    Point3d geocentric_center_coordinates;
    std::vector<Point3d> coordinates_enu;
    std::shared_ptr<Crs> epsg_geographic;
    std::shared_ptr<Crs> epsg_geocentric;
    std::shared_ptr<Crs> epsg_utm;
    

};


BOOST_FIXTURE_TEST_CASE(transform_utm_to_enu, EcefToEnuTest)
{
    std::vector<Point3d>  _coordinates_geocentric;

    tl::CrsTransform utm_to_geocentric(epsg_utm, epsg_geocentric);
    for (size_t i = 0; i < coordinates_utm.size(); i++) {
        auto point_geocentric = utm_to_geocentric.transform(coordinates_utm[i]);
        BOOST_CHECK_CLOSE(coordinates_geocentric[i].x, point_geocentric.x, 0.1);
        BOOST_CHECK_CLOSE(coordinates_geocentric[i].y, point_geocentric.y, 0.1);
        BOOST_CHECK_CLOSE(coordinates_geocentric[i].z, point_geocentric.z, 0.1);
        _coordinates_geocentric.push_back(point_geocentric);
    }

    /// Cálculo del centro
    tl::Point3<double> ecef_center;
    for (const auto &ecef : _coordinates_geocentric) {
        ecef_center += ecef / static_cast<double>(_coordinates_geocentric.size());
    }

    tl::CrsTransform crs_transfom_geocentric_to_geographic(epsg_geocentric, epsg_geographic);
    auto lla = crs_transfom_geocentric_to_geographic.transform(ecef_center);
    auto rotation = tl::rotationEnuToEcef(lla.x, lla.y);
    auto ecef_to_enu = std::make_shared<tl::EcefToEnu>(ecef_center, rotation);

    for (size_t i = 0; i < _coordinates_geocentric.size(); i++) {
        auto enu_point = ecef_to_enu->direct(_coordinates_geocentric[i]);
        BOOST_CHECK_CLOSE(coordinates_enu[i].x, enu_point.x, 0.1);
        BOOST_CHECK_CLOSE(coordinates_enu[i].y, enu_point.y, 0.1);
        BOOST_CHECK_CLOSE(coordinates_enu[i].z, enu_point.z, 0.1);
    }

}


BOOST_FIXTURE_TEST_CASE(transform_enu_to_utm, EcefToEnuTest)
{
    tl::CrsTransform crs_transfom_geocentric_to_geographic(epsg_geocentric, epsg_geographic);
    auto lla = crs_transfom_geocentric_to_geographic.transform(geocentric_center_coordinates);
    auto rotation = tl::rotationEnuToEcef(lla.x, lla.y);
    auto ecef_to_enu = std::make_shared<tl::EcefToEnu>(geocentric_center_coordinates, rotation);

    std::vector<Point3d>  _coordinates_geocentric;
    for (size_t i = 0; i < coordinates_enu.size(); i++) {
        auto ecef_point = ecef_to_enu->inverse(coordinates_enu[i]);
        BOOST_CHECK_CLOSE(coordinates_geocentric[i].x, ecef_point.x, 0.1);
        BOOST_CHECK_CLOSE(coordinates_geocentric[i].y, ecef_point.y, 0.1);
        BOOST_CHECK_CLOSE(coordinates_geocentric[i].z, ecef_point.z, 0.1);

        _coordinates_geocentric.push_back(ecef_point);
    }

    tl::CrsTransform geocentric_to_utm(epsg_geocentric, epsg_utm);
    for (size_t i = 0; i < _coordinates_geocentric.size(); i++) {
        auto point_utm = geocentric_to_utm.transform(_coordinates_geocentric[i]);
        BOOST_CHECK_CLOSE(coordinates_utm[i].x, point_utm.x, 0.1);
        BOOST_CHECK_CLOSE(coordinates_utm[i].y, point_utm.y, 0.1);
        BOOST_CHECK_CLOSE(coordinates_utm[i].z, point_utm.z, 0.1);
    }

}


BOOST_AUTO_TEST_SUITE_END()


#endif // TL_HAVE_GDAL && TL_HAVE_PROJ