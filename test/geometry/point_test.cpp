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
 
#define BOOST_TEST_MODULE Tidop Point test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/vector.h>
#include <tidop/math/algebra/matrix.h>
#include <tidop/geometry/base/Dimension.h>
#include <tidop/geometry/primitives/Point.h>
#include <tidop/geometry/primitives/MultiPoint.h>
#include <tidop/geometry/algorithms/spatial/Envelope.h>
#include <tidop/geometry/io/wkt/Proxy.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(PointTestSuite)

struct PointTest
{

    void setup()
    {
        point_xy_integer = Point2i(23, 67);
        point_xy_double = Point2d(253.56, 562.94);
        point_xy_float = Point2f(564.26f, 646.65f);

        point_xyz_integer = Point3i(23, 67, 54);
        point_xyz_double = Point3d(253.56, 562.94, 345.89);
        point_xyz_float = Point3f(564.26f, 646.65f, 45.89f);

        point_xym_double = Point2dm(253.56, 562.94, 255.);
        point_xym_float = Point2fm(564.26f, 646.65f, 10.f);
        point_xym_integer = Point2im(23, 67, 1);

        point_xyzm_double = Point3dm(253.56, 562.94, 345.89, 2563.2);
        point_xyzm_float = Point3fm(564.26f, 646.65f, 45.89f, 452.23f);
        point_xyzm_integer = Point3im(23, 67, 54, 10);

        point_xyzw_double = Point4d(253.56, 562.94, 345.89, 2563.2);
        point_xyzw_float = Point4f(564.26f, 646.65f, 45.89f, 452.23f);
        point_xyzw_integer = Point4i(23, 67, 54, 26);
    }

    void teardown()
    {

    }

    Point2d point_xy_double;
    Point2f point_xy_float;
    Point2i point_xy_integer;

    Point3d point_xyz_double;
    Point3f point_xyz_float;
    Point3i point_xyz_integer;

    Point2dm point_xym_double;
    Point2fm point_xym_float;
    Point2im point_xym_integer;

    Point3dm point_xyzm_double;
    Point3fm point_xyzm_float;
    Point3im point_xyzm_integer;

    Point4d point_xyzw_double;
    Point4f point_xyzw_float;
    Point4i point_xyzw_integer;

    std::array<int, 2> array_int{{ 23, 67 }};
};

BOOST_AUTO_TEST_CASE(default_constructors)
{
    Point2d point_xy;
    BOOST_CHECK_EQUAL(0., point_xy.x());
    BOOST_CHECK_EQUAL(0., point_xy.y());

    Point3d point_xyz;
    BOOST_CHECK_EQUAL(0., point_xyz.x());
    BOOST_CHECK_EQUAL(0., point_xyz.y());
    BOOST_CHECK_EQUAL(0., point_xyz.z());

    Point2dm point_xym;
    BOOST_CHECK_EQUAL(0., point_xym.x());
    BOOST_CHECK_EQUAL(0., point_xym.y());
    BOOST_CHECK_EQUAL(0., point_xym.m());

    Point3dm point_xyzm;
    BOOST_CHECK_EQUAL(0., point_xyzm.x());
    BOOST_CHECK_EQUAL(0., point_xyzm.y());
    BOOST_CHECK_EQUAL(0., point_xyzm.z());
    BOOST_CHECK_EQUAL(0., point_xyzm.m());

    Point4d point_xyzw;
    BOOST_CHECK_EQUAL(0., point_xyzw.x());
    BOOST_CHECK_EQUAL(0., point_xyzw.y());
    BOOST_CHECK_EQUAL(0., point_xyzw.z());
    BOOST_CHECK_EQUAL(0., point_xyzw.w());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, PointTest)
{
    Point2i point_xy_integer_copy(point_xy_integer);
    BOOST_CHECK_EQUAL(23, point_xy_integer_copy.x());
    BOOST_CHECK_EQUAL(67, point_xy_integer_copy.y());

    Point2d point_xy_double_copy(point_xy_double);
    BOOST_CHECK_CLOSE(253.56, point_xy_double_copy.x(), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xy_double_copy.y(), 0.01);

    Point2f point_xy_float_copy(point_xy_float);
    BOOST_CHECK_CLOSE(564.26f, point_xy_float_copy.x(), 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xy_float_copy.y(), 0.05);

    Point3d point_xyz_double_copy(point_xyz_double);
    BOOST_CHECK_CLOSE(253.56, point_xyz_double_copy.x(), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xyz_double_copy.y(), 0.01);
    BOOST_CHECK_CLOSE(345.89, point_xyz_double_copy.z(), 0.01);

    Point3f point_xyz_float_copy(point_xyz_float);
    BOOST_CHECK_CLOSE(564.26f, point_xyz_float_copy.x(), 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xyz_float_copy.y(), 0.05);
    BOOST_CHECK_CLOSE(45.89f, point_xyz_float_copy.z(), 0.05);

    Point3i point_xyz_integer_copy(point_xyz_integer);
    BOOST_CHECK_EQUAL(23, point_xyz_integer_copy.x());
    BOOST_CHECK_EQUAL(67, point_xyz_integer_copy.y());
    BOOST_CHECK_EQUAL(54, point_xyz_integer_copy.z());

    Point2dm point_xym_double_copy(point_xym_double);
    BOOST_CHECK_CLOSE(253.56, point_xym_double_copy.x(), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xym_double_copy.y(), 0.01);
    BOOST_CHECK_CLOSE(255., point_xym_double_copy.m(), 0.01);

    Point2fm point_xym_float_copy(point_xym_float);
    BOOST_CHECK_CLOSE(564.26f, point_xym_float_copy.x(), 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xym_float_copy.y(), 0.05);
    BOOST_CHECK_CLOSE(10.f, point_xym_float_copy.m(), 0.05);

    Point2im point_xym_integer_copy(point_xym_integer);
    BOOST_CHECK_EQUAL(23, point_xym_integer_copy.x());
    BOOST_CHECK_EQUAL(67, point_xym_integer_copy.y());
    BOOST_CHECK_EQUAL(1, point_xym_integer_copy.m());

    Point3dm point_xyzm_double_copy(point_xyzm_double);
    BOOST_CHECK_CLOSE(253.56, point_xyzm_double_copy.x(), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xyzm_double_copy.y(), 0.01);
    BOOST_CHECK_CLOSE(345.89, point_xyzm_double_copy.z(), 0.01);
    BOOST_CHECK_CLOSE(2563.2, point_xyzm_double_copy.m(), 0.01);

    Point3fm point_xyzm_float_copy(point_xyzm_float);
    BOOST_CHECK_CLOSE(564.26f, point_xyzm_float_copy.x(), 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xyzm_float_copy.y(), 0.05);
    BOOST_CHECK_CLOSE(45.89f, point_xyzm_float_copy.z(), 0.05);
    BOOST_CHECK_CLOSE(452.23f, point_xyzm_float_copy.m(), 0.05);

    Point3im point_xyzm_integer_copy(point_xyzm_integer);
    BOOST_CHECK_EQUAL(23, point_xyzm_integer_copy.x());
    BOOST_CHECK_EQUAL(67, point_xyzm_integer_copy.y());
    BOOST_CHECK_EQUAL(54, point_xyzm_integer_copy.z());
    BOOST_CHECK_EQUAL(10, point_xyzm_integer_copy.m());

    Point4d point_xyzw_double_copy(point_xyzw_double);
    BOOST_CHECK_CLOSE(253.56, point_xyzw_double_copy.x(), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xyzw_double_copy.y(), 0.01);
    BOOST_CHECK_CLOSE(345.89, point_xyzw_double_copy.z(), 0.01);
    BOOST_CHECK_CLOSE(2563.2, point_xyzw_double_copy.w(), 0.01);

    Point4f point_xyzw_float_copy(point_xyzw_float);
    BOOST_CHECK_CLOSE(564.26f, point_xyzw_float_copy.x(), 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xyzw_float_copy.y(), 0.05);
    BOOST_CHECK_CLOSE(45.89f, point_xyzw_float_copy.z(), 0.05);
    BOOST_CHECK_CLOSE(452.23f, point_xyzw_float_copy.w(), 0.05);

    Point4i point_xyzw_integer_copy(point_xyzw_integer);
    BOOST_CHECK_EQUAL(23, point_xyzw_integer_copy.x());
    BOOST_CHECK_EQUAL(67, point_xyzw_integer_copy.y());
    BOOST_CHECK_EQUAL(54, point_xyzw_integer_copy.z());
    BOOST_CHECK_EQUAL(26, point_xyzw_integer_copy.w());
}

BOOST_FIXTURE_TEST_CASE(at_position, PointTest)
{
    BOOST_CHECK_EQUAL(23, point_xy_integer.at(0));
    BOOST_CHECK_EQUAL(67, point_xy_integer.at(1));

    BOOST_CHECK_CLOSE(253.56, point_xy_double[0], 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xy_double[1], 0.01);

    BOOST_CHECK_CLOSE(253.56, point_xyz_double.at(0), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xyz_double.at(1), 0.01);
    BOOST_CHECK_CLOSE(345.89, point_xyz_double.at(2), 0.01);

    BOOST_CHECK_CLOSE(564.26f, point_xyz_float[0], 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xyz_float[1], 0.05);
    BOOST_CHECK_CLOSE(45.89f, point_xyz_float[2], 0.05);

    BOOST_CHECK_CLOSE(253.56, point_xym_double.at(0), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xym_double.at(1), 0.01);

    BOOST_CHECK_CLOSE(564.26f, point_xym_float[0], 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xym_float[1], 0.05);
    // No esta pensado para acceder a m (y no se devería) pero al estar contiguo en memoria funciona.
    BOOST_CHECK_CLOSE(10.f, point_xym_float[2], 0.05);

    BOOST_CHECK_CLOSE(253.56, point_xyzm_double.at(0), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xyzm_double.at(1), 0.01);
    BOOST_CHECK_CLOSE(345.89, point_xyzm_double.at(2), 0.01);

    BOOST_CHECK_CLOSE(564.26f, point_xyzm_float[0], 0.05);
    BOOST_CHECK_CLOSE(646.65f, point_xyzm_float[1], 0.05);
    BOOST_CHECK_CLOSE(45.89f, point_xyzm_float[2], 0.05);
    // No esta pensado para acceder a m (y no se devería) pero al estar contiguo en memoria funciona.
    BOOST_CHECK_CLOSE(452.23f, point_xyzm_float[3], 0.05);

    BOOST_CHECK_CLOSE(253.56, point_xyzw_double.at(0), 0.01);
    BOOST_CHECK_CLOSE(562.94, point_xyzw_double.at(1), 0.01);
    BOOST_CHECK_CLOSE(345.89, point_xyzw_double.at(2), 0.01);
    BOOST_CHECK_CLOSE(2563.2, point_xyzw_double.at(3), 0.01);

    BOOST_CHECK_EQUAL(23, point_xyzw_integer[0]);
    BOOST_CHECK_EQUAL(67, point_xyzw_integer[1]);
    BOOST_CHECK_EQUAL(54, point_xyzw_integer[2]);
    BOOST_CHECK_EQUAL(26, point_xyzw_integer[3]);
}

BOOST_FIXTURE_TEST_CASE(array_constructor, PointTest)
{
    Point2i pt_int(array_int);
    BOOST_CHECK_EQUAL(23, pt_int.x());
    BOOST_CHECK_EQUAL(67, pt_int.y());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, PointTest)
{
    Point2i pt(23, 67);
    Point2i pt2(std::move(pt));
    BOOST_CHECK_EQUAL(23, pt2.x());
    BOOST_CHECK_EQUAL(67, pt2.y());
}

BOOST_FIXTURE_TEST_CASE(type, PointTest)
{
    BOOST_CHECK(GeometryType::point == point_xy_integer.type());
    BOOST_CHECK(GeometryType::point == point_xy_double.type());
    BOOST_CHECK(GeometryType::point == point_xy_float.type());
}

BOOST_FIXTURE_TEST_CASE(dimension, PointTest)
{
    BOOST_CHECK(Dimension::dim2 == point_xy_integer.dimension());
    BOOST_CHECK(Dimension::dim2 == point_xy_double.dimension());
    BOOST_CHECK(Dimension::dim2 == point_xy_float.dimension());

    BOOST_CHECK(Dimension::dim3 == point_xyz_integer.dimension());
    BOOST_CHECK(Dimension::dim3 == point_xyz_double.dimension());
    BOOST_CHECK(Dimension::dim3 == point_xyz_float.dimension());

    BOOST_CHECK(Dimension::dim2 == point_xym_integer.dimension());
    BOOST_CHECK(Dimension::dim2 == point_xym_double.dimension());
    BOOST_CHECK(Dimension::dim2 == point_xym_float.dimension());

    BOOST_CHECK(Dimension::dim3 == point_xyzm_integer.dimension());
    BOOST_CHECK(Dimension::dim3 == point_xyzm_double.dimension());
    BOOST_CHECK(Dimension::dim3 == point_xyzm_float.dimension());

    BOOST_CHECK(Dimension::dim4 == point_xyzw_integer.dimension());
    BOOST_CHECK(Dimension::dim4 == point_xyzw_double.dimension());
    BOOST_CHECK(Dimension::dim4 == point_xyzw_float.dimension());
}

BOOST_FIXTURE_TEST_CASE(assing_operator, PointTest)
{
    Point2i pt_int_c = point_xy_integer;
    BOOST_CHECK_EQUAL(23, pt_int_c.x());
    BOOST_CHECK_EQUAL(67, pt_int_c.y());

    Point2d pt_double_c = point_xy_double;
    BOOST_CHECK_CLOSE(253.56, pt_double_c.x(), 0.01);
    BOOST_CHECK_CLOSE(562.94, pt_double_c.y(), 0.01);

    Point2f pt_float_c = point_xy_float;
    BOOST_CHECK_CLOSE(564.26f, pt_float_c.x(), 0.05);
    BOOST_CHECK_CLOSE(646.65f, pt_float_c.y(), 0.05);
}

BOOST_FIXTURE_TEST_CASE(cast, PointTest)
{
    Point2d pt_double = static_cast<Point2d>(point_xy_integer);

    BOOST_CHECK_EQUAL(23.0, pt_double.x());
    BOOST_CHECK_EQUAL(67.0, pt_double.y());

    Point2i pt_int = static_cast<Point2i>(point_xy_double);

    BOOST_CHECK_EQUAL(254, pt_int.x());
    BOOST_CHECK_EQUAL(563, pt_int.y());

    Point2f pt_float = static_cast<Point2f>(point_xyz_float);
    BOOST_CHECK_CLOSE(564.26f, pt_float.x(), 0.05);
    BOOST_CHECK_CLOSE(646.65f, pt_float.y(), 0.05);

    Point3d pt_double_3d = static_cast<Point3d>(point_xyz_integer);

    BOOST_CHECK_EQUAL(23.0, pt_double_3d.x());
    BOOST_CHECK_EQUAL(67.0, pt_double_3d.y());
    BOOST_CHECK_EQUAL(54.0, pt_double_3d.z());

    Point3f pt_float_3d = static_cast<Point3f>(point_xy_integer);

    BOOST_CHECK_EQUAL(23.0, pt_float_3d.x());
    BOOST_CHECK_EQUAL(67.0, pt_float_3d.y());
    BOOST_CHECK_EQUAL(0.0, pt_float_3d.z());

    Point4f pt_xyzw = static_cast<Point4f>(point_xy_integer);

    BOOST_CHECK_EQUAL(23.0, pt_xyzw.x());
    BOOST_CHECK_EQUAL(67.0, pt_xyzw.y());
    BOOST_CHECK_EQUAL(0.0, pt_xyzw.z());
    BOOST_CHECK_EQUAL(0.0, pt_xyzw.w());

    pt_xyzw = static_cast<Point4f>(point_xym_integer);

    BOOST_CHECK_EQUAL(23.0, pt_xyzw.x());
    BOOST_CHECK_EQUAL(67.0, pt_xyzw.y());
    BOOST_CHECK_EQUAL(0.0, pt_xyzw.z());
    BOOST_CHECK_EQUAL(0.0, pt_xyzw.w());

    pt_xyzw = static_cast<Point4f>(point_xyzm_integer);

    BOOST_CHECK_EQUAL(23.0, pt_xyzw.x());
    BOOST_CHECK_EQUAL(67.0, pt_xyzw.y());
    BOOST_CHECK_EQUAL(54.0, pt_xyzw.z());
    BOOST_CHECK_EQUAL(0.0, pt_xyzw.w());

    Point3fm pt_xyzm = static_cast<Point3fm>(point_xym_integer);

    BOOST_CHECK_EQUAL(23.0, pt_xyzm.x());
    BOOST_CHECK_EQUAL(67.0, pt_xyzm.y());
    BOOST_CHECK_EQUAL(0.0, pt_xyzm.z());
    BOOST_CHECK_EQUAL(1.0, pt_xyzm.m());

    Point2fm pt_xym = static_cast<Point2fm>(point_xyzm_integer);

    BOOST_CHECK_EQUAL(23.0, pt_xym.x());
    BOOST_CHECK_EQUAL(67.0, pt_xym.y());
    BOOST_CHECK_EQUAL(10.0, pt_xym.m());

    pt_xym = static_cast<Point2fm>(point_xyzw_integer);

    BOOST_CHECK_EQUAL(23.0, pt_xym.x());
    BOOST_CHECK_EQUAL(67.0, pt_xym.y());
    BOOST_CHECK_EQUAL(0.0, pt_xym.m());
}

BOOST_AUTO_TEST_SUITE_END()


/* Operaciones entre puntos */

BOOST_AUTO_TEST_CASE(point_substraction)
{
    Point2i pt_1(23, 67);
    Point2i pt_2(67, 56);

    auto pt_3 = pt_1 - pt_2;

    BOOST_CHECK_EQUAL(-44, pt_3.x());
    BOOST_CHECK_EQUAL(11, pt_3.y());

    Point2d pt_1d(256.26, 2526.36);
    Point2d pt_2d(894.66, 651.56);

    auto pt_3d = pt_1d - pt_2d;

    BOOST_CHECK_CLOSE(-638.4, pt_3d.x(), 0.01);
    BOOST_CHECK_CLOSE(1874.8, pt_3d.y(), 0.01);

    /// 3D

    Point3i pt3_1(23, 67, 10);
    Point3i pt3_2(67, 56, 25);

    auto pt3_3 = pt3_1 - pt3_2;

    BOOST_CHECK_EQUAL(-44, pt3_3.x());
    BOOST_CHECK_EQUAL(11, pt3_3.y());
    BOOST_CHECK_EQUAL(-15, pt3_3.z());

    Point3d pt3_1d(256.26, 2526.36, 12.36);
    Point3d pt3_2d(894.66, 651.56, 265.3);

    auto pt3_3d = pt3_1d - pt3_2d;

    BOOST_CHECK_CLOSE(-638.4, pt3_3d.x(), 0.01);
    BOOST_CHECK_CLOSE(1874.8, pt3_3d.y(), 0.01);
    BOOST_CHECK_CLOSE(-252.94, pt3_3d.z(), 0.01);

}

/// Multiplicación de un punto por una matriz

BOOST_AUTO_TEST_CASE(matrix_by_point)
{
    Matrix<int, 2, 3> _mat_2x3_i
    {
        6,8,6,
        9,6,2
    };
    Point3i point(1, 2, 3);
    Point2i point2 = _mat_2x3_i * point;

    BOOST_CHECK_EQUAL(40, point2[0]);
    BOOST_CHECK_EQUAL(27, point2[1]);

    Matrix<int> mat_dyn_2x3_i(2, 3);
    mat_dyn_2x3_i(0, 0) = 6;
    mat_dyn_2x3_i(0, 1) = 8;
    mat_dyn_2x3_i(0, 2) = 6;
    mat_dyn_2x3_i(1, 0) = 9;
    mat_dyn_2x3_i(1, 1) = 6;
    mat_dyn_2x3_i(1, 2) = 2;

    Point3i point3(1, 2, 3);
    auto vect4 = mat_dyn_2x3_i * point3;

    BOOST_CHECK_EQUAL(40, vect4[0]);
    BOOST_CHECK_EQUAL(27, vect4[1]);

    // Ver el resto de casos...
}

/* MultiPoint */

BOOST_AUTO_TEST_SUITE(MultiPointTestSuite)

struct MultiPointTest
{

    void setup()
    {
        multiPoint2d.emplace_back(23.6, 94.4);
        multiPoint2d.emplace_back(75.36, 246.33);
        multiPoint2d.emplace_back(256.6, 619.3);
        multiPoint2d.emplace_back(62.36, 6.60);

        multiPoint3d.emplace_back(23.6, 94.4, 2.);
        multiPoint3d.emplace_back(75.36, 246.33, 23.2);
        multiPoint3d.emplace_back(256.6, 619.3, 42.3);
        multiPoint3d.emplace_back(62.36, 6.60, 12.6);

        //std::vector<Point<double>> vector{
        //  Point<double>(23.6, 94.4),
        //  Point<double>(75.36, 246.33),
        //  Point<double>(256.6, 619.3),
        //  Point<double>(62.36, 6.60)};
    }

    void teardown()
    {

    }


    MultiPoint2d multiPoint2d;
    MultiPoint3d multiPoint3d;

};

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
    MultiPoint2d mp = {Point2d(0,0), Point2d(10,10), Point2d(20,20)};
    BOOST_CHECK_EQUAL(mp.size(), 3);
    BOOST_CHECK_EQUAL(mp[1].x(), 10);
}

BOOST_AUTO_TEST_CASE(multi_point_default_constructor)
{
    MultiPoint2i multiPoint;

    BOOST_CHECK_EQUAL(true, multiPoint.empty());
    BOOST_CHECK_EQUAL(0, multiPoint.size());
    BOOST_CHECK(GeometryType::multipoint == multiPoint.type());
    auto w = envelope(multiPoint);
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), w.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), w.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), w.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), w.pt2().y());

    MultiPoint3i multiPoint3;

    BOOST_CHECK_EQUAL(true, multiPoint3.empty());
    BOOST_CHECK_EQUAL(0, multiPoint3.size());
    BOOST_CHECK(GeometryType::multipoint == multiPoint3.type());
    auto box = envelope(multiPoint3);
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), box.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), box.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), box.pt2().z());
}

BOOST_AUTO_TEST_CASE(multi_point_reserve_constructor)
{
    MultiPoint2d multiPoint(10);
    auto w = envelope(multiPoint);

    BOOST_CHECK_EQUAL(10, multiPoint.size());
    BOOST_CHECK(multiPoint.type() == GeometryType::multipoint);
    BOOST_CHECK_EQUAL(0., w.pt1().x());
    BOOST_CHECK_EQUAL(0., w.pt1().y());
    BOOST_CHECK_EQUAL(0., w.pt2().x());
    BOOST_CHECK_EQUAL(0., w.pt2().y());
}

//BOOST_FIXTURE_TEST_CASE(vector_constructor, MultiPointTest)
//{
//    BOOST_CHECK(multiPointVector->type() == Entity::Type::multipoint2d);
//    BOOST_CHECK_EQUAL(4, multiPointVector->size());
//    BOOST_CHECK(multiPointVector->at(0) == Point<double>(23.6, 94.4));
//    BOOST_CHECK(multiPointVector->at(1) == Point<double>(75.36, 246.33));
//    BOOST_CHECK(multiPointVector->at(2) == Point<double>(256.6, 619.3));
//    BOOST_CHECK(multiPointVector->at(3) == Point<double>(62.36, 6.60));
//}
//
//BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiPointTest)
//{
//    MultiPointD multiPoint(*multiPointD);
//
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint2d);
//    WindowD w = multiPoint.window();
//    BOOST_CHECK_EQUAL(23.6, w.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, w.pt1.y);
//    BOOST_CHECK_EQUAL(256.6, w.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, w.pt2.y);
//    BOOST_CHECK_EQUAL(23.6, multiPoint.at(0).x);
//    BOOST_CHECK_EQUAL(94.4, multiPoint.at(0).y);
//    BOOST_CHECK_EQUAL(75.36, multiPoint.at(1).x);
//    BOOST_CHECK_EQUAL(246.33, multiPoint.at(1).y);
//    BOOST_CHECK_EQUAL(256.6, multiPoint.at(2).x);
//    BOOST_CHECK_EQUAL(619.3, multiPoint.at(2).y);
//    BOOST_CHECK_EQUAL(62.36, multiPoint.at(3).x);
//    BOOST_CHECK_EQUAL(6.60, multiPoint.at(3).y);
//}
//
//BOOST_FIXTURE_TEST_CASE(move_constructor, MultiPointTest)
//{
//    MultiPointD multiPoint1(*multiPointD);
//    MultiPointD multiPoint(std::move(multiPoint1));
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint2d);
//    WindowD w = multiPoint.window();
//    BOOST_CHECK_EQUAL(23.6, w.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, w.pt1.y);
//    BOOST_CHECK_EQUAL(256.6, w.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, w.pt2.y);
//    BOOST_CHECK_EQUAL(23.6, multiPoint.at(0).x);
//    BOOST_CHECK_EQUAL(94.4, multiPoint.at(0).y);
//    BOOST_CHECK_EQUAL(75.36, multiPoint.at(1).x);
//    BOOST_CHECK_EQUAL(246.33, multiPoint.at(1).y);
//    BOOST_CHECK_EQUAL(256.6, multiPoint.at(2).x);
//    BOOST_CHECK_EQUAL(619.3, multiPoint.at(2).y);
//    BOOST_CHECK_EQUAL(62.36, multiPoint.at(3).x);
//    BOOST_CHECK_EQUAL(6.60, multiPoint.at(3).y);
//}
//
//BOOST_FIXTURE_TEST_CASE(copy_assing_operator, MultiPointTest)
//{
//    MultiPointD multiPoint;
//    multiPoint = *multiPointD;
//
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint2d);
//    WindowD w = multiPoint.window();
//    BOOST_CHECK_EQUAL(23.6, w.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, w.pt1.y);
//    BOOST_CHECK_EQUAL(256.6, w.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, w.pt2.y);
//    BOOST_CHECK_EQUAL(23.6, multiPoint.at(0).x);
//    BOOST_CHECK_EQUAL(94.4, multiPoint.at(0).y);
//    BOOST_CHECK_EQUAL(75.36, multiPoint.at(1).x);
//    BOOST_CHECK_EQUAL(246.33, multiPoint.at(1).y);
//    BOOST_CHECK_EQUAL(256.6, multiPoint.at(2).x);
//    BOOST_CHECK_EQUAL(619.3, multiPoint.at(2).y);
//    BOOST_CHECK_EQUAL(62.36, multiPoint.at(3).x);
//    BOOST_CHECK_EQUAL(6.60, multiPoint.at(3).y);
//}
//
//BOOST_FIXTURE_TEST_CASE(move_assing_operator, MultiPointTest)
//{
//    MultiPointD multiPoint1(*multiPointD);
//    MultiPointD multiPoint;
//    multiPoint = std::move(multiPoint1);
//
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint2d);
//    WindowD w = multiPoint.window();
//    BOOST_CHECK_EQUAL(23.6, w.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, w.pt1.y);
//    BOOST_CHECK_EQUAL(256.6, w.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, w.pt2.y);
//    BOOST_CHECK_EQUAL(23.6, multiPoint.at(0).x);
//    BOOST_CHECK_EQUAL(94.4, multiPoint.at(0).y);
//    BOOST_CHECK_EQUAL(75.36, multiPoint.at(1).x);
//    BOOST_CHECK_EQUAL(246.33, multiPoint.at(1).y);
//    BOOST_CHECK_EQUAL(256.6, multiPoint.at(2).x);
//    BOOST_CHECK_EQUAL(619.3, multiPoint.at(2).y);
//    BOOST_CHECK_EQUAL(62.36, multiPoint.at(3).x);
//    BOOST_CHECK_EQUAL(6.60, multiPoint.at(3).y);
//}
//
//BOOST_AUTO_TEST_SUITE_END()
//
//
///* MultiPoint3D */
//
//BOOST_AUTO_TEST_SUITE(MultiPoint3DTestSuite)
//
//struct MultiPoint3DTest
//{
//
//    MultiPoint3DTest()
//        : multiPointD(nullptr),
//        multiPointVector(nullptr)
//    {
//
//    }
//
//    ~MultiPoint3DTest()
//    {
//        delete multiPointD;
//        delete multiPointVector;
//
//    }
//
//    void setup()
//    {
//        multiPointD = new MultiPoint3D<Point3<double>>();
//        multiPointD->push_back(Point3<double>(23.6, 94.4, 2.));
//        multiPointD->push_back(Point3<double>(75.36, 246.33, 23.2));
//        multiPointD->push_back(Point3<double>(256.6, 619.3, 42.3));
//        multiPointD->push_back(Point3<double>(62.36, 6.60, 12.6));
//
//        std::vector<Point3<double>> vector{
//          Point3<double>(23.6, 94.4, 2.),
//          Point3<double>(75.36, 246.33, 23.2),
//          Point3<double>(256.6, 619.3, 42.3),
//          Point3<double>(62.36, 6.60, 12.6)};
//
//        multiPointVector = new MultiPoint3D<Point3<double>>(vector);
//    }
//
//    void teardown()
//    {
//
//    }
//
//
//    MultiPoint3D<Point3<double>> *multiPointD;
//    MultiPoint3D<Point3<double>> *multiPointVector;
//};
//
//
//BOOST_FIXTURE_TEST_CASE(default_constructor, MultiPoint3DTest)
//{
//    MultiPoint3D<Point3<int>> multiPoint;
//
//    BOOST_CHECK_EQUAL(true, multiPoint.is3D());
//    BOOST_CHECK_EQUAL(true, multiPoint.empty());
//    BOOST_CHECK_EQUAL(0, multiPoint.size());
//    BOOST_CHECK(Entity::Type::multipoint3d == multiPoint.type());
//    BoundingBox<Point3<int>> box = multiPoint.boundingBox();
//    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1.x);
//    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1.y);
//    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1.z);
//    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), box.pt2.x);
//    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), box.pt2.y);
//    BOOST_CHECK_EQUAL(std::numeric_limits<int>::min(), box.pt2.z);
//}
//
//BOOST_FIXTURE_TEST_CASE(reserve_constructor, MultiPoint3DTest)
//{
//    MultiPoint3D<Point3<int>> multiPoint(10);
//
//    BOOST_CHECK_EQUAL(10, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint3d);
//    BoundingBox<Point3<int>> box = multiPoint.boundingBox();
//    BOOST_CHECK_EQUAL(0, box.pt1.x);
//    BOOST_CHECK_EQUAL(0, box.pt1.y);
//    BOOST_CHECK_EQUAL(0, box.pt1.z);
//    BOOST_CHECK_EQUAL(0, box.pt2.x);
//    BOOST_CHECK_EQUAL(0, box.pt2.y);
//    BOOST_CHECK_EQUAL(0, box.pt2.z);
//}
//
//BOOST_FIXTURE_TEST_CASE(vector_constructor, MultiPoint3DTest)
//{
//    BOOST_CHECK(multiPointVector->type() == Entity::Type::multipoint3d);
//    BOOST_CHECK_EQUAL(4, multiPointVector->size());
//    BOOST_CHECK(multiPointVector->at(0) == Point3<double>(23.6, 94.4, 2.));
//    BOOST_CHECK(multiPointVector->at(1) == Point3<double>(75.36, 246.33, 23.2));
//    BOOST_CHECK(multiPointVector->at(2) == Point3<double>(256.6, 619.3, 42.3));
//    BOOST_CHECK(multiPointVector->at(3) == Point3<double>(62.36, 6.60, 12.6));
//}
//
//BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiPoint3DTest)
//{
//    MultiPoint3D<Point3<double>> multiPoint(*multiPointD);
//
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint3d);
//    BoundingBox<Point3<double>> box = multiPoint.boundingBox();
//    BOOST_CHECK_EQUAL(23.6, box.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, box.pt1.y);
//    BOOST_CHECK_EQUAL(2., box.pt1.z);
//    BOOST_CHECK_EQUAL(256.6, box.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, box.pt2.y);
//    BOOST_CHECK_EQUAL(42.3, box.pt2.z);
//    BOOST_CHECK(multiPoint.at(0) == Point3<double>(23.6, 94.4, 2.));
//    BOOST_CHECK(multiPoint.at(1) == Point3<double>(75.36, 246.33, 23.2));
//    BOOST_CHECK(multiPoint.at(2) == Point3<double>(256.6, 619.3, 42.3));
//    BOOST_CHECK(multiPoint.at(3) == Point3<double>(62.36, 6.60, 12.6));
//}
//
//BOOST_FIXTURE_TEST_CASE(move_constructor, MultiPoint3DTest)
//{
//    MultiPoint3D<Point3<double>> multiPoint1(*multiPointD);
//    MultiPoint3D<Point3<double>> multiPoint(std::move(multiPoint1));
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint3d);
//    BoundingBox<Point3<double>> box = multiPoint.boundingBox();
//    BOOST_CHECK_EQUAL(23.6, box.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, box.pt1.y);
//    BOOST_CHECK_EQUAL(2., box.pt1.z);
//    BOOST_CHECK_EQUAL(256.6, box.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, box.pt2.y);
//    BOOST_CHECK_EQUAL(42.3, box.pt2.z);
//    BOOST_CHECK(multiPoint.at(0) == Point3<double>(23.6, 94.4, 2.));
//    BOOST_CHECK(multiPoint.at(1) == Point3<double>(75.36, 246.33, 23.2));
//    BOOST_CHECK(multiPoint.at(2) == Point3<double>(256.6, 619.3, 42.3));
//    BOOST_CHECK(multiPoint.at(3) == Point3<double>(62.36, 6.60, 12.6));
//}
//
//BOOST_FIXTURE_TEST_CASE(copy_assing_operator, MultiPoint3DTest)
//{
//    MultiPoint3D<Point3<double>> multiPoint;
//    multiPoint = *multiPointD;
//
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint3d);
//    BoundingBox<Point3<double>> box = multiPoint.boundingBox();
//    BOOST_CHECK_EQUAL(23.6, box.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, box.pt1.y);
//    BOOST_CHECK_EQUAL(2., box.pt1.z);
//    BOOST_CHECK_EQUAL(256.6, box.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, box.pt2.y);
//    BOOST_CHECK_EQUAL(42.3, box.pt2.z);
//    BOOST_CHECK(multiPoint.at(0) == Point3<double>(23.6, 94.4, 2.));
//    BOOST_CHECK(multiPoint.at(1) == Point3<double>(75.36, 246.33, 23.2));
//    BOOST_CHECK(multiPoint.at(2) == Point3<double>(256.6, 619.3, 42.3));
//    BOOST_CHECK(multiPoint.at(3) == Point3<double>(62.36, 6.60, 12.6));
//}
//
//BOOST_FIXTURE_TEST_CASE(move_assing_operator, MultiPoint3DTest)
//{
//    MultiPoint3D<Point3<double>> multiPoint1(*multiPointD);
//    MultiPoint3D<Point3<double>> multiPoint;
//    multiPoint = std::move(multiPoint1);
//
//    BOOST_CHECK_EQUAL(4, multiPoint.size());
//    BOOST_CHECK(multiPoint.type() == Entity::Type::multipoint3d);
//    BoundingBox<Point3<double>> box = multiPoint.boundingBox();
//    BOOST_CHECK_EQUAL(23.6, box.pt1.x);
//    BOOST_CHECK_EQUAL(6.60, box.pt1.y);
//    BOOST_CHECK_EQUAL(2., box.pt1.z);
//    BOOST_CHECK_EQUAL(256.6, box.pt2.x);
//    BOOST_CHECK_EQUAL(619.3, box.pt2.y);
//    BOOST_CHECK_EQUAL(42.3, box.pt2.z);
//    BOOST_CHECK(multiPoint.at(0) == Point3<double>(23.6, 94.4, 2.));
//    BOOST_CHECK(multiPoint.at(1) == Point3<double>(75.36, 246.33, 23.2));
//    BOOST_CHECK(multiPoint.at(2) == Point3<double>(256.6, 619.3, 42.3));
//    BOOST_CHECK(multiPoint.at(3) == Point3<double>(62.36, 6.60, 12.6));
//}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(PointSerializationTests)

BOOST_AUTO_TEST_CASE(test_point_wkt_output_stream)
{
    Point2d p2d(10.5, 20.0);
    std::stringstream ss;
    ss << wkt(p2d);

    BOOST_CHECK_EQUAL(ss.str(), "POINT (10.5 20)");

    Point3d p3d(1.0, 2.0, 3.5);
    ss.str("");
    ss << wkt(p3d);

    BOOST_CHECK_EQUAL(ss.str(), "POINT Z (1 2 3.5)");

    // Test XYM
    Point<double, xym_tag> p_m(10.0, 20.0, 500.0);
    ss.str("");
    ss << wkt(p_m);
    BOOST_CHECK_EQUAL(ss.str(), "POINT M (10 20 500)");

    // Test XYZM
    Point<double, xyzm_tag> p_zm(1.0, 2.0, 3.0, 99.0);
    ss.str("");
    ss << wkt(p_zm);
    BOOST_CHECK_EQUAL(ss.str(), "POINT ZM (1 2 3 99)");
}

BOOST_AUTO_TEST_CASE(test_point_wkt_formatter)
{
    Point2d p2d(40.96, -5.66);

    std::string formatted = tl::format("{}", wkt(p2d));

    BOOST_CHECK_EQUAL(formatted, "POINT (40.96 -5.66)");

    Point3d p3d(1.0, 2.0, 3.5);

    formatted = tl::format("{}", wkt(p3d));

    BOOST_CHECK_EQUAL(formatted, "POINT Z (1 2 3.5)");

    // Test XYM
    Point<double, xym_tag> p_m(10.0, 20.0, 500.0);
    formatted = tl::format("{}", wkt(p_m));
    BOOST_CHECK_EQUAL(formatted, "POINT M (10 20 500)");

    // Test XYZM
    Point<double, xyzm_tag> p_zm(1.0, 2.0, 3.0, 99.0);
    formatted = tl::format("{}", wkt(p_zm));
    BOOST_CHECK_EQUAL(formatted, "POINT ZM (1 2 3 99)");
}

BOOST_AUTO_TEST_CASE(test_point_precision_control)
{
    Point2d p(10.51234, 20.0);

    // Test 1: iostreams clásicos
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << wkt(p);
    BOOST_CHECK_EQUAL(ss.str(), "POINT (10.51 20.00)");

    ss.str(""); // Limpiar stream
    ss << std::scientific << std::setprecision(1) << wkt(p);
    BOOST_CHECK_EQUAL(ss.str(), "POINT (1.1e+01 2.0e+01)");
    ss.str(""); // Limpiar stream

    // Test 2: Formateo moderno con precisión
    // Usamos {:.3f} para forzar 3 decimales
    std::string formatted = tl::format("{:.3f}", wkt(p));
    BOOST_CHECK_EQUAL(formatted, "POINT (10.512 20.000)");

    formatted = tl::format("{}", wkt(p).withPrecision(3));
    BOOST_CHECK_EQUAL(formatted, "POINT (10.512 20.000)");
}

BOOST_AUTO_TEST_SUITE_END()