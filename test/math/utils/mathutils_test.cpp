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

#define BOOST_TEST_MODULE Tidop math utils test
#include <boost/test/unit_test.hpp>
#include <tidop/math/math.h>

using namespace tl;



BOOST_AUTO_TEST_CASE(tl_math_clamp)
{
  BOOST_CHECK_EQUAL(10, math::clamp(10, 1, 20));
  BOOST_CHECK_EQUAL(1, math::clamp(0, 1, 20));
  BOOST_CHECK_EQUAL(10, math::clamp(10, 10, 20));
  BOOST_CHECK_EQUAL(11, math::clamp(10, 11, 20));
  BOOST_CHECK_EQUAL(20, math::clamp(20, 11, 20));
  BOOST_CHECK_EQUAL(20, math::clamp(21, 11, 20));
}

BOOST_AUTO_TEST_CASE(tl_math_module)
{
  BOOST_CHECK_EQUAL(0, math::module(0, 0));
  BOOST_CHECK_EQUAL(10, math::module(10, 0));
  BOOST_CHECK_EQUAL(10, math::module(0, 10));
  BOOST_CHECK_CLOSE(14.142135623730, math::module(10, 10), 0.1);
}

BOOST_AUTO_TEST_CASE(scalar_is_zero)
{
  BOOST_CHECK_EQUAL(true, math::isZero(math::consts::zero<int>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<int>));
  BOOST_CHECK_EQUAL(true, math::isZero(math::consts::zero<unsigned int>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<unsigned int>));
  BOOST_CHECK_EQUAL(true, math::isZero(math::consts::zero<long>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<long>));
  BOOST_CHECK_EQUAL(true, math::isZero(math::consts::zero<long long>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<long long>));
  BOOST_CHECK_EQUAL(true, math::isZero(math::consts::zero<double>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<double>));
  BOOST_CHECK_EQUAL(true, math::isZero(math::consts::zero<float>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<float>));
  BOOST_CHECK_EQUAL(true, math::isZero(0.000000000000000000000000000000000000000000001f));
  BOOST_CHECK_EQUAL(false, math::isZero(0.00000000000000000000000000000000000000000001f));
}

BOOST_AUTO_TEST_CASE(floats_nearly_zero)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyZero(math::consts::zero<double>));
  BOOST_CHECK_EQUAL(false, math::isNearlyZero(math::consts::one<double>));
  BOOST_CHECK_EQUAL(true, math::isNearlyZero(math::consts::zero<float>));
  BOOST_CHECK_EQUAL(false, math::isNearlyZero(math::consts::one<float>));
  BOOST_CHECK_EQUAL(true, math::isNearlyZero(0.000000000000000000000000000000000000000000001f));
  BOOST_CHECK_EQUAL(true, math::isNearlyZero(0.00000000000000000000000000000000000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyZero(0.0000000000000000000000000000000000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyZero(std::numeric_limits<float>::min()));
  BOOST_CHECK_EQUAL(false, math::isNearlyZero(std::numeric_limits<double>::min()));
}

BOOST_AUTO_TEST_CASE(positive_large_numbers)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-1000000.1f, -1000000.2f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-1000000.2f, -1000000.1f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1000001.f, -1000002.f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1000002.f, -1000001.f));
}

BOOST_AUTO_TEST_CASE(negative_large_numbers)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-1000000.1f, -1000000.2f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-1000000.2f, -1000000.1f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1000001.f, -1000002.f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1000002.f, -1000001.f));
}

BOOST_AUTO_TEST_CASE(numbers_around_1)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(1.0000001f, 1.0000002f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(1.0000002f, 1.0000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(1.0002f, 1.0001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(1.0001f, 1.0002f));
}

BOOST_AUTO_TEST_CASE(numbers_around_minus_1)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-1.000001f, -1.000002f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-1.000002f, -1.000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1.0001f, -1.0002f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1.0002f, -1.0001f));
}

BOOST_AUTO_TEST_CASE(small_numbers)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(0.000000001000001f, 0.000000001000002f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(0.000000001000002f, 0.000000001000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(0.000000000001002f, 0.000000000001001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(0.000000000001001f, 0.000000000001002f));
}

BOOST_AUTO_TEST_CASE(small_negative_numbers)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-0.000000001000001f, -0.000000001000002f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-0.000000001000002f, -0.000000001000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-0.000000000001002f, -0.000000000001001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-0.000000000001001f, -0.000000000001002f));
}

BOOST_AUTO_TEST_CASE(small_differences_away_from_zero)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(0.3f, 0.30000003f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-0.3f, -0.30000003f));
}

BOOST_AUTO_TEST_CASE(comparisons_involving_zero)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(0.0f, 0.0f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(0.0f, -0.0f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-0.0f, -0.0f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(0.00000001f, 0.0f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(0.0f, 0.00000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-0.00000001f, 0.0f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(0.0f, -0.00000001f));
}

BOOST_AUTO_TEST_CASE(comparisons_involving_extreme_values)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<float>::max(), std::numeric_limits<float>::max() / 2.f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() / 2.f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max() / 2.f));

  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<double>::max(), std::numeric_limits<double>::max() / 2.f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max() / 2.f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max() / 2.f));

}

BOOST_AUTO_TEST_CASE(infinity_values)
{
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()));
}

BOOST_AUTO_TEST_CASE(nan_values)
{
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<float>::quiet_NaN(), 0.0f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-0.0f, std::numeric_limits<float>::quiet_NaN()));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(std::numeric_limits<float>::quiet_NaN(), -0.0f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(0.0f, std::numeric_limits<float>::quiet_NaN()));
}

BOOST_AUTO_TEST_CASE(numbers_opposite_sides_of_0)
{
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(1.000000001f, -1.0f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1.0f, 1.000000001f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(-1.000000001f, 1.0f));
  BOOST_CHECK_EQUAL(false, math::isNearlyEqual(1.0f, -1.000000001f));
  BOOST_CHECK_EQUAL(true, math::isNearlyEqual(-0.0000000000000000000000000000000000001f, 0.0000000000000000000000000000000000001f));
  //assertFalse(nearlyEqual(10000 * Float.MIN_VALUE, 10000 * -Float.MIN_VALUE));
}