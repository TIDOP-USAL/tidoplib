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
  { }
    
  ~FreakDescriptorTest()
  { 
    delete freakDescriptor;
  }

  void setup()
  {
      //: freakDescriptor(new FreakDescriptor(false, false, 11., 3))
      freakProperties.setOrientationNormalized(false);
      freakProperties.setScaleNormalized(false);
      freakProperties.setPatternScale(11.f);
      freakProperties.setOctaves(3);

      freakDescriptor = new FreakDescriptor(freakProperties);
  }

  void teardown()
  {
  }

  FreakProperties freakDefaultProperties;
  FreakProperties freakProperties;
  FreakDescriptor *freakDescriptor;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, FreakDescriptorTest)
{
  BOOST_CHECK_EQUAL(true, freakDefaultProperties.orientationNormalized());
  BOOST_CHECK_EQUAL(true, freakDefaultProperties.scaleNormalized());
  BOOST_CHECK_EQUAL(22., freakDefaultProperties.patternScale());
  BOOST_CHECK_EQUAL(4, freakDefaultProperties.octaves());
}

BOOST_FIXTURE_TEST_CASE(constructor, FreakDescriptorTest)
{
  BOOST_CHECK_EQUAL(false, freakProperties.orientationNormalized());
  BOOST_CHECK_EQUAL(false, freakProperties.scaleNormalized());
  BOOST_CHECK_EQUAL(11., freakProperties.patternScale());
  BOOST_CHECK_EQUAL(3, freakProperties.octaves());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, FreakDescriptorTest)
{
  FreakProperties copy(freakProperties);
  BOOST_CHECK_EQUAL(false, copy.orientationNormalized());
  BOOST_CHECK_EQUAL(false, copy.scaleNormalized());
  BOOST_CHECK_EQUAL(11., copy.patternScale());
  BOOST_CHECK_EQUAL(3, copy.octaves());
}

BOOST_FIXTURE_TEST_CASE(assign, FreakDescriptorTest)
{
  FreakProperties assign = freakProperties;
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
  FreakProperties freak = freakProperties;
  freak.reset();
  BOOST_CHECK_EQUAL(true, freak.orientationNormalized());
  BOOST_CHECK_EQUAL(true, freak.scaleNormalized());
  BOOST_CHECK_EQUAL(22., freak.patternScale());
  BOOST_CHECK_EQUAL(4, freak.octaves());
}

BOOST_FIXTURE_TEST_CASE(freak_descriptor_properties, FreakDescriptorTest)
{
    auto &properties = freakDescriptor->properties();

    BOOST_CHECK_EQUAL(false, properties.getProperty<bool>("OrientationNormalized"));
    BOOST_CHECK_EQUAL(false, properties.getProperty<bool>("ScaleNormalized"));
    BOOST_CHECK_EQUAL(11., properties.getProperty<float>("PatternScale"));
    BOOST_CHECK_EQUAL(3, properties.getProperty<int>("Octaves"));

    BOOST_CHECK_EQUAL("false", properties.getPropertyAsString("OrientationNormalized"));
    BOOST_CHECK_EQUAL("false", properties.getPropertyAsString("ScaleNormalized"));
    BOOST_CHECK_EQUAL("11.000000", properties.getPropertyAsString("PatternScale"));
    BOOST_CHECK_EQUAL("3", properties.getPropertyAsString("Octaves"));
}

BOOST_AUTO_TEST_SUITE_END() 
