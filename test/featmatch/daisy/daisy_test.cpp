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
 
#define BOOST_TEST_MODULE Tidop DaisyDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/daisy.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(DaisyDescriptorTestSuite)

struct DaisyDescriptorTest
{
    DaisyDescriptorTest()
    { }

    ~DaisyDescriptorTest()
    {
        delete daisyDescriptor;
    }

    void setup()
    {
        daisyProperties.setRadius(10.f);
        daisyProperties.setRadialDivisions(4);
        daisyProperties.setAngularDivision(4);
        daisyProperties.setGradientOrientationsDivision(4);
        daisyProperties.setNormalization("NRM_PARTIAL");
        daisyProperties.setInterpolation(false);
        daisyProperties.setUseOrientation(true);
        daisyDescriptor = new DaisyDescriptor(daisyProperties);
    }

    void teardown()
    {
    }

    DaisyProperties daisyDefaultProperties;
    DaisyProperties daisyProperties;
    DaisyDescriptor *daisyDescriptor;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, DaisyDescriptorTest)
{
    BOOST_CHECK_EQUAL(15.f, daisyDefaultProperties.radius());
    BOOST_CHECK_EQUAL(3, daisyDefaultProperties.radialDivisions());
    BOOST_CHECK_EQUAL(8, daisyDefaultProperties.angularDivision());
    BOOST_CHECK_EQUAL(8, daisyDefaultProperties.gradientOrientationsDivision());
    BOOST_CHECK_EQUAL("NRM_NONE", daisyDefaultProperties.normalization());
    BOOST_CHECK_EQUAL(true, daisyDefaultProperties.interpolation());
    BOOST_CHECK_EQUAL(false, daisyDefaultProperties.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(constructor, DaisyDescriptorTest)
{
    BOOST_CHECK_EQUAL(10.f, daisyProperties.radius());
    BOOST_CHECK_EQUAL(4, daisyProperties.radialDivisions());
    BOOST_CHECK_EQUAL(4, daisyProperties.angularDivision());
    BOOST_CHECK_EQUAL(4, daisyProperties.gradientOrientationsDivision());
    BOOST_CHECK_EQUAL("NRM_PARTIAL", daisyProperties.normalization());
    BOOST_CHECK_EQUAL(false, daisyProperties.interpolation());
    BOOST_CHECK_EQUAL(true, daisyProperties.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, DaisyDescriptorTest)
{
    DaisyProperties copy(daisyProperties);
    BOOST_CHECK_EQUAL(10.f, copy.radius());
    BOOST_CHECK_EQUAL(4, copy.radialDivisions());
    BOOST_CHECK_EQUAL(4, copy.angularDivision());
    BOOST_CHECK_EQUAL(4, copy.gradientOrientationsDivision());
    BOOST_CHECK_EQUAL("NRM_PARTIAL", copy.normalization());
    BOOST_CHECK_EQUAL(false, copy.interpolation());
    BOOST_CHECK_EQUAL(true, copy.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(assign, DaisyDescriptorTest)
{
    DaisyProperties assign = daisyProperties;
    BOOST_CHECK_EQUAL(10.f, assign.radius());
    BOOST_CHECK_EQUAL(4, assign.radialDivisions());
    BOOST_CHECK_EQUAL(4, assign.angularDivision());
    BOOST_CHECK_EQUAL(4, assign.gradientOrientationsDivision());
    BOOST_CHECK_EQUAL("NRM_PARTIAL", assign.normalization());
    BOOST_CHECK_EQUAL(false, assign.interpolation());
    BOOST_CHECK_EQUAL(true, assign.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(type, DaisyDescriptorTest)
{
    BOOST_CHECK(Feature::Type::daisy == daisyProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, DaisyDescriptorTest)
{
    BOOST_CHECK_EQUAL("DAISY", daisyProperties.name());
}

BOOST_FIXTURE_TEST_CASE(radius, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setRadius(5.0f);
    BOOST_CHECK_EQUAL(5.0f, daisy.radius());
    daisy.setRadius(20.0f);
    BOOST_CHECK_EQUAL(20.0f, daisy.radius());
}

BOOST_FIXTURE_TEST_CASE(radialDivisions, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setRadialDivisions(1);
    BOOST_CHECK_EQUAL(1, daisy.radialDivisions());
    daisy.setRadialDivisions(50);
    BOOST_CHECK_EQUAL(50, daisy.radialDivisions());
}

BOOST_FIXTURE_TEST_CASE(angularDivision, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setAngularDivision(2);
    BOOST_CHECK_EQUAL(2, daisy.angularDivision());
    daisy.setAngularDivision(30);
    BOOST_CHECK_EQUAL(30, daisy.angularDivision());
}

BOOST_FIXTURE_TEST_CASE(gradientOrientationsDivision, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setGradientOrientationsDivision(3);
    BOOST_CHECK_EQUAL(3, daisy.gradientOrientationsDivision());
    daisy.setGradientOrientationsDivision(20);
    BOOST_CHECK_EQUAL(20, daisy.gradientOrientationsDivision());
}

BOOST_FIXTURE_TEST_CASE(normalization, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setNormalization("NRM_NONE");
    BOOST_CHECK_EQUAL("NRM_NONE", daisy.normalization());
    daisy.setNormalization("NRM_PARTIAL");
    BOOST_CHECK_EQUAL("NRM_PARTIAL", daisy.normalization());
    daisy.setNormalization("NRM_FULL");
    BOOST_CHECK_EQUAL("NRM_FULL", daisy.normalization());
    daisy.setNormalization("NRM_SIFT");
    BOOST_CHECK_EQUAL("NRM_SIFT", daisy.normalization());
    daisy.setNormalization("BAD_VALUE");
    BOOST_CHECK_EQUAL("NRM_SIFT", daisy.normalization());
}

BOOST_FIXTURE_TEST_CASE(interpolation, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setInterpolation(true);
    BOOST_CHECK_EQUAL(true, daisy.interpolation());
    daisy.setInterpolation(false);
    BOOST_CHECK_EQUAL(false, daisy.interpolation());
}

BOOST_FIXTURE_TEST_CASE(useOrientation, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setUseOrientation(true);
    BOOST_CHECK_EQUAL(true, daisy.useOrientation());
    daisy.setUseOrientation(false);
    BOOST_CHECK_EQUAL(false, daisy.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(reset, DaisyDescriptorTest)
{
    DaisyProperties daisy = daisyProperties;
    daisy.reset();
    BOOST_CHECK_EQUAL(15.f, daisy.radius());
    BOOST_CHECK_EQUAL(3, daisy.radialDivisions());
    BOOST_CHECK_EQUAL(8, daisy.angularDivision());
    BOOST_CHECK_EQUAL(8, daisy.gradientOrientationsDivision());
    BOOST_CHECK_EQUAL("NRM_NONE", daisy.normalization());
    BOOST_CHECK_EQUAL(true, daisy.interpolation());
    BOOST_CHECK_EQUAL(false, daisy.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(properties, DaisyDescriptorTest)
{
    DaisyProperties properties = daisyDescriptor->properties();
    
    BOOST_CHECK_EQUAL(10.f, properties.getProperty<float>("Radius"));
    BOOST_CHECK_EQUAL(4, properties.getProperty<int>("QRadius"));
    BOOST_CHECK_EQUAL(4, properties.getProperty<int>("QTheta"));
    BOOST_CHECK_EQUAL(4, properties.getProperty<int>("QHist"));
    BOOST_CHECK_EQUAL("NRM_PARTIAL", properties.getProperty<std::string>("Norm"));
    BOOST_CHECK_EQUAL(false, properties.getProperty<bool>("Interpolation"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("UseOrientation"));

    BOOST_CHECK_EQUAL("10.000000", properties.getPropertyAsString("Radius"));
    BOOST_CHECK_EQUAL("4", properties.getPropertyAsString("QRadius"));
    BOOST_CHECK_EQUAL("4", properties.getPropertyAsString("QTheta"));
    BOOST_CHECK_EQUAL("4", properties.getPropertyAsString("QHist"));
    BOOST_CHECK_EQUAL("NRM_PARTIAL", properties.getPropertyAsString("Norm"));
    BOOST_CHECK_EQUAL("false", properties.getPropertyAsString("Interpolation"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("UseOrientation"));

}


BOOST_AUTO_TEST_SUITE_END() 
