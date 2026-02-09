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

#include "tidop/geometry/algorithms/analysis/Contains.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/base/GeometryCollection.h"

using namespace tl;

BOOST_AUTO_TEST_SUITE(ContainsTestSuite)

// ============================================================================
// TEST CASES BÁSICOS
// ============================================================================

BOOST_AUTO_TEST_CASE(point_contains_point)
{
    // Punto contiene a otro punto solo si son iguales
    tl::Point2d p1(1.0, 2.0);
    tl::Point2d p2(1.0, 2.0);
    tl::Point2d p3(3.0, 4.0);

    BOOST_CHECK(tl::contains(p1, p2));  // Iguales
    BOOST_CHECK(tl::contains(p2, p1));  // Simetría para iguales
    BOOST_CHECK(!tl::contains(p1, p3)); // Diferentes
    BOOST_CHECK(!tl::contains(p3, p1));
}

BOOST_AUTO_TEST_CASE(bbox_contains_point)
{
    tl::BoundingBox2d bbox(tl::Point2d(0.0, 0.0), tl::Point2d(10.0, 10.0));

    // Punto dentro
    BOOST_CHECK(tl::contains(bbox, tl::Point2d(5.0, 5.0)));

    // Punto en borde
    BOOST_CHECK(tl::contains(bbox, tl::Point2d(0.0, 5.0)));
    BOOST_CHECK(tl::contains(bbox, tl::Point2d(10.0, 5.0)));
    BOOST_CHECK(tl::contains(bbox, tl::Point2d(5.0, 0.0)));
    BOOST_CHECK(tl::contains(bbox, tl::Point2d(5.0, 10.0)));

    // Punto fuera
    BOOST_CHECK(!tl::contains(bbox, tl::Point2d(-1.0, 5.0)));
    BOOST_CHECK(!tl::contains(bbox, tl::Point2d(11.0, 5.0)));
    BOOST_CHECK(!tl::contains(bbox, tl::Point2d(5.0, -1.0)));
    BOOST_CHECK(!tl::contains(bbox, tl::Point2d(5.0, 11.0)));

    // Punto en esquina
    BOOST_CHECK(tl::contains(bbox, tl::Point2d(0.0, 0.0)));
    BOOST_CHECK(tl::contains(bbox, tl::Point2d(10.0, 10.0)));
}

BOOST_AUTO_TEST_CASE(polygon_contains_point_basic)
{
    // Cuadrado simple sin huecos
    tl::Polygon2d square;
    square.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    // Puntos dentro del polígono (interior)
    BOOST_CHECK(tl::contains(square, tl::Point2d(5.0, 5.0)));
    BOOST_CHECK(tl::contains(square, tl::Point2d(1.0, 1.0)));
    BOOST_CHECK(tl::contains(square, tl::Point2d(9.0, 9.0)));

    // Puntos en el borde (según OGC, contains = false para puntos en borde)
    // Para contains, el punto debe estar en el interior, no en la frontera
    BOOST_CHECK(!tl::contains(square, tl::Point2d(0.0, 5.0)));   // Borde izquierdo
    BOOST_CHECK(!tl::contains(square, tl::Point2d(10.0, 5.0)));  // Borde derecho
    BOOST_CHECK(!tl::contains(square, tl::Point2d(5.0, 0.0)));   // Borde inferior
    BOOST_CHECK(!tl::contains(square, tl::Point2d(5.0, 10.0)));  // Borde superior
    BOOST_CHECK(!tl::contains(square, tl::Point2d(0.0, 0.0)));   // Esquina

    // Puntos fuera del polígono
    BOOST_CHECK(!tl::contains(square, tl::Point2d(-1.0, 5.0)));
    BOOST_CHECK(!tl::contains(square, tl::Point2d(11.0, 5.0)));
    BOOST_CHECK(!tl::contains(square, tl::Point2d(5.0, -1.0)));
    BOOST_CHECK(!tl::contains(square, tl::Point2d(5.0, 11.0)));
    BOOST_CHECK(!tl::contains(square, tl::Point2d(15.0, 15.0)));
}

BOOST_AUTO_TEST_CASE(polygon_contains_point_with_holes)
{
    // Cuadrado con un hueco cuadrado en el centro
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

    // Puntos dentro del polígono (fuera del hueco)
    BOOST_CHECK(tl::contains(polygon_with_hole, tl::Point2d(1.0, 1.0)));
    BOOST_CHECK(tl::contains(polygon_with_hole, tl::Point2d(9.0, 9.0)));
    BOOST_CHECK(tl::contains(polygon_with_hole, tl::Point2d(1.0, 5.0)));  // Izquierda del hueco
    BOOST_CHECK(tl::contains(polygon_with_hole, tl::Point2d(9.0, 5.0)));  // Derecha del hueco
    BOOST_CHECK(tl::contains(polygon_with_hole, tl::Point2d(5.0, 1.0)));  // Debajo del hueco
    BOOST_CHECK(tl::contains(polygon_with_hole, tl::Point2d(5.0, 9.0)));  // Encima del hueco

    // Puntos dentro del hueco (NO están contenidos)
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(5.0, 5.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(4.0, 4.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(6.0, 6.0)));

    // Puntos en el borde del hueco (tampoco están contenidos)
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(3.0, 5.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(7.0, 5.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(5.0, 3.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(5.0, 7.0)));

    // Puntos en el borde exterior (tampoco están contenidos)
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(0.0, 5.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(10.0, 5.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(5.0, 0.0)));
    BOOST_CHECK(!tl::contains(polygon_with_hole, tl::Point2d(5.0, 10.0)));
}

BOOST_AUTO_TEST_CASE(polygon_contains_linestring)
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
    BOOST_CHECK(tl::contains(polygon, line1));

    // Línea que toca el borde (no debe estar contenida según OGC)
    tl::LineString2d line2 = {
        tl::Point2d(2.0, 0.0),  // En borde inferior
        tl::Point2d(8.0, 8.0)   // Dentro
    };
    BOOST_CHECK(!tl::contains(polygon, line2));

    // Línea que cruza el borde
    tl::LineString2d line3 = {
        tl::Point2d(-5.0, 5.0),  // Fuera
        tl::Point2d(15.0, 5.0)   // Fuera (pero pasa por dentro)
    };
    BOOST_CHECK(!tl::contains(polygon, line3));

    // Línea completamente fuera
    tl::LineString2d line4 = {
        tl::Point2d(-5.0, -5.0),
        tl::Point2d(-1.0, -1.0)
    };
    BOOST_CHECK(!tl::contains(polygon, line4));

    // Línea con un solo punto (punto dentro)
    tl::LineString2d line5 = {tl::Point2d(5.0, 5.0)};
    BOOST_CHECK(tl::contains(polygon, line5));

    // Línea con un solo punto (punto en borde)
    tl::LineString2d line6 = {tl::Point2d(0.0, 5.0)};
    BOOST_CHECK(!tl::contains(polygon, line6));
}

BOOST_AUTO_TEST_CASE(polygon_contains_polygon)
{
    // Polígono grande
    tl::Polygon2d outer;
    outer.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(20.0, 0.0),
        tl::Point2d(20.0, 20.0),
        tl::Point2d(0.0, 20.0),
        tl::Point2d(0.0, 0.0)
    };

    // Polígono completamente dentro
    tl::Polygon2d inner;
    inner.outer() = {
        tl::Point2d(5.0, 5.0),
        tl::Point2d(15.0, 5.0),
        tl::Point2d(15.0, 15.0),
        tl::Point2d(5.0, 15.0),
        tl::Point2d(5.0, 5.0)
    };
    BOOST_CHECK(tl::contains(outer, inner));
    BOOST_CHECK(!tl::contains(inner, outer));

    // Polígono que toca el borde (no debe estar contenido)
    tl::Polygon2d touching;
    touching.outer() = {
        tl::Point2d(0.0, 5.0),   // En borde de outer
        tl::Point2d(10.0, 5.0),
        tl::Point2d(10.0, 15.0),
        tl::Point2d(0.0, 15.0),
        tl::Point2d(0.0, 5.0)
    };
    BOOST_CHECK(!tl::contains(outer, touching));

    // Polígono que se solapa
    tl::Polygon2d overlapping;
    overlapping.outer() = {
        tl::Point2d(15.0, 15.0),
        tl::Point2d(25.0, 15.0),
        tl::Point2d(25.0, 25.0),
        tl::Point2d(15.0, 25.0),
        tl::Point2d(15.0, 15.0)
    };
    BOOST_CHECK(!tl::contains(outer, overlapping));

    // Polígono completamente fuera
    tl::Polygon2d outside;
    outside.outer() = {
        tl::Point2d(25.0, 25.0),
        tl::Point2d(30.0, 25.0),
        tl::Point2d(30.0, 30.0),
        tl::Point2d(25.0, 30.0),
        tl::Point2d(25.0, 25.0)
    };
    BOOST_CHECK(!tl::contains(outer, outside));
}

BOOST_AUTO_TEST_CASE(multipolygon_contains_point)
{
    tl::MultiPolygon2d archipelago;

    // Isla 1
    tl::Polygon2d island1;
    island1.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(5.0, 0.0),
        tl::Point2d(5.0, 5.0),
        tl::Point2d(0.0, 5.0),
        tl::Point2d(0.0, 0.0)
    };

    // Isla 2
    tl::Polygon2d island2;
    island2.outer() = {
        tl::Point2d(10.0, 10.0),
        tl::Point2d(15.0, 10.0),
        tl::Point2d(15.0, 15.0),
        tl::Point2d(10.0, 15.0),
        tl::Point2d(10.0, 10.0)
    };

    archipelago.push_back(island1);
    archipelago.push_back(island2);

    // Puntos dentro de las islas
    BOOST_CHECK(tl::contains(archipelago, tl::Point2d(2.0, 2.0)));   // En island1
    BOOST_CHECK(tl::contains(archipelago, tl::Point2d(12.0, 12.0))); // En island2

    // Puntos fuera de todas las islas
    BOOST_CHECK(!tl::contains(archipelago, tl::Point2d(7.0, 7.0)));  // Entre islas
    BOOST_CHECK(!tl::contains(archipelago, tl::Point2d(20.0, 20.0))); // Lejos

    // Puntos en bordes
    BOOST_CHECK(!tl::contains(archipelago, tl::Point2d(0.0, 2.0)));  // Borde de island1
    BOOST_CHECK(!tl::contains(archipelago, tl::Point2d(10.0, 12.0))); // Borde de island2
}

BOOST_AUTO_TEST_CASE(multipolygon_contains_polygon)
{
    tl::MultiPolygon2d multi;

    // Polígono 1
    tl::Polygon2d poly1;
    poly1.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    // Polígono 2
    tl::Polygon2d poly2;
    poly2.outer() = {
        tl::Point2d(20.0, 20.0),
        tl::Point2d(30.0, 20.0),
        tl::Point2d(30.0, 30.0),
        tl::Point2d(20.0, 30.0),
        tl::Point2d(20.0, 20.0)
    };

    multi.push_back(poly1);
    multi.push_back(poly2);

    // Polígono completamente dentro de poly1
    tl::Polygon2d inner1;
    inner1.outer() = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(8.0, 2.0),
        tl::Point2d(8.0, 8.0),
        tl::Point2d(2.0, 8.0),
        tl::Point2d(2.0, 2.0)
    };
    BOOST_CHECK(tl::contains(multi, inner1));

    // Polígono completamente dentro de poly2
    tl::Polygon2d inner2;
    inner2.outer() = {
        tl::Point2d(22.0, 22.0),
        tl::Point2d(28.0, 22.0),
        tl::Point2d(28.0, 28.0),
        tl::Point2d(22.0, 28.0),
        tl::Point2d(22.0, 22.0)
    };
    BOOST_CHECK(tl::contains(multi, inner2));

    // Polígono que cruza múltiples polígonos
    tl::Polygon2d crossing;
    crossing.outer() = {
        tl::Point2d(5.0, 5.0),   // En poly1
        tl::Point2d(25.0, 5.0),  // Entre poly1 y poly2
        tl::Point2d(25.0, 25.0), // En poly2
        tl::Point2d(5.0, 25.0),  // Entre poly1 y poly2
        tl::Point2d(5.0, 5.0)
    };
    BOOST_CHECK(!tl::contains(multi, crossing));

    // Polígono completamente fuera
    tl::Polygon2d outside;
    outside.outer() = {
        tl::Point2d(40.0, 40.0),
        tl::Point2d(50.0, 40.0),
        tl::Point2d(50.0, 50.0),
        tl::Point2d(40.0, 50.0),
        tl::Point2d(40.0, 40.0)
    };
    BOOST_CHECK(!tl::contains(multi, outside));
}

// ============================================================================
// TEST CASES PARA GEOMETRÍAS VACÍAS Y CASOS ESPECIALES
// ============================================================================

BOOST_AUTO_TEST_CASE(empty_geometry_contains)
{
    tl::LineString2d empty_line;
    tl::Polygon2d empty_polygon;
    tl::MultiPoint2d empty_multipoint;

    tl::Point2d point(1.0, 1.0);
    tl::LineString2d line = {tl::Point2d(0.0, 0.0), tl::Point2d(2.0, 2.0)};

    // Geometría vacía no contiene nada
    BOOST_CHECK(!tl::contains(empty_line, point));
    BOOST_CHECK(!tl::contains(empty_polygon, point));
    //TODO: Falta por implementar
    //BOOST_CHECK(!tl::contains(empty_multipoint, point));

    // Nada contiene geometría vacía (debate filosófico, pero matemáticamente true)
    // Según OGC, contiene(geom, empty) generalmente es true
    //TODO: Falta por implementar
    //BOOST_CHECK(tl::contains(point, empty_line));  // Punto contiene línea vacía
    //BOOST_CHECK(tl::contains(line, empty_line));   // Línea contiene línea vacía

    // Geometría vacía contiene geometría vacía
    //BOOST_CHECK(tl::contains(empty_line, empty_line));
    BOOST_CHECK(tl::contains(empty_polygon, empty_polygon));
}

BOOST_AUTO_TEST_CASE(degenerate_polygon_contains)
{
    // Polígono degenerado (línea)
    tl::Polygon2d degenerate;
    degenerate.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(0.0, 0.0)  // Cerrado pero sin área
    };

    // Polígono degenerado no debería contener puntos (según OGC)
    // aunque estén "sobre" la línea
    BOOST_CHECK(!tl::contains(degenerate, tl::Point2d(5.0, 0.0)));
    BOOST_CHECK(!tl::contains(degenerate, tl::Point2d(5.0, 1.0)));

    // Polígono con área cero pero más puntos
    tl::Polygon2d zero_area;
    zero_area.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(5.0, 5.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(5.0, 5.0),
        tl::Point2d(0.0, 0.0)
    };
    BOOST_CHECK(!tl::contains(zero_area, tl::Point2d(5.0, 5.0)));
}

BOOST_AUTO_TEST_CASE(complex_polygon_contains)
{
    // Polígono en forma de estrella
    tl::Polygon2d star;
    star.outer() = {
        tl::Point2d(0.0, 3.0),
        tl::Point2d(2.0, 2.0),
        tl::Point2d(3.0, 0.0),
        tl::Point2d(4.0, 2.0),
        tl::Point2d(6.0, 3.0),
        tl::Point2d(4.0, 4.0),
        tl::Point2d(3.0, 6.0),
        tl::Point2d(2.0, 4.0),
        tl::Point2d(0.0, 3.0)
    };

    // Punto en el centro de la estrella (debería estar dentro)
    BOOST_CHECK(tl::contains(star, tl::Point2d(3.0, 3.0)));

    // Punto en una punta de la estrella (en borde)
    BOOST_CHECK(!tl::contains(star, tl::Point2d(3.0, 0.0)));

    // Punto fuera
    BOOST_CHECK(!tl::contains(star, tl::Point2d(10.0, 10.0)));

    // Punto en concavidad (pero dentro)
    BOOST_CHECK(tl::contains(star, tl::Point2d(2.5, 2.5)));
}

// ============================================================================
// TEST CASES DE SIMETRÍA Y PROPIEDADES
// ============================================================================

BOOST_AUTO_TEST_CASE(contains_symmetry_and_properties)
{
    // contains no es simétrico en general
    tl::Polygon2d big;
    big.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    tl::Polygon2d small;
    small.outer() = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(8.0, 2.0),
        tl::Point2d(8.0, 8.0),
        tl::Point2d(2.0, 8.0),
        tl::Point2d(2.0, 2.0)
    };

    // contains no es simétrico
    BOOST_CHECK(tl::contains(big, small));
    BOOST_CHECK(!tl::contains(small, big));

    BOOST_CHECK(!tl::contains(big, big));
    BOOST_CHECK(!tl::contains(small, small));

    // Transitividad (si A contiene B y B contiene C, entonces A contiene C)
    tl::Polygon2d tiny;
    tiny.outer() = {
        tl::Point2d(3.0, 3.0),
        tl::Point2d(7.0, 3.0),
        tl::Point2d(7.0, 7.0),
        tl::Point2d(3.0, 7.0),
        tl::Point2d(3.0, 3.0)
    };

    if (tl::contains(big, small) && tl::contains(small, tiny)) {
        BOOST_CHECK(tl::contains(big, tiny));
    }
}

BOOST_AUTO_TEST_CASE(contains_vs_intersects_relationship)
{
    // Si A contiene B, entonces A intersecta B
    tl::Polygon2d container;
    container.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    tl::Polygon2d contained;
    contained.outer() = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(8.0, 2.0),
        tl::Point2d(8.0, 8.0),
        tl::Point2d(2.0, 8.0),
        tl::Point2d(2.0, 2.0)
    };

    if (tl::contains(container, contained)) {
        BOOST_CHECK(tl::intersects(container, contained));
    }

    // Pero lo inverso no es cierto: si intersectan, no necesariamente uno contiene al otro
    tl::Polygon2d overlapping;
    overlapping.outer() = {
        tl::Point2d(5.0, 5.0),
        tl::Point2d(15.0, 5.0),
        tl::Point2d(15.0, 15.0),
        tl::Point2d(5.0, 15.0),
        tl::Point2d(5.0, 5.0)
    };

    BOOST_CHECK(tl::intersects(container, overlapping));
    BOOST_CHECK(!tl::contains(container, overlapping));
    BOOST_CHECK(!tl::contains(overlapping, container));
}

// ============================================================================
// TEST CASES DE RENDIMIENTO Y CASOS LÍMITE
// ============================================================================

BOOST_AUTO_TEST_CASE(contains_large_coordinates)
{
    // Coordenadas grandes
    tl::Polygon2d large_poly;
    large_poly.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(1000000.0, 0.0),
        tl::Point2d(1000000.0, 1000000.0),
        tl::Point2d(0.0, 1000000.0),
        tl::Point2d(0.0, 0.0)
    };

    // Puntos dentro
    BOOST_CHECK(tl::contains(large_poly, tl::Point2d(500000.0, 500000.0)));
    BOOST_CHECK(tl::contains(large_poly, tl::Point2d(1.0, 1.0)));
    BOOST_CHECK(tl::contains(large_poly, tl::Point2d(999999.0, 999999.0)));

    // Puntos fuera
    BOOST_CHECK(!tl::contains(large_poly, tl::Point2d(-1.0, 500000.0)));
    BOOST_CHECK(!tl::contains(large_poly, tl::Point2d(500000.0, -1.0)));
    BOOST_CHECK(!tl::contains(large_poly, tl::Point2d(1000001.0, 500000.0)));
    BOOST_CHECK(!tl::contains(large_poly, tl::Point2d(500000.0, 1000001.0)));
}

BOOST_AUTO_TEST_CASE(contains_precision_limits)
{
    // Tests de precisión de punto flotante
    tl::Polygon2d poly;
    poly.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(1.0, 0.0),
        tl::Point2d(1.0, 1.0),
        tl::Point2d(0.0, 1.0),
        tl::Point2d(0.0, 0.0)
    };

    // Puntos muy cerca del borde (interior)
    BOOST_CHECK(tl::contains(poly, tl::Point2d(0.0000001, 0.5)));
    BOOST_CHECK(tl::contains(poly, tl::Point2d(0.5, 0.0000001)));
    BOOST_CHECK(tl::contains(poly, tl::Point2d(0.9999999, 0.5)));
    BOOST_CHECK(tl::contains(poly, tl::Point2d(0.5, 0.9999999)));

    // Puntos muy cerca del borde (exterior)
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(-0.0000001, 0.5)));
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(0.5, -0.0000001)));
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(1.0000001, 0.5)));
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(0.5, 1.0000001)));

    // Puntos en el borde (con precisión de máquina)
    tl::Point2d on_border_right(1.0, 0.5);
    tl::Point2d on_border_top(0.5, 1.0);

    // contains debería devolver false para puntos en el borde
    BOOST_CHECK(!tl::contains(poly, on_border_right));
    BOOST_CHECK(!tl::contains(poly, on_border_top));
}

// ============================================================================
// TEST CASES DE REGRESIÓN PARA BUGS ESPECÍFICOS
// ============================================================================

BOOST_AUTO_TEST_CASE(regression_point_on_polygon_vertex)
{
    // Bug común: puntos en vértices de polígono
    tl::Polygon2d poly;
    poly.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    // Puntos en vértices (esquinas)
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(0.0, 0.0)));    // Esquina inferior izquierda
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(10.0, 0.0)));   // Esquina inferior derecha
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(10.0, 10.0)));  // Esquina superior derecha
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(0.0, 10.0)));   // Esquina superior izquierda
}

BOOST_AUTO_TEST_CASE(regression_polygon_with_collinear_vertices)
{
    // Polígono con vértices colineales
    tl::Polygon2d poly;
    poly.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(5.0, 0.0),  // Colineal
        tl::Point2d(10.0, 0.0),
        tl::Point2d(10.0, 10.0),
        tl::Point2d(0.0, 10.0),
        tl::Point2d(0.0, 0.0)
    };

    // Puntos dentro
    BOOST_CHECK(tl::contains(poly, tl::Point2d(5.0, 5.0)));
    BOOST_CHECK(tl::contains(poly, tl::Point2d(2.0, 2.0)));

    // Puntos en el segmento colineal
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(5.0, 0.0)));  // En vértice colineal
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(2.0, 0.0)));  // En borde colineal
}

BOOST_AUTO_TEST_CASE(regression_multiple_holes_in_polygon)
{
    // Polígono con múltiples huecos
    tl::Polygon2d poly;
    poly.outer() = {
        tl::Point2d(0.0, 0.0),
        tl::Point2d(20.0, 0.0),
        tl::Point2d(20.0, 20.0),
        tl::Point2d(0.0, 20.0),
        tl::Point2d(0.0, 0.0)
    };

    // Hueco 1
    tl::LinearRing2d hole1 = {
        tl::Point2d(2.0, 2.0),
        tl::Point2d(8.0, 2.0),
        tl::Point2d(8.0, 8.0),
        tl::Point2d(2.0, 8.0),
        tl::Point2d(2.0, 2.0)
    };

    // Hueco 2
    tl::LinearRing2d hole2 = {
        tl::Point2d(12.0, 12.0),
        tl::Point2d(18.0, 12.0),
        tl::Point2d(18.0, 18.0),
        tl::Point2d(12.0, 18.0),
        tl::Point2d(12.0, 12.0)
    };

    poly.addInner(hole1);
    poly.addInner(hole2);

    // Puntos fuera de los huecos
    BOOST_CHECK(tl::contains(poly, tl::Point2d(1.0, 1.0)));   // Fuera hole1
    BOOST_CHECK(tl::contains(poly, tl::Point2d(10.0, 10.0))); // Entre huecos
    BOOST_CHECK(tl::contains(poly, tl::Point2d(19.0, 1.0)));  // Derecha de hole1

    // Puntos dentro de los huecos
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(5.0, 5.0)));   // En hole1
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(15.0, 15.0))); // En hole2

    // Puntos en bordes de huecos
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(2.0, 5.0)));   // Borde hole1
    BOOST_CHECK(!tl::contains(poly, tl::Point2d(12.0, 15.0))); // Borde hole2
}

// ============================================================================
// TEST CASES DE 3D (si tu implementación soporta 3D)
// ============================================================================

BOOST_AUTO_TEST_CASE(contains_3d_basic)
{
    // BoundingBox 3D
    tl::BoundingBox3d bbox3d(tl::Point3d(0.0, 0.0, 0.0),
        tl::Point3d(10.0, 10.0, 10.0));

    // Puntos dentro del bounding box 3D
    BOOST_CHECK(tl::contains(bbox3d, tl::Point3d(5.0, 5.0, 5.0)));
    BOOST_CHECK(tl::contains(bbox3d, tl::Point3d(0.0, 5.0, 5.0)));  // En cara XY
    BOOST_CHECK(tl::contains(bbox3d, tl::Point3d(5.0, 0.0, 5.0)));  // En cara XZ
    BOOST_CHECK(tl::contains(bbox3d, tl::Point3d(5.0, 5.0, 0.0)));  // En cara YZ

    // Puntos fuera
    BOOST_CHECK(!tl::contains(bbox3d, tl::Point3d(11.0, 5.0, 5.0)));
    BOOST_CHECK(!tl::contains(bbox3d, tl::Point3d(5.0, 11.0, 5.0)));
    BOOST_CHECK(!tl::contains(bbox3d, tl::Point3d(5.0, 5.0, 11.0)));

    // Punto 3D en esquina
    BOOST_CHECK(tl::contains(bbox3d, tl::Point3d(0.0, 0.0, 0.0)));
    BOOST_CHECK(tl::contains(bbox3d, tl::Point3d(10.0, 10.0, 10.0)));
}

// ============================================================================
// TEST CASES ADICIONALES DE PERFORMANCE
// ============================================================================

BOOST_AUTO_TEST_CASE(contains_performance_large_polygon)
{
    // Polígono con muchos vértices (100 vértices)
    tl::Polygon2d large_poly;

    // Crear un círculo aproximado con muchos vértices
    const int num_vertices = 100;
    const double radius = 10.0;
    const double center_x = 0.0;
    const double center_y = 0.0;

    for (int i = 0; i <= num_vertices; ++i) {
        double angle = 2.0 * consts::pi<double> * i / num_vertices;
        double x = center_x + radius * cos(angle);
        double y = center_y + radius * sin(angle);
        large_poly.outer().push_back(tl::Point2d(x, y));
    }
    // Cerrar el polígono
    if (!large_poly.outer().empty()) {
        large_poly.outer().push_back(large_poly.outer()[0]);
    }

    // Puntos dentro del círculo
    BOOST_CHECK(tl::contains(large_poly, tl::Point2d(0.0, 0.0)));        // Centro
    BOOST_CHECK(tl::contains(large_poly, tl::Point2d(5.0, 0.0)));        // Cerca del centro
    BOOST_CHECK(tl::contains(large_poly, tl::Point2d(0.0, 5.0)));        // Cerca del centro

    // Puntos fuera del círculo
    BOOST_CHECK(!tl::contains(large_poly, tl::Point2d(15.0, 0.0)));      // Fuera en X
    BOOST_CHECK(!tl::contains(large_poly, tl::Point2d(0.0, 15.0)));      // Fuera en Y
    BOOST_CHECK(!tl::contains(large_poly, tl::Point2d(10.1, 10.1)));     // Fuera en diagonal
}

BOOST_AUTO_TEST_SUITE_END()