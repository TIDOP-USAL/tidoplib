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
#include "tidop/geometry/primitives/Segment.h"
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

// Por ahora límitado a 2D
//BOOST_FIXTURE_TEST_CASE(Crosses_Point3D, CrossesTestFixture)
//{
//    BOOST_TEST(!crosses(point3d1, point3d2));
//}

BOOST_FIXTURE_TEST_CASE(Crosses_PointWithMeasure, CrossesTestFixture)
{
    BOOST_TEST(!crosses(point2dm1, point2dm2));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Point_Segment_Inside, CrossesTestFixture)
{
    Point2d p(5, 0);  // punto medio del segmento (0,0)-(10,0)
    Segment<Point2d> seg(Point2d(0, 0), Point2d(10, 0));

    // Si una geometría está completamente contenida en la otra → NO es crosses.
    BOOST_TEST(!crosses(p, seg));
    BOOST_TEST(!crosses(seg, p));  // simetría
}

BOOST_FIXTURE_TEST_CASE(Crosses_Point_Segment_Endpoint, CrossesTestFixture)
{
    Point2d p(0, 0);  // extremo del segmento
    Segment<Point2d> seg(Point2d(0, 0), Point2d(10, 0));
    BOOST_TEST(!crosses(p, seg));
    BOOST_TEST(!crosses(seg, p));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Point_Segment_Outside, CrossesTestFixture)
{
    Point2d p(5, 1);  // fuera del segmento
    Segment<Point2d> seg(Point2d(0, 0), Point2d(10, 0));
    BOOST_TEST(!crosses(p, seg));
    BOOST_TEST(!crosses(seg, p));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Segment_Crossing, CrossesTestFixture)
{
    // Horizontal (0,0)-(10,0) y vertical (5,-5)-(5,5) se cruzan en (5,0) interior de ambos
    Segment<Point2d> hor(Point2d(0, 0), Point2d(10, 0));
    Segment<Point2d> ver(Point2d(5, -5), Point2d(5, 5));
    BOOST_TEST(crosses(hor, ver));
    BOOST_TEST(crosses(ver, hor));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Segment_CrossingAtEndpoint, CrossesTestFixture)
{
    // Horizontal (0,0)-(10,0) y vertical desde (5,0) a (5,5) → cruce en extremo de vertical
    Segment<Point2d> hor(Point2d(0, 0), Point2d(10, 0));
    Segment<Point2d> ver(Point2d(5, 0), Point2d(5, 5));
    BOOST_TEST(!crosses(hor, ver));
    BOOST_TEST(!crosses(ver, hor));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Segment_CrossingAtBothEndpoints, CrossesTestFixture)
{
    // Dos segmentos que comparten un extremo (colineales)
    Segment<Point2d> seg1(Point2d(0, 0), Point2d(5, 0));
    Segment<Point2d> seg2(Point2d(5, 0), Point2d(10, 0));
    BOOST_TEST(!crosses(seg1, seg2));
    BOOST_TEST(!crosses(seg2, seg1));
    
    // Dos segmentos que comparten un extremo (no colineales)
    Segment<Point2d> seg3(Point2d(5, 0), Point2d(10, 1));
    BOOST_TEST(!crosses(seg1, seg3));
    BOOST_TEST(!crosses(seg3, seg1));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Segment_ColinearOverlap, CrossesTestFixture)
{
    // Segmentos colineales superpuestos → intersección es un segmento, no un punto
    Segment<Point2d> seg1(Point2d(0, 0), Point2d(10, 0));
    Segment<Point2d> seg2(Point2d(2, 0), Point2d(8, 0));
    BOOST_TEST(!crosses(seg1, seg2));
    BOOST_TEST(!crosses(seg2, seg1));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Segment_ParallelNoIntersect, CrossesTestFixture)
{
    Segment<Point2d> seg1(Point2d(0, 0), Point2d(10, 0));
    Segment<Point2d> seg2(Point2d(0, 1), Point2d(10, 1));
    BOOST_TEST(!crosses(seg1, seg2));
    BOOST_TEST(!crosses(seg2, seg1));
}

BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Segment_NoIntersect, CrossesTestFixture)
{
    Segment<Point2d> seg1(Point2d(0, 0), Point2d(10, 0));
    Segment<Point2d> seg2(Point2d(12, 0), Point2d(20, 0)); // colineales pero separados
    BOOST_TEST(!crosses(seg1, seg2));
    BOOST_TEST(!crosses(seg2, seg1));
}

// Por ahora límitado a 2D
//BOOST_FIXTURE_TEST_CASE(Crosses_Segment3D, CrossesTestFixture)
//{
//    // Segmentos 3D que se cruzan en un punto interior
//    Point3d p1(0, 0, 0), p2(10, 0, 0);
//    Point3d q1(5, -5, 0), q2(5, 5, 0);
//    Segment<Point3d> seg1(p1, p2);
//    Segment<Point3d> seg2(q1, q2);
//    BOOST_TEST(crosses(seg1, seg2));
//    BOOST_TEST(crosses(seg2, seg1));
//}

BOOST_FIXTURE_TEST_CASE(Crosses_Segment_WithMeasure, CrossesTestFixture)
{
    // Segmentos con medida (se ignora)
    Point2dm p1(0, 0, 10), p2(10, 0, 20);
    Point2dm q1(5, -5, 30), q2(5, 5, 40);
    Segment<Point2dm> seg1(p1, p2);
    Segment<Point2dm> seg2(q1, q2);
    BOOST_TEST(crosses(seg1, seg2));
    BOOST_TEST(crosses(seg2, seg1));
}
BOOST_AUTO_TEST_SUITE_END()