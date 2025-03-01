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
 
#define BOOST_TEST_MODULE Tidop LineString test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/linestring.h>
#include <tidop/geometry/entities/multilinestring.h>
#include <tidop/geometry/window.h>
#include <tidop/geometry/bbox.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(LineStringTestSuite)

std::initializer_list<Point<int>> initializer_list_line_string_integer 
{
  Point<int>(34, 34),
  Point<int>(45, 54),
  Point<int>(51, 45),
  Point<int>(12, 47),
  Point<int>(95, 25),
  Point<int>(15, 36),
  Point<int>(18, 82)
};

std::initializer_list<Point<double>> initializer_list_line_string_double 
{
  Point<double>(41572.22, 6647.89),
  Point<double>(41490.43, 6888.36),
  Point<double>(41728.03, 6903.40),
  Point<double>(41771.48, 6429.97),
  Point<double>(41370.12, 6718.08),
  Point<double>(41462.92, 6669.53),
  Point<double>(41387.59, 7026.70)
};



struct LineStringTest
{

  LineStringTest()
    : line_string_integer(nullptr),
      line_string_double(nullptr),
      line_string_float(nullptr),
      line_string_integer_copy(nullptr),
      line_string_double_copy(nullptr),
      line_string_float_copy(nullptr),
      line_string_constructor_reserve(nullptr),
      line_string_constructor_vector(nullptr)
  {

  }

  ~LineStringTest()
  {
    delete line_string_integer;
    delete line_string_double;
    delete line_string_float;
    delete line_string_integer_copy;
    delete line_string_double_copy;
    delete line_string_float_copy;
    delete line_string_constructor_reserve;
    delete line_string_constructor_vector;
  }

  void setup()
  {

    v_d.push_back(Point<double>(41572.22, 6647.89));
    v_d.push_back(Point<double>(41490.43, 6888.36));
    v_d.push_back(Point<double>(41728.03, 6903.40));
    v_d.push_back(Point<double>(41771.48, 6429.97));
    v_d.push_back(Point<double>(41370.12, 6718.08));
    v_d.push_back(Point<double>(41462.92, 6669.53));
    v_d.push_back(Point<double>(41387.59, 7026.70));

    line_string_integer = new LineString<Point<int>>(initializer_list_line_string_integer);
    line_string_double = new LineString<Point<double>>(initializer_list_line_string_double);
    line_string_float = new LineString<Point<float>>();

    line_string_integer_copy = new LineString<Point<int>>(*line_string_integer);
    line_string_double_copy = new LineString<Point<double>>(*line_string_double);
    line_string_float_copy = new LineString<Point<float>>(*line_string_float);
    
    line_string_constructor_reserve = new LineString<Point<int>>(10);

    line_string_constructor_vector = new LineString<Point<double>>(v_d);
  }
 
  void teardown()
  {

  }

  std::vector<Point<double>> v_d;

  LineString<Point<int>> line_string_default_constructor_integer;
  LineString<Point<double>> line_string_default_constructor_double;
  LineString<Point<float>> line_string_default_constructor_float;

  LineString<Point<int>> *line_string_integer;
  LineString<Point<double>> *line_string_double;
  LineString<Point<float>> *line_string_float;
  LineString<Point<int>> *line_string_integer_copy;
  LineString<Point<double>> *line_string_double_copy;
  LineString<Point<float>> *line_string_float_copy;

  LineString<Point<int>> *line_string_constructor_reserve;

  LineString<Point<double>> *line_string_constructor_vector;
};

BOOST_FIXTURE_TEST_CASE(copy_constructor, LineStringTest)
{
  for (size_t i = 0; i < line_string_double_copy->size(); i++) {
		BOOST_CHECK_EQUAL(v_d[i].x, line_string_double_copy->at(i).x);
		BOOST_CHECK_EQUAL(v_d[i].y, line_string_double_copy->at(i).y);
	}


  size_t j = 0;
	for (const auto &pt : initializer_list_line_string_integer) {
		BOOST_CHECK_EQUAL(pt.x, line_string_integer_copy->at(j).x);
		BOOST_CHECK_EQUAL(pt.y, line_string_integer_copy->at(j).y);
		j++;
	}
}

BOOST_FIXTURE_TEST_CASE(move_constructor, LineStringTest) 
{
  LineString<Point<double>> line_to_move(v_d);
  LineString<Point<double>> lineString(std::move(line_to_move));

  BOOST_CHECK_EQUAL(7, lineString.size());
  BOOST_CHECK_EQUAL(0, line_to_move.size());

  for (int i = 0; i < lineString.size(); i++) {
    BOOST_CHECK(lineString[i] == v_d[i]);
  }
    
}

BOOST_FIXTURE_TEST_CASE(size, LineStringTest) 
{
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_integer.size());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_double.size());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_float.size());
  BOOST_CHECK_EQUAL(7, line_string_integer->size());
  BOOST_CHECK_EQUAL(7, line_string_double->size());
  BOOST_CHECK_EQUAL(0, line_string_float->size());
  BOOST_CHECK_EQUAL(7, line_string_integer_copy->size());
  BOOST_CHECK_EQUAL(7, line_string_double_copy->size());
  BOOST_CHECK_EQUAL(0, line_string_float_copy->size());
  BOOST_CHECK_EQUAL(10, line_string_constructor_reserve->size());
  BOOST_CHECK_EQUAL(7, line_string_constructor_vector->size());
}

BOOST_FIXTURE_TEST_CASE(window, LineStringTest) 
{
  WindowI w_i = line_string_default_constructor_integer.window();
  BOOST_CHECK_EQUAL(TL_INT_MAX, w_i.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w_i.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w_i.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w_i.pt2.y);

  WindowD w_d = line_string_default_constructor_double.window();
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w_d.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w_d.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w_d.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w_d.pt2.y);
    
  WindowF w_f = line_string_default_constructor_float.window();
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.y);

  w_i = line_string_integer->window();
  BOOST_CHECK_EQUAL(12, w_i.pt1.x);
  BOOST_CHECK_EQUAL(25, w_i.pt1.y);
  BOOST_CHECK_EQUAL(95, w_i.pt2.x);
  BOOST_CHECK_EQUAL(82, w_i.pt2.y);

  w_d = line_string_double->window();
  BOOST_CHECK_EQUAL(41370.12, w_d.pt1.x);
  BOOST_CHECK_EQUAL(6429.97, w_d.pt1.y);
  BOOST_CHECK_EQUAL(41771.48, w_d.pt2.x);
  BOOST_CHECK_EQUAL(7026.70, w_d.pt2.y);
    
  w_f = line_string_float->window();
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.y);

  w_i = line_string_integer_copy->window();
  BOOST_CHECK_EQUAL(12, w_i.pt1.x);
  BOOST_CHECK_EQUAL(25, w_i.pt1.y);
  BOOST_CHECK_EQUAL(95, w_i.pt2.x);
  BOOST_CHECK_EQUAL(82, w_i.pt2.y);

  w_d = line_string_double_copy->window();
  BOOST_CHECK_EQUAL(41370.12, w_d.pt1.x);
  BOOST_CHECK_EQUAL(6429.97, w_d.pt1.y);
  BOOST_CHECK_EQUAL(41771.48, w_d.pt2.x);
  BOOST_CHECK_EQUAL(7026.70, w_d.pt2.y);
    
  w_f = line_string_float_copy->window();
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.y);

  w_d = line_string_constructor_reserve->window();
  BOOST_CHECK_EQUAL(0., w_d.pt1.x);
  BOOST_CHECK_EQUAL(0., w_d.pt1.y);
  BOOST_CHECK_EQUAL(0., w_d.pt2.x);
  BOOST_CHECK_EQUAL(0., w_d.pt2.y);

  w_d = line_string_constructor_vector->window();
  BOOST_CHECK_EQUAL(41370.12, w_d.pt1.x);
  BOOST_CHECK_EQUAL(6429.97, w_d.pt1.y);
  BOOST_CHECK_EQUAL(41771.48, w_d.pt2.x);
  BOOST_CHECK_EQUAL(7026.70, w_d.pt2.y);
}

BOOST_FIXTURE_TEST_CASE(length, LineStringTest) 
{
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_integer.length());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_double.length());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_float.length());
  BOOST_CHECK_CLOSE(285.409, line_string_integer->length(), 0.01);
  BOOST_CHECK_CLOSE(1931.316, line_string_double->length(), 0.01);
  BOOST_CHECK_EQUAL(0, line_string_float->length());
  BOOST_CHECK_CLOSE(285.409, line_string_integer_copy->length(), 0.01);
  BOOST_CHECK_CLOSE(1931.316, line_string_double_copy->length(), 0.01);
  BOOST_CHECK_EQUAL(0, line_string_float_copy->length());
  BOOST_CHECK_CLOSE(0, line_string_constructor_reserve->length(), 0.01);
  BOOST_CHECK_CLOSE(1931.316, line_string_constructor_vector->length(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(type, LineStringTest) 
{
  BOOST_CHECK(Entity::Type::linestring2d == line_string_default_constructor_integer.type());
  BOOST_CHECK(Entity::Type::linestring2d == line_string_integer->type());
  BOOST_CHECK(Entity::Type::linestring2d == line_string_integer_copy->type());
  BOOST_CHECK(Entity::Type::linestring2d == line_string_constructor_reserve->type());
  BOOST_CHECK(Entity::Type::linestring2d == line_string_constructor_vector->type());
}

BOOST_FIXTURE_TEST_CASE(is3D, LineStringTest) 
{
  BOOST_CHECK_EQUAL(false, line_string_default_constructor_integer.is3D());
  BOOST_CHECK_EQUAL(false, line_string_integer->is3D());
  BOOST_CHECK_EQUAL(false, line_string_integer_copy->is3D());
  BOOST_CHECK_EQUAL(false, line_string_constructor_reserve->is3D());
  BOOST_CHECK_EQUAL(false, line_string_constructor_vector->is3D());
}

BOOST_FIXTURE_TEST_CASE(assing_operator, LineStringTest) 
{
  LineStringD lineString;
  lineString = *line_string_double;

  for (int i = 0; i < lineString.size(); i++) {
    BOOST_CHECK(lineString[i] == line_string_double->at(i));
  }
}

BOOST_FIXTURE_TEST_CASE(move_operator, LineStringTest) 
{
  LineString<Point<double>> line_to_move(v_d);
  LineString<Point<double>> lineString;
  lineString = std::move(line_to_move);

  BOOST_CHECK_EQUAL(7, lineString.size());
  BOOST_CHECK_EQUAL(0, line_to_move.size());

  for (int i = 0; i < lineString.size(); i++) {
    BOOST_CHECK(lineString[i] == v_d[i]);
  }
}


BOOST_AUTO_TEST_SUITE_END()



/* LineString3D */

BOOST_AUTO_TEST_SUITE(LineString3DTestSuite)

std::initializer_list<Point3<int>> initializer_list_line_string_3d_integer 
{
  Point3<int>(34, 34, 3),
  Point3<int>(45, 54, 6),
  Point3<int>(51, 45, 43),
  Point3<int>(12, 47, 34),
  Point3<int>(95, 25, 24),
  Point3<int>(15, 36, 45),
  Point3<int>(18, 82, 36)
};


struct LineString3DTest
{

  LineString3DTest()
    : line_string_integer(nullptr),
      line_string_double(nullptr),
      line_string_float(nullptr),
      line_string_integer_copy(nullptr),
      line_string_double_copy(nullptr),
      line_string_float_copy(nullptr)
  {

  }

  ~LineString3DTest()
  {
    delete line_string_integer;
    delete line_string_double;
    delete line_string_float;
    delete line_string_integer_copy;
    delete line_string_double_copy;
    delete line_string_float_copy;
  }

  void setup()
  {

    v_d.push_back(Point3<double>(23.6, 94.4, 0.36));
    v_d.push_back(Point3<double>(75.36, 246.33, 454.3));
    v_d.push_back(Point3<double>(256.6, 619.3, 26.21));
    v_d.push_back(Point3<double>(62.36, 6.60, 62.61));

    line_string_integer = new LineString3D<Point3<int>>(initializer_list_line_string_3d_integer);
    line_string_double = new LineString3D<Point3<double>>(v_d);
    line_string_float = new LineString3D<Point3<float>>(10);

    line_string_integer_copy = new LineString3D<Point3<int>>(*line_string_integer);
    line_string_double_copy = new LineString3D<Point3<double>>(*line_string_double);
    line_string_float_copy = new LineString3D<Point3<float>>(*line_string_float);
    
  }
 
  void teardown()
  {

  }

  std::vector<Point3<double>> v_d;

  LineString3D<Point3<int>> line_string_default_constructor_integer;
  LineString3D<Point3<double>> line_string_default_constructor_double;
  LineString3D<Point3<float>> line_string_default_constructor_float;

  LineString3D<Point3<int>> *line_string_integer;
  LineString3D<Point3<double>> *line_string_double;
  LineString3D<Point3<float>> *line_string_float;
  LineString3D<Point3<int>> *line_string_integer_copy;
  LineString3D<Point3<double>> *line_string_double_copy;
  LineString3D<Point3<float>> *line_string_float_copy;
};


BOOST_FIXTURE_TEST_CASE(copy_constructor, LineString3DTest)
{
  for (size_t i = 0; i < line_string_double_copy->size(); i++) {
		BOOST_CHECK_EQUAL(v_d[i].x, line_string_double_copy->at(i).x);
		BOOST_CHECK_EQUAL(v_d[i].y, line_string_double_copy->at(i).y);
    BOOST_CHECK_EQUAL(v_d[i].z, line_string_double_copy->at(i).z);
	}


  size_t j = 0;
	for (const auto &pt : initializer_list_line_string_3d_integer) {
		BOOST_CHECK_EQUAL(pt.x, line_string_integer_copy->at(j).x);
		BOOST_CHECK_EQUAL(pt.y, line_string_integer_copy->at(j).y);
    BOOST_CHECK_EQUAL(pt.z, line_string_integer_copy->at(j).z);
		j++;
	}
}

BOOST_FIXTURE_TEST_CASE(move_constructor, LineString3DTest) 
{
  LineString3D<Point3<double>> line_to_move(v_d);
  LineString3D<Point3<double>> lineString(std::move(line_to_move));

  BOOST_CHECK_EQUAL(4, lineString.size());
  BOOST_CHECK_EQUAL(0, line_to_move.size());

  for (int i = 0; i < lineString.size(); i++) {
    BOOST_CHECK(lineString[i] == v_d[i]);
  }
    
}

BOOST_FIXTURE_TEST_CASE(size, LineString3DTest) 
{
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_integer.size());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_double.size());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_float.size());
  BOOST_CHECK_EQUAL(7, line_string_integer->size());
  BOOST_CHECK_EQUAL(4, line_string_double->size());
  BOOST_CHECK_EQUAL(10, line_string_float->size());
  BOOST_CHECK_EQUAL(7, line_string_integer_copy->size());
  BOOST_CHECK_EQUAL(4, line_string_double_copy->size());
  BOOST_CHECK_EQUAL(10, line_string_float_copy->size());
}

BOOST_FIXTURE_TEST_CASE(boundingBox, LineString3DTest) 
{
  BoundingBoxI box_i = line_string_default_constructor_integer.boundingBox();
  BOOST_CHECK_EQUAL(TL_INT_MAX, box_i.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box_i.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box_i.pt1.z);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box_i.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box_i.pt2.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box_i.pt2.z);

  BoundingBoxD box_d = line_string_default_constructor_double.boundingBox();
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box_d.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box_d.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box_d.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box_d.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box_d.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box_d.pt2.z);
    
  BoundingBoxF box_f = line_string_default_constructor_float.boundingBox();
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box_f.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box_f.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box_f.pt1.z);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box_f.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box_f.pt2.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box_f.pt2.z);

  box_i = line_string_integer->boundingBox();
  BOOST_CHECK_EQUAL(12, box_i.pt1.x);
  BOOST_CHECK_EQUAL(25, box_i.pt1.y);
  BOOST_CHECK_EQUAL(3, box_i.pt1.z);
  BOOST_CHECK_EQUAL(95, box_i.pt2.x);
  BOOST_CHECK_EQUAL(82, box_i.pt2.y);
  BOOST_CHECK_EQUAL(45, box_i.pt2.z);

  box_d = line_string_double->boundingBox();
  BOOST_CHECK_EQUAL(23.6, box_d.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box_d.pt1.y);
  BOOST_CHECK_EQUAL(0.36,  box_d.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box_d.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box_d.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box_d.pt2.z);
    
  box_f = line_string_float->boundingBox();
  BOOST_CHECK_EQUAL(0.f, box_f.pt1.x);
  BOOST_CHECK_EQUAL(0.f, box_f.pt1.y);
  BOOST_CHECK_EQUAL(0.f, box_f.pt1.z);
  BOOST_CHECK_EQUAL(0.f, box_f.pt2.x);
  BOOST_CHECK_EQUAL(0.f, box_f.pt2.y);
  BOOST_CHECK_EQUAL(0.f, box_f.pt2.z);

  box_i = line_string_integer_copy->boundingBox();
  BOOST_CHECK_EQUAL(12, box_i.pt1.x);
  BOOST_CHECK_EQUAL(25, box_i.pt1.y);
  BOOST_CHECK_EQUAL(3, box_i.pt1.z);
  BOOST_CHECK_EQUAL(95, box_i.pt2.x);
  BOOST_CHECK_EQUAL(82, box_i.pt2.y);
  BOOST_CHECK_EQUAL(45, box_i.pt2.z);

  box_d = line_string_double_copy->boundingBox();
  BOOST_CHECK_EQUAL(23.6, box_d.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box_d.pt1.y);
  BOOST_CHECK_EQUAL(0.36,  box_d.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box_d.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box_d.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box_d.pt2.z);
    
  box_f = line_string_float_copy->boundingBox();
  BOOST_CHECK_EQUAL(0.f, box_f.pt1.x);
  BOOST_CHECK_EQUAL(0.f, box_f.pt1.y);
  BOOST_CHECK_EQUAL(0.f, box_f.pt1.z);
  BOOST_CHECK_EQUAL(0.f, box_f.pt2.x);
  BOOST_CHECK_EQUAL(0.f, box_f.pt2.y);
  BOOST_CHECK_EQUAL(0.f, box_f.pt2.z);

}

BOOST_FIXTURE_TEST_CASE(length, LineString3DTest) 
{
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_integer.length());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_double.length());
  BOOST_CHECK_EQUAL(0, line_string_default_constructor_float.length());
  BOOST_CHECK_CLOSE(285.409, line_string_integer->length(), 0.01);
  BOOST_CHECK_CLOSE(1217.93, line_string_double->length(), 0.01);
  BOOST_CHECK_EQUAL(0, line_string_float->length());
  BOOST_CHECK_CLOSE(285.409, line_string_integer_copy->length(), 0.01);
  BOOST_CHECK_CLOSE(1217.93, line_string_double_copy->length(), 0.01);
  BOOST_CHECK_EQUAL(0, line_string_float_copy->length());
}

BOOST_FIXTURE_TEST_CASE(type, LineString3DTest) 
{
  BOOST_CHECK(Entity::Type::linestring3d == line_string_default_constructor_integer.type());
  BOOST_CHECK(Entity::Type::linestring3d == line_string_integer->type());
  BOOST_CHECK(Entity::Type::linestring3d == line_string_integer_copy->type());
  BOOST_CHECK(Entity::Type::linestring3d == line_string_float->type());
  BOOST_CHECK(Entity::Type::linestring3d == line_string_double_copy->type());
}

BOOST_FIXTURE_TEST_CASE(is3D, LineString3DTest) 
{
  BOOST_CHECK_EQUAL(true, line_string_default_constructor_integer.is3D());
  BOOST_CHECK_EQUAL(true, line_string_integer->is3D());
  BOOST_CHECK_EQUAL(true, line_string_integer_copy->is3D());
  BOOST_CHECK_EQUAL(true, line_string_float->is3D());
  BOOST_CHECK_EQUAL(true, line_string_double_copy->is3D());
}

BOOST_FIXTURE_TEST_CASE(copy_assing_operator, LineString3DTest) 
{
  LineString3D<Point3<double>> lineString;
  lineString = *line_string_double;

  for (int i = 0; i < lineString.size(); i++) {
    BOOST_CHECK(lineString[i] == line_string_double->at(i));
  }
}

BOOST_FIXTURE_TEST_CASE(move_operator, LineString3DTest) 
{
  LineString3D<Point3<double>> line_to_move(v_d);
  LineString3D<Point3<double>> lineString;
  lineString = std::move(line_to_move);

  BOOST_CHECK_EQUAL(4, lineString.size());
  BOOST_CHECK_EQUAL(0, line_to_move.size());

  for (int i = 0; i < lineString.size(); i++) {
    BOOST_CHECK(lineString[i] == v_d[i]);
  }
}

BOOST_AUTO_TEST_SUITE_END()




/* MultiLineString  */

BOOST_AUTO_TEST_SUITE(MultiLineStringTestSuite)

std::initializer_list<Point<int>> points1
{
  Point<int>(34, 34),
  Point<int>(45, 54),
  Point<int>(51, 45),
  Point<int>(12, 47),
  Point<int>(95, 25),
  Point<int>(15, 36),
  Point<int>(18, 82)
};

std::initializer_list<Point<int>> points2
{
  Point<int>(76, 70),
  Point<int>(26, 35),
  Point<int>(55, 25),
  Point<int>(82, 21)
};



struct MultiLineStringTest
{

  MultiLineStringTest()
    : multiline_string_integer(nullptr)
    //line_string_double(nullptr),
    //line_string_float(nullptr),
    //line_string_integer_copy(nullptr),
    //line_string_double_copy(nullptr),
    //line_string_float_copy(nullptr),
    //line_string_constructor_reserve(nullptr),
    //line_string_constructor_vector(nullptr)
  {

  }

  ~MultiLineStringTest()
  {
    delete multiline_string_integer;
    //delete line_string_double;
    //delete line_string_float;
    //delete line_string_integer_copy;
    //delete line_string_double_copy;
    //delete line_string_float_copy;
    //delete line_string_constructor_reserve;
    //delete line_string_constructor_vector;
  }

  void setup()
  {

    //v_d.push_back(Point<double>(41572.22, 6647.89));
    //v_d.push_back(Point<double>(41490.43, 6888.36));
    //v_d.push_back(Point<double>(41728.03, 6903.40));
    //v_d.push_back(Point<double>(41771.48, 6429.97));
    //v_d.push_back(Point<double>(41370.12, 6718.08));
    //v_d.push_back(Point<double>(41462.92, 6669.53));
    //v_d.push_back(Point<double>(41387.59, 7026.70));

    multiline_string_integer = new MultiLineString<Point<int>>();
    multiline_string_integer->push_back(LineStringI(points1));
    multiline_string_integer->push_back(LineStringI(points2));

    //line_string_double = new LineString<Point<double>>(initializer_list_line_string_double);
    //line_string_float = new LineString<Point<float>>();

    //line_string_integer_copy = new LineString<Point<int>>(*line_string_integer);
    //line_string_double_copy = new LineString<Point<double>>(*line_string_double);
    //line_string_float_copy = new LineString<Point<float>>(*line_string_float);

    //line_string_constructor_reserve = new LineString<Point<int>>(10);

    //line_string_constructor_vector = new LineString<Point<double>>(v_d);
  }

  void teardown()
  {

  }

  //std::vector<Point<double>> v_d;

  MultiLineString<Point<double>> multiline_default_constructor;

  MultiLineString<Point<int>> *multiline_string_integer;
  //LineString<Point<double>> *line_string_double;
  //LineString<Point<float>> *line_string_float;
  //LineString<Point<int>> *line_string_integer_copy;
  //LineString<Point<double>> *line_string_double_copy;
  //LineString<Point<float>> *line_string_float_copy;

  //LineString<Point<int>> *line_string_constructor_reserve;

  //LineString<Point<double>> *line_string_constructor_vector;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, MultiLineStringTest)
{
  WindowD w = multiline_default_constructor.window();
  
  BOOST_CHECK_EQUAL(0, multiline_default_constructor.size());
  BOOST_CHECK(multiline_default_constructor.type() == Entity::Type::multiline2d);
  BOOST_CHECK_EQUAL(false, multiline_default_constructor.is3D());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.y);
}

BOOST_FIXTURE_TEST_CASE(push_back, MultiLineStringTest)
{
  BOOST_CHECK_EQUAL(2, multiline_string_integer->size());
  BOOST_CHECK_EQUAL(7, multiline_string_integer->at(0).size());
  BOOST_CHECK_EQUAL(4, multiline_string_integer->at(1).size());
  BOOST_CHECK_EQUAL(34, multiline_string_integer->at(0).at(0).x);
  BOOST_CHECK_EQUAL(34, multiline_string_integer->at(0).at(0).y);
}

BOOST_AUTO_TEST_CASE(MultiLineString_constructor_reserve)
{
  MultiLineString<Point<double>> multiline(10);
  WindowD w = multiline.window();

  BOOST_CHECK_EQUAL(10, multiline.size());
  BOOST_CHECK(multiline.type() == Entity::Type::multiline2d);
  BOOST_CHECK_EQUAL(false, multiline.is3D());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.y);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiLineStringTest)
{
  MultiLineString<Point<int>> multiline(*multiline_string_integer);
  BOOST_CHECK_EQUAL(2, multiline.size());
  BOOST_CHECK_EQUAL(7, multiline.at(0).size());
  BOOST_CHECK_EQUAL(4, multiline.at(1).size());
}


//BOOST_AUTO_TEST_CASE(MultiLineString_copy_constructor) 
//{
//  std::vector<Point<int>> vect1
//  {
//    Point<int>(34, 34),
//    Point<int>(45, 54),
//    Point<int>(51, 45),
//    Point<int>(12, 47),
//    Point<int>(95, 25),
//    Point<int>(15, 36),
//    Point<int>(18, 82)
//  };
//  LineStringI  line1(vect1);
//
//  std::vector<Point<int>> vect2{
//	  Point<int>(4150653, 668925),
//	  Point<int>(4148532, 688836),
//	  Point<int>(4120568, 693696),
//	  Point<int>(4153569, 643656),
//	  Point<int>(4152639, 665658),
//	  Point<int>(4156305, 636996),
//	  Point<int>(4139568, 653366) };
//  LineStringI  line2(vect2);
//
//  MultiLineString<Point<int>> multiline;
//  multiline.push_back(line1);
//  multiline.push_back(line2);
//
//	MultiLineString<Point<int>> multiline_copy(multiline);
//  WindowI w = multiline_copy.window();
//
//  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline2d);
//  BOOST_CHECK_EQUAL(false, multiline_copy.is3D());
//
//	BOOST_CHECK_EQUAL(2, multiline_copy.size());
//  BOOST_CHECK_EQUAL(4120568, w.pt1.x);
//  BOOST_CHECK_EQUAL(636996, w.pt1.y);
//  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
//  BOOST_CHECK_EQUAL(702670, w.pt2.y);
//
//}

//BOOST_AUTO_TEST_CASE(MultiLineString_assing_operator)
//{
//  LineStringI  line1(ptsIn);
//  std::vector<Point<int>> vect{
//	  Point<int>(4150653, 668925),
//	  Point<int>(4148532, 688836),
//	  Point<int>(4120568, 693696),
//	  Point<int>(4153569, 643656),
//	  Point<int>(4152639, 665658),
//	  Point<int>(4156305, 636996),
//	  Point<int>(4139568, 653366) };
//  LineStringI  line2(vect);
//
//  MultiLineString<Point<int>> multiline;
//  multiline.push_back(line1);
//  multiline.push_back(line2);
//
//  MultiLineString<Point<int>> multiline_copy = multiline;
//  WindowI w = multiline_copy.window();
//
//  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline2d);
//  BOOST_CHECK_EQUAL(false, multiline_copy.is3D());
//	BOOST_CHECK_EQUAL(2, multiline_copy.size());
//  BOOST_CHECK_EQUAL(4120568, w.pt1.x);
//  BOOST_CHECK_EQUAL(636996, w.pt1.y);
//  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
//  BOOST_CHECK_EQUAL(702670, w.pt2.y);
//
//  LineString<Point<int>> linestring = multiline_copy[1];
//  for (size_t i = 0; i < linestring.size(); i++) {
//    BOOST_CHECK_EQUAL(vect[i].x, linestring[i].x);
//    BOOST_CHECK_EQUAL(vect[i].y, linestring[i].y);
//  }
//}

BOOST_AUTO_TEST_SUITE_END()

/* MultiLineString3D */

//BOOST_AUTO_TEST_CASE(MultiLineString3D_default_constructor) 
//{
//  MultiLineString3D<Point3<double>> multiline_3d;
//  BoundingBoxD box = multiline_3d.boundingBox();
//  
//  BOOST_CHECK_EQUAL(0, multiline_3d.size());
//  BOOST_CHECK(multiline_3d.type() == Entity::Type::multiline3d);
//  BOOST_CHECK(multiline_3d.is3D());
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);
//}
//
//BOOST_AUTO_TEST_CASE(MultiLineString3D_constructor_reserve) 
//{
//  MultiLineString3D<Point3<double>> multiline_3d(10);
//  BoundingBoxD box = multiline_3d.boundingBox();
//  
//  BOOST_CHECK_EQUAL(10, multiline_3d.size());
//  BOOST_CHECK(multiline_3d.type() == Entity::Type::multiline3d);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
//  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);
//}
//
//BOOST_AUTO_TEST_CASE(MultiLineString3D_copy_constructor) 
//{
//  std::vector<Point3<double>> vector{
//    Point3<double>(23.6, 94.4, 0.36),
//    Point3<double>(75.36, 246.33, 454.3),
//    Point3<double>(256.6, 619.3, 26.21),
//    Point3<double>(62.36, 6.60, 62.61) 
//  };
//
//  LineString3dD linestring(vector);
//
//  MultiLineString3D<Point3<double>> multiline;
//  multiline.push_back(linestring);
//
//	MultiLineString3D<Point3<double>> multiline_copy(multiline);
//  
//  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline3d);
//  BOOST_CHECK(multiline_copy.is3D());
//
//	BOOST_CHECK_EQUAL(1, multiline_copy.size());
//  BoundingBoxD box = multiline_copy.boundingBox();
//  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
//  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
//  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
//  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
//  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
//  BOOST_CHECK_EQUAL(454.3, box.pt2.z);
//}
//
//BOOST_AUTO_TEST_CASE(MultiLineString3D_assing_operator)
//{
//  std::vector<Point3<double>> vector{
//    Point3<double>(23.6, 94.4, 0.36),
//    Point3<double>(75.36, 246.33, 454.3),
//    Point3<double>(256.6, 619.3, 26.21),
//    Point3<double>(62.36, 6.60, 62.61) 
//  };
//
//  LineString3dD linestring(vector);
//
//  MultiLineString3D<Point3<double>> multiline;
//  multiline.push_back(linestring);
//
//  MultiLineString3D<Point3<double>> multiline_copy = multiline;
//
//  BOOST_CHECK(multiline_copy.type() == Entity::Type::multiline3d);
//  BOOST_CHECK(multiline_copy.is3D());
//
//	BOOST_CHECK_EQUAL(1, multiline_copy.size());
//  BoundingBoxD box = multiline_copy.boundingBox();
//  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
//  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
//  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
//  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
//  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
//  BOOST_CHECK_EQUAL(454.3, box.pt2.z);
//}