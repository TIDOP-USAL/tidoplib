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
 
#define BOOST_TEST_MODULE Tidop LatchDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/latch.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(LatchDescriptorTestSuite)

struct LatchDescriptorTest
{
    LatchDescriptorTest()
    {
    }

    ~LatchDescriptorTest()
    {
        delete latchDescriptor;
    }

    void setup()
    {
        bytes_value.push_back("1");
        bytes_value.push_back("2");
        bytes_value.push_back("4");
        bytes_value.push_back("8");
        bytes_value.push_back("16");
        bytes_value.push_back("32");
        bytes_value.push_back("64");
        bytes_value.push_back("bad_value");

        bytes_result.push_back("1");
        bytes_result.push_back("2");
        bytes_result.push_back("4");
        bytes_result.push_back("8");
        bytes_result.push_back("16");
        bytes_result.push_back("32");
        bytes_result.push_back("64");
        bytes_result.push_back("64");

        // "16", false, 2
        latchProperties.setBytes("16");
        latchProperties.setRotationInvariance(false);
        latchProperties.setHalfSsdSize(2);

        latchDescriptor = new LatchDescriptor(latchProperties);
    }

    void teardown()
    {
    }

    LatchProperties latchDefaultProperties;
    LatchProperties latchProperties;
    LatchDescriptor *latchDescriptor;
    std::vector<std::string> bytes_value;
    std::vector<std::string> bytes_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, LatchDescriptorTest)
{
    BOOST_CHECK_EQUAL("32", latchDefaultProperties.bytes());
    BOOST_CHECK_EQUAL(true, latchDefaultProperties.rotationInvariance());
    BOOST_CHECK_EQUAL(3, latchDefaultProperties.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(constructor, LatchDescriptorTest)
{
    BOOST_CHECK_EQUAL("16", latchProperties.bytes());
    BOOST_CHECK_EQUAL(false, latchProperties.rotationInvariance());
    BOOST_CHECK_EQUAL(2, latchProperties.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, LatchDescriptorTest)
{
    LatchProperties copy(latchProperties);
    BOOST_CHECK_EQUAL("16", copy.bytes());
    BOOST_CHECK_EQUAL(false, copy.rotationInvariance());
    BOOST_CHECK_EQUAL(2, copy.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(assign, LatchDescriptorTest)
{
    LatchProperties assign = latchProperties;
    BOOST_CHECK_EQUAL("16", assign.bytes());
    BOOST_CHECK_EQUAL(false, assign.rotationInvariance());
    BOOST_CHECK_EQUAL(2, assign.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(type, LatchDescriptorTest)
{
    BOOST_CHECK(Feature::Type::latch == latchProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, LatchDescriptorTest)
{
    BOOST_CHECK_EQUAL("LATCH", latchProperties.name());
}

BOOST_FIXTURE_TEST_CASE(bytes, LatchDescriptorTest)
{
    LatchProperties latch;
    for (size_t i = 0; i < bytes_value.size(); i++) {
        latch.setBytes(bytes_value[i]);
        BOOST_CHECK_EQUAL(bytes_result[i], latch.bytes());
    }
}

BOOST_FIXTURE_TEST_CASE(rotationInvariance, LatchDescriptorTest)
{
    LatchProperties latch;
    latch.setRotationInvariance(false);
    BOOST_CHECK_EQUAL(false, latch.rotationInvariance());
    latch.setRotationInvariance(true);
    BOOST_CHECK_EQUAL(true, latch.rotationInvariance());
}

BOOST_FIXTURE_TEST_CASE(halfSsdSize, LatchDescriptorTest)
{
    LatchProperties latch;
    latch.setHalfSsdSize(5);
    BOOST_CHECK_EQUAL(5, latch.halfSsdSize());
    latch.setHalfSsdSize(10);
    BOOST_CHECK_EQUAL(10, latch.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(reset, LatchDescriptorTest)
{
    LatchProperties latch = latchProperties;
    latch.reset();
    BOOST_CHECK_EQUAL("32", latch.bytes());
    BOOST_CHECK_EQUAL(true, latch.rotationInvariance());
    BOOST_CHECK_EQUAL(3, latch.halfSsdSize());
}

BOOST_FIXTURE_TEST_CASE(latch_descriptor_properties, LatchDescriptorTest)
{
    auto &properties = latchDescriptor->properties();

    BOOST_CHECK_EQUAL("16", properties.getProperty<std::string>("Bytes"));
    BOOST_CHECK_EQUAL(false, properties.getProperty<bool>("RotationInvariance"));
    BOOST_CHECK_EQUAL(2, properties.getProperty<int>("HalfSsdSize"));

    BOOST_CHECK_EQUAL("16", properties.getPropertyAsString("Bytes"));
    BOOST_CHECK_EQUAL("false", properties.getPropertyAsString("RotationInvariance"));
    BOOST_CHECK_EQUAL("2", properties.getPropertyAsString("HalfSsdSize"));
}

BOOST_AUTO_TEST_SUITE_END()
