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

#define BOOST_TEST_MODULE Tidop simd test
#include <boost/test/unit_test.hpp>
#include <tidop/math/simd.h>
#include <tidop/math/algebra/vector.h>
#include <tidop/math/algebra/matrix.h>

using namespace tl::math;


BOOST_AUTO_TEST_SUITE(PackedTestSuite)

struct PackedTest
{
  PackedTest()
  {}

  ~PackedTest()
  { 

  }
  
  void setup()
  {
    v1 = { 1, 0, 3, 5, 8, 9, 2, 2, 5, 2, 9, 5 };
    v2 = { -1, 4, 2, 2, 2, 8, 2, 3, 8, 4, 1, 7 };

  }
 
  void teardown()
  {

  }

  Packed<float> packed_a;
  Packed<float> packed_b;
  Packed<double> packed_a_d;
  Packed<double> packed_d_d;

  Vector<float> v1;
  Vector<float> v2;
  Vector<double> v1d;
  Vector<double> v2d;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, PackedTest)
{

}

BOOST_FIXTURE_TEST_CASE(load_store_aligned, PackedTest)
{
  Vector<float> v3(v1.size());

  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadAligned(&v1[i]);

    packed_a.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); i++)
  {
    v3[i] = v1[i];
  }

  for (size_t i = 0; i < v1.size(); i++) {
    BOOST_CHECK_EQUAL(v1[i], v3[i]);
  }
}


BOOST_FIXTURE_TEST_CASE(vector_add, PackedTest)
{

  Vector<float> v3(v1.size());

  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadAligned(&v1[i]);
    packed_b.loadAligned(&v2[i]);

    Packed<float> packed_c = packed_a + packed_b;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); ++i)
  {
    v3[i] = v1[i] + v2[i];
  }

  BOOST_CHECK_EQUAL(0.f, v3[0]);
  BOOST_CHECK_EQUAL(4.f, v3[1]);
  BOOST_CHECK_EQUAL(5.f, v3[2]);
  BOOST_CHECK_EQUAL(7.f, v3[3]);
  BOOST_CHECK_EQUAL(10.f, v3[4]);
  BOOST_CHECK_EQUAL(17.f, v3[5]);
  BOOST_CHECK_EQUAL(4.f, v3[6]);
  BOOST_CHECK_EQUAL(5.f, v3[7]);
  BOOST_CHECK_EQUAL(13.f, v3[8]);
  BOOST_CHECK_EQUAL(6.f, v3[9]);
  BOOST_CHECK_EQUAL(10.f, v3[10]);
  BOOST_CHECK_EQUAL(12.f, v3[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_multiplication, PackedTest)
{

  Vector<float> v3(v1.size());

  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadAligned(&v1[i]);
    packed_b.loadAligned(&v2[i]);

    Packed<float> packed_c = packed_a * packed_b;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); ++i)
  {
    v3[i] = v1[i] * v2[i];
  }

  BOOST_CHECK_EQUAL(-1.f, v3[0]);
  BOOST_CHECK_EQUAL(0.f, v3[1]);
  BOOST_CHECK_EQUAL(6.f, v3[2]);
  BOOST_CHECK_EQUAL(10.f, v3[3]);
  BOOST_CHECK_EQUAL(16.f, v3[4]);
  BOOST_CHECK_EQUAL(72.f, v3[5]);
  BOOST_CHECK_EQUAL(4.f, v3[6]);
  BOOST_CHECK_EQUAL(6.f, v3[7]);
  BOOST_CHECK_EQUAL(40.f, v3[8]);
  BOOST_CHECK_EQUAL(8.f, v3[9]);
  BOOST_CHECK_EQUAL(9.f, v3[10]);
  BOOST_CHECK_EQUAL(35.f, v3[11]);

}


BOOST_AUTO_TEST_SUITE_END()