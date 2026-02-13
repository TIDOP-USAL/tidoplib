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
 
#define BOOST_TEST_MODULE Tidop algorithms crosses test
#include <boost/test/unit_test.hpp>
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/analysis/Crosses.h"

using namespace tl; 

BOOST_AUTO_TEST_SUITE(CrossesAlgorithmTest)


struct CrossesTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(3.0, 4.0);
        point3d1 = Point3d(1.0, 2.0, 3.0);
        point3d2 = Point3d(4.0, 5.0, 6.0);
        point2dm1 = Point2dm(1.0, 2.0, 10.0);
        point2dm2 = Point2dm(3.0, 4.0, 20.0);
    }

    void teardown()
    {

    }

    Point2d point2d1;
    Point2d point2d2;
    Point3d point3d1;
    Point3d point3d2;
    Point2dm point2dm1;
    Point2dm point2dm2;
};


BOOST_FIXTURE_TEST_CASE(Crosses_Point_Point_Different, CrossesTestFixture)
{
    BOOST_TEST(!crosses(point2d1, point2d2));
    BOOST_TEST(!crosses(point2d2, point2d1)); // Simetría
}

BOOST_FIXTURE_TEST_CASE(Crosses_Point_Point_Same, CrossesTestFixture)
{
    // Un punto no se cruza consigo mismo según OGC
    BOOST_TEST(!crosses(point2d1, point2d1));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Point3D, CrossesTestFixture)
{
    BOOST_TEST(!crosses(point3d1, point3d2));
}

BOOST_FIXTURE_TEST_CASE(Crosses_PointWithMeasure, CrossesTestFixture)
{
    BOOST_TEST(!crosses(point2dm1, point2dm2));
}

BOOST_AUTO_TEST_SUITE_END()