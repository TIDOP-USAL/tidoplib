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
 
#define BOOST_TEST_MODULE Tidop AkazeDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/akaze.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(AkazeDetectorDescriptorTestSuite)

struct AkazeDetectorDescriptorTest
{
    AkazeDetectorDescriptorTest()
    { }

    ~AkazeDetectorDescriptorTest()
    {
        delete akazeDetectorDescriptor;
    }

    void setup()
    {
        descriptor_type_value.push_back("KAZE");
        descriptor_type_value.push_back("KAZE_UPRIGHT");
        descriptor_type_value.push_back("MLDB");
        descriptor_type_value.push_back("MLDB_UPRIGHT");
        descriptor_type_value.push_back("BAD_VALUE");

        descriptor_type_result.push_back("KAZE");
        descriptor_type_result.push_back("KAZE_UPRIGHT");
        descriptor_type_result.push_back("MLDB");
        descriptor_type_result.push_back("MLDB_UPRIGHT");
        descriptor_type_result.push_back("MLDB_UPRIGHT");

        diffusivity_value.push_back("DIFF_PM_G1");
        diffusivity_value.push_back("DIFF_PM_G2");
        diffusivity_value.push_back("DIFF_WEICKERT");
        diffusivity_value.push_back("DIFF_CHARBONNIER");
        diffusivity_value.push_back("BAD_VALUE");

        diffusivity_result.push_back("DIFF_PM_G1");
        diffusivity_result.push_back("DIFF_PM_G2");
        diffusivity_result.push_back("DIFF_WEICKERT");
        diffusivity_result.push_back("DIFF_CHARBONNIER");
        diffusivity_result.push_back("DIFF_CHARBONNIER");

        akazeProperties.setDescriptorType("KAZE_UPRIGHT");
        akazeProperties.setDescriptorSize(32);
        akazeProperties.setDescriptorChannels(4);
        akazeProperties.setThreshold(0.1);
        akazeProperties.setOctaves(3);
        akazeProperties.setOctaveLayers(6);
        akazeProperties.setDiffusivity("DIFF_WEICKERT");

        akazeDetectorDescriptor = new AkazeDetectorDescriptor(akazeProperties);
    }

    void teardown()
    {
    }

    AkazeProperties akazeDefaultProperties;
    AkazeProperties akazeProperties;
    AkazeDetectorDescriptor *akazeDetectorDescriptor;
    std::vector<std::string> descriptor_type_value;
    std::vector<std::string> descriptor_type_result;
    std::vector<std::string> diffusivity_value;
    std::vector<std::string> diffusivity_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, AkazeDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL("MLDB", akazeDefaultProperties.descriptorType());
    BOOST_CHECK_EQUAL(0, akazeDefaultProperties.descriptorSize());
    BOOST_CHECK_EQUAL(3, akazeDefaultProperties.descriptorChannels());
    BOOST_CHECK_EQUAL(0.001, akazeDefaultProperties.threshold());
    BOOST_CHECK_EQUAL(4, akazeDefaultProperties.octaves());
    BOOST_CHECK_EQUAL(4, akazeDefaultProperties.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_PM_G2", akazeDefaultProperties.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(constructor, AkazeDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL("KAZE_UPRIGHT", akazeProperties.descriptorType());
    BOOST_CHECK_EQUAL(32, akazeProperties.descriptorSize());
    BOOST_CHECK_EQUAL(4, akazeProperties.descriptorChannels());
    BOOST_CHECK_EQUAL(0.1, akazeProperties.threshold());
    BOOST_CHECK_EQUAL(3, akazeProperties.octaves());
    BOOST_CHECK_EQUAL(6, akazeProperties.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_WEICKERT", akazeProperties.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, AkazeDetectorDescriptorTest)
{
    AkazeProperties copy(akazeProperties);
    BOOST_CHECK_EQUAL("KAZE_UPRIGHT", copy.descriptorType());
    BOOST_CHECK_EQUAL(32, copy.descriptorSize());
    BOOST_CHECK_EQUAL(4, copy.descriptorChannels());
    BOOST_CHECK_EQUAL(0.1, copy.threshold());
    BOOST_CHECK_EQUAL(3, copy.octaves());
    BOOST_CHECK_EQUAL(6, copy.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_WEICKERT", copy.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(assign, AkazeDetectorDescriptorTest)
{
    AkazeProperties assign = akazeProperties;
    BOOST_CHECK_EQUAL("KAZE_UPRIGHT", assign.descriptorType());
    BOOST_CHECK_EQUAL(32, assign.descriptorSize());
    BOOST_CHECK_EQUAL(4, assign.descriptorChannels());
    BOOST_CHECK_EQUAL(0.1, assign.threshold());
    BOOST_CHECK_EQUAL(3, assign.octaves());
    BOOST_CHECK_EQUAL(6, assign.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_WEICKERT", assign.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(type, AkazeDetectorDescriptorTest)
{
    BOOST_CHECK(Feature::Type::akaze == akazeProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, AkazeDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL("AKAZE", akazeProperties.name());
}

BOOST_FIXTURE_TEST_CASE(descriptorType, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    for (size_t i = 0; i < descriptor_type_value.size(); i++) {
        akaze.setDescriptorType(descriptor_type_value[i]);
        BOOST_CHECK_EQUAL(descriptor_type_result[i], akaze.descriptorType());
    }
}

BOOST_FIXTURE_TEST_CASE(descriptorSize, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    akaze.setDescriptorSize(32);
    BOOST_CHECK_EQUAL(32, akaze.descriptorSize());
    akaze.setDescriptorSize(64);
    BOOST_CHECK_EQUAL(64, akaze.descriptorSize());
}

BOOST_FIXTURE_TEST_CASE(descriptorChannels, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    akaze.setDescriptorChannels(1);
    BOOST_CHECK_EQUAL(1, akaze.descriptorChannels());
    akaze.setDescriptorChannels(2);
    BOOST_CHECK_EQUAL(2, akaze.descriptorChannels());
}

BOOST_FIXTURE_TEST_CASE(threshold, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    akaze.setThreshold(5.0);
    BOOST_CHECK_EQUAL(5.0, akaze.threshold());
    akaze.setThreshold(10.0);
    BOOST_CHECK_EQUAL(10.0, akaze.threshold());
}

BOOST_FIXTURE_TEST_CASE(octaves, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    akaze.setOctaves(4);
    BOOST_CHECK_EQUAL(4, akaze.octaves());
    akaze.setOctaves(8);
    BOOST_CHECK_EQUAL(8, akaze.octaves());
}

BOOST_FIXTURE_TEST_CASE(octaveLayers, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    akaze.setOctaveLayers(3);
    BOOST_CHECK_EQUAL(3, akaze.octaveLayers());
    akaze.setOctaveLayers(3);
    BOOST_CHECK_EQUAL(3, akaze.octaveLayers());
}

BOOST_FIXTURE_TEST_CASE(diffusivity, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    for (size_t i = 0; i < diffusivity_value.size(); i++) {
        akaze.setDiffusivity(diffusivity_value[i]);
        BOOST_CHECK_EQUAL(diffusivity_result[i], akaze.diffusivity());
    }
}

BOOST_FIXTURE_TEST_CASE(reset, AkazeDetectorDescriptorTest)
{
    AkazeProperties akaze;
    akaze.setDescriptorType("KAZE_UPRIGHT");
    akaze.setDescriptorSize(32);
    akaze.setDescriptorChannels(4);
    akaze.setThreshold(0.1);
    akaze.setOctaves(3);
    akaze.setOctaveLayers(6);
    akaze.setDiffusivity("DIFF_WEICKERT");
    akaze.reset();
    BOOST_CHECK_EQUAL("MLDB", akaze.descriptorType());
    BOOST_CHECK_EQUAL(0, akaze.descriptorSize());
    BOOST_CHECK_EQUAL(3, akaze.descriptorChannels());
    BOOST_CHECK_EQUAL(0.001, akaze.threshold());
    BOOST_CHECK_EQUAL(4, akaze.octaves());
    BOOST_CHECK_EQUAL(4, akaze.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_PM_G2", akaze.diffusivity());
}


BOOST_FIXTURE_TEST_CASE(akaze_detector_properties, AkazeDetectorDescriptorTest)
{
    auto properties = akazeDetectorDescriptor->properties();

    BOOST_CHECK_EQUAL("KAZE_UPRIGHT", properties.getProperty<std::string>("DescriptorType"));
    BOOST_CHECK_EQUAL(32, properties.getProperty<int>("DescriptorSize"));
    BOOST_CHECK_EQUAL(4, properties.getProperty<int>("DescriptorChannels"));
    BOOST_CHECK_EQUAL(0.1, properties.getProperty<double>("Threshold"));
    BOOST_CHECK_EQUAL(3, properties.getProperty<int>("Octaves"));
    BOOST_CHECK_EQUAL(6, properties.getProperty<int>("OctaveLayers"));
    BOOST_CHECK_EQUAL("DIFF_WEICKERT", properties.getProperty<std::string>("Diffusivity"));

    BOOST_CHECK_EQUAL("KAZE_UPRIGHT", properties.getPropertyAsString("DescriptorType"));
    BOOST_CHECK_EQUAL("32", properties.getPropertyAsString("DescriptorSize"));
    BOOST_CHECK_EQUAL("4", properties.getPropertyAsString("DescriptorChannels"));
    BOOST_CHECK_EQUAL("0.100000", properties.getPropertyAsString("Threshold"));
    BOOST_CHECK_EQUAL("3", properties.getPropertyAsString("Octaves"));
    BOOST_CHECK_EQUAL("6", properties.getPropertyAsString("OctaveLayers"));
    BOOST_CHECK_EQUAL("DIFF_WEICKERT", properties.getPropertyAsString("Diffusivity"));
}

BOOST_AUTO_TEST_SUITE_END()
