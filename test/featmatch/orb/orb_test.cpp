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
 
#define BOOST_TEST_MODULE Tidop OrbDetectorDescriptor test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/orb.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(OrbDetectorDescriptorTestSuite)

struct OrbDetectorDescriptorTest
{
    OrbDetectorDescriptorTest()
    {
    }

    ~OrbDetectorDescriptorTest()
    {
        delete orbDetectorDescriptor;
    }

    void setup()
    {
        orbProperties.setFeaturesNumber(4000);
        orbProperties.setScaleFactor(1.5);
        orbProperties.setLevelsNumber(6);
        orbProperties.setEdgeThreshold(11);
        orbProperties.setWTA_K(2);
        orbProperties.setScoreType("FAST");
        orbProperties.setPatchSize(11);
        orbProperties.setFastThreshold(10);

        orbDetectorDescriptor = new OrbDetectorDescriptor(orbProperties);
    }

    void teardown()
    {
    }

    OrbProperties orbDefaultProperties;
    OrbProperties orbProperties;
    OrbDetectorDescriptor *orbDetectorDescriptor;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, OrbDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL(5000, orbDefaultProperties.featuresNumber());
    BOOST_CHECK_EQUAL(1.2f, orbDefaultProperties.scaleFactor());
    BOOST_CHECK_EQUAL(8, orbDefaultProperties.levelsNumber());
    BOOST_CHECK_EQUAL(31, orbDefaultProperties.edgeThreshold());
    BOOST_CHECK_EQUAL(2, orbDefaultProperties.wta_k());
    BOOST_CHECK_EQUAL("Harris", orbDefaultProperties.scoreType());
    BOOST_CHECK_EQUAL(31, orbDefaultProperties.patchSize());
    BOOST_CHECK_EQUAL(20, orbDefaultProperties.fastThreshold());
}

BOOST_FIXTURE_TEST_CASE(constructor, OrbDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL(4000, orbProperties.featuresNumber());
    BOOST_CHECK_EQUAL(1.5, orbProperties.scaleFactor());
    BOOST_CHECK_EQUAL(6, orbProperties.levelsNumber());
    BOOST_CHECK_EQUAL(11, orbProperties.edgeThreshold());
    BOOST_CHECK_EQUAL(2, orbProperties.wta_k());
    BOOST_CHECK_EQUAL("FAST", orbProperties.scoreType());
    BOOST_CHECK_EQUAL(11, orbProperties.patchSize());
    BOOST_CHECK_EQUAL(10, orbProperties.fastThreshold());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, OrbDetectorDescriptorTest)
{
    OrbProperties copy(orbProperties);
    BOOST_CHECK_EQUAL(4000, copy.featuresNumber());
    BOOST_CHECK_EQUAL(1.5, copy.scaleFactor());
    BOOST_CHECK_EQUAL(6, copy.levelsNumber());
    BOOST_CHECK_EQUAL(11, copy.edgeThreshold());
    BOOST_CHECK_EQUAL(2, copy.wta_k());
    BOOST_CHECK_EQUAL("FAST", copy.scoreType());
    BOOST_CHECK_EQUAL(11, copy.patchSize());
    BOOST_CHECK_EQUAL(10, copy.fastThreshold());
}

BOOST_FIXTURE_TEST_CASE(assign, OrbDetectorDescriptorTest)
{
    OrbProperties assign = orbProperties;
    BOOST_CHECK_EQUAL(4000, assign.featuresNumber());
    BOOST_CHECK_EQUAL(1.5, assign.scaleFactor());
    BOOST_CHECK_EQUAL(6, assign.levelsNumber());
    BOOST_CHECK_EQUAL(11, assign.edgeThreshold());
    BOOST_CHECK_EQUAL(2, assign.wta_k());
    BOOST_CHECK_EQUAL("FAST", assign.scoreType());
    BOOST_CHECK_EQUAL(11, assign.patchSize());
    BOOST_CHECK_EQUAL(10, assign.fastThreshold());
}

BOOST_FIXTURE_TEST_CASE(type, OrbDetectorDescriptorTest)
{
    BOOST_CHECK(Feature::Type::orb == orbProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, OrbDetectorDescriptorTest)
{
    BOOST_CHECK_EQUAL("ORB", orbProperties.name());
}

BOOST_FIXTURE_TEST_CASE(featuresNumber, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setFeaturesNumber(500);
    BOOST_CHECK_EQUAL(500, orb.featuresNumber());
    orb.setFeaturesNumber(10000);
    BOOST_CHECK_EQUAL(10000, orb.featuresNumber());
}

BOOST_FIXTURE_TEST_CASE(scaleFactor, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setScaleFactor(1.5);
    BOOST_CHECK_EQUAL(1.5, orb.scaleFactor());
    orb.setScaleFactor(2.0);
    BOOST_CHECK_EQUAL(2.0, orb.scaleFactor());
}

BOOST_FIXTURE_TEST_CASE(levelsNumber, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setLevelsNumber(2);
    BOOST_CHECK_EQUAL(2, orb.levelsNumber());
    orb.setLevelsNumber(3);
    BOOST_CHECK_EQUAL(3, orb.levelsNumber());
}

BOOST_FIXTURE_TEST_CASE(edgeThreshold, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setEdgeThreshold(7);
    BOOST_CHECK_EQUAL(7, orb.edgeThreshold());
    orb.setEdgeThreshold(11);
    BOOST_CHECK_EQUAL(11, orb.edgeThreshold());
}

BOOST_FIXTURE_TEST_CASE(firstLevel, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setFirstLevel(1);
    BOOST_CHECK_EQUAL(1, orb.firstLevel());
    orb.setFirstLevel(2);
    BOOST_CHECK_EQUAL(2, orb.firstLevel());
}

BOOST_FIXTURE_TEST_CASE(wta_k, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setWTA_K(3);
    BOOST_CHECK_EQUAL(3, orb.wta_k());
    orb.setWTA_K(4);
    BOOST_CHECK_EQUAL(4, orb.wta_k());
}

BOOST_FIXTURE_TEST_CASE(scoreType, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setScoreType("FAST");
    BOOST_CHECK_EQUAL("FAST", orb.scoreType());
    orb.setScoreType("Harris");
    BOOST_CHECK_EQUAL("Harris", orb.scoreType());
}

BOOST_FIXTURE_TEST_CASE(patchSize, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setPatchSize(2);
    BOOST_CHECK_EQUAL(2, orb.patchSize());
    orb.setPatchSize(3);
    BOOST_CHECK_EQUAL(3, orb.patchSize());
}

BOOST_FIXTURE_TEST_CASE(fastThreshold, OrbDetectorDescriptorTest)
{
    OrbProperties orb;
    orb.setFastThreshold(2);
    BOOST_CHECK_EQUAL(2, orb.fastThreshold());
    orb.setFastThreshold(3);
    BOOST_CHECK_EQUAL(3, orb.fastThreshold());
}

BOOST_FIXTURE_TEST_CASE(reset, OrbDetectorDescriptorTest)
{
    OrbProperties orb = orbProperties;
    orb.reset();
    BOOST_CHECK_EQUAL(5000, orb.featuresNumber());
    BOOST_CHECK_EQUAL(1.2f, orb.scaleFactor());
    BOOST_CHECK_EQUAL(8, orb.levelsNumber());
    BOOST_CHECK_EQUAL(31, orb.edgeThreshold());
    BOOST_CHECK_EQUAL(2, orb.wta_k());
    BOOST_CHECK_EQUAL("Harris", orb.scoreType());
    BOOST_CHECK_EQUAL(31, orb.patchSize());
    BOOST_CHECK_EQUAL(20, orb.fastThreshold());
}

BOOST_FIXTURE_TEST_CASE(orb_detector_descriptor_properties, OrbDetectorDescriptorTest)
{
    auto &properties = orbDetectorDescriptor->properties();

    BOOST_CHECK_EQUAL(4000, properties.getProperty<int>("FeaturesNumber"));
    BOOST_CHECK_EQUAL(1.5f, properties.getProperty<float>("ScaleFactor"));
    BOOST_CHECK_EQUAL(6, properties.getProperty<int>("LevelsNumber"));
    BOOST_CHECK_EQUAL(11, properties.getProperty<int>("EdgeThreshold"));
    BOOST_CHECK_EQUAL(2, properties.getProperty<int>("WTA_K"));
    BOOST_CHECK_EQUAL("FAST", properties.getProperty<std::string>("ScoreType"));
    BOOST_CHECK_EQUAL(11, properties.getProperty<int>("PatchSize"));
    BOOST_CHECK_EQUAL(10, properties.getProperty<int>("FastThreshold"));

    BOOST_CHECK_EQUAL("4000", properties.getPropertyAsString("FeaturesNumber"));
    BOOST_CHECK_EQUAL("1.500000", properties.getPropertyAsString("ScaleFactor"));
    BOOST_CHECK_EQUAL("6", properties.getPropertyAsString("LevelsNumber"));
    BOOST_CHECK_EQUAL("11", properties.getPropertyAsString("EdgeThreshold"));
    BOOST_CHECK_EQUAL("2", properties.getPropertyAsString("WTA_K"));
    BOOST_CHECK_EQUAL("FAST", properties.getPropertyAsString("ScoreType"));
    BOOST_CHECK_EQUAL("11", properties.getPropertyAsString("PatchSize"));
    BOOST_CHECK_EQUAL("10", properties.getPropertyAsString("FastThreshold"));
}

BOOST_AUTO_TEST_SUITE_END()
