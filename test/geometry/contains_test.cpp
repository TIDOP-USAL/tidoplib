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
 
#define BOOST_TEST_MODULE Tidop Containts test
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include "geometry_test_fixture.h"
#include "tidop/geometry/algorithms/analysis/Contains.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/primitives/LinearRing.h"
#include "tidop/geometry/base/GeometryCollection.h"

using namespace tl;
using namespace test;

BOOST_AUTO_TEST_SUITE(ContainsTestSuite)

// ============================================================================
// Point - Point
// ============================================================================

BOOST_FIXTURE_TEST_CASE(point_contains_point, GeometryTestFixture)
{
    BOOST_CHECK(contains(point2d1, point2d1));
    BOOST_CHECK(!contains(point2d1, point2d2)); // Diferentes
    BOOST_CHECK(!contains(point2d2, point2d1)); // Simetría
    BOOST_CHECK(!contains(point2d4, point2d5));
    BOOST_CHECK(contains(point2d4, point2d5, policy));
}

BOOST_FIXTURE_TEST_CASE(point_contains_point_with_measure, GeometryTestFixture)
{
    BOOST_CHECK(contains(point2dm1, point2dm1, policy));
    BOOST_CHECK(contains(point2d1, point2dm1, policy));
    BOOST_CHECK(!contains(point2dm1, point2dm2, policy));
}


// ============================================================================
// Segment - Segment
// ============================================================================

BOOST_FIXTURE_TEST_CASE(contains_segment_segment, GeometryTestFixture)
{
    // 1. Segmento que contiene a otro
    BOOST_CHECK(contains(segment9, segment10));
    BOOST_CHECK(!contains(segment10, segment9));

    // 2. Segmento que coincide con otro (iguales)
    BOOST_CHECK(contains(segment1, segment1));

    // 3. Contención con un extremo compartido
    BOOST_CHECK(contains(segment1, segment6));
    BOOST_CHECK(!contains(segment6, segment1));

    // 4. Segmento que sobresale por un extremo
    BOOST_CHECK(!contains(segment7, segment3));
    BOOST_CHECK(!contains(segment3, segment7));

    // 5. Segmento disjunto
    BOOST_CHECK(!contains(segment3, segment_out));

    // 6. Solo comparten un endpoint
    BOOST_CHECK(!contains(segment_colineal_1, segment_colineal_2));

    // 7. Segmento no colineal
    BOOST_CHECK(!contains(segment1, segment2));

    // 8. Segmento degenerado (punto) dentro de otro segmento
    Point2d point(5.0, 5.0);
    Segment2d segment(point, point); // degenerado (punto)
    BOOST_CHECK(contains(segment1, segment));
    // Un segmento degenerado no puede contener un segmento no degenerado
    BOOST_CHECK(!contains(segment, segment1));
    // Un segmento degenerado solo puede contener a sí mismo
    BOOST_CHECK(contains(segment, segment));

    // ------------------------------------------------------------------------
    // Pruebas con política de precisión
    // ------------------------------------------------------------------------

    // Segmento casi contenido (con desviación menor que la resolución)
    // segment3 = (0,0)-(10,0)
    Segment2d seg_almost(Point2d(2, 1e-8), Point2d(8, 1e-8)); // se convierte a y=0 con la política
    BOOST_CHECK(contains(segment3, seg_almost, policy));

    // Segmento casi contenido pero con desviación mayor que la resolución
    Segment2d seg_not_contained(Point2d(2, 0.001), Point2d(8, 0.001));
    BOOST_CHECK(!contains(segment3, seg_not_contained, policy));

    // Segmento que casi empieza en el extremo (dentro de la tolerancia)
    Segment2d seg_touch_start(Point2d(1e-8, 0), Point2d(5, 0));
    BOOST_CHECK(contains(segment3, seg_touch_start, policy));

    // Segmento que casi termina en el extremo final (dentro de la tolerancia)
    Segment2d seg_touch_end(Point2d(5, 0), Point2d(10 - 1e-8, 0));
    BOOST_CHECK(contains(segment3, seg_touch_end, policy));

    // Segmento degenerado casi en el interior
    Segment2d seg_degen_almost(point2d2, point2d2); // (2.5,2.5) ya está, pero probamos con una pequeña desviación
    Point2d p_almost(2.5, 2.5 + 1e-8);
    Segment2d seg_degen_almost2(p_almost, p_almost);
    BOOST_CHECK(contains(segment1, seg_degen_almost2, policy));

    // Segmento degenerado casi en el extremo (debe dar false)
    Point2d p_start_almost(1e-8, 0);
    Segment2d seg_degen_almost_start(p_start_almost, p_start_almost);
    BOOST_CHECK(!contains(segment3, seg_degen_almost_start, policy));

    // ------------------------------------------------------------------------
    // Pruebas con medidas
    // ------------------------------------------------------------------------

    Point2dm pm1(1.0, 2.0, 10.0);
    Point2dm pm2(3.0, 4.0, 20.0);
    Point2dm qm1(1.5, 2.5, 15.0);
    Point2dm qm2(2.5, 3.5, 25.0);
    Segment<Point2dm> segH(pm1, pm2);
    Segment<Point2dm> segI(qm1, qm2);
    BOOST_CHECK(contains(segH, segI)); // ignora medida

}


// ============================================================================
// LineString - LineString
// ============================================================================

BOOST_FIXTURE_TEST_CASE(contains_linestring_linestring, GeometryTestFixture)
{
    // Línea base: una polilínea con varios segmentos: (0,0)-(5,0)-(5,5)-(10,5)
    LineString2d base({point2d1, Point2d(5,0), point2d7, Point2d(10,5)});
    // Nota: point2d7 = (5,5), point2d1 = (0,0)

    // 1. Línea contenida: un subconjunto de segmentos, mismo orden
    LineString2d sub1({point2d1, Point2d(5,0)}); // primer segmento
    BOOST_CHECK(contains(base, sub1));

    // 2. Línea contenida: segmento interior (5,0)-(5,5)
    LineString2d sub2({Point2d(5,0), point2d7});
    BOOST_CHECK(contains(base, sub2));

    // 3. Línea contenida: último segmento (5,5)-(10,5)
    LineString2d sub3({point2d7, Point2d(10,5)});
    BOOST_CHECK(contains(base, sub3));

    // 4. Línea contenida: combinación de varios segmentos (0,0)-(5,0)-(5,5)
    LineString2d sub4({point2d1, Point2d(5,0), point2d7});
    BOOST_CHECK(contains(base, sub4));

    // 5. Línea contenida: todos los segmentos (igual)
    BOOST_CHECK(contains(base, base));

    // 6. Línea contenida con vértices extra colineales (0,0)-(2.5,0)-(5,0)-(5,5)-(7.5,5)-(10,5)
    LineString2d sub5({point2d1, Point2d(2.5,0), Point2d(5,0), point2d7, Point2d(7.5,5), Point2d(10,5)});
    BOOST_CHECK(contains(base, sub5));

    // 7. Línea contenida con orden inverso (debe seguir contenida, porque los puntos están sobre la línea)
    LineString2d sub6({Point2d(10,5), point2d7, Point2d(5,0), point2d1});
    BOOST_CHECK(contains(base, sub6));

    // 8. Línea que se sale por un extremo (0,0)-(12,5) no está sobre base
    LineString2d out1({point2d1, Point2d(12,5)});
    BOOST_CHECK(!contains(base, out1));

    // 9. Línea que tiene un punto fuera (0,0)-(6,0) pero (6,0) no está en base
    LineString2d out2({point2d1, Point2d(6,0)});
    BOOST_CHECK(!contains(base, out2));

    // 10. Línea que se desvía (0,0)-(5,1)-(5,5) → (5,1) no está en base
    LineString2d out3({point2d1, Point2d(5,1), point2d7});
    BOOST_CHECK(!contains(base, out3)); // 

    // 11. Línea cerrada (anillo) y otra línea que es parte de ella
    LineString2d ring({point2d1, Point2d(10,0), Point2d(10,10), Point2d(0,10), point2d1});
    LineString2d ring_part({point2d1, Point2d(10,0), Point2d(10,5)}); // parte del anillo
    BOOST_CHECK(contains(ring, ring_part));

    // 12. Línea cerrada que contiene a otra cerrada más pequeña que es un subconjunto (mismo recorrido)
    LineString2d ring_small({point2d1, Point2d(5,0), Point2d(5,5), Point2d(0,5), point2d1});
    // Pero ring_small no está sobre ring porque sus puntos no están en el mismo camino (por ejemplo (5,5) no está en ring)
    // Para que esté contenida, todos sus puntos deben estar sobre ring. Por ejemplo, una línea que recorre parte del anillo:
    LineString2d ring_arc({point2d1, Point2d(10,0), Point2d(10,10)}); // parte del anillo
    BOOST_CHECK(contains(ring, ring_arc));

    // 13. Línea vacía
    LineString2d empty;
    BOOST_CHECK(!contains(base, empty));
    BOOST_CHECK(!contains(ring, empty));

    
    LineString2d containee7({Point2d(2, 0.0001), Point2d(5, 0.0001), Point2d(5, 5.0001)});
    BOOST_CHECK(contains(base, containee7, policy));

}


// ============================================================================
// Segment - LineString
// ============================================================================

BOOST_FIXTURE_TEST_CASE(contains_segment_linestring, GeometryTestFixture)
{
    Segment2d seg(Point2d(0, 0), Point2d(10, 0));

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Línea con un punto interior
    LineString2d line1({Point2d(5, 0)});
    BOOST_CHECK(contains(seg, line1, policy));

    // Línea con dos puntos interiores
    LineString2d line2({Point2d(2, 0), Point2d(8, 0)});
    BOOST_CHECK(contains(seg, line2, policy));

    // Línea con un punto en extremo (frontera)
    LineString2d line3({Point2d(0, 0), Point2d(5, 0)});
    BOOST_CHECK(!contains(seg, line3, policy));

    // Línea con ambos extremos en los extremos del segmento
    LineString2d line4({Point2d(0, 0), Point2d(10, 0)});
    BOOST_CHECK(!contains(seg, line4, policy));

    // Línea con un punto fuera del rango
    LineString2d line5({Point2d(2, 0), Point2d(12, 0)});
    BOOST_CHECK(!contains(seg, line5, policy));

    // Línea no colineal
    LineString2d line6({Point2d(2, 1), Point2d(8, 1)});
    BOOST_CHECK(!contains(seg, line6, policy));

    // Línea con puntos ligeramente desviados, dentro de tolerancia
    LineString2d line7({Point2d(5, 1e-4), Point2d(6, -1e-4)});
    BOOST_CHECK(contains(seg, line7, policy));

    LineString2d empty_line;
    BOOST_CHECK(!contains(seg, empty_line, policy));
}


// ============================================================================
// LineString - Segment
// ============================================================================

BOOST_FIXTURE_TEST_CASE(contains_linestring_segment, GeometryTestFixture)
{
    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Línea contenedora: (0,0)-(5,0)-(10,0)
    LineString2d container({Point2d(0,0), Point2d(5,0), Point2d(10,0)});

    // Segmento interior (2,0)-(8,0)
    Segment2d seg1(Point2d(2, 0), Point2d(8, 0));
    BOOST_CHECK(contains(container, seg1, policy));

    // Segmento que coincide con un tramo pero incluye un vértice (5,0) (interior) → debe ser true
    Segment2d seg2(Point2d(2, 0), Point2d(5, 0));
    BOOST_CHECK(contains(container, seg2, policy));

    // Segmento que tiene un extremo en la frontera (0,0)
    Segment2d seg3(Point2d(0, 0), Point2d(5, 0));
    BOOST_CHECK(contains(container, seg3, policy));

    // Segmento que tiene ambos extremos en la frontera (0,0)-(10,0)
    Segment2d seg4(Point2d(0, 0), Point2d(10, 0));
    BOOST_CHECK(contains(container, seg4, policy));

    // Segmento con un punto fuera del rango
    Segment2d seg5(Point2d(2, 0), Point2d(12, 0));
    BOOST_CHECK(!contains(container, seg5, policy));

    // Segmento no colineal
    Segment2d seg6(Point2d(2, 0), Point2d(8, 1));
    BOOST_CHECK(!contains(container, seg6, policy));

    // Segmento degenerado (punto) interior
    Segment2d seg7(Point2d(5, 0), Point2d(5, 0));
    BOOST_CHECK(contains(container, seg7, policy));

    // Segmento degenerado en extremo
    Segment2d seg8(Point2d(0, 0), Point2d(0, 0));
    BOOST_CHECK(contains(container, seg8, policy));

    // Segmento ligeramente desviado
    Segment2d seg9(Point2d(2, 1e-4), Point2d(8, -1e-4));
    BOOST_CHECK(contains(container, seg9, policy));

    // Segmento con un extremo muy cerca de la frontera
    Segment2d seg10(Point2d(1e-4, 0), Point2d(5, 0));
    BOOST_CHECK(contains(container, seg10, policy));

    LineString2d linestring({Point2d(0,2), Point2d(2,0), Point2d(4,2)});
    Segment2d seg11(Point2d(1, 1), Point2d(3, 1));
    BOOST_CHECK(!contains(linestring, seg11, policy));

    LineString2d base({point2d1, Point2d(5,0), point2d7, Point2d(10,5)});
    Segment2d segment({Point2d(5,0), point2d7});
    BOOST_CHECK(contains(base, segment));
}


// ============================================================================
// Polygon - Polygon
// ============================================================================

//BOOST_FIXTURE_TEST_CASE(polygon_contains_polygon, GeometryTestFixture)
//{
//    // Polígono grande
//    Polygon2d outer;
//    outer.outer() = {
//        Point2d(0.0, 0.0),
//        Point2d(20.0, 0.0),
//        Point2d(20.0, 20.0),
//        Point2d(0.0, 20.0),
//        Point2d(0.0, 0.0)
//    };
//
//    // Polígono completamente dentro
//    Polygon2d inner;
//    inner.outer() = {
//        Point2d(5.0, 5.0),
//        Point2d(15.0, 5.0),
//        Point2d(15.0, 15.0),
//        Point2d(5.0, 15.0),
//        Point2d(5.0, 5.0)
//    };
//    BOOST_CHECK(contains(square, square_inner));
//    BOOST_CHECK(!contains(square_inner, square));
//
//    // Polígono que toca el borde (no debe estar contenido)
//    Polygon2d touching;
//    touching.outer() = {
//        Point2d(0.0, 5.0),
//        Point2d(10.0, 5.0),
//        Point2d(10.0, 15.0),
//        Point2d(0.0, 15.0),
//        Point2d(0.0, 5.0)
//    };
//    BOOST_CHECK(!contains(outer, touching));
//
//    // Polígono que se solapa
//    Polygon2d overlapping;
//    overlapping.outer() = {
//        Point2d(15.0, 15.0),
//        Point2d(25.0, 15.0),
//        Point2d(25.0, 25.0),
//        Point2d(15.0, 25.0),
//        Point2d(15.0, 15.0)
//    };
//    BOOST_CHECK(!contains(outer, overlapping));
//
//    // Polígono completamente fuera
//    Polygon2d outside;
//    outside.outer() = {
//        Point2d(25.0, 25.0),
//        Point2d(30.0, 25.0),
//        Point2d(30.0, 30.0),
//        Point2d(25.0, 30.0),
//        Point2d(25.0, 25.0)
//    };
//    BOOST_CHECK(!contains(outer, outside));
//}
 
BOOST_FIXTURE_TEST_CASE(polygon_contains_polygon, GeometryTestFixture)
{
    // Polígono base: cuadrado sin agujero
    Polygon2d outer = square;   // (0,0)-(10,0)-(10,10)-(0,10)

    // Caso 1: Polígono pequeño completamente dentro
    Polygon2d inner;
    inner.outer() = {Point2d(2,2), Point2d(8,2), Point2d(8,8), Point2d(2,8), Point2d(2,2)};
    BOOST_CHECK(contains(square, inner));
    BOOST_CHECK(!contains(inner, square));   // no es simétrico

    // Caso 2: Polígono que toca el borde exterior (parte del borde) pero interior dentro
    Polygon2d touch_boundary;
    touch_boundary.outer() = {Point2d(0,0), Point2d(5,0), Point2d(5,5), Point2d(0,5), Point2d(0,0)};
    BOOST_CHECK(contains(square, touch_boundary));   // interior está dentro, toca frontera permitido
    BOOST_CHECK(!contains(touch_boundary, square));

    // Caso 3: Polígono que se solapa parcialmente (no contenido)
    Polygon2d overlap;
    overlap.outer() = {Point2d(5,0), Point2d(15,0), Point2d(15,10), Point2d(5,10), Point2d(5,0)};
    BOOST_CHECK(!contains(square, overlap));
    BOOST_CHECK(!contains(overlap, square));

    // Caso 4: Polígono completamente fuera
    Polygon2d outside;
    outside.outer() = {Point2d(15,15), Point2d(20,15), Point2d(20,20), Point2d(15,20), Point2d(15,15)};
    BOOST_CHECK(!contains(square, outside));

    // ============================================================
    // Polígono con agujero (square_with_hole)

    // Caso 5: Polígono dentro del anillo exterior pero fuera del agujero (contenido)
    Polygon2d valid;
    valid.outer() = {Point2d(1,1), Point2d(2,1), Point2d(2,2), Point2d(1,2), Point2d(1,1)};
    BOOST_CHECK(contains(square_with_hole, valid));

    // Caso 6: Polígono dentro del agujero (no contenido)
    Polygon2d in_hole;
    in_hole.outer() = {Point2d(3,3), Point2d(7,3), Point2d(7,7), Point2d(3,7), Point2d(3,3)};
    BOOST_CHECK(!contains(square_with_hole, in_hole));

    // Caso 7: Polígono que toca el borde del agujero desde fuera (contenido)
    Polygon2d touch_hole_boundary;
    touch_hole_boundary.outer() = {Point2d(2.5,2.5), Point2d(3.5,2.5), Point2d(3.5,3.5), Point2d(2.5,3.5), Point2d(2.5,2.5)};
    BOOST_CHECK(contains(square_with_hole, touch_hole_boundary));   // toca la frontera del agujero, pero interior dentro del anillo exterior

    // Caso 8: Polígono que cruza el agujero (no contenido)
    Polygon2d cross_hole;
    cross_hole.outer() = {Point2d(2,2), Point2d(8,2), Point2d(8,8), Point2d(2,8), Point2d(2,2)};
    BOOST_CHECK(!contains(square_with_hole, cross_hole));

    // Caso 9: Polígono que coincide exactamente con el anillo exterior (contenido? sí, un polígono es contenido por sí mismo)
    BOOST_CHECK(contains(square_with_hole, square_with_hole));

    // Caso 10: Polígono que contiene al agujero pero no es el mismo (no contenido porque el agujero no es parte del interior)
    Polygon2d bigger;
    bigger.outer() = {Point2d(0,0), Point2d(10,0), Point2d(10,10), Point2d(0,10), Point2d(0,0)};
    BOOST_CHECK(!contains(square_with_hole, bigger));   // with_hole no contiene su anillo exterior porque tiene un agujero
}



// Debería comportarse como un poligono. Ahora no lo hace
BOOST_AUTO_TEST_CASE(bbox_contains_point)
{
    BoundingBox2d bbox(Point2d(0.0, 0.0), Point2d(10.0, 10.0));

    // Punto dentro
    BOOST_CHECK(contains(bbox, Point2d(5.0, 5.0)));

    // Punto en borde
    BOOST_CHECK(contains(bbox, Point2d(0.0, 5.0)));
    BOOST_CHECK(contains(bbox, Point2d(10.0, 5.0)));
    BOOST_CHECK(contains(bbox, Point2d(5.0, 0.0)));
    BOOST_CHECK(contains(bbox, Point2d(5.0, 10.0)));

    // Punto fuera
    BOOST_CHECK(!contains(bbox, Point2d(-1.0, 5.0)));
    BOOST_CHECK(!contains(bbox, Point2d(11.0, 5.0)));
    BOOST_CHECK(!contains(bbox, Point2d(5.0, -1.0)));
    BOOST_CHECK(!contains(bbox, Point2d(5.0, 11.0)));

    // Punto en esquina
    BOOST_CHECK(contains(bbox, Point2d(0.0, 0.0)));
    BOOST_CHECK(contains(bbox, Point2d(10.0, 10.0)));
}

BOOST_AUTO_TEST_CASE(polygon_contains_point_basic)
{
    // Cuadrado simple sin huecos
    Polygon2d square;
    square.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Puntos dentro del polígono (interior)
    BOOST_CHECK(contains(square, Point2d(5.0, 5.0)));
    BOOST_CHECK(contains(square, Point2d(1.0, 1.0)));
    BOOST_CHECK(contains(square, Point2d(9.0, 9.0)));

    // Puntos en el borde (según OGC, contains = false para puntos en borde)
    // Para contains, el punto debe estar en el interior, no en la frontera
    BOOST_CHECK(!contains(square, Point2d(0.0, 5.0)));   // Borde izquierdo
    BOOST_CHECK(!contains(square, Point2d(10.0, 5.0)));  // Borde derecho
    BOOST_CHECK(!contains(square, Point2d(5.0, 0.0)));   // Borde inferior
    BOOST_CHECK(!contains(square, Point2d(5.0, 10.0)));  // Borde superior
    BOOST_CHECK(!contains(square, Point2d(0.0, 0.0)));   // Esquina

    // Puntos fuera del polígono
    BOOST_CHECK(!contains(square, Point2d(-1.0, 5.0)));
    BOOST_CHECK(!contains(square, Point2d(11.0, 5.0)));
    BOOST_CHECK(!contains(square, Point2d(5.0, -1.0)));
    BOOST_CHECK(!contains(square, Point2d(5.0, 11.0)));
    BOOST_CHECK(!contains(square, Point2d(15.0, 15.0)));
}

BOOST_AUTO_TEST_CASE(polygon_contains_point_with_holes)
{
    // Cuadrado con un hueco cuadrado en el centro
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

    // Puntos dentro del polígono (fuera del hueco)
    BOOST_CHECK(contains(polygon_with_hole, Point2d(1.0, 1.0)));
    BOOST_CHECK(contains(polygon_with_hole, Point2d(9.0, 9.0)));
    BOOST_CHECK(contains(polygon_with_hole, Point2d(1.0, 5.0)));  // Izquierda del hueco
    BOOST_CHECK(contains(polygon_with_hole, Point2d(9.0, 5.0)));  // Derecha del hueco
    BOOST_CHECK(contains(polygon_with_hole, Point2d(5.0, 1.0)));  // Debajo del hueco
    BOOST_CHECK(contains(polygon_with_hole, Point2d(5.0, 9.0)));  // Encima del hueco

    // Puntos dentro del hueco (NO están contenidos)
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(5.0, 5.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(4.0, 4.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(6.0, 6.0)));

    // Puntos en el borde del hueco (tampoco están contenidos)
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(3.0, 5.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(7.0, 5.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(5.0, 3.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(5.0, 7.0)));

    // Puntos en el borde exterior (tampoco están contenidos)
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(0.0, 5.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(10.0, 5.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(5.0, 0.0)));
    BOOST_CHECK(!contains(polygon_with_hole, Point2d(5.0, 10.0)));
}

BOOST_AUTO_TEST_CASE(polygon_contains_linestring)
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
    BOOST_CHECK(contains(polygon, line1));

    // Línea que toca el borde (no debe estar contenida según OGC)
    LineString2d line2 = {
        Point2d(2.0, 0.0),  // En borde inferior
        Point2d(8.0, 8.0)   // Dentro
    };
    BOOST_CHECK(!contains(polygon, line2));

    // Línea que cruza el borde
    LineString2d line3 = {
        Point2d(-5.0, 5.0),  // Fuera
        Point2d(15.0, 5.0)   // Fuera (pero pasa por dentro)
    };
    BOOST_CHECK(!contains(polygon, line3));

    // Línea completamente fuera
    LineString2d line4 = {
        Point2d(-5.0, -5.0),
        Point2d(-1.0, -1.0)
    };
    BOOST_CHECK(!contains(polygon, line4));

    // Línea con un solo punto (punto dentro)
    LineString2d line5 = {Point2d(5.0, 5.0)};
    BOOST_CHECK(contains(polygon, line5));

    // Línea con un solo punto (punto en borde)
    LineString2d line6 = {Point2d(0.0, 5.0)};
    BOOST_CHECK(!contains(polygon, line6));
}



BOOST_AUTO_TEST_CASE(multipolygon_contains_point)
{
    MultiPolygon2d archipelago;

    // Isla 1
    Polygon2d island1;
    island1.outer() = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(0.0, 5.0),
        Point2d(0.0, 0.0)
    };

    // Isla 2
    Polygon2d island2;
    island2.outer() = {
        Point2d(10.0, 10.0),
        Point2d(15.0, 10.0),
        Point2d(15.0, 15.0),
        Point2d(10.0, 15.0),
        Point2d(10.0, 10.0)
    };

    archipelago.push_back(island1);
    archipelago.push_back(island2);

    // Puntos dentro de las islas
    BOOST_CHECK(contains(archipelago, Point2d(2.0, 2.0)));   // En island1
    BOOST_CHECK(contains(archipelago, Point2d(12.0, 12.0))); // En island2

    // Puntos fuera de todas las islas
    BOOST_CHECK(!contains(archipelago, Point2d(7.0, 7.0)));  // Entre islas
    BOOST_CHECK(!contains(archipelago, Point2d(20.0, 20.0))); // Lejos

    // Puntos en bordes
    BOOST_CHECK(!contains(archipelago, Point2d(0.0, 2.0)));  // Borde de island1
    BOOST_CHECK(!contains(archipelago, Point2d(10.0, 12.0))); // Borde de island2
}

BOOST_AUTO_TEST_CASE(multipolygon_contains_polygon)
{
    MultiPolygon2d multi;

    // Polígono 1
    Polygon2d poly1;
    poly1.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Polígono 2
    Polygon2d poly2;
    poly2.outer() = {
        Point2d(20.0, 20.0),
        Point2d(30.0, 20.0),
        Point2d(30.0, 30.0),
        Point2d(20.0, 30.0),
        Point2d(20.0, 20.0)
    };

    multi.push_back(poly1);
    multi.push_back(poly2);

    // Polígono completamente dentro de poly1
    Polygon2d inner1;
    inner1.outer() = {
        Point2d(2.0, 2.0),
        Point2d(8.0, 2.0),
        Point2d(8.0, 8.0),
        Point2d(2.0, 8.0),
        Point2d(2.0, 2.0)
    };
    BOOST_CHECK(contains(multi, inner1));

    // Polígono completamente dentro de poly2
    Polygon2d inner2;
    inner2.outer() = {
        Point2d(22.0, 22.0),
        Point2d(28.0, 22.0),
        Point2d(28.0, 28.0),
        Point2d(22.0, 28.0),
        Point2d(22.0, 22.0)
    };
    BOOST_CHECK(contains(multi, inner2));

    // Polígono que cruza múltiples polígonos
    Polygon2d crossing;
    crossing.outer() = {
        Point2d(5.0, 5.0),   // En poly1
        Point2d(25.0, 5.0),  // Entre poly1 y poly2
        Point2d(25.0, 25.0), // En poly2
        Point2d(5.0, 25.0),  // Entre poly1 y poly2
        Point2d(5.0, 5.0)
    };
    BOOST_CHECK(!contains(multi, crossing));

    // Polígono completamente fuera
    Polygon2d outside;
    outside.outer() = {
        Point2d(40.0, 40.0),
        Point2d(50.0, 40.0),
        Point2d(50.0, 50.0),
        Point2d(40.0, 50.0),
        Point2d(40.0, 40.0)
    };
    BOOST_CHECK(!contains(multi, outside));
}

// ============================================================================
// TEST CASES PARA GEOMETRÍAS VACÍAS Y CASOS ESPECIALES
// ============================================================================

BOOST_AUTO_TEST_CASE(empty_geometry_contains)
{
    LineString2d empty_line;
    Polygon2d empty_polygon;
    MultiPoint2d empty_multipoint;

    Point2d point(1.0, 1.0);
    LineString2d line = {Point2d(0.0, 0.0), Point2d(2.0, 2.0)};

    BOOST_CHECK(!contains(empty_line, point));
    BOOST_CHECK(!contains(empty_polygon, point));
    BOOST_CHECK(!contains(empty_multipoint, point));

    BOOST_CHECK(!contains(point, empty_line));
    BOOST_CHECK(!contains(line, empty_line));
    BOOST_CHECK(!contains(empty_line, empty_line));
    BOOST_CHECK(!contains(empty_polygon, empty_polygon));
}

BOOST_AUTO_TEST_CASE(degenerate_polygon_contains)
{
    // Polígono degenerado (línea)
    Polygon2d degenerate;
    degenerate.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(0.0, 0.0)  // Cerrado pero sin área
    };

    // Polígono degenerado no debería contener puntos (según OGC)
    // aunque estén "sobre" la línea
    BOOST_CHECK(!contains(degenerate, Point2d(5.0, 0.0)));
    BOOST_CHECK(!contains(degenerate, Point2d(5.0, 1.0)));

    // Polígono con área cero pero más puntos
    Polygon2d zero_area;
    zero_area.outer() = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 5.0),
        Point2d(10.0, 10.0),
        Point2d(5.0, 5.0),
        Point2d(0.0, 0.0)
    };
    BOOST_CHECK(!contains(zero_area, Point2d(5.0, 5.0)));
}

BOOST_AUTO_TEST_CASE(complex_polygon_contains)
{
    // Polígono en forma de estrella
    Polygon2d star;
    star.outer() = {
        Point2d(0.0, 3.0),
        Point2d(2.0, 2.0),
        Point2d(3.0, 0.0),
        Point2d(4.0, 2.0),
        Point2d(6.0, 3.0),
        Point2d(4.0, 4.0),
        Point2d(3.0, 6.0),
        Point2d(2.0, 4.0),
        Point2d(0.0, 3.0)
    };

    // Punto en el centro de la estrella (debería estar dentro)
    BOOST_CHECK(contains(star, Point2d(3.0, 3.0)));

    // Punto en una punta de la estrella (en borde)
    BOOST_CHECK(!contains(star, Point2d(3.0, 0.0)));

    // Punto fuera
    BOOST_CHECK(!contains(star, Point2d(10.0, 10.0)));

    // Punto en concavidad (pero dentro)
    BOOST_CHECK(contains(star, Point2d(2.5, 2.5)));
}

// ============================================================================
// TEST CASES DE SIMETRÍA Y PROPIEDADES
// ============================================================================

BOOST_AUTO_TEST_CASE(contains_symmetry_and_properties)
{
    // contains no es simétrico en general
    Polygon2d big;
    big.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    Polygon2d small;
    small.outer() = {
        Point2d(2.0, 2.0),
        Point2d(8.0, 2.0),
        Point2d(8.0, 8.0),
        Point2d(2.0, 8.0),
        Point2d(2.0, 2.0)
    };

    // contains no es simétrico
    BOOST_CHECK(contains(big, small));
    BOOST_CHECK(!contains(small, big));

    BOOST_CHECK(!contains(big, big));
    BOOST_CHECK(!contains(small, small));

    // Transitividad (si A contiene B y B contiene C, entonces A contiene C)
    Polygon2d tiny;
    tiny.outer() = {
        Point2d(3.0, 3.0),
        Point2d(7.0, 3.0),
        Point2d(7.0, 7.0),
        Point2d(3.0, 7.0),
        Point2d(3.0, 3.0)
    };

    if (contains(big, small) && contains(small, tiny)) {
        BOOST_CHECK(contains(big, tiny));
    }
}

// ============================================================================
// TEST CASES DE RENDIMIENTO Y CASOS LÍMITE
// ============================================================================

BOOST_AUTO_TEST_CASE(contains_large_coordinates)
{
    // Coordenadas grandes
    Polygon2d large_poly;
    large_poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(1000000.0, 0.0),
        Point2d(1000000.0, 1000000.0),
        Point2d(0.0, 1000000.0),
        Point2d(0.0, 0.0)
    };

    // Puntos dentro
    BOOST_CHECK(contains(large_poly, Point2d(500000.0, 500000.0)));
    BOOST_CHECK(contains(large_poly, Point2d(1.0, 1.0)));
    BOOST_CHECK(contains(large_poly, Point2d(999999.0, 999999.0)));

    // Puntos fuera
    BOOST_CHECK(!contains(large_poly, Point2d(-1.0, 500000.0)));
    BOOST_CHECK(!contains(large_poly, Point2d(500000.0, -1.0)));
    BOOST_CHECK(!contains(large_poly, Point2d(1000001.0, 500000.0)));
    BOOST_CHECK(!contains(large_poly, Point2d(500000.0, 1000001.0)));
}

BOOST_AUTO_TEST_CASE(contains_precision_limits)
{
    // Tests de precisión de punto flotante
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(1.0, 0.0),
        Point2d(1.0, 1.0),
        Point2d(0.0, 1.0),
        Point2d(0.0, 0.0)
    };

    // Puntos muy cerca del borde (interior)
    BOOST_CHECK(contains(poly, Point2d(0.0000001, 0.5)));
    BOOST_CHECK(contains(poly, Point2d(0.5, 0.0000001)));
    BOOST_CHECK(contains(poly, Point2d(0.9999999, 0.5)));
    BOOST_CHECK(contains(poly, Point2d(0.5, 0.9999999)));

    // Puntos muy cerca del borde (exterior)
    BOOST_CHECK(!contains(poly, Point2d(-0.0000001, 0.5)));
    BOOST_CHECK(!contains(poly, Point2d(0.5, -0.0000001)));
    BOOST_CHECK(!contains(poly, Point2d(1.0000001, 0.5)));
    BOOST_CHECK(!contains(poly, Point2d(0.5, 1.0000001)));

    // Puntos en el borde (con precisión de máquina)
    Point2d on_border_right(1.0, 0.5);
    Point2d on_border_top(0.5, 1.0);

    // contains debería devolver false para puntos en el borde
    BOOST_CHECK(!contains(poly, on_border_right));
    BOOST_CHECK(!contains(poly, on_border_top));
}

// ============================================================================
// TEST CASES DE REGRESIÓN PARA BUGS ESPECÍFICOS
// ============================================================================

BOOST_AUTO_TEST_CASE(regression_point_on_polygon_vertex)
{
    // Bug común: puntos en vértices de polígono
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Puntos en vértices (esquinas)
    BOOST_CHECK(!contains(poly, Point2d(0.0, 0.0)));    // Esquina inferior izquierda
    BOOST_CHECK(!contains(poly, Point2d(10.0, 0.0)));   // Esquina inferior derecha
    BOOST_CHECK(!contains(poly, Point2d(10.0, 10.0)));  // Esquina superior derecha
    BOOST_CHECK(!contains(poly, Point2d(0.0, 10.0)));   // Esquina superior izquierda
}

BOOST_AUTO_TEST_CASE(regression_polygon_with_collinear_vertices)
{
    // Polígono con vértices colineales
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(5.0, 0.0),  // Colineal
        Point2d(10.0, 0.0),
        Point2d(10.0, 10.0),
        Point2d(0.0, 10.0),
        Point2d(0.0, 0.0)
    };

    // Puntos dentro
    BOOST_CHECK(contains(poly, Point2d(5.0, 5.0)));
    BOOST_CHECK(contains(poly, Point2d(2.0, 2.0)));

    // Puntos en el segmento colineal
    BOOST_CHECK(!contains(poly, Point2d(5.0, 0.0)));  // En vértice colineal
    BOOST_CHECK(!contains(poly, Point2d(2.0, 0.0)));  // En borde colineal
}

BOOST_AUTO_TEST_CASE(regression_multiple_holes_in_polygon)
{
    // Polígono con múltiples huecos
    Polygon2d poly;
    poly.outer() = {
        Point2d(0.0, 0.0),
        Point2d(20.0, 0.0),
        Point2d(20.0, 20.0),
        Point2d(0.0, 20.0),
        Point2d(0.0, 0.0)
    };

    // Hueco 1
    LinearRing2d hole1 = {
        Point2d(2.0, 2.0),
        Point2d(8.0, 2.0),
        Point2d(8.0, 8.0),
        Point2d(2.0, 8.0),
        Point2d(2.0, 2.0)
    };

    // Hueco 2
    LinearRing2d hole2 = {
        Point2d(12.0, 12.0),
        Point2d(18.0, 12.0),
        Point2d(18.0, 18.0),
        Point2d(12.0, 18.0),
        Point2d(12.0, 12.0)
    };

    poly.addInner(hole1);
    poly.addInner(hole2);

    // Puntos fuera de los huecos
    BOOST_CHECK(contains(poly, Point2d(1.0, 1.0)));   // Fuera hole1
    BOOST_CHECK(contains(poly, Point2d(10.0, 10.0))); // Entre huecos
    BOOST_CHECK(contains(poly, Point2d(19.0, 1.0)));  // Derecha de hole1

    // Puntos dentro de los huecos
    BOOST_CHECK(!contains(poly, Point2d(5.0, 5.0)));   // En hole1
    BOOST_CHECK(!contains(poly, Point2d(15.0, 15.0))); // En hole2

    // Puntos en bordes de huecos
    BOOST_CHECK(!contains(poly, Point2d(2.0, 5.0)));   // Borde hole1
    BOOST_CHECK(!contains(poly, Point2d(12.0, 15.0))); // Borde hole2
}


// ============================================================================
// Segment - Point
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_Segment_Point)
{
    Segment2d seg(Point2d(0, 0), Point2d(10, 0));

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Puntos en el interior
    BOOST_CHECK(contains(seg, Point2d(5, 0), policy));
    BOOST_CHECK(contains(seg, Point2d(2.5, 0), policy));

    // Puntos en los extremos (frontera) → false
    BOOST_CHECK(!contains(seg, Point2d(0, 0), policy));
    BOOST_CHECK(!contains(seg, Point2d(10, 0), policy));

    // Puntos fuera, colineales
    BOOST_CHECK(!contains(seg, Point2d(15, 0), policy));
    BOOST_CHECK(!contains(seg, Point2d(-5, 0), policy));

    // Punto no colineal
    BOOST_CHECK(!contains(seg, Point2d(5, 5), policy));

    // Punto cerca del interior, ligeramente desviado
    BOOST_CHECK(contains(seg, Point2d(5, 0.0001), policy));
    BOOST_CHECK(!contains(seg, Point2d(5, 0.01), policy));

    // Punto muy cerca del extremo izquierdo
    // Debe considerarse en la frontera, por tanto false
    BOOST_CHECK(!contains(seg, Point2d(0.0001, 0), policy));

    // Punto cerca del extremo pero con distancia al extremo > tol
    BOOST_CHECK(contains(seg, Point2d(0.01, 0), policy));
}



// ============================================================================
// Contains: LineString - Point
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_LineString_Point)
{
    // Línea abierta de (0,0) a (10,0) con un vértice intermedio en (5,0)
    LineString2d line_open({Point2d(0,0), Point2d(5,0), Point2d(10,0)});

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Puntos en el interior (sobre la línea pero no en vértices extremos)
    BOOST_CHECK(contains(line_open, Point2d(2.5, 0), policy));   // en el primer segmento
    BOOST_CHECK(contains(line_open, Point2d(7.5, 0), policy));   // en el segundo segmento
    BOOST_CHECK(contains(line_open, Point2d(5, 0), policy));     // vértice intermedio (interior)

    // Puntos en los extremos (frontera) → false
    BOOST_CHECK(!contains(line_open, Point2d(0, 0), policy));
    BOOST_CHECK(!contains(line_open, Point2d(10, 0), policy));

    // Puntos fuera, colineales pero más allá de los extremos
    BOOST_CHECK(!contains(line_open, Point2d(-1, 0), policy));
    BOOST_CHECK(!contains(line_open, Point2d(11, 0), policy));

    // Puntos no colineales
    BOOST_CHECK(!contains(line_open, Point2d(5, 1), policy));

    // Línea cerrada (por ejemplo, un triángulo)
    LineString2d line_closed({Point2d(0,0), Point2d(10,0), Point2d(5,10), Point2d(0,0)});
    // En una línea cerrada, todos los puntos de la línea (incluidos los vértices) son interior
    BOOST_CHECK(contains(line_closed, Point2d(0, 0), policy));        // vértice inicial/cierre
    BOOST_CHECK(contains(line_closed, Point2d(10, 0), policy));       // otro vértice
    BOOST_CHECK(contains(line_closed, Point2d(5, 10), policy));       // vértice superior
    BOOST_CHECK(!contains(line_closed, Point2d(5, 5), policy));       // punto en el interior del triángulo
    BOOST_CHECK(!contains(line_closed, Point2d(15, 15), policy));     // Punto fuera

    BOOST_CHECK(contains(line_open, Point2d(2.5, 0.0001), policy));
    BOOST_CHECK(!contains(line_open, Point2d(2.5, 0.01), policy));

    // Punto muy cerca de un extremo, pero con distancia al extremo menor que la tolerancia
    // Debe considerarse en la frontera, por tanto false
    BOOST_CHECK(!contains(line_open, Point2d(1e-4, 0), policy));
    BOOST_CHECK(contains(line_open, Point2d(1e-3, 0), policy));

    // Línea vacía
    LineString2d empty_line;
    BOOST_CHECK(!contains(empty_line, Point2d(0, 0), policy));   // vacío no contiene nada
}

// ============================================================================
// Contains: Polygon - Segment
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_Polygon_Segment)
{
    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Cuadrado unidad (0,0)-(1,0)-(1,1)-(0,1)
    LinearRing2d outer({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)});
    Polygon2d square(outer);

    // Segmento interior (ambos puntos en el interior)
    Segment2d seg1(Point2d(0.2, 0.2), Point2d(0.8, 0.8));
    BOOST_CHECK(contains(square, seg1, policy));

    // Segmento con un punto en el borde (frontera) → false
    Segment2d seg2(Point2d(0.0, 0.5), Point2d(0.5, 0.5));
    BOOST_CHECK(!contains(square, seg2, policy));

    // Segmento con ambos puntos en el borde (sobre el mismo lado) → false
    Segment2d seg3(Point2d(0.0, 0.0), Point2d(1.0, 0.0));
    BOOST_CHECK(!contains(square, seg3, policy));

    // Segmento que cruza el borde (un punto dentro, otro fuera)
    Segment2d seg4(Point2d(0.2, 0.2), Point2d(1.2, 0.2));
    BOOST_CHECK(!contains(square, seg4, policy));

    // Segmento completamente fuera
    Segment2d seg5(Point2d(2, 2), Point2d(3, 3));
    BOOST_CHECK(!contains(square, seg5, policy));

    // Segmento degenerado (punto) interior
    Segment2d seg6(Point2d(0.5, 0.5), Point2d(0.5, 0.5));
    BOOST_CHECK(contains(square, seg6, policy));

    // Segmento degenerado en el borde → false
    Segment2d seg7(Point2d(1.0, 0.5), Point2d(1.0, 0.5));
    BOOST_CHECK(!contains(square, seg7, policy));

    // Polígono con hueco (donut)
    LinearRing2d outer_donut({Point2d(0,0), Point2d(3,0), Point2d(3,3), Point2d(0,3), Point2d(0,0)});
    LinearRing2d inner_donut({Point2d(1,1), Point2d(2,1), Point2d(2,2), Point2d(1,2), Point2d(1,1)});
    Polygon2d donut(outer_donut, {inner_donut});

    // Segmento en el interior del donut (entre agujero y borde exterior)
    Segment2d seg8(Point2d(0.5, 0.5), Point2d(0.5, 2.5));
    BOOST_CHECK(contains(donut, seg8, policy));

    // Segmento que pasa por el agujero → false
    Segment2d seg9(Point2d(0.5, 1.5), Point2d(2.5, 1.5));
    BOOST_CHECK(!contains(donut, seg9, policy));

    // Segmento completamente dentro del agujero → false
    Segment2d seg10(Point2d(1.2, 1.2), Point2d(1.8, 1.8));
    BOOST_CHECK(!contains(donut, seg10, policy));

    // Segmento ligeramente desviado del interior
    Segment2d seg11(Point2d(0.2, 0.2 + 1e-4), Point2d(0.8, 0.8 - 1e-4));
    BOOST_CHECK(contains(square, seg11, policy));

    // Segmento con un punto muy cerca del borde (debido a la precisión se considera borde)
    Segment2d seg12(Point2d(0.2, 0.2), Point2d(0.9999, 0.5));
    BOOST_CHECK(!contains(square, seg12, policy));

    LinearRing2d outer2({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0.5, 0.5), Point2d(0,1), Point2d(0,0)});
    Polygon2d pol(outer2);
    Segment2d seg13(Point2d(0.1, 0.7), Point2d(0.9, 0.7));
    BOOST_CHECK(!contains(pol, seg13, policy));
}

// ============================================================================
// Contains: MultiPoint - Point
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_MultiPoint_Point)
{
    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    MultiPoint2d mp({Point2d(0,0), Point2d(1,1), Point2d(2,2)});

    // Punto existente
    BOOST_CHECK(contains(mp, Point2d(1, 1), policy));
    // Punto no existente
    BOOST_CHECK(!contains(mp, Point2d(3, 3), policy));
    // Punto muy cercano a uno existente, dentro de tolerancia
    BOOST_CHECK(contains(mp, Point2d(1.0001, 1.0001), policy));
    BOOST_CHECK(!contains(mp, Point2d(1.001, 1.001), policy));
    // MultiPoint vacío
    MultiPoint2d empty;
    BOOST_CHECK(!contains(empty, Point2d(0, 0), policy));
}

// ============================================================================
// Contains: MultiLineString - Point
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_MultiLineString_Point)
{
    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    LineString2d l1({Point2d(0,0), Point2d(5,0)});
    LineString2d l2({Point2d(5,0), Point2d(10,0)});
    MultiLineString2d mls({l1, l2});

    // Punto interior de l1
    BOOST_CHECK(contains(mls, Point2d(2, 0), policy));
    // Punto interior de l2
    BOOST_CHECK(contains(mls, Point2d(7, 0), policy));
    // Punto en un extremo común (5,0) – es extremo de ambas líneas, luego frontera → false
    BOOST_CHECK(!contains(mls, Point2d(5, 0), policy));
    // Punto fuera
    BOOST_CHECK(!contains(mls, Point2d(12, 0), policy));
    // Multi vacío
    MultiLineString2d empty;
    BOOST_CHECK(!contains(empty, Point2d(0, 0), policy));
}

// ============================================================================
// Contains: MultiLineString - LineString
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_MultiLineString_LineString)
{
    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    LineString2d l1({Point2d(0,0), Point2d(5,0)});
    LineString2d l2({Point2d(5,0), Point2d(10,0)});
    MultiLineString2d mls({l1, l2});

    // Línea contenida completamente en l1
    LineString2d inside1({Point2d(1,0), Point2d(4,0)});
    BOOST_CHECK(contains(mls, inside1, policy));

    // Línea contenida a caballo entre l1 y l2, pero sin tocar el extremo común (5,0)
    LineString2d across({Point2d(3,0), Point2d(7,0)});
    BOOST_CHECK(!contains(mls, across, policy)); // cada segmento está contenido en alguna línea

    // Línea que incluye el extremo común (5,0) – ese punto es frontera → false
    LineString2d boundary({Point2d(4,0), Point2d(6,0)});
    BOOST_CHECK(!contains(mls, boundary, policy));

    // Línea con un punto fuera
    LineString2d outside({Point2d(3,0), Point2d(12,0)});
    BOOST_CHECK(!contains(mls, outside, policy));

    // Línea vacía
    LineString2d empty_line;
    BOOST_CHECK(!contains(mls, empty_line, policy));
}

// ============================================================================
// Contains: MultiLineString - Segment
// ============================================================================

//BOOST_AUTO_TEST_CASE(Contains_MultiLineString_Segment)
//{
//    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);
//
//    LineString2d l1({Point2d(0,0), Point2d(5,0)});
//    LineString2d l2({Point2d(5,0), Point2d(10,0)});
//    MultiLineString2d mls({l1, l2});
//
//    // Segmento interior de l1
//    Segment2d seg1(Point2d(1, 0), Point2d(4, 0));
//    BOOST_CHECK(contains(mls, seg1, policy));
//
//    // Segmento a caballo entre l1 y l2, sin extremo común
//    Segment2d seg2(Point2d(3, 0), Point2d(7, 0));
//    BOOST_CHECK(!contains(mls, seg2, policy));
//
//    // Segmento que incluye el extremo común (5,0)
//    Segment2d seg3(Point2d(4, 0), Point2d(6, 0));
//    BOOST_CHECK(!contains(mls, seg3, policy));
//
//    // Segmento fuera
//    Segment2d seg4(Point2d(12, 0), Point2d(15, 0));
//    BOOST_CHECK(!contains(mls, seg4, policy));
//
//    // Segmento degenerado (punto) interior
//    Segment2d seg5(Point2d(2, 0), Point2d(2, 0));
//    BOOST_CHECK(contains(mls, seg5, policy));
//
//    // Segmento degenerado en extremo común
//    Segment2d seg6(Point2d(5, 0), Point2d(5, 0));
//    BOOST_CHECK(!contains(mls, seg6, policy));
//}

// ============================================================================
// Contains: MultiPolygon - Point
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_MultiPolygon_Point)
{
    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
    Polygon2d triangle(LinearRing2d({Point2d(2,0), Point2d(3,0), Point2d(2.5,1), Point2d(2,0)}));
    MultiPolygon2d mp({square, triangle});

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Punto dentro del cuadrado
    BOOST_CHECK(contains(mp, Point2d(0.5, 0.5), policy));
    // Punto dentro del triángulo
    BOOST_CHECK(contains(mp, Point2d(2.5, 0.5), policy));
    // Punto en el borde del cuadrado → false
    BOOST_CHECK(!contains(mp, Point2d(0.5, 0), policy));
    // Punto fuera
    BOOST_CHECK(!contains(mp, Point2d(5, 5), policy));
    // Multi vacío
    MultiPolygon2d empty;
    BOOST_CHECK(!contains(empty, Point2d(0, 0), policy));
}

// ============================================================================
// Contains: MultiPolygon - Polygon
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_MultiPolygon_Polygon)
{
    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
    Polygon2d big_square(LinearRing2d({Point2d(0,0), Point2d(2,0), Point2d(2,2), Point2d(0,2), Point2d(0,0)}));
    MultiPolygon2d mp({big_square});  // solo big_square

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Polígono pequeño dentro de big_square
    Polygon2d small(LinearRing2d({Point2d(0.2,0.2), Point2d(0.8,0.2), Point2d(0.8,0.8), Point2d(0.2,0.8), Point2d(0.2,0.2)}));
    BOOST_CHECK(contains(mp, small, policy));

    // Polígono igual a big_square → true (se contiene a sí mismo)
    BOOST_CHECK(!contains(mp, big_square, policy));

    // Polígono que toca el borde de big_square (en el borde) → false
    Polygon2d touching(LinearRing2d({Point2d(0,0), Point2d(0.5,0), Point2d(0.5,0.5), Point2d(0,0.5), Point2d(0,0)}));
    BOOST_CHECK(!contains(mp, touching, policy));

    // Polígono fuera
    Polygon2d outside(LinearRing2d({Point2d(3,3), Point2d(4,3), Point2d(4,4), Point2d(3,4), Point2d(3,3)}));
    BOOST_CHECK(!contains(mp, outside, policy));

    // Multi vacío
    MultiPolygon2d empty;
    BOOST_CHECK(!contains(empty, small, policy));
}

// ============================================================================
// Contains: MultiPolygon - Segment
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_MultiPolygon_Segment)
{
    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
    Polygon2d triangle(LinearRing2d({Point2d(2,0), Point2d(3,0), Point2d(2.5,1), Point2d(2,0)}));
    MultiPolygon2d mp({square, triangle});

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Segmento interior del cuadrado
    Segment2d seg1(Point2d(0.2, 0.2), Point2d(0.8, 0.8));
    BOOST_CHECK(contains(mp, seg1, policy));

    // Segmento interior del triángulo
    Segment2d seg2(Point2d(2.2, 0.2), Point2d(2.8, 0.2));
    BOOST_CHECK(contains(mp, seg2, policy));

    // Segmento con un punto en el borde del cuadrado
    Segment2d seg3(Point2d(0.0, 0.5), Point2d(0.5, 0.5));
    BOOST_CHECK(!contains(mp, seg3, policy));

    // Segmento a caballo entre cuadrado y triángulo (ambos interiores)
    Segment2d seg4(Point2d(0.5, 0.5), Point2d(2.5, 0.5)); // parte fuera (entre 1 y 2)
    BOOST_CHECK(!contains(mp, seg4, policy));

    // Segmento fuera
    Segment2d seg5(Point2d(5, 5), Point2d(6, 6));
    BOOST_CHECK(!contains(mp, seg5, policy));
}

// ============================================================================
// Contains: MultiPolygon - LineString
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_MultiPolygon_LineString)
{
    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
    Polygon2d triangle(LinearRing2d({Point2d(2,0), Point2d(3,0), Point2d(2.5,1), Point2d(2,0)}));
    MultiPolygon2d mp({square, triangle});

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    // Línea completamente dentro del cuadrado
    LineString2d line1({Point2d(0.2,0.2), Point2d(0.8,0.8)});
    BOOST_CHECK(contains(mp, line1, policy));

    // Línea dentro del triángulo
    LineString2d line2({Point2d(2.2,0.2), Point2d(2.8,0.2)});
    BOOST_CHECK(contains(mp, line2, policy));

    // Línea que toca el borde del cuadrado (punto en borde) → false
    LineString2d line3({Point2d(0.2,0.2), Point2d(0.5,0.0)});
    BOOST_CHECK(!contains(mp, line3, policy));

    // Línea que atraviesa ambos polígonos (parte en cuadrado, parte en triángulo, pero con un hueco entre 1 y 2) → false
    LineString2d line4({Point2d(0.5,0.5), Point2d(2.5,0.5)});
    BOOST_CHECK(!contains(mp, line4, policy));

    // Línea vacía
    LineString2d empty;
    BOOST_CHECK(!contains(mp, empty, policy));
}

// ============================================================================
// Contains: GeometryCollection - Point / LineString / Segment / Polygon
// ============================================================================

BOOST_AUTO_TEST_CASE(Contains_GeometryCollection_Point)
{
    Point2d p(1, 1);
    LineString2d l({Point2d(0,0), Point2d(2,2)});
    Polygon2d poly(LinearRing2d({Point2d(0,0), Point2d(3,0), Point2d(3,3), Point2d(0,3), Point2d(0,0)}));

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);

    GeometryCollection<Point2d> col;
    col.add(p);
    col.add(l);
    col.add(poly);

    double tol = 1e-9;

    // Punto igual al elemento puntual
    BOOST_CHECK(contains(col, Point2d(1, 1), policy));
    // Punto contenido en la línea (interior)
    BOOST_CHECK(contains(col, Point2d(1, 1), policy)); // ya probado, pero otro punto
    BOOST_CHECK(contains(col, Point2d(0.5, 0.5), policy));
    // Punto contenido en el polígono (interior)
    BOOST_CHECK(contains(col, Point2d(1.5, 1.5), policy));
    // Punto en el borde del polígono → false (no está en interior)
    BOOST_CHECK(!contains(col, Point2d(0, 0), policy));
    // Punto fuera de todo
    BOOST_CHECK(!contains(col, Point2d(10, 10), policy));

    // Colección vacía
    GeometryCollection<Point2d> empty;
    BOOST_CHECK(!contains(empty, Point2d(0, 0), policy));
}

//BOOST_AUTO_TEST_CASE(Contains_GeometryCollection_LineString)
//{
//    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);
//
//    Point2d p(1, 1);
//    LineString2d l1({Point2d(0,0), Point2d(5,5)});
//    Polygon2d poly(LinearRing2d({Point2d(0,0), Point2d(4,0), Point2d(4,4), Point2d(0,4), Point2d(0,0)}));
//    GeometryCollection<Point2d> col;
//    col.add(p);
//    col.add(l1);
//    col.add(poly);
//
//    // Línea contenida en l1 (misma dirección, interior)
//    LineString2d inside_l({Point2d(1,1), Point2d(2,2)});
//    BOOST_CHECK(contains(col, inside_l, policy));
//
//    // Línea contenida en poly
//    LineString2d inside_poly({Point2d(1,1), Point2d(3,3)});
//    BOOST_CHECK(contains(col, inside_poly, policy));
//
//    // Línea que toca el borde de poly (punto en borde) → false
//    LineString2d touch_poly({Point2d(0,0), Point2d(2,2)});
//    BOOST_CHECK(!contains(col, touch_poly, policy));
//
//    // Línea que cruza elementos pero no está completamente contenida en uno solo
//    LineString2d cross({Point2d(2,2), Point2d(6,6)}); // parte en poly/l1, parte fuera
//    BOOST_CHECK(!contains(col, cross, policy));
//
//    // Línea vacía
//    LineString2d empty;
//    BOOST_CHECK(!contains(col, empty, policy));
//}

//BOOST_AUTO_TEST_CASE(Contains_GeometryCollection_Segment)
//{
//    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);
//
//    Point2d p(1, 1);
//    LineString2d l({Point2d(0,0), Point2d(5,5)});
//    Polygon2d poly(LinearRing2d({Point2d(0,0), Point2d(4,0), Point2d(4,4), Point2d(0,4), Point2d(0,0)}));
//    GeometryCollection<Point2d> col;
//    col.add(p);
//    col.add(l);
//    col.add(poly);
//
//    // Segmento interior de l
//    Segment2d seg_l(Point2d(1, 1), Point2d(2, 2));
//    BOOST_CHECK(contains(col, seg_l, policy));
//
//    // Segmento interior de poly
//    Segment2d seg_poly(Point2d(1, 1), Point2d(3, 3));
//    BOOST_CHECK(contains(col, seg_poly, policy));
//
//    // Segmento que toca borde de poly
//    Segment2d seg_touch(Point2d(0, 0), Point2d(2, 2));
//    BOOST_CHECK(!contains(col, seg_touch, policy));
//
//    // Segmento fuera
//    Segment2d seg_out(Point2d(10, 10), Point2d(12, 12));
//    BOOST_CHECK(!contains(col, seg_out, policy));
//}
//
//BOOST_AUTO_TEST_CASE(Contains_GeometryCollection_Polygon)
//{
//    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy(0.001);
//
//    Point2d p(1, 1);
//    LineString2d l({Point2d(0,0), Point2d(5,5)});
//    Polygon2d poly(LinearRing2d({Point2d(0,0), Point2d(4,0), Point2d(4,4), Point2d(0,4), Point2d(0,0)}));
//    GeometryCollection<Point2d> col;
//    col.add(p);
//    col.add(l);
//    col.add(poly);
//
//    // Polígono pequeño dentro de poly
//    Polygon2d small(LinearRing2d({Point2d(1,1), Point2d(3,1), Point2d(3,3), Point2d(1,3), Point2d(1,1)}));
//    BOOST_CHECK(contains(col, small, policy));
//
//    // Polígono igual a poly → true
//    BOOST_CHECK(!contains(col, poly, policy));
//
//    // Polígono que toca el borde de poly (en el borde) → false
//    Polygon2d touching(LinearRing2d({Point2d(0,0), Point2d(2,0), Point2d(2,2), Point2d(0,2), Point2d(0,0)}));
//    BOOST_CHECK(!contains(col, touching, policy));
//
//    // Polígono fuera
//    Polygon2d outside(LinearRing2d({Point2d(10,10), Point2d(12,10), Point2d(12,12), Point2d(10,12), Point2d(10,10)}));
//    BOOST_CHECK(!contains(col, outside, policy));
//
//    // Polígono vacío
//    Polygon2d empty_poly;
//    BOOST_CHECK(!contains(col, empty_poly, policy));
//}
//
//// ============================================================================
//// TEST CASES ADICIONALES DE PERFORMANCE
//// ============================================================================
//
//BOOST_AUTO_TEST_CASE(contains_performance_large_polygon)
//{
//    // Polígono con muchos vértices (100 vértices)
//    Polygon2d large_poly;
//
//    // Crear un círculo aproximado con muchos vértices
//    const int num_vertices = 100;
//    const double radius = 10.0;
//    const double center_x = 0.0;
//    const double center_y = 0.0;
//
//    for (int i = 0; i <= num_vertices; ++i) {
//        double angle = 2.0 * consts::pi<double> * i / num_vertices;
//        double x = center_x + radius * cos(angle);
//        double y = center_y + radius * sin(angle);
//        large_poly.outer().push_back(Point2d(x, y));
//    }
//    // Cerrar el polígono
//    if (!large_poly.outer().isEmpty()) {
//        large_poly.outer().push_back(large_poly.outer()[0]);
//    }
//
//    // Puntos dentro del círculo
//    BOOST_CHECK(contains(large_poly, Point2d(0.0, 0.0)));        // Centro
//    BOOST_CHECK(contains(large_poly, Point2d(5.0, 0.0)));        // Cerca del centro
//    BOOST_CHECK(contains(large_poly, Point2d(0.0, 5.0)));        // Cerca del centro
//
//    // Puntos fuera del círculo
//    BOOST_CHECK(!contains(large_poly, Point2d(15.0, 0.0)));      // Fuera en X
//    BOOST_CHECK(!contains(large_poly, Point2d(0.0, 15.0)));      // Fuera en Y
//    BOOST_CHECK(!contains(large_poly, Point2d(10.1, 10.1)));     // Fuera en diagonal
//}


BOOST_AUTO_TEST_SUITE_END()