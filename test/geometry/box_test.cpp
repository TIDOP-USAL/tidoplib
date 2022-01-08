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
#include <tidop/geometry/entities/bbox.h>

using namespace tl;


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
    box_integer = new BoundingBoxI(Point3I(0, 0, 0), Point3I(100, 100, 100));
    box_double = new BoundingBoxD(Point3D(0., 0., 0.), Point3D(100., 100., 100.));
    box_float = new BoundingBoxF(Point3F(0.f, 0.f, 0.f), Point3F(100.f, 100.f, 100.f));

    box_integer_copy = new BoundingBoxI(*box_integer);
    box_double_copy = new BoundingBoxD(*box_double);
    box_float_copy = new BoundingBoxF(*box_float);

    ptc_integer = Point3I(50, 50, 50);
    ptc_double = Point3D(50.67, 50.76, 50.76);
    ptc_float = Point3F(50.67f, 34.45f, 10.23f);
    
    box_constructor_center_side_integer_even_size = new BoundingBoxI(ptc_integer, 50);
    box_constructor_center_side_integer_odd_size = new BoundingBoxI(ptc_integer, 51);
    box_constructor_center_side_double = new BoundingBoxD(ptc_double, 50.);
    box_constructor_center_side_float = new BoundingBoxF(ptc_float, 50.f);

    box_constructor_center_size_integer_even_size = new BoundingBoxI(ptc_integer, 50, 60, 70);
    box_constructor_center_size_integer_odd_size = new BoundingBoxI(ptc_integer, 51, 61, 71);
    box_constructor_center_size_double = new BoundingBoxD(ptc_double, 100.32, 254.25, 124.5);
    box_constructor_center_size_float = new BoundingBoxF(ptc_float, 100.34f, 254.23f, 123.34f);
  }
 
  void teardown()
  {

  }

  BoundingBoxI box_default_constructor_integer;
  BoundingBoxD box_default_constructor_double;
  BoundingBoxF box_default_constructor_float;

  BoundingBoxI *box_integer;
  BoundingBoxD *box_double;
  BoundingBoxF *box_float;
  BoundingBoxI *box_integer_copy;
  BoundingBoxD *box_double_copy;
  BoundingBoxF *box_float_copy;

  Point3I ptc_integer;
  Point3D ptc_double;
  Point3F ptc_float;

  BoundingBoxI *box_constructor_center_side_integer_even_size;
  BoundingBoxI *box_constructor_center_side_integer_odd_size;
  BoundingBoxD *box_constructor_center_side_double;
  BoundingBoxF *box_constructor_center_side_float;

  BoundingBoxI *box_constructor_center_size_integer_even_size;
  BoundingBoxI *box_constructor_center_size_integer_odd_size;
  BoundingBoxD *box_constructor_center_size_double;
  BoundingBoxF *box_constructor_center_size_float;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, BoundingBoxTest) 
{
  BOOST_CHECK_EQUAL(TL_INT_MAX, box_default_constructor_integer.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box_default_constructor_integer.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box_default_constructor_integer.pt1.z);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box_default_constructor_integer.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box_default_constructor_integer.pt2.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box_default_constructor_integer.pt2.z);

  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box_default_constructor_double.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box_default_constructor_double.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box_default_constructor_double.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box_default_constructor_double.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box_default_constructor_double.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box_default_constructor_double.pt2.z);
    
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box_default_constructor_float.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box_default_constructor_float.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box_default_constructor_float.pt1.z);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box_default_constructor_float.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box_default_constructor_float.pt2.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box_default_constructor_float.pt2.z);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, BoundingBoxTest) 
{
  BOOST_CHECK_EQUAL(box_integer->pt1.x, box_integer_copy->pt1.x);
  BOOST_CHECK_EQUAL(box_integer->pt1.y, box_integer_copy->pt1.y);
  BOOST_CHECK_EQUAL(box_integer->pt1.z, box_integer_copy->pt1.z);
  BOOST_CHECK_EQUAL(box_integer->pt2.x, box_integer_copy->pt2.x);
  BOOST_CHECK_EQUAL(box_integer->pt2.y, box_integer_copy->pt2.y);
  BOOST_CHECK_EQUAL(box_integer->pt2.z, box_integer_copy->pt2.z);

  BOOST_CHECK_EQUAL(box_double->pt1.x, box_double_copy->pt1.x);
  BOOST_CHECK_EQUAL(box_double->pt1.y, box_double_copy->pt1.y);
  BOOST_CHECK_EQUAL(box_double->pt1.z, box_double_copy->pt1.z);
  BOOST_CHECK_EQUAL(box_double->pt2.x, box_double_copy->pt2.x);
  BOOST_CHECK_EQUAL(box_double->pt2.y, box_double_copy->pt2.y);
  BOOST_CHECK_EQUAL(box_double->pt2.z, box_double_copy->pt2.z);
    
  BOOST_CHECK_EQUAL(box_float->pt1.x, box_float_copy->pt1.x);
  BOOST_CHECK_EQUAL(box_float->pt1.y, box_float_copy->pt1.y);
  BOOST_CHECK_EQUAL(box_float->pt1.z, box_float_copy->pt1.z);
  BOOST_CHECK_EQUAL(box_float->pt2.x, box_float_copy->pt2.x);
  BOOST_CHECK_EQUAL(box_float->pt2.y, box_float_copy->pt2.y);
  BOOST_CHECK_EQUAL(box_float->pt2.z, box_float_copy->pt2.z);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor_dif_types, BoundingBoxTest)
{
  BoundingBoxD box(Point3D(0.5, 0.4, 1.9), Point3D(100.6, 100.4, 105.49));
  BoundingBoxI box2(box);
  BOOST_CHECK_EQUAL(1, box2.pt1.x);
  BOOST_CHECK_EQUAL(0, box2.pt1.y);
  BOOST_CHECK_EQUAL(2, box2.pt1.z);
  BOOST_CHECK_EQUAL(101, box2.pt2.x);
  BOOST_CHECK_EQUAL(100, box2.pt2.y);
  BOOST_CHECK_EQUAL(105, box2.pt2.z);
}

BOOST_FIXTURE_TEST_CASE(constructor_center_side, BoundingBoxTest) 
{
  BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_even_size->pt1.x);
  BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_even_size->pt1.y);
  BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_even_size->pt1.z);
  BOOST_CHECK_EQUAL(75, box_constructor_center_side_integer_even_size->pt2.x);
  BOOST_CHECK_EQUAL(75, box_constructor_center_side_integer_even_size->pt2.y);
  BOOST_CHECK_EQUAL(75, box_constructor_center_side_integer_even_size->pt2.z);

  BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_odd_size->pt1.x);
  BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_odd_size->pt1.y);
  BOOST_CHECK_EQUAL(25, box_constructor_center_side_integer_odd_size->pt1.z);
  BOOST_CHECK_EQUAL(76, box_constructor_center_side_integer_odd_size->pt2.x);
  BOOST_CHECK_EQUAL(76, box_constructor_center_side_integer_odd_size->pt2.y);
  BOOST_CHECK_EQUAL(76, box_constructor_center_side_integer_odd_size->pt2.z);

  BOOST_CHECK_CLOSE(25.67, box_constructor_center_side_double->pt1.x, 0.01);
  BOOST_CHECK_CLOSE(25.76, box_constructor_center_side_double->pt1.y, 0.01);
  BOOST_CHECK_CLOSE(25.76, box_constructor_center_side_double->pt1.z, 0.01);
  BOOST_CHECK_CLOSE(75.67, box_constructor_center_side_double->pt2.x, 0.01);
  BOOST_CHECK_CLOSE(75.76, box_constructor_center_side_double->pt2.y, 0.01);
  BOOST_CHECK_CLOSE(75.76, box_constructor_center_side_double->pt2.z, 0.01);
    
  BOOST_CHECK_CLOSE(25.67f, box_constructor_center_side_float->pt1.x, 0.01);
  BOOST_CHECK_CLOSE(9.45f, box_constructor_center_side_float->pt1.y, 0.01);
  BOOST_CHECK_CLOSE(-14.77f, box_constructor_center_side_float->pt1.z, 0.01);
  BOOST_CHECK_CLOSE(75.67f, box_constructor_center_side_float->pt2.x, 0.01);
  BOOST_CHECK_CLOSE(59.45f, box_constructor_center_side_float->pt2.y, 0.01);
  BOOST_CHECK_CLOSE(35.23f, box_constructor_center_side_float->pt2.z, 0.01);
}

BOOST_FIXTURE_TEST_CASE(constructor_center_size, BoundingBoxTest) 
{
  BOOST_CHECK_EQUAL(25, box_constructor_center_size_integer_even_size->pt1.x);
  BOOST_CHECK_EQUAL(20, box_constructor_center_size_integer_even_size->pt1.y);
  BOOST_CHECK_EQUAL(15, box_constructor_center_size_integer_even_size->pt1.z);
  BOOST_CHECK_EQUAL(75, box_constructor_center_size_integer_even_size->pt2.x);
  BOOST_CHECK_EQUAL(80, box_constructor_center_size_integer_even_size->pt2.y);
  BOOST_CHECK_EQUAL(85, box_constructor_center_size_integer_even_size->pt2.z);

  BOOST_CHECK_EQUAL(25, box_constructor_center_size_integer_odd_size->pt1.x);
  BOOST_CHECK_EQUAL(20, box_constructor_center_size_integer_odd_size->pt1.y);
  BOOST_CHECK_EQUAL(15, box_constructor_center_size_integer_odd_size->pt1.z);
  BOOST_CHECK_EQUAL(76, box_constructor_center_size_integer_odd_size->pt2.x);
  BOOST_CHECK_EQUAL(81, box_constructor_center_size_integer_odd_size->pt2.y);
  BOOST_CHECK_EQUAL(86, box_constructor_center_size_integer_odd_size->pt2.z);

  BOOST_CHECK_CLOSE(0.51, box_constructor_center_size_double->pt1.x, 0.01);
  BOOST_CHECK_CLOSE(-76.365, box_constructor_center_size_double->pt1.y, 0.01);
  BOOST_CHECK_CLOSE(-11.49, box_constructor_center_size_double->pt1.z, 0.01);
  BOOST_CHECK_CLOSE(100.83, box_constructor_center_size_double->pt2.x, 0.01);
  BOOST_CHECK_CLOSE(177.885, box_constructor_center_size_double->pt2.y, 0.01);
  BOOST_CHECK_CLOSE(113.01, box_constructor_center_size_double->pt2.z, 0.01);

  BOOST_CHECK_CLOSE(0.5f, box_constructor_center_size_float->pt1.x, 0.01);
  BOOST_CHECK_CLOSE(-92.665f, box_constructor_center_size_float->pt1.y, 0.01);
  BOOST_CHECK_CLOSE(-51.44f, box_constructor_center_size_float->pt1.z, 0.01);
  BOOST_CHECK_CLOSE(100.84f, box_constructor_center_size_float->pt2.x, 0.01);
  BOOST_CHECK_CLOSE(161.565f, box_constructor_center_size_float->pt2.y, 0.01);
  BOOST_CHECK_CLOSE(71.9f, box_constructor_center_size_float->pt2.z, 0.01);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, BoundingBoxTest)
{
  BoundingBoxD box(Point3D(50., 20., 30.), Point3D(100., 100., 100.));
  BoundingBoxD box2(std::move(box));
  BOOST_CHECK_EQUAL(50., box2.pt1.x);
  BOOST_CHECK_EQUAL(20., box2.pt1.y);
  BOOST_CHECK_EQUAL(30., box2.pt1.z);
  BOOST_CHECK_EQUAL(100., box2.pt2.x);
  BOOST_CHECK_EQUAL(100., box2.pt2.y);
  BOOST_CHECK_EQUAL(100., box2.pt2.z);
  BOOST_CHECK_EQUAL(0., box.pt1.x);
  BOOST_CHECK_EQUAL(0., box.pt1.y);
  BOOST_CHECK_EQUAL(0., box.pt1.z);
  BOOST_CHECK_EQUAL(0., box.pt2.x);
  BOOST_CHECK_EQUAL(0., box.pt2.y);
  BOOST_CHECK_EQUAL(0., box.pt2.z);
}

BOOST_FIXTURE_TEST_CASE(copy_assing_operator, BoundingBoxTest)
{
  BoundingBoxD box(Point3D(50., 20., 30.), Point3D(100., 100., 100.));
  BoundingBoxD box2 = box;
  BOOST_CHECK_EQUAL(50., box2.pt1.x);
  BOOST_CHECK_EQUAL(20., box2.pt1.y);
  BOOST_CHECK_EQUAL(30., box2.pt1.z);
  BOOST_CHECK_EQUAL(100., box2.pt2.x);
  BOOST_CHECK_EQUAL(100., box2.pt2.y);
  BOOST_CHECK_EQUAL(100., box2.pt2.z);
}

BOOST_FIXTURE_TEST_CASE(move_assing_operator, BoundingBoxTest)
{
  BoundingBoxD box(Point3D(50., 20., 30.), Point3D(100., 100., 100.));
  BoundingBoxD box2 = std::move(box);
  BOOST_CHECK_EQUAL(50., box2.pt1.x);
  BOOST_CHECK_EQUAL(20., box2.pt1.y);
  BOOST_CHECK_EQUAL(30., box2.pt1.z);
  BOOST_CHECK_EQUAL(100., box2.pt2.x);
  BOOST_CHECK_EQUAL(100., box2.pt2.y);
  BOOST_CHECK_EQUAL(100., box2.pt2.z);
  BOOST_CHECK_EQUAL(0., box.pt1.x);
  BOOST_CHECK_EQUAL(0., box.pt1.y);
  BOOST_CHECK_EQUAL(0., box.pt1.z);
  BOOST_CHECK_EQUAL(0., box.pt2.x);
  BOOST_CHECK_EQUAL(0., box.pt2.y);
  BOOST_CHECK_EQUAL(0., box.pt2.z);
}

BOOST_FIXTURE_TEST_CASE(center, BoundingBoxTest) 
{
  BOOST_CHECK(Point3I(0, 0, 0) == box_default_constructor_integer.center());
  BOOST_CHECK(Point3D(0., 0., 0.) == box_default_constructor_double.center());
  BOOST_CHECK(Point3F(0.f, 0.f, 0.f) == box_default_constructor_float.center());

  BOOST_CHECK(Point3I(50, 50, 50) == box_integer->center());
  BOOST_CHECK(Point3D(50., 50., 50.) == box_double->center());
  BOOST_CHECK(Point3F(50.f, 50.f, 50.f) == box_float->center());

  BOOST_CHECK(Point3I(50, 50, 50) == box_integer_copy->center());
  BOOST_CHECK(Point3D(50., 50., 50.) == box_double_copy->center());
  BOOST_CHECK(Point3F(50.f, 50.f, 50.f) == box_float_copy->center());

  BOOST_CHECK(ptc_integer == box_constructor_center_side_integer_even_size->center());
  BOOST_CHECK(ptc_integer == box_constructor_center_side_integer_odd_size->center());
  BOOST_CHECK_CLOSE(ptc_double.x, box_constructor_center_side_double->center().x, 0.01);
  BOOST_CHECK_CLOSE(ptc_double.y, box_constructor_center_side_double->center().y, 0.01);
  BOOST_CHECK_CLOSE(ptc_double.z, box_constructor_center_side_double->center().z, 0.01);
  BOOST_CHECK_CLOSE(ptc_float.x, box_constructor_center_side_float->center().x, 0.01);
  BOOST_CHECK_CLOSE(ptc_float.y, box_constructor_center_side_float->center().y, 0.01);
  BOOST_CHECK_CLOSE(ptc_float.z, box_constructor_center_side_float->center().z, 0.01);
  BOOST_CHECK(ptc_integer == box_constructor_center_size_integer_even_size->center());
  BOOST_CHECK(ptc_integer == box_constructor_center_size_integer_odd_size->center()); 
  BOOST_CHECK_CLOSE(ptc_double.x, box_constructor_center_size_double->center().x, 0.01);
  BOOST_CHECK_CLOSE(ptc_double.y, box_constructor_center_size_double->center().y, 0.01);
  BOOST_CHECK_CLOSE(ptc_double.z, box_constructor_center_size_double->center().z, 0.01);
  BOOST_CHECK_CLOSE(ptc_float.x, box_constructor_center_size_float->center().x, 0.01);
  BOOST_CHECK_CLOSE(ptc_float.y, box_constructor_center_size_float->center().y, 0.01);
  BOOST_CHECK_CLOSE(ptc_float.z, box_constructor_center_size_float->center().z, 0.01);
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

BOOST_FIXTURE_TEST_CASE(type, BoundingBoxTest) 
{
  BOOST_CHECK(box_default_constructor_integer.type() == Entity::Type::bounding_box);
  BOOST_CHECK(box_integer->type() == Entity::Type::bounding_box);
  BOOST_CHECK(box_integer_copy->type() == Entity::Type::bounding_box);
  BOOST_CHECK(box_constructor_center_side_integer_even_size->type() == Entity::Type::bounding_box);
  BOOST_CHECK(box_constructor_center_size_integer_even_size->type() == Entity::Type::bounding_box);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, BoundingBoxTest) 
{
  BOOST_CHECK(box_default_constructor_integer.isEmpty());
  BOOST_CHECK(box_integer->isEmpty() == false);
  BOOST_CHECK(box_integer_copy->isEmpty() == false);
  BOOST_CHECK(box_constructor_center_side_integer_even_size->isEmpty() == false);
  BOOST_CHECK(box_constructor_center_size_integer_even_size->isEmpty() == false);
}

BOOST_FIXTURE_TEST_CASE(is3D, BoundingBoxTest) 
{
  BOOST_CHECK(box_default_constructor_integer.is3D());
  BOOST_CHECK(box_integer->is3D());
  BOOST_CHECK(box_integer_copy->is3D());
  BOOST_CHECK(box_constructor_center_side_integer_even_size->is3D());
  BOOST_CHECK(box_constructor_center_size_integer_even_size->is3D());
}

BOOST_FIXTURE_TEST_CASE(containsPoint, BoundingBoxTest) 
{
  Point3I point1(0, 0, 0);
  Point3I point2(50, 50, 50);
  Point3I point3(-50, -50, -50);
  BOOST_CHECK_EQUAL(false, box_default_constructor_integer.containsPoint(point1));
  BOOST_CHECK_EQUAL(true, box_integer->containsPoint(point1));
  BOOST_CHECK_EQUAL(true, box_integer->containsPoint(point2));
  BOOST_CHECK_EQUAL(false, box_integer->containsPoint(point3));

}

BOOST_FIXTURE_TEST_CASE(containsBox, BoundingBoxTest)
{
  BoundingBoxI box(Point3I(0, 0, 0), Point3I(100, 100, 100));
  BOOST_CHECK_EQUAL(false, box_default_constructor_integer.containsBox(BoundingBoxI(Point3I(50, 50, 50), 50)));
  BOOST_CHECK_EQUAL(true, box_integer->containsBox(BoundingBoxI(Point3I(50, 50, 50), 50)));
  BOOST_CHECK_EQUAL(false, box_integer->containsBox(BoundingBoxI(Point3I(200, 50, 50), 50)));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(Box_join)
{
  BoundingBoxI box1(Point3I(0, 0, 0), Point3I(50, 50, 50));
  BoundingBoxI box2(Point3I(20, 30, 10), Point3I(100, 100, 100));
  BoundingBoxI box3 = joinBoundingBoxes(box1, box2);
  BOOST_CHECK_EQUAL(0, box3.pt1.x);
  BOOST_CHECK_EQUAL(0, box3.pt1.y);
  BOOST_CHECK_EQUAL(0, box3.pt1.z);
  BOOST_CHECK_EQUAL(100, box3.pt2.x);
  BOOST_CHECK_EQUAL(100, box3.pt2.y);
  BOOST_CHECK_EQUAL(100, box3.pt2.z);
}

BOOST_AUTO_TEST_CASE(Box_intersect)
{
  BoundingBoxI box1(Point3I(0, 0, 0), Point3I(50, 50, 50));
  BoundingBoxI box2(Point3I(20, 30, 10), Point3I(100, 100, 100));
  BoundingBoxI box3 = intersectBoundingBoxes(box1, box2);
  BOOST_CHECK_EQUAL(20, box3.pt1.x);
  BOOST_CHECK_EQUAL(30, box3.pt1.y);
  BOOST_CHECK_EQUAL(10, box3.pt1.z);
  BOOST_CHECK_EQUAL(50, box3.pt2.x);
  BOOST_CHECK_EQUAL(50, box3.pt2.y);
  BOOST_CHECK_EQUAL(50, box3.pt2.z);
}

BOOST_AUTO_TEST_CASE(Box_cast)
{
  const BoundingBoxD box(Point3D(0.5, 0.4, 1.9), Point3D(100.6, 100.4, 105.49));
  BoundingBoxI box2 = static_cast<BoundingBoxI>(box);
  BOOST_CHECK_EQUAL(1, box2.pt1.x);
  BOOST_CHECK_EQUAL(0, box2.pt1.y);
  BOOST_CHECK_EQUAL(2, box2.pt1.z);
  BOOST_CHECK_EQUAL(101, box2.pt2.x);
  BOOST_CHECK_EQUAL(100, box2.pt2.y);
  BOOST_CHECK_EQUAL(105, box2.pt2.z);
}