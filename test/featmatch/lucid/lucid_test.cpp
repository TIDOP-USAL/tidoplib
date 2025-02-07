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

#define BOOST_TEST_MODULE Tidop LucidDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/lucid.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(LucidDescriptorTestSuite)

struct LucidDescriptorTest
{
  LucidDescriptorTest()
    
  { }
    
  ~LucidDescriptorTest()
  { 
    delete lucidDescriptor;
  }

  void setup()
  {
      //: lucidDescriptor(new LucidDescriptor(3, 3))
      lucidProperties.setBlurKernel(3);
      lucidProperties.setLucidKernel(3);
      lucidDescriptor = new LucidDescriptor(lucidProperties);
  }

  void teardown()
  {
  }

  LucidProperties lucidDefaultProperties;
  LucidProperties lucidProperties;
  LucidDescriptor *lucidDescriptor;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, LucidDescriptorTest)
{
  BOOST_CHECK_EQUAL(1, lucidDefaultProperties.lucidKernel());
  BOOST_CHECK_EQUAL(2, lucidDefaultProperties.blurKernel());
}

BOOST_FIXTURE_TEST_CASE(constructor, LucidDescriptorTest)
{
  BOOST_CHECK_EQUAL(3, lucidProperties.lucidKernel());
  BOOST_CHECK_EQUAL(3, lucidProperties.blurKernel());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, LucidDescriptorTest)
{
  LucidProperties copy(lucidProperties);
  BOOST_CHECK_EQUAL(3, copy.lucidKernel());
  BOOST_CHECK_EQUAL(3, copy.blurKernel());
}

BOOST_FIXTURE_TEST_CASE(assign, LucidDescriptorTest)
{
  LucidProperties assign = lucidProperties;
  BOOST_CHECK_EQUAL(3, assign.lucidKernel());
  BOOST_CHECK_EQUAL(3, assign.blurKernel());
}

BOOST_FIXTURE_TEST_CASE(type, LucidDescriptorTest)
{
  BOOST_CHECK(Feature::Type::lucid == lucidProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, LucidDescriptorTest)
{
  BOOST_CHECK_EQUAL("LUCID", lucidProperties.name());
}

BOOST_FIXTURE_TEST_CASE(lucidKernel, LucidDescriptorTest)
{
  LucidProperties lucid;
  lucid.setLucidKernel(1);
  BOOST_CHECK_EQUAL(1, lucid.lucidKernel());
  lucid.setLucidKernel(2);
  BOOST_CHECK_EQUAL(2, lucid.lucidKernel());
}

BOOST_FIXTURE_TEST_CASE(blurKernel, LucidDescriptorTest)
{
  LucidProperties lucid;
  lucid.setBlurKernel(1);
  BOOST_CHECK_EQUAL(1, lucid.blurKernel());
  lucid.setBlurKernel(2);
  BOOST_CHECK_EQUAL(2, lucid.blurKernel());
}

BOOST_FIXTURE_TEST_CASE(reset, LucidDescriptorTest)
{
  LucidProperties lucid = lucidProperties;
  lucid.reset();
  BOOST_CHECK_EQUAL(1, lucid.lucidKernel());
  BOOST_CHECK_EQUAL(2, lucid.blurKernel());
}

BOOST_FIXTURE_TEST_CASE(lucid_descriptor_properties, LucidDescriptorTest)
{
    auto &properties = lucidDescriptor->properties();

    BOOST_CHECK_EQUAL(3, properties.getProperty<int>("LucidKernel"));
    BOOST_CHECK_EQUAL(3, properties.getProperty<int>("BlurKernel"));

    BOOST_CHECK_EQUAL("3", properties.getPropertyAsString("LucidKernel"));
    BOOST_CHECK_EQUAL("3", properties.getPropertyAsString("BlurKernel"));
}

BOOST_AUTO_TEST_SUITE_END()
