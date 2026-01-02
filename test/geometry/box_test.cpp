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

#define BOOST_TEST_MODULE Tidop bounding box test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/spatial/BoundingBox.h>

using namespace tl;
using namespace tl::geometry;


BOOST_AUTO_TEST_SUITE(BoundingBoxTestSuite)

struct BoundingBoxTest
{

    BoundingBoxTest()
      : box_integer(nullptr),
        box_double(nullptr),
        box_float(nullptr),
        box_integer_copy(nullptr),
        box_double_copy(nullptr),
        box_float_copy(nullptr),
        box_constructor_center_side_integer_even_size(nullptr),
        box_constructor_center_side_integer_odd_size(nullptr),
        box_constructor_center_side_double(nullptr),
        box_constructor_center_side_float(nullptr),
        box_constructor_center_size_integer_even_size(nullptr),
        box_constructor_center_size_integer_odd_size(nullptr),
        box_constructor_center_size_double(nullptr),
        box_constructor_center_size_float(nullptr)
    {

    }

    ~BoundingBoxTest()
    {
        delete box_integer;
        delete box_double;
        delete box_float;
        delete box_integer_copy;
        delete box_double_copy;
        delete box_float_copy;
        delete box_constructor_center_side_integer_even_size;
        delete box_constructor_center_side_integer_odd_size;
        delete box_constructor_center_side_double;
        delete box_constructor_center_side_float;
        delete box_constructor_center_size_integer_even_size;
        delete box_constructor_center_size_integer_odd_size;
        delete box_constructor_center_size_double;
        delete box_constructor_center_size_float;
    }

    void setup()
    {
        box_integer = new BoundingBox<Point3i>(Point3i(0, 0, 0), Point3i(100, 100, 100));
        box_double = new BoundingBox<Point3d>(Point3d(0., 0., 0.), Point3d(100., 100., 100.));
        box_float = new BoundingBox<Point3f>(Point3f(0.f, 0.f, 0.f), Point3f(100.f, 100.f, 100.f));

        box_integer_copy = new BoundingBox<Point3i>(*box_integer);
        box_double_copy = new BoundingBox<Point3d>(*box_double);
        box_float_copy = new BoundingBox<Point3f>(*box_float);

        ptc_integer = Point3i(50, 50, 50);
        ptc_double = Point3d(50.67, 50.76, 50.76);
        ptc_float = Point3f(50.67f, 34.45f, 10.23f);

        box_constructor_center_side_integer_even_size = new BoundingBox<Point3i>(ptc_integer, 50);
        box_constructor_center_side_integer_odd_size = new BoundingBox<Point3i>(ptc_integer, 51);
        box_constructor_center_side_double = new BoundingBox<Point3d>(ptc_double, 50.);
        box_constructor_center_side_float = new BoundingBox<Point3f>(ptc_float, 50.f);

        box_constructor_center_size_integer_even_size = new BoundingBox<Point3i>(ptc_integer, 50, 60, 70);
        box_constructor_center_size_integer_odd_size = new BoundingBox<Point3i>(ptc_integer, 51, 61, 71);
        box_constructor_center_size_double = new BoundingBox<Point3d>(ptc_double, 100.32, 254.25, 124.5);
        box_constructor_center_size_float = new BoundingBox<Point3f>(ptc_float, 100.34f, 254.23f, 123.34f);
    }

    void teardown()
    {

    }

    BoundingBox<Point3i> box_default_constructor_integer;
    BoundingBox<Point3d> box_default_constructor_double;
    BoundingBox<Point3f> box_default_constructor_float;

    BoundingBox<Point3i> *box_integer;
    BoundingBox<Point3d> *box_double;
    BoundingBox<Point3f> *box_float;
    BoundingBox<Point3i> *box_integer_copy;
    BoundingBox<Point3d> *box_double_copy;
    BoundingBox<Point3f> *box_float_copy;

    Point3i ptc_integer;
    Point3d ptc_double;
    Point3f ptc_float;

    BoundingBox<Point3i> *box_constructor_center_side_integer_even_size;
    BoundingBox<Point3i> *box_constructor_center_side_integer_odd_size;
    BoundingBox<Point3d> *box_constructor_center_side_double;
    BoundingBox<Point3f> *box_constructor_center_side_float;

    BoundingBox<Point3i> *box_constructor_center_size_integer_even_size;
    BoundingBox<Point3i> *box_constructor_center_size_integer_odd_size;
    BoundingBox<Point3d> *box_constructor_center_size_double;
    BoundingBox<Point3f> *box_constructor_center_size_float;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, BoundingBoxTest)
{
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box_default_constructor_integer.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box_default_constructor_integer.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box_default_constructor_integer.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), box_default_constructor_integer.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), box_default_constructor_integer.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), box_default_constructor_integer.pt2().z());

    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box_default_constructor_double.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box_default_constructor_double.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box_default_constructor_double.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box_default_constructor_double.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box_default_constructor_double.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box_default_constructor_double.pt2().z());

    BOOST_CHECK_EQUAL(std::numeric_limits<float>::max(), box_default_constructor_float.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::max(), box_default_constructor_float.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::max(), box_default_constructor_float.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::lowest(), box_default_constructor_float.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::lowest(), box_default_constructor_float.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::lowest(), box_default_constructor_float.pt2().z());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, BoundingBoxTest)
{
    BOOST_CHECK_EQUAL(box_integer->pt1().x(), box_integer_copy->pt1().x());
    BOOST_CHECK_EQUAL(box_integer->pt1().y(), box_integer_copy->pt1().y());
    BOOST_CHECK_EQUAL(box_integer->pt1().z(), box_integer_copy->pt1().z());
    BOOST_CHECK_EQUAL(box_integer->pt2().x(), box_integer_copy->pt2().x());
    BOOST_CHECK_EQUAL(box_integer->pt2().y(), box_integer_copy->pt2().y());
    BOOST_CHECK_EQUAL(box_integer->pt2().z(), box_integer_copy->pt2().z());

    BOOST_CHECK_EQUAL(box_double->pt1().x(), box_double_copy->pt1().x());
    BOOST_CHECK_EQUAL(box_double->pt1().y(), box_double_copy->pt1().y());
    BOOST_CHECK_EQUAL(box_double->pt1().z(), box_double_copy->pt1().z());
    BOOST_CHECK_EQUAL(box_double->pt2().x(), box_double_copy->pt2().x());
    BOOST_CHECK_EQUAL(box_double->pt2().y(), box_double_copy->pt2().y());
    BOOST_CHECK_EQUAL(box_double->pt2().z(), box_double_copy->pt2().z());

    BOOST_CHECK_EQUAL(box_float->pt1().x(), box_float_copy->pt1().x());
    BOOST_CHECK_EQUAL(box_float->pt1().y(), box_float_copy->pt1().y());
    BOOST_CHECK_EQUAL(box_float->pt1().z(), box_float_copy->pt1().z());
    BOOST_CHECK_EQUAL(box_float->pt2().x(), box_float_copy->pt2().x());
    BOOST_CHECK_EQUAL(box_float->pt2().y(), box_float_copy->pt2().y());
    BOOST_CHECK_EQUAL(box_float->pt2().z(), box_float_copy->pt2().z());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor_dif_types, BoundingBoxTest)
{
    BoundingBox<Point3d> box(Point3d(0.5, 0.4, 1.9), Point3d(100.6, 100.4, 105.49));
    BoundingBox<Point3i> box2(box);
    BOOST_CHECK_EQUAL(1, box2.pt1().x());
    BOOST_CHECK_EQUAL(0, box2.pt1().y());
    BOOST_CHECK_EQUAL(2, box2.pt1().z());
    BOOST_CHECK_EQUAL(101, box2.pt2().x());
    BOOST_CHECK_EQUAL(100, box2.pt2().y());
    BOOST_CHECK_EQUAL(105, box2.pt2().z());
}

BOOST_FIXTURE_TEST_CASE(constructor_center_side, BoundingBoxTest)
{
    BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_even_size->pt1().x());
    BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_even_size->pt1().y());
    BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_even_size->pt1().z());
    BOOST_CHECK_EQUAL(75, box_constructor_center_side_integer_even_size->pt2().x());
    BOOST_CHECK_EQUAL(75, box_constructor_center_side_integer_even_size->pt2().y());
    BOOST_CHECK_EQUAL(75, box_constructor_center_side_integer_even_size->pt2().z());

    BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_odd_size->pt1().x());
    BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_odd_size->pt1().y());
    BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_odd_size->pt1().z());
    BOOST_CHECK_EQUAL(76, box_constructor_center_side_integer_odd_size->pt2().x());
    BOOST_CHECK_EQUAL(76, box_constructor_center_side_integer_odd_size->pt2().y());
    BOOST_CHECK_EQUAL(76, box_constructor_center_side_integer_odd_size->pt2().z());

    BOOST_CHECK_CLOSE(25.67, box_constructor_center_side_double->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(25.76, box_constructor_center_side_double->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(25.76, box_constructor_center_side_double->pt1().z(), 0.01);
    BOOST_CHECK_CLOSE(75.67, box_constructor_center_side_double->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(75.76, box_constructor_center_side_double->pt2().y(), 0.01);
    BOOST_CHECK_CLOSE(75.76, box_constructor_center_side_double->pt2().z(), 0.01);

    BOOST_CHECK_CLOSE(25.67f, box_constructor_center_side_float->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(9.45f, box_constructor_center_side_float->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(-14.77f, box_constructor_center_side_float->pt1().z(), 0.01);
    BOOST_CHECK_CLOSE(75.67f, box_constructor_center_side_float->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(59.45f, box_constructor_center_side_float->pt2().y(), 0.01);
    BOOST_CHECK_CLOSE(35.23f, box_constructor_center_side_float->pt2().z(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(constructor_center_size, BoundingBoxTest)
{
    BOOST_CHECK_EQUAL(25, box_constructor_center_size_integer_even_size->pt1().x());
    BOOST_CHECK_EQUAL(20, box_constructor_center_size_integer_even_size->pt1().y());
    BOOST_CHECK_EQUAL(15, box_constructor_center_size_integer_even_size->pt1().z());
    BOOST_CHECK_EQUAL(75, box_constructor_center_size_integer_even_size->pt2().x());
    BOOST_CHECK_EQUAL(80, box_constructor_center_size_integer_even_size->pt2().y());
    BOOST_CHECK_EQUAL(85, box_constructor_center_size_integer_even_size->pt2().z());

    BOOST_CHECK_EQUAL(25, box_constructor_center_size_integer_odd_size->pt1().x());
    BOOST_CHECK_EQUAL(20, box_constructor_center_size_integer_odd_size->pt1().y());
    BOOST_CHECK_EQUAL(15, box_constructor_center_size_integer_odd_size->pt1().z());
    BOOST_CHECK_EQUAL(76, box_constructor_center_size_integer_odd_size->pt2().x());
    BOOST_CHECK_EQUAL(81, box_constructor_center_size_integer_odd_size->pt2().y());
    BOOST_CHECK_EQUAL(86, box_constructor_center_size_integer_odd_size->pt2().z());

    BOOST_CHECK_CLOSE(0.51, box_constructor_center_size_double->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(-76.365, box_constructor_center_size_double->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(-11.49, box_constructor_center_size_double->pt1().z(), 0.01);
    BOOST_CHECK_CLOSE(100.83, box_constructor_center_size_double->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(177.885, box_constructor_center_size_double->pt2().y(), 0.01);
    BOOST_CHECK_CLOSE(113.01, box_constructor_center_size_double->pt2().z(), 0.01);

    BOOST_CHECK_CLOSE(0.5f, box_constructor_center_size_float->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(-92.665f, box_constructor_center_size_float->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(-51.44f, box_constructor_center_size_float->pt1().z(), 0.01);
    BOOST_CHECK_CLOSE(100.84f, box_constructor_center_size_float->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(161.565f, box_constructor_center_size_float->pt2().y(), 0.01);
    BOOST_CHECK_CLOSE(71.9f, box_constructor_center_size_float->pt2().z(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, BoundingBoxTest)
{
    BoundingBox<Point3d> box(Point3d(50., 20., 30.), Point3d(100., 100., 100.));
    BoundingBox<Point3d> box2(std::move(box));
    BOOST_CHECK_EQUAL(50., box2.pt1().x());
    BOOST_CHECK_EQUAL(20., box2.pt1().y());
    BOOST_CHECK_EQUAL(30., box2.pt1().z());
    BOOST_CHECK_EQUAL(100., box2.pt2().x());
    BOOST_CHECK_EQUAL(100., box2.pt2().y());
    BOOST_CHECK_EQUAL(100., box2.pt2().z());
}

BOOST_FIXTURE_TEST_CASE(copy_assing_operator, BoundingBoxTest)
{
    BoundingBox<Point3d> box(Point3d(50., 20., 30.), Point3d(100., 100., 100.));
    BoundingBox<Point3d> box2;
    box2 = box;
    BOOST_CHECK_EQUAL(50., box2.pt1().x());
    BOOST_CHECK_EQUAL(20., box2.pt1().y());
    BOOST_CHECK_EQUAL(30., box2.pt1().z());
    BOOST_CHECK_EQUAL(100., box2.pt2().x());
    BOOST_CHECK_EQUAL(100., box2.pt2().y());
    BOOST_CHECK_EQUAL(100., box2.pt2().z());
}

BOOST_FIXTURE_TEST_CASE(move_assing_operator, BoundingBoxTest)
{
    BoundingBox<Point3d> box(Point3d(50., 20., 30.), Point3d(100., 100., 100.));
    BoundingBox<Point3d> box2;
    box2 = std::move(box);
    BOOST_CHECK_EQUAL(50., box2.pt1().x());
    BOOST_CHECK_EQUAL(20., box2.pt1().y());
    BOOST_CHECK_EQUAL(30., box2.pt1().z());
    BOOST_CHECK_EQUAL(100., box2.pt2().x());
    BOOST_CHECK_EQUAL(100., box2.pt2().y());
    BOOST_CHECK_EQUAL(100., box2.pt2().z());
}

BOOST_FIXTURE_TEST_CASE(center, BoundingBoxTest)
{
    BOOST_CHECK(Point3i(0, 0, 0) == box_default_constructor_integer.center());
    BOOST_CHECK(Point3d(0., 0., 0.) == box_default_constructor_double.center());
    BOOST_CHECK(Point3f(0.f, 0.f, 0.f) == box_default_constructor_float.center());

    BOOST_CHECK(Point3i(50, 50, 50) == box_integer->center());
    BOOST_CHECK(Point3d(50., 50., 50.) == box_double->center());
    BOOST_CHECK(Point3f(50.f, 50.f, 50.f) == box_float->center());

    BOOST_CHECK(Point3i(50, 50, 50) == box_integer_copy->center());
    BOOST_CHECK(Point3d(50., 50., 50.) == box_double_copy->center());
    BOOST_CHECK(Point3f(50.f, 50.f, 50.f) == box_float_copy->center());

    BOOST_CHECK(ptc_integer == box_constructor_center_side_integer_even_size->center());
    BOOST_CHECK(ptc_integer == box_constructor_center_side_integer_odd_size->center());
    BOOST_CHECK_CLOSE(ptc_double.x(), box_constructor_center_side_double->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_double.y(), box_constructor_center_side_double->center().y(), 0.01);
    BOOST_CHECK_CLOSE(ptc_double.z(), box_constructor_center_side_double->center().z(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.x(), box_constructor_center_side_float->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.y(), box_constructor_center_side_float->center().y(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.z(), box_constructor_center_side_float->center().z(), 0.01);
    BOOST_CHECK(ptc_integer == box_constructor_center_size_integer_even_size->center());
    BOOST_CHECK(ptc_integer == box_constructor_center_size_integer_odd_size->center());
    BOOST_CHECK_CLOSE(ptc_double.x(), box_constructor_center_size_double->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_double.y(), box_constructor_center_size_double->center().y(), 0.01);
    BOOST_CHECK_CLOSE(ptc_double.z(), box_constructor_center_size_double->center().z(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.x(), box_constructor_center_size_float->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.y(), box_constructor_center_size_float->center().y(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.z(), box_constructor_center_size_float->center().z(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(width, BoundingBoxTest)
{
    BOOST_CHECK_EQUAL(0, box_default_constructor_integer.width());
    BOOST_CHECK_EQUAL(0., box_default_constructor_double.width());
    BOOST_CHECK_EQUAL(0.f, box_default_constructor_float.width());

    BOOST_CHECK_EQUAL(100, box_integer->width());
    BOOST_CHECK_EQUAL(100., box_double->width());
    BOOST_CHECK_EQUAL(100.f, box_float->width());

    BOOST_CHECK_EQUAL(100, box_integer_copy->width());
    BOOST_CHECK_EQUAL(100., box_double_copy->width());
    BOOST_CHECK_EQUAL(100.f, box_float_copy->width());

    BOOST_CHECK_EQUAL(50, box_constructor_center_side_integer_even_size->width());
    BOOST_CHECK_EQUAL(51, box_constructor_center_side_integer_odd_size->width());
    BOOST_CHECK_EQUAL(50., box_constructor_center_side_double->width());
    BOOST_CHECK_EQUAL(50.f, box_constructor_center_side_float->width());
    BOOST_CHECK_EQUAL(50, box_constructor_center_size_integer_even_size->width());
    BOOST_CHECK_EQUAL(51, box_constructor_center_size_integer_odd_size->width());
    BOOST_CHECK_EQUAL(100.32, box_constructor_center_size_double->width());
    BOOST_CHECK_EQUAL(100.34f, box_constructor_center_size_float->width());
}

BOOST_FIXTURE_TEST_CASE(height, BoundingBoxTest)
{
    BOOST_CHECK_EQUAL(0, box_default_constructor_integer.height());
    BOOST_CHECK_EQUAL(0., box_default_constructor_double.height());
    BOOST_CHECK_EQUAL(0.f, box_default_constructor_float.height());

    BOOST_CHECK_EQUAL(100, box_integer->height());
    BOOST_CHECK_EQUAL(100., box_double->height());
    BOOST_CHECK_EQUAL(100.f, box_float->height());

    BOOST_CHECK_EQUAL(100, box_integer_copy->height());
    BOOST_CHECK_EQUAL(100., box_double_copy->height());
    BOOST_CHECK_EQUAL(100.f, box_float_copy->height());

    BOOST_CHECK_EQUAL(50, box_constructor_center_side_integer_even_size->height());
    BOOST_CHECK_EQUAL(51, box_constructor_center_side_integer_odd_size->height());
    BOOST_CHECK_CLOSE(50., box_constructor_center_side_double->height(), 0.01);
    BOOST_CHECK_CLOSE(50.f, box_constructor_center_side_float->height(), 0.01);
    BOOST_CHECK_EQUAL(60, box_constructor_center_size_integer_even_size->height());
    BOOST_CHECK_EQUAL(61, box_constructor_center_size_integer_odd_size->height());
    BOOST_CHECK_CLOSE(254.25, box_constructor_center_size_double->height(), 0.01);
    BOOST_CHECK_CLOSE(254.23f, box_constructor_center_size_float->height(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(depth, BoundingBoxTest)
{
    BOOST_CHECK_EQUAL(0, box_default_constructor_integer.depth());
    BOOST_CHECK_EQUAL(0., box_default_constructor_double.depth());
    BOOST_CHECK_EQUAL(0.f, box_default_constructor_float.depth());

    BOOST_CHECK_EQUAL(100, box_integer->depth());
    BOOST_CHECK_EQUAL(100., box_double->depth());
    BOOST_CHECK_EQUAL(100.f, box_float->depth());

    BOOST_CHECK_EQUAL(100, box_integer_copy->depth());
    BOOST_CHECK_EQUAL(100., box_double_copy->depth());
    BOOST_CHECK_EQUAL(100.f, box_float_copy->depth());

    BOOST_CHECK_EQUAL(50, box_constructor_center_side_integer_even_size->depth());
    BOOST_CHECK_EQUAL(51, box_constructor_center_side_integer_odd_size->depth());
    BOOST_CHECK_CLOSE(50., box_constructor_center_side_double->depth(), 0.01);
    BOOST_CHECK_CLOSE(50.f, box_constructor_center_side_float->depth(), 0.01);
    BOOST_CHECK_EQUAL(70, box_constructor_center_size_integer_even_size->depth());
    BOOST_CHECK_EQUAL(71, box_constructor_center_size_integer_odd_size->depth());
    BOOST_CHECK_CLOSE(124.5, box_constructor_center_size_double->depth(), 0.01);
    BOOST_CHECK_CLOSE(123.34f, box_constructor_center_size_float->depth(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, BoundingBoxTest)
{
    BOOST_CHECK(box_default_constructor_integer.isEmpty());
    BOOST_CHECK(box_integer->isEmpty() == false);
    BOOST_CHECK(box_integer_copy->isEmpty() == false);
    BOOST_CHECK(box_constructor_center_side_integer_even_size->isEmpty() == false);
    BOOST_CHECK(box_constructor_center_size_integer_even_size->isEmpty() == false);
}

BOOST_FIXTURE_TEST_CASE(isValid, BoundingBoxTest)
{
    BOOST_CHECK(!box_default_constructor_integer.isValid());
    BOOST_CHECK(box_integer->isValid() == true);
    BOOST_CHECK(box_integer_copy->isValid() == true);
    BOOST_CHECK(box_constructor_center_side_integer_even_size->isValid() == true);
    BOOST_CHECK(box_constructor_center_size_integer_even_size->isValid() == true);
}

//BOOST_FIXTURE_TEST_CASE(containsPoint, BoundingBoxTest) 
//{
//  Point3i point1(0, 0, 0);
//  Point3i point2(50, 50, 50);
//  Point3i point3(-50, -50, -50);
//  BOOST_CHECK_EQUAL(false, box_default_constructor_integer.containsPoint(point1));
//  BOOST_CHECK_EQUAL(true, box_integer->containsPoint(point1));
//  BOOST_CHECK_EQUAL(true, box_integer->containsPoint(point2));
//  BOOST_CHECK_EQUAL(false, box_integer->containsPoint(point3));
//
//}
//
//BOOST_FIXTURE_TEST_CASE(containsBox, BoundingBoxTest)
//{
//  BoundingBox<Point3i> box(Point3i(0, 0, 0), Point3i(100, 100, 100));
//  BOOST_CHECK_EQUAL(false, box_default_constructor_integer.containsBox(BoundingBox<Point3i>(Point3i(50, 50, 50), 50)));
//  BOOST_CHECK_EQUAL(true, box_integer->containsBox(BoundingBox<Point3i>(Point3i(50, 50, 50), 50)));
//  BOOST_CHECK_EQUAL(false, box_integer->containsBox(BoundingBox<Point3i>(Point3i(200, 50, 50), 50)));
//}

BOOST_AUTO_TEST_SUITE_END()

//BOOST_AUTO_TEST_CASE(Box_join)
//{
//  BoundingBox<Point3i> box1(Point3i(0, 0, 0), Point3i(50, 50, 50));
//  BoundingBox<Point3i> box2(Point3i(20, 30, 10), Point3i(100, 100, 100));
//  BoundingBox<Point3i> box3 = joinBoundingBoxes(box1, box2);
//  BOOST_CHECK_EQUAL(0, box3.pt1().x());
//  BOOST_CHECK_EQUAL(0, box3.pt1().y());
//  BOOST_CHECK_EQUAL(0, box3.pt1().z());
//  BOOST_CHECK_EQUAL(100, box3.pt2().x());
//  BOOST_CHECK_EQUAL(100, box3.pt2().y());
//  BOOST_CHECK_EQUAL(100, box3.pt2().z());
//}
//
//BOOST_AUTO_TEST_CASE(Box_intersect)
//{
//  BoundingBox<Point3i> box1(Point3i(0, 0, 0), Point3i(50, 50, 50));
//  BoundingBox<Point3i> box2(Point3i(20, 30, 10), Point3i(100, 100, 100));
//  BoundingBox<Point3i> box3 = intersectBoundingBoxes(box1, box2);
//  BOOST_CHECK_EQUAL(20, box3.pt1().x());
//  BOOST_CHECK_EQUAL(30, box3.pt1().y());
//  BOOST_CHECK_EQUAL(10, box3.pt1().z());
//  BOOST_CHECK_EQUAL(50, box3.pt2().x());
//  BOOST_CHECK_EQUAL(50, box3.pt2().y());
//  BOOST_CHECK_EQUAL(50, box3.pt2().z());
//}
//
//BOOST_AUTO_TEST_CASE(Box_cast)
//{
//  const BoundingBox<Point3d> box(Point3d(0.5, 0.4, 1.9), Point3d(100.6, 100.4, 105.49));
//  BoundingBox<Point3i> box2 = static_cast<BoundingBox<Point3i>>(box);
//  BOOST_CHECK_EQUAL(1, box2.pt1().x());
//  BOOST_CHECK_EQUAL(0, box2.pt1().y());
//  BOOST_CHECK_EQUAL(2, box2.pt1().z());
//  BOOST_CHECK_EQUAL(101, box2.pt2().x());
//  BOOST_CHECK_EQUAL(100, box2.pt2().y());
//  BOOST_CHECK_EQUAL(105, box2.pt2().z());
//}



BOOST_AUTO_TEST_SUITE(BoundingBox2DTestSuite)

struct BoundingBox2DTest
{

    BoundingBox2DTest()
      : bbox_integer(nullptr),
        bbox_double(nullptr),
        bbox_float(nullptr),
        bbox_integer_copy(nullptr),
        bbox_double_copy(nullptr),
        bbox_float_copy(nullptr),
        bbox_constructor_center_side_integer_even_size(nullptr),
        bbox_constructor_center_side_integer_odd_size(nullptr),
        bbox_constructor_center_side_double(nullptr),
        bbox_constructor_center_side_float(nullptr),
        bbox_constructor_center_size_integer_even_size(nullptr),
        bbox_constructor_center_size_integer_odd_size(nullptr),
        bbox_constructor_center_size_double(nullptr),
        bbox_constructor_center_size_float(nullptr)
    {

    }

    ~BoundingBox2DTest()
    {
        delete bbox_integer;
        delete bbox_double;
        delete bbox_float;
        delete bbox_integer_copy;
        delete bbox_double_copy;
        delete bbox_float_copy;
        delete bbox_constructor_center_side_integer_even_size;
        delete bbox_constructor_center_side_integer_odd_size;
        delete bbox_constructor_center_side_double;
        delete bbox_constructor_center_side_float;
        delete bbox_constructor_center_size_integer_even_size;
        delete bbox_constructor_center_size_integer_odd_size;
        delete bbox_constructor_center_size_double;
        delete bbox_constructor_center_size_float;
    }

    void setup()
    {
        bbox_integer = new BoundingBox<Point2i>(Point2i(0, 0), Point2i(100, 100));
        bbox_double = new BoundingBox<Point2d>(Point2d(0., 0.), Point2d(100., 100.));
        bbox_float = new BoundingBox<Point2f>(Point2f(0.f, 0.f), Point2f(100.f, 100.f));

        bbox_integer_copy = new BoundingBox<Point2i>(*bbox_integer);
        bbox_double_copy = new BoundingBox<Point2d>(*bbox_double);
        bbox_float_copy = new BoundingBox<Point2f>(*bbox_float);

        ptc_integer = Point2i(50, 50);
        ptc_double = Point2d(50.67, 50.76);
        ptc_float = Point2f(50.67f, 34.45f);

        bbox_constructor_center_side_integer_even_size = new BoundingBox<Point2i>(ptc_integer, 50);
        bbox_constructor_center_side_integer_odd_size = new BoundingBox<Point2i>(ptc_integer, 51);
        bbox_constructor_center_side_double = new BoundingBox<Point2d>(ptc_double, 50.);
        bbox_constructor_center_side_float = new BoundingBox<Point2f>(ptc_float, 50.f);

        bbox_constructor_center_size_integer_even_size = new BoundingBox<Point2i>(ptc_integer, 50, 60);
        bbox_constructor_center_size_integer_odd_size = new BoundingBox<Point2i>(ptc_integer, 51, 61);
        bbox_constructor_center_size_double = new BoundingBox<Point2d>(ptc_double, 100.32, 254.25);
        bbox_constructor_center_size_float = new BoundingBox<Point2f>(ptc_float, 100.34f, 254.23f);
    }

    void teardown()
    {

    }

    BoundingBox<Point2i> bbox_default_constructor_integer;
    BoundingBox<Point2d> bbox_default_constructor_double;
    BoundingBox<Point2f> bbox_default_constructor_float;

    BoundingBox<Point2i> *bbox_integer;
    BoundingBox<Point2d> *bbox_double;
    BoundingBox<Point2f> *bbox_float;
    BoundingBox<Point2i> *bbox_integer_copy;
    BoundingBox<Point2d> *bbox_double_copy;
    BoundingBox<Point2f> *bbox_float_copy;

    Point2i ptc_integer;
    Point2d ptc_double;
    Point2f ptc_float;

    BoundingBox<Point2i> *bbox_constructor_center_side_integer_even_size;
    BoundingBox<Point2i> *bbox_constructor_center_side_integer_odd_size;
    BoundingBox<Point2d> *bbox_constructor_center_side_double;
    BoundingBox<Point2f> *bbox_constructor_center_side_float;

    BoundingBox<Point2i> *bbox_constructor_center_size_integer_even_size;
    BoundingBox<Point2i> *bbox_constructor_center_size_integer_odd_size;
    BoundingBox<Point2d> *bbox_constructor_center_size_double;
    BoundingBox<Point2f> *bbox_constructor_center_size_float;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), bbox_default_constructor_integer.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), bbox_default_constructor_integer.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), bbox_default_constructor_integer.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), bbox_default_constructor_integer.pt2().y());

    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_default_constructor_double.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_default_constructor_double.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_default_constructor_double.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_default_constructor_double.pt2().y());

    BOOST_CHECK_EQUAL(std::numeric_limits<float>::max(), bbox_default_constructor_float.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::max(), bbox_default_constructor_float.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::lowest(), bbox_default_constructor_float.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::lowest(), bbox_default_constructor_float.pt2().y());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(bbox_integer->pt1().x(), bbox_integer_copy->pt1().x());
    BOOST_CHECK_EQUAL(bbox_integer->pt1().y(), bbox_integer_copy->pt1().y());
    BOOST_CHECK_EQUAL(bbox_integer->pt2().x(), bbox_integer_copy->pt2().x());
    BOOST_CHECK_EQUAL(bbox_integer->pt2().y(), bbox_integer_copy->pt2().y());

    BOOST_CHECK_EQUAL(bbox_double->pt1().x(), bbox_double_copy->pt1().x());
    BOOST_CHECK_EQUAL(bbox_double->pt1().y(), bbox_double_copy->pt1().y());
    BOOST_CHECK_EQUAL(bbox_double->pt2().x(), bbox_double_copy->pt2().x());
    BOOST_CHECK_EQUAL(bbox_double->pt2().y(), bbox_double_copy->pt2().y());

    BOOST_CHECK_EQUAL(bbox_float->pt1().x(), bbox_float_copy->pt1().x());
    BOOST_CHECK_EQUAL(bbox_float->pt1().y(), bbox_float_copy->pt1().y());
    BOOST_CHECK_EQUAL(bbox_float->pt2().x(), bbox_float_copy->pt2().x());
    BOOST_CHECK_EQUAL(bbox_float->pt2().y(), bbox_float_copy->pt2().y());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor_dif_types, BoundingBox2DTest)
{
    const BoundingBox<Point2d> w(Point2d(0.5, 0.4), Point2d(100.6, 100.4));
    BoundingBox<Point2i> w2(w);
    BOOST_CHECK_EQUAL(1, w2.pt1().x());
    BOOST_CHECK_EQUAL(0, w2.pt1().y());
    BOOST_CHECK_EQUAL(101, w2.pt2().x());
    BOOST_CHECK_EQUAL(100, w2.pt2().y());

    const BoundingBox<Point2i> w_int(Point2i(1, 4), Point2i(100, 100));
    BoundingBox<Point2d> w_double(w_int);
    BOOST_CHECK_EQUAL(1, w_double.pt1().x());
    BOOST_CHECK_EQUAL(4, w_double.pt1().y());
    BOOST_CHECK_EQUAL(100, w_double.pt2().x());
    BOOST_CHECK_EQUAL(100, w_double.pt2().y());
}

BOOST_FIXTURE_TEST_CASE(constructor_center_side, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(25, bbox_constructor_center_side_integer_even_size->pt1().x());
    BOOST_CHECK_EQUAL(25, bbox_constructor_center_side_integer_even_size->pt1().y());
    BOOST_CHECK_EQUAL(75, bbox_constructor_center_side_integer_even_size->pt2().x());
    BOOST_CHECK_EQUAL(75, bbox_constructor_center_side_integer_even_size->pt2().y());

    BOOST_CHECK_EQUAL(25, bbox_constructor_center_side_integer_odd_size->pt1().x());
    BOOST_CHECK_EQUAL(25, bbox_constructor_center_side_integer_odd_size->pt1().y());
    BOOST_CHECK_EQUAL(76, bbox_constructor_center_side_integer_odd_size->pt2().x());
    BOOST_CHECK_EQUAL(76, bbox_constructor_center_side_integer_odd_size->pt2().y());

    BOOST_CHECK_CLOSE(25.67, bbox_constructor_center_side_double->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(25.76, bbox_constructor_center_side_double->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(75.67, bbox_constructor_center_side_double->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(75.76, bbox_constructor_center_side_double->pt2().y(), 0.01);

    BOOST_CHECK_CLOSE(25.67f, bbox_constructor_center_side_float->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(9.45f, bbox_constructor_center_side_float->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(75.67f, bbox_constructor_center_side_float->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(59.45f, bbox_constructor_center_side_float->pt2().y(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(constructor_center_size, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(25, bbox_constructor_center_size_integer_even_size->pt1().x());
    BOOST_CHECK_EQUAL(20, bbox_constructor_center_size_integer_even_size->pt1().y());
    BOOST_CHECK_EQUAL(75, bbox_constructor_center_size_integer_even_size->pt2().x());
    BOOST_CHECK_EQUAL(80, bbox_constructor_center_size_integer_even_size->pt2().y());

    BOOST_CHECK_EQUAL(25, bbox_constructor_center_size_integer_odd_size->pt1().x());
    BOOST_CHECK_EQUAL(20, bbox_constructor_center_size_integer_odd_size->pt1().y());
    BOOST_CHECK_EQUAL(76, bbox_constructor_center_size_integer_odd_size->pt2().x());
    BOOST_CHECK_EQUAL(81, bbox_constructor_center_size_integer_odd_size->pt2().y());

    BOOST_CHECK_CLOSE(0.51, bbox_constructor_center_size_double->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(-76.365, bbox_constructor_center_size_double->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(100.83, bbox_constructor_center_size_double->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(177.885, bbox_constructor_center_size_double->pt2().y(), 0.01);

    BOOST_CHECK_CLOSE(0.5f, bbox_constructor_center_size_float->pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(-92.665f, bbox_constructor_center_size_float->pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(100.84f, bbox_constructor_center_size_float->pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(161.565f, bbox_constructor_center_size_float->pt2().y(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(center, BoundingBox2DTest)
{
    BOOST_CHECK(Point2i(0, 0) == bbox_default_constructor_integer.center());
    BOOST_CHECK(Point2d(0., 0.) == bbox_default_constructor_double.center());
    BOOST_CHECK(Point2f(0.f, 0.f) == bbox_default_constructor_float.center());

    BOOST_CHECK(Point2i(50, 50) == bbox_integer->center());
    BOOST_CHECK(Point2d(50., 50.) == bbox_double->center());
    BOOST_CHECK(Point2f(50.f, 50.f) == bbox_float->center());

    BOOST_CHECK(Point2i(50, 50) == bbox_integer_copy->center());
    BOOST_CHECK(Point2d(50., 50.) == bbox_double_copy->center());
    BOOST_CHECK(Point2f(50.f, 50.f) == bbox_float_copy->center());

    BOOST_CHECK(ptc_integer == bbox_constructor_center_side_integer_even_size->center());
    BOOST_CHECK(ptc_integer == bbox_constructor_center_side_integer_odd_size->center());
    BOOST_CHECK_CLOSE(ptc_double.x(), bbox_constructor_center_side_double->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_double.y(), bbox_constructor_center_side_double->center().y(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.x(), bbox_constructor_center_side_float->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.y(), bbox_constructor_center_side_float->center().y(), 0.01);
    BOOST_CHECK(ptc_integer == bbox_constructor_center_size_integer_even_size->center());
    BOOST_CHECK(ptc_integer == bbox_constructor_center_size_integer_odd_size->center());
    BOOST_CHECK_CLOSE(ptc_double.x(), bbox_constructor_center_size_double->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_double.y(), bbox_constructor_center_size_double->center().y(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.x(), bbox_constructor_center_size_float->center().x(), 0.01);
    BOOST_CHECK_CLOSE(ptc_float.y(), bbox_constructor_center_size_float->center().y(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(width, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(0, bbox_default_constructor_integer.width());
    BOOST_CHECK_EQUAL(0., bbox_default_constructor_double.width());
    BOOST_CHECK_EQUAL(0.f, bbox_default_constructor_float.width());

    BOOST_CHECK_EQUAL(100, bbox_integer->width());
    BOOST_CHECK_EQUAL(100., bbox_double->width());
    BOOST_CHECK_EQUAL(100.f, bbox_float->width());

    BOOST_CHECK_EQUAL(100, bbox_integer_copy->width());
    BOOST_CHECK_EQUAL(100., bbox_double_copy->width());
    BOOST_CHECK_EQUAL(100.f, bbox_float_copy->width());

    BOOST_CHECK_EQUAL(50, bbox_constructor_center_side_integer_even_size->width());
    BOOST_CHECK_EQUAL(51, bbox_constructor_center_side_integer_odd_size->width());
    BOOST_CHECK_EQUAL(50., bbox_constructor_center_side_double->width());
    BOOST_CHECK_EQUAL(50.f, bbox_constructor_center_side_float->width());
    BOOST_CHECK_EQUAL(50, bbox_constructor_center_size_integer_even_size->width());
    BOOST_CHECK_EQUAL(51, bbox_constructor_center_size_integer_odd_size->width());
    BOOST_CHECK_EQUAL(100.32, bbox_constructor_center_size_double->width());
    BOOST_CHECK_EQUAL(100.34f, bbox_constructor_center_size_float->width());
}

BOOST_FIXTURE_TEST_CASE(height, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(0, bbox_default_constructor_integer.height());
    BOOST_CHECK_EQUAL(0., bbox_default_constructor_double.height());
    BOOST_CHECK_EQUAL(0.f, bbox_default_constructor_float.height());

    BOOST_CHECK_EQUAL(100, bbox_integer->height());
    BOOST_CHECK_EQUAL(100., bbox_double->height());
    BOOST_CHECK_EQUAL(100.f, bbox_float->height());

    BOOST_CHECK_EQUAL(100, bbox_integer_copy->height());
    BOOST_CHECK_EQUAL(100., bbox_double_copy->height());
    BOOST_CHECK_EQUAL(100.f, bbox_float_copy->height());

    BOOST_CHECK_EQUAL(50, bbox_constructor_center_side_integer_even_size->height());
    BOOST_CHECK_EQUAL(51, bbox_constructor_center_side_integer_odd_size->height());
    BOOST_CHECK_CLOSE(50., bbox_constructor_center_side_double->height(), 0.01);
    BOOST_CHECK_CLOSE(50.f, bbox_constructor_center_side_float->height(), 0.01);
    BOOST_CHECK_EQUAL(60, bbox_constructor_center_size_integer_even_size->height());
    BOOST_CHECK_EQUAL(61, bbox_constructor_center_size_integer_odd_size->height());
    BOOST_CHECK_CLOSE(254.25, bbox_constructor_center_size_double->height(), 0.01);
    BOOST_CHECK_CLOSE(254.23f, bbox_constructor_center_size_float->height(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(true, bbox_default_constructor_integer.isEmpty());
    BOOST_CHECK_EQUAL(false, bbox_integer->isEmpty());
    BOOST_CHECK_EQUAL(false, bbox_integer_copy->isEmpty());
    BOOST_CHECK_EQUAL(false, bbox_constructor_center_side_integer_even_size->isEmpty());
    BOOST_CHECK_EQUAL(false, bbox_constructor_center_size_integer_even_size->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(isValid, BoundingBox2DTest)
{
    BOOST_CHECK_EQUAL(false, bbox_default_constructor_integer.isValid());
    BOOST_CHECK_EQUAL(true, bbox_integer->isValid());
    BOOST_CHECK_EQUAL(true, bbox_integer_copy->isValid());
    BOOST_CHECK_EQUAL(true, bbox_constructor_center_side_integer_even_size->isValid());
    BOOST_CHECK_EQUAL(true, bbox_constructor_center_size_integer_even_size->isValid());
}

BOOST_FIXTURE_TEST_CASE(normalized, BoundingBox2DTest)
{
    BoundingBox<Point2i> bbox(Point2i(100, 100), Point2i(0, 0));

    BOOST_CHECK_EQUAL(true, bbox.isValid());

    bbox.pt1().x() = 100;
    bbox.pt1().y() = 100;
    bbox.pt2().x() = 0;
    bbox.pt2().y() = 0;

    BOOST_CHECK_EQUAL(false, bbox.isValid());

    bbox.normalized();

    BOOST_CHECK_EQUAL(true, bbox.isValid());
    BOOST_CHECK_EQUAL(0, bbox.pt1().x());
    BOOST_CHECK_EQUAL(0, bbox.pt1().y());
    BOOST_CHECK_EQUAL(100, bbox.pt2().x());
    BOOST_CHECK_EQUAL(100, bbox.pt2().y());
}

//BOOST_FIXTURE_TEST_CASE(containsPoint, BoundingBox2DTest)
//{
//    Point2i point1(0, 0);
//    Point2i point2(50, 50);
//    Point2i point3(-50, -50);
//    BOOST_CHECK_EQUAL(false, bbox_default_constructor_integer.containsPoint(point1));
//    BOOST_CHECK_EQUAL(true, bbox_integer->containsPoint(point1));
//    BOOST_CHECK_EQUAL(true, bbox_integer->containsPoint(point2));
//    BOOST_CHECK_EQUAL(false, bbox_integer->containsPoint(point3));
//
//}

//BOOST_FIXTURE_TEST_CASE(containsWindow, BoundingBox2DTest)
//{
//    BoundingBox w1(Point2i(50, 50), 50);
//    BoundingBox w2(Point2i(200, 50), 50);
//    BOOST_CHECK_EQUAL(false, bbox_default_constructor_integer.containsWindow(w1));
//    BOOST_CHECK_EQUAL(true, bbox_integer->containsWindow(w1));
//    BOOST_CHECK_EQUAL(false, bbox_integer->containsWindow(w2));
//}

BOOST_AUTO_TEST_SUITE_END()