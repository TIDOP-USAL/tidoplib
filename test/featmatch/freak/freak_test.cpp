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
 
#define BOOST_TEST_MODULE Tidop FreakDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/freak.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(FreakDescriptorTestSuite)

struct FreakDescriptorTest
{
  FreakDescriptorTest()
    : freakDescriptor(new FreakDescriptor(false, false, 11., 3))
  { }
    
  ~FreakDescriptorTest()
  { 
    delete freakDescriptor;
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  FreakProperties freakProperties;
  FreakDescriptor *freakDescriptor;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, FreakDescriptorTest)
{
  BOOST_CHECK_EQUAL(true, freakProperties.orientationNormalized());
  BOOST_CHECK_EQUAL(true, freakProperties.scaleNormalized());
  BOOST_CHECK_EQUAL(22., freakProperties.patternScale());
  BOOST_CHECK_EQUAL(4, freakProperties.octaves());
}

BOOST_FIXTURE_TEST_CASE(constructor, FreakDescriptorTest)
{
  BOOST_CHECK_EQUAL(false, freakDescriptor->orientationNormalized());
  BOOST_CHECK_EQUAL(false, freakDescriptor->scaleNormalized());
  BOOST_CHECK_EQUAL(11., freakDescriptor->patternScale());
  BOOST_CHECK_EQUAL(3, freakDescriptor->octaves());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, FreakDescriptorTest)
{
  FreakProperties copy(*freakDescriptor);
  BOOST_CHECK_EQUAL(false, copy.orientationNormalized());
  BOOST_CHECK_EQUAL(false, copy.scaleNormalized());
  BOOST_CHECK_EQUAL(11., copy.patternScale());
  BOOST_CHECK_EQUAL(3, copy.octaves());
}

BOOST_FIXTURE_TEST_CASE(assign, FreakDescriptorTest)
{
  FreakProperties assign = *freakDescriptor;
  BOOST_CHECK_EQUAL(false, assign.orientationNormalized());
  BOOST_CHECK_EQUAL(false, assign.scaleNormalized());
  BOOST_CHECK_EQUAL(11., assign.patternScale());
  BOOST_CHECK_EQUAL(3, assign.octaves());
}

BOOST_FIXTURE_TEST_CASE(type, FreakDescriptorTest)
{
  BOOST_CHECK(Feature::Type::freak == freakProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, FreakDescriptorTest)
{
  BOOST_CHECK_EQUAL("FREAK", freakProperties.name());
}

BOOST_FIXTURE_TEST_CASE(orientationNormalized, FreakDescriptorTest)
{
  FreakProperties freak;
  freak.setOrientationNormalized(false);
  BOOST_CHECK_EQUAL(false, freak.orientationNormalized());
  freak.setOrientationNormalized(true);
  BOOST_CHECK_EQUAL(true, freak.orientationNormalized());
}

BOOST_FIXTURE_TEST_CASE(scaleNormalized, FreakDescriptorTest)
{
  FreakProperties freak;
  freak.setScaleNormalized(false);
  BOOST_CHECK_EQUAL(false, freak.scaleNormalized());
  freak.setScaleNormalized(true);
  BOOST_CHECK_EQUAL(true, freak.scaleNormalized());
}

BOOST_FIXTURE_TEST_CASE(patternScale, FreakDescriptorTest)
{
  FreakProperties freak;
  freak.setPatternScale(5.0);
  BOOST_CHECK_EQUAL(5.0, freak.patternScale());
  freak.setPatternScale(10.);
  BOOST_CHECK_EQUAL(10., freak.patternScale());
}

BOOST_FIXTURE_TEST_CASE(octaves, FreakDescriptorTest)
{
  FreakProperties freak;
  freak.setOctaves(1);
  BOOST_CHECK_EQUAL(1, freak.octaves());
  freak.setOctaves(3);
  BOOST_CHECK_EQUAL(3, freak.octaves());
}

BOOST_FIXTURE_TEST_CASE(reset, FreakDescriptorTest)
{
  FreakProperties freak(*freakDescriptor);
  freak.reset();
  BOOST_CHECK_EQUAL(true, freak.orientationNormalized());
  BOOST_CHECK_EQUAL(true, freak.scaleNormalized());
  BOOST_CHECK_EQUAL(22., freak.patternScale());
  BOOST_CHECK_EQUAL(4, freak.octaves());
}

BOOST_AUTO_TEST_SUITE_END() 
