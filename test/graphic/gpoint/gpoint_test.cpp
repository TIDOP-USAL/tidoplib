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

#define BOOST_TEST_MODULE Tidop graphic point test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/entities/point.h>


using namespace tl;
//using namespace graph;

BOOST_AUTO_TEST_SUITE(GPointTestSuite)

struct GPointTest
{

  GPointTest()
  {
  }

  ~GPointTest()
  {
  }

  graph::GPoint point;
  graph::GPoint3D point_3d;
  graph::GMultiPoint multi_point;
  graph::GMultiPoint3D multi_point_3d;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, GPointTest)
{
  BOOST_CHECK_EQUAL(false, point.isMultiEntity());
  BOOST_CHECK_EQUAL(true, point.isSimpleEntity());
  BOOST_CHECK_EQUAL(false, point.is3D());

  BOOST_CHECK_EQUAL(false, point_3d.isMultiEntity());
  BOOST_CHECK_EQUAL(true, point_3d.isSimpleEntity());
  BOOST_CHECK_EQUAL(true, point_3d.is3D());

  BOOST_CHECK_EQUAL(true, multi_point.isMultiEntity());
  BOOST_CHECK_EQUAL(false, multi_point.isSimpleEntity());
  BOOST_CHECK_EQUAL(false, multi_point.is3D());

  BOOST_CHECK_EQUAL(true, multi_point_3d.isMultiEntity());
  BOOST_CHECK_EQUAL(false, multi_point_3d.isSimpleEntity());
  BOOST_CHECK_EQUAL(true, multi_point_3d.is3D());
  
}


BOOST_AUTO_TEST_SUITE_END()