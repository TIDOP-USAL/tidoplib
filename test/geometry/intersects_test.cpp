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
 
#define BOOST_TEST_MODULE Tidop intersects test
#include <boost/test/unit_test.hpp>

#include "tidop/geometry/algorithms/analysis/Intersects.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/base/GeometryCollection.h"

using namespace tl;

BOOST_AUTO_TEST_SUITE(IntersectsTestSuite)

// ============================================================================
// TEST CASES POR TIPO DE GEOMETRÍA
// ============================================================================

BOOST_AUTO_TEST_CASE(point_point_intersects)
{
    tl::Point2d p1(1.0, 2.0);
    tl::Point2d p2(1.0, 2.0);
    tl::Point2d p3(3.0, 4.0);

    // Puntos iguales deben intersectar
    BOOST_CHECK(tl::intersects(p1, p2));
    BOOST_CHECK(tl::intersects(p2, p1)); // Simetría

    // Puntos diferentes no intersectan
    BOOST_CHECK(!tl::intersects(p1, p3));
    BOOST_CHECK(!tl::intersects(p3, p1));
}

BOOST_AUTO_TEST_CASE(point_segment_intersects)
{
    tl::Point2d p1(5.0, 5.0);
    tl::Segment2d seg(tl::Point2d(0.0, 0.0), tl::Point2d(10.0, 10.0));

    // Punto en el segmento (en el medio)
    BOOST_CHECK(tl::intersects(p1, seg));
    BOOST_CHECK(tl::intersects(seg, p1)); // Simetría

    // Punto en extremo del segmento
    tl::Point2d p2(0.0, 0.0);
    BOOST_CHECK(tl::intersects(p2, seg));

    // Punto fuera del segmento
    tl::Point2d p3(5.0, 6.0);
    BOOST_CHECK(!tl::intersects(p3, seg));

    // Punto colineal pero fuera del segmento
    tl::Point2d p4(15.0, 15.0);
    BOOST_CHECK(!tl::intersects(p4, seg));
}

BOOST_AUTO_TEST_CASE(segment_segment_intersects)
{
    // Segmentos que se cruzan
    tl::Segment2d s1(tl::Point2d(0.0, 0.0), tl::Point2d(10.0, 10.0));
    tl::Segment2d s2(tl::Point2d(0.0, 10.0), tl::Point2d(10.0, 0.0));
    BOOST_CHECK(tl::intersects(s1, s2));
    BOOST_CHECK(tl::intersects(s2, s1));

    // Segmentos que se tocan en extremo
    tl::Segment2d s3(tl::Point2d(0.0, 0.0), tl::Point2d(5.0, 5.0));
    tl::Segment2d s4(tl::Point2d(5.0, 5.0), tl::Point2d(10.0, 10.0));
    BOOST_CHECK(tl::intersects(s3, s4));

    // Segmentos paralelos no intersectantes
    tl::Segment2d s5(tl::Point2d(0.0, 0.0), tl::Point2d(10.0, 0.0));
    tl::Segment2d s6(tl::Point2d(0.0, 5.0), tl::Point2d(10.0, 5.0));
    BOOST_CHECK(!tl::intersects(s5, s6));

    // Segmentos colineales que se solapan
    tl::Segment2d s7(tl::Point2d(0.0, 0.0), tl::Point2d(5.0, 0.0));
    tl::Segment2d s8(tl::Point2d(3.0, 0.0), tl::Point2d(8.0, 0.0));
    BOOST_CHECK(tl::intersects(s7, s8));
}

BOOST_AUTO_TEST_CASE(point_linestring_intersects)
{
    tl::LineString2d line = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(5.0, 0.0),
        tl::Point2d(5.0, 5.0),
        tl::Point2d(0.0, 5.0)
    };

    // Punto en vértice
    BOOST_CHECK(tl::intersects(tl::Point2d(0.0, 0.0), line));

    // Punto en borde
    BOOST_CHECK(tl::intersects(tl::Point2d(2.5, 0.0), line));

    // Punto dentro del segmento no horizontal/vertical
    BOOST_CHECK(tl::intersects(tl::Point2d(5.0, 2.5), line));

    // Punto fuera
    BOOST_CHECK(!tl::intersects(tl::Point2d(10.0, 10.0), line));

    // Línea vacía
    tl::LineString2d empty_line;
    BOOST_CHECK(!tl::intersects(tl::Point2d(0.0, 0.0), empty_line));
}

BOOST_AUTO_TEST_CASE(point_polygon_intersects)
{
    // Polígono cuadrado con un hueco
    tl::Polygon2d polygon;
    polygon.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    // Hueco cuadrado en el centro
    tl::LinearRing2d hole = {
        tl::Point2d(3.0, 3.0),
        tl::Point2d(7.0, 3.0),
        tl::Point2d(7.0, 7.0),
        tl::Point2d(3.0, 7.0),
        tl::Point2d(3.0, 3.0)
    };
    polygon.addInner(hole);

    // Punto dentro del polígono (no en hueco)
    BOOST_CHECK(tl::intersects(tl::Point2d(1.0, 1.0), polygon));

    // Punto en borde exterior
    BOOST_CHECK(tl::intersects(tl::Point2d(5.0, 0.0), polygon));

    // Punto en borde del hueco
    BOOST_CHECK(tl::intersects(tl::Point2d(3.0, 5.0), polygon));

    // Punto en hueco (fuera del polígono)
    BOOST_CHECK(!tl::intersects(tl::Point2d(5.0, 5.0), polygon));

    // Punto fuera completamente
    BOOST_CHECK(!tl::intersects(tl::Point2d(15.0, 15.0), polygon));
}

BOOST_AUTO_TEST_CASE(linestring_linestring_intersects)
{
    tl::LineString2d line1 = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 10.0)
    };

    tl::LineString2d line2 = {
        tl::Point2d(0.0, 10.0),
        tl::Point2d(10.0, 0.0)
    };

    // Líneas que se cruzan
    BOOST_CHECK(tl::intersects(line1, line2));

    // Líneas que se tocan en extremo
    tl::LineString2d line3 = {
        tl::Point2d(10.0, 10.0),
        tl::Point2d(20.0, 20.0)
    };
    BOOST_CHECK(tl::intersects(line1, line3));

    // Líneas paralelas no intersectantes
    tl::LineString2d line4 = {
        tl::Point2d(0.0, 5.0),
        tl::Point2d(10.0, 15.0)
    };
    BOOST_CHECK(!tl::intersects(line1, line4));
}

BOOST_AUTO_TEST_CASE(linestring_polygon_intersects)
{
    tl::Polygon2d polygon;
    polygon.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    // Línea completamente dentro
    tl::LineString2d line1 = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(8.0, 8.0)
    };
    BOOST_CHECK(tl::intersects(line1, polygon));

    // Línea que cruza el polígono
    tl::LineString2d line2 = {
        tl::Point2d(-5.0, 5.0),
        tl::Point2d(15.0, 5.0)
    };
    BOOST_CHECK(tl::intersects(line2, polygon));

    // Línea que toca el borde
    tl::LineString2d line3 = {
        tl::Point2d(-5.0, 0.0),
        tl::Point2d(5.0, 0.0)
    };
    BOOST_CHECK(tl::intersects(line3, polygon));

    // Línea completamente fuera
    tl::LineString2d line4 = {
        tl::Point2d(-5.0, -5.0),
        tl::Point2d(-1.0, -1.0)
    };
    BOOST_CHECK(!tl::intersects(line4, polygon));
}

BOOST_AUTO_TEST_CASE(polygon_polygon_intersects)
{
    // Polígono 1: Cuadrado grande
    tl::Polygon2d poly1;
    poly1.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    // Polígono 2: Se solapa parcialmente
    tl::Polygon2d poly2;
    poly2.outer() = {
        tl::Point2d(5.0, 5.0),
        tl::Point2d(15.0, 5.0),
        tl::Point2d(15.0, 15.0),
        tl::Point2d(5.0, 15.0),
        tl::Point2d(5.0, 5.0)
    };

    // Polígono 3: Completamente dentro de poly1
    tl::Polygon2d poly3;
    poly3.outer() = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(4.0, 2.0),
        tl::Point2d(4.0, 4.0),
        tl::Point2d(2.0, 4.0),
        tl::Point2d(2.0, 2.0)
    };

    // Polígono 4: Completamente fuera
    tl::Polygon2d poly4;
    poly4.outer() = {
        tl::Point2d(20.0, 20.0),
        tl::Point2d(25.0, 20.0),
        tl::Point2d(25.0, 25.0),
        tl::Point2d(20.0, 25.0),
        tl::Point2d(20.0, 20.0)
    };

    BOOST_CHECK(tl::intersects(poly1, poly2));  // Solapamiento parcial
    BOOST_CHECK(tl::intersects(poly1, poly3));  // Contención completa
    BOOST_CHECK(!tl::intersects(poly1, poly4)); // Sin intersección
    BOOST_CHECK(tl::intersects(poly2, poly1));  // Simetría
}

BOOST_AUTO_TEST_CASE(multipoint_intersects)
{
    tl::MultiPoint2d mp1 = {
        tl::Point2d(1.0, 1.0),
        tl::Point2d(2.0, 2.0),
        tl::Point2d(3.0, 3.0)
    };

    tl::MultiPoint2d mp2 = {
        tl::Point2d(3.0, 3.0),  // Punto común
        tl::Point2d(4.0, 4.0)
    };

    tl::MultiPoint2d mp3 = {
        tl::Point2d(5.0, 5.0),
        tl::Point2d(6.0, 6.0)
    };

    tl::Point2d p1(2.0, 2.0);
    tl::Point2d p2(7.0, 7.0);

    // Multipunto con punto (punto contenido)
    BOOST_CHECK(tl::intersects(mp1, p1));
    BOOST_CHECK(tl::intersects(p1, mp1));

    // Multipunto con punto (punto no contenido)
    BOOST_CHECK(!tl::intersects(mp1, p2));

    // Multipunto con multipunto (intersección)
    BOOST_CHECK(tl::intersects(mp1, mp2));

    // Multipunto con multipunto (sin intersección)
    BOOST_CHECK(!tl::intersects(mp1, mp3));
}

BOOST_AUTO_TEST_CASE(multilinestring_intersects)
{
    tl::MultiLineString2d mls1 = {
        tl::LineString2d{tl::Point2d(0.0, 0.0), tl::Point2d(5.0, 5.0)},
        tl::LineString2d{tl::Point2d(0.0, 5.0), tl::Point2d(5.0, 0.0)}
    };

    tl::MultiLineString2d mls2 = {
        tl::LineString2d{tl::Point2d(2.0, 2.0), tl::Point2d(8.0, 8.0)},
        tl::LineString2d{tl::Point2d(0.0, 8.0), tl::Point2d(8.0, 0.0)}
    };

    tl::MultiLineString2d mls3 = {
        tl::LineString2d{tl::Point2d(10.0, 10.0), tl::Point2d(15.0, 15.0)}
    };

    tl::LineString2d line = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(8.0, 2.0)
    };

    // MultiLineString con LineString (intersección)
    BOOST_CHECK(tl::intersects(mls1, line));
    BOOST_CHECK(tl::intersects(line, mls1));

    // MultiLineString con MultiLineString (intersección)
    BOOST_CHECK(tl::intersects(mls1, mls2));

    // MultiLineString con MultiLineString (sin intersección)
    BOOST_CHECK(!tl::intersects(mls1, mls3));
}

BOOST_AUTO_TEST_CASE(multipolygon_intersects)
{
    // Crear varios polígonos para MultiPolygon
    tl::Polygon2d poly1;
    poly1.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(5.0, 0.0),
        tl::Point2d(5.0, 5.0),
        tl::Point2d(0.0, 5.0),
        tl::Point2d(0.0, 0.0)
    };

    tl::Polygon2d poly2;
    poly2.outer() = {
        tl::Point2d(3.0, 3.0),
        tl::Point2d(8.0, 3.0),
        tl::Point2d(8.0, 8.0),
        tl::Point2d(3.0, 8.0),
        tl::Point2d(3.0, 3.0)
    };

    tl::Polygon2d poly3;
    poly3.outer() = {
        tl::Point2d(10.0, 10.0),
        tl::Point2d(15.0, 10.0),
        tl::Point2d(15.0, 15.0),
        tl::Point2d(10.0, 15.0),
        tl::Point2d(10.0, 10.0)
    };

    tl::MultiPolygon2d mp1 = {poly1, poly2};
    tl::MultiPolygon2d mp2 = {poly2, poly3};
    tl::MultiPolygon2d mp3 = {poly3};

    tl::Point2d point_in_poly1(2.0, 2.0);
    tl::Point2d point_outside(20.0, 20.0);

    // MultiPolygon con Point (punto dentro)
    BOOST_CHECK(tl::intersects(mp1, point_in_poly1));
    BOOST_CHECK(tl::intersects(point_in_poly1, mp1));

    // MultiPolygon con Point (punto fuera)
    BOOST_CHECK(!tl::intersects(mp1, point_outside));

    // MultiPolygon con MultiPolygon (intersección)
    BOOST_CHECK(tl::intersects(mp1, mp2));

    // MultiPolygon con MultiPolygon (sin intersección)
    BOOST_CHECK(!tl::intersects(mp1, mp3));
}

BOOST_AUTO_TEST_CASE(geometry_collection_intersects)
{
    // Crear una GeometryCollection con diferentes tipos
    tl::GeometryCollection<Point2d> collection;

    // Agregar un punto
    collection.push_back(tl::Point2d(5.0, 5.0));

    // Agregar una línea
    tl::LineString2d line = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 10.0)
    };
    collection.push_back(line);

    // Agregar un polígono
    tl::Polygon2d polygon;
    polygon.outer() = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(8.0, 2.0),
        tl::Point2d(8.0, 8.0),
        tl::Point2d(2.0, 8.0),
        tl::Point2d(2.0, 2.0)
    };
    collection.push_back(polygon);

    // Crear otra colección
    tl::GeometryCollection<Point2d> collection2;
    collection2.push_back(tl::Point2d(7.0, 7.0));
    collection2.push_back(tl::LineString2d{tl::Point2d(0.0, 10.0), tl::Point2d(10.0, 0.0)});

    // Punto que intersecta con elementos de la colección
    BOOST_CHECK(tl::intersects(tl::Point2d(5.0, 5.0), collection));
    BOOST_CHECK(tl::intersects(collection, tl::Point2d(5.0, 5.0)));

    // Punto que no intersecta
    BOOST_CHECK(!tl::intersects(tl::Point2d(20.0, 20.0), collection));

    // Línea que intersecta
    tl::LineString2d test_line = {
        tl::Point2d(0.0, 5.0),
        tl::Point2d(10.0, 5.0)
    };
    BOOST_CHECK(tl::intersects(test_line, collection));
    BOOST_CHECK(tl::intersects(collection, test_line));

    // Colección con colección (intersección)
    BOOST_CHECK(tl::intersects(collection, collection2));

    // Colección con colección (sin intersección)
    tl::GeometryCollection<Point2d> collection3;
    collection3.push_back(tl::Point2d(50.0, 50.0));
    BOOST_CHECK(!tl::intersects(collection, collection3));
}

BOOST_AUTO_TEST_CASE(bounding_box_intersects)
{
    tl::BoundingBox2d bbox1(tl::Point2d(0.0, 0.0), tl::Point2d(10.0, 10.0));
    tl::BoundingBox2d bbox2(tl::Point2d(5.0, 5.0), tl::Point2d(15.0, 15.0));
    tl::BoundingBox2d bbox3(tl::Point2d(20.0, 20.0), tl::Point2d(30.0, 30.0));
    tl::BoundingBox2d empty_bbox;

    // BoundingBox con BoundingBox (intersección)
    BOOST_CHECK(tl::intersects(bbox1, bbox2));

    // BoundingBox con BoundingBox (sin intersección)
    BOOST_CHECK(!tl::intersects(bbox1, bbox3));

    // BoundingBox vacío
    BOOST_CHECK(!tl::intersects(bbox1, empty_bbox));
    BOOST_CHECK(!tl::intersects(empty_bbox, bbox1));

    // BoundingBox con Point (punto dentro)
    BOOST_CHECK(tl::intersects(bbox1, tl::Point2d(5.0, 5.0)));
    BOOST_CHECK(tl::intersects(tl::Point2d(5.0, 5.0), bbox1));

    // BoundingBox con Point (punto fuera)
    BOOST_CHECK(!tl::intersects(bbox1, tl::Point2d(15.0, 15.0)));

    // BoundingBox con Point (punto en borde)
    BOOST_CHECK(tl::intersects(bbox1, tl::Point2d(10.0, 5.0)));
}

BOOST_AUTO_TEST_CASE(edge_cases_and_special_cases)
{
    // Geometrías vacías
    tl::LineString2d empty_line;
    tl::Polygon2d empty_polygon;
    tl::MultiPoint2d empty_multipoint;
    tl::MultiLineString2d empty_multilinestring;
    tl::MultiPolygon2d empty_multipolygon;

    tl::Point2d point(1.0, 1.0);
    tl::LineString2d line = {tl::Point2d(0.0, 0.0), tl::Point2d(2.0, 2.0)};

    // Punto con geometría vacía
    BOOST_CHECK(!tl::intersects(point, empty_line));
    BOOST_CHECK(!tl::intersects(point, empty_polygon));
    BOOST_CHECK(!tl::intersects(point, empty_multipoint));

    // Línea con geometría vacía
    BOOST_CHECK(!tl::intersects(line, empty_line));
    BOOST_CHECK(!tl::intersects(line, empty_polygon));

    // Geometría vacía consigo misma
    BOOST_CHECK(!tl::intersects(empty_line, empty_line));
    BOOST_CHECK(!tl::intersects(empty_polygon, empty_polygon));

    // Polígonos degenerados (menos de 3 puntos)
    tl::Polygon2d degenerate_polygon;
    degenerate_polygon.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(1.0, 0.0)
        // Falta el tercer punto para cerrar
    };
    BOOST_CHECK(!tl::intersects(point, degenerate_polygon));

    // Líneas con puntos duplicados
    tl::LineString2d line_with_duplicates = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(0.0, 0.0),  // Duplicado
        tl::Point2d(1.0, 1.0)
    };
    BOOST_CHECK(tl::intersects(tl::Point2d(0.0, 0.0), line_with_duplicates));

    // Polígono con auto-intersección (no simple)
    tl::Polygon2d self_intersecting_polygon;
    self_intersecting_polygon.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };
    // Nota: intersect() debería funcionar incluso con polígonos no simples
    BOOST_CHECK(tl::intersects(tl::Point2d(5.0, 5.0), self_intersecting_polygon));
}

BOOST_AUTO_TEST_CASE(performance_and_optimization_checks)
{
    // Verificar que el chequeo de bounding boxes funciona
    tl::Polygon2d poly1;
    poly1.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(100.0, 0.0),
        tl::Point2d(100.0, 100.0),
        tl::Point2d(0.0, 100.0),
        tl::Point2d(0.0, 0.0)
    };

    // Punto muy lejos del polígono
    tl::Point2d far_point(1000.0, 1000.0);

    // El bounding box check debería evitar cálculos costosos
    BOOST_CHECK(!tl::intersects(poly1, far_point));

    // Polígonos con bounding boxes que no se intersectan
    tl::Polygon2d poly2;
    poly2.outer() = {
        tl::Point2d(200.0, 200.0),
        tl::Point2d(300.0, 200.0),
        tl::Point2d(300.0, 300.0),
        tl::Point2d(200.0, 300.0),
        tl::Point2d(200.0, 200.0)
    };

    BOOST_CHECK(!tl::intersects(poly1, poly2));

    // Geometrías grandes donde el bbox check es útil
    tl::LineString2d long_line;
    for (int i = 0; i < 1000; ++i) {
        long_line.push_back(tl::Point2d(i * 10.0, i * 10.0));
    }

    tl::Point2d point_near_start(5.0, 5.0);
    tl::Point2d point_far_away(10000.0, 10000.0);

    BOOST_CHECK(tl::intersects(point_near_start, long_line));
    BOOST_CHECK(!tl::intersects(point_far_away, long_line));
}

// ============================================================================
// TESTS DE REGRESIÓN PARA BUGS ESPECÍFICOS
// ============================================================================

BOOST_AUTO_TEST_CASE(regression_point_on_polygon_hole_boundary)
{
    // Bug: Un punto en el borde de un hueco debería intersectar el polígono
    tl::Polygon2d polygon_with_hole;
    polygon_with_hole.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    tl::LinearRing2d hole = {
        tl::Point2d(3.0, 3.0),
        tl::Point2d(7.0, 3.0),
        tl::Point2d(7.0, 7.0),
        tl::Point2d(3.0, 7.0),
        tl::Point2d(3.0, 3.0)
    };
    polygon_with_hole.addInner(hole);

    // Punto en el borde del hueco
    tl::Point2d point_on_hole_boundary(5.0, 3.0);
    BOOST_CHECK(tl::intersects(point_on_hole_boundary, polygon_with_hole));

    // Punto en el borde exterior
    tl::Point2d point_on_outer_boundary(5.0, 0.0);
    BOOST_CHECK(tl::intersects(point_on_outer_boundary, polygon_with_hole));

    // Punto dentro del hueco (no intersecta)
    tl::Point2d point_in_hole(5.0, 5.0);
    BOOST_CHECK(!tl::intersects(point_in_hole, polygon_with_hole));
}

BOOST_AUTO_TEST_CASE(regression_colinear_segments)
{
    // Segmentos colineales que se tocan en un punto
    tl::Segment2d s1(tl::Point2d(0.0, 0.0), tl::Point2d(5.0, 0.0));
    tl::Segment2d s2(tl::Point2d(5.0, 0.0), tl::Point2d(10.0, 0.0));

    BOOST_CHECK(tl::intersects(s1, s2));

    // Segmentos colineales que se solapan
    tl::Segment2d s3(tl::Point2d(0.0, 0.0), tl::Point2d(7.0, 0.0));
    tl::Segment2d s4(tl::Point2d(3.0, 0.0), tl::Point2d(10.0, 0.0));

    BOOST_CHECK(tl::intersects(s3, s4));

    // Segmentos colineales que no se tocan
    tl::Segment2d s5(tl::Point2d(0.0, 0.0), tl::Point2d(3.0, 0.0));
    tl::Segment2d s6(tl::Point2d(7.0, 0.0), tl::Point2d(10.0, 0.0));

    BOOST_CHECK(!tl::intersects(s5, s6));
}

BOOST_AUTO_TEST_CASE(regression_floating_point_precision)
{
    // Punto muy cerca del segmento (dentro de la tolerancia)
    tl::Point2d point(1.0, 1.000000001);  // Muy cerca de y = x
    tl::Segment2d seg(tl::Point2d(0.0, 0.0), tl::Point2d(2.0, 2.0));

    // Debería intersectar debido a la tolerancia
    BOOST_CHECK(tl::intersects(point, seg));

    // Punto justo fuera de la tolerancia
    tl::Point2d point2(1.0, 1.0001);
    BOOST_CHECK(!tl::intersects(point2, seg));

    // Puntos idénticos con diferencia de floating point
    tl::Point2d p1(1.0 / 3.0, 1.0 / 3.0);
    tl::Point2d p2(0.3333333333333333, 0.3333333333333333);

    BOOST_CHECK(tl::intersects(p1, p2));
}

BOOST_AUTO_TEST_SUITE_END()