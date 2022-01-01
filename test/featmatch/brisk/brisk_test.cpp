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
 
#define BOOST_TEST_MODULE Tidop BriskDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/brisk.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(BriskDetectorDescriptorTestSuite)

struct BriskDetectorDescriptorTest
{
  BriskDetectorDescriptorTest()
    : briskDescriptor(new BriskDetectorDescriptor(15, 4, 2.))
  { }
    
  ~BriskDetectorDescriptorTest()
  { 
    delete briskDescriptor;
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  BriskProperties briskProperties;
  BriskDetectorDescriptor *briskDescriptor;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, BriskDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(30, briskProperties.threshold());
  BOOST_CHECK_EQUAL(3, briskProperties.octaves());
  BOOST_CHECK_EQUAL(1.0, briskProperties.patternScale());
}

BOOST_FIXTURE_TEST_CASE(constructor, BriskDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(15, briskDescriptor->threshold());
  BOOST_CHECK_EQUAL(4, briskDescriptor->octaves());
  BOOST_CHECK_EQUAL(2., briskDescriptor->patternScale());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, BriskDetectorDescriptorTest)
{
  BriskProperties copy(*briskDescriptor);
  BOOST_CHECK_EQUAL(15, copy.threshold());
  BOOST_CHECK_EQUAL(4, copy.octaves());
  BOOST_CHECK_EQUAL(2., copy.patternScale());
}

BOOST_FIXTURE_TEST_CASE(assign, BriskDetectorDescriptorTest)
{
  BriskProperties assign = *briskDescriptor;
  BOOST_CHECK_EQUAL(15, assign.threshold());
  BOOST_CHECK_EQUAL(4, assign.octaves());
  BOOST_CHECK_EQUAL(2., assign.patternScale());
}

BOOST_FIXTURE_TEST_CASE(type, BriskDetectorDescriptorTest)
{
  BOOST_CHECK(Feature::Type::brisk == briskProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, BriskDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL("BRISK", briskProperties.name());
}

BOOST_FIXTURE_TEST_CASE(threshold, BriskDetectorDescriptorTest)
{
  BriskProperties brisk;
  brisk.setThreshold(11);
  BOOST_CHECK_EQUAL(11, brisk.threshold());
  brisk.setThreshold(20);
  BOOST_CHECK_EQUAL(20, brisk.threshold());
}

BOOST_FIXTURE_TEST_CASE(octaves, BriskDetectorDescriptorTest)
{
  BriskProperties brisk;
  brisk.setOctaves(4);
  BOOST_CHECK_EQUAL(4, brisk.octaves());
  brisk.setOctaves(8);
  BOOST_CHECK_EQUAL(8, brisk.octaves());
}

BOOST_FIXTURE_TEST_CASE(patternScale, BriskDetectorDescriptorTest)
{
  BriskProperties brisk;
  brisk.setPatternScale(2.);
  BOOST_CHECK_EQUAL(2., brisk.patternScale());
  brisk.setPatternScale(8.);
  BOOST_CHECK_EQUAL(8., brisk.patternScale());
}

BOOST_FIXTURE_TEST_CASE(reset, BriskDetectorDescriptorTest)
{
  BriskProperties brisk(*briskDescriptor);
  brisk.reset();
  BOOST_CHECK_EQUAL(30, brisk.threshold());
  BOOST_CHECK_EQUAL(3, brisk.octaves());
  BOOST_CHECK_EQUAL(1.0, brisk.patternScale());
}

BOOST_AUTO_TEST_SUITE_END() 
