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
 
#define BOOST_TEST_MODULE Tidop BriefDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/brief.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(BriefDescriptorTestSuite)

struct BriefDescriptorTest
{
    BriefDescriptorTest()
    { }

    ~BriefDescriptorTest()
    {
        delete briefDescriptor;
    }

    void setup()
    {
        bytes_value.push_back("16");
        bytes_value.push_back("32");
        bytes_value.push_back("64");
        bytes_value.push_back("BAD_VALUE");

        bytes_result.push_back("16");
        bytes_result.push_back("32");
        bytes_result.push_back("64");
        bytes_result.push_back("64");

        briefProperties.setBytes("16");
        briefProperties.setUseOrientation(true);
       
        briefDescriptor = new BriefDescriptor(briefProperties);
    }

    void teardown()
    {
    }

    BriefProperties briefDefaultProperties;
    BriefProperties briefProperties;
    BriefDescriptor *briefDescriptor;
    std::vector<std::string> bytes_value;
    std::vector<std::string> bytes_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, BriefDescriptorTest)
{
    BOOST_CHECK_EQUAL("32", briefDefaultProperties.bytes());
    BOOST_CHECK_EQUAL(false, briefDefaultProperties.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(constructor, BriefDescriptorTest)
{
    BOOST_CHECK_EQUAL("16", briefProperties.bytes());
    BOOST_CHECK_EQUAL(true, briefProperties.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, BriefDescriptorTest)
{
    BriefProperties copy(briefProperties);
    BOOST_CHECK_EQUAL("16", copy.bytes());
    BOOST_CHECK_EQUAL(true, copy.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(assign, BriefDescriptorTest)
{
    BriefProperties assign = briefProperties;
    BOOST_CHECK_EQUAL("16", assign.bytes());
    BOOST_CHECK_EQUAL(true, assign.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(type, BriefDescriptorTest)
{
    BOOST_CHECK(Feature::Type::brief == briefProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, BriefDescriptorTest)
{
    BOOST_CHECK_EQUAL("BRIEF", briefProperties.name());
}

BOOST_FIXTURE_TEST_CASE(bytes, BriefDescriptorTest)
{
    BriefProperties brief;
    for (size_t i = 0; i < bytes_value.size(); i++) {
        brief.setBytes(bytes_value[i]);
        BOOST_CHECK_EQUAL(bytes_result[i], brief.bytes());
    }
}

BOOST_FIXTURE_TEST_CASE(scaleFactor, BriefDescriptorTest)
{
    BriefProperties brief;
    brief.setUseOrientation(false);
    BOOST_CHECK_EQUAL(false, brief.useOrientation());
    brief.setUseOrientation(true);
    BOOST_CHECK_EQUAL(true, brief.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(reset, BriefDescriptorTest)
{
    BriefProperties brief = briefProperties;
    brief.reset();
    BOOST_CHECK_EQUAL("32", brief.bytes());
    BOOST_CHECK_EQUAL(false, brief.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(brief_descriptor_properties, BriefDescriptorTest)
{
    auto properties = briefDescriptor->properties();

    BOOST_CHECK_EQUAL("16", properties.getProperty<std::string>("Bytes"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("UseOrientation"));

    BOOST_CHECK_EQUAL("16", properties.getPropertyAsString("Bytes"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("UseOrientation"));
}

BOOST_AUTO_TEST_SUITE_END() 
