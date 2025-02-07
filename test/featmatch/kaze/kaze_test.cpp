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

#define BOOST_TEST_MODULE Tidop KazeDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/kaze.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(KazeDetectorDescriptorTestSuite)

struct KazeDetectorDescriptorTest
{
    KazeDetectorDescriptorTest()
    {
    }

    ~KazeDetectorDescriptorTest()
    {
        delete kazeDetectorDescriptor;
    }

    void setup()
    {
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

        kazeProperties.setExtendedDescriptor(true);
        kazeProperties.setUprightDescriptor(true);
        kazeProperties.setThreshold(0.05f);
        kazeProperties.setOctaves(8);
        kazeProperties.setOctaveLayers(3);
        kazeProperties.setDiffusivity("DIFF_PM_G1");

        kazeDetectorDescriptor = new KazeDetectorDescriptor(kazeProperties);
    }

    void teardown()
    {
    }

    KazeProperties kazeDefaultProperties;
    KazeProperties kazeProperties;
    KazeDetectorDescriptor *kazeDetectorDescriptor;
    std::vector<std::string> diffusivity_value;
    std::vector<std::string> diffusivity_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, KazeDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL(false, kazeDefaultProperties.extendedDescriptor());
    BOOST_CHECK_EQUAL(false, kazeDefaultProperties.uprightDescriptor());
    BOOST_CHECK_EQUAL(0.001f, kazeDefaultProperties.threshold());
    BOOST_CHECK_EQUAL(4, kazeDefaultProperties.octaves());
    BOOST_CHECK_EQUAL(4, kazeDefaultProperties.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_PM_G2", kazeDefaultProperties.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(constructor, KazeDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL(true, kazeProperties.extendedDescriptor());
    BOOST_CHECK_EQUAL(true, kazeProperties.uprightDescriptor());
    BOOST_CHECK_EQUAL(0.05f, kazeProperties.threshold());
    BOOST_CHECK_EQUAL(8, kazeProperties.octaves());
    BOOST_CHECK_EQUAL(3, kazeProperties.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_PM_G1", kazeProperties.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, KazeDetectorDescriptorTest)
{
    KazeProperties copy(kazeProperties);
    BOOST_CHECK_EQUAL(true, copy.extendedDescriptor());
    BOOST_CHECK_EQUAL(true, copy.uprightDescriptor());
    BOOST_CHECK_EQUAL(0.05f, copy.threshold());
    BOOST_CHECK_EQUAL(8, copy.octaves());
    BOOST_CHECK_EQUAL(3, copy.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_PM_G1", copy.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(assign, KazeDetectorDescriptorTest)
{
    KazeProperties assign = kazeProperties;
    BOOST_CHECK_EQUAL(true, assign.extendedDescriptor());
    BOOST_CHECK_EQUAL(true, assign.uprightDescriptor());
    BOOST_CHECK_EQUAL(0.05f, assign.threshold());
    BOOST_CHECK_EQUAL(8, assign.octaves());
    BOOST_CHECK_EQUAL(3, assign.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_PM_G1", assign.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(type, KazeDetectorDescriptorTest)
{
    BOOST_CHECK(Feature::Type::kaze == kazeProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, KazeDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL("KAZE", kazeProperties.name());
}

BOOST_FIXTURE_TEST_CASE(extendedDescriptor, KazeDetectorDescriptorTest)
{
    KazeProperties kaze;
    kaze.setExtendedDescriptor(false);
    BOOST_CHECK_EQUAL(false, kaze.extendedDescriptor());
    kaze.setExtendedDescriptor(true);
    BOOST_CHECK_EQUAL(true, kaze.extendedDescriptor());
}

BOOST_FIXTURE_TEST_CASE(uprightDescriptor, KazeDetectorDescriptorTest)
{
    KazeProperties kaze;
    kaze.setUprightDescriptor(false);
    BOOST_CHECK_EQUAL(false, kaze.uprightDescriptor());
    kaze.setUprightDescriptor(true);
    BOOST_CHECK_EQUAL(true, kaze.uprightDescriptor());
}

BOOST_FIXTURE_TEST_CASE(threshold, KazeDetectorDescriptorTest)
{
    KazeProperties kaze;
    kaze.setThreshold(5.0f);
    BOOST_CHECK_EQUAL(5.0f, kaze.threshold());
    kaze.setThreshold(10.0f);
    BOOST_CHECK_EQUAL(10.0f, kaze.threshold());
}

BOOST_FIXTURE_TEST_CASE(octaves, KazeDetectorDescriptorTest)
{
    KazeProperties kaze;
    kaze.setOctaves(4);
    BOOST_CHECK_EQUAL(4, kaze.octaves());
    kaze.setOctaves(8);
    BOOST_CHECK_EQUAL(8, kaze.octaves());
}

BOOST_FIXTURE_TEST_CASE(octaveLayers, KazeDetectorDescriptorTest)
{
    KazeProperties kaze;
    kaze.setOctaveLayers(3);
    BOOST_CHECK_EQUAL(3, kaze.octaveLayers());
    kaze.setOctaveLayers(3);
    BOOST_CHECK_EQUAL(3, kaze.octaveLayers());
}

BOOST_FIXTURE_TEST_CASE(diffusivity, KazeDetectorDescriptorTest)
{
    KazeProperties kaze;
    for (size_t i = 0; i < diffusivity_value.size(); i++) {
        kaze.setDiffusivity(diffusivity_value[i]);
        BOOST_CHECK_EQUAL(diffusivity_result[i], kaze.diffusivity());
    }
}

BOOST_FIXTURE_TEST_CASE(reset, KazeDetectorDescriptorTest)
{
    KazeProperties kaze = kazeProperties;
    kaze.reset();
    BOOST_CHECK_EQUAL(false, kaze.extendedDescriptor());
    BOOST_CHECK_EQUAL(false, kaze.uprightDescriptor());
    BOOST_CHECK_EQUAL(0.001f, kaze.threshold());
    BOOST_CHECK_EQUAL(4, kaze.octaves());
    BOOST_CHECK_EQUAL(4, kaze.octaveLayers());
    BOOST_CHECK_EQUAL("DIFF_PM_G2", kaze.diffusivity());
}

BOOST_FIXTURE_TEST_CASE(kaze_detector_properties, KazeDetectorDescriptorTest)
{
    auto properties = kazeDetectorDescriptor->properties();

    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("ExtendedDescriptor"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("UprightDescriptor"));
    BOOST_CHECK_EQUAL(0.05f, properties.getProperty<double>("Threshold"));
    BOOST_CHECK_EQUAL(8, properties.getProperty<int>("Octaves"));
    BOOST_CHECK_EQUAL(3, properties.getProperty<int>("OctaveLayers"));
    BOOST_CHECK_EQUAL("DIFF_PM_G1", properties.getProperty<std::string>("Diffusivity"));

    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("ExtendedDescriptor"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("UprightDescriptor"));
    BOOST_CHECK_EQUAL("0.050000", properties.getPropertyAsString("Threshold"));
    BOOST_CHECK_EQUAL("8", properties.getPropertyAsString("Octaves"));
    BOOST_CHECK_EQUAL("3", properties.getPropertyAsString("OctaveLayers"));
    BOOST_CHECK_EQUAL("DIFF_PM_G1", properties.getPropertyAsString("Diffusivity"));
}

BOOST_AUTO_TEST_SUITE_END()
