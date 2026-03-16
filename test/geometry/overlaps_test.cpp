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


BOOST_AUTO_TEST_SUITE_END()