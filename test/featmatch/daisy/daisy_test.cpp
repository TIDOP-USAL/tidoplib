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
        : daisyDescriptor(new DaisyDescriptor(10., 4, 4, 4, "NRM_PARTIAL", false, true))
    { }

    ~DaisyDescriptorTest()
    {
        delete daisyDescriptor;
    }

    void setup()
    {
    }

    void teardown()
    {
    }

    DaisyProperties daisyProperties;
    DaisyDescriptor *daisyDescriptor;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, DaisyDescriptorTest)
{
    BOOST_CHECK_EQUAL(15., daisyProperties.radius());
    BOOST_CHECK_EQUAL(3, daisyProperties.qRadius());
    BOOST_CHECK_EQUAL(8, daisyProperties.qTheta());
    BOOST_CHECK_EQUAL(8, daisyProperties.qHist());
    BOOST_CHECK_EQUAL("NRM_NONE", daisyProperties.norm());
    BOOST_CHECK_EQUAL(true, daisyProperties.interpolation());
    BOOST_CHECK_EQUAL(false, daisyProperties.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(constructor, DaisyDescriptorTest)
{
    BOOST_CHECK_EQUAL(10., daisyDescriptor->radius());
    BOOST_CHECK_EQUAL(4, daisyDescriptor->qRadius());
    BOOST_CHECK_EQUAL(4, daisyDescriptor->qTheta());
    BOOST_CHECK_EQUAL(4, daisyDescriptor->qHist());
    BOOST_CHECK_EQUAL("NRM_PARTIAL", daisyDescriptor->norm());
    BOOST_CHECK_EQUAL(false, daisyDescriptor->interpolation());
    BOOST_CHECK_EQUAL(true, daisyDescriptor->useOrientation());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, DaisyDescriptorTest)
{
    DaisyProperties copy(*daisyDescriptor);
    BOOST_CHECK_EQUAL(10., copy.radius());
    BOOST_CHECK_EQUAL(4, copy.qRadius());
    BOOST_CHECK_EQUAL(4, copy.qTheta());
    BOOST_CHECK_EQUAL(4, copy.qHist());
    BOOST_CHECK_EQUAL("NRM_PARTIAL", copy.norm());
    BOOST_CHECK_EQUAL(false, copy.interpolation());
    BOOST_CHECK_EQUAL(true, copy.useOrientation());
}

BOOST_FIXTURE_TEST_CASE(assign, DaisyDescriptorTest)
{
    DaisyProperties assign = *daisyDescriptor;
    BOOST_CHECK_EQUAL(10., assign.radius());
    BOOST_CHECK_EQUAL(4, assign.qRadius());
    BOOST_CHECK_EQUAL(4, assign.qTheta());
    BOOST_CHECK_EQUAL(4, assign.qHist());
    BOOST_CHECK_EQUAL("NRM_PARTIAL", assign.norm());
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
    daisy.setRadius(5.0);
    BOOST_CHECK_EQUAL(5.0, daisy.radius());
    daisy.setRadius(20.0);
    BOOST_CHECK_EQUAL(20.0, daisy.radius());
}

BOOST_FIXTURE_TEST_CASE(qRadius, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setQRadius(1);
    BOOST_CHECK_EQUAL(1, daisy.qRadius());
    daisy.setQRadius(50);
    BOOST_CHECK_EQUAL(50, daisy.qRadius());
}

BOOST_FIXTURE_TEST_CASE(qTheta, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setQTheta(2);
    BOOST_CHECK_EQUAL(2, daisy.qTheta());
    daisy.setQTheta(30);
    BOOST_CHECK_EQUAL(30, daisy.qTheta());
}

BOOST_FIXTURE_TEST_CASE(qHist, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setQHist(3);
    BOOST_CHECK_EQUAL(3, daisy.qHist());
    daisy.setQHist(20);
    BOOST_CHECK_EQUAL(20, daisy.qHist());
}

BOOST_FIXTURE_TEST_CASE(norm, DaisyDescriptorTest)
{
    DaisyProperties daisy;
    daisy.setNorm("NRM_NONE");
    BOOST_CHECK_EQUAL("NRM_NONE", daisy.norm());
    daisy.setNorm("NRM_PARTIAL");
    BOOST_CHECK_EQUAL("NRM_PARTIAL", daisy.norm());
    daisy.setNorm("NRM_FULL");
    BOOST_CHECK_EQUAL("NRM_FULL", daisy.norm());
    daisy.setNorm("NRM_SIFT");
    BOOST_CHECK_EQUAL("NRM_SIFT", daisy.norm());
    daisy.setNorm("BAD_VALUE");
    BOOST_CHECK_EQUAL("NRM_SIFT", daisy.norm());
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
    DaisyProperties daisy(*daisyDescriptor);
    daisy.reset();
    BOOST_CHECK_EQUAL(15., daisy.radius());
    BOOST_CHECK_EQUAL(3, daisy.qRadius());
    BOOST_CHECK_EQUAL(8, daisy.qTheta());
    BOOST_CHECK_EQUAL(8, daisy.qHist());
    BOOST_CHECK_EQUAL("NRM_NONE", daisy.norm());
    BOOST_CHECK_EQUAL(true, daisy.interpolation());
    BOOST_CHECK_EQUAL(false, daisy.useOrientation());
}

BOOST_AUTO_TEST_SUITE_END() 
