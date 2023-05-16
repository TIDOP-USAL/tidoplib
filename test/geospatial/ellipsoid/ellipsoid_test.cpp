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
 
#define BOOST_TEST_MODULE Tidop geospatial Ellipsoid test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/util.h>
#include <tidop/geospatial/ellipsoid.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(EllipsoidTestSuite)

struct EllipsoidTest
{
  EllipsoidTest()
  {

  }

  ~EllipsoidTest()
  {
    if (wgs84) {
      delete wgs84;
      wgs84 = nullptr;
    }

    if (sphere) {
      delete sphere;
      sphere = nullptr;
    }
  }

  virtual void setup()
  {
    wgs84 = new Ellipsoid("WGS84", 6378137.0, 6356752.3142451793);
    sphere = new Ellipsoid("sphere", 6370997., 6370997.);
  }

  virtual void teardown()
  {

  }

  Ellipsoid *wgs84;
  Ellipsoid *sphere;
};


BOOST_FIXTURE_TEST_CASE(flattening, EllipsoidTest)
{ 
  BOOST_CHECK_CLOSE(0.0033528106647475126, wgs84->flattening(), 0.01);
  BOOST_CHECK_EQUAL(0., sphere->flattening());
}

BOOST_FIXTURE_TEST_CASE(inverse_flattening, EllipsoidTest)
{
  BOOST_CHECK_CLOSE(298.257223563, wgs84->inverseFlattening(), 0.01);
  BOOST_CHECK_EQUAL(0., sphere->inverseFlattening());
}

BOOST_AUTO_TEST_SUITE_END()

