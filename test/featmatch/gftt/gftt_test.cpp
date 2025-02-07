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

#define BOOST_TEST_MODULE Tidop GfttDetector test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/gftt.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(GfttDetectorTestSuite)

struct GfttDetectorTest
{
  GfttDetectorTest()
  { }
    
  ~GfttDetectorTest()
  { 
    delete gfttDetector;
  }

  void setup()
  {
      gfttProperties.setMaxFeatures(2000);
      gfttProperties.setQualityLevel(0.05);
      gfttProperties.setMinDistance(2);
      gfttProperties.setBlockSize(5);
      gfttProperties.setHarrisDetector(true);
      gfttProperties.setK(0.05);

      gfttDetector = new GfttDetector(gfttProperties);
  }

  void teardown()
  {
  }

  GfttProperties gfttDefaultProperties;
  GfttProperties gfttProperties;
  GfttDetector *gfttDetector;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, GfttDetectorTest)
{
  BOOST_CHECK_EQUAL(1000, gfttDefaultProperties.maxFeatures());
  BOOST_CHECK_EQUAL(0.01, gfttDefaultProperties.qualityLevel());
  BOOST_CHECK_EQUAL(1, gfttDefaultProperties.minDistance());
  BOOST_CHECK_EQUAL(3, gfttDefaultProperties.blockSize());
  BOOST_CHECK_EQUAL(false, gfttDefaultProperties.harrisDetector());
  BOOST_CHECK_EQUAL(0.04, gfttDefaultProperties.k());
}

BOOST_FIXTURE_TEST_CASE(constructor, GfttDetectorTest)
{
  BOOST_CHECK_EQUAL(2000, gfttProperties.maxFeatures());
  BOOST_CHECK_EQUAL(0.05, gfttProperties.qualityLevel());
  BOOST_CHECK_EQUAL(2, gfttProperties.minDistance());
  BOOST_CHECK_EQUAL(5, gfttProperties.blockSize());
  BOOST_CHECK_EQUAL(true, gfttProperties.harrisDetector());
  BOOST_CHECK_EQUAL(0.05, gfttProperties.k());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, GfttDetectorTest)
{
  GfttProperties copy(gfttProperties);
  BOOST_CHECK_EQUAL(2000, copy.maxFeatures());
  BOOST_CHECK_EQUAL(0.05, copy.qualityLevel());
  BOOST_CHECK_EQUAL(2, copy.minDistance());
  BOOST_CHECK_EQUAL(5, copy.blockSize());
  BOOST_CHECK_EQUAL(true, copy.harrisDetector());
  BOOST_CHECK_EQUAL(0.05, copy.k());
}

BOOST_FIXTURE_TEST_CASE(assign, GfttDetectorTest)
{
  GfttProperties assign = gfttProperties;
  BOOST_CHECK_EQUAL(2000, assign.maxFeatures());
  BOOST_CHECK_EQUAL(0.05, assign.qualityLevel());
  BOOST_CHECK_EQUAL(2, assign.minDistance());
  BOOST_CHECK_EQUAL(5, assign.blockSize());
  BOOST_CHECK_EQUAL(true, assign.harrisDetector());
  BOOST_CHECK_EQUAL(0.05, assign.k());
}

BOOST_FIXTURE_TEST_CASE(type, GfttDetectorTest)
{
  BOOST_CHECK(Feature::Type::gftt == gfttProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, GfttDetectorTest)
{
  BOOST_CHECK_EQUAL("GFTT", gfttProperties.name());
}

BOOST_FIXTURE_TEST_CASE(maxFeatures, GfttDetectorTest)
{
  GfttProperties gftt;
  gftt.setMaxFeatures(2000);
  BOOST_CHECK_EQUAL(2000, gftt.maxFeatures());
  gftt.setMaxFeatures(10000);
  BOOST_CHECK_EQUAL(10000, gftt.maxFeatures());
}

BOOST_FIXTURE_TEST_CASE(qualityLevel, GfttDetectorTest)
{
  GfttProperties gftt;
  gftt.setQualityLevel(0.01);
  BOOST_CHECK_EQUAL(0.01, gftt.qualityLevel());
  gftt.setQualityLevel(10.);
  BOOST_CHECK_EQUAL(10., gftt.qualityLevel());
}

BOOST_FIXTURE_TEST_CASE(minDistance, GfttDetectorTest)
{
  GfttProperties gftt;
  gftt.setMinDistance(2);
  BOOST_CHECK_EQUAL(2, gftt.minDistance());
  gftt.setMinDistance(30);
  BOOST_CHECK_EQUAL(30, gftt.minDistance());
}

BOOST_FIXTURE_TEST_CASE(blockSize, GfttDetectorTest)
{
  GfttProperties gftt;
  gftt.setBlockSize(3);
  BOOST_CHECK_EQUAL(3, gftt.blockSize());
  gftt.setBlockSize(4);
  BOOST_CHECK_EQUAL(4, gftt.blockSize());
}

BOOST_FIXTURE_TEST_CASE(interpolation, GfttDetectorTest)
{
  GfttProperties gftt;
  gftt.setHarrisDetector(true);
  BOOST_CHECK_EQUAL(true, gftt.harrisDetector());
  gftt.setHarrisDetector(false);
  BOOST_CHECK_EQUAL(false, gftt.harrisDetector());
}

BOOST_FIXTURE_TEST_CASE(k, GfttDetectorTest)
{
  GfttProperties gftt;
  gftt.setK(0.04);
  BOOST_CHECK_EQUAL(0.04, gftt.k());
  gftt.setK(0.1);
  BOOST_CHECK_EQUAL(0.1, gftt.k());
}

BOOST_FIXTURE_TEST_CASE(reset, GfttDetectorTest)
{
  GfttProperties gftt = gfttProperties;
  gftt.reset();
  BOOST_CHECK_EQUAL(1000, gftt.maxFeatures());
  BOOST_CHECK_EQUAL(0.01, gftt.qualityLevel());
  BOOST_CHECK_EQUAL(1, gftt.minDistance());
  BOOST_CHECK_EQUAL(3, gftt.blockSize());
  BOOST_CHECK_EQUAL(false, gftt.harrisDetector());
  BOOST_CHECK_EQUAL(0.04, gftt.k());
}

BOOST_FIXTURE_TEST_CASE(gftt_detector_properties, GfttDetectorTest)
{
    auto &properties = gfttDetector->properties();

    BOOST_CHECK_EQUAL(2000, properties.getProperty<int>("MaxFeatures"));
    BOOST_CHECK_EQUAL(0.05, properties.getProperty<double>("QualityLevel"));
    BOOST_CHECK_EQUAL(2., properties.getProperty<double>("MinDistance"));
    BOOST_CHECK_EQUAL(5, properties.getProperty<int>("BlockSize"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("HarrisDetector"));
    BOOST_CHECK_EQUAL(0.05, properties.getProperty<double>("K"));

    BOOST_CHECK_EQUAL("2000", properties.getPropertyAsString("MaxFeatures"));
    BOOST_CHECK_EQUAL("0.050000", properties.getPropertyAsString("QualityLevel"));
    BOOST_CHECK_EQUAL("2.000000", properties.getPropertyAsString("MinDistance"));
    BOOST_CHECK_EQUAL("5", properties.getPropertyAsString("BlockSize"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("HarrisDetector"));
    BOOST_CHECK_EQUAL("0.050000", properties.getPropertyAsString("K"));
}


BOOST_AUTO_TEST_SUITE_END() 
