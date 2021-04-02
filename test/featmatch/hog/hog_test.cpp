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
 
#define BOOST_TEST_MODULE Tidop HogDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/hog.h>
#include <tidop/geometry/size.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(HogDescriptorTestSuite)

struct HogDescriptorTest
{
  HogDescriptorTest()
    : hogDescriptor(new HogDescriptor(Size<int>(32, 32), Size<int>(8, 8), Size<int>(4, 4), Size<int>(4, 4), 5, 2))
  { }
    
  ~HogDescriptorTest()
  { 
    delete hogDescriptor;
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  HogProperties hogProperties;
  HogDescriptor *hogDescriptor;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, HogDescriptorTest)
{
  BOOST_CHECK(Size<int>(16, 16) == hogProperties.winSize());
  BOOST_CHECK(Size<int>(4, 4) == hogProperties.blockSize());
  BOOST_CHECK(Size<int>(2, 2) == hogProperties.blockStride());
  BOOST_CHECK(Size<int>(2, 2) == hogProperties.cellSize());
  BOOST_CHECK_EQUAL(9, hogProperties.nbins());
  BOOST_CHECK_EQUAL(1, hogProperties.derivAperture());
}

BOOST_FIXTURE_TEST_CASE(constructor, HogDescriptorTest)
{
  BOOST_CHECK(Size<int>(32, 32) == hogDescriptor->winSize());
  BOOST_CHECK(Size<int>(8, 8) == hogDescriptor->blockSize());
  BOOST_CHECK(Size<int>(4, 4) == hogDescriptor->blockStride());
  BOOST_CHECK(Size<int>(4, 4) == hogDescriptor->cellSize());
  BOOST_CHECK_EQUAL(5, hogDescriptor->nbins());
  BOOST_CHECK_EQUAL(2, hogDescriptor->derivAperture());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, HogDescriptorTest)
{
  HogProperties copy(*hogDescriptor);
  BOOST_CHECK(Size<int>(32, 32) == copy.winSize());
  BOOST_CHECK(Size<int>(8, 8) == copy.blockSize());
  BOOST_CHECK(Size<int>(4, 4) == copy.blockStride());
  BOOST_CHECK(Size<int>(4, 4) == copy.cellSize());
  BOOST_CHECK_EQUAL(5, copy.nbins());
  BOOST_CHECK_EQUAL(2, copy.derivAperture());
}

BOOST_FIXTURE_TEST_CASE(assign, HogDescriptorTest)
{
  HogProperties assign = *hogDescriptor;
  BOOST_CHECK(Size<int>(32, 32) == assign.winSize());
  BOOST_CHECK(Size<int>(8, 8) == assign.blockSize());
  BOOST_CHECK(Size<int>(4, 4) == assign.blockStride());
  BOOST_CHECK(Size<int>(4, 4) == assign.cellSize());
  BOOST_CHECK_EQUAL(5, assign.nbins());
  BOOST_CHECK_EQUAL(2, assign.derivAperture());
}

BOOST_FIXTURE_TEST_CASE(type, HogDescriptorTest)
{
  BOOST_CHECK(Feature::Type::hog == hogProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, HogDescriptorTest)
{
  BOOST_CHECK_EQUAL("HOG", hogProperties.name());
}

BOOST_FIXTURE_TEST_CASE(winSize, HogDescriptorTest)
{
  HogProperties hog;
  hog.setWinSize(Size<int>(0,0));
  BOOST_CHECK(Size<int>(0,0) == hog.winSize());
  hog.setWinSize(Size<int>(64,64));
  BOOST_CHECK(Size<int>(64,64) == hog.winSize());
}

BOOST_FIXTURE_TEST_CASE(blockSize, HogDescriptorTest)
{
  HogProperties hog;
  hog.setBlockSize(Size<int>(64,64));
  BOOST_CHECK(Size<int>(64,64) == hog.blockSize());
  hog.setBlockSize(Size<int>(64,128));
  BOOST_CHECK(Size<int>(64,128) == hog.blockSize());
}

BOOST_FIXTURE_TEST_CASE(blockStride, HogDescriptorTest)
{
  HogProperties hog;
  hog.setBlockStride(Size<int>(64,64));
  BOOST_CHECK(Size<int>(64,64) == hog.blockStride());
  hog.setBlockStride(Size<int>(64,128));
  BOOST_CHECK(Size<int>(64,128) == hog.blockStride());
}

BOOST_FIXTURE_TEST_CASE(cellSize, HogDescriptorTest)
{
  HogProperties hog;
  hog.setCellSize(Size<int>(64,64));
  BOOST_CHECK(Size<int>(64,64) == hog.cellSize());
  hog.setCellSize(Size<int>(64,128));
  BOOST_CHECK(Size<int>(64,128) == hog.cellSize());
}

BOOST_FIXTURE_TEST_CASE(nbins, HogDescriptorTest)
{
  HogProperties hog;
  hog.setNbins(1);
  BOOST_CHECK_EQUAL(1, hog.nbins());
  hog.setNbins(200);
  BOOST_CHECK_EQUAL(200, hog.nbins());
}

BOOST_FIXTURE_TEST_CASE(derivAperture, HogDescriptorTest)
{
  HogProperties hog;
  hog.setDerivAperture(22);
  BOOST_CHECK_EQUAL(22, hog.derivAperture());
  hog.setDerivAperture(46);
  BOOST_CHECK_EQUAL(46, hog.derivAperture());
}

BOOST_FIXTURE_TEST_CASE(reset, HogDescriptorTest)
{
  HogProperties hog(*hogDescriptor);
  hog.reset();
  BOOST_CHECK(Size<int>(16, 16) == hog.winSize());
  BOOST_CHECK(Size<int>(4, 4) == hog.blockSize());
  BOOST_CHECK(Size<int>(2, 2) == hog.blockStride());
  BOOST_CHECK(Size<int>(2, 2) == hog.cellSize());
  BOOST_CHECK_EQUAL(9, hog.nbins());
  BOOST_CHECK_EQUAL(1, hog.derivAperture());
}

BOOST_AUTO_TEST_SUITE_END() 

