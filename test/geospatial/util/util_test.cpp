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
  BOOST_CHECK_EQUAL(1, utmZoneFromLongitude(-180));
  BOOST_CHECK_EQUAL(1, utmZoneFromLongitude(-179));
  BOOST_CHECK_EQUAL(2, utmZoneFromLongitude(-174));
  BOOST_CHECK_EQUAL(2, utmZoneFromLongitude(-170));
  BOOST_CHECK_EQUAL(3, utmZoneFromLongitude(-168));
  BOOST_CHECK_EQUAL(4, utmZoneFromLongitude(-162));
  BOOST_CHECK_EQUAL(29, utmZoneFromLongitude(-7));
  BOOST_CHECK_EQUAL(30, utmZoneFromLongitude(-6));
  BOOST_CHECK_EQUAL(30, utmZoneFromLongitude(-3.5));
  BOOST_CHECK_EQUAL(31, utmZoneFromLongitude(0));
  BOOST_CHECK_EQUAL(31, utmZoneFromLongitude(3.5));
  BOOST_CHECK_EQUAL(32, utmZoneFromLongitude(6));
  BOOST_CHECK_EQUAL(60, utmZoneFromLongitude(179));
  BOOST_CHECK_EQUAL(1, utmZoneFromLongitude(180));

}

BOOST_AUTO_TEST_CASE(TEST_utmZoneFromLonLat)
{
  auto zone_band = utmZoneFromLonLat(0, 0);
  BOOST_CHECK_EQUAL(31, zone_band.first);
  BOOST_CHECK_EQUAL('N', zone_band.second);
  zone_band = utmZoneFromLonLat(0, 10);
  BOOST_CHECK_EQUAL(31, zone_band.first);
  BOOST_CHECK_EQUAL('P', zone_band.second);
  zone_band = utmZoneFromLonLat(0, -10);
  BOOST_CHECK_EQUAL(31, zone_band.first);
  BOOST_CHECK_EQUAL('L', zone_band.second);
  zone_band = utmZoneFromLonLat(-3.5, 20);
  BOOST_CHECK_EQUAL(30, zone_band.first);
  BOOST_CHECK_EQUAL('Q', zone_band.second);
  zone_band = utmZoneFromLonLat(-6, -35);
  BOOST_CHECK_EQUAL(30, zone_band.first);
  BOOST_CHECK_EQUAL('H', zone_band.second);
  zone_band = utmZoneFromLonLat(3.5, -8);
  BOOST_CHECK_EQUAL(31, zone_band.first);
  BOOST_CHECK_EQUAL('M', zone_band.second);
  zone_band = utmZoneFromLonLat(6, -53);
  BOOST_CHECK_EQUAL(32, zone_band.first);
  BOOST_CHECK_EQUAL('F', zone_band.second);

}
