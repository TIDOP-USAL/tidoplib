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
 
#define BOOST_TEST_MODULE Tidop algorithms disjoint test
#include <boost/test/unit_test.hpp>

#include "geometry_test_fixture.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/analysis/Disjoint.h"

using namespace tl;
using namespace test;

BOOST_AUTO_TEST_SUITE(DisjointAlgorithmTest)

// ============================================================================
// Point - Point
// ============================================================================

BOOST_FIXTURE_TEST_CASE(disjoint_point_point, GeometryTestFixture)
{
    BOOST_CHECK(disjoint(point2d1, point2d2));
    BOOST_CHECK(disjoint(point2d2, point2d1));  // Simetría
    BOOST_CHECK(!disjoint(point2d1, point2d1)); // Mismo punto no es disjunto

    BOOST_CHECK(disjoint(point2d4, point2d5));
    BOOST_CHECK(!disjoint(point2d4, point2d5, policy)); // Intersecta
}

BOOST_FIXTURE_TEST_CASE(disjoint_point_point_with_measure, GeometryTestFixture)
{
    BOOST_CHECK(!disjoint(point2d1, point2dm1));
    BOOST_CHECK(disjoint(point2dm1, point2dm2));
}

// ============================================================================
// Segment - Segment
// ============================================================================

BOOST_FIXTURE_TEST_CASE(disjoint_segment_segment, GeometryTestFixture)
{
    // ------------------------------------------------------------------------
    // Casos donde NO son disjuntos (tienen al menos un punto en común)
    // ------------------------------------------------------------------------

    // 1. Segmentos que se cruzan en un punto interior
    BOOST_CHECK(!disjoint(segment1, segment4));   // diagonal (0,0)-(10,10) y horizontal (0,5)-(10,5) se cruzan en (5,5)
    BOOST_CHECK(!disjoint(segment4, segment1));   // simetría

    // 2. Segmentos que se tocan en un extremo
    BOOST_CHECK(!disjoint(segment_colineal_1, segment_colineal_2)); // (0,0)-(5,0) y (5,0)-(10,0) tocan en (5,0)
    BOOST_CHECK(!disjoint(segment_colineal_2, segment_colineal_1));

    // 3. Segmentos iguales (mismos puntos)
    Segment2d seg_dup(point2d1, point2d3);
    BOOST_CHECK(!disjoint(segment1, seg_dup));
    BOOST_CHECK(!disjoint(seg_dup, segment1));

    // 4. Segmentos colineales con superposición parcial
    Segment2d seg_col1(point2d1, Point2d(10, 0));
    Segment2d seg_col2(Point2d(2, 0), Point2d(8, 0));
    BOOST_CHECK(!disjoint(seg_col1, seg_col2));
    BOOST_CHECK(!disjoint(seg_col2, seg_col1));

    // 5. Segmentos colineales donde uno contiene al otro
    Segment2d seg_col3(Point2d(0, 0), Point2d(10, 0));
    Segment2d seg_col4(Point2d(3, 0), Point2d(7, 0));
    BOOST_CHECK(!disjoint(seg_col3, seg_col4));
    BOOST_CHECK(!disjoint(seg_col4, seg_col3));

    // 6. Segmentos que comparten un punto interior de uno y extremo del otro
    Segment2d seg5(Point2d(0, 0), Point2d(5, 5));
    Segment2d seg6(Point2d(5, 5), Point2d(10, 10));
    BOOST_CHECK(!disjoint(seg5, seg6)); // punto (5,5) común (extremo de ambos)
    BOOST_CHECK(!disjoint(seg6, seg5));

    // ------------------------------------------------------------------------
    // Casos donde SÍ son disjuntos (ningún punto en común)
    // ------------------------------------------------------------------------

    // 7. Segmentos paralelos y separados
    Segment2d seg_horiz1(Point2d(0, 0), Point2d(5, 0));
    Segment2d seg_horiz2(Point2d(0, 1), Point2d(5, 1));
    BOOST_CHECK(disjoint(seg_horiz1, seg_horiz2));
    BOOST_CHECK(disjoint(seg_horiz2, seg_horiz1));

    // 8. Segmentos no paralelos que no se cruzan
    Segment2d seg7(Point2d(0, 0), Point2d(5, 0));
    Segment2d seg8(Point2d(10, 10), Point2d(15, 15));
    BOOST_CHECK(disjoint(seg7, seg8));
    BOOST_CHECK(disjoint(seg8, seg7));

    // 9. Segmentos colineales pero separados (con hueco)
    Segment2d seg9(Point2d(0, 0), Point2d(5, 0));
    Segment2d seg10(Point2d(7, 0), Point2d(10, 0));
    BOOST_CHECK(disjoint(seg9, seg10));
    BOOST_CHECK(disjoint(seg10, seg9));

    // 10. Segmento degenerado (punto) con otro segmento que no lo contiene
    Segment2d seg_degen(point2d1, point2d1); // (0,0)-(0,0)
    Segment2d seg11(Point2d(1, 1), Point2d(2, 2));
    BOOST_CHECK(disjoint(seg_degen, seg11));
    BOOST_CHECK(disjoint(seg11, seg_degen));

    // ------------------------------------------------------------------------
    // Pruebas con política de precisión (puntos casi coincidentes)
    // ------------------------------------------------------------------------

    // Segmento horizontal de (0,0) a (10,0)
    Segment2d seg_hor(Point2d(0, 0), Point2d(10, 0));

    // Segmento casi horizontal pero ligeramente desviado, que casi toca en un punto
    Segment2d seg_almost(Point2d(5, 1e-8), Point2d(5, 1e-8)); // degenerado, pero en realidad es un punto
    // Con política de resolución 0.001, 1e-8 < 0.001, por lo que se considera igual a 0, luego el punto (5,0) está sobre seg_hor?
    // El segmento degenerado se considera un punto. Como está sobre seg_hor (por la tolerancia), no son disjuntos.
    // Pero la política actual usa escala 1/0.001 = 1000, luego snap(1e-8) = 0, por lo que el punto se convierte a (5,0) y está sobre el segmento.
    BOOST_CHECK(!disjoint(seg_hor, seg_almost, policy));

    // Segmento casi paralelo pero ligeramente separado, con distancia < resolución
    Segment2d seg_almost_parallel(Point2d(0, 1e-8), Point2d(10, 1e-8));
    // Con la política, estos puntos se convierten a y=0, por lo que el segmento se superpone exactamente con seg_hor
    BOOST_CHECK(!disjoint(seg_hor, seg_almost_parallel, policy));

    // Segmento con separación mayor que la resolución
    Segment2d seg_far(Point2d(0, 0.01), Point2d(10, 0.01));
    BOOST_CHECK(disjoint(seg_hor, seg_far, policy));
}

BOOST_AUTO_TEST_SUITE_END()