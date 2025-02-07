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
 
#define BOOST_TEST_MODULE Tidop SurfDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/surf.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(SurfDetectorDescriptorTestSuite)

struct SurfDetectorDescriptorTest
{
    SurfDetectorDescriptorTest()
    {
    }

    ~SurfDetectorDescriptorTest()
    {
#ifdef OPENCV_ENABLE_NONFREE
        delete surfDetectorDescriptor;
#endif
    }

    void setup()
    {
        surfProperties.setHessianThreshold(50.);
        surfProperties.setOctaves(2);
        surfProperties.setOctaveLayers(5);
        surfProperties.setExtendedDescriptor(true);
        surfProperties.setUpright(true);

#ifdef OPENCV_ENABLE_NONFREE
        surfDetectorDescriptor = new SurfDetectorDescriptor(surfProperties);
#endif
    }

    void teardown()
    {
    }

    SurfProperties surfDefaultProperties;
    SurfProperties surfProperties;
#ifdef OPENCV_ENABLE_NONFREE
    SurfDetectorDescriptor *surfDetectorDescriptor;
#endif

};

BOOST_FIXTURE_TEST_CASE(default_constructor, SurfDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL(100, surfDefaultProperties.hessianThreshold());
    BOOST_CHECK_EQUAL(4, surfDefaultProperties.octaves());
    BOOST_CHECK_EQUAL(3, surfDefaultProperties.octaveLayers());
    BOOST_CHECK_EQUAL(false, surfDefaultProperties.extendedDescriptor());
    BOOST_CHECK_EQUAL(false, surfDefaultProperties.upright());
}

#ifdef OPENCV_ENABLE_NONFREE
BOOST_FIXTURE_TEST_CASE(constructor, SurfDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL(50., surfProperties.hessianThreshold());
    BOOST_CHECK_EQUAL(2, surfProperties.octaves());
    BOOST_CHECK_EQUAL(5, surfProperties.octaveLayers());
    BOOST_CHECK_EQUAL(true, surfProperties.extendedDescriptor());
    BOOST_CHECK_EQUAL(true, surfProperties.upright());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, SurfDetectorDescriptorTest)
{
    SurfProperties copy(surfProperties);
    BOOST_CHECK_EQUAL(50., copy.hessianThreshold());
    BOOST_CHECK_EQUAL(2, copy.octaves());
    BOOST_CHECK_EQUAL(5, copy.octaveLayers());
    BOOST_CHECK_EQUAL(true, copy.extendedDescriptor());
    BOOST_CHECK_EQUAL(true, copy.upright());
}

BOOST_FIXTURE_TEST_CASE(assign, SurfDetectorDescriptorTest)
{
    SurfProperties assign = surfProperties;
    BOOST_CHECK_EQUAL(50., assign.hessianThreshold());
    BOOST_CHECK_EQUAL(2, assign.octaves());
    BOOST_CHECK_EQUAL(5, assign.octaveLayers());
    BOOST_CHECK_EQUAL(true, assign.extendedDescriptor());
    BOOST_CHECK_EQUAL(true, assign.upright());
}
#endif

BOOST_FIXTURE_TEST_CASE(type, SurfDetectorDescriptorTest)
{
    BOOST_CHECK(Feature::Type::surf == surfProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, SurfDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL("SURF", surfProperties.name());
}

BOOST_FIXTURE_TEST_CASE(hessianThreshold, SurfDetectorDescriptorTest)
{
    SurfProperties surf;
    surf.setHessianThreshold(50);
    BOOST_CHECK_EQUAL(50, surf.hessianThreshold());
    surf.setHessianThreshold(100);
    BOOST_CHECK_EQUAL(100, surf.hessianThreshold());
}

BOOST_FIXTURE_TEST_CASE(octaves, SurfDetectorDescriptorTest)
{
    SurfProperties surf;
    surf.setOctaves(4);
    BOOST_CHECK_EQUAL(4, surf.octaves());
    surf.setOctaves(8);
    BOOST_CHECK_EQUAL(8, surf.octaves());
}

BOOST_FIXTURE_TEST_CASE(octaveLayers, SurfDetectorDescriptorTest)
{
    SurfProperties surf;
    surf.setOctaveLayers(3);
    BOOST_CHECK_EQUAL(3, surf.octaveLayers());
    surf.setOctaveLayers(4);
    BOOST_CHECK_EQUAL(4, surf.octaveLayers());
}

BOOST_FIXTURE_TEST_CASE(extendedDescriptor, SurfDetectorDescriptorTest)
{
    SurfProperties surf;
    surf.setExtendedDescriptor(false);
    BOOST_CHECK_EQUAL(false, surf.extendedDescriptor());
    surf.setExtendedDescriptor(true);
    BOOST_CHECK_EQUAL(true, surf.extendedDescriptor());
}

BOOST_FIXTURE_TEST_CASE(upright, SurfDetectorDescriptorTest)
{
    SurfProperties surf;
    surf.setUpright(false);
    BOOST_CHECK_EQUAL(false, surf.upright());
    surf.setUpright(true);
    BOOST_CHECK_EQUAL(true, surf.upright());
}


BOOST_FIXTURE_TEST_CASE(reset, SurfDetectorDescriptorTest)
{
    SurfProperties surf = surfProperties;
    surf.reset();
    BOOST_CHECK_EQUAL(100, surf.hessianThreshold());
    BOOST_CHECK_EQUAL(4, surf.octaves());
    BOOST_CHECK_EQUAL(3, surf.octaveLayers());
    BOOST_CHECK_EQUAL(false, surf.extendedDescriptor());
    BOOST_CHECK_EQUAL(false, surf.upright());
}

#ifdef OPENCV_ENABLE_NONFREE
BOOST_FIXTURE_TEST_CASE(surf_detector_descriptor_properties, SurfDetectorDescriptorTest)
{
    auto &properties = surfDetectorDescriptor->properties();

    BOOST_CHECK_EQUAL(50., properties.getProperty<double>("HessianThreshold"));
    BOOST_CHECK_EQUAL(2, properties.getProperty<int>("Octaves"));
    BOOST_CHECK_EQUAL(5, properties.getProperty<int>("OctaveLayers"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("ExtendedDescriptor"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("Upright"));

    BOOST_CHECK_EQUAL("50.000000", properties.getPropertyAsString("HessianThreshold"));
    BOOST_CHECK_EQUAL("2", properties.getPropertyAsString("Octaves"));
    BOOST_CHECK_EQUAL("5", properties.getPropertyAsString("OctaveLayers"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("ExtendedDescriptor"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("Upright"));
}
#endif

BOOST_AUTO_TEST_SUITE_END()
