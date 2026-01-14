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
 
#define BOOST_TEST_MODULE Tidop intersection test
#include <boost/test/unit_test.hpp>

#include "tidop/geometry/algorithms/spatial/Intersection.h"
#include "tidop/geometry/spatial/BoundingBox.h"

using namespace tl;

//BOOST_AUTO_TEST_CASE(segment_intersection_test)
//{
//    // Caso 1: Cruz perfecta en (5,5)
//    Segment2d s1(Point2d{0, 5}, Point2d{10, 5});
//    Segment2d s2(Point2d{5, 0}, Point2d{5, 10});
//    Point2d result;
//    BOOST_CHECK(intersect_segments(s1, s2, result));
//    BOOST_CHECK_CLOSE(result.x(), 5.0, 0.001);
//    BOOST_CHECK_CLOSE(result.y(), 5.0, 0.001);
//
//    // Caso 2: Segmentos que comparten un extremo (T-junction)
//    Segment2d s3(Point2d{0, 0}, Point2d{10, 0});
//    Segment2d s4(Point2d{10, 0}, Point2d{10, 10});
//    BOOST_CHECK(intersect_segments(s3, s4, result));
//    BOOST_CHECK_EQUAL(result.x(), 10.0);
//    BOOST_CHECK_EQUAL(result.y(), 0.0);
//
//    // Caso 3: Paralelos (No deben intersecar)
//    Segment2d s5(Point2d{0, 0}, Point2d{10, 0});
//    Segment2d s6(Point2d{0, 1}, Point2d{10, 1});
//    BOOST_CHECK(!intersect_segments(s5, s6, result));
//}

BOOST_AUTO_TEST_CASE(bbox_intersection_dispatch_test)
{
    BoundingBox2d a(Point2d(0, 0), Point2d(10, 10));
    BoundingBox2d b(Point2d(5, 5), Point2d(15, 15));

    // Llamada a través de la interfaz genérica
    auto res = intersection(a, b);

    BOOST_CHECK_EQUAL(res.pt1().x(), 5.0);
    BOOST_CHECK_EQUAL(res.pt1().y(), 5.0);
    BOOST_CHECK_EQUAL(res.pt2().x(), 10.0);
    BOOST_CHECK_EQUAL(res.pt2().y(), 10.0);
    BOOST_CHECK(!res.isEmpty());

    // Test de no intersección
    BoundingBox2d c(Point2d(20, 20), Point2d(30, 30));
    auto res_empty = tl::intersection(a, c);
    BOOST_CHECK(res_empty.isEmpty());
}