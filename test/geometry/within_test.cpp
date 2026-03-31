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
 
#define BOOST_TEST_MODULE Tidop algorithms within test
#include <boost/test/unit_test.hpp>

#include "geometry_test_fixture.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/base/GeometryCollection.h"
#include "tidop/geometry/algorithms/analysis/Within.h"

using namespace tl; 
using namespace test;

BOOST_AUTO_TEST_SUITE(WithinAlgorithmTest)

// ============================================================================
// Point - Point
// ============================================================================
// Point - Point: Siempre falso
// No se tocan, aunque sean iguales (se contiene a sí mismo, pero no se toca 
// a sí mismo)
// ============================================================================

BOOST_FIXTURE_TEST_CASE(within_point_point, GeometryTestFixture)
{
    BOOST_CHECK(within(point2d1, point2d1));
    BOOST_CHECK(within(point2d2, point2d2));
    BOOST_CHECK(!within(point2d1, point2d2));
    BOOST_CHECK(!within(point2d4, point2d5));
    BOOST_CHECK(within(point2d4, point2d5, policy));
}

BOOST_FIXTURE_TEST_CASE(within_point_point_with_measure, GeometryTestFixture)
{
    BOOST_CHECK(within(point2dm1, point2dm1));
    BOOST_CHECK(within(point2dm2, point2dm2));
    BOOST_CHECK(!within(point2dm1, point2dm2));
}

// ============================================================================
// Segment - Segment
// ============================================================================

BOOST_FIXTURE_TEST_CASE(within_segment_segment, GeometryTestFixture)
{
    // ------------------------------------------------------------------------
    // Casos donde SÍ hay within (A está completamente dentro de B)
    // ------------------------------------------------------------------------

    // 1. Segmento contenido en otro más grande
    BOOST_CHECK(within(segment10, segment9));

    // 2. Segmento que coincide con otro (iguales)
    BOOST_CHECK(within(segment1, segment1));

    // Nota: within no es simétrico, solo probamos una dirección.
    Segment2d seg_ext(Point2d(0, 0), Point2d(10, 0));
    Segment2d seg_int(Point2d(2, 0), Point2d(8, 0));

    // 3. Segmento que empieza en un extremo y termina dentro
    Segment2d seg_half(Point2d(0, 0), Point2d(5, 0));
    BOOST_CHECK(within(seg_half, seg_ext));

    // 4. Segmento degenerado (punto) que está en el interior de otro segmento
    Segment2d seg_degen_on(point2d2, point2d2); // (2.5,2.5) interior de segment1
    BOOST_CHECK(within(seg_degen_on, segment1));

    // ------------------------------------------------------------------------
    // Casos donde NO hay within
    // ------------------------------------------------------------------------

    // 5. Segmento degenerado en un extremo (no tiene interior, luego no cumple)
    Segment2d seg_degen_at_start(point2d1, point2d1); // (0,0) extremo de segment1
    BOOST_CHECK(!within(seg_degen_at_start, segment1));

    // 6. Segmento que se cruza (intersección en punto interior) → false
    BOOST_CHECK(!within(segment1, segment4)); // diagonal vs horizontal
    BOOST_CHECK(!within(segment4, segment1));

    // 7. Segmentos que se tocan en un extremo pero no están contenidos
    BOOST_CHECK(!within(segment_colineal_1, segment_colineal_2));
    BOOST_CHECK(!within(segment_colineal_2, segment_colineal_1));

    // 8. Segmento que se sale parcialmente
    Segment2d seg_largo(Point2d(0, 0), Point2d(12, 0));
    BOOST_CHECK(!within(seg_largo, seg_ext));

    // 9. Segmento colineal separado (sin superposición)
    Segment2d seg_sep(Point2d(12, 0), Point2d(15, 0));
    BOOST_CHECK(!within(seg_sep, seg_ext));

    // 10. Segmento no colineal (diferente dirección)
    BOOST_CHECK(!within(segment3, segment2)); // horizontal vs vertical
    BOOST_CHECK(!within(segment2, segment3));

    // 11. Segmento degenerado (punto) fuera del segmento
    Segment2d seg_degen_off(point2d4, point2d4); // (15,15)
    BOOST_CHECK(!within(seg_degen_off, segment1));

    // ------------------------------------------------------------------------
    // Pruebas con política de precisión
    // ------------------------------------------------------------------------

    // Segmento casi contenido (con desviación menor que la resolución)
    Segment2d seg_almost(Point2d(2, 1e-8), Point2d(8, 1e-8)); // se convierte a y=0
    BOOST_CHECK(within(seg_almost, seg_ext, policy));

    // Segmento casi contenido pero con desviación mayor que la resolución
    Segment2d seg_not_contained(Point2d(2, 0.001), Point2d(8, 0.001));
    BOOST_CHECK(!within(seg_not_contained, seg_ext, policy));

    // Segmento que casi empieza en el extremo (dentro)
    Segment2d seg_touch_start(Point2d(1e-8, 0), Point2d(5, 0));
    BOOST_CHECK(within(seg_touch_start, seg_ext, policy));

    // Segmento que casi termina en el extremo final (dentro)
    Segment2d seg_touch_end(Point2d(5, 0), Point2d(10 - 1e-8, 0));
    BOOST_CHECK(within(seg_touch_end, seg_ext, policy));

    // Segmento degenerado casi en el interior
    Segment2d seg_degen_almost(point2d2, point2d2); // (2.5,2.5)
    BOOST_CHECK(within(seg_degen_almost, segment1, policy));

    // Segmento degenerado casi en el extremo (debe dar false)
    Segment2d seg_degen_almost_start(Point2d(1e-8, 0), Point2d(1e-8, 0));
    BOOST_CHECK(!within(seg_degen_almost_start, seg_ext, policy));
}


// ============================================================================
// LineString - LineString
// ============================================================================

BOOST_FIXTURE_TEST_CASE(within_linestring_linestring, GeometryTestFixture)
{
    // Línea larga (0,0)-(10,0)
    LineString2d line_long({point2d1, Point2d(10,0)});
    // Línea corta dentro (2,0)-(8,0)
    LineString2d line_short({Point2d(2,0), Point2d(8,0)});
    // Línea que empieza en extremo (0,0)-(5,0)
    LineString2d line_start({point2d1, Point2d(5,0)});
    // Línea que termina en extremo (5,0)-(10,0)
    LineString2d line_end({Point2d(5,0), Point2d(10,0)});
    // Línea con un punto interior (punto degenerado)
    LineString2d line_point({point2d2}); // solo un punto

    // Línea que no está contenida (se sale)
    LineString2d line_out({Point2d(-1,0), Point2d(5,0)});
    // Línea que cruza
    LineString2d line_cross({Point2d(5,-5), Point2d(5,5)});

    // 1. Línea corta contenida en larga (colineal)
    BOOST_CHECK(within(line_short, line_long));
    // 2. Línea que empieza en extremo (sí está contenida, porque el extremo es parte de la línea)
    BOOST_CHECK(within(line_start, line_long));
    // 3. Línea que termina en extremo
    BOOST_CHECK(within(line_end, line_long));
    // 4. Línea con un punto interior
    BOOST_CHECK(within(line_point, line_long));
    // 5. Línea que se sale por la izquierda
    BOOST_CHECK(!within(line_out, line_long));
    // 6. Línea que cruza (no contenida)
    BOOST_CHECK(!within(line_cross, line_long));
    // 7. Línea vacía
    LineString2d empty;
    BOOST_CHECK(!within(empty, line_long));
    // 8. Contención en línea cerrada
    LineString2d line_triangle({point2d1, Point2d(10,0), Point2d(5,10), point2d1});
    LineString2d line_inside_triangle({Point2d(5,1), Point2d(6,2)}); // puntos dentro? pero no es colineal, luego no está contenida
    BOOST_CHECK(!within(line_inside_triangle, line_triangle)); // porque la línea no es un área, solo el contorno
    // Una línea contenida en el contorno de un triángulo sería parte de él, por ejemplo un lado
    LineString2d line_side({point2d1, Point2d(10,0)});
    BOOST_CHECK(within(line_side, line_triangle));
}



//// ============================================================================
//// Punto vs MultiPunto
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Point_MultiPoint, WithinTestFixture)
//{
//    BOOST_CHECK(within(p1, mp_single));              // punto contenido
//    BOOST_CHECK(within(p1, mp_double));              // punto en el multi
//    BOOST_CHECK(!within(p3, mp_double));             // punto no contenido
//    BOOST_CHECK(!within(p1, mp_empty));              // multi vacío -> false
//}
//
//// ============================================================================
//// Punto vs Línea (LineString)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Point_LineString, WithinTestFixture)
//{
//    // punto sobre la línea
//    BOOST_CHECK(!within(p1, line_diagonal));           // extremo inicial
//    BOOST_CHECK(within(p2, line_diagonal));           // punto interior (vértice)
//    BOOST_CHECK(within(p_on_line, line_diagonal));    // punto intermedio en segmento (0.5,0.5)
//
//    // punto fuera
//    BOOST_CHECK(!within(Point2d(0.0, 0.1), line_diagonal));
//
//    // línea vacía (si permitimos line_string vacía)
//    LineString<Point2d> empty_line;
//    BOOST_CHECK(!within(p1, empty_line));
//}
//
//// ============================================================================
//// Punto vs Polígono
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Point_Polygon, WithinTestFixture)
//{
//    // interior
//    BOOST_CHECK(within(p_inside_polygon, square));
//    // borde (debe considerarse fuera)
//    BOOST_CHECK(!within(p_on_polygon_boundary, square));
//    // exterior
//    BOOST_CHECK(!within(p_outside_polygon, square));
//
//    // polígono con agujero: punto dentro del agujero está fuera del polígono
//    Point2d inside_hole(1.5, 1.5);   // dentro del agujero (donut)
//    BOOST_CHECK(!within(inside_hole, donut));
//    // punto en el anillo exterior (borde del agujero)
//    Point2d on_hole_boundary(1.0, 1.0); // vértice del agujero
//    BOOST_CHECK(!within(on_hole_boundary, donut));
//
//    // polígono vacío (si existe)
//    // Polygon<...> empty_poly; BOOST_CHECK(!within(p1, empty_poly));
//}
//
//// ============================================================================
//// Punto vs MultiLínea
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Point_MultiLineString, WithinTestFixture)
//{
//    BOOST_CHECK(!within(p1, mls_one));                // punto en la única línea (en el borde)
//    BOOST_CHECK(within(p2, mls_one));                 // punto en la única línea
//    BOOST_CHECK(!within(p1, mls_two));                // punto en alguna línea (en el borde)
//    BOOST_CHECK(within(p2, mls_two));                // punto en alguna línea (en el borde)
//    BOOST_CHECK(!within(p3, mls_one));                // p3 no está en line_diagonal? En realidad p3 es (2,2) y line_diagonal llega hasta (2,2) así que sí está. Usemos otro punto.
//    Point2d p_not_on_lines(0.0, 0.5);
//    BOOST_CHECK(!within(p_not_on_lines, mls_two));    // no pertenece a ninguna línea
//    BOOST_CHECK(!within(p1, mls_empty));              // multi vacío
//}
//
//// ============================================================================
//// Punto vs MultiPolígono
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Point_MultiPolygon, WithinTestFixture)
//{
//    BOOST_CHECK(within(p_inside_polygon, mpoly_one));      // dentro del único polígono
//    BOOST_CHECK(within(p_inside_polygon, mpoly_two));      // dentro de alguno
//    Point2d in_donut(2.5, 2.5);   // dentro del donut (anillo exterior, fuera del agujero)
//    BOOST_CHECK(within(in_donut, mpoly_two));              // dentro del donut
//    Point2d in_hole(1.5, 1.5);     // dentro del agujero del donut (no pertenece a donut, pero podría estar en square? no, square es unidad)
//    BOOST_CHECK(!within(in_hole, mpoly_two));              // fuera de ambos
//    BOOST_CHECK(!within(p1, mpoly_empty));                 // multi vacío
//}
//
//// ============================================================================
//// Segmento vs Segmento
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Segment_Segment, WithinTestFixture)
//{
//    // segmento contenido en otro más largo (misma dirección)
//    Segment<Point2d> short_seg(p1, Point2d(0.5, 0.5));
//    BOOST_CHECK(within(short_seg, seg_diag));
//
//    // segmento igual
//    BOOST_CHECK(within(seg_diag, seg_diag));
//
//    // segmento no contenido (por ejemplo, perpendicular)
//    Segment<Point2d> perp_seg(Point2d(0.5, 0.0), Point2d(0.5, 1.0));
//    BOOST_CHECK(!within(perp_seg, seg_diag));
//
//    // segmento vacío (si se permite)
//    // Segment<...> empty_seg; BOOST_CHECK(!within(empty_seg, seg_diag));
//}
//
//// ============================================================================
//// Segmento vs Línea
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Segment_LineString, WithinTestFixture)
//{
//    // segmento que forma parte de la línea
//    BOOST_CHECK(within(seg_diag, line_diagonal));          // (0,0)-(1,1) está en la línea
//    // segmento que no está completamente en la línea (aunque sus puntos sí)
//    Segment<Point2d> seg_across(line_diagonal[0], line_diagonal[2]); // (0,0)-(2,2) no es un segmento de la línea porque la línea tiene un vértice intermedio; si la línea es recta, geométricamente el segmento sí está contenido, pero según OGC, una línea no contiene un segmento que no sea uno de sus segmentos constituyentes a menos que la línea sea exactamente recta entre esos puntos. En nuestro caso line_diagonal tiene tres puntos colineales, por lo que el segmento (0,0)-(2,2) sí está contenido. Para una línea quebrada, no. Podemos usar una línea quebrada para probar.
//    LineString<Point2d> broken_line({Point2d{0,0},Point2d{1,0},Point2d{1,1}});
//    Segment<Point2d> seg_broken(Point2d(0, 0), Point2d(1, 1)); // diagonal que no sigue la línea quebrada
//    BOOST_CHECK(!within(seg_broken, broken_line));
//
//    // segmento no contenido
//    BOOST_CHECK(!within(seg_horiz, line_diagonal));
//}
//
//// ============================================================================
//// Segmento vs Polígono
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Segment_Polygon, WithinTestFixture)
//{
//    // segmento interior
//    
//    BOOST_CHECK(within(interior_seg, square));
//
//    // segmento que toca el borde pero no sale
//    Segment<Point2d> boundary_seg(Point2d(0.0, 0.5), Point2d(0.5, 0.5)); // desde borde izquierdo hacia interior
//    BOOST_CHECK(within(boundary_seg, square));
//
//    // segmento que cruza el borde (parcialmente fuera)
//    Segment<Point2d> crossing_seg(Point2d(-0.5, 0.5), Point2d(0.5, 0.5));
//    BOOST_CHECK(!within(crossing_seg, square));
//
//    // segmento completamente fuera
//    BOOST_CHECK(!within(seg_horiz, square)); // seg_horiz es (0,0)-(2,0) pero el cuadrado es de 0-1, luego parte fuera? En realidad seg_horiz va de (0,0) a (2,0), está parcialmente fuera, luego no contenido.
//    // Para uno completamente fuera: (2,2)-(3,3)
//    BOOST_CHECK(!within(outside_seg, square));
//}
//
//// ============================================================================
//// Segmento vs MultiLínea / MultiPolígono (brevemente)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Segment_MultiLineString, WithinTestFixture)
//{
//    BOOST_CHECK(within(seg_diag, mls_two));      // contenido en alguna línea
//    BOOST_CHECK(!within(seg_horiz, mls_one));    // no contenido
//}
//
//BOOST_FIXTURE_TEST_CASE(Within_Segment_MultiPolygon, WithinTestFixture)
//{
//    BOOST_CHECK(within(interior_seg, mpoly_one));
//    BOOST_CHECK(!within(outside_seg, mpoly_two));
//}
//
//// ============================================================================
//// Línea vs Línea
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_LineString_LineString, WithinTestFixture)
//{
//    // línea contenida en otra (misma trayectoria)
//    BOOST_CHECK(within(line_partial, line_diagonal));   // (0,0)-(1,1) dentro de (0,0)-(1,1)-(2,2)
//    // línea idéntica
//    BOOST_CHECK(within(line_diagonal, line_diagonal));
//    // línea diferente (no contenida)
//    BOOST_CHECK(!within(line_horizontal, line_diagonal));
//
//    // línea que comparte puntos pero no está contenida (p.ej., línea quebrada)
//    LineString<Point2d> zigzag({Point2d{0,0},Point2d{1,0},Point2d{1,1}});
//    LineString<Point2d> straight({Point2d{0,0},Point2d{1,1}});
//    BOOST_CHECK(!within(straight, zigzag));   // la recta no es subconjunto de la quebrada
//}
//
//// ============================================================================
//// Línea vs Polígono
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_LineString_Polygon, WithinTestFixture)
//{
//    // línea completamente interior
//    LineString<Point2d> inner_line({Point2d{0.2,0.2}, Point2d{0.8,0.8}});
//    BOOST_CHECK(within(inner_line, square));
//
//    // línea que toca el borde pero no sale
//    LineString<Point2d> touch_boundary({Point2d{0.0,0.5}, Point2d{0.5,0.5}});
//    BOOST_CHECK(within(touch_boundary, square));
//
//    // línea que cruza el borde (parcialmente fuera)
//    LineString<Point2d> cross({Point2d{-0.5,0.5}, Point2d{1.5,0.5}});
//    BOOST_CHECK(!within(cross, square));
//
//    // línea completamente fuera
//    LineString<Point2d> outside({Point2d{2,2}, Point2d{3,3}});
//    BOOST_CHECK(!within(outside, square));
//
//    // línea que pasa por un agujero (donut)
//    LineString<Point2d> through_hole({Point2d{0.5,0.5}, Point2d{2.5,2.5}}); // atraviesa agujero y exterior
//    BOOST_CHECK(!within(through_hole, donut));
//    LineString<Point2d> inside_donut_ring({Point2d{2.5,0.5}, Point2d{2.5,2.5}}); // dentro del anillo, fuera del agujero? Depende
//    // mejor no complicar
//}
//
//// ============================================================================
//// Línea vs MultiLínea / MultiPolígono
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_LineString_MultiLineString, WithinTestFixture)
//{
//    BOOST_CHECK(within(line_partial, mls_two));      // contenida en alguna línea
//    BOOST_CHECK(!within(line_horizontal, mls_one));  // no contenida
//}
//
//BOOST_FIXTURE_TEST_CASE(Within_LineString_MultiPolygon, WithinTestFixture)
//{
//    LineString<Point2d> inner_line({Point2d{0.2,0.2}, Point2d{0.8,0.8}});
//    BOOST_CHECK(within(inner_line, mpoly_two));      // dentro del cuadrado
//    // una línea que está dentro del donut
//    LineString<Point2d> donut_line({Point2d{2.2,2.2}, Point2d{2.8,2.8}});
//    BOOST_CHECK(within(donut_line, mpoly_two));      // dentro del donut (anillo)
//}
//
//// ============================================================================
//// Polígono vs Polígono
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Polygon_Polygon, WithinTestFixture)
//{
//    // polígono pequeño dentro de otro
//    Polygon2d small_square(LinearRing<Point2d>({
//        Point2d{0.2,0.2}, Point2d{0.8,0.2}, Point2d{0.8,0.8}, Point2d{0.2,0.8}, Point2d{0.2,0.2}
//        }));
//    BOOST_CHECK(within(small_square, square));
//
//    // polígono igual
//    BOOST_CHECK(within(square, square));
//
//    // polígono que toca el borde pero no sale
//    Polygon2d touching(LinearRing<Point2d>({
//        Point2d{0.0,0.0}, Point2d{0.5,0.0}, Point2d{0.5,0.5}, Point2d{0.0,0.5}, Point2d{0.0,0.0}
//        })); // comparte el borde inferior
//    BOOST_CHECK(within(touching, square));
//
//    // polígono que se sale
//    Polygon2d overlapping(LinearRing<Point2d>({
//        Point2d{0.5,0.5}, Point2d{1.5,0.5}, Point2d{1.5,1.5}, Point2d{0.5,1.5}, Point2d{0.5,0.5}
//        }));
//    BOOST_CHECK(!within(overlapping, square));
//
//    // polígono dentro de donut (en el agujero) -> no está dentro
//    Polygon2d hole_poly(LinearRing<Point2d>({
//        Point2d{1.2,1.2}, Point2d{1.8,1.2}, Point2d{1.8,1.8}, Point2d{1.2,1.8}, Point2d{1.2,1.2}
//        }));
//    BOOST_CHECK(!within(hole_poly, donut));
//}
//
//// ============================================================================
//// Polígono vs MultiPolígono
//// ============================================================================
//
////BOOST_FIXTURE_TEST_CASE(Within_Polygon_MultiPolygon, WithinTestFixture)
////{
////    Polygon2d small_square(/*...*/);
////    BOOST_CHECK(within(small_square, mpoly_two));      // dentro del cuadrado
////    // polígono dentro del donut
////    Polygon2d donut_part(LinearRing<Point2d>({
////        {2.2,2.2}, {2.8,2.2}, {2.8,2.8}, {2.2,2.8}, {2.2,2.2}
////        }));
////    BOOST_CHECK(within(donut_part, mpoly_two));        // dentro del donut
////}
//
//// ============================================================================
//// MultiPunto vs MultiPunto, Línea, Polígono, etc.
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_MultiPoint_MultiPoint, WithinTestFixture)
//{
//    BOOST_CHECK(within(mp_single, mp_double));         // todos los puntos de mp_single (solo p1) están en mp_double
//    BOOST_CHECK(!within(mp_double, mp_single));        // mp_double tiene un punto que no está en mp_single
//    BOOST_CHECK(!within(mp_empty, mp_double));
//    BOOST_CHECK(!within(mp_double, mp_empty));
//}
//
//BOOST_FIXTURE_TEST_CASE(Within_MultiPoint_LineString, WithinTestFixture)
//{
//    MultiPoint<Point2d> mp_on_line({Point2d(0.25,0.25), Point2d(0.75,0.75)});
//    BOOST_CHECK(within(mp_on_line, line_diagonal));    // ambos puntos sobre la línea
//    MultiPoint<Point2d> mp_mixed({p1, p_outside_polygon});
//    BOOST_CHECK(!within(mp_mixed, line_diagonal));     // uno fuera
//}
//
//BOOST_FIXTURE_TEST_CASE(Within_MultiPoint_Polygon, WithinTestFixture)
//{
//    Point2d p_inside2(0.2, 0.8);
//    MultiPoint<Point2d> mp_inside({p_inside_polygon, p_inside2});
//    BOOST_CHECK(within(mp_inside, square));
//    MultiPoint<Point2d> mp_outside({p_inside_polygon, p_outside_polygon});
//    BOOST_CHECK(!within(mp_outside, square));
//}
//
//BOOST_FIXTURE_TEST_CASE(Within_MultiPoint_MultiLineString, WithinTestFixture)
//{
//    MultiPoint<Point2d> mp({Point2d(0.25,0.25), p_on_line});
//    BOOST_CHECK(within(mp, mls_two));
//    MultiPoint<Point2d> mp_bad({p1, Point2d(0.5,0.0)});
//    BOOST_CHECK(!within(mp_bad, mls_two));
//}
//
//BOOST_FIXTURE_TEST_CASE(Within_MultiPoint_MultiPolygon, WithinTestFixture)
//{
//    MultiPoint<Point2d> mp({p_inside_polygon, Point2d(2.5,2.5)}); // segundo dentro del donut
//    BOOST_CHECK(within(mp, mpoly_two));
//    //MultiPoint<Point2d> mp_bad({p_inside_polygon, in_hole}); // in_hole está en el agujero
//    //BOOST_CHECK(!within(mp_bad, mpoly_two));
//}
//
//// ============================================================================
//// MultiLínea vs MultiLínea, Polígono, MultiPolígono
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_MultiLineString_MultiLineString, WithinTestFixture)
//{
//    MultiLineString<Point2d> mls_small({line_partial});
//    BOOST_CHECK(within(mls_small, mls_two));            // una línea contenida en alguna de mls_two
//    MultiLineString<Point2d> mls_bad({line_horizontal});
//    BOOST_CHECK(!within(mls_bad, mls_one));             // horizontal no está en diagonal
//}
//
////BOOST_FIXTURE_TEST_CASE(Within_MultiLineString_Polygon, WithinTestFixture)
////{
////    MultiLineString<Point2d> mls_inside({inner_line, touch_boundary});
////    BOOST_CHECK(within(mls_inside, square));
////    MultiLineString<Point2d> mls_cross({cross});
////    BOOST_CHECK(!within(mls_cross, square));
////}
//
////BOOST_FIXTURE_TEST_CASE(Within_MultiLineString_MultiPolygon, WithinTestFixture)
////{
////    MultiLineString<Point2d> mls_inside({inner_line, donut_line});
////    BOOST_CHECK(within(mls_inside, mpoly_two));
////}
//
//// ============================================================================
//// MultiPolígono vs MultiPolígono
//// ============================================================================
//
////BOOST_FIXTURE_TEST_CASE(Within_MultiPolygon_MultiPolygon, WithinTestFixture)
////{
////    MultiPolygon<Point2d> mp_small({small_square});
////    BOOST_CHECK(within(mp_small, mpoly_two));
////    MultiPolygon<Point2d> mp_bad({hole_poly}); // polígono en el agujero
////    BOOST_CHECK(!within(mp_bad, mpoly_two));
////}
//
//// ============================================================================
//// GeometryCollection (si existe)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_GeometryCollection, WithinTestFixture)
//{
//    // En collection_mixed tenemos p1 (dentro), line_diagonal (parcialmente fuera), square (igual). El resultado debe ser false porque line_diagonal no está dentro.
//    BOOST_CHECK(!within(collection_mixed, square));
//
//    // Colección como segunda geometría: la primera debe estar dentro de la unión
//    BOOST_CHECK(within(p1, collection_mixed));          // p1 está en la colección
//    //BOOST_CHECK(within(small_square, collection_mixed)); // small_square está dentro de square, que está en la colección
//    // Un punto que no está en ningún elemento
//    Point2d p_orphan(10, 10);
//    BOOST_CHECK(!within(p_orphan, collection_mixed));
//    // Línea que está parcialmente dentro pero no completamente en un solo elemento? Dentro de la unión sí, pero nuestra implementación puede requerir que esté contenida en un solo elemento. Probablemente devuelva false si no hay un elemento que la contenga completamente.
//    // BOOST_CHECK(!within(cross, collection_mixed)); // cross cruza square pero no está contenido en él
//
//    // Colección vacía
//    BOOST_CHECK(!within(p1, collection_empty));
//    BOOST_CHECK(!within(collection_empty, square));   // colección vacía dentro de algo? false.
//}
//
//// ============================================================================
//// Pruebas de tolerancia y casos extremos
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Within_Tolerance, WithinTestFixture)
//{
//    // Punto casi sobre la línea
//    BOOST_CHECK(within(Point2d(0.5, 0.5001), line_diagonal, policy));
//    BOOST_CHECK(!within(Point2d(0.5, 0.501), line_diagonal, policy));
//
//    // Segmento casi contenido
//    Segment<Point2d> almost_inside(Point2d(0.0, 0.001), Point2d(1.0, 0.999));
//    BOOST_CHECK(within(almost_inside, square, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Within_EmptyGeometries, WithinTestFixture)
//{
//    Point2d dummy(0, 0);
//    LineString<Point2d> empty_line;
//    Polygon2d empty_poly;  // asumiendo que se puede construir vacío
//    MultiPoint<Point2d> empty_mp;
//    MultiLineString<Point2d> empty_mls;
//    MultiPolygon<Point2d> empty_mpoly;
//
//    // Primera geometría vacía
//    BOOST_CHECK(!within(empty_line, square));    // línea vacía no está dentro de nada (excepto quizá otra vacía)
//    BOOST_CHECK(!within(empty_poly, square));
//    BOOST_CHECK(!within(empty_mp, square));
//
//    // Segunda geometría vacía
//    BOOST_CHECK(!within(dummy, empty_line));
//    BOOST_CHECK(!within(dummy, empty_poly));
//    BOOST_CHECK(!within(dummy, empty_mp));
//
//    // Ambas vacías: ¿debe ser true? OGC no especifica, pero por consistencia podríamos considerar que dos geometrías vacías son iguales, pero dentro no es igual. Normalmente se devuelve false.
//    // BOOST_CHECK(!within(empty_line, empty_line)); // si está implementado
//}

BOOST_AUTO_TEST_SUITE_END()