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

#define BOOST_TEST_MODULE Tidop simd test
#include <boost/test/unit_test.hpp>
#include <tidop/math/simd.h>
#include <tidop/math/algebra/vector.h>
#include <tidop/math/algebra/matrix.h>

#ifdef TL_HAVE_SIMD_INTRINSICS

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
    v1ui32 = {45, 65, 75, 23, 574, 23, 23, 426, 2362, 346, 235, 755};
    v2ui32 = {962, 59, 98, 854, 175, 45, 2, 78, 785, 786, 298, 725};
    v1i8 = {1, 0, 3, 5, 8, 9, 2, 2, 5, 2, 9, 5, 9, 7, 2, 1, 1, 0, 3, 5, 8, 9, 2, 2, 5, 2, 9, 5, 9, 7, 2, 1};
    v2i8 = {-1, 4, 2, 2, 2, 8, 2, 3, 8, 4, 1, 7, 0, 3, -2, 2, -1, 4, 2, 2, 2, 8, 2, 3, 8, 4, 1, 7, 0, 3, -2, 2};

    matrix1 = { 0.81, 7.45, 1.17, 3.44, 4.13, 5.67, 4.57, 2.03, 5.53, 5.25,
                6.69, 7.87, 1.70, 4.32, 6.33, 0.67, 8.99, 7.63, 7.08, 7.24,
                6.39, 3.54, 6.47, 7.15, 0.37, 5.52, 3.21, 8.52, 3.20, 5.83,
                7.86, 5.01, 5.82, 0.94, 8.94, 6.53, 2.04, 5.91, 5.94, 5.18,
                0.11, 3.99, 1.09, 2.89, 5.71, 6.51, 6.84, 4.33, 3.46, 8.86,
                0.78, 2.50, 2.20, 5.76, 4.13, 2.64, 6.41, 4.38, 3.75, 8.01,
                4.52, 0.05, 1.16, 2.97, 2.71, 1.97, 8.81, 3.89, 2.37, 3.86,
                6.65, 2.05, 8.86, 3.24, 0.83, 7.56, 8.70, 6.93, 0.93, 0.79,
                2.09, 4.23, 6.07, 8.07, 7.64, 0.80, 7.43, 0.15, 0.07, 7.02,
                2.24, 3.95, 5.79, 3.45, 3.43, 4.62, 5.88, 2.01, 3.04, 3.06 };

    matrix2 = { 1.48, 1.19, 1.71, 7.35, 2.66, 8.25, 8.28, 3.86, 1.82, 2.69,
                2.19, 5.53, 2.06, 6.51, 6.39, 1.18, 8.83, 3.41, 6.19, 5.41,
                8.52, 4.30, 0.60, 0.11, 7.88, 2.41, 3.63, 7.20, 5.96, 5.61,
                6.35, 0.27, 5.50, 4.55, 2.46, 3.81, 3.20, 6.69, 5.08, 3.68,
                8.14, 1.13, 5.17, 0.64, 4.79, 6.49, 4.00, 1.22, 6.89, 0.20,
                2.10, 6.48, 5.23, 1.51, 2.13, 2.42, 7.35, 7.44, 0.24, 3.89,
                6.93, 8.66, 0.94, 3.01, 3.90, 7.55, 0.08, 8.03, 3.37, 7.25,
                2.42, 6.20, 3.22, 8.21, 7.63, 2.82, 0.79, 7.68, 7.07, 3.24,
                2.89, 3.61, 5.30, 2.00, 4.53, 2.41, 7.54, 0.27, 3.02, 4.24,
                0.58, 0.69, 5.86, 3.62, 5.66, 7.92, 2.53, 3.01, 6.05, 0.82 };
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
  Packed<uint32_t> packed_a_ui32;
  Packed<uint32_t> packed_b_ui32;
  Packed<int8_t> packed_a_i8;
  Packed<int8_t> packed_b_i8;

  Vector<float> v1;
  Vector<float> v2;
  Vector<double> v1d;
  Vector<double> v2d;
  Vector<int32_t> v1i32;
  Vector<int32_t> v2i32;
  Vector<uint32_t> v1ui32;
  Vector<uint32_t> v2ui32;
  Vector<int8_t> v1i8;
  Vector<int8_t> v2i8;

  Matrix<double, 10, 10> matrix1;
  Matrix<double, 10, 10> matrix2;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, PackedTest)
{

}

BOOST_FIXTURE_TEST_CASE(size, PackedTest)
{
#ifdef TL_HAVE_AVX
  BOOST_CHECK_EQUAL(4, Packed<double>::size());
  BOOST_CHECK_EQUAL(8, Packed<float>::size());
#elif defined TL_HAVE_SSE2
  BOOST_CHECK_EQUAL(2, Packed<double>::size());
  BOOST_CHECK_EQUAL(4, Packed<float>::size());
#endif

#ifdef TL_HAVE_AVX2
  BOOST_CHECK_EQUAL(32, Packed<int8_t>::size());
  BOOST_CHECK_EQUAL(32, Packed<uint8_t>::size());
  BOOST_CHECK_EQUAL(16, Packed<int16_t>::size());
  BOOST_CHECK_EQUAL(16, Packed<uint16_t>::size());
  BOOST_CHECK_EQUAL(8, Packed<int32_t>::size());
  BOOST_CHECK_EQUAL(8, Packed<uint32_t>::size());
  BOOST_CHECK_EQUAL(4, Packed<int64_t>::size());
  BOOST_CHECK_EQUAL(4, Packed<uint64_t>::size());
#else
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


BOOST_FIXTURE_TEST_CASE(load_store_aligned_float, PackedTest)
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

BOOST_FIXTURE_TEST_CASE(load_store_aligned_double, PackedTest)
{
  Vector<double> v3(v1d.size());

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadAligned(&v1d[i]);

    packed_a_d.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); i++)   {
    v3[i] = v1d[i];
  }

  for (size_t i = 0; i < v1d.size(); i++) {
    BOOST_CHECK_EQUAL(v1d[i], v3[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(load_store_aligned_int32, PackedTest)
{
  Vector<int32_t> v3(v1i32.size());

  size_t max_vector = (v1i32.size() / packed_a_i32.size()) * packed_a_i32.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i32.size()) {

    packed_a_i32.loadAligned(&v1i32[i]);

    packed_a_i32.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i32.size(); i++) {
    v3[i] = v1i32[i];
  }

  for (size_t i = 0; i < v1i32.size(); i++) {
    BOOST_CHECK_EQUAL(v1i32[i], v3[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(load_store_aligned_int8, PackedTest)
{
  Vector<int8_t> v3(v1i8.size());

  size_t max_vector = (v1i8.size() / packed_a_i8.size()) * packed_a_i8.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i8.size()) {

    packed_a_i8.loadAligned(&v1i8[i]);

    packed_a_i8.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i8.size(); i++) {
    v3[i] = v1i8[i];
  }

  for (size_t i = 0; i < v1i8.size(); i++) {
    BOOST_CHECK_EQUAL(v1i8[i], v3[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(load_store_unaligned_float, PackedTest)
{
  Vector<float> v3(v1.size());

  size_t max_vector = (v1.size() / packed_a.size()) * packed_a.size();
  for (size_t i = 0; i < max_vector; i += packed_a.size()) {

    packed_a.loadUnaligned(&v1[i]);

    packed_a.storeUnaligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); i++)   {
    v3[i] = v1[i];
  }

  for (size_t i = 0; i < v1.size(); i++) {
    BOOST_CHECK_EQUAL(v1[i], v3[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(load_store_unaligned_double, PackedTest)
{
  Vector<double> v3(v1d.size());

  size_t max_vector = (v1d.size() / packed_a_d.size()) * packed_a_d.size();
  for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {

    packed_a_d.loadUnaligned(&v1d[i]);

    packed_a_d.storeUnaligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1.size(); i++) {
    v3[i] = v1d[i];
  }

  for (size_t i = 0; i < v1d.size(); i++) {
    BOOST_CHECK_EQUAL(v1d[i], v3[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(load_store_unaligned_int32, PackedTest)
{
  Vector<int32_t> v3(v1i32.size());

  size_t max_vector = (v1i32.size() / packed_a_i32.size()) * packed_a_i32.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i32.size()) {

    packed_a_i32.loadUnaligned(&v1i32[i]);

    packed_a_i32.storeUnaligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i32.size(); i++) {
    v3[i] = v1i32[i];
  }

  for (size_t i = 0; i < v1i32.size(); i++) {
    BOOST_CHECK_EQUAL(v1i32[i], v3[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(load_store_unaligned_int8, PackedTest)
{
  Vector<int8_t> v3(v1i8.size());

  size_t max_vector = (v1i8.size() / packed_a_i8.size()) * packed_a_i8.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i8.size()) {

    packed_a_i8.loadUnaligned(&v1i8[i]);

    packed_a_i8.storeUnaligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i8.size(); i++) {
    v3[i] = v1i8[i];
  }

  for (size_t i = 0; i < v1i8.size(); i++) {
    BOOST_CHECK_EQUAL(v1i8[i], v3[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(set_scalar, PackedTest)
{
  {
    Packed<float> packed_float;
    packed_float.setScalar(2.5f);
    Vector<float> v(packed_float.size());
    packed_float.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_float.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], 2.5f);
    }
  }

  {
    Packed<double> packed_double;
    packed_double.setScalar(2.5);
    Vector<double> v(packed_double.size());
    packed_double.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_double.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], 2.5);
    }
  }

  {
    Packed<int64_t> packed_i64;
    packed_i64.setScalar(-234234);
    Vector<int64_t> v(packed_i64.size());
    packed_i64.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_i64.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], -234234);
    }
  }

  {
    Packed<uint64_t> packed_ui64;
    packed_ui64.setScalar(45735);
    Vector<uint64_t> v(packed_ui64.size());
    packed_ui64.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_ui64.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], 45735);
    }
  }

  {
    Packed<int32_t> packed_i32;
    packed_i32.setScalar(-24577);
    Vector<int32_t> v(packed_i32.size());
    packed_i32.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_i32.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], -24577);
    }
  }

  {
    Packed<int32_t> packed_ui32;
    packed_ui32.setScalar(47268);
    Vector<int32_t> v(packed_ui32.size());
    packed_ui32.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_ui32.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], 47268);
    }
  }

  {
    Packed<int32_t> packed_i16;
    packed_i16.setScalar(-2356);
    Vector<int32_t> v(packed_i16.size());
    packed_i16.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_i16.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], -2356);
    }
  }

  {
    Packed<uint16_t> packed_ui16;
    packed_ui16.setScalar(23456);
    Vector<uint16_t> v(packed_ui16.size());
    packed_ui16.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_ui16.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], 23456);
    }
  }

  {
    Packed<int8_t> packed_i8;
    packed_i8.setScalar(-124);
    Vector<int8_t> v(packed_i8.size());
    packed_i8.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_i8.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], -124);
    }
  }

  {
    Packed<uint8_t> packed_ui8;
    packed_ui8.setScalar(250);
    Vector<uint8_t> v(packed_ui8.size());
    packed_ui8.storeUnaligned(&v[0]);

    for(size_t i = 0; i < packed_ui8.size(); i++) {
      BOOST_CHECK_EQUAL(v[i], 250);
    }
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

BOOST_FIXTURE_TEST_CASE(vector_add_uint32, PackedTest)
{

  Vector<uint32_t> v3(v1ui32.size());

  size_t max_vector = (v1ui32.size() / packed_a_ui32.size()) * packed_a_ui32.size();
  for(size_t i = 0; i < max_vector; i += packed_a_ui32.size()) {

    packed_a_ui32.loadAligned(&v1ui32[i]);
    packed_b_ui32.loadAligned(&v2ui32[i]);

    Packed<uint32_t> packed_c = packed_a_ui32 + packed_b_ui32;
    packed_c.storeAligned(&v3[i]);

  }

  for(size_t i = max_vector; i < v1ui32.size(); ++i) {
    v3[i] = v1ui32[i] + v2ui32[i];
  }
  v1ui32 = {45, 65, 75, 23, 574, 23, 23, 426, 2362, 346, 235, 755};
  v2ui32 = {962, 59, 98, 854, 175, 45, 2, 78, 785, 786, 298, 725};
  BOOST_CHECK_EQUAL(1007, v3[0]);
  BOOST_CHECK_EQUAL(124, v3[1]);
  BOOST_CHECK_EQUAL(173, v3[2]);
  BOOST_CHECK_EQUAL(877, v3[3]);
  BOOST_CHECK_EQUAL(749, v3[4]);
  BOOST_CHECK_EQUAL(68, v3[5]);
  BOOST_CHECK_EQUAL(25, v3[6]);
  BOOST_CHECK_EQUAL(504, v3[7]);
  BOOST_CHECK_EQUAL(3147, v3[8]);
  BOOST_CHECK_EQUAL(1132, v3[9]);
  BOOST_CHECK_EQUAL(533, v3[10]);
  BOOST_CHECK_EQUAL(1480, v3[11]);

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

BOOST_FIXTURE_TEST_CASE(vector_add_int8, PackedTest)
{

  Vector<int8_t> v3(v1i8.size());

  size_t max_vector = (v1i8.size() / packed_a_i8.size()) * packed_a_i8.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i8.size()) {

    packed_a_i8.loadAligned(&v1i8[i]);
    packed_b_i8.loadAligned(&v2i8[i]);

    Packed<int8_t> packed_c = packed_a_i8 + packed_b_i8;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i8.size(); ++i)   {
    v3[i] = v1i8[i] + v2i8[i];
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
  BOOST_CHECK_EQUAL(9, v3[12]);
  BOOST_CHECK_EQUAL(10, v3[13]);
  BOOST_CHECK_EQUAL(0, v3[14]);
  BOOST_CHECK_EQUAL(3, v3[15]);
}

BOOST_FIXTURE_TEST_CASE(vector_add_equal_int8, PackedTest)
{

  size_t max_vector = (v1i8.size() / packed_a_i8.size()) * packed_a_i8.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i8.size()) {

    packed_a_i8.loadAligned(&v1i8[i]);
    packed_b_i8.loadAligned(&v2i8[i]);

    packed_a_i8 += packed_b_i8;
    packed_a_i8.storeAligned(&v1i8[i]);

  }

  for (size_t i = max_vector; i < v1i8.size(); ++i)   {
    v1i8[i] += v2i8[i];
  }

  BOOST_CHECK_EQUAL(v1i8[0], 0);
  BOOST_CHECK_EQUAL(v1i8[1], 4);
  BOOST_CHECK_EQUAL(v1i8[2], 5);
  BOOST_CHECK_EQUAL(v1i8[3], 7);
  BOOST_CHECK_EQUAL(v1i8[4], 10);
  BOOST_CHECK_EQUAL(v1i8[5], 17);
  BOOST_CHECK_EQUAL(v1i8[6], 4);
  BOOST_CHECK_EQUAL(v1i8[7], 5);
  BOOST_CHECK_EQUAL(v1i8[8], 13);
  BOOST_CHECK_EQUAL(v1i8[9], 6);
  BOOST_CHECK_EQUAL(v1i8[10], 10);
  BOOST_CHECK_EQUAL(v1i8[11], 12);
  BOOST_CHECK_EQUAL(v1i8[12], 9);
  BOOST_CHECK_EQUAL(v1i8[13], 10);
  BOOST_CHECK_EQUAL(v1i8[14], 0);
  BOOST_CHECK_EQUAL(v1i8[15], 3);
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

BOOST_FIXTURE_TEST_CASE(vector_sub_int32, PackedTest)
{

  Vector<int32_t> v3(v1i32.size());

  size_t max_vector = (v1i32.size() / packed_a_i32.size()) * packed_a_i32.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i32.size()) {

    packed_a_i32.loadAligned(&v1i32[i]);
    packed_b_i32.loadAligned(&v2i32[i]);

    Packed<int32_t> packed_c = packed_a_i32 - packed_b_i32;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i32.size(); ++i)   {
    v3[i] = v1i32[i] - v2i32[i];
  }

  BOOST_CHECK_EQUAL(2, v3[0]);
  BOOST_CHECK_EQUAL(-4, v3[1]);
  BOOST_CHECK_EQUAL(1, v3[2]);
  BOOST_CHECK_EQUAL(3, v3[3]);
  BOOST_CHECK_EQUAL(6, v3[4]);
  BOOST_CHECK_EQUAL(1, v3[5]);
  BOOST_CHECK_EQUAL(0, v3[6]);
  BOOST_CHECK_EQUAL(-1, v3[7]);
  BOOST_CHECK_EQUAL(-3, v3[8]);
  BOOST_CHECK_EQUAL(-2, v3[9]);
  BOOST_CHECK_EQUAL(8, v3[10]);
  BOOST_CHECK_EQUAL(-2, v3[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_sub_equal_int32, PackedTest)
{

  size_t max_vector = (v1i32.size() / packed_a_i32.size()) * packed_a_i32.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i32.size()) {

    packed_a_i32.loadAligned(&v1i32[i]);
    packed_b_i32.loadAligned(&v2i32[i]);

    packed_a_i32 -= packed_b_i32;
    packed_a_i32.storeAligned(&v1i32[i]);

  }

  for (size_t i = max_vector; i < v1i32.size(); ++i)   {
    v1i32[i] -= v2i32[i];
  }

  BOOST_CHECK_EQUAL(2, v1i32[0]);
  BOOST_CHECK_EQUAL(-4, v1i32[1]);
  BOOST_CHECK_EQUAL(1, v1i32[2]);
  BOOST_CHECK_EQUAL(3, v1i32[3]);
  BOOST_CHECK_EQUAL(6, v1i32[4]);
  BOOST_CHECK_EQUAL(1, v1i32[5]);
  BOOST_CHECK_EQUAL(0, v1i32[6]);
  BOOST_CHECK_EQUAL(-1, v1i32[7]);
  BOOST_CHECK_EQUAL(-3, v1i32[8]);
  BOOST_CHECK_EQUAL(-2, v1i32[9]);
  BOOST_CHECK_EQUAL(8, v1i32[10]);
  BOOST_CHECK_EQUAL(-2, v1i32[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_sub_int8, PackedTest)
{

  Vector<int8_t> v3(v1i8.size());

  size_t max_vector = (v1i8.size() / packed_a_i8.size()) * packed_a_i8.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i8.size()) {

    packed_a_i8.loadAligned(&v1i8[i]);
    packed_b_i8.loadAligned(&v2i8[i]);

    Packed<int8_t> packed_c = packed_a_i8 - packed_b_i8;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i8.size(); ++i) {
    v3[i] = v1i8[i] - v2i8[i];
  }

  BOOST_CHECK_EQUAL(2, v3[0]);
  BOOST_CHECK_EQUAL(-4, v3[1]);
  BOOST_CHECK_EQUAL(1, v3[2]);
  BOOST_CHECK_EQUAL(3, v3[3]);
  BOOST_CHECK_EQUAL(6, v3[4]);
  BOOST_CHECK_EQUAL(1, v3[5]);
  BOOST_CHECK_EQUAL(0, v3[6]);
  BOOST_CHECK_EQUAL(-1, v3[7]);
  BOOST_CHECK_EQUAL(-3, v3[8]);
  BOOST_CHECK_EQUAL(-2, v3[9]);
  BOOST_CHECK_EQUAL(8, v3[10]);
  BOOST_CHECK_EQUAL(-2, v3[11]);
  BOOST_CHECK_EQUAL(9, v3[12]);
  BOOST_CHECK_EQUAL(4, v3[13]);
  BOOST_CHECK_EQUAL(4, v3[14]);
  BOOST_CHECK_EQUAL(-1, v3[15]);
}

BOOST_FIXTURE_TEST_CASE(vector_sub_equal_int8, PackedTest)
{

  size_t max_vector = (v1i8.size() / packed_a_i8.size()) * packed_a_i8.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i8.size()) {

    packed_a_i8.loadAligned(&v1i8[i]);
    packed_b_i8.loadAligned(&v2i8[i]);

    packed_a_i8 -= packed_b_i8;
    packed_a_i8.storeAligned(&v1i8[i]);

  }

  for (size_t i = max_vector; i < v1i8.size(); ++i) {
    v1i8[i] -= v2i8[i];
  }

  BOOST_CHECK_EQUAL(2, v1i8[0]);
  BOOST_CHECK_EQUAL(-4, v1i8[1]);
  BOOST_CHECK_EQUAL(1, v1i8[2]);
  BOOST_CHECK_EQUAL(3, v1i8[3]);
  BOOST_CHECK_EQUAL(6, v1i8[4]);
  BOOST_CHECK_EQUAL(1, v1i8[5]);
  BOOST_CHECK_EQUAL(0, v1i8[6]);
  BOOST_CHECK_EQUAL(-1, v1i8[7]);
  BOOST_CHECK_EQUAL(-3, v1i8[8]);
  BOOST_CHECK_EQUAL(-2, v1i8[9]);
  BOOST_CHECK_EQUAL(8, v1i8[10]);
  BOOST_CHECK_EQUAL(-2, v1i8[11]);
  BOOST_CHECK_EQUAL(9, v1i8[12]);
  BOOST_CHECK_EQUAL(4, v1i8[13]);
  BOOST_CHECK_EQUAL(4, v1i8[14]);
  BOOST_CHECK_EQUAL(-1, v1i8[15]);
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

BOOST_FIXTURE_TEST_CASE(vector_multiplication_int32, PackedTest)
{

  Vector<int32_t> v3(v1i32.size());

  size_t max_vector = (v1i32.size() / packed_a_i32.size()) * packed_a_i32.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i32.size()) {

    packed_a_i32.loadAligned(&v1i32[i]);
    packed_b_i32.loadAligned(&v2i32[i]);

    Packed<int32_t> packed_c = packed_a_i32 * packed_b_i32;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i32.size(); ++i)   {
    v3[i] = v1i32[i] * v2i32[i];
  }

  BOOST_CHECK_EQUAL(-1, v3[0]);
  BOOST_CHECK_EQUAL(0, v3[1]);
  BOOST_CHECK_EQUAL(6, v3[2]);
  BOOST_CHECK_EQUAL(10, v3[3]);
  BOOST_CHECK_EQUAL(16, v3[4]);
  BOOST_CHECK_EQUAL(72, v3[5]);
  BOOST_CHECK_EQUAL(4, v3[6]);
  BOOST_CHECK_EQUAL(6, v3[7]);
  BOOST_CHECK_EQUAL(40, v3[8]);
  BOOST_CHECK_EQUAL(8, v3[9]);
  BOOST_CHECK_EQUAL(9, v3[10]);
  BOOST_CHECK_EQUAL(35, v3[11]);

}

BOOST_FIXTURE_TEST_CASE(vector_multiplication_int8, PackedTest)
{

  Vector<int8_t> v3(v1i8.size());

  size_t max_vector = (v1i8.size() / packed_a_i8.size()) * packed_a_i8.size();
  for (size_t i = 0; i < max_vector; i += packed_a_i8.size()) {

    packed_a_i8.loadAligned(&v1i8[i]);
    packed_b_i8.loadAligned(&v2i8[i]);

    Packed<int8_t> packed_c = packed_a_i8 * packed_b_i8;
    packed_c.storeAligned(&v3[i]);

  }

  for (size_t i = max_vector; i < v1i8.size(); ++i) {
    v3[i] = v1i8[i] * v2i8[i];
  }

  BOOST_CHECK_EQUAL(-1, v3[0]);
  BOOST_CHECK_EQUAL(0, v3[1]);
  BOOST_CHECK_EQUAL(6, v3[2]);
  BOOST_CHECK_EQUAL(10, v3[3]);
  BOOST_CHECK_EQUAL(16, v3[4]);
  BOOST_CHECK_EQUAL(72, v3[5]);
  BOOST_CHECK_EQUAL(4, v3[6]);
  BOOST_CHECK_EQUAL(6, v3[7]);
  BOOST_CHECK_EQUAL(40, v3[8]);
  BOOST_CHECK_EQUAL(8, v3[9]);
  BOOST_CHECK_EQUAL(9, v3[10]);
  BOOST_CHECK_EQUAL(35, v3[11]);
  BOOST_CHECK_EQUAL(0, v3[12]);
  BOOST_CHECK_EQUAL(21, v3[13]);
  BOOST_CHECK_EQUAL(-4, v3[14]);
  BOOST_CHECK_EQUAL(2, v3[15]);
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


BOOST_FIXTURE_TEST_CASE(matrix_mul_double, PackedTest)
{

  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();

  size_t max_vector = (matrix2.cols() / packed_b_d.size()) * packed_b_d.size();
  Packed<double> packed_c;

  for (size_t r = 0; r < matrix1.rows(); r++) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a = matrix1[r][i];

      for (size_t c = 0; c < max_vector; c += packed_b_d.size()) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);
        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += a * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a * matrix2(i, c);
      }

    }
  }

  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);


}


BOOST_FIXTURE_TEST_CASE(matrix_mul_double_2, PackedTest)
{

  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();

  size_t max_vector = (matrix2.cols() / packed_b_d.size()) * packed_b_d.size();
  Packed<double> packed_c;

  constexpr size_t step = Packed<double>::size();

  int iter = (9 / 2) * 2;

  for (size_t r = 0; r < iter/*matrix1.rows()*/; r += 2) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a1 = matrix1[r][i];
      double a2 = matrix1[r + 1][i];

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += a1 * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);

        packed_c.loadUnaligned(&mat[r + 1][c]);
        packed_c += a2 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 1][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a1 * matrix2(i, c);
        mat(r + 1, c) += a2 * matrix2(i, c);
      }

    }
  }

  for (size_t r = iter; r < matrix1.rows(); r++) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a = matrix1[r][i];

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += a * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a * matrix2(i, c);
      }

    }

  }


  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);


}

BOOST_FIXTURE_TEST_CASE(matrix_mul_double_3, PackedTest)
{

  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();

  size_t max_vector = (matrix2.cols() / packed_b_d.size()) * packed_b_d.size();
  Packed<double> packed_c;


  for (size_t r = 0; r < matrix1.rows(); r++) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a = matrix1[r][i];
      Packed<double> packed_a(a);

      for (size_t c = 0; c < max_vector; c += packed_b_d.size()) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);
        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a * matrix2(i, c);
      }

    }
  }

  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);


}

BOOST_FIXTURE_TEST_CASE(matrix_mul_intri_block_2, PackedTest)
{

  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();

  size_t max_vector = (matrix2.cols() / packed_b_d.size()) * packed_b_d.size();
  Packed<double> packed_c;

  constexpr size_t step = Packed<double>::size();

  size_t iter = (matrix1.rows() / 2) * 2;


  for (size_t r = 0; r < iter; r += 2) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a1 = matrix1[r][i];
      Packed<double> packed_a1(a1);
      double a2 = matrix1[r + 1][i];
      Packed<double> packed_a2(a2);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a1 * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);

        packed_c.loadUnaligned(&mat[r + 1][c]);
        packed_c += packed_a2 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 1][c]);

      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a1 * matrix2(i, c);
        mat(r + 1, c) += a2 * matrix2(i, c);
      }

    }
  }

  for (size_t r = iter; r < matrix1.rows(); r++) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      Packed<double> packed_a(matrix1[r][i]);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += matrix1[r][i] * matrix2(i, c);
      }

    }
  }

  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);


}


BOOST_FIXTURE_TEST_CASE(matrix_mul_intri_block_4, PackedTest)
{

  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();

  size_t max_vector = (matrix2.cols() / packed_b_d.size()) * packed_b_d.size();
  Packed<double> packed_c;

  constexpr size_t step = Packed<double>::size();

  size_t iter = (matrix1.rows() / 4) * 4;


  for (size_t r = 0; r < iter; r += 4) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a1 = matrix1[r][i];
      Packed<double> packed_a1(a1);
      double a2 = matrix1[r + 1][i];
      Packed<double> packed_a2(a2);
      double a3 = matrix1[r + 2][i];
      Packed<double> packed_a3(a3);
      double a4 = matrix1[r + 3][i];
      Packed<double> packed_a4(a4);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a1 * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);

        packed_c.loadUnaligned(&mat[r + 1][c]);
        packed_c += packed_a2 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 1][c]);

        packed_c.loadUnaligned(&mat[r + 2][c]);
        packed_c += packed_a3 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 2][c]);

        packed_c.loadUnaligned(&mat[r + 3][c]);
        packed_c += packed_a4 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 3][c]);

      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a1 * matrix2(i, c);
        mat(r + 1, c) += a2 * matrix2(i, c);
        mat(r + 2, c) += a3 * matrix2(i, c);
        mat(r + 3, c) += a4 * matrix2(i, c);
      }

    }
  }

  for (size_t r = iter; r < matrix1.rows(); r++) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      Packed<double> packed_a(matrix1[r][i]);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += matrix1[r][i] * matrix2(i, c);
      }

    }
  }

  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);


}


BOOST_FIXTURE_TEST_CASE(tl_multi_matrix_intri_block_8, PackedTest)
{

  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();

  size_t max_vector = (matrix2.cols() / packed_b_d.size()) * packed_b_d.size();
  Packed<double> packed_c;

  constexpr size_t step = Packed<double>::size();

  size_t iter = (matrix1.rows() / 8) * 8;


  for (size_t r = 0; r < iter; r += 8) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a1 = matrix1[r][i];
      Packed<double> packed_a1(a1);
      double a2 = matrix1[r+1][i];
      Packed<double> packed_a2(a2);
      double a3 = matrix1[r + 2][i];
      Packed<double> packed_a3(a3);
      double a4 = matrix1[r + 3][i];
      Packed<double> packed_a4(a4);
      double a5 = matrix1[r + 4][i];
      Packed<double> packed_a5(a5);
      double a6 = matrix1[r + 5][i];
      Packed<double> packed_a6(a6);
      double a7 = matrix1[r + 6][i];
      Packed<double> packed_a7(a7);
      double a8 = matrix1[r + 7][i];
      Packed<double> packed_a8(a8);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a1 * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);

        packed_c.loadUnaligned(&mat[r+1][c]);
        packed_c += packed_a2 * packed_b_d;
        packed_c.storeUnaligned(&mat[r+1][c]);

        packed_c.loadUnaligned(&mat[r + 2][c]);
        packed_c += packed_a3 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 2][c]);

        packed_c.loadUnaligned(&mat[r + 3][c]);
        packed_c += packed_a4 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 3][c]);

        packed_c.loadUnaligned(&mat[r + 4][c]);
        packed_c += packed_a5 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 4][c]);

        packed_c.loadUnaligned(&mat[r + 5][c]);
        packed_c += packed_a6 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 5][c]);

        packed_c.loadUnaligned(&mat[r + 6][c]);
        packed_c += packed_a7 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 6][c]);

        packed_c.loadUnaligned(&mat[r + 7][c]);
        packed_c += packed_a8 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 7][c]);

      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a1 * matrix2(i, c);
        mat(r + 1, c) += a2 * matrix2(i, c);
        mat(r + 2, c) += a3 * matrix2(i, c);
        mat(r + 3, c) += a4 * matrix2(i, c);
        mat(r + 4, c) += a5 * matrix2(i, c);
        mat(r + 5, c) += a6 * matrix2(i, c);
        mat(r + 6, c) += a7 * matrix2(i, c);
        mat(r + 7, c) += a8 * matrix2(i, c);
      }

    }
  }

  for (size_t r = iter; r < matrix1.rows(); r++) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      Packed<double> packed_a(matrix1[r][i]);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += matrix1[r][i] * matrix2(i, c);
      }

    }
  }

  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);


}



BOOST_FIXTURE_TEST_CASE(tl_multi_matrix_intri_block_10, PackedTest)
{

  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();

  size_t max_vector = (matrix2.cols() / packed_b_d.size()) * packed_b_d.size();
  Packed<double> packed_c;

  constexpr size_t step = Packed<double>::size();

  size_t iter = (matrix1.rows() / 10) * 10;

  Packed<double> packed_a1{};
  Packed<double> packed_a2{};
  Packed<double> packed_a3{};
  Packed<double> packed_a4{};
  Packed<double> packed_a5{};
  Packed<double> packed_a6{};
  Packed<double> packed_a7{};
  Packed<double> packed_a8{};
  Packed<double> packed_a9{};
  Packed<double> packed_a10{};

  for (size_t r = 0; r < iter; r += 10) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      double a1 = matrix1[r][i];
      double a2 = matrix1[r + 1][i];
      double a3 = matrix1[r + 2][i];
      double a4 = matrix1[r + 3][i];
      double a5 = matrix1[r + 4][i];
      double a6 = matrix1[r + 5][i];
      double a7 = matrix1[r + 6][i];
      double a8 = matrix1[r + 7][i];
      double a9 = matrix1[r + 8][i];
      double a10 = matrix1[r + 9][i];

      packed_a1.setScalar(a1);
      packed_a2.setScalar(a2);
      packed_a3.setScalar(a3);
      packed_a4.setScalar(a4);
      packed_a5.setScalar(a5);
      packed_a6.setScalar(a6);
      packed_a7.setScalar(a7);
      packed_a8.setScalar(a8);
      packed_a9.setScalar(a9);
      packed_a10.setScalar(a10);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a1 * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);

        packed_c.loadUnaligned(&mat[r + 1][c]);
        packed_c += packed_a2 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 1][c]);

        packed_c.loadUnaligned(&mat[r + 2][c]);
        packed_c += packed_a3 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 2][c]);

        packed_c.loadUnaligned(&mat[r + 3][c]);
        packed_c += packed_a4 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 3][c]);

        packed_c.loadUnaligned(&mat[r + 4][c]);
        packed_c += packed_a5 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 4][c]);

        packed_c.loadUnaligned(&mat[r + 5][c]);
        packed_c += packed_a6 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 5][c]);

        packed_c.loadUnaligned(&mat[r + 6][c]);
        packed_c += packed_a7 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 6][c]);

        packed_c.loadUnaligned(&mat[r + 7][c]);
        packed_c += packed_a8 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 7][c]);

        packed_c.loadUnaligned(&mat[r + 8][c]);
        packed_c += packed_a9 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 8][c]);

        packed_c.loadUnaligned(&mat[r + 9][c]);
        packed_c += packed_a10 * packed_b_d;
        packed_c.storeUnaligned(&mat[r + 9][c]);

      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += a1 * matrix2(i, c);
        mat(r + 1, c) += a2 * matrix2(i, c);
        mat(r + 2, c) += a3 * matrix2(i, c);
        mat(r + 3, c) += a4 * matrix2(i, c);
        mat(r + 4, c) += a5 * matrix2(i, c);
        mat(r + 5, c) += a6 * matrix2(i, c);
        mat(r + 6, c) += a7 * matrix2(i, c);
        mat(r + 7, c) += a8 * matrix2(i, c);
        mat(r + 8, c) += a9 * matrix2(i, c);
        mat(r + 9, c) += a10 * matrix2(i, c);
      }

    }
  }

  for (size_t r = iter; r < matrix1.rows(); r++) {
    for (size_t i = 0; i < matrix1.cols(); i++) {

      Packed<double> packed_a(matrix1[r][i]);

      for (size_t c = 0; c < max_vector; c += step) {

        packed_b_d.loadUnaligned(&matrix2[i][c]);

        packed_c.loadUnaligned(&mat[r][c]);
        packed_c += packed_a * packed_b_d;
        packed_c.storeUnaligned(&mat[r][c]);
      }

      for (size_t c = max_vector; c < matrix2.cols(); c++) {
        mat(r, c) += matrix1[r][i] * matrix2(i, c);
      }

    }
  }

  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);


}



//BOOST_FIXTURE_TEST_CASE(matrix_mul_double_4, PackedTest)
//{
//
//  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();
//
//
//  Packed<double> packed_c;
//  Packed<double> packed_a;
//
//  size_t max_vector = (matrix1.cols() / packed_a.size()) * packed_a.size();
//
//  for (size_t r = 0; r < max_vector; r += packed_a.size()) {
//    for (size_t i = 0; i < matrix1.cols(); i++) {
//
//      packed_a.loadUnaligned(&matrix1[r][i]);
//
//      for (size_t c = 0; c < max_vector; c++) {
//
//        double b1 = matrix2[i][c];
//        tl::math::simd::Packed<double> packed_b1(b1);
//        packed_c.loadUnaligned(&mat[r][c]);
//        packed_c += packed_a * packed_b_d;
//        packed_c.storeUnaligned(&mat[r][c]);
//      }
//
//    }
//  }
//
//  for (size_t r = max_vector; r < matrix1.rows(); r++) {
//    for (size_t i = 0; i < matrix1.cols(); i++) {
//      double a = matrix1[r][i];
//      for (size_t c = 0; c < max_vector; c++) {
//        mat(r, c) += a * matrix2(i, c);
//      }
//    }
//  }
//
//  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);
//
//
//}

//BOOST_FIXTURE_TEST_CASE(matrix_mul_double_5, PackedTest)
//{
//
//  Matrix<double, 10, 10> mat = Matrix<double, 10, 10>::zero();
//
//
//  Packed<double> packed_c;
//  Packed<double> packed_a;
//
//  size_t max_vector = (matrix1.cols() / packed_a.size()) * packed_a.size();
//
//  for (size_t r = 0; r < matrix1.rows(); r++) {
//    for (size_t i = 0; i < max_vector; i += packed_a_d.size()) {
//
//      packed_a_d.loadAligned(&matrix1[r][i]);
//
//      for (size_t c = 0; c < matrix2.cols(); c++/*c += packed_b_d.size()*/) {
//
//
//
//        std::array<double, tl::math::simd::PackedTraits<tl::math::simd::Packed<double>>::size> b;
//        for (size_t k = 0; k < packed_b_d.size(); k++) {
//          b[k] = matrix2[i + k][c];
//        }
//
//        packed_b_d.loadAligned(b.data());
//
//        packed_c = packed_a_d * packed_b_d;
//
//        mat[r][c] += packed_c.sum();
//        //matrix(r, c) += matrix1(r, i) * B(i, c);
//      }
//    }
//  }
//
//  for (size_t r = 0; r < matrix1.rows(); r++) {
//    for (size_t i = max_vector; i < matrix1.cols(); i++) {
//      double a = matrix1(r, i);
//      for (size_t c = 0; c < matrix2.cols(); c++) {
//        mat(r, c) += a * matrix2(i, c);
//      }
//    }
//  }
//
//
//  BOOST_CHECK_CLOSE(mat[0][0], 150.46129999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][1], 165.27870000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][2], 158.26670000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][3], 141.92560000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][4], 187.37970000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][5], 167.06009999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][6], 202.88790000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][7], 176.77520000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][8], 180.07070000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[0][9], 152.05050000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][0], 227.41139999999996, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][1], 227.16690000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][2], 201.63189999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][3], 255.38209999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][4], 290.18440000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][5], 291.53309999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][6], 253.57210000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][7], 261.00040000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][8], 286.16719999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[1][9], 215.73440000000005, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][0], 187.83359999999996, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][1], 189.31679999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][2], 173.78440000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][3], 218.94399999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][4], 246.74090000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][5], 217.63609999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][6], 218.45099999999994, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][7], 282.29669999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][8], 228.28930000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[2][9], 189.72190000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][0], 213.25510000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][1], 194.08109999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][2], 195.57950000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][3], 196.17829999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][4], 267.10329999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][5], 249.59680000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][6], 279.93429999999995, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][7], 234.07190000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][8], 245.87960000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[3][9], 174.91820000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][0], 169.70760000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][1], 180.98440000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][2], 179.15429999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][3], 148.66820000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][4], 208.23980000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][5], 214.42800000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][6], 172.50779999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][7], 212.39560000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][8], 204.69629999999995, 0.01);
//  BOOST_CHECK_CLOSE(mat[4][9], 150.65260000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][0], 171.61580000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][1], 149.27350000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][2], 161.58570000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][3], 146.83769999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][4], 195.70380000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][5], 203.04889999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][6], 143.38870000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][7], 200.82369999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][8], 200.71049999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[5][9], 143.38950000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][0], 141.29339999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][1], 138.90479999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][2], 105.16480000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][3], 129.06590000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][4], 142.59010000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][5], 187.58380000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][6], 108.31489999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][7], 176.67999999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][8], 137.38419999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[6][9], 127.76190000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][0], 213.23240000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][1], 230.35989999999998, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][2], 122.61140000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][3], 177.68850000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][4], 224.14439999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][5], 208.38609999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][6], 189.76090000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][7], 301.09850000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][8], 187.49260000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[7][9], 210.30039999999997, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][0], 235.31420000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][1], 138.34640000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][2], 152.97290000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][3], 135.53080000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][4], 208.74430000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][5], 231.41550000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][6], 157.95550000000003, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][7], 217.42079999999996, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][8], 228.77390000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[8][9], 157.30339999999995, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][0], 176.99920000000000, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][1], 160.61970000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][2], 122.35509999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][3], 119.04239999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][4], 180.94049999999999, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][5], 165.30430000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][6], 165.88210000000004, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][7], 198.12640000000002, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][8], 167.02330000000001, 0.01);
//  BOOST_CHECK_CLOSE(mat[9][9], 155.77199999999999, 0.01);
//
//
//}
BOOST_AUTO_TEST_SUITE_END()

#endif // TL_HAVE_SIMD_INTRINSICS
