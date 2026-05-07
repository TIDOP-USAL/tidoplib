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
 
#define BOOST_TEST_MODULE Tidop boundary test
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include "tidop/geometry/algorithms/analysis/Boundary.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/base/GeometryCollection.h"

#include <variant>

using namespace tl;

BOOST_AUTO_TEST_SUITE(BoundaryTestSuite)

// ============================================================================
// TEST CASES POR TIPO DE GEOMETRÍA
// ============================================================================

BOOST_AUTO_TEST_CASE(point_boundary)
{
    // Boundary de un Point siempre es MultiPoint vacío
    Point2d point(1.0, 2.0);
    auto b = boundary(point);
    
    // Verificar que es un MultiPoint
    BOOST_CHECK_EQUAL(b.size(), 0);
    BOOST_CHECK(b.isEmpty());
    
    // Punto 3D también
    Point3d point3d(1.0, 2.0, 3.0);
    auto boundary3d = boundary(point3d);
    BOOST_CHECK_EQUAL(boundary3d.size(), 0);
}

BOOST_AUTO_TEST_CASE(segment_boundary)
{
    // Boundary de un Segment son sus dos extremos
    Segment2d segment(Point2d(0.0, 0.0), Point2d(10.0, 0.0));
    auto b = boundary(segment);
    
    BOOST_CHECK_EQUAL(b.size(), 2);
    BOOST_CHECK(b[0] == Point2d(0.0, 0.0));
    BOOST_CHECK(b[1] == Point2d(10.0, 0.0));
    
    // Segmento 3D
    Segment3d segment3d(Point3d(0.0, 0.0, 0.0), Point3d(10.0, 10.0, 10.0));
    auto boundary3d = boundary(segment3d);
    
    BOOST_CHECK_EQUAL(boundary3d.size(), 2);
    BOOST_CHECK(boundary3d[0] == Point3d(0.0, 0.0, 0.0));
    BOOST_CHECK(boundary3d[1] == Point3d(10.0, 10.0, 10.0));
}

BOOST_AUTO_TEST_CASE(linestring_boundary)
{
    // LineString abierta
    LineString2d open_line = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 0.0),
        Point2d(5.0, 5.0)
    };
    auto boundary_open = boundary(open_line);
    
    BOOST_CHECK_EQUAL(boundary_open.size(), 2);
    BOOST_CHECK(boundary_open[0] == Point2d(0.0, 0.0));
    BOOST_CHECK(boundary_open[1] == Point2d(5.0, 5.0));
    
    // LineString cerrada (anillo)
    LineString2d closed_line = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)  // Cierra el anillo
    };
    auto boundary_closed = boundary(closed_line);
    
    // Anillo cerrado -> boundary vacío
    BOOST_CHECK_EQUAL(boundary_closed.size(), 0);
    
    // LineString con solo 1 punto (degenerada)
    LineString2d single_point = { Point2d(1.0, 1.0) };
    auto boundary_single = boundary(single_point);
    BOOST_CHECK_EQUAL(boundary_single.size(), 0);
    
    // LineString vacía
    LineString2d empty_line;
    auto boundary_empty = boundary(empty_line);
    BOOST_CHECK_EQUAL(boundary_empty.size(), 0);
}

BOOST_AUTO_TEST_CASE(polygon_boundary)
{
    // Polígono simple sin huecos
    Polygon2d simple_poly;
    simple_poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };
    
    auto boundary_simple = boundary(simple_poly);
    
    BOOST_CHECK_EQUAL(boundary_simple.size(), 1); // Solo el anillo exterior
    BOOST_CHECK_EQUAL(boundary_simple[0].size(), 5); // 5 puntos (cerrado)
    BOOST_CHECK(boundary_simple[0][0] == Point2d(0.0, 0.0));
    BOOST_CHECK(boundary_simple[0][1] == Point2d(10.0, 0.0));
    BOOST_CHECK(boundary_simple[0][2] == Point2d(10.0, 10.0));
    BOOST_CHECK(boundary_simple[0][3] == Point2d(0.0, 10.0));
    BOOST_CHECK(boundary_simple[0][4] == Point2d(0.0, 0.0));
    
    // Polígono con huecos
    Polygon2d poly_with_holes;
    poly_with_holes.outer() = {
        Point2d(0.0, 0.0),
        Point2d(20.0, 0.0),
        Point2d(20.0, 20.0),
        Point2d(0.0, 20.0),
        Point2d(0.0, 0.0)
    };
    
    // Hueco 1
    LinearRing2d hole1 = {
        Point2d(5.0, 5.0),
        Point2d(15.0, 5.0),
        Point2d(15.0, 15.0),
        Point2d(5.0, 15.0),
        Point2d(5.0, 5.0)
    };
    
    // Hueco 2
    LinearRing2d hole2 = {
        Point2d(2.0, 2.0),
        Point2d(4.0, 2.0),
        Point2d(4.0, 4.0),
        Point2d(2.0, 4.0),
        Point2d(2.0, 2.0)
    };
    
    poly_with_holes.addInner(hole1);
    poly_with_holes.addInner(hole2);
    
    auto boundary_with_holes = boundary(poly_with_holes);
    
    BOOST_CHECK_EQUAL(boundary_with_holes.size(), 3); // Exterior + 2 huecos
    BOOST_CHECK_EQUAL(boundary_with_holes[0].size(), 5); // Exterior
    BOOST_CHECK_EQUAL(boundary_with_holes[1].size(), 5); // Hueco 1
    BOOST_CHECK_EQUAL(boundary_with_holes[2].size(), 5); // Hueco 2
    
    // Polígono degenerado (menos de 3 puntos)
    Polygon2d degenerate_poly;
    degenerate_poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(1.0, 0.0)
        // No cierra
    };
    
    auto boundary_degenerate = boundary(degenerate_poly);
    BOOST_CHECK_EQUAL(boundary_degenerate.size(), 1);
    BOOST_CHECK_EQUAL(boundary_degenerate[0].size(), 2);
}

BOOST_AUTO_TEST_CASE(multipoint_boundary)
{
    // MultiPoint siempre tiene boundary vacío
    MultiPoint2d multipoint = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(10.0, 10.0)
    };
    
    auto b = boundary(multipoint);
    BOOST_CHECK_EQUAL(b.size(), 0);
    
    // MultiPoint vacío
    MultiPoint2d empty_multipoint;
    auto boundary_empty = boundary(empty_multipoint);
    BOOST_CHECK_EQUAL(boundary_empty.size(), 0);
}

BOOST_AUTO_TEST_CASE(multilinestring_boundary)
{
    // MultiLineString: aplica regla de cancelación (mod 2)
    
    // Caso 1: Líneas abiertas que no se conectan
    MultiLineString2d mls1 = {
        LineString2d{Point2d(0.0, 0.0), Point2d(5.0, 0.0)},
        LineString2d{Point2d(10.0, 0.0), Point2d(15.0, 0.0)}
    };
    
    auto boundary1 = boundary(mls1);
    BOOST_CHECK_EQUAL(boundary1.size(), 4); // 4 extremos distintos
    
    // Caso 2: Líneas que forman una cadena (extremos compartidos se cancelan)
    MultiLineString2d mls2 = {
        LineString2d{Point2d(0.0, 0.0), Point2d(5.0, 0.0)},  // A
        LineString2d{Point2d(5.0, 0.0), Point2d(10.0, 0.0)}   // B: comparte extremo con A
    };
    
    auto boundary2 = boundary(mls2);
    // Extremo (5,0) aparece 2 veces -> se cancela
    // Quedan (0,0) y (10,0)
    BOOST_CHECK_EQUAL(boundary2.size(), 2);
    
    bool has_p0 = false, has_p10 = false;
    for (const auto& pt : boundary2) {
        if (pt == Point2d(0.0, 0.0)) has_p0 = true;
        if (pt == Point2d(10.0, 0.0)) has_p10 = true;
    }
    BOOST_CHECK(has_p0 && has_p10);
    
    // Caso 3: Línea cerrada (no contribuye al boundary)
    MultiLineString2d mls3 = {
        LineString2d{Point2d(0.0, 0.0), Point2d(5.0, 0.0), 
                         Point2d(5.0, 5.0), Point2d(0.0, 5.0),
                         Point2d(0.0, 0.0)}  // Cerrada
    };
    
    auto boundary3 = boundary(mls3);
    BOOST_CHECK_EQUAL(boundary3.size(), 0); // Línea cerrada -> sin boundary
    
    // Caso 4: Mezcla de líneas abiertas y cerradas
    MultiLineString2d mls4 = {
        LineString2d{Point2d(0.0, 0.0), Point2d(5.0, 0.0)},  // Abierta
        LineString2d{Point2d(0.0, 5.0), Point2d(5.0, 5.0), 
                         Point2d(5.0, 10.0), Point2d(0.0, 5.0)}  // Cerrada (triángulo)
    };
    
    auto boundary4 = boundary(mls4);
    // Solo la línea abierta contribuye: (0,0) y (5,0)
    BOOST_CHECK_EQUAL(boundary4.size(), 2);
}

BOOST_AUTO_TEST_CASE(multipolygon_boundary)
{
    // MultiPolygon: boundary es la unión de los boundaries de cada polígono
    
    // Polígono 1
    Polygon2d poly1;
    poly1.outer() = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(0.0, 5.0),
        Point2d(0.0, 0.0)
    };
    
    // Polígono 2
    Polygon2d poly2;
    poly2.outer() = {
        Point2d(10.0, 10.0),
        Point2d(15.0, 10.0),
        Point2d(15.0, 15.0),
        Point2d(10.0, 15.0),
        Point2d(10.0, 10.0)
    };
    
    MultiPolygon2d multipoly = {poly1, poly2};
    auto b = boundary(multipoly);
    
    BOOST_CHECK_EQUAL(b.size(), 2); // Dos polígonos -> dos anillos
    BOOST_CHECK_EQUAL(b[0].size(), 5); // Primer anillo
    BOOST_CHECK_EQUAL(b[1].size(), 5); // Segundo anillo
    
    // MultiPolygon con polígonos con huecos
    Polygon2d poly_with_hole;
    poly_with_hole.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };
    
    LinearRing2d hole = {
        Point2d(3.0, 3.0),
        Point2d(7.0, 3.0),
        Point2d(7.0, 7.0),
        Point2d(3.0, 7.0),
        Point2d(3.0, 3.0)
    };
    poly_with_hole.addInner(hole);
    
    MultiPolygon2d multipoly_with_holes = {poly1, poly_with_hole};
    auto boundary_with_holes = boundary(multipoly_with_holes);
    
    // poly1: 1 anillo, poly_with_hole: 2 anillos (exterior + hueco)
    BOOST_CHECK_EQUAL(boundary_with_holes.size(), 3);
}

BOOST_AUTO_TEST_CASE(geometry_collection_boundary)
{
    // GeometryCollection: boundary de cada elemento, sin cancelación entre elementos
    
    GeometryCollection<Point2d> collection;
    
    // Agregar un punto
    collection.push_back(Point2d(5.0, 5.0));
    
    // Agregar una línea abierta
    LineString2d line = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 0.0),
        Point2d(5.0, 5.0)
    };
    collection.push_back(line);
    
    // Agregar un polígono
    Polygon2d poly;
    poly.outer() = {
        Point2d(10.0, 10.0),
        Point2d(20.0, 10.0),
        Point2d(20.0, 20.0),
        Point2d(10.0, 20.0),
        Point2d(10.0, 10.0)
    };
    collection.push_back(poly);
    
    auto b = boundary(collection);
    
    // Debería contener:
    // 1. Boundary del punto: MultiPoint vacío (no agrega nada)
    // 2. Boundary de la línea: MultiPoint con 2 puntos
    // 3. Boundary del polígono: MultiLineString con 1 anillo
    
    BOOST_CHECK_EQUAL(b.size(), 2); // MultiPoint + MultiLineString
    
    // El primer elemento debería ser un MultiPoint (de la línea)
    // El segundo elemento debería ser un MultiLineString (del polígono)
}

BOOST_AUTO_TEST_CASE(empty_geometry_boundary)
{
    // Geometrías vacías
    LineString2d empty_line;
    Polygon2d empty_polygon;
    MultiPoint2d empty_multipoint;
    MultiLineString2d empty_multilinestring;
    MultiPolygon2d empty_multipolygon;
    GeometryCollection<Point2d> empty_collection;
    
    // Boundary de geometría vacía debería ser la correspondiente geometría vacía
    auto b1 = boundary(empty_line);
    BOOST_CHECK_EQUAL(b1.size(), 0);
    
    auto b2 = boundary(empty_polygon);
    BOOST_CHECK_EQUAL(b2.size(), 0);
    
    auto b3 = boundary(empty_multipoint);
    BOOST_CHECK_EQUAL(b3.size(), 0);
    
    auto b4 = boundary(empty_multilinestring);
    BOOST_CHECK_EQUAL(b4.size(), 0);
    
    auto b5 = boundary(empty_multipolygon);
    BOOST_CHECK_EQUAL(b5.size(), 0);
    
    auto b6 = boundary(empty_collection);
    BOOST_CHECK_EQUAL(b6.size(), 0);
}

BOOST_AUTO_TEST_CASE(three_dimensional_boundary)
{
    // Tests 3D
    Point3d point3d(1.0, 2.0, 3.0);
    auto b_point = boundary(point3d);
    BOOST_CHECK_EQUAL(b_point.size(), 0);
    
    Segment3d segment3d(Point3d(0.0, 0.0, 0.0), 
                           Point3d(10.0, 10.0, 10.0));
    auto b_segment = boundary(segment3d);
    BOOST_CHECK_EQUAL(b_segment.size(), 2);
    BOOST_CHECK(b_segment[0] == Point3d(0.0, 0.0, 0.0));
    BOOST_CHECK(b_segment[1] == Point3d(10.0, 10.0, 10.0));
    
    LineString3d line3d = {
        Point3d(0.0, 0.0, 0.0),
        Point3d(5.0, 0.0, 0.0),
        Point3d(5.0, 5.0, 0.0)
    };
    auto b_line = boundary(line3d);
    BOOST_CHECK_EQUAL(b_line.size(), 2);
}

BOOST_AUTO_TEST_CASE(boundary_consistency_with_ogc)
{
    // Verificar consistencia con reglas OGC
    
    // 1. Boundary de un punto es vacío (dimensionalidad 0)
    Point2d p(1, 1);
    auto b_p = boundary(p);
    BOOST_CHECK_EQUAL(b_p.size(), 0);
    
    // 2. Boundary de un segmento son sus extremos
    Segment2d s(Point2d(0, 0), Point2d(10, 0));
    auto b_s = boundary(s);
    BOOST_CHECK_EQUAL(b_s.size(), 2);
    
    // 3. Boundary de LineString cerrada es vacío
    LineString2d closed = {
        Point2d(0, 0),
        Point2d(10, 0),
        Point2d(10, 10),
        Point2d(0, 10),
        Point2d(0, 0)
    };
    auto b_closed = boundary(closed);
    BOOST_CHECK_EQUAL(b_closed.size(), 0);
    
    // 4. Boundary de LineString abierta son extremos
    LineString2d open = {
        Point2d(0, 0),
        Point2d(10, 0),
        Point2d(10, 10)
    };
    auto b_open = boundary(open);
    BOOST_CHECK_EQUAL(b_open.size(), 2);
    
    // 5. MultiLineString: cancelación de extremos
    // Crear dos líneas que comparten un extremo
    MultiLineString2d mls = {
        LineString2d{Point2d(0, 0), Point2d(5, 0)},
        LineString2d{Point2d(5, 0), Point2d(10, 0)}
    };
    auto b_mls = boundary(mls);
    // Extremo (5,0) aparece 2 veces -> se cancela
    // Quedan (0,0) y (10,0)
    BOOST_CHECK_EQUAL(b_mls.size(), 2);
}

BOOST_AUTO_TEST_CASE(boundary_complex_cases)
{
    // Casos complejos
    
    // 1. Polígono con huecos muy pequeños
    Polygon2d poly;
    poly.outer() = {
        Point2d(0, 0),
        Point2d(100, 0),
        Point2d(100, 100),
        Point2d(0, 100),
        Point2d(0, 0)
    };
    
    // Hueco de 1x1
    LinearRing2d small_hole = {
        Point2d(10, 10),
        Point2d(11, 10),
        Point2d(11, 11),
        Point2d(10, 11),
        Point2d(10, 10)
    };
    poly.addInner(small_hole);
    
    auto b = boundary(poly);
    BOOST_CHECK_EQUAL(b.size(), 2);
    BOOST_CHECK_EQUAL(b[0].size(), 5); // Exterior
    BOOST_CHECK_EQUAL(b[1].size(), 5); // Hueco
    
    // 2. MultiLineString con muchas líneas
    MultiLineString2d complex_mls;
    for (int i = 0; i < 10; ++i) {
        complex_mls.push_back(LineString2d{
            Point2d(i * 10.0, 0.0),
            Point2d((i + 1) * 10.0, 0.0)
        });
    }
    
    auto b_complex = boundary(complex_mls);
    // Todos los extremos internos se cancelan (aparecen 2 veces)
    // Solo quedan el primer y último punto
    BOOST_CHECK_EQUAL(b_complex.size(), 2);
    BOOST_CHECK(b_complex[0] == Point2d(0.0, 0.0));
    BOOST_CHECK(b_complex[1] == Point2d(100.0, 0.0));
    
    // 3. GeometryCollection anidada (si es soportado)
    // Nota: La implementación actual no soporta GeometryCollection anidada
    // pero podemos probar con colecciones complejas
    GeometryCollection<Point2d> complex_collection;
    
    // Agregar un MultiPoint
    MultiPoint2d mp = {
        Point2d(1, 1),
        Point2d(2, 2)
    };
    // Nota: Necesitamos verificar si GeometryCollection acepta MultiPoint
    // Asumiendo que sí...
    
    // Agregar un MultiLineString
    MultiLineString2d mls = {
        LineString2d{Point2d(0, 0), Point2d(5, 0)},
        LineString2d{Point2d(5, 0), Point2d(10, 0)}
    };
    
    // Agregar un MultiPolygon
    MultiPolygon2d mpy;
    Polygon2d p1;
    p1.outer() = {Point2d(0, 0), Point2d(5, 0), 
                  Point2d(5, 5), Point2d(0, 5),
                  Point2d(0, 0)};
    mpy.push_back(p1);
    
    // Agregar al collection (asumiendo que se puede)
    // complex_collection.push_back(mp);
    // complex_collection.push_back(mls);
    // complex_collection.push_back(mpy);
    
    // auto b_collection = boundary(complex_collection);
    // BOOST_CHECK(b_collection.size() > 0);
}

BOOST_AUTO_TEST_CASE(boundary_edge_cases)
{
    // Casos límite
    
    // 1. LineString con puntos duplicados
    LineString2d line_duplicate = {
        Point2d(0, 0),
        Point2d(0, 0),  // Duplicado
        Point2d(5, 0),
        Point2d(5, 0)   // Duplicado
    };
    auto b1 = boundary(line_duplicate);
    // Aún debería dar los extremos únicos
    BOOST_CHECK_EQUAL(b1.size(), 2);
    
    // 2. Segmento degenerado (puntos iguales)
    Segment2d degenerate_segment(Point2d(1, 1), Point2d(1, 1));
    auto b2 = boundary(degenerate_segment);
    BOOST_CHECK_EQUAL(b2.size(), 2); // Dos puntos iguales
    
    // 3. Polígono con anillo exterior degenerado (2 puntos)
    Polygon2d degenerate_poly;
    degenerate_poly.outer() = {
        Point2d(0, 0),
        Point2d(1, 0)
        // No cierra
    };
    auto b3 = boundary(degenerate_poly);
    BOOST_CHECK_EQUAL(b3.size(), 1);
    BOOST_CHECK_EQUAL(b3[0].size(), 2);
    
    // 4. MultiLineString vacío
    MultiLineString2d empty_mls;
    auto b4 = boundary(empty_mls);
    BOOST_CHECK_EQUAL(b4.size(), 0);
    
    // 5. LineString con todos puntos iguales
    LineString2d all_same = {
        Point2d(1, 1),
        Point2d(1, 1),
        Point2d(1, 1)
    };
    auto b5 = boundary(all_same);
    // Considerado cerrado (primer y último iguales)
    BOOST_CHECK_EQUAL(b5.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()