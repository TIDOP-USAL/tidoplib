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
 
#define BOOST_TEST_MODULE Tidop MserDetector test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/mser.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(MserDetectorTestSuite)

struct MserDetectorTest
{
  MserDetectorTest()
    : mserDetector(new MserDetector(10, 50, 10000, 0.5, .3, 150, 1.1, 0.01, 6))
  { }
    
  ~MserDetectorTest()
  { 
    delete mserDetector;
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  MserProperties mserProperties;
  MserDetector *mserDetector;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, MserDetectorTest)
{
  BOOST_CHECK_EQUAL(5, mserProperties.delta());
  BOOST_CHECK_EQUAL(60, mserProperties.minArea());
  BOOST_CHECK_EQUAL(14400, mserProperties.maxArea());
  BOOST_CHECK_EQUAL(0.25, mserProperties.maxVariation());
  BOOST_CHECK_EQUAL(.2, mserProperties.minDiversity());
  BOOST_CHECK_EQUAL(200, mserProperties.maxEvolution());
  BOOST_CHECK_EQUAL(1.01, mserProperties.areaThreshold());
  BOOST_CHECK_EQUAL(0.003, mserProperties.minMargin());
  BOOST_CHECK_EQUAL(5, mserProperties.edgeBlurSize());
}

BOOST_FIXTURE_TEST_CASE(constructor, MserDetectorTest)
{
  BOOST_CHECK_EQUAL(10, mserDetector->delta());
  BOOST_CHECK_EQUAL(50, mserDetector->minArea());
  BOOST_CHECK_EQUAL(10000, mserDetector->maxArea());
  BOOST_CHECK_EQUAL(0.5, mserDetector->maxVariation());
  BOOST_CHECK_EQUAL(.3, mserDetector->minDiversity());
  BOOST_CHECK_EQUAL(150, mserDetector->maxEvolution());
  BOOST_CHECK_EQUAL(1.1, mserDetector->areaThreshold());
  BOOST_CHECK_EQUAL(0.01, mserDetector->minMargin());
  BOOST_CHECK_EQUAL(6, mserDetector->edgeBlurSize());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, MserDetectorTest)
{
  MserProperties copy(*mserDetector);
  BOOST_CHECK_EQUAL(10, copy.delta());
  BOOST_CHECK_EQUAL(50, copy.minArea());
  BOOST_CHECK_EQUAL(10000, copy.maxArea());
  BOOST_CHECK_EQUAL(0.5, copy.maxVariation());
  BOOST_CHECK_EQUAL(.3, copy.minDiversity());
  BOOST_CHECK_EQUAL(150, copy.maxEvolution());
  BOOST_CHECK_EQUAL(1.1, copy.areaThreshold());
  BOOST_CHECK_EQUAL(0.01, copy.minMargin());
  BOOST_CHECK_EQUAL(6, copy.edgeBlurSize());
}

BOOST_FIXTURE_TEST_CASE(assign, MserDetectorTest)
{
  MserProperties assign = *mserDetector;
  BOOST_CHECK_EQUAL(10, assign.delta());
  BOOST_CHECK_EQUAL(50, assign.minArea());
  BOOST_CHECK_EQUAL(10000, assign.maxArea());
  BOOST_CHECK_EQUAL(0.5, assign.maxVariation());
  BOOST_CHECK_EQUAL(.3, assign.minDiversity());
  BOOST_CHECK_EQUAL(150, assign.maxEvolution());
  BOOST_CHECK_EQUAL(1.1, assign.areaThreshold());
  BOOST_CHECK_EQUAL(0.01, assign.minMargin());
  BOOST_CHECK_EQUAL(6, assign.edgeBlurSize());
}

BOOST_FIXTURE_TEST_CASE(type, MserDetectorTest)
{
  BOOST_CHECK(Feature::Type::mser == mserProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, MserDetectorTest)
{
  BOOST_CHECK_EQUAL("MSER", mserProperties.name());
}

BOOST_FIXTURE_TEST_CASE(delta, MserDetectorTest)
{
  MserProperties mser;
  mser.setDelta(5);
  BOOST_CHECK_EQUAL(5, mser.delta());
  mser.setDelta(20);
  BOOST_CHECK_EQUAL(20, mser.delta());
}

BOOST_FIXTURE_TEST_CASE(minArea, MserDetectorTest)
{
  MserProperties mser;
  mser.setMinArea(5);
  BOOST_CHECK_EQUAL(5, mser.minArea());
  mser.setMinArea(10);
  BOOST_CHECK_EQUAL(10, mser.minArea());
}

BOOST_FIXTURE_TEST_CASE(maxArea, MserDetectorTest)
{
  MserProperties mser;
  mser.setMaxArea(100);
  BOOST_CHECK_EQUAL(100, mser.maxArea());
  mser.setMaxArea(500);
  BOOST_CHECK_EQUAL(500, mser.maxArea());
}

BOOST_FIXTURE_TEST_CASE(maxVariation, MserDetectorTest)
{
  MserProperties mser;
  mser.setMaxVariation(0.25);
  BOOST_CHECK_EQUAL(0.25, mser.maxVariation());
  mser.setMaxVariation(10);
  BOOST_CHECK_EQUAL(10, mser.maxVariation());
}

BOOST_FIXTURE_TEST_CASE(minDiversity, MserDetectorTest)
{
  MserProperties mser;
  mser.setMinDiversity(0.25);
  BOOST_CHECK_EQUAL(0.25, mser.minDiversity());
  mser.setMinDiversity(10);
  BOOST_CHECK_EQUAL(10, mser.minDiversity());
}

BOOST_FIXTURE_TEST_CASE(maxEvolution, MserDetectorTest)
{
  MserProperties mser;
  mser.setMaxEvolution(5);
  BOOST_CHECK_EQUAL(5, mser.maxEvolution());
  mser.setMaxEvolution(20);
  BOOST_CHECK_EQUAL(20, mser.maxEvolution());
}

BOOST_FIXTURE_TEST_CASE(areaThreshold, MserDetectorTest)
{
  MserProperties mser;
  mser.setAreaThreshold(0.25);
  BOOST_CHECK_EQUAL(0.25, mser.areaThreshold());
  mser.setAreaThreshold(1.5);
  BOOST_CHECK_EQUAL(1.5, mser.areaThreshold());
}

BOOST_FIXTURE_TEST_CASE(minMargin, MserDetectorTest)
{
  MserProperties mser;
  mser.setMinMargin(0.25);
  BOOST_CHECK_EQUAL(0.25, mser.minMargin());
  mser.setMinMargin(0.5);
  BOOST_CHECK_EQUAL(0.5, mser.minMargin());
}

BOOST_FIXTURE_TEST_CASE(edgeBlurSize, MserDetectorTest)
{
  MserProperties mser;
  mser.setEdgeBlurSize(5);
  BOOST_CHECK_EQUAL(5, mser.edgeBlurSize());
  mser.setEdgeBlurSize(20);
  BOOST_CHECK_EQUAL(20, mser.edgeBlurSize());
}

BOOST_FIXTURE_TEST_CASE(reset, MserDetectorTest)
{
  MserProperties mser(*mserDetector);
  mser.reset();
  BOOST_CHECK_EQUAL(5, mser.delta());
  BOOST_CHECK_EQUAL(60, mser.minArea());
  BOOST_CHECK_EQUAL(14400, mser.maxArea());
  BOOST_CHECK_EQUAL(0.25, mser.maxVariation());
  BOOST_CHECK_EQUAL(.2, mser.minDiversity());
  BOOST_CHECK_EQUAL(200, mser.maxEvolution());
  BOOST_CHECK_EQUAL(1.01, mser.areaThreshold());
  BOOST_CHECK_EQUAL(0.003, mser.minMargin());
  BOOST_CHECK_EQUAL(5, mser.edgeBlurSize());
}

BOOST_AUTO_TEST_SUITE_END()
