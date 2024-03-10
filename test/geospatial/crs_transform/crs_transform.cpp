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
 
#define BOOST_TEST_MODULE Tidop geospatial crs_transform test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/crstransf.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;

/* CrsTransformTest */

#if defined HAVE_GDAL && defined HAVE_PROJ4

BOOST_AUTO_TEST_SUITE(CrsTransformTestSuite)

struct CrsTransformTest
{
  CrsTransformTest()
  {
    epsg25830 = std::make_shared<Crs>("EPSG:25830");
    epsg4258 = std::make_shared<Crs>("EPSG:4258");
  }

  ~CrsTransformTest()
  {
  }


  virtual void setup()
  {

  }

  virtual void teardown()
  {

  }

  std::shared_ptr<Crs> epsg25830;
  std::shared_ptr<Crs> epsg4258;
};


BOOST_FIXTURE_TEST_CASE(transform, CrsTransformTest)
{
  CrsTransform trf(epsg25830, epsg4258);
  Point3D pt_utm(281815.044, 4827675.243, 0.);
  Point3D pt_geo;
  trf.transform(pt_utm, pt_geo);
  BOOST_CHECK_CLOSE(43.570113, pt_geo.x, 0.1);
  BOOST_CHECK_CLOSE(-5.701905, pt_geo.y, 0.1);
  BOOST_CHECK_CLOSE(0., pt_geo.z, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(EcefToEnuTestSuite)

struct EcefToEnuTest
{
  EcefToEnuTest()
  {
  }

  ~EcefToEnuTest()
  {
  }


  virtual void setup()
  {

  }

  virtual void teardown()
  {

  }

};


BOOST_FIXTURE_TEST_CASE(transform, EcefToEnuTest)
{

}

BOOST_AUTO_TEST_SUITE_END()


#endif // HAVE_GDAL