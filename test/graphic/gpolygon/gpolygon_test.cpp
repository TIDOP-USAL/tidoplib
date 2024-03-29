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

#define BOOST_TEST_MODULE Tidop graphic polygon test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/entities/polygon.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(GPolygonTestSuite)

struct GPolygonTest
{

  GPolygonTest()
  {
  }

  ~GPolygonTest()
  {
  }

  GPolygon polygon;
  GPolygon3D polygon_3d;
  GMultiPolygon multi_polygon;
  GMultiPolygon3D multi_polygon_3d;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, GPolygonTest)
{
  BOOST_CHECK_EQUAL(false, polygon.isMultiEntity());
  BOOST_CHECK_EQUAL(true, polygon.isSimpleEntity());
  BOOST_CHECK_EQUAL(false, polygon.is3D());

  BOOST_CHECK_EQUAL(false, polygon_3d.isMultiEntity());
  BOOST_CHECK_EQUAL(true, polygon_3d.isSimpleEntity());
  BOOST_CHECK_EQUAL(true, polygon_3d.is3D());

  BOOST_CHECK_EQUAL(true, multi_polygon.isMultiEntity());
  BOOST_CHECK_EQUAL(false, multi_polygon.isSimpleEntity());
  BOOST_CHECK_EQUAL(false, multi_polygon.is3D());

  BOOST_CHECK_EQUAL(true, multi_polygon_3d.isMultiEntity());
  BOOST_CHECK_EQUAL(false, multi_polygon_3d.isSimpleEntity());
  BOOST_CHECK_EQUAL(true, multi_polygon_3d.is3D());
  
}


BOOST_AUTO_TEST_SUITE_END()