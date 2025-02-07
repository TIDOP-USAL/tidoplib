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
 
#define BOOST_TEST_MODULE Tidop VggDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/vgg.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(VggDescriptorTestSuite)

struct VggDescriptorTest
{
    VggDescriptorTest()
    {
    }

    ~VggDescriptorTest()
    {
        delete vggDescriptor;
    }

    void setup()
    {
        descriptor_type_value.push_back("VGG_120");
        descriptor_type_value.push_back("VGG_80");
        descriptor_type_value.push_back("VGG_64");
        descriptor_type_value.push_back("VGG_48");
        descriptor_type_value.push_back("BAD_VALUE");

        descriptor_type_result.push_back("VGG_120");
        descriptor_type_result.push_back("VGG_80");
        descriptor_type_result.push_back("VGG_64");
        descriptor_type_result.push_back("VGG_48");
        descriptor_type_result.push_back("VGG_48");

        
        vggProperties.setDescriptorType("VGG_80");
        vggProperties.setScaleFactor(6.75f);
        vggProperties.setSigma(1.6f);
        vggProperties.setUseNormalizeDescriptor(true);
        vggProperties.setUseNormalizeImage(false);
        vggProperties.setUseScaleOrientation(false);
        vggDescriptor = new VggDescriptor(vggProperties);
    }

    void teardown()
    {
    }

    VggProperties vggDefaultProperties;
    VggProperties vggProperties;
    VggDescriptor *vggDescriptor;
    std::vector<std::string> descriptor_type_value;
    std::vector<std::string> descriptor_type_result;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, VggDescriptorTest)
{
    BOOST_CHECK_EQUAL("VGG_120", vggDefaultProperties.descriptorType());
    BOOST_CHECK_EQUAL(6.25f, vggDefaultProperties.scaleFactor());
    BOOST_CHECK_EQUAL(1.4f, vggDefaultProperties.sigma());
    BOOST_CHECK_EQUAL(false, vggDefaultProperties.useNormalizeDescriptor());
    BOOST_CHECK_EQUAL(true, vggDefaultProperties.useNormalizeImage());
    BOOST_CHECK_EQUAL(true, vggDefaultProperties.useScaleOrientation());
}

BOOST_FIXTURE_TEST_CASE(constructor, VggDescriptorTest)
{
    BOOST_CHECK_EQUAL("VGG_80", vggProperties.descriptorType());
    BOOST_CHECK_EQUAL(6.75f, vggProperties.scaleFactor());
    BOOST_CHECK_EQUAL(1.6f, vggProperties.sigma());
    BOOST_CHECK_EQUAL(true, vggProperties.useNormalizeDescriptor());
    BOOST_CHECK_EQUAL(false, vggProperties.useNormalizeImage());
    BOOST_CHECK_EQUAL(false, vggProperties.useScaleOrientation());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, VggDescriptorTest)
{
    VggProperties vgg_copy(vggProperties);
    BOOST_CHECK_EQUAL("VGG_80", vgg_copy.descriptorType());
    BOOST_CHECK_EQUAL(6.75f, vgg_copy.scaleFactor());
    BOOST_CHECK_EQUAL(1.6f, vgg_copy.sigma());
    BOOST_CHECK_EQUAL(true, vgg_copy.useNormalizeDescriptor());
    BOOST_CHECK_EQUAL(false, vgg_copy.useNormalizeImage());
    BOOST_CHECK_EQUAL(false, vgg_copy.useScaleOrientation());
}

BOOST_FIXTURE_TEST_CASE(assign, VggDescriptorTest)
{
    VggProperties vgg_assign = vggProperties;
    BOOST_CHECK_EQUAL("VGG_80", vgg_assign.descriptorType());
    BOOST_CHECK_EQUAL(6.75f, vgg_assign.scaleFactor());
    BOOST_CHECK_EQUAL(1.6f, vgg_assign.sigma());
    BOOST_CHECK_EQUAL(true, vgg_assign.useNormalizeDescriptor());
    BOOST_CHECK_EQUAL(false, vgg_assign.useNormalizeImage());
    BOOST_CHECK_EQUAL(false, vgg_assign.useScaleOrientation());
}

BOOST_FIXTURE_TEST_CASE(type, VggDescriptorTest)
{
    BOOST_CHECK(Feature::Type::vgg == vggProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, VggDescriptorTest)
{
    BOOST_CHECK_EQUAL("VGG", vggProperties.name());
}

BOOST_FIXTURE_TEST_CASE(threshold, VggDescriptorTest)
{
    VggProperties vgg;
    for (size_t i = 0; i < descriptor_type_value.size(); i++) {
        vgg.setDescriptorType(descriptor_type_value[i]);
        BOOST_CHECK_EQUAL(descriptor_type_result[i], vgg.descriptorType());
    }
}

BOOST_FIXTURE_TEST_CASE(scaleFactor, VggDescriptorTest)
{
    VggProperties vgg;
    vgg.setScaleFactor(6.75f);
    BOOST_CHECK_EQUAL(6.75f, vgg.scaleFactor());
    vgg.setScaleFactor(5.00f);
    BOOST_CHECK_EQUAL(5.00f, vgg.scaleFactor());
}

BOOST_FIXTURE_TEST_CASE(sigma, VggDescriptorTest)
{
    VggProperties vgg;
    vgg.setSigma(1.5f);
    BOOST_CHECK_EQUAL(1.5f, vgg.sigma());
    vgg.setSigma(1.7f);
    BOOST_CHECK_EQUAL(1.7f, vgg.sigma());
}

BOOST_FIXTURE_TEST_CASE(useNormalizeImage, VggDescriptorTest)
{
    VggProperties vgg;
    vgg.setUseNormalizeImage(false);
    BOOST_CHECK_EQUAL(false, vgg.useNormalizeImage());
    vgg.setUseNormalizeImage(true);
    BOOST_CHECK_EQUAL(true, vgg.useNormalizeImage());
}

BOOST_FIXTURE_TEST_CASE(useScaleOrientation, VggDescriptorTest)
{
    VggProperties vgg;
    vgg.setUseScaleOrientation(false);
    BOOST_CHECK_EQUAL(false, vgg.useScaleOrientation());
    vgg.setUseScaleOrientation(true);
    BOOST_CHECK_EQUAL(true, vgg.useScaleOrientation());
}

BOOST_FIXTURE_TEST_CASE(reset, VggDescriptorTest)
{
    VggProperties vgg = vggProperties;
    vgg.reset();
    BOOST_CHECK_EQUAL("VGG_120", vgg.descriptorType());
    BOOST_CHECK_EQUAL(6.25f, vgg.scaleFactor());
    BOOST_CHECK_EQUAL(1.4f, vgg.sigma());
    BOOST_CHECK_EQUAL(false, vgg.useNormalizeDescriptor());
    BOOST_CHECK_EQUAL(true, vgg.useNormalizeImage());
    BOOST_CHECK_EQUAL(true, vgg.useScaleOrientation());
}

BOOST_FIXTURE_TEST_CASE(vgg_descriptor_properties, VggDescriptorTest)
{
    auto &properties = vggDescriptor->properties();

    BOOST_CHECK_EQUAL("VGG_80", properties.getProperty<std::string>("DescriptorType"));
    BOOST_CHECK_EQUAL(6.75f, properties.getProperty<float>("ScaleFactor"));
    BOOST_CHECK_EQUAL(1.6f, properties.getProperty<float>("Sigma"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("UseNormalizeDescriptor"));
    BOOST_CHECK_EQUAL(false, properties.getProperty<bool>("UseNormalizeImage"));
    BOOST_CHECK_EQUAL(false, properties.getProperty<bool>("UseScaleOrientation"));

    BOOST_CHECK_EQUAL("VGG_80", properties.getPropertyAsString("DescriptorType"));
    BOOST_CHECK_EQUAL("6.750000", properties.getPropertyAsString("ScaleFactor"));
    BOOST_CHECK_EQUAL("1.600000", properties.getPropertyAsString("Sigma"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("UseNormalizeDescriptor"));
    BOOST_CHECK_EQUAL("false", properties.getPropertyAsString("UseNormalizeImage"));
    BOOST_CHECK_EQUAL("false", properties.getPropertyAsString("UseScaleOrientation"));
}

BOOST_AUTO_TEST_SUITE_END()
