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
 
#define BOOST_TEST_MODULE Tidop Rect test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/rect.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(RectTestSuite)

struct RectTest
{

  RectTest()
    : rect_integer(nullptr),
      rect_double(nullptr),
      rect_float(nullptr),
      rect_integer_copy(nullptr),
      rect_double_copy(nullptr),
      rect_float_copy(nullptr),
      rect_constructor_integer_top_left_botton_right(nullptr),
      rect_constructor_double_top_left_botton_right(nullptr),
      rect_constructor_float_top_left_botton_right(nullptr),
      rect_constructor_integer_top_left_size(nullptr),
      rect_constructor_double_top_left_size(nullptr),
      rect_constructor_float_top_left_size(nullptr),
      rect_constructor_integer_top_left_width_height(nullptr),
      rect_constructor_double_top_left_width_height(nullptr),
      rect_constructor_float_top_left_width_height(nullptr)
  {

  }

  ~RectTest()
  {
    delete rect_integer;
    delete rect_double;
    delete rect_float;
    delete rect_integer_copy;
    delete rect_double_copy;
    delete rect_float_copy;
    delete rect_constructor_integer_top_left_botton_right;
    delete rect_constructor_double_top_left_botton_right;
    delete rect_constructor_float_top_left_botton_right;
    delete rect_constructor_integer_top_left_size;
    delete rect_constructor_double_top_left_size;
    delete rect_constructor_float_top_left_size;
    delete rect_constructor_integer_top_left_width_height;
    delete rect_constructor_double_top_left_width_height;
    delete rect_constructor_float_top_left_width_height;
  }

  void setup()
  {
    rect_integer = new Recti(5, 10, 100, 100);
    rect_double = new Rectd(5.5, 10.6, 100.4, 100.2);
    rect_float = new Rectf(5.5f, 10.6f, 100.4f, 100.2f);

    rect_integer_copy = new Recti(*rect_integer);
    rect_double_copy = new Rectd(*rect_double);
    rect_float_copy = new Rectf(*rect_float);

    topLeft_integer = Point<int>(5, 10);
    bottomRight_integer = Point<int>(105, 110);
    topLeft_double = Point<double>(5.5, 10.6);
    bottomRight_double = Point<double>(105.9, 110.8);
    topLeft_float = Point<float>(5.5f, 10.6f);
    bottomRight_float = Point<float>(105.9f, 110.8f);

    rect_constructor_integer_top_left_botton_right = new Recti(topLeft_integer, bottomRight_integer);
    rect_constructor_double_top_left_botton_right = new Rectd(topLeft_double, bottomRight_double);
    rect_constructor_float_top_left_botton_right = new Rectf(topLeft_float, bottomRight_float);

    rect_constructor_integer_top_left_size = new Recti(topLeft_integer, Size<int>(100, 100));
    rect_constructor_double_top_left_size = new Rectd(topLeft_double, Size<double>(100.4, 100.2));
    rect_constructor_float_top_left_size = new Rectf(topLeft_float, Size<float>(100.4f, 100.2f));

    rect_constructor_integer_top_left_width_height = new Recti(topLeft_integer, 100, 100);
    rect_constructor_double_top_left_width_height = new Rectd(topLeft_double, 100.4, 100.2);
    rect_constructor_float_top_left_width_height = new Rectf(topLeft_float, 100.4f, 100.2f);

  }
 
  void teardown()
  {

  }

  Recti rect_default_constructor_integer;
  Rectd rect_default_constructor_double;
  Rectf rect_default_constructor_float;

  Recti *rect_integer;
  Rectd *rect_double;
  Rectf *rect_float;
  Recti *rect_integer_copy;
  Rectd *rect_double_copy;
  Rectf *rect_float_copy;

  Point<int> topLeft_integer;
  Point<int> bottomRight_integer;
  Point<double> topLeft_double;
  Point<double> bottomRight_double;
  Point<float> topLeft_float;
  Point<float> bottomRight_float;
  Recti *rect_constructor_integer_top_left_botton_right;
  Rectd *rect_constructor_double_top_left_botton_right;
  Rectf *rect_constructor_float_top_left_botton_right;
  Recti *rect_constructor_integer_top_left_size;
  Rectd *rect_constructor_double_top_left_size;
  Rectf *rect_constructor_float_top_left_size;
  Recti *rect_constructor_integer_top_left_width_height;
  Rectd *rect_constructor_double_top_left_width_height;
  Rectf *rect_constructor_float_top_left_width_height;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.y);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.width);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.height);

  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.x);
  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.y);
  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.width);
  BOOST_CHECK_EQUAL(0., rect_default_constructor_double.height);
    
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.x);
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.y);
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.width);
  BOOST_CHECK_EQUAL(0.f, rect_default_constructor_float.height);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, RectTest) 
{
  BOOST_CHECK_EQUAL(rect_integer->x, rect_integer_copy->x);
  BOOST_CHECK_EQUAL(rect_integer->y, rect_integer_copy->y);
  BOOST_CHECK_EQUAL(rect_integer->width, rect_integer_copy->width);
  BOOST_CHECK_EQUAL(rect_integer->height, rect_integer_copy->height);
                    
  BOOST_CHECK_EQUAL(rect_double->x, rect_double_copy->x);
  BOOST_CHECK_EQUAL(rect_double->y, rect_double_copy->y);
  BOOST_CHECK_EQUAL(rect_double->width, rect_double_copy->width);
  BOOST_CHECK_EQUAL(rect_double->height, rect_double_copy->height);
                    
  BOOST_CHECK_EQUAL(rect_float->x, rect_float_copy->x);
  BOOST_CHECK_EQUAL(rect_float->y, rect_float_copy->y);
  BOOST_CHECK_EQUAL(rect_float->width, rect_float_copy->width);
  BOOST_CHECK_EQUAL(rect_float->height, rect_float_copy->height);
}

BOOST_FIXTURE_TEST_CASE(constructor_top_left_botton_right, RectTest) 
{
  BOOST_CHECK_EQUAL(topLeft_integer.x, rect_constructor_integer_top_left_botton_right->x);
  BOOST_CHECK_EQUAL(topLeft_integer.y, rect_constructor_integer_top_left_botton_right->y);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_botton_right->width);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_botton_right->height);
                    
  BOOST_CHECK_EQUAL(topLeft_double.x, rect_constructor_double_top_left_botton_right->x);
  BOOST_CHECK_EQUAL(topLeft_double.y, rect_constructor_double_top_left_botton_right->y);
  BOOST_CHECK_CLOSE(100.4, rect_constructor_double_top_left_botton_right->width, 0.01);
  BOOST_CHECK_CLOSE(100.2, rect_constructor_double_top_left_botton_right->height, 0.01);
                    
  BOOST_CHECK_EQUAL(topLeft_float.x, rect_constructor_float_top_left_botton_right->x);
  BOOST_CHECK_EQUAL(topLeft_float.y, rect_constructor_float_top_left_botton_right->y);
  BOOST_CHECK_CLOSE(100.4f, rect_constructor_float_top_left_botton_right->width, 0.01);
  BOOST_CHECK_CLOSE(100.2f, rect_constructor_float_top_left_botton_right->height, 0.01);
}

BOOST_FIXTURE_TEST_CASE(constructor_top_left_size, RectTest) 
{
  BOOST_CHECK_EQUAL(topLeft_integer.x, rect_constructor_integer_top_left_size->x);
  BOOST_CHECK_EQUAL(topLeft_integer.y, rect_constructor_integer_top_left_size->y);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_size->width);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_size->height);
                    
  BOOST_CHECK_EQUAL(topLeft_double.x, rect_constructor_double_top_left_size->x);
  BOOST_CHECK_EQUAL(topLeft_double.y, rect_constructor_double_top_left_size->y);
  BOOST_CHECK_EQUAL(100.4, rect_constructor_double_top_left_size->width);
  BOOST_CHECK_EQUAL(100.2, rect_constructor_double_top_left_size->height);
                    
  BOOST_CHECK_EQUAL(topLeft_float.x, rect_constructor_float_top_left_size->x);
  BOOST_CHECK_EQUAL(topLeft_float.y, rect_constructor_float_top_left_size->y);
  BOOST_CHECK_EQUAL(100.4f, rect_constructor_float_top_left_size->width);
  BOOST_CHECK_EQUAL(100.2f, rect_constructor_float_top_left_size->height);
}

BOOST_FIXTURE_TEST_CASE(constructor_top_left_width_height, RectTest) 
{
  BOOST_CHECK_EQUAL(topLeft_integer.x, rect_constructor_integer_top_left_width_height->x);
  BOOST_CHECK_EQUAL(topLeft_integer.y, rect_constructor_integer_top_left_width_height->y);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_width_height->width);
  BOOST_CHECK_EQUAL(100, rect_constructor_integer_top_left_width_height->height);
                    
  BOOST_CHECK_EQUAL(topLeft_double.x, rect_constructor_double_top_left_width_height->x);
  BOOST_CHECK_EQUAL(topLeft_double.y, rect_constructor_double_top_left_width_height->y);
  BOOST_CHECK_EQUAL(100.4, rect_constructor_double_top_left_width_height->width);
  BOOST_CHECK_EQUAL(100.2, rect_constructor_double_top_left_width_height->height);
                    
  BOOST_CHECK_EQUAL(topLeft_float.x, rect_constructor_float_top_left_width_height->x);
  BOOST_CHECK_EQUAL(topLeft_float.y, rect_constructor_float_top_left_width_height->y);
  BOOST_CHECK_EQUAL(100.4f, rect_constructor_float_top_left_width_height->width);
  BOOST_CHECK_EQUAL(100.2f, rect_constructor_float_top_left_width_height->height);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, RectTest) 
{
  Recti rect(5, 10, 100, 100);
  Recti rect2(std::move(rect));
  BOOST_CHECK_EQUAL(5, rect2.x);
  BOOST_CHECK_EQUAL(10, rect2.y);
  BOOST_CHECK_EQUAL(100, rect2.width);
  BOOST_CHECK_EQUAL(100, rect2.height);
}

BOOST_FIXTURE_TEST_CASE(copy_assignment, RectTest)
{
  Recti rect(5, 10, 100, 100);
  Recti rect2;
  rect2 = rect;
  BOOST_CHECK_EQUAL(5, rect2.x);
  BOOST_CHECK_EQUAL(10, rect2.y);
  BOOST_CHECK_EQUAL(100, rect2.width);
  BOOST_CHECK_EQUAL(100, rect2.height);
}

BOOST_FIXTURE_TEST_CASE(move_assignment, RectTest)
{
  Recti rect2;
  rect2 = Recti(5, 10, 100, 100);
  BOOST_CHECK_EQUAL(5, rect2.x);
  BOOST_CHECK_EQUAL(10, rect2.y);
  BOOST_CHECK_EQUAL(100, rect2.width);
  BOOST_CHECK_EQUAL(100, rect2.height);
}

BOOST_FIXTURE_TEST_CASE(topLeft, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topLeft().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topLeft().y);

  BOOST_CHECK_EQUAL(5, rect_integer->topLeft().x);
  BOOST_CHECK_EQUAL(10, rect_integer->topLeft().y);
                    
  BOOST_CHECK_CLOSE(5.5, rect_double->topLeft().x, 0.01);
  BOOST_CHECK_CLOSE(10.6, rect_double->topLeft().y, 0.01);
    
  BOOST_CHECK_CLOSE(5.5f, rect_float->topLeft().x, 0.01);
  BOOST_CHECK_CLOSE(10.6f, rect_float->topLeft().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(topRight, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topRight().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.topRight().y);

  BOOST_CHECK_EQUAL(105, rect_integer->topRight().x);
  BOOST_CHECK_EQUAL(10, rect_integer->topRight().y);
                    
  BOOST_CHECK_CLOSE(105.9, rect_double->topRight().x, 0.01);
  BOOST_CHECK_CLOSE(10.6, rect_double->topRight().y, 0.01);
    
  BOOST_CHECK_CLOSE(105.9f, rect_float->topRight().x, 0.01);
  BOOST_CHECK_CLOSE(10.6f, rect_float->topRight().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(bottomRight, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomRight().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomRight().y);

  BOOST_CHECK_EQUAL(105, rect_integer->bottomRight().x);
  BOOST_CHECK_EQUAL(110, rect_integer->bottomRight().y);
                    
  BOOST_CHECK_CLOSE(105.9, rect_double->bottomRight().x, 0.01);
  BOOST_CHECK_CLOSE(110.8, rect_double->bottomRight().y, 0.01);
    
  BOOST_CHECK_CLOSE(105.9f, rect_float->bottomRight().x, 0.01);
  BOOST_CHECK_CLOSE(110.8f, rect_float->bottomRight().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(bottomLeft, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomLeft().x);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.bottomLeft().y);

  BOOST_CHECK_EQUAL(5, rect_integer->bottomLeft().x);
  BOOST_CHECK_EQUAL(110, rect_integer->bottomLeft().y);
                    
  BOOST_CHECK_CLOSE(5.5, rect_double->bottomLeft().x, 0.01);
  BOOST_CHECK_CLOSE(110.8, rect_double->bottomLeft().y, 0.01);
    
  BOOST_CHECK_CLOSE(5.5f, rect_float->bottomLeft().x, 0.01);
  BOOST_CHECK_CLOSE(110.8f, rect_float->bottomLeft().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(size, RectTest) 
{
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.size().width);
  BOOST_CHECK_EQUAL(0, rect_default_constructor_integer.size().height);

  BOOST_CHECK_EQUAL(100, rect_integer->size().width);
  BOOST_CHECK_EQUAL(100, rect_integer->size().height);
                    
  BOOST_CHECK_EQUAL(100.4, rect_double->size().width);
  BOOST_CHECK_EQUAL(100.2, rect_double->size().height);
    
  BOOST_CHECK_EQUAL(100.4f, rect_float->size().width);
  BOOST_CHECK_EQUAL(100.2f, rect_float->size().height);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, RectTest)
{
  BOOST_CHECK_EQUAL(true, rect_default_constructor_integer.isEmpty());
  BOOST_CHECK_EQUAL(true, rect_default_constructor_double.isEmpty());
  BOOST_CHECK_EQUAL(true, rect_default_constructor_float.isEmpty());
  BOOST_CHECK_EQUAL(false, rect_integer->isEmpty());
  BOOST_CHECK_EQUAL(false, rect_double->isEmpty());
  BOOST_CHECK_EQUAL(false, rect_double->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(isValid, RectTest)
{
  BOOST_CHECK_EQUAL(false, rect_default_constructor_integer.isValid());
  BOOST_CHECK_EQUAL(false, rect_default_constructor_double.isValid());
  BOOST_CHECK_EQUAL(false, rect_default_constructor_float.isValid());
  BOOST_CHECK_EQUAL(true, rect_integer->isValid());
  BOOST_CHECK_EQUAL(true, rect_double->isValid());
  BOOST_CHECK_EQUAL(true, rect_double->isValid());
  Rect<int> rect_invalid(10, 20, -30, 10);
  BOOST_CHECK_EQUAL(false, rect_invalid.isValid());
}

BOOST_FIXTURE_TEST_CASE(normalized, RectTest)
{
  Rect<int> rect_normalized(10, 20, -30, -10);
  rect_normalized.normalized();
  BOOST_CHECK_EQUAL(-20, rect_normalized.x);
  BOOST_CHECK_EQUAL(10, rect_normalized.y);
  BOOST_CHECK_EQUAL(30, rect_normalized.width);
  BOOST_CHECK_EQUAL(10, rect_normalized.height);
}

BOOST_FIXTURE_TEST_CASE(contains, RectTest)
{
  BOOST_CHECK_EQUAL(false, rect_default_constructor_integer.contains(Point<int>(50, 50)));
  BOOST_CHECK_EQUAL(false, rect_default_constructor_integer.contains(Point<int>(0, 0)));
  BOOST_CHECK(rect_integer->contains(Point<int>(50, 50)));
  BOOST_CHECK_EQUAL(false, rect_integer->contains(Point<int>(0, 0)));
}

BOOST_FIXTURE_TEST_CASE(cast, RectTest)
{
  Recti recti = static_cast<Recti>(*rect_double);
  BOOST_CHECK_EQUAL(6, recti.x);
  BOOST_CHECK_EQUAL(11, recti.y);
  BOOST_CHECK_EQUAL(100, recti.width);
  BOOST_CHECK_EQUAL(100, recti.height);

  Rectd rectd = static_cast<Rectd>(*rect_integer);
  BOOST_CHECK_EQUAL(5., rectd.x);
  BOOST_CHECK_EQUAL(10., rectd.y);
  BOOST_CHECK_EQUAL(100., rectd.width);
  BOOST_CHECK_EQUAL(100., rectd.height);
}

BOOST_FIXTURE_TEST_CASE(is_equal, RectTest)
{
  Recti rect(5, 10, 100, 100);
  BOOST_CHECK(rect == *rect_integer);
}

BOOST_FIXTURE_TEST_CASE(is_not_equal, RectTest)
{
  Recti rect(8, 2, 45, 78);
  BOOST_CHECK(rect != *rect_integer);
}

BOOST_FIXTURE_TEST_CASE(intersection, RectTest)
{
  Recti rect1(50, 50, 100, 100);
  Recti rect2(75, 75, 150, 150);
  Recti rect3 = intersect(rect1, rect2);
  BOOST_CHECK_EQUAL(75, rect3.x);
  BOOST_CHECK_EQUAL(75, rect3.y);
  BOOST_CHECK_EQUAL(75, rect3.width);
  BOOST_CHECK_EQUAL(75, rect3.height);
}

BOOST_AUTO_TEST_SUITE_END()
