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
#include <tidop/geometry/base/GeometryCollection.h>

using namespace tl; 


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
        pt_int = Point2i(10, 50);

        pt1 = Point2d(21.45, 34.67);
        pt2 = Point2d(234.54, 45.76);

        pt1d = Point3d(3423.56, 345.78, 34.8);
        pt2d = Point3d(1234.45, 456.98, 45.9);

        p1 = Point2d(0.0, 0.0);
        p2 = Point2d(3.0, 4.0);
        p3 = Point3d(0.0, 0.0, 0.0);
        p4 = Point3d(1.0, 1.0, 1.0);

        segment = Segment<Point2d>(Point2d(0., 0.), Point2d(10., 0.));

        line.emplace_back(0.0, 0.0);
        line.emplace_back(5.0, 0.0);
        line.emplace_back(5.0, 5.0);
    }

    void teardown()
    {

    }

    Point2i pt_int;
    Point2d pt1;
    Point2d pt2;
    Point3d pt1d;
    Point3d pt2d;
    Point2d p1;
    Point2d p2;
    Point3d p3;
    Point3d p4;

    Segment<Point2d> segment;
    LineString<Point2d> line;
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
    Point2d pInside(5.0, 5.0);
    BOOST_CHECK_CLOSE(distance(pInside, segment), 5.0, 0.001);

    // Caso B: El punto está antes del inicio (p1)
    Point2d pBefore(-3.0, 4.0);
    BOOST_CHECK_CLOSE(distance(pBefore, segment), 5.0, 0.001); // Distancia a (0,0)

    // Caso C: El punto está después del final (p2)
    Point2d pAfter(13.0, 4.0);
    BOOST_CHECK_CLOSE(distance(pAfter, segment), 5.0, 0.001); // Distancia a (10,0)
}

// 3. Test Distancia Punto a LineString
BOOST_FIXTURE_TEST_CASE(point_to_linestring_distance, DistanceTest)
{
    Point2d p(7.0, 2.0);
    // El punto más cercano está en el segmento (5,0)-(5,5)
    // La distancia ortogonal a ese segmento es |7.0 - 5.0| = 2.0
    BOOST_CHECK_CLOSE(distance(p, line), 2.0, 0.001);
}

// 4. Test Distancia Punto a Polígono (incluyendo huecos)
BOOST_AUTO_TEST_CASE(point_to_polygon_distance)
{
    tl::Polygon<Point2d> poly;
    // Cuadrado exterior de 10x10
    poly.outer().emplace_back(0.0, 0.0);
    poly.outer().emplace_back(10.0, 0.0);
    poly.outer().emplace_back(10.0, 10.0);
    poly.outer().emplace_back(0.0, 10.0);

    // Punto fuera del polígono
    Point2d pOutside(12.0, 5.0);
    BOOST_CHECK_CLOSE(distance(pOutside, poly), 2.0, 0.001);

    // Punto dentro del polígono pero cerca de un hueco
    // Añadimos un hueco central de 2x2
    LinearRing<Point2d> hole;
    hole.emplace_back(4.0, 4.0);
    hole.emplace_back(6.0, 4.0);
    hole.emplace_back(6.0, 6.0);
    hole.emplace_back(4.0, 6.0);
    poly.addInner(hole);

    Point2d pNearHole(5.0, 4.5);
    // El punto está a 0.5 de la base del hueco
    BOOST_CHECK_CLOSE(distance(pNearHole, poly), 0.5, 0.001);
}

// 5. Test de Simetría (Tag Dispatching)
BOOST_AUTO_TEST_CASE(distance_symmetry)
{
    Point2d p(5.0, 5.0);
    Segment<Point2d> s(Point2d(0.0, 0.0), Point2d(10.0, 0.0));

    // distance(p, s) debe ser igual a distance(s, p)
    BOOST_CHECK_EQUAL(distance(p, s), distance(s, p));
}

BOOST_FIXTURE_TEST_CASE(point_to_multipoint_distance, DistanceTest)
{
    MultiPoint2d mp = {Point2d(0.,0.), Point2d(10.,10.), Point2d(20.,20.)};
    Point2d p(0, 5);

    BOOST_CHECK_CLOSE(distance(p, mp), 5.0, 0.001);

    MultiPoint3d mp3 = {Point3d{0,0,0}, Point3d{10,10,10}};
    Point3d p3d(0, 0, 10);

    BOOST_CHECK_CLOSE(distance(p3d, mp3), 10.0, 0.001);
}

BOOST_FIXTURE_TEST_CASE(point_to_multilinestring_distance, DistanceTest)
{
    MultiLineString2d mls;

    // Línea A: Horizontal en y=0
    LineString2d la = {Point2d(0,0), Point2d(10,0)};
    // Línea B: Horizontal en y=10
    LineString2d lb = {Point2d(0,10), Point2d(10,10)};

    mls.push_back(la);
    mls.push_back(lb);

    Point2d p(5, 2); // Cerca de la línea A (distancia 2)
    BOOST_CHECK_CLOSE(distance(p, mls), 2.0, 0.001);

    Point2d p2(5, 9); // Cerca de la línea B (distancia 1)
    BOOST_CHECK_CLOSE(distance(p2, mls), 1.0, 0.001);
}

BOOST_FIXTURE_TEST_CASE(point_to_multipolygon_distance, DistanceTest)
{
    MultiPolygon2d archipelago;

    // Isla 1: Cuadrado 0,0 a 2,2
    Polygon2d i1;
    i1.outer() = {Point2d(0,0),
                  Point2d(2,0),
                  Point2d(2,2),
                  Point2d(0,2)};

    // Isla 2: Cuadrado 10,10 a 12,12
    Polygon2d i2;
    i2.outer() = {Point2d(10,10), 
                  Point2d(12,10), 
                  Point2d(12,12),
                  Point2d(10,12)};

    archipelago.push_back(i1);
    archipelago.push_back(i2);

    // Punto entre islas pero más cerca de la i1
    Point2d p(4, 1);
    BOOST_CHECK_CLOSE(distance(p, archipelago), 2.0, 0.001);

    // Punto exactamente sobre una isla
    BOOST_CHECK_SMALL(distance(Point2d(1, 1), archipelago), 0.00001);
}

BOOST_AUTO_TEST_CASE(point_to_segment_3d)
{
    Point3d s1(0, 0, 0), s2(0, 0, 10);
    Segment<Point3d> seg3d(s1, s2);

    Point3d p(5, 0, 5); // Proyecta en la mitad del segmento Z
    BOOST_CHECK_CLOSE(distance(p, seg3d), 5.0, 0.001);
}

BOOST_AUTO_TEST_CASE(linestring_to_linestring_distance)
{
    LineString2d line1{Point2d{0,0}, Point2d{5,0}, Point2d{5,5}};
    LineString2d line2{Point2d{10,0}, Point2d{10,5}};  // A 5 unidades de distancia

    BOOST_CHECK_CLOSE(distance(line1, line2), 5.0, 0.001);

    LineString2d line3{Point2d{3,2}, Point2d{7,2}};  // Se cruza con line1 en (5,2)
    BOOST_CHECK_SMALL(distance(line1, line3), 0.0001);
}

BOOST_AUTO_TEST_CASE(segment_to_linestring_distance)
{
    Segment2d seg(Point2d(0, 0), Point2d(0, 10));
    LineString2d line{Point2d{5,0}, Point2d{5,5}, Point2d{10,5}};  // Mínima distancia es 5

    BOOST_CHECK_CLOSE(distance(seg, line), 5.0, 0.001);
}

BOOST_AUTO_TEST_CASE(polygon_to_polygon_distance)
{
    // Dos polígonos separados
    Polygon2d poly1{{Point2d{0,0}, Point2d{2,0}, Point2d{2,2}, Point2d{0,2}}};
    Polygon2d poly2{{Point2d{5,0}, Point2d{7,0}, Point2d{7,2}, Point2d{5,2}}};
    BOOST_CHECK_CLOSE(distance(poly1, poly2), 3.0, 0.001);

    // Polígonos que se tocan
    Polygon2d poly3{{Point2d{0,0}, Point2d{2,0}, Point2d{2,2}, Point2d{0,2}}};
    Polygon2d poly4{{Point2d{2,0}, Point2d{4,0}, Point2d{4,2}, Point2d{2,2}}};
    BOOST_CHECK_SMALL(distance(poly3, poly4), 0.0001);

    // Un polígono dentro de otro (con hueco)
    Polygon2d outer{{Point2d{0,0}, Point2d{10,0}, Point2d{10,10}, Point2d{0,10}}};
    Polygon2d inner{{Point2d{3,3}, Point2d{7,3}, Point2d{7,7}, Point2d{3,7}}};
    BOOST_CHECK_SMALL(distance(outer, inner), 0.0001);
}

BOOST_AUTO_TEST_CASE(multipoint_to_multipoint_distance)
{
    MultiPoint2d cloud1 = {Point2d(0,0), Point2d(5,5)};
    MultiPoint2d cloud2 = {Point2d(1,1), Point2d(6,6)};
    // Distancia mínima: (0,0) a (1,1) = √2 ≈ 1.414
    BOOST_CHECK_CLOSE(distance(cloud1, cloud2), std::sqrt(2), 0.001);
}

//BOOST_AUTO_TEST_CASE(geometry_collection_distance)
//{
//    GeometryCollection<Point2d> gc1;
//    gc1.push_back(Point2d(0, 0));
//    gc1.push_back(LineString2d{Point2d{1,1}, Point2d{2,2}});
//
//    GeometryCollection<Point2d> gc2;
//    gc2.push_back(Point2d(3, 3));
//    gc2.push_back(Polygon2d{{Point2d{4,4}, Point2d{6,4}, Point2d{6,6}, Point2d{4,6}}});
//
//    // La distancia mínima es entre (2,2) y (3,3) = √2 ≈ 1.414
//    BOOST_CHECK_CLOSE(distance(gc1, gc2), std::sqrt(2), 0.001);
//}

BOOST_AUTO_TEST_SUITE_END()