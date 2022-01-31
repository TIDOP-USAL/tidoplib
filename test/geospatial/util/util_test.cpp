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
 
#define BOOST_TEST_MODULE Tidop geospatial geospatial_util test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/util.h>

using namespace tl;
using namespace geospatial;


BOOST_AUTO_TEST_CASE(TEST_utmZoneFromLongitude)
{
  BOOST_TEST(30, utmZoneFromLongitude(0));
  BOOST_TEST(30, utmZoneFromLongitude(-3.5));
  BOOST_TEST(30, utmZoneFromLongitude(-6));
  BOOST_TEST(31, utmZoneFromLongitude(3.5));
  BOOST_TEST(31, utmZoneFromLongitude(6));
  BOOST_TEST(60, utmZoneFromLongitude(179));
  BOOST_TEST(60, utmZoneFromLongitude(180));
  BOOST_TEST(1, utmZoneFromLongitude(-180));
  BOOST_TEST(1, utmZoneFromLongitude(-179));
}