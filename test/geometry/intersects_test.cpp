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

#include "geometry_test_fixture.h"
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
using namespace test;

BOOST_AUTO_TEST_SUITE(IntersectsTestSuite)


// ============================================================================
// Point - Point
// ============================================================================

BOOST_FIXTURE_TEST_CASE(intersects_point_point, GeometryTestFixture)
{
    BOOST_CHECK(!intersects(point2d1, point2d2));
    BOOST_CHECK(!intersects(point2d2, point2d1));
    BOOST_CHECK(intersects(point2d1, point2d1));

    BOOST_CHECK(!intersects(point2d4, point2d5));
    BOOST_CHECK(intersects(point2d4, point2d5, policy));
}

BOOST_FIXTURE_TEST_CASE(disjoint_point_point_with_measure, GeometryTestFixture)
{
    BOOST_CHECK(intersects(point2d1, point2dm1));
    BOOST_CHECK(!intersects(point2dm1, point2dm2));
}

// ============================================================================
// Segment - Segment
// ============================================================================

BOOST_FIXTURE_TEST_CASE(intersects_segment_segment, GeometryTestFixture)
{
    // ------------------------------------------------------------------------
    // Casos donde SÍ hay intersección (al menos un punto en común)
    // ------------------------------------------------------------------------

    // 1. Segmentos que se cruzan en un punto interior
    BOOST_CHECK(intersects(segment1, segment4));   // diagonal (0,0)-(10,10) y horizontal (0,5)-(10,5) se cruzan en (5,5)
    BOOST_CHECK(intersects(segment4, segment1));   // simetría

    // 2. Segmentos que se tocan en un extremo
    BOOST_CHECK(intersects(segment_colineal_1, segment_colineal_2)); // (0,0)-(5,0) y (5,0)-(10,0) tocan en (5,0)
    BOOST_CHECK(intersects(segment_colineal_2, segment_colineal_1));

    // 3. Segmentos iguales (mismos puntos)
    Segment2d seg_dup(point2d1, point2d3);
    BOOST_CHECK(intersects(segment1, seg_dup));
    BOOST_CHECK(intersects(seg_dup, segment1));

    // 4. Segmentos colineales con superposición parcial
    BOOST_CHECK(intersects(segment7, segment3));
    BOOST_CHECK(intersects(segment3, segment7));

    // 5. Segmentos colineales donde uno contiene al otro
    BOOST_CHECK(intersects(segment9, segment10));
    BOOST_CHECK(intersects(segment10, segment9));

    // 6. Segmentos que comparten un punto interior de uno y extremo del otro
    BOOST_CHECK(intersects(segment4, segment2));
    BOOST_CHECK(intersects(segment2, segment4));

    // 7. Segmento degenerado (punto) que está sobre otro segmento
    Segment2d seg_degen_on(point2d2, point2d2); // (2.5,2.5) que está sobre segment1
    BOOST_CHECK(intersects(segment1, seg_degen_on));
    BOOST_CHECK(intersects(seg_degen_on, segment1));

    // ------------------------------------------------------------------------
    // Casos donde NO hay intersección (ningún punto en común)
    // ------------------------------------------------------------------------

    // 8. Segmentos paralelos y separados
    BOOST_CHECK(!intersects(segment4, segment3));
    BOOST_CHECK(!intersects(segment3, segment4));

    // 9. Segmentos no paralelos que no se cruzan
    BOOST_CHECK(!intersects(segment1, segment_out));
    BOOST_CHECK(!intersects(segment_out, segment1));

    // 10. Segmentos colineales pero separados (con hueco)
    Segment2d seg9(Point2d(0, 0), Point2d(5, 0));
    Segment2d seg10(Point2d(7, 0), Point2d(10, 0));
    BOOST_CHECK(!intersects(seg9, seg10));
    BOOST_CHECK(!intersects(seg10, seg9));

    // 11. Segmento degenerado (punto) que no está sobre el otro segmento
    Segment2d seg_degen_off(point2d4, point2d4); // (15,15) fuera de segment1
    BOOST_CHECK(!intersects(segment1, seg_degen_off));
    BOOST_CHECK(!intersects(seg_degen_off, segment1));

    // ------------------------------------------------------------------------
    // Pruebas con política de precisión (puntos casi coincidentes)
    // ------------------------------------------------------------------------

    // Segmento casi horizontal pero ligeramente desviado, que casi toca en un punto
    Segment2d seg_almost(Point2d(5, 1e-8), Point2d(5, 1e-8)); // degenerado, punto (5, 1e-8)
    // Con política de resolución 0.001, 1e-8 < 0.001, por lo que se considera igual a 0, luego el punto (5,0) está sobre seg_hor.
    BOOST_CHECK(intersects(segment3, seg_almost, policy));

    // Segmento casi paralelo pero ligeramente separado, con distancia < resolución
    Segment2d seg_almost_parallel(Point2d(0, 1e-8), Point2d(10, 1e-8));
    // Con la política, estos puntos se convierten a y=0, por lo que el segmento se superpone exactamente con seg_hor
    BOOST_CHECK(intersects(segment3, seg_almost_parallel, policy));

    // Segmento con separación mayor que la resolución
    Segment2d seg_far(Point2d(0, 0.01), Point2d(10, 0.01));
    BOOST_CHECK(!intersects(segment3, seg_far, policy));

    // Segmento que casi se cruza en un punto (desviación pequeña)
    // El segmento vertical se convierte a (5,0)-(5,0). Segmento degenerado
    Segment2d seg_cross_almost(Point2d(5, -1e-8), Point2d(5, 1e-8)); // vertical casi pasando por (5,0)
    BOOST_CHECK(intersects(segment3, seg_cross_almost, policy));

    // Ahora con una desviación mayor que la resolución
    Segment2d seg_cross_far(Point2d(5, -0.01), Point2d(5, 0.01));
    BOOST_CHECK(intersects(segment3, seg_cross_far, policy));
}


// ============================================================================
// LineString - LineString
// ============================================================================

BOOST_FIXTURE_TEST_CASE(intersects_linestring_linestring, GeometryTestFixture)
{
    // Línea horizontal (0,0)-(10,0)
    LineString2d line_horiz({point2d1, Point2d(10,0)});
    // Línea vertical (5,0)-(5,10)
    LineString2d line_vert({Point2d(5,0), Point2d(5,10)});
    // Línea diagonal (0,0)-(10,10)
    LineString2d line_diag({point2d1, point2d3});
    // Línea en L (0,0)-(5,0)-(5,5)
    LineString2d line_L({point2d1, Point2d(5,0), point2d7});
    // Línea que cruza en un vértice (0,5)-(10,5)
    LineString2d line_horiz_mid({Point2d(0,5), Point2d(10,5)});
    // Línea cerrada (triángulo)
    LineString2d line_triangle({point2d1, Point2d(10,0), Point2d(5,10), point2d1});

    // 1. Cruce en punto interior
    BOOST_CHECK(intersects(line_diag, line_horiz_mid));  // (5,5) interior de ambas
    BOOST_CHECK(intersects(line_horiz_mid, line_diag));  // simetría

    // 2. Toque en un extremo (horizontal y vertical se tocan en (5,0))
    BOOST_CHECK(intersects(line_horiz, line_vert));      // (5,0) común
    BOOST_CHECK(intersects(line_vert, line_horiz));

    // 3. Línea que contiene a otra (colineales)
    LineString2d line_long({point2d1, Point2d(10,0)});
    LineString2d line_short({Point2d(2,0), Point2d(8,0)});
    BOOST_CHECK(intersects(line_long, line_short));      // superposición parcial
    BOOST_CHECK(intersects(line_short, line_long));

    // 4. Líneas que comparten un vértice interior (L y horizontal)
    // line_L tiene vértice en (5,0) que es extremo de line_horiz? No, line_horiz termina en (10,0), luego (5,0) es interior de line_horiz.
    // Por tanto, (5,0) es interior de line_horiz y vértice de line_L (no extremo de line_L porque line_L tiene extremos (0,0) y (5,5)). Luego es interior de line_L? (5,0) es un vértice de line_L pero no es extremo global (extremos son (0,0) y (5,5)), luego es interior de line_L. Por tanto, intersección interior-interior, sí es intersects.
    BOOST_CHECK(intersects(line_L, line_horiz));         // punto (5,0)
    BOOST_CHECK(intersects(line_horiz, line_L));

    // 5. Líneas que no se tocan (paralelas separadas)
    LineString2d line_par_up({Point2d(0,1), Point2d(10,1)});
    BOOST_CHECK(!intersects(line_horiz, line_par_up));

    // 6. Línea cerrada consigo misma
    BOOST_CHECK(intersects(line_triangle, line_triangle));

    // 7. Línea vacía
    LineString2d empty;
    BOOST_CHECK(!intersects(empty, line_horiz));
    BOOST_CHECK(!intersects(line_horiz, empty));

    LineString2d triangle2({Point2d(5,0), Point2d(15,0), Point2d(10,10), Point2d(5,0)});
    BOOST_CHECK(intersects(line_triangle, triangle2));
    BOOST_CHECK(intersects(triangle2, line_triangle));

    LineString2d triangle3({Point2d(5,0), Point2d(15,0), Point2d(10,-10), Point2d(5,0)});
    BOOST_CHECK(intersects(line_triangle, triangle3));
    BOOST_CHECK(intersects(triangle3, line_triangle));

    // ============================================================================
    // Casos con líneas que se cruzan en múltiples puntos
    // ============================================================================

    // Línea ondulada que cruza repetidamente a otra
    LineString2d wavy({point2d1, Point2d(3,3), Point2d(6,0), Point2d(9,3), Point2d(12,0)});
    LineString2d straight({Point2d(0,1.5), Point2d(12,1.5)});
    // Cruza en 4 puntos, todos interiores de ambas → crosses
    BOOST_CHECK(intersects(wavy, straight));
    BOOST_CHECK(intersects(straight, wavy));

}


// ============================================================================
// Polygon - Polygon
// ============================================================================

BOOST_FIXTURE_TEST_CASE(polygon_polygon_intersects, GeometryTestFixture)
{
    // Polígono base: cuadrado sin agujero
    Polygon2d poly1 = square;   // (0,0)-(10,0)-(10,10)-(0,10)

    // ------------------------------------------------------------
    // Casos con dos polígonos sin agujeros
    // ------------------------------------------------------------

    // 1. Disjuntos (separados)
    Polygon2d separate;
    separate.outer() = {Point2d(15,15), Point2d(20,15), Point2d(20,20), Point2d(15,20), Point2d(15,15)};
    BOOST_CHECK(!intersects(poly1, separate));
    BOOST_CHECK(!intersects(separate, poly1));

    // 2. Tocar en un vértice
    Polygon2d touch_vertex;
    touch_vertex.outer() = {Point2d(10,10), Point2d(15,10), Point2d(15,15), Point2d(10,15), Point2d(10,10)};
    BOOST_CHECK(intersects(poly1, touch_vertex));
    BOOST_CHECK(intersects(touch_vertex, poly1));

    // 3. Tocar en un borde
    Polygon2d touch_edge;
    touch_edge.outer() = {Point2d(10,2), Point2d(15,2), Point2d(15,8), Point2d(10,8), Point2d(10,2)};
    BOOST_CHECK(intersects(poly1, touch_edge));
    BOOST_CHECK(intersects(touch_edge, poly1));

    // 4. Solapamiento parcial (intersección área)
    Polygon2d overlap;
    overlap.outer() = {Point2d(5,0), Point2d(15,0), Point2d(15,10), Point2d(5,10), Point2d(5,0)};
    BOOST_CHECK(intersects(poly1, overlap));
    BOOST_CHECK(intersects(overlap, poly1));

    // 5. Polígono completamente dentro (contenido)
    Polygon2d inside;
    inside.outer() = {Point2d(2,2), Point2d(8,2), Point2d(8,8), Point2d(2,8), Point2d(2,2)};
    BOOST_CHECK(intersects(poly1, inside));
    BOOST_CHECK(intersects(inside, poly1));

    // 6. Polígono igual
    BOOST_CHECK(intersects(poly1, poly1));

    // ------------------------------------------------------------
    // Casos con polígono que tiene un agujero (square_with_hole)
    // ------------------------------------------------------------
    Polygon2d poly_hole = square_with_hole;   // exterior (0,0)-(10,0)-(10,10)-(0,10), agujero (2.5,2.5)-(7.5,2.5)-(7.5,7.5)-(2.5,7.5)

    // 7. Polígono completamente dentro del agujero (no debe intersectar porque el agujero es vacío)
    Polygon2d in_hole;
    in_hole.outer() = {Point2d(3,3), Point2d(7,3), Point2d(7,7), Point2d(3,7), Point2d(3,3)};
    BOOST_CHECK(!intersects(poly_hole, in_hole));
    BOOST_CHECK(!intersects(in_hole, poly_hole));

    // 8. Polígono que cruza el agujero (parte dentro del agujero, parte fuera)
    Polygon2d cross_hole;
    cross_hole.outer() = {Point2d(2,2), Point2d(8,2), Point2d(8,8), Point2d(2,8), Point2d(2,2)};
    BOOST_CHECK(intersects(poly_hole, cross_hole));
    BOOST_CHECK(intersects(cross_hole, poly_hole));

    // 9. Polígono dentro del anillo exterior pero fuera del agujero
    Polygon2d outer_ring;
    outer_ring.outer() = {Point2d(1,1), Point2d(2,1), Point2d(2,2), Point2d(1,2), Point2d(1,1)};
    BOOST_CHECK(intersects(poly_hole, outer_ring));
    BOOST_CHECK(intersects(outer_ring, poly_hole));

    // 10. Polígono que toca el borde del agujero desde fuera
    Polygon2d touch_hole_boundary;
    touch_hole_boundary.outer() = {Point2d(2.5,2.5), Point2d(3.5,2.5), Point2d(3.5,3.5), Point2d(2.5,3.5), Point2d(2.5,2.5)};
    BOOST_CHECK(intersects(poly_hole, touch_hole_boundary));
    BOOST_CHECK(intersects(touch_hole_boundary, poly_hole));

    // 11. Polígono que coincide exactamente con el agujero (borde interior)
    Polygon2d exact_hole;
    exact_hole.outer() = {Point2d(2.5,2.5), Point2d(7.5,2.5), Point2d(7.5,7.5), Point2d(2.5,7.5), Point2d(2.5,2.5)};
    // El agujero es vacío, pero el borde del agujero es parte del polígono (frontera). Por tanto, sí hay intersección (la frontera común).
    BOOST_CHECK(intersects(poly_hole, exact_hole));
    BOOST_CHECK(intersects(exact_hole, poly_hole));

    // 12. Polígono que contiene completamente al polígono con agujero (intersección = polígono con agujero)
    Polygon2d bigger;
    bigger.outer() = {Point2d(-1,-1), Point2d(11,-1), Point2d(11,11), Point2d(-1,11), Point2d(-1,-1)};
    BOOST_CHECK(intersects(bigger, poly_hole));
    BOOST_CHECK(intersects(poly_hole, bigger));

    // 13. Polígono vacío (si se permite)
    Polygon2d empty_poly;
    BOOST_CHECK(!intersects(poly1, empty_poly));
    BOOST_CHECK(!intersects(empty_poly, poly1));
}




BOOST_AUTO_TEST_CASE(point_segment_intersects)
{
    Point2d p1(5.0, 5.0);
    Segment2d seg(Point2d(0.0, 0.0), Point2d(10.0, 10.0));

    // Punto en el segmento (en el medio)
    BOOST_CHECK(intersects(p1, seg));
    BOOST_CHECK(intersects(seg, p1)); // Simetría

    // Punto en extremo del segmento
    Point2d p2(0.0, 0.0);
    BOOST_CHECK(intersects(p2, seg));

    // Punto fuera del segmento
    Point2d p3(5.0, 6.0);
    BOOST_CHECK(!intersects(p3, seg));

    // Punto colineal pero fuera del segmento
    Point2d p4(15.0, 15.0);
    BOOST_CHECK(!intersects(p4, seg));
}


BOOST_AUTO_TEST_CASE(segment_LineString_intersects)
{
    Segment2d s(Point2d(0.0, 0.0), Point2d(10.0, 10.0));
    LineString2d ls = {
        Point2d(0.0, 10.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 9.0)};

    BOOST_CHECK(intersects(s, ls));
    BOOST_CHECK(intersects(ls, s));

    LineString2d ls2 = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 9.0)};
    BOOST_CHECK(intersects(s, ls2));
}

BOOST_AUTO_TEST_CASE(point_linestring_intersects)
{
    LineString2d line = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(0.0, 5.0)
    };

    // Punto en vértice
    BOOST_CHECK(intersects(Point2d(0.0, 0.0), line));

    // Punto en borde
    BOOST_CHECK(intersects(Point2d(2.5, 0.0), line));

    // Punto dentro del segmento no horizontal/vertical
    BOOST_CHECK(intersects(Point2d(5.0, 2.5), line));

    // Punto fuera
    BOOST_CHECK(!intersects(Point2d(10.0, 10.0), line));

    // Línea vacía
    LineString2d empty_line;
    BOOST_CHECK(!intersects(Point2d(0.0, 0.0), empty_line));
}

BOOST_AUTO_TEST_CASE(point_polygon_intersects)
{
    // Polígono cuadrado con un hueco
    Polygon2d polygon;
    polygon.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Hueco cuadrado en el centro
    LinearRing2d hole = {
        Point2d(3.0, 3.0),
        Point2d(7.0, 3.0),
        Point2d(7.0, 7.0),
        Point2d(3.0, 7.0),
        Point2d(3.0, 3.0)
    };
    polygon.addInner(hole);

    // Punto dentro del polígono (no en hueco)
    BOOST_CHECK(intersects(Point2d(1.0, 1.0), polygon));

    // Punto en borde exterior
    BOOST_CHECK(intersects(Point2d(5.0, 0.0), polygon));

    // Punto en borde del hueco
    BOOST_CHECK(intersects(Point2d(3.0, 5.0), polygon));

    // Punto en hueco (fuera del polígono)
    BOOST_CHECK(!intersects(Point2d(5.0, 5.0), polygon));

    // Punto fuera completamente
    BOOST_CHECK(!intersects(Point2d(15.0, 15.0), polygon));
}

//BOOST_AUTO_TEST_CASE(linestring_linestring_intersects)
//{
//    LineString2d line1 = {
//        Point2d(0.0, 0.0),
//        Point2d(10.0, 10.0)
//    };
//
//    LineString2d line2 = {
//        Point2d(0.0, 10.0),
//        Point2d(10.0, 0.0)
//    };
//
//    // Líneas que se cruzan
//    BOOST_CHECK(intersects(line1, line2));
//
//    // Líneas que se tocan en extremo
//    LineString2d line3 = {
//        Point2d(10.0, 10.0),
//        Point2d(20.0, 20.0)
//    };
//    BOOST_CHECK(intersects(line1, line3));
//
//    // Líneas paralelas no intersectantes
//    LineString2d line4 = {
//        Point2d(0.0, 5.0),
//        Point2d(10.0, 15.0)
//    };
//    BOOST_CHECK(!intersects(line1, line4));
//}


BOOST_AUTO_TEST_CASE(segment_polygon_intersects)
{
    Polygon2d polygon;
    polygon.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Segmento completamente dentro
    Segment2d segment1(Point2d(2.0, 2.0), Point2d(8.0, 8.0));
    BOOST_CHECK(intersects(segment1, polygon));

    // Segmento que cruza el polígono
    Segment2d segment2(Point2d(-5.0, 5.0), Point2d(15.0, 5.0));
    BOOST_CHECK(intersects(segment2, polygon));

    // Segmento que toca el borde
    Segment2d segment3(Point2d(-5.0, 0.0), Point2d(5.0, 0.0));
    BOOST_CHECK(intersects(segment3, polygon));

    // Segmento completamente fuera
    Segment2d segment4(Point2d(-5.0, -5.0), Point2d(-1.0, -1.0));
    BOOST_CHECK(!intersects(segment4, polygon));
}

BOOST_AUTO_TEST_CASE(linestring_polygon_intersects)
{
    Polygon2d polygon;
    polygon.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Línea completamente dentro
    LineString2d line1 = {
        Point2d(2.0, 2.0),
        Point2d(8.0, 8.0)
    };
    BOOST_CHECK(intersects(line1, polygon));

    // Línea que cruza el polígono
    LineString2d line2 = {
        Point2d(-5.0, 5.0),
        Point2d(15.0, 5.0)
    };
    BOOST_CHECK(intersects(line2, polygon));

    // Línea que toca el borde
    LineString2d line3 = {
        Point2d(-5.0, 0.0),
        Point2d(5.0, 0.0)
    };
    BOOST_CHECK(intersects(line3, polygon));

    // Línea completamente fuera
    LineString2d line4 = {
        Point2d(-5.0, -5.0),
        Point2d(-1.0, -1.0)
    };
    BOOST_CHECK(!intersects(line4, polygon));
}
//
//BOOST_AUTO_TEST_CASE(polygon_polygon_intersects)
//{
//    // Polígono 1: Cuadrado grande
//    Polygon2d poly1;
//    poly1.outer() = {
//        Point2d(0.0, 0.0),
//        Point2d(10.0, 0.0),
//        Point2d(10.0, 10.0),
//        Point2d(0.0, 10.0),
//        Point2d(0.0, 0.0)
//    };
//
//    // Polígono 2: Se solapa parcialmente
//    Polygon2d poly2;
//    poly2.outer() = {
//        Point2d(5.0, 5.0),
//        Point2d(15.0, 5.0),
//        Point2d(15.0, 15.0),
//        Point2d(5.0, 15.0),
//        Point2d(5.0, 5.0)
//    };
//
//    // Polígono 3: Completamente dentro de poly1
//    Polygon2d poly3;
//    poly3.outer() = {
//        Point2d(2.0, 2.0),
//        Point2d(4.0, 2.0),
//        Point2d(4.0, 4.0),
//        Point2d(2.0, 4.0),
//        Point2d(2.0, 2.0)
//    };
//
//    // Polígono 4: Completamente fuera
//    Polygon2d poly4;
//    poly4.outer() = {
//        Point2d(20.0, 20.0),
//        Point2d(25.0, 20.0),
//        Point2d(25.0, 25.0),
//        Point2d(20.0, 25.0),
//        Point2d(20.0, 20.0)
//    };
//
//    BOOST_CHECK(intersects(poly1, poly2));  // Solapamiento parcial
//    BOOST_CHECK(intersects(poly1, poly3));  // Contención completa
//    BOOST_CHECK(!intersects(poly1, poly4)); // Sin intersección
//    BOOST_CHECK(intersects(poly2, poly1));  // Simetría
//}

BOOST_AUTO_TEST_CASE(multipoint_intersects)
{
    MultiPoint2d mp1 = {
        Point2d(1.0, 1.0),
        Point2d(2.0, 2.0),
        Point2d(3.0, 3.0)
    };

    MultiPoint2d mp2 = {
        Point2d(3.0, 3.0),  // Punto común
        Point2d(4.0, 4.0)
    };

    MultiPoint2d mp3 = {
        Point2d(5.0, 5.0),
        Point2d(6.0, 6.0)
    };

    Point2d p1(2.0, 2.0);
    Point2d p2(7.0, 7.0);

    // Multipunto con punto (punto contenido)
    BOOST_CHECK(intersects(mp1, p1));
    BOOST_CHECK(intersects(p1, mp1));

    // Multipunto con punto (punto no contenido)
    BOOST_CHECK(!intersects(mp1, p2));

    // Multipunto con multipunto (intersección)
    BOOST_CHECK(intersects(mp1, mp2));

    // Multipunto con multipunto (sin intersección)
    BOOST_CHECK(!intersects(mp1, mp3));
}

BOOST_AUTO_TEST_CASE(multilinestring_intersects)
{
    MultiLineString2d mls1 = {
        LineString2d{Point2d(0.0, 0.0), Point2d(5.0, 5.0)},
        LineString2d{Point2d(0.0, 5.0), Point2d(5.0, 0.0)}
    };

    MultiLineString2d mls2 = {
        LineString2d{Point2d(2.0, 2.0), Point2d(8.0, 8.0)},
        LineString2d{Point2d(0.0, 8.0), Point2d(8.0, 0.0)}
    };

    MultiLineString2d mls3 = {
        LineString2d{Point2d(10.0, 10.0), Point2d(15.0, 15.0)}
    };

    LineString2d line = {
        Point2d(2.0, 2.0),
        Point2d(8.0, 2.0)
    };

    // MultiLineString con LineString (intersección)
    BOOST_CHECK(intersects(mls1, line));
    BOOST_CHECK(intersects(line, mls1));

    // MultiLineString con MultiLineString (intersección)
    BOOST_CHECK(intersects(mls1, mls2));

    // MultiLineString con MultiLineString (sin intersección)
    BOOST_CHECK(!intersects(mls1, mls3));
}

BOOST_AUTO_TEST_CASE(multipolygon_intersects)
{
    // Crear varios polígonos para MultiPolygon
    Polygon2d poly1;
    poly1.outer() = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(0.0, 5.0),
        Point2d(0.0, 0.0)
    };

    Polygon2d poly2;
    poly2.outer() = {
        Point2d(3.0, 3.0),
        Point2d(8.0, 3.0),
        Point2d(8.0, 8.0),
        Point2d(3.0, 8.0),
        Point2d(3.0, 3.0)
    };

    Polygon2d poly3;
    poly3.outer() = {
        Point2d(10.0, 10.0),
        Point2d(15.0, 10.0),
        Point2d(15.0, 15.0),
        Point2d(10.0, 15.0),
        Point2d(10.0, 10.0)
    };

    MultiPolygon2d mp1 = {poly1, poly2};
    MultiPolygon2d mp2 = {poly2, poly3};
    MultiPolygon2d mp3 = {poly3};

    Point2d point_in_poly1(2.0, 2.0);
    Point2d point_outside(20.0, 20.0);

    // MultiPolygon con Point (punto dentro)
    BOOST_CHECK(intersects(mp1, point_in_poly1));
    BOOST_CHECK(intersects(point_in_poly1, mp1));

    // MultiPolygon con Point (punto fuera)
    BOOST_CHECK(!intersects(mp1, point_outside));

    // MultiPolygon con MultiPolygon (intersección)
    BOOST_CHECK(intersects(mp1, mp2));

    // MultiPolygon con MultiPolygon (sin intersección)
    BOOST_CHECK(!intersects(mp1, mp3));
}

BOOST_AUTO_TEST_CASE(geometry_collection_intersects)
{
    // Crear una GeometryCollection con diferentes tipos
    GeometryCollection<Point2d> collection;

    // Agregar un punto
    collection.push_back(Point2d(5.0, 5.0));

    // Agregar una línea
    LineString2d line = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 10.0)
    };
    collection.push_back(line);

    // Agregar un polígono
    Polygon2d polygon;
    polygon.outer() = {
        Point2d(2.0, 2.0),
        Point2d(8.0, 2.0),
        Point2d(8.0, 8.0),
        Point2d(2.0, 8.0),
        Point2d(2.0, 2.0)
    };
    collection.push_back(polygon);

    // Crear otra colección
    GeometryCollection<Point2d> collection2;
    collection2.push_back(Point2d(7.0, 7.0));
    collection2.push_back(LineString2d{Point2d(0.0, 10.0), Point2d(10.0, 0.0)});

    // Punto que intersecta con elementos de la colección
    BOOST_CHECK(intersects(Point2d(5.0, 5.0), collection));
    BOOST_CHECK(intersects(collection, Point2d(5.0, 5.0)));

    // Punto que no intersecta
    BOOST_CHECK(!intersects(Point2d(20.0, 20.0), collection));

    // Línea que intersecta
    LineString2d test_line = {
        Point2d(0.0, 5.0),
        Point2d(10.0, 5.0)
    };
    BOOST_CHECK(intersects(test_line, collection));
    BOOST_CHECK(intersects(collection, test_line));

    // Colección con colección (intersección)
    BOOST_CHECK(intersects(collection, collection2));

    // Colección con colección (sin intersección)
    GeometryCollection<Point2d> collection3;
    collection3.push_back(Point2d(50.0, 50.0));
    BOOST_CHECK(!intersects(collection, collection3));
}

BOOST_AUTO_TEST_CASE(bounding_box_intersects)
{
    BoundingBox2d bbox1(Point2d(0.0, 0.0), Point2d(10.0, 10.0));
    BoundingBox2d bbox2(Point2d(5.0, 5.0), Point2d(15.0, 15.0));
    BoundingBox2d bbox3(Point2d(20.0, 20.0), Point2d(30.0, 30.0));
    BoundingBox2d empty_bbox;

    // BoundingBox con BoundingBox (intersección)
    BOOST_CHECK(intersects(bbox1, bbox2));

    // BoundingBox con BoundingBox (sin intersección)
    BOOST_CHECK(!intersects(bbox1, bbox3));

    // BoundingBox vacío
    BOOST_CHECK(!intersects(bbox1, empty_bbox));
    BOOST_CHECK(!intersects(empty_bbox, bbox1));

    // BoundingBox con Point (punto dentro)
    BOOST_CHECK(intersects(bbox1, Point2d(5.0, 5.0)));
    BOOST_CHECK(intersects(Point2d(5.0, 5.0), bbox1));

    // BoundingBox con Point (punto fuera)
    BOOST_CHECK(!intersects(bbox1, Point2d(15.0, 15.0)));

    // BoundingBox con Point (punto en borde)
    BOOST_CHECK(intersects(bbox1, Point2d(10.0, 5.0)));
}

BOOST_AUTO_TEST_CASE(edge_cases_and_special_cases)
{
    // Geometrías vacías
    LineString2d empty_line;
    Polygon2d empty_polygon;
    MultiPoint2d empty_multipoint;
    MultiLineString2d empty_multilinestring;
    MultiPolygon2d empty_multipolygon;

    Point2d point(1.0, 1.0);
    LineString2d line = {Point2d(0.0, 0.0), Point2d(2.0, 2.0)};

    // Punto con geometría vacía
    BOOST_CHECK(!intersects(point, empty_line));
    BOOST_CHECK(!intersects(point, empty_polygon));
    BOOST_CHECK(!intersects(point, empty_multipoint));

    // Línea con geometría vacía
    BOOST_CHECK(!intersects(line, empty_line));
    BOOST_CHECK(!intersects(line, empty_polygon));

    // Geometría vacía consigo misma
    BOOST_CHECK(!intersects(empty_line, empty_line));
    BOOST_CHECK(!intersects(empty_polygon, empty_polygon));

    // Polígonos degenerados (menos de 3 puntos)
    Polygon2d degenerate_polygon;
    degenerate_polygon.outer() = {
        Point2d(0.0, 0.0),
        Point2d(1.0, 0.0)
        // Falta el tercer punto para cerrar
    };
    BOOST_CHECK(!intersects(point, degenerate_polygon));

    // Líneas con puntos duplicados
    LineString2d line_with_duplicates = {
        Point2d(0.0, 0.0),
        Point2d(0.0, 0.0),  // Duplicado
        Point2d(1.0, 1.0)
    };
    BOOST_CHECK(intersects(Point2d(0.0, 0.0), line_with_duplicates));

    // Polígono con auto-intersección (no simple)
    Polygon2d self_intersecting_polygon;
    self_intersecting_polygon.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(0.0, 10.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 0.0)
    };
    // Nota: intersect() debería funcionar incluso con polígonos no simples
    BOOST_CHECK(intersects(Point2d(5.0, 5.0), self_intersecting_polygon));
}

BOOST_AUTO_TEST_CASE(performance_and_optimization_checks)
{
    // Verificar que el chequeo de bounding boxes funciona
    Polygon2d poly1;
    poly1.outer() = {
        Point2d(0.0, 0.0),
        Point2d(100.0, 0.0),
        Point2d(100.0, 100.0),
        Point2d(0.0, 100.0),
        Point2d(0.0, 0.0)
    };

    // Punto muy lejos del polígono
    Point2d far_point(1000.0, 1000.0);

    // El bounding box check debería evitar cálculos costosos
    BOOST_CHECK(!intersects(poly1, far_point));

    // Polígonos con bounding boxes que no se intersectan
    Polygon2d poly2;
    poly2.outer() = {
        Point2d(200.0, 200.0),
        Point2d(300.0, 200.0),
        Point2d(300.0, 300.0),
        Point2d(200.0, 300.0),
        Point2d(200.0, 200.0)
    };

    BOOST_CHECK(!intersects(poly1, poly2));

    // Geometrías grandes donde el bbox check es útil
    LineString2d long_line;
    for (int i = 0; i < 1000; ++i) {
        long_line.push_back(Point2d(i * 10.0, i * 10.0));
    }

    Point2d point_near_start(5.0, 5.0);
    Point2d point_far_away(10000.0, 10000.0);

    BOOST_CHECK(intersects(point_near_start, long_line));
    BOOST_CHECK(!intersects(point_far_away, long_line));
}

// ============================================================================
// TESTS DE REGRESIÓN PARA BUGS ESPECÍFICOS
// ============================================================================

BOOST_AUTO_TEST_CASE(regression_point_on_polygon_hole_boundary)
{
    // Bug: Un punto en el borde de un hueco debería intersectar el polígono
    Polygon2d polygon_with_hole;
    polygon_with_hole.outer() = {
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
    polygon_with_hole.addInner(hole);

    // Punto en el borde del hueco
    Point2d point_on_hole_boundary(5.0, 3.0);
    BOOST_CHECK(intersects(point_on_hole_boundary, polygon_with_hole));

    // Punto en el borde exterior
    Point2d point_on_outer_boundary(5.0, 0.0);
    BOOST_CHECK(intersects(point_on_outer_boundary, polygon_with_hole));

    // Punto dentro del hueco (no intersecta)
    Point2d point_in_hole(5.0, 5.0);
    BOOST_CHECK(!intersects(point_in_hole, polygon_with_hole));
}

BOOST_AUTO_TEST_CASE(regression_colinear_segments)
{
    // Segmentos colineales que se tocan en un punto
    Segment2d s1(Point2d(0.0, 0.0), Point2d(5.0, 0.0));
    Segment2d s2(Point2d(5.0, 0.0), Point2d(10.0, 0.0));

    BOOST_CHECK(intersects(s1, s2));

    // Segmentos colineales que se solapan
    Segment2d s3(Point2d(0.0, 0.0), Point2d(7.0, 0.0));
    Segment2d s4(Point2d(3.0, 0.0), Point2d(10.0, 0.0));

    BOOST_CHECK(intersects(s3, s4));

    // Segmentos colineales que no se tocan
    Segment2d s5(Point2d(0.0, 0.0), Point2d(3.0, 0.0));
    Segment2d s6(Point2d(7.0, 0.0), Point2d(10.0, 0.0));

    BOOST_CHECK(!intersects(s5, s6));
}

BOOST_AUTO_TEST_CASE(regression_floating_point_precision)
{
    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Punto muy cerca del segmento
    Point2d point(1.0, 1.0001);  // Muy cerca de y = x
    Segment2d seg(Point2d(0.0, 0.0), Point2d(2.0, 2.0));

    // Debería intersectar debido a la tolerancia
    BOOST_CHECK(intersects(point, seg, policy));

    // Punto justo fuera de la tolerancia
    Point2d point2(1.0, 1.0001);
    BOOST_CHECK(!intersects(point2, seg));

    // Puntos idénticos con diferencia de floating point
    Point2d p1(1.0 / 3.0, 1.0 / 3.0);
    Point2d p2(0.3333333333333333, 0.3333333333333333);

    BOOST_CHECK(intersects(p1, p2));
}

BOOST_AUTO_TEST_SUITE_END()