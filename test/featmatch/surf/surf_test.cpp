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
 
#define BOOST_TEST_MODULE Tidop SurfDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/surf.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(SurfDetectorDescriptorTestSuite)

struct SurfDetectorDescriptorTest
{
  SurfDetectorDescriptorTest()
#ifdef OPENCV_ENABLE_NONFREE
    : surfDetectorDescriptor(new SurfDetectorDescriptor(50., 2, 5, true, true))
#endif
  { }
    
  ~SurfDetectorDescriptorTest()
  { 
#ifdef OPENCV_ENABLE_NONFREE
    delete surfDetectorDescriptor;
#endif
  }

  void setup()
  {

  }

  void teardown()
  {
  }

  SurfProperties surfProperties;
#ifdef OPENCV_ENABLE_NONFREE
  SurfDetectorDescriptor *surfDetectorDescriptor;
#endif

};

BOOST_FIXTURE_TEST_CASE(default_constructor, SurfDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(100, surfProperties.hessianThreshold());
  BOOST_CHECK_EQUAL(4, surfProperties.octaves());
  BOOST_CHECK_EQUAL(3, surfProperties.octaveLayers());
  BOOST_CHECK_EQUAL(false, surfProperties.extendedDescriptor());
  BOOST_CHECK_EQUAL(false, surfProperties.upright());
}

#ifdef OPENCV_ENABLE_NONFREE
BOOST_FIXTURE_TEST_CASE(constructor, SurfDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL(50., surfDetectorDescriptor->hessianThreshold());
  BOOST_CHECK_EQUAL(2, surfDetectorDescriptor->octaves());
  BOOST_CHECK_EQUAL(5, surfDetectorDescriptor->octaveLayers());
  BOOST_CHECK_EQUAL(true, surfDetectorDescriptor->extendedDescriptor());
  BOOST_CHECK_EQUAL(true, surfDetectorDescriptor->upright());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, SurfDetectorDescriptorTest)
{
  SurfProperties copy(*surfDetectorDescriptor);
  BOOST_CHECK_EQUAL(50., copy.hessianThreshold());
  BOOST_CHECK_EQUAL(2, copy.octaves());
  BOOST_CHECK_EQUAL(5, copy.octaveLayers());
  BOOST_CHECK_EQUAL(true, copy.extendedDescriptor());
  BOOST_CHECK_EQUAL(true, copy.upright());
}

BOOST_FIXTURE_TEST_CASE(assign, SurfDetectorDescriptorTest)
{
  SurfProperties assign = *surfDetectorDescriptor;
  BOOST_CHECK_EQUAL(50., assign.hessianThreshold());
  BOOST_CHECK_EQUAL(2, assign.octaves());
  BOOST_CHECK_EQUAL(5, assign.octaveLayers());
  BOOST_CHECK_EQUAL(true, assign.extendedDescriptor());
  BOOST_CHECK_EQUAL(true, assign.upright());
}
#endif

BOOST_FIXTURE_TEST_CASE(type, SurfDetectorDescriptorTest)
{
  BOOST_CHECK(Feature::Type::surf == surfProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, SurfDetectorDescriptorTest)
{
  BOOST_CHECK_EQUAL("SURF", surfProperties.name());
}

BOOST_FIXTURE_TEST_CASE(hessianThreshold, SurfDetectorDescriptorTest)
{
  SurfProperties surf;
  surf.setHessianThreshold(50);
  BOOST_CHECK_EQUAL(50, surf.hessianThreshold());
  surf.setHessianThreshold(100);
  BOOST_CHECK_EQUAL(100, surf.hessianThreshold());
}

BOOST_FIXTURE_TEST_CASE(octaves, SurfDetectorDescriptorTest)
{
  SurfProperties surf;
  surf.setOctaves(4);
  BOOST_CHECK_EQUAL(4, surf.octaves());
  surf.setOctaves(8);
  BOOST_CHECK_EQUAL(8, surf.octaves());
}

BOOST_FIXTURE_TEST_CASE(octaveLayers, SurfDetectorDescriptorTest)
{
  SurfProperties surf;
  surf.setOctaveLayers(3);
  BOOST_CHECK_EQUAL(3, surf.octaveLayers());
  surf.setOctaveLayers(4);
  BOOST_CHECK_EQUAL(4, surf.octaveLayers());
}

BOOST_FIXTURE_TEST_CASE(extendedDescriptor, SurfDetectorDescriptorTest)
{
  SurfProperties surf;
  surf.setExtendedDescriptor(false);
  BOOST_CHECK_EQUAL(false, surf.extendedDescriptor());
  surf.setExtendedDescriptor(true);
  BOOST_CHECK_EQUAL(true, surf.extendedDescriptor());
}

BOOST_FIXTURE_TEST_CASE(upright, SurfDetectorDescriptorTest)
{
  SurfProperties surf;
  surf.setUpright(false);
  BOOST_CHECK_EQUAL(false, surf.upright());
  surf.setUpright(true);
  BOOST_CHECK_EQUAL(true, surf.upright());
}

#ifdef OPENCV_ENABLE_NONFREE
BOOST_FIXTURE_TEST_CASE(reset, SurfDetectorDescriptorTest)
{
  SurfProperties surf(*surfDetectorDescriptor);
  surf.reset();
  BOOST_CHECK_EQUAL(100, surf.hessianThreshold());
  BOOST_CHECK_EQUAL(4, surf.octaves());
  BOOST_CHECK_EQUAL(3, surf.octaveLayers());
  BOOST_CHECK_EQUAL(false, surf.extendedDescriptor());
  BOOST_CHECK_EQUAL(false, surf.upright());
}
#endif

BOOST_AUTO_TEST_SUITE_END()
