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
 
#define BOOST_TEST_MODULE Tidop algorithms distance test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/algorithms/measurement/Length.h>
#include <tidop/geometry/primitives/Point.h>
#include <tidop/geometry/primitives/Segment.h>
#include <tidop/geometry/primitives/LineString.h>
#include <tidop/geometry/primitives/Polygon.h>
#include <tidop/geometry/primitives/MultiPoint.h>
#include <tidop/geometry/primitives/MultiLineString.h>
#include <tidop/geometry/primitives/MultiPolygon.h>

using namespace tl; 
using namespace geometry;


BOOST_AUTO_TEST_SUITE(DistanceAlgorithmsTestSuite)


struct DistanceTest
{
    DistanceTest()
    {

    }

    ~DistanceTest()
    {

    }

    void setup()
    {
        pt_int = geometry::Point2i(10, 50);

        pt1 = geometry::Point2d(21.45, 34.67);
        pt2 = geometry::Point2d(234.54, 45.76);

        pt1d = geometry::Point3d(3423.56, 345.78, 34.8);
        pt2d = geometry::Point3d(1234.45, 456.98, 45.9);

        p1 = geometry::Point2d(0.0, 0.0);
        p2 = geometry::Point2d(3.0, 4.0);
        p3 = geometry::Point3d(0.0, 0.0, 0.0);
        p4 = geometry::Point3d(1.0, 1.0, 1.0);

        segment = Segment<geometry::Point2d>(geometry::Point2d(0., 0.), geometry::Point2d(10., 0.));

        line.emplace_back(0.0, 0.0);
        line.emplace_back(5.0, 0.0);
        line.emplace_back(5.0, 5.0);
    }

    void teardown()
    {

    }

    geometry::Point2i pt_int;
    geometry::Point2d pt1;
    geometry::Point2d pt2;
    geometry::Point3d pt1d;
    geometry::Point3d pt2d;
    geometry::Point2d p1;
    geometry::Point2d p2;
    geometry::Point3d p3;
    geometry::Point3d p4;

    Segment<geometry::Point2d> segment;
    LineString<geometry::Point2d> line;
};


// 1. Test Distancia Punto a Punto (2D y 3D)
BOOST_FIXTURE_TEST_CASE(point_to_point_distance, DistanceTest)
{
    BOOST_CHECK_CLOSE(213.378, distance(pt1, pt2), 0.001);
    BOOST_CHECK_CLOSE(213.378, distance(pt2, pt1), 0.001);
    BOOST_CHECK_CLOSE(2191.961, distance(pt1d, pt2d), 0.001);
    BOOST_CHECK_CLOSE(2191.961, distance(pt2d, pt1d), 0.001);
    BOOST_CHECK_CLOSE(5.0, distance(p1, p2), 0.001);
    BOOST_CHECK_CLOSE(1.73205, distance(p3, p4), 0.01);
}

// 2. Test Distancia Punto a Segmento (Casos críticos)
BOOST_FIXTURE_TEST_CASE(point_to_segment_distance, DistanceTest)
{
    // Caso A: El punto proyecta dentro del segmento
    geometry::Point2d pInside(5.0, 5.0);
    BOOST_CHECK_CLOSE(distance(pInside, segment), 5.0, 0.001);

    // Caso B: El punto está antes del inicio (p1)
    geometry::Point2d pBefore(-3.0, 4.0);
    BOOST_CHECK_CLOSE(distance(pBefore, segment), 5.0, 0.001); // Distancia a (0,0)

    // Caso C: El punto está después del final (p2)
    geometry::Point2d pAfter(13.0, 4.0);
    BOOST_CHECK_CLOSE(distance(pAfter, segment), 5.0, 0.001); // Distancia a (10,0)
}

// 3. Test Distancia Punto a LineString
BOOST_FIXTURE_TEST_CASE(point_to_linestring_distance, DistanceTest)
{
    geometry::Point2d p(7.0, 2.0);
    // El punto más cercano está en el segmento (5,0)-(5,5)
    // La distancia ortogonal a ese segmento es |7.0 - 5.0| = 2.0
    BOOST_CHECK_CLOSE(distance(p, line), 2.0, 0.001);
}

// 4. Test Distancia Punto a Polígono (incluyendo huecos)
BOOST_AUTO_TEST_CASE(point_to_polygon_distance)
{
    geometry::Polygon<geometry::Point2d> poly;
    // Cuadrado exterior de 10x10
    poly.outer().emplace_back(0.0, 0.0);
    poly.outer().emplace_back(10.0, 0.0);
    poly.outer().emplace_back(10.0, 10.0);
    poly.outer().emplace_back(0.0, 10.0);

    // Punto fuera del polígono
    geometry::Point2d pOutside(12.0, 5.0);
    BOOST_CHECK_CLOSE(distance(pOutside, poly), 2.0, 0.001);

    // Punto dentro del polígono pero cerca de un hueco
    // Añadimos un hueco central de 2x2
    LinearRing<geometry::Point2d> hole;
    hole.emplace_back(4.0, 4.0);
    hole.emplace_back(6.0, 4.0);
    hole.emplace_back(6.0, 6.0);
    hole.emplace_back(4.0, 6.0);
    poly.addInner(hole);

    geometry::Point2d pNearHole(5.0, 4.5);
    // El punto está a 0.5 de la base del hueco
    BOOST_CHECK_CLOSE(distance(pNearHole, poly), 0.5, 0.001);
}

// 5. Test de Simetría (Tag Dispatching)
BOOST_AUTO_TEST_CASE(distance_symmetry)
{
    geometry::Point2d p(5.0, 5.0);
    Segment<geometry::Point2d> s(geometry::Point2d(0.0, 0.0), geometry::Point2d(10.0, 0.0));

    // distance(p, s) debe ser igual a distance(s, p)
    BOOST_CHECK_EQUAL(distance(p, s), distance(s, p));
}

BOOST_FIXTURE_TEST_CASE(point_to_multipoint_distance, DistanceTest)
{
    geometry::MultiPoint2d mp = {geometry::Point2d(0.,0.), geometry::Point2d(10.,10.), geometry::Point2d(20.,20.)};
    geometry::Point2d p(0, 5);

    BOOST_CHECK_CLOSE(distance(p, mp), 5.0, 0.001);

    geometry::MultiPoint3d mp3 = {geometry::Point3d{0,0,0}, geometry::Point3d{10,10,10}};
    geometry::Point3d p3d(0, 0, 10);

    BOOST_CHECK_CLOSE(distance(p3d, mp3), 10.0, 0.001);
}

BOOST_FIXTURE_TEST_CASE(point_to_multilinestring_distance, DistanceTest)
{
    geometry::MultiLineString2d mls;

    // Línea A: Horizontal en y=0
    geometry::LineString2d la = {geometry::Point2d(0,0), geometry::Point2d(10,0)};
    // Línea B: Horizontal en y=10
    geometry::LineString2d lb = {geometry::Point2d(0,10), geometry::Point2d(10,10)};

    mls.push_back(la);
    mls.push_back(lb);

    geometry::Point2d p(5, 2); // Cerca de la línea A (distancia 2)
    BOOST_CHECK_CLOSE(distance(p, mls), 2.0, 0.001);

    geometry::Point2d p2(5, 9); // Cerca de la línea B (distancia 1)
    BOOST_CHECK_CLOSE(distance(p2, mls), 1.0, 0.001);
}

BOOST_FIXTURE_TEST_CASE(point_to_multipolygon_distance, DistanceTest)
{
    geometry::MultiPolygon2d archipelago;

    // Isla 1: Cuadrado 0,0 a 2,2
    geometry::Polygon2d i1;
    i1.outer() = {geometry::Point2d(0,0),
                  geometry::Point2d(2,0),
                  geometry::Point2d(2,2),
                  geometry::Point2d(0,2)};

    // Isla 2: Cuadrado 10,10 a 12,12
    geometry::Polygon2d i2;
    i2.outer() = {geometry::Point2d(10,10), 
                  geometry::Point2d(12,10), 
                  geometry::Point2d(12,12),
                  geometry::Point2d(10,12)};

    archipelago.push_back(i1);
    archipelago.push_back(i2);

    // Punto entre islas pero más cerca de la i1
    geometry::Point2d p(4, 1);
    BOOST_CHECK_CLOSE(distance(p, archipelago), 2.0, 0.001);

    // Punto exactamente sobre una isla
    BOOST_CHECK_SMALL(distance(geometry::Point2d(1, 1), archipelago), 0.00001);
}

BOOST_AUTO_TEST_CASE(point_to_segment_3d)
{
    geometry::Point3d s1(0, 0, 0), s2(0, 0, 10);
    geometry::Segment<geometry::Point3d> seg3d(s1, s2);

    geometry::Point3d p(5, 0, 5); // Proyecta en la mitad del segmento Z
    BOOST_CHECK_CLOSE(distance(p, seg3d), 5.0, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()