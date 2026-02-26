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
#include "tidop/geometry/algorithms/analysis/Equals.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include <variant>
#include <type_traits>

using namespace tl;


BOOST_AUTO_TEST_SUITE(IntersectionAlgorithmTest)

struct IntersectionTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(3.0, 4.0);

        point3d1 = Point3d(1.0, 2.0, 3.0);
        point3d2 = Point3d(4.0, 5.0, 6.0);

        point2dm1 = Point2dm(1.0, 2.0, 10.0);
        point2dm2 = Point2dm(1.0, 2.0, 20.0);

        point2i1 = Point2i(5, 10);
        point2i2 = Point2i(5, 11);
    }

    void teardown()
    {

    }

    Point2d point2d1;
    Point2d point2d2;
    Point3d point3d1;
    Point3d point3d2;
    Point2dm point2dm1;
    Point2dm point2dm2;
    Point2i point2i1;
    Point2i point2i2;
};

BOOST_FIXTURE_TEST_CASE(Intersection_Point2D_SamePoint, IntersectionTestFixture)
{
    auto result = intersection(point2d1, point2d1);

    // Debe contener un punto
    BOOST_REQUIRE(result.has_value());
    const auto &intersec_point = result.value();

    BOOST_CHECK(intersec_point.x() == point2d1.x());
    BOOST_CHECK(intersec_point.y() == point2d1.y());
}

BOOST_FIXTURE_TEST_CASE(Intersection_Point2D_DifferentPoints, IntersectionTestFixture)
{
    auto result = intersection(point2d1, point2d2);

    BOOST_REQUIRE(!result.has_value());
}

BOOST_FIXTURE_TEST_CASE(Intersection_Point2D_WithinTolerance, IntersectionTestFixture)
{
    static constexpr double EPSILON = 1e-5;
    Point2d p2{1.0 + EPSILON / 2, 2.0 + EPSILON / 2};

    auto result = intersection(point2d1, p2);
    bool check = result.has_value();
    BOOST_CHECK(result.has_value());
    const auto &intersec_point = result.value();

    BOOST_CHECK(equals(intersec_point, point2d1));
}

//BOOST_FIXTURE_TEST_CASE(Intersection_Point3D, IntersectionTestFixture)
//{
//    auto result1 = intersection(point3d1, point3d1);
//    auto result2 = intersection(point3d1, point3d2);
//
//    BOOST_REQUIRE(std::holds_alternative<Point3d>(result1));
//    BOOST_REQUIRE(std::holds_alternative<std::monostate>(result2));
//
//    const auto &intersec_point = std::get<Point3d>(result1);
//    BOOST_TEST(intersec_point.x() == point3d1.x());
//    BOOST_TEST(intersec_point.y() == point3d1.y());
//    BOOST_TEST(intersec_point.z() == point3d1.z());
//}
//
////BOOST_FIXTURE_TEST_CASE(Intersection_Point2D_Point3D, IntersectionTestFixture)
////{
////    Point2D p2d{1.0, 2.0};
////    Point3D p3d{1.0, 2.0, 0.0};
////
////    // Geometrías con dimensiones diferentes, no hay intersección
////    auto result = intersection(p2d, p3d);
////    BOOST_REQUIRE(std::holds_alternative<std::monostate>(result));
////}

BOOST_FIXTURE_TEST_CASE(Intersection_PointWithMeasure_SameMeasure, IntersectionTestFixture)
{
    auto result = intersection(point2dm1, point2dm1);

    BOOST_REQUIRE(result.has_value());
    const auto &intersec_point = result.value();

    BOOST_TEST(intersec_point.x() == point2dm1.x());
    BOOST_TEST(intersec_point.y() == point2dm1.y());
}

//BOOST_FIXTURE_TEST_CASE(Intersection_PointWithMeasure_DifferentMeasure, IntersectionTestFixture)
//{
//    // Aunque las coordenadas espaciales son iguales, las medidas son diferentes
//    // Dependiendo de tu implementación, podrías:
//    // 1. Considerarlos iguales (ignorando medida)
//    // 2. Considerarlos diferentes
//    // Vamos a probar ambas posibilidades
//
//    auto result = intersection(point2dm1, point2dm2);
//
//    // Si tu implementación ignora la medida para intersección:
//    // BOOST_REQUIRE(std::holds_alternative<Point2DM>(result));
//
//    // Si tu implementación considera la medida:
//    BOOST_REQUIRE(std::holds_alternative<std::monostate>(result));
//}
//
//BOOST_FIXTURE_TEST_CASE(Intersection_Point_Integer, IntersectionTestFixture)
//{
//    auto result1 = intersection(point2i1, point2i1);
//    auto result2 = intersection(point2i1, point2i2);
//
//    BOOST_REQUIRE(std::holds_alternative<Point2i>(result1));
//    BOOST_REQUIRE(std::holds_alternative<std::monostate>(result2));
//
//    const auto &intersec_point = std::get<Point2i>(result1);
//    BOOST_TEST(intersec_point.x() == 5);
//    BOOST_TEST(intersec_point.y() == 10);
//}
//
//BOOST_FIXTURE_TEST_CASE(Intersection_CommutativeProperty, IntersectionTestFixture)
//{
//    Point2d p1{2.0, 3.0};
//    Point2d p2{2.0, 3.0};
//    Point2d p3{4.0, 5.0};
//
//    // La intersección debe ser conmutativa
//    auto result1 = intersection(p1, p2);
//    auto result2 = intersection(p2, p1);
//
//    BOOST_REQUIRE(std::holds_alternative<Point2d>(result1));
//    BOOST_REQUIRE(std::holds_alternative<Point2d>(result2));
//
//    const auto &intersec1 = std::get<Point2d>(result1);
//    const auto &intersec2 = std::get<Point2d>(result2);
//
//    BOOST_TEST(equals(intersec1, intersec2));
//
//    // Para puntos diferentes también
//    auto result3 = intersection(p1, p3);
//    auto result4 = intersection(p3, p1);
//
//    BOOST_REQUIRE(std::holds_alternative<std::monostate>(result3));
//    BOOST_REQUIRE(std::holds_alternative<std::monostate>(result4));
//}
//
//BOOST_FIXTURE_TEST_CASE(Intersection_EmptyResultType, IntersectionTestFixture)
//{
//    auto result = intersection(point2d1, point2d2);
//
//    // Verificar que el tipo de retorno para vacío es std::monostate
//    static_assert(std::is_same_v<
//        decltype(result),
//        std::variant<std::monostate, Point2d>
//    >, "Return type should be variant<monostate, Point2D>");
//
//    BOOST_TEST(std::holds_alternative<std::monostate>(result));
//}
//
//BOOST_FIXTURE_TEST_CASE(Intersection_ZeroPoint, IntersectionTestFixture)
//{
//    Point2d p1{0.0, 0.0};
//    Point2d p2{0.0, 0.0};
//    Point2d p3{0.0, 0.000001}; // Fuera de tolerancia
//
//    auto result1 = intersection(p1, p2);
//    auto result2 = intersection(p1, p3);
//
//    BOOST_REQUIRE(std::holds_alternative<Point2d>(result1));
//
//    // Dependiendo de la tolerancia
//    if (equals(p1, p3)) {
//        BOOST_REQUIRE(std::holds_alternative<Point2d>(result2));
//    } else {
//        BOOST_REQUIRE(std::holds_alternative<std::monostate>(result2));
//    }
//}
//
//BOOST_FIXTURE_TEST_CASE(Intersection_FloatPrecision, IntersectionTestFixture)
//{
//    Point2f p1{1.0f, 2.0f};
//    Point2f p2{1.0f, 2.0f};
//    Point2f p3{1.00001f, 2.00001f}; // Dentro de tolerancia para float
//
//    auto result1 = intersection(p1, p2);
//    auto result2 = intersection(p1, p3);
//
//    BOOST_REQUIRE(std::holds_alternative<Point2f>(result1));
//
//    // Para float, la tolerancia es mayor
//    const auto &intersec_point = std::get<Point2f>(result1);
//    BOOST_TEST(intersec_point.x() == 1.0f);
//    BOOST_TEST(intersec_point.y() == 2.0f);
//}

BOOST_AUTO_TEST_SUITE_END()


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

    BOOST_CHECK_EQUAL(res.min().x(), 5.0);
    BOOST_CHECK_EQUAL(res.min().y(), 5.0);
    BOOST_CHECK_EQUAL(res.max().x(), 10.0);
    BOOST_CHECK_EQUAL(res.max().y(), 10.0);
    BOOST_CHECK(!res.isEmpty());

    // Test de no intersección
    BoundingBox2d c(Point2d(20, 20), Point2d(30, 30));
    auto res_empty = tl::intersection(a, c);
    BOOST_CHECK(res_empty.isEmpty());
}