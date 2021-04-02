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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop Point test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/point.h>
#include <tidop/geometry/entities/multipoint.h>
#include <tidop/geometry/entities/window.h>
#include <tidop/geometry/entities/bbox.h>
#include <tidop/core/utils.h>
#include <tidop/math/algebra/vector.h>

using namespace tl;

BOOST_AUTO_TEST_SUITE(PointTestSuite)

struct PointTest
{

  PointTest()
    : point_integer(nullptr),
      point_double(nullptr),
      point_float(nullptr),
      point_integer_copy(nullptr),
      point_double_copy(nullptr),
      point_float_copy(nullptr),
      point_array_constructor_integer(nullptr)
  {

  }

  ~PointTest()
  {
    delete point_integer;
    delete point_double;
    delete point_float;
    delete point_integer_copy;
    delete point_double_copy;
    delete point_float_copy;
    delete point_array_constructor_integer;
  }

  void setup()
  {
    point_integer = new Point<int>(23, 67);
    point_double = new Point<double>(253.56, 562.94);
    point_float = new Point<float>(564.26f, 646.65f);

    point_integer_copy = new Point<int>(*point_integer);
    point_double_copy = new Point<double>(*point_double);
    point_float_copy = new Point<float>(*point_float);

    math::Vector2i vector_int{{23, 67}};
    math::Vector2d vector_double{{253.56, 562.94}};
    math::Vector2f vector_float{{564.26f, 646.65f}};

    std::array<int, 2> pt_int{ { 23, 67 } };

    point_array_constructor_integer = new Point<int>(pt_int);
  }
 
  void teardown()
  {

  }

  Point<int> point_default_constructor_integer;
  Point<double> point_default_constructor_double;
  Point<float> point_default_constructor_float;

  Point<int> *point_integer;
  Point<double> *point_double;
  Point<float> *point_float;
  Point<int> *point_integer_copy;
  Point<double> *point_double_copy;
  Point<float> *point_float_copy;

  Point<int> *point_array_constructor_integer;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, PointTest) 
{
  BOOST_CHECK_EQUAL(0, point_default_constructor_integer.x);
  BOOST_CHECK_EQUAL(0, point_default_constructor_integer.y);

  BOOST_CHECK_EQUAL(0., point_default_constructor_double.x);
  BOOST_CHECK_EQUAL(0., point_default_constructor_double.y);
    
  BOOST_CHECK_EQUAL(0.f, point_default_constructor_float.x);
  BOOST_CHECK_EQUAL(0.f, point_default_constructor_float.y);
}

BOOST_FIXTURE_TEST_CASE(xy_constructor, PointTest) 
{
  BOOST_CHECK_EQUAL(23, point_integer->x);
  BOOST_CHECK_EQUAL(67, point_integer->y);
    
  BOOST_CHECK_CLOSE(253.56, point_double->x, 0.01);
  BOOST_CHECK_CLOSE(562.94, point_double->y, 0.01);
    
  BOOST_CHECK_CLOSE(564.26f, point_float->x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, point_float->y, 0.05);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, PointTest) 
{
  BOOST_CHECK_EQUAL(23, point_integer_copy->x);
  BOOST_CHECK_EQUAL(67, point_integer_copy->y);
    
  BOOST_CHECK_CLOSE(253.56, point_double_copy->x, 0.01);
  BOOST_CHECK_CLOSE(562.94, point_double_copy->y, 0.01);
    
  BOOST_CHECK_CLOSE(564.26f, point_float_copy->x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, point_float_copy->y, 0.05);
}

BOOST_FIXTURE_TEST_CASE(array_constructor, PointTest) 
{
  BOOST_CHECK_EQUAL(23, point_array_constructor_integer->x);
  BOOST_CHECK_EQUAL(67, point_array_constructor_integer->y);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, PointTest) 
{
  Point<int> pt(23, 67);
  Point<int> pt2(std::move(pt));
  BOOST_CHECK_EQUAL(23, pt2.x);
  BOOST_CHECK_EQUAL(67, pt2.y);
  BOOST_CHECK_EQUAL(0, pt.x);
  BOOST_CHECK_EQUAL(0, pt.y);
}

BOOST_FIXTURE_TEST_CASE(type, PointTest) 
{
  BOOST_CHECK(Entity::Type::point2d == point_default_constructor_integer.type());
  BOOST_CHECK(Entity::Type::point2d == point_integer->type());
  BOOST_CHECK(Entity::Type::point2d == point_double->type());
  BOOST_CHECK(Entity::Type::point2d == point_float->type());
  BOOST_CHECK(Entity::Type::point2d == point_integer_copy->type());
  BOOST_CHECK(Entity::Type::point2d == point_array_constructor_integer->type());
}

BOOST_FIXTURE_TEST_CASE(is3D, PointTest) 
{
  BOOST_CHECK_EQUAL(false, point_default_constructor_integer.is3D());
  BOOST_CHECK_EQUAL(false, point_integer->is3D());
  BOOST_CHECK_EQUAL(false, point_double->is3D());
  BOOST_CHECK_EQUAL(false, point_float->is3D());
  BOOST_CHECK_EQUAL(false, point_integer_copy->is3D());
}

BOOST_FIXTURE_TEST_CASE(assing_operator, PointTest)
{
  PointI pt_int_c = *point_integer;

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);

  PointD pt_double_c = *point_double;

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);

  PointF pt_float_c = *point_float;

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
}

BOOST_FIXTURE_TEST_CASE(assing_move_operator, PointTest) 
{
  Point<int> pt(23, 67);
  Point<int> pt2 = std::move(pt);
  BOOST_CHECK_EQUAL(23, pt2.x);
  BOOST_CHECK_EQUAL(67, pt2.y);
  BOOST_CHECK_EQUAL(0, pt.x);
  BOOST_CHECK_EQUAL(0, pt.y);
}

BOOST_FIXTURE_TEST_CASE(cast, PointTest)
{

  PointD pt_double = static_cast<PointD>(*point_integer);

  BOOST_CHECK_EQUAL(23.0, pt_double.x);
  BOOST_CHECK_EQUAL(67.0, pt_double.y);

  PointI pt_int = static_cast<PointD>(*point_double);

  BOOST_CHECK_EQUAL(254, pt_int.x);
  BOOST_CHECK_EQUAL(563, pt_int.y);

  Point3F pt_float3D(564.26f, 646.65f, 23.32f);
  PointF pt_float = static_cast<PointF>(pt_float3D);
  BOOST_CHECK_CLOSE(564.26f, pt_float.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float.y, 0.05);
}

BOOST_AUTO_TEST_SUITE_END()




/* Punto 3D */

BOOST_AUTO_TEST_SUITE(Point3TestSuite)

struct Point3Test
{

  Point3Test()
    : point_integer(nullptr),
      point_double(nullptr),
      point_float(nullptr),
      point_integer_copy(nullptr),
      point_double_copy(nullptr),
      point_float_copy(nullptr),
      point_array_constructor_integer(nullptr)
  {

  }

  ~Point3Test()
  {
    delete point_integer;
    delete point_double;
    delete point_float;
    delete point_integer_copy;
    delete point_double_copy;
    delete point_float_copy;
    delete point_array_constructor_integer;
  }

  void setup()
  {
    point_integer = new Point3<int>(23, 67, 54);
    point_double = new Point3<double>(253.56, 562.94, 345.89);
    point_float = new Point3<float>(564.26f, 646.65f, 45.89f);

    point_integer_copy = new Point3<int>(*point_integer);
    point_double_copy = new Point3<double>(*point_double);
    point_float_copy = new Point3<float>(*point_float);

    math::Vector3i vector_int{{23, 67, 23}};
    math::Vector3d vector_double{{253.56, 562.94, 234.67}};
    math::Vector3f vector_float{{564.26f, 646.65f, 56.21f}};

    std::array<int, 3> pt_int{ { 23, 67, 23 } };

    point_array_constructor_integer = new Point3<int>(pt_int);
  }
 
  void teardown()
  {

  }

  Point3<int> point_default_constructor_integer;
  Point3<double> point_default_constructor_double;
  Point3<float> point_default_constructor_float;

  Point3<int> *point_integer;
  Point3<double> *point_double;
  Point3<float> *point_float;
  Point3<int> *point_integer_copy;
  Point3<double> *point_double_copy;
  Point3<float> *point_float_copy;

  Point3<int> *point_array_constructor_integer;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, Point3Test) 
{
  BOOST_CHECK_EQUAL(0, point_default_constructor_integer.x);
  BOOST_CHECK_EQUAL(0, point_default_constructor_integer.y);
  BOOST_CHECK_EQUAL(0, point_default_constructor_integer.z);

  BOOST_CHECK_EQUAL(0., point_default_constructor_double.x);
  BOOST_CHECK_EQUAL(0., point_default_constructor_double.y);
  BOOST_CHECK_EQUAL(0., point_default_constructor_double.z);

  BOOST_CHECK_EQUAL(0.f, point_default_constructor_float.x);
  BOOST_CHECK_EQUAL(0.f, point_default_constructor_float.y);
  BOOST_CHECK_EQUAL(0.f, point_default_constructor_float.z);
}

BOOST_FIXTURE_TEST_CASE(xy_constructor, Point3Test) 
{
  BOOST_CHECK_EQUAL(23, point_integer->x);
  BOOST_CHECK_EQUAL(67, point_integer->y);
  BOOST_CHECK_EQUAL(54, point_integer->z);

  BOOST_CHECK_CLOSE(253.56, point_double->x, 0.01);
  BOOST_CHECK_CLOSE(562.94, point_double->y, 0.01);
  BOOST_CHECK_CLOSE(345.89, point_double->z, 0.01);

  BOOST_CHECK_CLOSE(564.26f, point_float->x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, point_float->y, 0.05);
  BOOST_CHECK_CLOSE(45.89f, point_float->z, 0.05);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, Point3Test) 
{
  BOOST_CHECK_EQUAL(23, point_integer_copy->x);
  BOOST_CHECK_EQUAL(67, point_integer_copy->y);
  BOOST_CHECK_EQUAL(54, point_integer_copy->z);

  BOOST_CHECK_CLOSE(253.56, point_double_copy->x, 0.01);
  BOOST_CHECK_CLOSE(562.94, point_double_copy->y, 0.01);
  BOOST_CHECK_CLOSE(345.89, point_double_copy->z, 0.01);

  BOOST_CHECK_CLOSE(564.26f, point_float_copy->x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, point_float_copy->y, 0.05);
  BOOST_CHECK_CLOSE(45.89f, point_float_copy->z, 0.05);
}

BOOST_FIXTURE_TEST_CASE(array_constructor, Point3Test) 
{
  BOOST_CHECK_EQUAL(23, point_array_constructor_integer->x);
  BOOST_CHECK_EQUAL(67, point_array_constructor_integer->y);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, Point3Test) 
{
  Point3<int> pt(23, 67, 23);
  Point3<int> pt2(std::move(pt));
  BOOST_CHECK_EQUAL(23, pt2.x);
  BOOST_CHECK_EQUAL(67, pt2.y);
  BOOST_CHECK_EQUAL(23, pt2.z);
  BOOST_CHECK_EQUAL(0, pt.x);
  BOOST_CHECK_EQUAL(0, pt.y);
  BOOST_CHECK_EQUAL(0, pt.z);
}

BOOST_FIXTURE_TEST_CASE(type, Point3Test) 
{
  BOOST_CHECK(Entity::Type::point3d == point_default_constructor_integer.type());
  BOOST_CHECK(Entity::Type::point3d == point_integer->type());
  BOOST_CHECK(Entity::Type::point3d == point_double->type());
  BOOST_CHECK(Entity::Type::point3d == point_float->type());
  BOOST_CHECK(Entity::Type::point3d == point_integer_copy->type());
  BOOST_CHECK(Entity::Type::point3d == point_array_constructor_integer->type());
}

BOOST_FIXTURE_TEST_CASE(is3D, Point3Test) 
{
  BOOST_CHECK_EQUAL(true, point_default_constructor_integer.is3D());
  BOOST_CHECK_EQUAL(true, point_integer->is3D());
  BOOST_CHECK_EQUAL(true, point_double->is3D());
  BOOST_CHECK_EQUAL(true, point_float->is3D());
  BOOST_CHECK_EQUAL(true, point_integer_copy->is3D());
}

BOOST_FIXTURE_TEST_CASE(assing_operator, Point3Test)
{
  Point3I pt_int_c = *point_integer;

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);
  BOOST_CHECK_EQUAL(54, pt_int_c.z);

  Point3D pt_double_c = *point_double;

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);
  BOOST_CHECK_CLOSE(345.89, pt_double_c.z, 0.01);

  Point3F pt_float_c = *point_float;

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
  BOOST_CHECK_CLOSE(45.89f, pt_float_c.z, 0.05);
}

BOOST_FIXTURE_TEST_CASE(assing_move_operator, Point3Test) 
{
  Point3<int> pt(23, 67, 23);
  Point3<int> pt2 = std::move(pt);
  BOOST_CHECK_EQUAL(23, pt2.x);
  BOOST_CHECK_EQUAL(67, pt2.y);
  BOOST_CHECK_EQUAL(23, pt2.z);
  BOOST_CHECK_EQUAL(0, pt.x);
  BOOST_CHECK_EQUAL(0, pt.y);
  BOOST_CHECK_EQUAL(0, pt.z);
}

BOOST_FIXTURE_TEST_CASE(cast, Point3Test)
{

  Point3D pt_double = static_cast<Point3D>(*point_integer);

  BOOST_CHECK_EQUAL(23.0, pt_double.x);
  BOOST_CHECK_EQUAL(67.0, pt_double.y);
  BOOST_CHECK_EQUAL(54.0, pt_double.z);

  PointI pt_int = static_cast<PointD>(*point_double);

  BOOST_CHECK_EQUAL(254, pt_int.x);
  BOOST_CHECK_EQUAL(563, pt_int.y);

  PointF pt_float(564.26f, 646.65f);
  Point3F pt_float_3d = static_cast<Point3F>(pt_float);
  BOOST_CHECK_CLOSE(564.26f, pt_float_3d.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_3d.y, 0.05);
  BOOST_CHECK_CLOSE(0.f, pt_float_3d.z, 0.05);
}

BOOST_AUTO_TEST_SUITE_END()


/* Operaciones entre puntos */

BOOST_AUTO_TEST_CASE(Point_append)
{
  /// 2D

  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  pt_2 += pt_1;

  BOOST_CHECK_EQUAL(90, pt_2.x);
  BOOST_CHECK_EQUAL(123, pt_2.y);

  PointD pt_1d(23.45, 67.52);
  PointD pt_2d(67.59, 56.39);

  pt_2d += pt_1d;

  BOOST_CHECK_CLOSE(91.04, pt_2d.x, 0.01);
  BOOST_CHECK_CLOSE(123.91, pt_2d.y, 0.01);

  // Tipos diferentes
  pt_1 += pt_2d;
  BOOST_CHECK_EQUAL(114, pt_1.x);
  BOOST_CHECK_EQUAL(191, pt_1.y);

  /// 3D

  Point3I pt3_1(23, 67, 10);
  Point3I pt3_2(67, 56, 25);

  pt3_2 += pt3_1;

  BOOST_CHECK_EQUAL(90, pt3_2.x);
  BOOST_CHECK_EQUAL(123, pt3_2.y);
  BOOST_CHECK_EQUAL(35, pt3_2.z);

  Point3D pt3_1d(23.45, 67.52, 10.5);
  Point3D pt3_2d(67.59, 56.39, 15.9);

  pt3_2d += pt3_1d;

  BOOST_CHECK_CLOSE(91.04, pt3_2d.x, 0.01);
  BOOST_CHECK_CLOSE(123.91, pt3_2d.y, 0.01);
  BOOST_CHECK_CLOSE(26.4, pt3_2d.z, 0.01);

  // Tipos diferentes
  pt3_1 += pt3_2d;
  BOOST_CHECK_EQUAL(114, pt3_1.x);
  BOOST_CHECK_EQUAL(191, pt3_1.y);
  BOOST_CHECK_EQUAL(36, pt3_1.z);
}

BOOST_AUTO_TEST_CASE(Point_addition)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  PointI pt_3 = pt_1 + pt_2;

  BOOST_CHECK_EQUAL(90, pt_3.x);
  BOOST_CHECK_EQUAL(123, pt_3.y);

  PointD pt_1d(23.45, 67.52);
  PointD pt_2d(67.59, 56.39);

  PointD pt_3d = pt_1d + pt_2d;

  BOOST_CHECK_CLOSE(91.04, pt_3d.x, 0.01);
  BOOST_CHECK_CLOSE(123.91, pt_3d.y, 0.01);

  /// 3D

  Point3I pt3_1(23, 67, 10);
  Point3I pt3_2(67, 56, 25);

  Point3I pt3_3 = pt3_1 + pt3_2;

  BOOST_CHECK_EQUAL(90, pt3_3.x);
  BOOST_CHECK_EQUAL(123, pt3_3.y);
  BOOST_CHECK_EQUAL(35, pt3_3.z);

  Point3D pt3_1d(23.45, 67.52, 10.5);
  Point3D pt3_2d(67.59, 56.39, 15.9);

  Point3D pt3_3d = pt3_1d + pt3_2d;

  BOOST_CHECK_CLOSE(91.04, pt3_3d.x, 0.01);
  BOOST_CHECK_CLOSE(123.91, pt3_3d.y, 0.01);
  BOOST_CHECK_CLOSE(26.4, pt3_3d.z, 0.01);
}

BOOST_AUTO_TEST_CASE(Point_substraction)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  PointI pt_3 = pt_1 - pt_2;

  BOOST_CHECK_EQUAL(-44, pt_3.x);
  BOOST_CHECK_EQUAL(11, pt_3.y);

  PointD pt_1d(256.26, 2526.36);
  PointD pt_2d(894.66, 651.56);

  PointD pt_3d = pt_1d - pt_2d;

  BOOST_CHECK_CLOSE(-638.4, pt_3d.x, 0.01);
  BOOST_CHECK_CLOSE(1874.8, pt_3d.y, 0.01);

  /// 3D

  Point3I pt3_1(23, 67, 10);
  Point3I pt3_2(67, 56, 25);

  Point3I pt3_3 = pt3_1 - pt3_2;

  BOOST_CHECK_EQUAL(-44, pt3_3.x);
  BOOST_CHECK_EQUAL(11, pt3_3.y);
  BOOST_CHECK_EQUAL(-15, pt3_3.z);

  Point3D pt3_1d(256.26, 2526.36, 12.36);
  Point3D pt3_2d(894.66, 651.56, 265.3);

  Point3D pt3_3d = pt3_1d - pt3_2d;

  BOOST_CHECK_CLOSE(-638.4, pt3_3d.x, 0.01);
  BOOST_CHECK_CLOSE(1874.8, pt3_3d.y, 0.01);
  BOOST_CHECK_CLOSE(-252.94, pt3_3d.z, 0.01);

}

BOOST_AUTO_TEST_CASE(Point_substraction2)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  pt_1 -= pt_2;

  BOOST_CHECK_EQUAL(-44, pt_1.x);
  BOOST_CHECK_EQUAL(11, pt_1.y);

  PointD pt_1d(256.26, 2526.36);
  PointD pt_2d(894.66, 651.56);

  pt_1d -= pt_2d;

  BOOST_CHECK_CLOSE(-638.4, pt_1d.x, 0.01);
  BOOST_CHECK_CLOSE(1874.8, pt_1d.y, 0.01);

  // Tipos diferentes
  pt_1 -= pt_1d;
  BOOST_CHECK_EQUAL(594, pt_1.x);
  BOOST_CHECK_EQUAL(-1864, pt_1.y);

  /// 3D

  Point3I pt3_1(23, 67, 10);
  Point3I pt3_2(67, 56, 25);

  pt3_1 -= pt3_2;

  BOOST_CHECK_EQUAL(-44, pt3_1.x);
  BOOST_CHECK_EQUAL(11, pt3_1.y);
  BOOST_CHECK_EQUAL(-15, pt3_1.z);

  Point3D pt3_1d(256.26, 2526.36, 12.36);
  Point3D pt3_2d(894.66, 651.56, 265.3);

  pt3_1d -= pt3_2d;

  BOOST_CHECK_CLOSE(-638.4, pt3_1d.x, 0.01);
  BOOST_CHECK_CLOSE(1874.8, pt3_1d.y, 0.01);
  BOOST_CHECK_CLOSE(-252.94, pt3_1d.z, 0.01);

  // Tipos diferentes
  pt3_1 -= pt3_1d;
  BOOST_CHECK_EQUAL(594, pt3_1.x);
  BOOST_CHECK_EQUAL(-1864, pt3_1.y);
  BOOST_CHECK_EQUAL(238, pt3_1.z);
}

/* Multiplicación de un punto por un escalar */

BOOST_AUTO_TEST_CASE(Point_multiplication)
{
  PointI pt(23, 67);
  int s = 3;

  pt *= s;

  BOOST_CHECK_EQUAL(69, pt.x);
  BOOST_CHECK_EQUAL(201, pt.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  pt_d *= s_d;

  BOOST_CHECK_CLOSE(6022.11, pt_d.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt_d.y, 0.01);

  PointI pt2(23, 67);
  double s2 = 3.3;

  pt2 *= s2;
  BOOST_CHECK_EQUAL(76, pt2.x);
  BOOST_CHECK_EQUAL(221, pt2.y);


  /// 3D

  Point3I pt3(23, 67, 15);

  pt3 *= s;

  BOOST_CHECK_EQUAL(69, pt3.x);
  BOOST_CHECK_EQUAL(201, pt3.y);
  BOOST_CHECK_EQUAL(45, pt3.z);

  Point3D pt3_d(256.26, 2526.36, 236.47);

  pt3_d *= s_d;

  BOOST_CHECK_CLOSE(6022.11, pt3_d.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt3_d.y, 0.01);
  BOOST_CHECK_CLOSE(5557.045, pt3_d.z, 0.01);

  Point3I pt3_2(23, 67, 26);

  pt3_2 *= s2;
  BOOST_CHECK_EQUAL(76, pt3_2.x);
  BOOST_CHECK_EQUAL(221, pt3_2.y);
  BOOST_CHECK_EQUAL(86, pt3_2.z);

}

/* División de un punto por un escalar */

BOOST_AUTO_TEST_CASE(Point_division)
{
  PointI pt(23, 67);
  int s = 3;

  pt /= s;

  BOOST_CHECK_EQUAL(8, pt.x);
  BOOST_CHECK_EQUAL(22, pt.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  pt_d /= s_d;

  BOOST_CHECK_CLOSE(10.905, pt_d.x, 0.01);
  BOOST_CHECK_CLOSE(107.505, pt_d.y, 0.01);

  PointI pt2(23, 67);
  double s2 = 3.3;

  pt2 /= s2;
  BOOST_CHECK_EQUAL(7, pt2.x);
  BOOST_CHECK_EQUAL(20, pt2.y);


  /// 3D

  Point3I pt3(23, 67, 15);

  pt3 /= s;

  BOOST_CHECK_EQUAL(8, pt3.x);
  BOOST_CHECK_EQUAL(22, pt3.y);
  BOOST_CHECK_EQUAL(5, pt3.z);

  Point3D pt3_d(256.26, 2526.36, 236.47);

  pt3_d /= s_d;

  BOOST_CHECK_CLOSE(10.905, pt3_d.x, 0.01);
  BOOST_CHECK_CLOSE(107.505, pt3_d.y, 0.01);
  BOOST_CHECK_CLOSE(10.0625, pt3_d.z, 0.01);

  Point3I pt3_2(23, 67, 26);

  pt3_2 /= s2;
  BOOST_CHECK_EQUAL(7, pt3_2.x);
  BOOST_CHECK_EQUAL(20, pt3_2.y);
  BOOST_CHECK_EQUAL(8, pt3_2.z);
}


/* Igualdad == */
BOOST_AUTO_TEST_CASE(Point_equal)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(256.26, 2526.36);
  PointD pt3(234.323, 234.36);
  BOOST_CHECK(pt1 == pt2);
  BOOST_CHECK(pt1 != pt3);


  Point3D pt1_3d(256.26, 2526.36, 10.6);
  Point3D pt2_3d(256.26, 2526.36, 10.6);
  Point3D pt3_3d(234.323, 234.36, 25.3);
  BOOST_CHECK(pt1_3d == pt2_3d);
  BOOST_CHECK(pt1_3d != pt3_3d);
}

/* Diferencia != */

BOOST_AUTO_TEST_CASE(Point_difference)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(256.26, 2526.36);
  PointD pt3(234.323, 234.36);
  BOOST_CHECK(pt1 == pt2);
  BOOST_CHECK(pt1 != pt3);
}

/* Invertir punto */

BOOST_AUTO_TEST_CASE(Point_invert)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(-256.26, -2526.36);
  BOOST_CHECK(pt2 == -pt1);
}

/* Punto por escalar */

BOOST_AUTO_TEST_CASE(Point_multiplication1)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt2 = pt * s;

  BOOST_CHECK_EQUAL(69, pt2.x);
  BOOST_CHECK_EQUAL(201, pt2.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d2 = pt_d * s_d;

  BOOST_CHECK_CLOSE(6022.11, pt_d2.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt_d2.y, 0.01);

  pt2 = PointI(23, 67) * 3.3;
  BOOST_CHECK_EQUAL(76, pt2.x);
  BOOST_CHECK_EQUAL(221, pt2.y);

  /// 3D

  Point3I pt3_1(23, 67, 15);

  Point3I pt3_2 = pt3_1 * s;

  BOOST_CHECK_EQUAL(69, pt3_2.x);
  BOOST_CHECK_EQUAL(201, pt3_2.y);
  BOOST_CHECK_EQUAL(45, pt3_2.z);

  Point3D pt3_d(256.26, 2526.36, 236.47);

  Point3D pt4_d = pt3_d * s_d;

  BOOST_CHECK_CLOSE(6022.11, pt4_d.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt4_d.y, 0.01);
  BOOST_CHECK_CLOSE(5557.045, pt4_d.z, 0.01);

  Point3D pt5 = Point3I(23, 67, 26) * 3.3;
  BOOST_CHECK_EQUAL(76, pt5.x);
  BOOST_CHECK_EQUAL(221, pt5.y);
  BOOST_CHECK_EQUAL(86, pt5.z);
}

/* Escalar por punto */

BOOST_AUTO_TEST_CASE(Point_multiplication2)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt2 = s * pt;

  BOOST_CHECK_EQUAL(69, pt2.x);
  BOOST_CHECK_EQUAL(201, pt2.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d2 = s_d * pt_d;

  BOOST_CHECK_CLOSE(6022.11, pt_d2.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt_d2.y, 0.01);


  pt2 =  3.3 * PointI(23, 67);
  BOOST_CHECK_EQUAL(76, pt2.x);
  BOOST_CHECK_EQUAL(221, pt2.y);
}

/* Punto entre escalar */

BOOST_AUTO_TEST_CASE(Point_division2)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt_1 = pt / s;

  BOOST_CHECK_EQUAL(8, pt_1.x);
  BOOST_CHECK_EQUAL(22, pt_1.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d1 = pt_d / s_d;

  BOOST_CHECK_CLOSE(10.905, pt_d1.x, 0.01);
  BOOST_CHECK_CLOSE(107.505, pt_d1.y, 0.001);

  PointI pt2(23, 67);
  double s2 = 3.3;

  PointI pt2_1 = pt2 / s2;
  BOOST_CHECK_EQUAL(7, pt2_1.x);
  BOOST_CHECK_EQUAL(20, pt2_1.y);
}

/* MultiPoint */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(MultiPoint_default_constructor) 
{
  
  MultiPointI multiPointI;

  BOOST_CHECK_EQUAL(0, multiPointI.size());
  BOOST_CHECK(multiPointI.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(false, multiPointI.is3D());

  MultiPointD multiPointD;
  WindowD w = multiPointD.window();

  BOOST_CHECK_EQUAL(0, multiPointD.size());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.y);

  MultiPointF multiPointF;
    
  BOOST_CHECK_EQUAL(0, multiPointF.size());

  MultiPoint<uint16_t> multiPoint16;
    
  BOOST_CHECK_EQUAL(0, multiPoint16.size());
  
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(MultiPoint_constructor_reserve)
{
  MultiPointD multiPointD(10);
  WindowD w = multiPointD.window();

  BOOST_CHECK_EQUAL(10, multiPointD.size());
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(0., w.pt1.x);
  BOOST_CHECK_EQUAL(0., w.pt1.y);
  BOOST_CHECK_EQUAL(0., w.pt2.x);
  BOOST_CHECK_EQUAL(0., w.pt2.y);

}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(MultiPoint_copy_constructor) 
{
  MultiPointD multiPointD;
  multiPointD.push_back(PointD(23.6, 94.4));
  multiPointD.push_back(PointD(75.36, 246.33));
  multiPointD.push_back(PointD(256.6, 619.3));
  multiPointD.push_back(PointD(62.36, 6.60));

  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

  MultiPointD multiPointD_c(multiPointD);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (int i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor a partir de un vector de puntos */

BOOST_AUTO_TEST_CASE(MultiPoint_vector) 
{
  std::vector<PointD> multiPointD{ PointD(23.6, 94.4),
                                   PointD(75.36, 246.33),
                                   PointD(256.6, 619.3),
                                   PointD(62.36, 6.60) };

  MultiPointD multiPointD_c(multiPointD);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (int i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor lista de inicializadores */

BOOST_AUTO_TEST_CASE(MultiPoint_initializer_list) 
{
  MultiPointD multiPointD{ PointD(23.6, 94.4),
                           PointD(75.36, 246.33),
                           PointD(256.6, 619.3),
                           PointD(62.36, 6.60) };
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

}


/* Operador de asignación */

BOOST_AUTO_TEST_CASE(MultiPoint_assing_operator)
{
  MultiPointD multiPointD{ PointD(23.6, 94.4),
                           PointD(75.36, 246.33),
                           PointD(256.6, 619.3),
                           PointD(62.36, 6.60) };

  MultiPointD multiPointD_c = multiPointD;

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

BOOST_AUTO_TEST_CASE(MultiPoint_window)
{
  MultiPointD multiPointD {PointD(23.6, 94.4),
                           PointD(75.36, 246.33),
                           PointD(256.6, 619.3),
                           PointD(62.36, 6.60)};

  WindowD window = multiPointD.window();
  BOOST_CHECK_EQUAL(23.6, window.pt1.x);
  BOOST_CHECK_EQUAL(6.60, window.pt1.y);
  BOOST_CHECK_EQUAL(256.6, window.pt2.x);
  BOOST_CHECK_EQUAL(619.3, window.pt2.y);
}

/* MultiPoint3D */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(MultiPoint3D_default_constructor) 
{
  
  MultiPoint3dI multiPointI;

  BOOST_CHECK_EQUAL(0, multiPointI.size());
  BOOST_CHECK(multiPointI.type() == Entity::Type::multipoint3d);
  BOOST_CHECK(multiPointI.is3D());

  MultiPoint3dD multiPointD;
  BoundingBoxD box = multiPointD.boundingBox();

  BOOST_CHECK_EQUAL(0, multiPointD.size());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);

  MultiPoint3dF multiPointF;
    
  BOOST_CHECK_EQUAL(0, multiPointF.size());

  MultiPoint3D<uint16_t> multiPoint16;
    
  BOOST_CHECK_EQUAL(0, multiPoint16.size());
  
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(MultiPoint3D_constructor_reserve)
{
  MultiPoint3dD multiPointD(10);
  BoundingBoxD box = multiPointD.boundingBox();

  BOOST_CHECK_EQUAL(10, multiPointD.size());
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(0., box.pt1.x);
  BOOST_CHECK_EQUAL(0., box.pt1.y);
  BOOST_CHECK_EQUAL(0., box.pt1.z);
  BOOST_CHECK_EQUAL(0., box.pt2.x);
  BOOST_CHECK_EQUAL(0., box.pt2.y);
  BOOST_CHECK_EQUAL(0., box.pt2.z);

}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(MultiPoint3D_copy_constructor) 
{
  MultiPoint3dD multiPointD;
  multiPointD.push_back(Point3D(23.6, 94.4, 2.3));
  multiPointD.push_back(Point3D(75.36, 246.33, 36.36));
  multiPointD.push_back(Point3D(256.6, 619.3, 56.12));
  multiPointD.push_back(Point3D(62.36, 6.60, 24.63));

  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

  MultiPoint3dD multiPointD_c(multiPointD);
  BoundingBoxD box = multiPointD_c.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(2.3, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(56.12, box.pt2.z);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor a partir de un vector de puntos */

BOOST_AUTO_TEST_CASE(MultiPoint3D_vector) 
{
  std::vector<Point3D> multiPointD{ Point3D(23.6, 94.4, 0.36),
                                    Point3D(75.36, 246.33, 454.3),
                                    Point3D(256.6, 619.3, 26.21),
                                    Point3D(62.36, 6.60, 62.61) };

  MultiPoint3dD multiPointD_c(multiPointD);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor lista de inicializadores */

BOOST_AUTO_TEST_CASE(MultiPoint3D_initializer_list) 
{
  MultiPoint3dD multiPointD{ Point3D(23.6, 94.4, 0.36),
                             Point3D(75.36, 246.33, 454.3),
                             Point3D(256.6, 619.3, 26.21),
                             Point3D(62.36, 6.60, 62.61) };
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

}


/* Operador de asignación */

BOOST_AUTO_TEST_CASE(MultiPoint3D_assing_operator)
{
  MultiPoint3dD multiPointD{ Point3D(23.6, 94.4, 0.36),
                             Point3D(75.36, 246.33, 454.3),
                             Point3D(256.6, 619.3, 26.21),
                             Point3D(62.36, 6.60, 62.61) };

  MultiPoint3dD multiPointD_c = multiPointD;

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

BOOST_AUTO_TEST_CASE(MultiPoint3D_box) 
{
  std::vector<Point3D> multiPointD{ Point3D(23.6, 94.4, 0.36),
                                    Point3D(75.36, 246.33, 454.3),
                                    Point3D(256.6, 619.3, 26.21),
                                    Point3D(62.36, 6.60, 62.61) };

  MultiPoint3dD multiPointD_c(multiPointD);
  BoundingBoxD box = multiPointD_c.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box.pt2.z);
}