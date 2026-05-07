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
 
#define BOOST_TEST_MODULE Tidop algorithms overlaps test
#include <boost/test/unit_test.hpp>

#include "geometry_test_fixture.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/analysis/Overlaps.h"

using namespace tl; 
using namespace test;

BOOST_AUTO_TEST_SUITE(OverlapsAlgorithmTest)


// ============================================================================
// Point - Point
// ============================================================================
// Siempre falso, ya que un punto no puede solaparse con otro punto (pueden ser 
// iguales o disjuntos, pero no solapados)
// ============================================================================

BOOST_FIXTURE_TEST_CASE(overlaps_point_point, GeometryTestFixture)
{
    BOOST_CHECK(!overlaps(point2d1, point2d2));
    BOOST_CHECK(!overlaps(point2d2, point2d1));
    BOOST_CHECK(!overlaps(point2d1, point2d1));

    BOOST_CHECK(!overlaps(point2d4, point2d5));
    BOOST_CHECK(!overlaps(point2d4, point2d5, policy));
}

BOOST_FIXTURE_TEST_CASE(overlaps_point_point_with_measure, GeometryTestFixture)
{
    BOOST_CHECK(!overlaps(point2d1, point2dm1));
    BOOST_CHECK(!overlaps(point2dm1, point2dm2));
}


// ============================================================================
// Segment - Segment
// ============================================================================

BOOST_FIXTURE_TEST_CASE(overlaps_segment_segment, GeometryTestFixture)
{

    // 1. Segmentos que se cruzan en un punto interior
    BOOST_CHECK(!overlaps(segment1, segment4));   // diagonal (0,0)-(10,10) y horizontal (0,5)-(10,5) se cruzan en (5,5)
    BOOST_CHECK(!overlaps(segment4, segment1));   // simetría

    // 2. Segmentos colineales que intersectan en un extremo pero no se solapan
    BOOST_CHECK(!overlaps(segment_colineal_1, segment_colineal_2)); // (0,0)-(5,0) y (5,0)-(10,0) tocan en (5,0)
    BOOST_CHECK(!overlaps(segment_colineal_2, segment_colineal_1));

    // 3. Segmentos iguales (mismos puntos)
    Segment2d seg_dup(point2d1, point2d3);
    BOOST_CHECK(!overlaps(segment1, seg_dup));
    BOOST_CHECK(!overlaps(seg_dup, segment1));

    // 4. Segmentos colineales con superposición parcial
    BOOST_CHECK(overlaps(segment7, segment3));
    BOOST_CHECK(overlaps(segment3, segment7));

    // 5. Segmentos colineales donde uno contiene al otro
    BOOST_CHECK(!overlaps(segment9, segment10));
    BOOST_CHECK(!overlaps(segment10, segment9));

    // 6. Segmentos que comparten un punto interior de uno y extremo del otro
    BOOST_CHECK(!overlaps(segment4, segment2));
    BOOST_CHECK(!overlaps(segment2, segment4));

    // 7. Segmento degenerado (punto) que está sobre otro segmento
    // Overlaps requiere que ambos segmentos tengan una parte no degenerada, 
    // por lo que un segmento degenerado (punto) no se considera que se solape 
    // con otro segmento, aunque ese punto esté sobre el otro segmento. Por eso se espera false aquí.
    Segment2d seg_degen_on(point2d2, point2d2); // (2.5,2.5) que está sobre segment1
    BOOST_CHECK(!overlaps(segment1, seg_degen_on));
    BOOST_CHECK(!overlaps(seg_degen_on, segment1));

    // ------------------------------------------------------------------------
    // Casos donde NO hay overlaps (ningún punto en común)
    // ------------------------------------------------------------------------

    // 8. Segmentos paralelos y separados
    BOOST_CHECK(!overlaps(segment4, segment3));
    BOOST_CHECK(!overlaps(segment3, segment4));

    // 9. Segmentos no paralelos que no se cruzan
    BOOST_CHECK(!overlaps(segment1, segment_out));
    BOOST_CHECK(!overlaps(segment_out, segment1));

    // 10. Segmentos colineales pero separados (con hueco)
    Segment2d seg9(Point2d(0, 0), Point2d(5, 0));
    Segment2d seg10(Point2d(7, 0), Point2d(10, 0));
    BOOST_CHECK(!overlaps(seg9, seg10));
    BOOST_CHECK(!overlaps(seg10, seg9));

    // ------------------------------------------------------------------------
    // Pruebas con política de precisión (puntos casi coincidentes)
    // ------------------------------------------------------------------------
    
    // Segmento paralelo casi contenido (con desviación menor que la resolución)
    Segment2d seg_almost(Point2d(0.0001, 0.0001), Point2d(10.0001, 0.0001));
    BOOST_CHECK(overlaps(segment7, seg_almost, policy));

    // Segmento paralelo casi contenido pero con desviación mayor que la resolución
    Segment2d seg_not_contained(Point2d(0.001, 0.001), Point2d(10.001, 0.001));
    BOOST_CHECK(!overlaps(segment7, seg_not_contained, policy));

    // Segmento que cruza pero esta dentro de la precisión
    Segment2d seg_crosses(Point2d(-0.0001, -0.0001), Point2d(10.0001, 0.0001));
    BOOST_CHECK(overlaps(segment7, seg_crosses, policy));
}


// ============================================================================
// LineString - LineString
// ============================================================================

BOOST_FIXTURE_TEST_CASE(overlaps_linestring_linestring, GeometryTestFixture)
{
    // Para que dos líneas se solapen (overlaps), deben tener la misma dimensión (1) y la intersección debe tener dimensión 1
    // y no deben estar una contenida en la otra (eso sería contains/within).
    // Además, deben ser del mismo tipo (línea-línea) y la intersección debe ser unidimensional.

    // 1. Segmentos colineales con superposición parcial (no contención total)
    LineString2d line_a({point2d1, Point2d(10,0)});      // (0.0, 0.0) - (10,0)
    LineString2d line_b({Point2d(2,0), Point2d(12,0)});  // (2.0, 0.0) - (12,0)
    BOOST_CHECK(overlaps(line_a, line_b));
    BOOST_CHECK(overlaps(line_b, line_a));

    // 2. Superposición total (una contenida en la otra) → no overlaps
    LineString2d line_c({point2d1, Point2d(10,0)});
    LineString2d line_d({Point2d(3,0), Point2d(7,0)});
    BOOST_CHECK(!overlaps(line_c, line_d)); // porque line_d está contenida en line_c

    // 3. Líneas que se cruzan en un punto (dimension 0) → no overlaps
    LineString2d line_horiz({point2d1, Point2d(10,0)});
    LineString2d line_vert({Point2d(5,0), Point2d(5,10)});
    BOOST_CHECK(!overlaps(line_horiz, line_vert));

    // 4. Líneas que no se tocan → no overlaps
    LineString2d line_par({Point2d(0,1), Point2d(10,1)});
    BOOST_CHECK(!overlaps(line_horiz, line_par));

    // 5. Líneas colineales que solo se tocan en un punto → no overlaps
    LineString2d line_left({point2d1, Point2d(5,0)});
    LineString2d line_right({Point2d(5,0), Point2d(10,0)});
    BOOST_CHECK(!overlaps(line_left, line_right));

    // 6. Líneas iguales (misma geometría) → no overlaps
    LineString2d line_dup({point2d1, Point2d(10,0)});
    BOOST_CHECK(!overlaps(line_a, line_dup));

    // 7. Línea con un solo punto (degenerada) con otra línea que la contiene → no overlaps (dimensión 0 vs 1)
    LineString2d line_point({point2d2});
    BOOST_CHECK(!overlaps(line_a, line_point));
    BOOST_CHECK(!overlaps(line_point, line_a));


    // ============================================================================
    // Casos con líneas cerradas (anillos)
    // ============================================================================

    // Cuadrado cerrado (0,0)-(10,0)-(10,10)-(0,10)-(0,0)
    LineString2d square({point2d1, Point2d(10,0), Point2d(10,10), Point2d(0,10), point2d1});

    // 1. Línea cerrada que comparte un lado completo con el cuadrado → superposición unidimensional
    LineString2d shared_side({point2d1, Point2d(10,0)}); // lado inferior del cuadrado
    // Aunque shared_side es abierta, la intersección con square es el segmento (0,0)-(10,0), que tiene dimensión 1
    // pero shared_side es un subconjunto de square (contenido), no es overlaps porque una está contenida en la otra
    BOOST_CHECK(!overlaps(square, shared_side));
    BOOST_CHECK(!overlaps(shared_side, square));

    // 2. Dos cuadrados que se superponen parcialmente en un área (pero como líneas, solo el contorno)
    // Cuadrado2: (5,-5)-(15,-5)-(15,5)-(5,5)-(5,-5)
    LineString2d square2({Point2d(5,-5), Point2d(15,-5), Point2d(15,5), Point2d(5,5), Point2d(5,-5)});
    // La intersección entre los contornos de ambos cuadrados es el segmento (5,0)-(10,0)??? No, el cuadrado2 está desplazado.
    // En realidad, el primer cuadrado es (0,0)-(10,0)-(10,10)-(0,10)
    // El segundo es (5,-5)-(15,-5)-(15,5)-(5,5)
    // Se intersecan en el segmento (5,0)-(10,0)? No, (5,0) está en el borde inferior del primer cuadrado, pero el segundo cuadrado tiene su lado inferior en y=-5, luego no se tocan.
    // Mejor usar otro ejemplo más claro.

    // ============================================================================
    // Ejemplo más claro: dos anillos que comparten un segmento
    // ============================================================================

    // Anillo A: (0,0)-(10,0)-(10,5)-(0,5)-(0,0)
    LineString2d ringA({point2d1, Point2d(10,0), Point2d(10,5), Point2d(0,5), point2d1});

    // Anillo B: (5,0)-(15,0)-(15,5)-(5,5)-(5,0)
    LineString2d ringB({Point2d(5,0), Point2d(15,0), Point2d(15,5), Point2d(5,5), Point2d(5,0)});

    // La intersección es el segmento (5,0)-(10,0) (dimensión 1). Ninguno contiene al otro (cada uno sobresale).
    // Por tanto, debe ser overlaps.
    BOOST_CHECK(overlaps(ringA, ringB));
    BOOST_CHECK(overlaps(ringB, ringA));

    // ============================================================================
    // Múltiples zonas de superposición (dos anillos que se cruzan en dos segmentos)
    // ============================================================================

    // Anillo C: (0,0)-(10,0)-(10,10)-(0,10)-(0,0)
    LineString2d ringC({point2d1, Point2d(10,0), Point2d(10,10), Point2d(0,10), point2d1});

    // Anillo D: (5,5)-(15,5)-(15,15)-(5,15)-(5,5)
    LineString2d ringD({Point2d(5,5), Point2d(15,5), Point2d(15,15), Point2d(5,15), Point2d(5,5)});

    // La intersección es el segmento (10,5)-(10,10)? No, el anillo C tiene borde derecho en x=10, desde y=0 a y=10.
    // El anillo D tiene borde izquierdo en x=5, desde y=5 a y=15. Luego la intersección es el punto (10,5)? No, (10,5) no está en ringD.
    // Mejor usar un ejemplo que genere dos segmentos de intersección.

    // ============================================================================
    // Ejemplo con dos líneas que se solapan en dos segmentos separados
    // ============================================================================

    // Línea en zigzag que va y vuelve sobre sí misma (no cerrada)
    LineString2d zigzag_overlap({point2d1, Point2d(5,0), Point2d(5,5), Point2d(10,5), Point2d(10,0), Point2d(15,0)});
    // Otra línea que se superpone en los tramos (0,0)-(5,0) y (10,0)-(15,0)
    LineString2d horiz_overlap({point2d1, Point2d(15,0)});

    // La intersección tiene dos segmentos: [0,5] y [10,15], ambos con dimensión 1.
    // Ninguna línea contiene a la otra (horiz_overlap tiene puntos en (5,0)-(10,0) que no están en zigzag_overlap? En zigzag_overlap, (5,0)-(10,0) no existe porque va (5,0)-(5,5)-(10,5)-(10,0). Luego en (5,0)-(10,0) no hay puntos de zigzag_overlap, excepto los extremos (5,0) y (10,0). Por tanto, la intersección no es un segmento continuo, son dos segmentos separados.
    // Para overlaps, se requiere que la intersección tenga dimensión 1. Aquí la intersección tiene dos componentes de dimensión 1, pero sigue siendo de dimensión 1. ¿Se considera overlaps? Sí, porque la intersección no es puntual. Además, ninguna está contenida en la otra (horiz_overlap sobresale en el medio). Por tanto, debe ser overlaps.
    BOOST_CHECK(overlaps(zigzag_overlap, horiz_overlap));
    BOOST_CHECK(overlaps(horiz_overlap, zigzag_overlap));

    // ============================================================================
    // Caso donde se superponen en un segmento pero también se tocan en un punto
    // ============================================================================

    LineString2d line_long({point2d1, Point2d(10,0)});
    LineString2d line_T({Point2d(5,0), Point2d(5,5)});
    // La intersección es el punto (5,0) (dimensión 0) → no overlaps
    BOOST_CHECK(!overlaps(line_long, line_T));

    // ============================================================================
    // Línea cerrada con otra abierta que comparte un segmento completo
    // ============================================================================

    // Cuadrado (0,0)-(10,0)-(10,10)-(0,10)-(0,0)
    LineString2d square3({point2d1, Point2d(10,0), Point2d(10,10), Point2d(0,10), point2d1});
    // Segmento que coincide con el lado inferior
    LineString2d bottom_side({point2d1, Point2d(10,0)});

    // bottom_side está contenida en square3, no es overlaps
    BOOST_CHECK(!overlaps(square3, bottom_side));
    BOOST_CHECK(!overlaps(bottom_side, square3));

    // ============================================================================
    // Caso con polígonos complejos (estrella) que se superponen en múltiples segmentos
    // ============================================================================

    // Estrella (forma de X) como línea abierta que se cruza a sí misma
    LineString2d star({point2d1, Point2d(10,10), Point2d(0,10), Point2d(10,0), point2d1});
    // Otra línea que sigue el mismo recorrido pero desplazada
    LineString2d star_shifted({Point2d(1,1), Point2d(9,9), Point2d(1,9), Point2d(9,1), Point2d(1,1)});
    // Se intersecta así que es falso
    BOOST_CHECK(!overlaps(star, star_shifted));
    BOOST_CHECK(!overlaps(star_shifted, star));
}


// ============================================================================
// Polygon - Polygon
// ============================================================================

//BOOST_FIXTURE_TEST_CASE(polygon_polygon_overlaps, GeometryTestFixture)
//{
//    // Polígono base: cuadrado sin agujero
//    Polygon2d poly1 = square;   // (0,0)-(10,0)-(10,10)-(0,10)
//
//    // ------------------------------------------------------------
//    // Casos con dos polígonos sin agujeros
//    // ------------------------------------------------------------
//
//    // 1. Solapamiento parcial (intersección área) → overlaps
//    Polygon2d overlap;
//    overlap.outer() = {Point2d(5,0), Point2d(15,0), Point2d(15,10), Point2d(5,10), Point2d(5,0)};
//    BOOST_CHECK(overlaps(poly1, overlap, policy));
//    BOOST_CHECK(overlaps(overlap, poly1, policy));
//
//    // 2. Un polígono contenido completamente en el otro → no overlaps
//    Polygon2d inside;
//    inside.outer() = {Point2d(2,2), Point2d(8,2), Point2d(8,8), Point2d(2,8), Point2d(2,2)};
//    BOOST_CHECK(!overlaps(poly1, inside, policy));
//    BOOST_CHECK(!overlaps(inside, poly1, policy));
//
//    // 3. Polígonos iguales → no overlaps
//    BOOST_CHECK(!overlaps(poly1, poly1, policy));
//
//    // 4. Tocar en un vértice (intersección punto) → no overlaps
//    Polygon2d touch_vertex;
//    touch_vertex.outer() = {Point2d(10,10), Point2d(15,10), Point2d(15,15), Point2d(10,15), Point2d(10,10)};
//    BOOST_CHECK(!overlaps(poly1, touch_vertex, policy));
//
//    // 5. Tocar en un borde (intersección segmento) → no overlaps
//    Polygon2d touch_edge;
//    touch_edge.outer() = {Point2d(10,2), Point2d(15,2), Point2d(15,8), Point2d(10,8), Point2d(10,2)};
//    BOOST_CHECK(!overlaps(poly1, touch_edge, policy));
//
//    // 6. Disjuntos → no overlaps
//    Polygon2d separate;
//    separate.outer() = {Point2d(15,15), Point2d(20,15), Point2d(20,20), Point2d(15,20), Point2d(15,15)};
//    BOOST_CHECK(!overlaps(poly1, separate, policy));
//
//    // ------------------------------------------------------------
//    // Casos con polígono que tiene un agujero (square_with_hole)
//    // ------------------------------------------------------------
//    Polygon2d poly_hole = square_with_hole;   // exterior (0,0)-(10,0)-(10,10)-(0,10), agujero (2.5,2.5)-(7.5,2.5)-(7.5,7.5)-(2.5,7.5)
//
//    // 7. Polígono que solapa parcialmente el anillo exterior y el agujero (intersección es el área fuera del agujero)
//    //    Este polígono cubre parte del anillo exterior y parte del agujero (pero el agujero no pertenece al polígono con agujero).
//    //    La intersección real es la parte del polígono que está en el anillo exterior (fuera del agujero).
//    Polygon2d overlap_with_hole;
//    overlap_with_hole.outer() = {Point2d(2,2), Point2d(8,2), Point2d(8,8), Point2d(2,8), Point2d(2,2)};
//    // Este polígono cruza el agujero: parte está dentro del agujero (no intersecta) y parte fuera. La intersección es un "donut" (anillo entre agujero y exterior).
//    // Esa intersección tiene área, y no es contenida por poly_hole (porque poly_hole tiene un agujero que falta) ni overlap_with_hole está contenida.
//    // Por lo tanto, debería ser overlaps.
//    BOOST_CHECK(overlaps(poly_hole, overlap_with_hole, policy));
//    BOOST_CHECK(overlaps(overlap_with_hole, poly_hole, policy));
//
//    // 8. Polígono completamente dentro del agujero → no overlaps (intersección vacía)
//    Polygon2d in_hole;
//    in_hole.outer() = {Point2d(3,3), Point2d(7,3), Point2d(7,7), Point2d(3,7), Point2d(3,3)};
//    BOOST_CHECK(!overlaps(poly_hole, in_hole, policy));
//
//    // 9. Polígono que toca el borde del agujero desde fuera (intersección segmento) → no overlaps
//    Polygon2d touch_hole_edge;
//    touch_hole_edge.outer() = {Point2d(2.5,2.5), Point2d(3.5,2.5), Point2d(3.5,3.5), Point2d(2.5,3.5), Point2d(2.5,2.5)};
//    // La intersección es el segmento de borde del agujero (dimensión 1) → no overlaps
//    BOOST_CHECK(!overlaps(poly_hole, touch_hole_edge, policy));
//
//    // 10. Polígono contenido en el anillo exterior pero que no toca el agujero (contenido) → no overlaps
//    Polygon2d inside_outer_ring;
//    inside_outer_ring.outer() = {Point2d(1,1), Point2d(2,1), Point2d(2,2), Point2d(1,2), Point2d(1,1)};
//    BOOST_CHECK(!overlaps(poly_hole, inside_outer_ring, policy));
//    BOOST_CHECK(!overlaps(inside_outer_ring, poly_hole, policy));
//}

BOOST_AUTO_TEST_SUITE_END()