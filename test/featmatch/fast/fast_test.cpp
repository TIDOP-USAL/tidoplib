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
 
#define BOOST_TEST_MODULE Tidop FastDetector test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/fast.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(FastDetectorTestSuite)

struct FastDetectorTest
{
  FastDetectorTest()
  { }
    
  ~FastDetectorTest()
  { 
    delete fastDetector;
  }

  void setup()
  {
    descriptor_type_value.push_back("TYPE_5_8");
    descriptor_type_value.push_back("TYPE_7_12");
    descriptor_type_value.push_back("TYPE_9_16");
    descriptor_type_value.push_back("BAD_VALUE");
    
    descriptor_type_result.push_back("TYPE_5_8");
    descriptor_type_result.push_back("TYPE_7_12");
    descriptor_type_result.push_back("TYPE_9_16");
    descriptor_type_result.push_back("TYPE_9_16");

    fastProperties.setThreshold(11);
    fastProperties.setDetectorType("TYPE_7_12");
    fastProperties.setNonmaxSuppression(false);
    fastDetector = new FastDetector(fastProperties);
  }

  void teardown()
  {
  }

  FastProperties fastDefaultProperties;
  FastProperties fastProperties;
  FastDetector *fastDetector;
  std::vector<std::string> descriptor_type_value;
  std::vector<std::string> descriptor_type_result;
  
};

BOOST_FIXTURE_TEST_CASE(default_constructor, FastDetectorTest)
{
  BOOST_CHECK_EQUAL(10, fastDefaultProperties.threshold());
  BOOST_CHECK_EQUAL("TYPE_9_16", fastDefaultProperties.detectorType());
  BOOST_CHECK_EQUAL(true, fastDefaultProperties.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(constructor, FastDetectorTest)
{
  BOOST_CHECK_EQUAL(11, fastProperties.threshold());
  BOOST_CHECK_EQUAL("TYPE_7_12", fastProperties.detectorType());
  BOOST_CHECK_EQUAL(false, fastProperties.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, FastDetectorTest)
{
  FastProperties copy(fastProperties);
  BOOST_CHECK_EQUAL(11, copy.threshold());
  BOOST_CHECK_EQUAL("TYPE_7_12", copy.detectorType());
  BOOST_CHECK_EQUAL(false, copy.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(assign, FastDetectorTest)
{
  FastProperties assign = fastProperties;
  BOOST_CHECK_EQUAL(11, assign.threshold());
  BOOST_CHECK_EQUAL("TYPE_7_12", assign.detectorType());
  BOOST_CHECK_EQUAL(false, assign.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(type, FastDetectorTest)
{
  BOOST_CHECK(Feature::Type::fast == fastProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, FastDetectorTest)
{
  BOOST_CHECK_EQUAL("FAST", fastProperties.name());
}

BOOST_FIXTURE_TEST_CASE(threshold, FastDetectorTest)
{
  FastProperties fast;
  fast.setThreshold(11);
  BOOST_CHECK_EQUAL(11, fast.threshold());
  fast.setThreshold(20);
  BOOST_CHECK_EQUAL(20, fast.threshold());
}

BOOST_FIXTURE_TEST_CASE(nonmaxSuppression, FastDetectorTest)
{
  FastProperties fast;
  fast.setNonmaxSuppression(true);
  BOOST_CHECK_EQUAL(true, fast.nonmaxSuppression());
  fast.setNonmaxSuppression(false);
  BOOST_CHECK_EQUAL(false, fast.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(detectorType, FastDetectorTest)
{
  FastProperties fast;
  for (size_t i = 0; i < descriptor_type_value.size(); i++) {
    fast.setDetectorType(descriptor_type_value[i]);
    BOOST_CHECK_EQUAL(descriptor_type_result[i], fast.detectorType());
  }
}

BOOST_FIXTURE_TEST_CASE(reset, FastDetectorTest)
{
  FastProperties fast = fastProperties;
  fast.reset();
  BOOST_CHECK_EQUAL(10, fast.threshold());
  BOOST_CHECK_EQUAL("TYPE_9_16", fast.detectorType());
  BOOST_CHECK_EQUAL(true, fast.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(fast_detector_properties, FastDetectorTest)
{
    auto &properties = fastDetector->properties();

    BOOST_CHECK_EQUAL(11, properties.getProperty<int>("Threshold"));
    BOOST_CHECK_EQUAL(false, properties.getProperty<bool>("NonmaxSuppression"));
    BOOST_CHECK_EQUAL("TYPE_7_12", properties.getProperty<std::string>("DetectorType"));

    BOOST_CHECK_EQUAL("11", properties.getPropertyAsString("Threshold"));
    BOOST_CHECK_EQUAL("false", properties.getPropertyAsString("NonmaxSuppression"));
    BOOST_CHECK_EQUAL("TYPE_7_12", properties.getPropertyAsString("DetectorType"));
}

BOOST_AUTO_TEST_SUITE_END() 
