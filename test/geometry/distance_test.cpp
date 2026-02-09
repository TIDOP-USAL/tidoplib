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


// Test Distancia Punto a Punto (2D y 3D)
BOOST_FIXTURE_TEST_CASE(point_to_point_distance, DistanceTest)
{
    BOOST_CHECK_CLOSE(213.378, distance(pt1, pt2), 0.001);
    BOOST_CHECK_CLOSE(213.378, distance(pt2, pt1), 0.001);
    BOOST_CHECK_CLOSE(2191.961, distance(pt1d, pt2d), 0.001);
    BOOST_CHECK_CLOSE(2191.961, distance(pt2d, pt1d), 0.001);
    BOOST_CHECK_CLOSE(5.0, distance(p1, p2), 0.001);
    BOOST_CHECK_CLOSE(1.73205, distance(p3, p4), 0.01);
}

// Test Distancia Punto a Segmento (Casos críticos)
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

    // Test de Simetría
    Point2d p(5.0, 5.0);
    Segment<Point2d> s(Point2d(0.0, 0.0), Point2d(10.0, 0.0));

    // distance(p, s) debe ser igual a distance(s, p)
    BOOST_CHECK_EQUAL(distance(p, s), distance(s, p));
}

// Test Distancia Punto a LineString
BOOST_FIXTURE_TEST_CASE(point_to_linestring_distance, DistanceTest)
{
    Point2d p(7.0, 2.0);
    // El punto más cercano está en el segmento (5,0)-(5,5)
    // La distancia ortogonal a ese segmento es |7.0 - 5.0| = 2.0
    BOOST_CHECK_CLOSE(distance(p, line), 2.0, 0.001);

    // Test de Simetría
    BOOST_CHECK_CLOSE(distance(line, p), 2.0, 0.001);
}

// Test Distancia Punto a Polígono (incluyendo huecos)
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

    // Test de Simetría
    BOOST_CHECK_CLOSE(distance(poly, pNearHole), 0.5, 0.001);
}

BOOST_FIXTURE_TEST_CASE(point_to_multipoint_distance, DistanceTest)
{
    MultiPoint2d mp = {Point2d(0.,0.), Point2d(10.,10.), Point2d(20.,20.)};
    Point2d p(0, 5);

    BOOST_CHECK_CLOSE(distance(p, mp), 5.0, 0.001);

    MultiPoint3d mp3 = {Point3d{0,0,0}, Point3d{10,10,10}};
    Point3d p3d(0, 0, 10);

    BOOST_CHECK_CLOSE(distance(p3d, mp3), 10.0, 0.001);

    // Test de Simetría
    BOOST_CHECK_CLOSE(distance(mp, p), 5.0, 0.001);
    BOOST_CHECK_CLOSE(distance(mp3, p3d), 10., 0.001);
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
    

    // Test de Simetría
    BOOST_CHECK_CLOSE(distance(mls, p), 2.0, 0.001);
    BOOST_CHECK_CLOSE(distance(mls, p2), 1.0, 0.001);
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

    // Test de Simetría
    BOOST_CHECK_CLOSE(distance(archipelago, p), 2.0, 0.001);
    BOOST_CHECK_SMALL(distance(archipelago, Point2d(1, 1)), 0.00001);
}

BOOST_AUTO_TEST_CASE(point_to_segment_3d)
{
    Point3d s1(0, 0, 0), s2(0, 0, 10);
    Segment<Point3d> seg3d(s1, s2);

    Point3d p(5, 0, 5); // Proyecta en la mitad del segmento Z
    BOOST_CHECK_CLOSE(distance(p, seg3d), 5.0, 0.001);
    BOOST_CHECK_CLOSE(distance(seg3d, p), 5.0, 0.001);
}

BOOST_AUTO_TEST_CASE(linestring_to_linestring_distance)
{
    LineString2d line1{Point2d{0,0}, Point2d{5,0}, Point2d{5,5}};
    LineString2d line2{Point2d{10,0}, Point2d{10,5}};  // A 5 unidades de distancia

    BOOST_CHECK_CLOSE(distance(line1, line2), 5.0, 0.001);

    LineString2d line3{Point2d{3,2}, Point2d{7,2}};  // Se cruza con line1 en (5,2)
    auto d = distance(line1, line3);
    BOOST_CHECK_SMALL(distance(line1, line3), 0.0001);
}

BOOST_AUTO_TEST_CASE(segment_to_linestring_distance)
{
    Segment2d seg(Point2d(0, 0), Point2d(0, 10));
    LineString2d line{Point2d{5,0}, Point2d{5,5}, Point2d{10,5}};  // Mínima distancia es 5

    BOOST_CHECK_CLOSE(distance(seg, line), 5.0, 0.001);
    BOOST_CHECK_CLOSE(distance(line, seg), 5.0, 0.001);
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
    BOOST_CHECK_CLOSE(distance(cloud2, cloud1), std::sqrt(2), 0.001);
}

BOOST_AUTO_TEST_CASE(geometry_collection_distance)
{
    GeometryCollection<Point2d> gc1;
    gc1.push_back(Point2d(0, 0));
    gc1.push_back(LineString2d{Point2d{1,1}, Point2d{2,2}});

    GeometryCollection<Point2d> gc2;
    gc2.push_back(Point2d(3, 3));
    gc2.push_back(Polygon2d{{Point2d{4,4}, Point2d{6,4}, Point2d{6,6}, Point2d{4,6}}});

    // La distancia mínima es entre (2,2) y (3,3) = √2 ≈ 1.414
    BOOST_CHECK_CLOSE(distance(gc1, gc2), std::sqrt(2), 0.001);
}

BOOST_AUTO_TEST_CASE(segment_to_segment_distance)
{
    // Segmentos paralelos no intersectantes
    Segment2d s1(Point2d(0.0, 0.0), Point2d(10.0, 0.0));
    Segment2d s2(Point2d(0.0, 5.0), Point2d(10.0, 5.0));
    BOOST_CHECK_CLOSE(distance(s1, s2), 5.0, 0.001);

    // Segmentos que se intersectan (distancia 0)
    Segment2d s3(Point2d(0.0, 0.0), Point2d(10.0, 10.0));
    Segment2d s4(Point2d(0.0, 10.0), Point2d(10.0, 0.0));
    BOOST_CHECK_SMALL(distance(s3, s4), 0.0001);

    // Segmentos que se tocan en extremo
    Segment2d s5(Point2d(0.0, 0.0), Point2d(5.0, 0.0));
    Segment2d s6(Point2d(5.0, 0.0), Point2d(10.0, 0.0));
    BOOST_CHECK_SMALL(distance(s5, s6), 0.0001);

    // Segmentos en 3D
    Segment3d s7(Point3d(0.0, 0.0, 0.0), Point3d(0.0, 0.0, 10.0));
    Segment3d s8(Point3d(5.0, 0.0, 5.0), Point3d(10.0, 0.0, 5.0));
    BOOST_CHECK_CLOSE(distance(s7, s8), 5.0, 0.001);
}

BOOST_AUTO_TEST_CASE(segment_to_polygon_distance)
{
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Segmento completamente dentro del polígono
    Segment2d s1(Point2d(2.0, 2.0), Point2d(8.0, 8.0));
    BOOST_CHECK_SMALL(distance(s1, poly), 0.0001);

    // Segmento que cruza el polígono
    Segment2d s2(Point2d(-5.0, 5.0), Point2d(15.0, 5.0));
    BOOST_CHECK_SMALL(distance(s2, poly), 0.0001);

    // Segmento fuera del polígono (paralelo a un lado)
    Segment2d s3(Point2d(-5.0, -5.0), Point2d(-1.0, -1.0));
    BOOST_CHECK_CLOSE(distance(s3, poly), std::sqrt(2.0), 0.001);

    // Segmento que toca el borde
    Segment2d s4(Point2d(-5.0, 0.0), Point2d(-1.0, 0.0));
    BOOST_CHECK_CLOSE(distance(s4, poly), 1.0, 0.001);

    // Segmento vertical fuera
    Segment2d s5(Point2d(-5.0, 2.0), Point2d(-5.0, 8.0));
    BOOST_CHECK_CLOSE(distance(s5, poly), 5.0, 0.001);

    // Segmento horizontal fuera
    Segment2d s6(Point2d(2.0, -5.0), Point2d(8.0, -5.0));
    BOOST_CHECK_CLOSE(distance(s6, poly), 5.0, 0.001);
}

BOOST_AUTO_TEST_CASE(segment_to_multipoint_distance)
{
    MultiPoint2d mp = {
        Point2d(1.0, 1.0),
        Point2d(5.0, 5.0),
        Point2d(10.0, 10.0)
    };

    Segment2d seg(Point2d(0.0, 0.0), Point2d(8.0, 0.0));

    // El punto más cercano es (1.0, 1.0) con distancia 1.0
    BOOST_CHECK_CLOSE(distance(seg, mp), 1.0, 0.001);
    BOOST_CHECK_CLOSE(distance(mp, seg), 1.0, 0.001);
}

BOOST_AUTO_TEST_CASE(segment_to_multilinestring_distance)
{
    MultiLineString2d mls;
    mls.push_back(LineString2d{Point2d(0.0, 5.0), Point2d(10.0, 5.0)});
    mls.push_back(LineString2d{Point2d(5.0, 0.0), Point2d(5.0, 10.0)});

    Segment2d seg(Point2d(0.0, 0.0), Point2d(0.0, 4.0));

    // El segmento está a 0.0 de la segunda línea (toca en (0,0)?)
    // En realidad, el segmento está vertical y la línea horizontal está en y=5
    // La distancia más cercana es desde (0,4) a (0,5) = 1.0
    BOOST_CHECK_CLOSE(distance(seg, mls), 1.0, 0.001);

    Segment2d seg2(Point2d(6.0, 6.0), Point2d(8.0, 8.0));
    // Más cerca de la línea vertical en x=5
    BOOST_CHECK_CLOSE(distance(seg2, mls), 1.0, 0.001);
}

BOOST_AUTO_TEST_CASE(segment_to_multipolygon_distance)
{
    MultiPolygon2d mp;

    Polygon2d poly1;
    poly1.outer() = {Point2d(0.0, 0.0), Point2d(5.0, 0.0),
                     Point2d(5.0, 5.0), Point2d(0.0, 5.0),
                     Point2d(0.0, 0.0)};

    Polygon2d poly2;
    poly2.outer() = {Point2d(10.0, 10.0), Point2d(15.0, 10.0),
                     Point2d(15.0, 15.0), Point2d(10.0, 15.0),
                     Point2d(10.0, 10.0)};

    mp.push_back(poly1);
    mp.push_back(poly2);

    Segment2d seg1(Point2d(2.0, 2.0), Point2d(3.0, 3.0));
    BOOST_CHECK_SMALL(distance(seg1, mp), 0.0001);

    Segment2d seg2(Point2d(7.0, 7.0), Point2d(8.0, 8.0));
    // Distancia al polígono 1: sqrt(2^2 + 2^2) ≈ 2.828
    BOOST_CHECK_CLOSE(distance(seg2, mp), std::sqrt(8), 0.001);
}

BOOST_AUTO_TEST_CASE(linestring_to_polygon_distance)
{
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Línea completamente dentro
    LineString2d line1 = {Point2d(2.0, 2.0), Point2d(8.0, 8.0)};
    BOOST_CHECK_SMALL(distance(line1, poly), 0.0001);

    // Línea que cruza el polígono
    LineString2d line2 = {Point2d(-5.0, 5.0), Point2d(15.0, 5.0)};
    BOOST_CHECK_SMALL(distance(line2, poly), 0.0001);

    // Línea fuera del polígono
    LineString2d line3 = {Point2d(-5.0, -5.0), Point2d(-1.0, -1.0)};
    BOOST_CHECK_CLOSE(distance(line3, poly), std::sqrt(2), 0.001);

    // Línea que toca el borde
    LineString2d line4 = {Point2d(-5.0, 0.0), Point2d(-1.0, 0.0)};
    BOOST_CHECK_CLOSE(distance(line4, poly), 1.0, 0.001);
}

BOOST_AUTO_TEST_CASE(linestring_to_multipoint_distance)
{
    MultiPoint2d mp = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(10.0, 0.0)
    };

    LineString2d line = {Point2d(0.0, 5.0), Point2d(10.0, 5.0)};

    // Distancia al punto (5,5) = 0.0? No, (5,5) a la línea y=5 es 0
    // Pero (5,5) está en la línea
    BOOST_CHECK_SMALL(distance(line, mp), 0.0001);

    LineString2d line2 = {Point2d(0.0, 10.0), Point2d(10.0, 10.0)};
    BOOST_CHECK_CLOSE(distance(line2, mp), 5.0, 0.001);
}

BOOST_AUTO_TEST_CASE(linestring_to_multilinestring_distance)
{
    MultiLineString2d mls1 = {
        LineString2d{Point2d(0.0, 0.0), Point2d(5.0, 5.0)},
        LineString2d{Point2d(0.0, 5.0), Point2d(5.0, 0.0)}
    };

    MultiLineString2d mls2 = {
        LineString2d{Point2d(10.0, 0.0), Point2d(10.0, 10.0)},
        LineString2d{Point2d(0.0, 10.0), Point2d(10.0, 10.0)}
    };

    LineString2d line = {Point2d(5.0, 5.0), Point2d(5.0, 15.0)};

    // La línea intersecta la primera multiLineString en (5,5)
    BOOST_CHECK_SMALL(distance(line, mls1), 0.0001);

    // Distancia a la segunda multiLineString
    BOOST_CHECK_CLOSE(distance(line, mls2), 0.0, 0.001); // Toca en (5,10)
}

BOOST_AUTO_TEST_CASE(linestring_to_multipolygon_distance)
{
    MultiPolygon2d mp;

    Polygon2d poly1;
    poly1.outer() = {Point2d(0.0, 0.0), Point2d(5.0, 0.0),
                     Point2d(5.0, 5.0), Point2d(0.0, 5.0),
                     Point2d(0.0, 0.0)};

    Polygon2d poly2;
    poly2.outer() = {Point2d(10.0, 10.0), Point2d(15.0, 10.0),
                     Point2d(15.0, 15.0), Point2d(10.0, 15.0),
                     Point2d(10.0, 10.0)};

    mp.push_back(poly1);
    mp.push_back(poly2);

    LineString2d line1 = {Point2d(2.0, 2.0), Point2d(3.0, 3.0)};
    BOOST_CHECK_SMALL(distance(line1, mp), 0.0001);

    LineString2d line2 = {Point2d(7.0, 7.0), Point2d(8.0, 8.0)};
    BOOST_CHECK_CLOSE(distance(line2, mp), std::sqrt(8), 0.001);
}

BOOST_AUTO_TEST_CASE(polygon_to_multipoint_distance)
{
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    MultiPoint2d mp_inside = {
        Point2d(5.0, 5.0),
        Point2d(6.0, 6.0)
    };

    MultiPoint2d mp_outside = {
        Point2d(-5.0, -5.0),
        Point2d(15.0, 15.0)
    };

    MultiPoint2d mp_mixed = {
        Point2d(5.0, 5.0),
        Point2d(15.0, 15.0)
    };

    BOOST_CHECK_SMALL(distance(poly, mp_inside), 0.0001);
    BOOST_CHECK_CLOSE(distance(poly, mp_outside), std::sqrt(50), 0.001);
    BOOST_CHECK_SMALL(distance(poly, mp_mixed), 0.0001);
}

BOOST_AUTO_TEST_CASE(polygon_to_multilinestring_distance)
{
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    MultiLineString2d mls_inside = {
        LineString2d{Point2d(2.0, 2.0), Point2d(8.0, 8.0)},
        LineString2d{Point2d(3.0, 3.0), Point2d(7.0, 7.0)}
    };

    MultiLineString2d mls_outside = {
        LineString2d{Point2d(-5.0, -5.0), Point2d(-1.0, -1.0)},
        LineString2d{Point2d(15.0, 15.0), Point2d(20.0, 20.0)}
    };

    MultiLineString2d mls_crossing = {
        LineString2d{Point2d(-5.0, 5.0), Point2d(15.0, 5.0)},
        LineString2d{Point2d(5.0, -5.0), Point2d(5.0, 15.0)}
    };

    BOOST_CHECK_SMALL(distance(poly, mls_inside), 0.0001);
    BOOST_CHECK_CLOSE(distance(poly, mls_outside), std::sqrt(2), 0.001);
    BOOST_CHECK_SMALL(distance(poly, mls_crossing), 0.0001);
}

BOOST_AUTO_TEST_CASE(multipoint_to_multilinestring_distance)
{
    MultiPoint2d mp = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(10.0, 0.0)
    };

    MultiLineString2d mls = {
        LineString2d{Point2d(0.0, 5.0), Point2d(10.0, 5.0)},
        LineString2d{Point2d(5.0, 0.0), Point2d(5.0, 10.0)}
    };

    // El punto (5,5) está en ambas líneas
    BOOST_CHECK_SMALL(distance(mp, mls), 0.0001);

    MultiPoint2d mp2 = {
        Point2d(-5.0, -5.0),
        Point2d(15.0, 15.0)
    };

    // Distancia de (-5,-5) a:
    // - línea1: al punto (0,5) = √(5² + 10²) = √125 ≈ 11.1803
    // - línea2: al punto (5,0) = √(10² + 5²) = √125 ≈ 11.1803
    // Distancia de (15,15) a:
    // - línea1: al punto (10,5) = √(5² + 10²) = √125 ≈ 11.1803  
    // - línea2: al punto (5,10) = √(10² + 5²) = √125 ≈ 11.1803
    // Distancia mínima = √125
    BOOST_CHECK_CLOSE(distance(mp2, mls), std::sqrt(125.0), 0.001);
}

BOOST_AUTO_TEST_CASE(multipoint_to_multipolygon_distance)
{
    MultiPolygon2d mp;

    Polygon2d poly1;
    poly1.outer() = {Point2d(0.0, 0.0), Point2d(5.0, 0.0),
                     Point2d(5.0, 5.0), Point2d(0.0, 5.0),
                     Point2d(0.0, 0.0)};

    Polygon2d poly2;
    poly2.outer() = {Point2d(10.0, 10.0), Point2d(15.0, 10.0),
                     Point2d(15.0, 15.0), Point2d(10.0, 15.0),
                     Point2d(10.0, 10.0)};

    mp.push_back(poly1);
    mp.push_back(poly2);

    MultiPoint2d points_inside = {
        Point2d(2.0, 2.0),
        Point2d(12.0, 12.0)
    };

    MultiPoint2d points_outside = {
        Point2d(-5.0, -5.0),
        Point2d(20.0, 20.0)
    };

    MultiPoint2d points_mixed = {
        Point2d(2.0, 2.0),
        Point2d(20.0, 20.0)
    };

    BOOST_CHECK_SMALL(distance(points_inside, mp), 0.0001);
    BOOST_CHECK_CLOSE(distance(points_outside, mp), std::sqrt(50), 0.001);
    BOOST_CHECK_SMALL(distance(points_mixed, mp), 0.0001);
}

BOOST_AUTO_TEST_CASE(multilinestring_to_multipolygon_distance)
{
    MultiPolygon2d mp;

    Polygon2d poly1;
    poly1.outer() = {Point2d(0.0, 0.0), Point2d(5.0, 0.0),
                     Point2d(5.0, 5.0), Point2d(0.0, 5.0),
                     Point2d(0.0, 0.0)};

    Polygon2d poly2;
    poly2.outer() = {Point2d(10.0, 10.0), Point2d(15.0, 10.0),
                     Point2d(15.0, 15.0), Point2d(10.0, 15.0),
                     Point2d(10.0, 10.0)};

    mp.push_back(poly1);
    mp.push_back(poly2);

    MultiLineString2d mls_inside = {
        LineString2d{Point2d(1.0, 1.0), Point2d(4.0, 4.0)},
        LineString2d{Point2d(11.0, 11.0), Point2d(14.0, 14.0)}
    };

    MultiLineString2d mls_outside = {
        LineString2d{Point2d(-5.0, -5.0), Point2d(-1.0, -1.0)},
        LineString2d{Point2d(20.0, 20.0), Point2d(25.0, 25.0)}
    };

    MultiLineString2d mls_crossing = {
        LineString2d{Point2d(-5.0, 2.5), Point2d(15.0, 2.5)},
        LineString2d{Point2d(2.5, -5.0), Point2d(2.5, 15.0)}
    };

    BOOST_CHECK_SMALL(distance(mls_inside, mp), 0.0001);
    BOOST_CHECK_CLOSE(distance(mls_outside, mp), std::sqrt(2.), 0.001);
    BOOST_CHECK_SMALL(distance(mls_crossing, mp), 0.0001);
}

BOOST_AUTO_TEST_CASE(geometry_collection_with_various_types_distance)
{
    GeometryCollection<Point2d> gc1;
    gc1.push_back(Point2d(0.0, 0.0));
    gc1.push_back(LineString2d{Point2d(1.0, 1.0), Point2d(2.0, 2.0)});
    gc1.push_back(Polygon2d{{Point2d(3.0, 3.0), Point2d(6.0, 3.0),
                             Point2d(6.0, 6.0), Point2d(3.0, 6.0)}});

    // Test con Point
    BOOST_CHECK_SMALL(distance(gc1, Point2d(0.0, 0.0)), 0.0001);
    BOOST_CHECK_CLOSE(distance(gc1, Point2d(10.0, 10.0)), std::sqrt(32), 0.001);

    // Test con LineString
    LineString2d line = {Point2d(0.0, 5.0), Point2d(5.0, 5.0)};
    BOOST_CHECK_CLOSE(distance(gc1, line), 0.0, 0.001); // Toca en (3,5) o (5,5)

    // Test con Polygon
    Polygon2d poly;
    poly.outer() = {Point2d(10.0, 10.0), Point2d(15.0, 10.0),
                    Point2d(15.0, 15.0), Point2d(10.0, 15.0),
                    Point2d(10.0, 10.0)};
    BOOST_CHECK_CLOSE(distance(gc1, poly), std::sqrt(32), 0.001);

    // Test con MultiPoint
    MultiPoint2d mp = {Point2d(7.0, 7.0), Point2d(8.0, 8.0)};
    BOOST_CHECK_CLOSE(distance(gc1, mp), std::sqrt(2.0), 0.001);
}

BOOST_AUTO_TEST_CASE(empty_geometry_distance)
{
    LineString2d empty_line;
    Polygon2d empty_polygon;
    MultiPoint2d empty_multipoint;

    Point2d point(1.0, 1.0);
    LineString2d line = {Point2d(0.0, 0.0), Point2d(2.0, 2.0)};

    // Distancia a geometría vacía debe ser "infinito" o máximo
    BOOST_CHECK_EQUAL(distance(point, empty_line), std::numeric_limits<double>::max());
    BOOST_CHECK_EQUAL(distance(empty_line, point), std::numeric_limits<double>::max());

    BOOST_CHECK_EQUAL(distance(line, empty_polygon), std::numeric_limits<double>::max());
    BOOST_CHECK_EQUAL(distance(empty_polygon, line), std::numeric_limits<double>::max());

    // Geometría vacía con geometría vacía
    BOOST_CHECK_EQUAL(distance(empty_line, empty_line), std::numeric_limits<double>::max());
    BOOST_CHECK_EQUAL(distance(empty_multipoint, empty_multipoint), std::numeric_limits<double>::max());
}

BOOST_AUTO_TEST_CASE(distance_with_tolerance_and_edge_cases)
{
    // Punto muy cerca de una línea (dentro de tolerancia)
    Point2d point(1.0, 1.0000001);
    LineString2d line = {Point2d(0.0, 1.0), Point2d(2.0, 1.0)};

    // Debería ser aproximadamente 0.0000001
    BOOST_CHECK_CLOSE(distance(point, line), 0.0000001, 0.1);

    // Puntos casi coincidentes
    Point2d p1(1.0, 1.0);
    Point2d p2(1.000000001, 1.000000001);
    BOOST_CHECK_SMALL(distance(p1, p2), 0.00001);

    // Segmentos casi paralelos
    Segment2d s1(Point2d(0.0, 0.0), Point2d(10.0, 0.0001));
    Segment2d s2(Point2d(0.0, 1.0), Point2d(10.0, 1.0001));

    // La distancia debería ser aproximadamente 1.0
    BOOST_CHECK_CLOSE(distance(s1, s2), 1.0, 0.001);
}

BOOST_AUTO_TEST_CASE(distance_symmetry_comprehensive)
{
    // Verificar simetría para todas las combinaciones
    Point2d p(3.0, 4.0);
    Segment2d s(Point2d(0.0, 0.0), Point2d(10.0, 0.0));
    LineString2d l = {Point2d(0.0, 5.0), Point2d(10.0, 5.0)};

    Polygon2d poly;
    poly.outer() = {Point2d(0.0, 0.0), Point2d(5.0, 0.0),
                    Point2d(5.0, 5.0), Point2d(0.0, 5.0),
                    Point2d(0.0, 0.0)};

    MultiPoint2d mp = {Point2d(1.0, 1.0), Point2d(6.0, 6.0)};
    MultiLineString2d mls = {l, LineString2d{Point2d(0.0, 10.0), Point2d(10.0, 10.0)}};
    MultiPolygon2d mpy = {poly};

    // Verificar simetría para cada par
    BOOST_CHECK_EQUAL(distance(p, s), distance(s, p));
    BOOST_CHECK_EQUAL(distance(p, l), distance(l, p));
    BOOST_CHECK_EQUAL(distance(p, poly), distance(poly, p));
    BOOST_CHECK_EQUAL(distance(p, mp), distance(mp, p));
    BOOST_CHECK_EQUAL(distance(p, mls), distance(mls, p));
    BOOST_CHECK_EQUAL(distance(p, mpy), distance(mpy, p));

    BOOST_CHECK_EQUAL(distance(s, l), distance(l, s));
    BOOST_CHECK_EQUAL(distance(s, poly), distance(poly, s));
    BOOST_CHECK_EQUAL(distance(s, mp), distance(mp, s));
    BOOST_CHECK_EQUAL(distance(s, mls), distance(mls, s));
    BOOST_CHECK_EQUAL(distance(s, mpy), distance(mpy, s));

    BOOST_CHECK_EQUAL(distance(l, poly), distance(poly, l));
    BOOST_CHECK_EQUAL(distance(l, mp), distance(mp, l));
    BOOST_CHECK_EQUAL(distance(l, mls), distance(mls, l));
    BOOST_CHECK_EQUAL(distance(l, mpy), distance(mpy, l));

    BOOST_CHECK_EQUAL(distance(poly, mp), distance(mp, poly));
    BOOST_CHECK_EQUAL(distance(poly, mls), distance(mls, poly));
    BOOST_CHECK_EQUAL(distance(poly, mpy), distance(mpy, poly));
}

BOOST_AUTO_TEST_CASE(three_dimensional_distance_comprehensive)
{
    // Tests exhaustivos para 3D
    Point3d p1(1.0, 2.0, 3.0);
    Point3d p2(4.0, 6.0, 8.0);

    // Distancia 3D: sqrt((4-1)² + (6-2)² + (8-3)²) = sqrt(9 + 16 + 25) = sqrt(50) ≈ 7.071
    BOOST_CHECK_CLOSE(distance(p1, p2), std::sqrt(50.0), 0.001);

    Segment3d seg3d(Point3d(0.0, 0.0, 0.0), Point3d(10.0, 0.0, 0.0));
    Point3d p3d(5.0, 5.0, 5.0);

    // Distancia de punto a segmento en 3D
    // Proyección en el segmento: (5,0,0)
    // Distancia: sqrt((5-5)² + (5-0)² + (5-0)²) = sqrt(0 + 25 + 25) = sqrt(50) ≈ 7.071
    BOOST_CHECK_CLOSE(distance(p3d, seg3d), std::sqrt(50.0), 0.001);

    // Segmento 3D a segmento 3D
    Segment3d seg1_3d(Point3d(0.0, 0.0, 0.0), Point3d(0.0, 0.0, 10.0));
    Segment3d seg2_3d(Point3d(5.0, 0.0, 5.0), Point3d(10.0, 0.0, 5.0));

    BOOST_CHECK_CLOSE(distance(seg1_3d, seg2_3d), 5.0, 0.001);

    // LineString 3D
    LineString3d line3d = {Point3d(0.0, 0.0, 0.0),
                           Point3d(5.0, 0.0, 0.0),
                           Point3d(5.0, 5.0, 0.0),
                           Point3d(5.0, 5.0, 5.0)};

    Point3d p3d2(2.0, 2.0, 2.0);

    BOOST_CHECK_CLOSE(distance(p3d2, line3d), std::sqrt(8.0), 0.001);
}

BOOST_AUTO_TEST_SUITE_END()