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

#define BOOST_TEST_MODULE Tidop Size test
#include <boost/test/unit_test.hpp>
#include <tidop/core/base/size.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(SizeTestSuite)

struct SizeTest
{

  SizeTest()
    : size_integer(nullptr),
      size_double(nullptr),
      size_float(nullptr),
      size_integer_copy(nullptr),
      size_double_copy(nullptr),
      size_float_copy(nullptr),
      size_constructor_integer_width_height(nullptr),
      size_constructor_double_width_height(nullptr),
      size_constructor_float_width_height(nullptr)
  {

  }

  ~SizeTest()
  {
    delete size_integer;
    delete size_double;
    delete size_float;
    delete size_integer_copy;
    delete size_double_copy;
    delete size_float_copy;
    delete size_constructor_integer_width_height;
    delete size_constructor_double_width_height;
    delete size_constructor_float_width_height;
  }

  void setup()
  {
    size_integer = new Size<int>(100, 100);
    size_double = new Size<double>(100.4, 100.2);
    size_float = new Size<float>(100.4f, 100.2f);

    size_integer_copy = new Size<int>(*size_integer);
    size_double_copy = new Size<double>(*size_double);
    size_float_copy = new Size<float>(*size_float);

    size_constructor_integer_width_height = new Size<int>(100, 100);
    size_constructor_double_width_height = new Size<double>(100.4, 100.2);
    size_constructor_float_width_height = new Size<float>(100.4f, 100.2f);

  }
 
  void teardown()
  {

  }

  Size<int> size_default_constructor_integer;
  Size<double> size_default_constructor_double;
  Size<float> size_default_constructor_float;

  Size<int> *size_integer;
  Size<double> *size_double;
  Size<float> *size_float;
  Size<int> *size_integer_copy;
  Size<double> *size_double_copy;
  Size<float> *size_float_copy;

  Size<int> *size_constructor_integer_width_height;
  Size<double> *size_constructor_double_width_height;
  Size<float> *size_constructor_float_width_height;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, SizeTest) 
{

  BOOST_CHECK_EQUAL(0, size_default_constructor_integer.width);
  BOOST_CHECK_EQUAL(0, size_default_constructor_integer.height);


  BOOST_CHECK_EQUAL(0., size_default_constructor_double.width);
  BOOST_CHECK_EQUAL(0., size_default_constructor_double.height);
    
  BOOST_CHECK_EQUAL(0.f, size_default_constructor_float.width);
  BOOST_CHECK_EQUAL(0.f, size_default_constructor_float.height);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, SizeTest) 
{
  BOOST_CHECK_EQUAL(size_integer->width, size_integer_copy->width);
  BOOST_CHECK_EQUAL(size_integer->height, size_integer_copy->height);
                    
  BOOST_CHECK_EQUAL(size_double->width, size_double_copy->width);
  BOOST_CHECK_EQUAL(size_double->height, size_double_copy->height);
                    
  BOOST_CHECK_EQUAL(size_float->width, size_float_copy->width);
  BOOST_CHECK_EQUAL(size_float->height, size_float_copy->height);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, SizeTest) 
{
  Size<int> size(100, 100);
  Size<int> size2(std::move(size));
  BOOST_CHECK_EQUAL(100, size2.width);
  BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_FIXTURE_TEST_CASE(copy_assignment, SizeTest)
{
  Size<int> size(100, 100);
  Size<int> size2(50, 50);
  size2 = size;
  BOOST_CHECK_EQUAL(100, size2.width);
  BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_FIXTURE_TEST_CASE(move_assignment, SizeTest)
{
  Size<int> size2;
  size2 = Size<int>(100, 100);
  BOOST_CHECK_EQUAL(100, size2.width);
  BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_FIXTURE_TEST_CASE(constructor_width_height, SizeTest) 
{
  BOOST_CHECK_EQUAL(100, size_constructor_integer_width_height->width);
  BOOST_CHECK_EQUAL(100, size_constructor_integer_width_height->height);
                    
  BOOST_CHECK_EQUAL(100.4, size_constructor_double_width_height->width);
  BOOST_CHECK_EQUAL(100.2, size_constructor_double_width_height->height);
                    
  BOOST_CHECK_EQUAL(100.4f, size_constructor_float_width_height->width);
  BOOST_CHECK_EQUAL(100.2f, size_constructor_float_width_height->height);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, SizeTest)
{
  BOOST_CHECK_EQUAL(true, size_default_constructor_integer.isEmpty());
  BOOST_CHECK_EQUAL(true, size_default_constructor_double.isEmpty());
  BOOST_CHECK_EQUAL(true, size_default_constructor_float.isEmpty());
  BOOST_CHECK_EQUAL(false, size_integer->isEmpty());
  BOOST_CHECK_EQUAL(false, size_double->isEmpty());
  BOOST_CHECK_EQUAL(false, size_double->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(cast, SizeTest)
{
  Size<int> size = static_cast<Size<int>>(*size_double);
  BOOST_CHECK_EQUAL(100, size.width);
  BOOST_CHECK_EQUAL(100, size.height);

  Size<double> size2 = static_cast<Size<double>>(*size_integer);
  BOOST_CHECK_EQUAL(100., size2.width);
  BOOST_CHECK_EQUAL(100., size2.height);
}

BOOST_FIXTURE_TEST_CASE(is_equal, SizeTest)
{
  Size<int> size = *size_integer;
  BOOST_CHECK(size == *size_integer);
}

BOOST_FIXTURE_TEST_CASE(is_not_equal, SizeTest)
{
  Size<int> size(50, 50);
  BOOST_CHECK(size != *size_integer);
}

BOOST_FIXTURE_TEST_CASE(add1, SizeTest)
{
  Size<int> size1(50, 50);
  Size<int> size2(20, 30);
  Size<int> size3 = size1 + size2;
  BOOST_CHECK_EQUAL(70, size3.width);
  BOOST_CHECK_EQUAL(80, size3.height);
}

BOOST_FIXTURE_TEST_CASE(add2, SizeTest)
{
  Size<int> size1(50, 50);
  Size<int> size2(20, 30);
  size2 += size1;
  BOOST_CHECK_EQUAL(70, size2.width);
  BOOST_CHECK_EQUAL(80, size2.height);
}

BOOST_FIXTURE_TEST_CASE(minus1, SizeTest)
{
  Size<int> size1(50, 50);
  Size<int> size2(20, 30);
  Size<int> size3 = size1 - size2;
  BOOST_CHECK_EQUAL(30, size3.width);
  BOOST_CHECK_EQUAL(20, size3.height);
}

BOOST_FIXTURE_TEST_CASE(minus2, SizeTest)
{
  Size<int> size1(50, 50);
  Size<int> size2(20, 30);
  size2 -= size1;
  BOOST_CHECK_EQUAL(-30, size2.width);
  BOOST_CHECK_EQUAL(-20, size2.height);
}

BOOST_FIXTURE_TEST_CASE(mul1, SizeTest)
{
  Size<int> size1(14, 23);
  Size<int> size2 = size1 * 10;
  BOOST_CHECK_EQUAL(140, size2.width);
  BOOST_CHECK_EQUAL(230, size2.height);
}

BOOST_FIXTURE_TEST_CASE(mul2, SizeTest)
{
  Size<int> size1(14, 23);
  size1 *= 10;
  BOOST_CHECK_EQUAL(140, size1.width);
  BOOST_CHECK_EQUAL(230, size1.height);
}

BOOST_FIXTURE_TEST_CASE(div1, SizeTest)
{
  Size<double> size1(14., 23.);
  Size<double> size2 = size1 / 10.;
  BOOST_CHECK_EQUAL(1.4, size2.width);
  BOOST_CHECK_EQUAL(2.3, size2.height);
}

BOOST_FIXTURE_TEST_CASE(div2, SizeTest)
{
  Size<double> size1(14., 23.);
  size1 /= 10.;
  BOOST_CHECK_EQUAL(1.4, size1.width);
  BOOST_CHECK_EQUAL(2.3, size1.height);
}

BOOST_AUTO_TEST_SUITE_END()

