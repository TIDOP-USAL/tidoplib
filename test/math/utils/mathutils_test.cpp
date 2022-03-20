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

BOOST_AUTO_TEST_CASE(scalar_is_zero)
{
  BOOST_CHECK_EQUAL(true,  math::isZero(math::consts::zero<int>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<int>));
  BOOST_CHECK_EQUAL(true,  math::isZero(math::consts::zero<unsigned int>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<unsigned int>));
  BOOST_CHECK_EQUAL(true,  math::isZero(math::consts::zero<long>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<long>));
  BOOST_CHECK_EQUAL(true,  math::isZero(math::consts::zero<long long>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<long long>));
  BOOST_CHECK_EQUAL(true,  math::isZero(math::consts::zero<double>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<double>));
  BOOST_CHECK_EQUAL(true,  math::isZero(math::consts::zero<float>));
  BOOST_CHECK_EQUAL(false, math::isZero(math::consts::one<float>));
  BOOST_CHECK_EQUAL(true,  math::isZero(0.000000000000000000000000000000000000000000001f));
  BOOST_CHECK_EQUAL(false, math::isZero(0.00000000000000000000000000000000000000000001f));



}



