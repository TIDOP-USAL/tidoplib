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

#include "geometry_test_fixture.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/algorithms/analysis/Crosses.h"

using namespace tl; 
using namespace test;

BOOST_AUTO_TEST_SUITE(CrossesAlgorithmTest)


struct CrossesTestFixture
{
    CrossesTestFixture()
        : policy(0.001)   // resolución 0.001
    {
    }

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(3.0, 4.0);
        point3d1 = Point3d(1.0, 2.0, 3.0);
        point3d2 = Point3d(4.0, 5.0, 6.0);
        point2dm1 = Point2dm(1.0, 2.0, 10.0);
        point2dm2 = Point2dm(3.0, 4.0, 20.0);

        square = Polygon2d(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
    }

    void teardown() {}

    Point2d point2d1;
    Point2d point2d2;
    Point3d point3d1;
    Point3d point3d2;
    Point2dm point2dm1;
    Point2dm point2dm2;
    Polygon2d square;

    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy;
};

// ============================================================================
// Point - Point
// ============================================================================
// Point - Point: Siempre falso
// No se cruzan, aunque sean iguales (se contiene a sí mismo, pero no se cruza)
// ============================================================================

BOOST_FIXTURE_TEST_CASE(crosses_point_point, GeometryTestFixture)
{
    BOOST_CHECK(!crosses(point2d1, point2d1));
    BOOST_CHECK(!crosses(point2d1, point2d1, policy));
    BOOST_CHECK(!crosses(point2d1, point2d2, policy));
}

BOOST_FIXTURE_TEST_CASE(crosses_point_point_with_measure, GeometryTestFixture)
{
    BOOST_CHECK(!crosses(point2dm1, point2dm2, policy));
}

// ============================================================================
// Segment - Segment
// ============================================================================

BOOST_FIXTURE_TEST_CASE(crosses_segment_segment_crossing, GeometryTestFixture)
{
    // Horizontal (0,0)-(10,0) y vertical (5,-5)-(5,5) se tocan en (5,0)
    BOOST_CHECK(crosses(segment3, segment17, policy));
    BOOST_CHECK(crosses(segment17, segment3, policy));
}

BOOST_FIXTURE_TEST_CASE(crosses_segment_segment_endpoint, GeometryTestFixture)
{
    // Horizontal (0,5)-(10,5) y vertical desde (0,0) a (0,10) → cruce en (0,5)
    BOOST_CHECK(!crosses(segment2, segment4, policy));
    BOOST_CHECK(!crosses(segment4, segment2, policy));
}

BOOST_FIXTURE_TEST_CASE(crosses_segment_segment_at_both_endpoints, GeometryTestFixture)
{
    // Dos segmentos que comparten un extremo (colineales)
    BOOST_CHECK(!crosses(segment_colineal_1, segment_colineal_2, policy));
    BOOST_CHECK(!crosses(segment_colineal_2, segment_colineal_1, policy));

    // Dos segmentos que comparten un extremo (no colineales)
    BOOST_CHECK(!crosses(segment1, segment2, policy));
    BOOST_CHECK(!crosses(segment2, segment1, policy));
}

BOOST_FIXTURE_TEST_CASE(crosses_segment_segment_colinear_overlap, GeometryTestFixture)
{
    // Segmentos colineales superpuestos → intersección es un segmento, no un punto
    BOOST_CHECK(!crosses(segment9, segment10, policy));
    BOOST_CHECK(!crosses(segment10, segment9, policy));
}

BOOST_FIXTURE_TEST_CASE(crosses_segment_segment_no_crosses, GeometryTestFixture)
{
    BOOST_CHECK(!crosses(segment_out, segment2, policy));
    BOOST_CHECK(!crosses(segment2, segment_out, policy));
}

BOOST_FIXTURE_TEST_CASE(crosses_segment_with_measure, GeometryTestFixture)
{
    // Segmentos con medida (se ignora la M, solo coordenadas espaciales)
    Point2dm p1(0, 0, 10), p2(10, 0, 20);
    Point2dm q1(5, -5, 30), q2(5, 5, 40);
    Segment<Point2dm> seg1(p1, p2);
    Segment<Point2dm> seg2(q1, q2);
    BOOST_CHECK(crosses(seg1, seg2, policy));
    BOOST_CHECK(crosses(seg2, seg1, policy));
}




//
//
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Point_Segment_Inside, CrossesTestFixture)
//{
//    Point2d p(5, 0);  // punto medio del segmento (0,0)-(10,0)
//    Segment<Point2d> seg(Point2d(0, 0), Point2d(10, 0));
//
//    // Si una geometría está completamente contenida en la otra → NO es crosses.
//    BOOST_CHECK(!crosses(p, seg, policy));
//    BOOST_CHECK(!crosses(seg, p, policy));  // simetría
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Point_Segment_Endpoint, CrossesTestFixture)
//{
//    Point2d p(0, 0);  // extremo del segmento
//    Segment<Point2d> seg(Point2d(0, 0), Point2d(10, 0));
//    BOOST_CHECK(!crosses(p, seg, policy));
//    BOOST_CHECK(!crosses(seg, p, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Point_Segment_Outside, CrossesTestFixture)
//{
//    Point2d p(5, 1);  // fuera del segmento
//    Segment<Point2d> seg(Point2d(0, 0), Point2d(10, 0));
//    BOOST_CHECK(!crosses(p, seg, policy));
//    BOOST_CHECK(!crosses(seg, p, policy));
//}
//
//
//// ============================================================================
//// Crosses: LineString - LineString (intersección en punto interior)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_LineString_Crossing, CrossesTestFixture)
//{
//    // Línea diagonal (0,0)-(2,2) y línea horizontal (0,1)-(2,1) se cruzan en (1,1)
//    LineString2d line1({Point2d(0,0), Point2d(2,2)});
//    LineString2d line2({Point2d(0,1), Point2d(2,1)});
//    BOOST_CHECK(crosses(line1, line2));
//    BOOST_CHECK(crosses(line2, line1));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_LineString_TouchAtEndpoint, CrossesTestFixture)
//{
//    // Líneas que se tocan en un extremo (una termina en la otra)
//    LineString2d line1({Point2d(0,0), Point2d(2,0)});
//    LineString2d line2({Point2d(2,0), Point2d(3,1)});
//    BOOST_CHECK(!crosses(line1, line2));
//    BOOST_CHECK(!crosses(line2, line1));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_LineString_Overlap, CrossesTestFixture)
//{
//    // Líneas colineales superpuestas parcialmente
//    LineString2d line1({Point2d(0,0), Point2d(4,0)});
//    LineString2d line2({Point2d(2,0), Point2d(6,0)});
//    BOOST_CHECK(!crosses(line1, line2));
//    BOOST_CHECK(!crosses(line2, line1));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_LineString_ShareVertex, CrossesTestFixture)
//{
//    // Comparten un vértice intermedio pero no se cruzan (ángulo)
//    LineString2d line1({Point2d(0,0), Point2d(2,0), Point2d(4,0)}); // horizontal
//    LineString2d line2({Point2d(2,0), Point2d(2,2)}); // vertical en el vértice
//    // Intersección en (2,0) que es vértice de line1 (frontera) y extremo de line2 (frontera)
//    BOOST_CHECK(!crosses(line1, line2));
//    BOOST_CHECK(!crosses(line2, line1));
//}
//
//// ============================================================================
//// Crosses: LineString - Segment
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_Segment_Crossing, CrossesTestFixture)
//{
//    LineString2d line({Point2d(0,0), Point2d(2,2)});
//    Segment2d seg(Point2d(0, 1), Point2d(2, 1));
//    BOOST_CHECK(crosses(line, seg));
//    BOOST_CHECK(crosses(seg, line)); // simétrico
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_Segment_TouchAtEndpoint, CrossesTestFixture)
//{
//    LineString2d line({Point2d(0,0), Point2d(2,0)});
//    Segment2d seg(Point2d(2, 0), Point2d(3, 1));
//    BOOST_CHECK(!crosses(line, seg));
//    BOOST_CHECK(!crosses(seg, line));
//}
//
//// ============================================================================
//// Crosses: Segment - Segment
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Segment_Tolerance, CrossesTestFixture)
//{
//    Segment2d hor(Point2d(0, 0), Point2d(10, 0));
//    Segment2d ver(Point2d(5, -0.001), Point2d(5, 0.001));
//
//    BOOST_CHECK(crosses(hor, ver, policy));
//    BOOST_CHECK(crosses(ver, hor, policy));
//
//    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy_small(0.01);
//    BOOST_CHECK(!crosses(hor, ver, policy_small));
//    BOOST_CHECK(!crosses(ver, hor, policy_small));
//}
//
//// ============================================================================
//// Crosses: LineString - Polygon
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_Polygon_Crossing, CrossesTestFixture)
//{
//    // Línea diagonal que atraviesa el cuadrado (0,0)-(1,1) -> debe cruzarlo
//    LineString2d line({Point2d(-1,-1), Point2d(2,2)});
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(crosses(line, square, policy));
//    BOOST_CHECK(!crosses(square, line, policy)); // no simétrico
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_Polygon_TouchBoundary, CrossesTestFixture)
//{
//    // Línea que solo toca el borde del polígono en un punto
//    LineString2d line({Point2d(-1,0), Point2d(0,0)}); // termina en el borde
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(!crosses(line, square, policy));
//    BOOST_CHECK(!crosses(square, line, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_Polygon_Inside, CrossesTestFixture)
//{
//    // Línea completamente dentro del polígono
//    LineString2d line({Point2d(0.2,0.2), Point2d(0.8,0.8)});
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(!crosses(line, square, policy));
//    BOOST_CHECK(!crosses(square, line, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_Polygon_Outside, CrossesTestFixture)
//{
//    // Línea completamente fuera
//    LineString2d line({Point2d(2,2), Point2d(3,3)});
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(!crosses(line, square, policy));
//    BOOST_CHECK(!crosses(square, line, policy));
//}
//
//// ============================================================================
//// Crosses: Segment - Polygon (análogo a LineString-Polygon)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Polygon_Crossing, CrossesTestFixture)
//{
//    Segment2d seg(Point2d(-1, -1), Point2d(2, 2));
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(crosses(seg, square, policy));
//    BOOST_CHECK(!crosses(square, seg, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Segment_Polygon_TouchBoundary, CrossesTestFixture)
//{
//    Segment2d seg(Point2d(-1, 0), Point2d(0, 0));
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(!crosses(seg, square, policy));
//    BOOST_CHECK(!crosses(square, seg, policy));
//}
//
//// ============================================================================
//// Crosses: LineString - MultiLineString
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_MultiLineString_Crossing, CrossesTestFixture)
//{
//    LineString2d line({Point2d(-1,-1), Point2d(2,2)});
//    MultiLineString2d mls({LineString2d({Point2d(0,1), Point2d(2,1)}), LineString2d({Point2d(1,0), Point2d(1,2)})});
//    BOOST_CHECK(crosses(line, mls, policy));
//    BOOST_CHECK(crosses(mls, line, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_MultiLineString_Touch, CrossesTestFixture)
//{
//    LineString2d line({Point2d(0,0), Point2d(2,0)});
//    MultiLineString2d mls({LineString2d({Point2d(2,0), Point2d(3,1)}), LineString2d({Point2d(1,1), Point2d(1,2)})});
//    BOOST_CHECK(!crosses(line, mls, policy));
//    BOOST_CHECK(!crosses(mls, line, policy));
//}
//
//// ============================================================================
//// Crosses: LineString - MultiPolygon
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_MultiPolygon_Crossing, CrossesTestFixture)
//{
//    LineString2d line({Point2d(-1,-1), Point2d(2,2)});
//    MultiPolygon2d mpoly({square}); // square definido en el fixture
//    BOOST_CHECK(crosses(line, mpoly, policy));
//    BOOST_CHECK(!crosses(mpoly, line, policy));
//}
//
//// Linea totalmente dentro del polígono → no se cruza según OGC, aunque sí interseccionan
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_MultiPolygon_Inside, CrossesTestFixture)
//{
//    LineString2d line({Point2d(0.2,0.2), Point2d(0.8,0.8)});
//    MultiPolygon2d mpoly({square});
//    BOOST_CHECK(!crosses(line, mpoly, policy));
//    BOOST_CHECK(!crosses(mpoly, line, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_LineString_Polygon_MultipleCrossings, CrossesTestFixture)
//{
//    // cuadrado
//    Polygon2d square(
//        LinearRing2d({
//            Point2d(0,0),
//            Point2d(10,0),
//            Point2d(10,10),
//            Point2d(0,10),
//            Point2d(0,0)
//            })
//    );
//
//    // línea que entra y sale dos veces
//    LineString2d line({
//        Point2d(-5,5),
//        Point2d(5,5),
//        Point2d(5,15),
//        Point2d(15,5)
//        });
//
//    BOOST_CHECK(crosses(line, square, policy));
//}
//
//// ============================================================================
//// Crosses: MultiLineString - MultiLineString
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_MultiLineString_MultiLineString_Crossing, CrossesTestFixture)
//{
//    MultiLineString2d mls1({LineString2d({Point2d(0,0), Point2d(2,2)})});
//    MultiLineString2d mls2({LineString2d({Point2d(0,1), Point2d(2,1)})});
//    BOOST_CHECK(crosses(mls1, mls2, policy));
//    BOOST_CHECK(crosses(mls2, mls1, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_MultiLineString_MultiLineString_NoCross, CrossesTestFixture)
//{
//    MultiLineString2d mls1({LineString2d({Point2d(0,0), Point2d(2,0)})});
//    MultiLineString2d mls2({LineString2d({Point2d(0,1), Point2d(2,1)})});
//    BOOST_CHECK(!crosses(mls1, mls2, policy));
//    BOOST_CHECK(!crosses(mls2, mls1, policy));
//}
//
//// ============================================================================
//// Crosses: MultiLineString - Polygon
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_MultiLineString_Polygon_Crossing, CrossesTestFixture)
//{
//    MultiLineString2d mls({LineString2d({Point2d(-1,-1), Point2d(2,2)})});
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(crosses(mls, square, policy));
//    BOOST_CHECK(!crosses(square, mls, policy));
//}
//
//// ============================================================================
//// Crosses: MultiPolygon - MultiPolygon? OGC no define crosses para dos superficies
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_MultiPolygon_MultiPolygon, CrossesTestFixture)
//{
//    MultiPolygon2d mpoly1({square});
//    MultiPolygon2d mpoly2({Polygon2d(LinearRing2d({Point2d(0.5,0.5), Point2d(1.5,0.5), Point2d(1.5,1.5), Point2d(0.5,1.5), Point2d(0.5,0.5)}))});
//    BOOST_CHECK(!crosses(mpoly1, mpoly2, policy));
//    BOOST_CHECK(!crosses(mpoly2, mpoly1, policy));
//}
//
//// ============================================================================
//// Crosses: Punto con cualquier otra cosa (siempre false)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Point_LineString, CrossesTestFixture)
//{
//    Point2d p(0, 0);
//    LineString2d line({Point2d(0,0), Point2d(10,0)});
//    BOOST_CHECK(!crosses(p, line, policy));
//    BOOST_CHECK(!crosses(line, p, policy));
//}
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Point_Polygon, CrossesTestFixture)
//{
//    Point2d p(0.5, 0.5);
//    Polygon2d square(LinearRing2d({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)}));
//    BOOST_CHECK(!crosses(p, square, policy));
//    BOOST_CHECK(!crosses(square, p, policy));
//}
//
//// ============================================================================
//// Crosses con geometrías vacías (siempre false)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_EmptyGeometries, CrossesTestFixture)
//{
//    LineString2d empty_line;
//    Polygon2d empty_poly;
//    MultiLineString2d empty_mls;
//    BOOST_CHECK(!crosses(empty_line, square, policy));
//    BOOST_CHECK(!crosses(square, empty_line, policy));
//    BOOST_CHECK(!crosses(empty_line, empty_line, policy));
//}
//
//// ============================================================================
//// Crosses con tolerancia (adaptado a políticas con resolución)
//// ============================================================================
//
//BOOST_FIXTURE_TEST_CASE(Crosses_Tolerance, CrossesTestFixture)
//{
//    Segment2d hor(Point2d(0, 0), Point2d(10, 0));
//    Segment2d ver(Point2d(5, -1e-8), Point2d(5, 1e-8));
//
//    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy_large(1e-6);
//    BOOST_CHECK(crosses(hor, ver, policy_large));
//
//    PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy_small(1e-10);
//    BOOST_CHECK(!crosses(hor, ver, policy_small));
//}

BOOST_AUTO_TEST_SUITE_END()