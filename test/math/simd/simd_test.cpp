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
using namespace simd;

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
    v1 = { 1.f, 0.f, 3.f, 5.f, 8.f, 9.f, 2.f, 2.f, 5.f, 2.f, 9.f, 5.f };
    v2 = { -1.f, 4.f, 2.f, 2.f, 2.f, 8.f, 2.f, 3.f, 8.f, 4.f, 1.f, 7.f };
    v1d = { 1., 0., 3., 5., 8., 9., 2., 2., 5., 2., 9., 5. };
    v2d = { -1., 4., 2., 2., 2., 8., 2., 3., 8., 4., 1., 7. };
    v1i32 = { 1, 0, 3, 5, 8, 9, 2, 2, 5, 2, 9, 5 };
    v2i32 = { -1, 4, 2, 2, 2, 8, 2, 3, 8, 4, 1, 7 };
  }
 
  void teardown()
  {

  }

  Packed<float> packed_a;
  Packed<float> packed_b;
  Packed<double> packed_a_d;
  Packed<double> packed_b_d;
  Packed<int32_t> packed_a_i32;
  Packed<int32_t> packed_b_i32;

  Vector<float> v1;
  Vector<float> v2;
  Vector<double> v1d;
  Vector<double> v2d;
  Vector<int32_t> v1i32;
  Vector<int32_t> v2i32;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, PackedTest)
{

}

BOOST_FIXTURE_TEST_CASE(size, PackedTest)
{
#ifdef HAVE_AVX
  BOOST_CHECK_EQUAL(4, Packed<double>::size());
  BOOST_CHECK_EQUAL(8, Packed<float>::size());
  BOOST_CHECK_EQUAL(32, Packed<int8_t>::size());
  BOOST_CHECK_EQUAL(32, Packed<uint8_t>::size());
  BOOST_CHECK_EQUAL(16, Packed<int16_t>::size());
  BOOST_CHECK_EQUAL(16, Packed<uint16_t>::size());
  BOOST_CHECK_EQUAL(8, Packed<int32_t>::size());
  BOOST_CHECK_EQUAL(8, Packed<uint32_t>::size());
  BOOST_CHECK_EQUAL(4, Packed<int64_t>::size());
  BOOST_CHECK_EQUAL(4, Packed<uint64_t>::size());
#elif defined HAVE_SSE2
  BOOST_CHECK_EQUAL(2, Packed<double>::size());
  BOOST_CHECK_EQUAL(4, Packed<float>::size());
  BOOST_CHECK_EQUAL(16, Packed<int8_t>::size());
  BOOST_CHECK_EQUAL(16, Packed<uint8_t>::size());
  BOOST_CHECK_EQUAL(8, Packed<int16_t>::size());
  BOOST_CHECK_EQUAL(8, Packed<uint16_t>::size());
  BOOST_CHECK_EQUAL(4, Packed<int32_t>::size());
  BOOST_CHECK_EQUAL(4, Packed<uint32_t>::size());
  BOOST_CHECK_EQUAL(2, Packed<int64_t>::size());
  BOOST_CHECK_EQUAL(2, Packed<uint64_t>::size());
#endif
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


BOOST_FIXTURE_TEST_CASE(vector_add_float, PackedTest)
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

BOOST_FIXTURE_TEST_CASE(vector_add_equal_float, PackedTest)
{

  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadAligned(&v1[i]);
    packed_b.loadAligned(&v2[i]);

    packed_a += packed_b;
    packed_a.storeAligned(&v1[i]);

  }

  for (size_t i = max_vector; i < v1.size(); ++i)
  {
    v1[i] += v2[i];
  }

  BOOST_CHECK_EQUAL(v1[0], 0.f);
  BOOST_CHECK_EQUAL(v1[1], 4.f);
  BOOST_CHECK_EQUAL(v1[2], 5.f);
  BOOST_CHECK_EQUAL(v1[3], 7.f);
  BOOST_CHECK_EQUAL(v1[4], 10.f);
  BOOST_CHECK_EQUAL(v1[5], 17.f);
  BOOST_CHECK_EQUAL(v1[6], 4.f);
  BOOST_CHECK_EQUAL(v1[7], 5.f);
  BOOST_CHECK_EQUAL(v1[8], 13.f);
  BOOST_CHECK_EQUAL(v1[9], 6.0f);
  BOOST_CHECK_EQUAL(v1[10], 10.f);
  BOOST_CHECK_EQUAL(v1[11], 12.f);

}

BOOST_FIXTURE_TEST_CASE(vector_add_double, PackedTest)
{

  Vector<double> v3(v1d.size());

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadAligned(&v1d[i]);
    packed_b_d.loadAligned(&v2d[i]);

    Packed<double> packed_c = packed_a_d + packed_b_d;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1d.size(); ++i)
  {
    v3[i] = v1d[i] + v2d[i];
  }

  BOOST_CHECK_EQUAL(0., v3[0]);
  BOOST_CHECK_EQUAL(4., v3[1]);
  BOOST_CHECK_EQUAL(5., v3[2]);
  BOOST_CHECK_EQUAL(7., v3[3]);
  BOOST_CHECK_EQUAL(10., v3[4]);
  BOOST_CHECK_EQUAL(17., v3[5]);
  BOOST_CHECK_EQUAL(4., v3[6]);
  BOOST_CHECK_EQUAL(5., v3[7]);
  BOOST_CHECK_EQUAL(13., v3[8]);
  BOOST_CHECK_EQUAL(6., v3[9]);
  BOOST_CHECK_EQUAL(10., v3[10]);
  BOOST_CHECK_EQUAL(12., v3[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_add_equal_double, PackedTest)
{

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadAligned(&v1d[i]);
    packed_b_d.loadAligned(&v2d[i]);

    packed_a_d += packed_b_d;
    packed_a_d.storeAligned(&v1d[i]);

  }

  for (size_t i = max_vector; i < v1d.size(); ++i)
  {
    v1d[i] += v2d[i];
  }

  BOOST_CHECK_EQUAL(v1d[0], 0.);
  BOOST_CHECK_EQUAL(v1d[1], 4.);
  BOOST_CHECK_EQUAL(v1d[2], 5.);
  BOOST_CHECK_EQUAL(v1d[3], 7.);
  BOOST_CHECK_EQUAL(v1d[4], 10.);
  BOOST_CHECK_EQUAL(v1d[5], 17.);
  BOOST_CHECK_EQUAL(v1d[6], 4.);
  BOOST_CHECK_EQUAL(v1d[7], 5.);
  BOOST_CHECK_EQUAL(v1d[8], 13.);
  BOOST_CHECK_EQUAL(v1d[9], 6.);
  BOOST_CHECK_EQUAL(v1d[10], 10.);
  BOOST_CHECK_EQUAL(v1d[11], 12.);

}

BOOST_FIXTURE_TEST_CASE(vector_add_int32, PackedTest)
{

  Vector<int32_t> v3(v1i32.size());

  size_t max_vector = (v1i32.size() / packed_a_i32.size()) * packed_a_i32.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i32.size()) {

    packed_a_i32.loadAligned(&v1i32[i]);
    packed_b_i32.loadAligned(&v2i32[i]);

    Packed<int32_t> packed_c = packed_a_i32 + packed_b_i32;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i32.size(); ++i)
  {
    v3[i] = v1i32[i] + v2i32[i];
  }

  BOOST_CHECK_EQUAL(0, v3[0]);
  BOOST_CHECK_EQUAL(4, v3[1]);
  BOOST_CHECK_EQUAL(5, v3[2]);
  BOOST_CHECK_EQUAL(7, v3[3]);
  BOOST_CHECK_EQUAL(10, v3[4]);
  BOOST_CHECK_EQUAL(17, v3[5]);
  BOOST_CHECK_EQUAL(4, v3[6]);
  BOOST_CHECK_EQUAL(5, v3[7]);
  BOOST_CHECK_EQUAL(13, v3[8]);
  BOOST_CHECK_EQUAL(6, v3[9]);
  BOOST_CHECK_EQUAL(10, v3[10]);
  BOOST_CHECK_EQUAL(12, v3[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_add_equal_int32, PackedTest)
{

  size_t max_vector = (v1i32.size() / packed_a_i32.size()) * packed_a_i32.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i32.size()) {

    packed_a_i32.loadAligned(&v1i32[i]);
    packed_b_i32.loadAligned(&v2i32[i]);

    packed_a_i32 += packed_b_i32;
    packed_a_i32.storeAligned(&v1i32[i]);

  }

  for (size_t i = max_vector; i < v1i32.size(); ++i)
  {
    v1i32[i] += v2i32[i];
  }

  BOOST_CHECK_EQUAL(v1i32[0], 0);
  BOOST_CHECK_EQUAL(v1i32[1], 4);
  BOOST_CHECK_EQUAL(v1i32[2], 5);
  BOOST_CHECK_EQUAL(v1i32[3], 7);
  BOOST_CHECK_EQUAL(v1i32[4], 10);
  BOOST_CHECK_EQUAL(v1i32[5], 17);
  BOOST_CHECK_EQUAL(v1i32[6], 4);
  BOOST_CHECK_EQUAL(v1i32[7], 5);
  BOOST_CHECK_EQUAL(v1i32[8], 13);
  BOOST_CHECK_EQUAL(v1i32[9], 6);
  BOOST_CHECK_EQUAL(v1i32[10], 10);
  BOOST_CHECK_EQUAL(v1i32[11], 12);

}

BOOST_FIXTURE_TEST_CASE(vector_sub_float, PackedTest)
{

  Vector<float> v3(v1.size());

  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadAligned(&v1[i]);
    packed_b.loadAligned(&v2[i]);

    Packed<float> packed_c = packed_a - packed_b;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); ++i)
  {
    v3[i] = v1[i] - v2[i];
  }

  BOOST_CHECK_EQUAL(2.f, v3[0]);
  BOOST_CHECK_EQUAL(-4.f, v3[1]);
  BOOST_CHECK_EQUAL(1.f, v3[2]);
  BOOST_CHECK_EQUAL(3.f, v3[3]);
  BOOST_CHECK_EQUAL(6.f, v3[4]);
  BOOST_CHECK_EQUAL(1.f, v3[5]);
  BOOST_CHECK_EQUAL(0.f, v3[6]);
  BOOST_CHECK_EQUAL(-1.f, v3[7]);
  BOOST_CHECK_EQUAL(-3.f, v3[8]);
  BOOST_CHECK_EQUAL(-2.f, v3[9]);
  BOOST_CHECK_EQUAL(8.f, v3[10]);
  BOOST_CHECK_EQUAL(-2.f, v3[11]);
}

BOOST_FIXTURE_TEST_CASE(vector_sub_equal_float, PackedTest)
{

  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadAligned(&v1[i]);
    packed_b.loadAligned(&v2[i]);

    packed_a -= packed_b;
    
    packed_a.storeAligned(&v1[i]);
  }

  for (size_t i = max_vector; i < v1.size(); ++i)
  {
    v1[i] -= v2[i];
  }

  BOOST_CHECK_EQUAL(2.f, v1[0]);
  BOOST_CHECK_EQUAL(-4.f, v1[1]);
  BOOST_CHECK_EQUAL(1.f, v1[2]);
  BOOST_CHECK_EQUAL(3.f, v1[3]);
  BOOST_CHECK_EQUAL(6.f, v1[4]);
  BOOST_CHECK_EQUAL(1.f, v1[5]);
  BOOST_CHECK_EQUAL(0.f, v1[6]);
  BOOST_CHECK_EQUAL(-1.f, v1[7]);
  BOOST_CHECK_EQUAL(-3.f, v1[8]);
  BOOST_CHECK_EQUAL(-2.f, v1[9]);
  BOOST_CHECK_EQUAL(8.f, v1[10]);
  BOOST_CHECK_EQUAL(-2.f, v1[11]);
}

BOOST_FIXTURE_TEST_CASE(vector_sub_double, PackedTest)
{

  Vector<double> v3(v1d.size());

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadAligned(&v1d[i]);
    packed_b_d.loadAligned(&v2d[i]);

    Packed<double> packed_c = packed_a_d - packed_b_d;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1d.size(); ++i)
  {
    v3[i] = v1d[i] - v2d[i];
  }

  BOOST_CHECK_EQUAL(2., v3[0]);
  BOOST_CHECK_EQUAL(-4., v3[1]);
  BOOST_CHECK_EQUAL(1., v3[2]);
  BOOST_CHECK_EQUAL(3., v3[3]);
  BOOST_CHECK_EQUAL(6., v3[4]);
  BOOST_CHECK_EQUAL(1., v3[5]);
  BOOST_CHECK_EQUAL(0., v3[6]);
  BOOST_CHECK_EQUAL(-1., v3[7]);
  BOOST_CHECK_EQUAL(-3., v3[8]);
  BOOST_CHECK_EQUAL(-2., v3[9]);
  BOOST_CHECK_EQUAL(8., v3[10]);
  BOOST_CHECK_EQUAL(-2., v3[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_sub_equal_double, PackedTest)
{

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadAligned(&v1d[i]);
    packed_b_d.loadAligned(&v2d[i]);

    packed_a_d -= packed_b_d;
    packed_a_d.storeAligned(&v1d[i]);

  }

  for (size_t i = max_vector; i < v1d.size(); ++i)
  {
    v1d[i] -= v2d[i];
  }

  BOOST_CHECK_EQUAL(2., v1d[0]);
  BOOST_CHECK_EQUAL(-4., v1d[1]);
  BOOST_CHECK_EQUAL(1., v1d[2]);
  BOOST_CHECK_EQUAL(3., v1d[3]);
  BOOST_CHECK_EQUAL(6., v1d[4]);
  BOOST_CHECK_EQUAL(1., v1d[5]);
  BOOST_CHECK_EQUAL(0., v1d[6]);
  BOOST_CHECK_EQUAL(-1., v1d[7]);
  BOOST_CHECK_EQUAL(-3., v1d[8]);
  BOOST_CHECK_EQUAL(-2., v1d[9]);
  BOOST_CHECK_EQUAL(8., v1d[10]);
  BOOST_CHECK_EQUAL(-2., v1d[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_multiplication_float, PackedTest)
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

BOOST_FIXTURE_TEST_CASE(vector_multiplication_double, PackedTest)
{

  Vector<double> v3(v1d.size());

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadAligned(&v1d[i]);
    packed_b_d.loadAligned(&v2d[i]);

    Packed<double> packed_c = packed_a_d * packed_b_d;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1d.size(); ++i)
  {
    v3[i] = v1d[i] * v2d[i];
  }

  BOOST_CHECK_EQUAL(-1., v3[0]);
  BOOST_CHECK_EQUAL(0., v3[1]);
  BOOST_CHECK_EQUAL(6., v3[2]);
  BOOST_CHECK_EQUAL(10., v3[3]);
  BOOST_CHECK_EQUAL(16., v3[4]);
  BOOST_CHECK_EQUAL(72., v3[5]);
  BOOST_CHECK_EQUAL(4., v3[6]);
  BOOST_CHECK_EQUAL(6., v3[7]);
  BOOST_CHECK_EQUAL(40., v3[8]);
  BOOST_CHECK_EQUAL(8., v3[9]);
  BOOST_CHECK_EQUAL(9., v3[10]);
  BOOST_CHECK_EQUAL(35., v3[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_div_float, PackedTest)
{

  Vector<float> v3(v1.size());
  v3 = v1 / v2;
  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadAligned(&v1[i]);
    packed_b.loadAligned(&v2[i]);

    Packed<float> packed_c = packed_a / packed_b;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); ++i)
  {
    v3[i] = v1[i] / v2[i];
  }

  BOOST_CHECK_EQUAL(-1.f, v3[0]);
  BOOST_CHECK_EQUAL(0.f, v3[1]);
  BOOST_CHECK_EQUAL(1.5f, v3[2]);
  BOOST_CHECK_EQUAL(2.5f, v3[3]);
  BOOST_CHECK_EQUAL(4.f, v3[4]);
  BOOST_CHECK_EQUAL(1.125f, v3[5]);
  BOOST_CHECK_EQUAL(1.f, v3[6]);
  BOOST_CHECK_CLOSE(0.66666f, v3[7], 0.01);
  BOOST_CHECK_EQUAL(0.625f, v3[8]);
  BOOST_CHECK_EQUAL(0.5f, v3[9]);
  BOOST_CHECK_EQUAL(9.f, v3[10]);
  BOOST_CHECK_CLOSE(0.714285f, v3[11], 0.01);

}

BOOST_FIXTURE_TEST_CASE(vector_div_double, PackedTest)
{

  Vector<double> v3(v1d.size());

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadAligned(&v1d[i]);
    packed_b_d.loadAligned(&v2d[i]);

    Packed<double> packed_c = packed_a_d / packed_b_d;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1d.size(); ++i)
  {
    v3[i] = v1d[i] / v2d[i];
  }

  BOOST_CHECK_EQUAL(-1., v3[0]);
  BOOST_CHECK_EQUAL(0., v3[1]);
  BOOST_CHECK_EQUAL(1.5, v3[2]);
  BOOST_CHECK_EQUAL(2.5, v3[3]);
  BOOST_CHECK_EQUAL(4., v3[4]);
  BOOST_CHECK_EQUAL(1.125, v3[5]);
  BOOST_CHECK_EQUAL(1., v3[6]);
  BOOST_CHECK_CLOSE(0.66666, v3[7], 0.01);
  BOOST_CHECK_EQUAL(0.625, v3[8]);
  BOOST_CHECK_EQUAL(0.5, v3[9]);
  BOOST_CHECK_EQUAL(9., v3[10]);
  BOOST_CHECK_CLOSE(0.714285, v3[11], 0.01);

}

BOOST_AUTO_TEST_SUITE_END()