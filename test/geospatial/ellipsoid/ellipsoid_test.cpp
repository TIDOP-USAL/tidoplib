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
 
#define BOOST_TEST_MODULE Tidop geospatial Ellipsoid test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/util.h>
#include <tidop/geospatial/ellipsoid.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(EllipsoidTestSuite)

struct EllipsoidTest
{
    EllipsoidTest()
    {

    }

    ~EllipsoidTest()
    {
        if (wgs84) {
            delete wgs84;
            wgs84 = nullptr;
        }
        if (ed50) {
            delete ed50;
            ed50 = nullptr;
        }
        if (sphere) {
            delete sphere;
            sphere = nullptr;
        }
    }

    virtual void setup()
    {
        wgs84 = new Ellipsoid("WGS84", 6378137.0, 298.257);
        ed50 = new Ellipsoid("ED50", 6378388.0, 297);
        sphere = new Ellipsoid("sphere", 6370997., 0.);
    }

    virtual void teardown()
    {

    }

    Ellipsoid *wgs84;
    Ellipsoid *ed50;
    Ellipsoid *sphere;
};

BOOST_FIXTURE_TEST_CASE(a, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(6378137.0, wgs84->semiMajorAxis(), 0.01);
    BOOST_CHECK_CLOSE(6378388.0, ed50->semiMajorAxis(), 0.01);
    BOOST_CHECK_EQUAL(6370997., sphere->semiMajorAxis());
}

BOOST_FIXTURE_TEST_CASE(b, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(6356752.3142451793, wgs84->semiMinorAxis(), 0.01);
    BOOST_CHECK_CLOSE(6356911.946127946, ed50->semiMinorAxis(), 0.01);
    BOOST_CHECK_EQUAL(6370997., sphere->semiMinorAxis());
}

BOOST_FIXTURE_TEST_CASE(flattening, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(0.0033528106647475126, wgs84->flattening(), 0.01);
    BOOST_CHECK_CLOSE(0.0033670033670034, ed50->flattening(), 0.01);
    BOOST_CHECK_EQUAL(0., sphere->flattening());
}

BOOST_FIXTURE_TEST_CASE(inverse_flattening, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(298.257222101, wgs84->inverseFlattening(), 0.01);
    BOOST_CHECK_CLOSE(297., ed50->inverseFlattening(), 0.01);
    BOOST_CHECK_EQUAL(0., sphere->inverseFlattening());
}

BOOST_FIXTURE_TEST_CASE(second_flattening, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(0.0033640923510633012, wgs84->secondFlattening(), 0.01);
    BOOST_CHECK_CLOSE(0.0033783783783783213, ed50->secondFlattening(), 0.01);
    BOOST_CHECK_EQUAL(0., sphere->secondFlattening());
}

BOOST_FIXTURE_TEST_CASE(third_flattening, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(0.001679221647182138, wgs84->thirdFlattening(), 0.01);
    BOOST_CHECK_CLOSE(0.001686340640809415, ed50->thirdFlattening(), 0.01);
    BOOST_CHECK_EQUAL(0., sphere->thirdFlattening());
}

BOOST_FIXTURE_TEST_CASE(eccentricity, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(0.081819221455524407, wgs84->eccentricity(), 0.01);
    BOOST_CHECK_CLOSE(0.081991889979029189, ed50->eccentricity(), 0.01);
    BOOST_CHECK_EQUAL(0., sphere->eccentricity());
}

BOOST_FIXTURE_TEST_CASE(second_eccentricity, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(0.082094468872592891, wgs84->secondEccentricity(), 0.01);
    BOOST_CHECK_CLOSE(0.08226888960733672, ed50->secondEccentricity(), 0.01);
    BOOST_CHECK_EQUAL(0., sphere->secondEccentricity());
}

BOOST_FIXTURE_TEST_CASE(third_eccentricity, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(0.057951995861245623, wgs84->thirdEccentricity(), 0.01);
    BOOST_CHECK_CLOSE(0.058074707839139428, ed50->thirdEccentricity(), 0.01);
    BOOST_CHECK_EQUAL(0., sphere->thirdEccentricity());
}

BOOST_FIXTURE_TEST_CASE(geocentric_radius, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(6378137.0, wgs84->geocentricRadius(0.), 0.01);
    BOOST_CHECK_CLOSE(6356752.3142451793, wgs84->geocentricRadius(90.), 0.01);

    BOOST_CHECK_CLOSE(6378388.0, ed50->geocentricRadius(0.), 0.01);
    BOOST_CHECK_CLOSE(6356911.946127946, ed50->geocentricRadius(90.), 0.01);

    BOOST_CHECK_CLOSE(6370997., sphere->geocentricRadius(0.), 0.01);
    BOOST_CHECK_CLOSE(6370997., sphere->geocentricRadius(90.), 0.01);
}

BOOST_FIXTURE_TEST_CASE(prime_vertical, EllipsoidTest)
{
    Degrees<double> lat(40, 39, 3.6396);
    double N = ed50->primeVertical(lat.value());

    BOOST_CHECK_CLOSE(6387506.3307531672, N, 0.001);
}

BOOST_FIXTURE_TEST_CASE(meridional_radius_of_curvature, EllipsoidTest)
{
    Degrees<double> lat(40, 39, 3.6396);
    double M = ed50->meridionalRadiusOfCurvature(lat.value());

    BOOST_CHECK_CLOSE(6362718.1572734, M, 0.001);
}

BOOST_FIXTURE_TEST_CASE(authalic_radius, EllipsoidTest)
{
    BOOST_CHECK_CLOSE(6371007.2, wgs84->authalicRadius(), 0.001);
}

BOOST_FIXTURE_TEST_CASE(geodetic_to_geocentric_latitude, EllipsoidTest)
{
    Degrees<double> lat(40, 39, 3.6396);
    Degrees<double> geocentric_latitude(wgs84->geodeticToGeocentricLatitude(lat.value()));

    BOOST_CHECK_EQUAL(40, geocentric_latitude.degrees());
    BOOST_CHECK_EQUAL(27, geocentric_latitude.minutes());
    BOOST_CHECK_CLOSE(39.2299049, geocentric_latitude.seconds(), 0.01);

    geocentric_latitude = wgs84->geodeticToGeocentricLatitude(lat.value(), 100.);

    BOOST_CHECK_EQUAL(40, geocentric_latitude.degrees());
    BOOST_CHECK_EQUAL(27, geocentric_latitude.minutes());
    BOOST_CHECK_CLOSE(39.2406506, geocentric_latitude.seconds(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(geodetic_to_parametric_latitude, EllipsoidTest)
{
    Degrees<double> lat(40, 39, 3.6396);
    Degrees<double> parametric_latitude(wgs84->geodeticToParametricLatitude(lat.value()));

    BOOST_CHECK_EQUAL(40, parametric_latitude.degrees());
    BOOST_CHECK_EQUAL(27, parametric_latitude.minutes());
    BOOST_CHECK_CLOSE(39.2299049, parametric_latitude.seconds(), 0.01);
}


BOOST_AUTO_TEST_SUITE_END()

