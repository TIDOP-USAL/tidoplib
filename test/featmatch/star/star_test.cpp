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
 
#define BOOST_TEST_MODULE Tidop StarDetector test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/star.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(StarDetectorTestSuite)

struct StarDetectorTest
{
    StarDetectorTest()
    {
    }

    ~StarDetectorTest()
    {
        delete starDetector;
    }

    void setup()
    {
        starProperties.setMaxSize(35);
        starProperties.setResponseThreshold(20);
        starProperties.setLineThresholdProjected(20);
        starProperties.setLineThresholdBinarized(10);
        starProperties.setSuppressNonmaxSize(20);
        starDetector = new StarDetector(starProperties);
    }

    void teardown()
    {
    }

    StarProperties starDefaultProperties;
    StarProperties starProperties;
    StarDetector *starDetector;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, StarDetectorTest)
{
    BOOST_CHECK_EQUAL(45, starDefaultProperties.maxSize());
    BOOST_CHECK_EQUAL(30, starDefaultProperties.responseThreshold());
    BOOST_CHECK_EQUAL(10, starDefaultProperties.lineThresholdProjected());
    BOOST_CHECK_EQUAL(8, starDefaultProperties.lineThresholdBinarized());
    BOOST_CHECK_EQUAL(5, starDefaultProperties.suppressNonmaxSize());
}

BOOST_FIXTURE_TEST_CASE(constructor, StarDetectorTest)
{
    BOOST_CHECK_EQUAL(35, starProperties.maxSize());
    BOOST_CHECK_EQUAL(20, starProperties.responseThreshold());
    BOOST_CHECK_EQUAL(20, starProperties.lineThresholdProjected());
    BOOST_CHECK_EQUAL(10, starProperties.lineThresholdBinarized());
    BOOST_CHECK_EQUAL(20, starProperties.suppressNonmaxSize());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, StarDetectorTest)
{
    StarProperties copy(starProperties);
    BOOST_CHECK_EQUAL(35, copy.maxSize());
    BOOST_CHECK_EQUAL(20, copy.responseThreshold());
    BOOST_CHECK_EQUAL(20, copy.lineThresholdProjected());
    BOOST_CHECK_EQUAL(10, copy.lineThresholdBinarized());
    BOOST_CHECK_EQUAL(20, copy.suppressNonmaxSize());
}

BOOST_FIXTURE_TEST_CASE(assign, StarDetectorTest)
{
    StarProperties assign = starProperties;
    BOOST_CHECK_EQUAL(35, assign.maxSize());
    BOOST_CHECK_EQUAL(20, assign.responseThreshold());
    BOOST_CHECK_EQUAL(20, assign.lineThresholdProjected());
    BOOST_CHECK_EQUAL(10, assign.lineThresholdBinarized());
    BOOST_CHECK_EQUAL(20, assign.suppressNonmaxSize());
}

BOOST_FIXTURE_TEST_CASE(type, StarDetectorTest)
{
    BOOST_CHECK(Feature::Type::star == starProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, StarDetectorTest)
{
    BOOST_CHECK_EQUAL("STAR", starProperties.name());
}

BOOST_FIXTURE_TEST_CASE(maxSize, StarDetectorTest)
{
    StarProperties star;
    star.setMaxSize(5);
    BOOST_CHECK_EQUAL(5, star.maxSize());
    star.setMaxSize(20);
    BOOST_CHECK_EQUAL(20, star.maxSize());
}

BOOST_FIXTURE_TEST_CASE(responseThreshold, StarDetectorTest)
{
    StarProperties star;
    star.setResponseThreshold(11);
    BOOST_CHECK_EQUAL(11, star.responseThreshold());
    star.setResponseThreshold(20);
    BOOST_CHECK_EQUAL(20, star.responseThreshold());
}

BOOST_FIXTURE_TEST_CASE(lineThresholdProjected, StarDetectorTest)
{
    StarProperties star;
    star.setLineThresholdProjected(7);
    BOOST_CHECK_EQUAL(7, star.lineThresholdProjected());
    star.setLineThresholdProjected(13);
    BOOST_CHECK_EQUAL(13, star.lineThresholdProjected());
}

BOOST_FIXTURE_TEST_CASE(suppressNonmaxSize, StarDetectorTest)
{
    StarProperties star;
    star.setSuppressNonmaxSize(10);
    BOOST_CHECK_EQUAL(10, star.suppressNonmaxSize());
    star.setSuppressNonmaxSize(15);
    BOOST_CHECK_EQUAL(15, star.suppressNonmaxSize());
}

BOOST_FIXTURE_TEST_CASE(reset, StarDetectorTest)
{
    StarProperties star = starProperties;
    star.reset();
    BOOST_CHECK_EQUAL(45, star.maxSize());
    BOOST_CHECK_EQUAL(30, star.responseThreshold());
    BOOST_CHECK_EQUAL(10, star.lineThresholdProjected());
    BOOST_CHECK_EQUAL(8, star.lineThresholdBinarized());
    BOOST_CHECK_EQUAL(5, star.suppressNonmaxSize());
}

BOOST_FIXTURE_TEST_CASE(star_detector_properties, StarDetectorTest)
{
    auto &properties = starDetector->properties();

    BOOST_CHECK_EQUAL(35, properties.getProperty<int>("MaxSize"));
    BOOST_CHECK_EQUAL(20, properties.getProperty<int>("ResponseThreshold"));
    BOOST_CHECK_EQUAL(20, properties.getProperty<int>("LineThresholdProjected"));
    BOOST_CHECK_EQUAL(10, properties.getProperty<int>("LineThresholdBinarized"));
    BOOST_CHECK_EQUAL(20, properties.getProperty<int>("SuppressNonmaxSize"));

    BOOST_CHECK_EQUAL("35", properties.getPropertyAsString("MaxSize"));
    BOOST_CHECK_EQUAL("20", properties.getPropertyAsString("ResponseThreshold"));
    BOOST_CHECK_EQUAL("20", properties.getPropertyAsString("LineThresholdProjected"));
    BOOST_CHECK_EQUAL("10", properties.getPropertyAsString("LineThresholdBinarized"));
    BOOST_CHECK_EQUAL("20", properties.getPropertyAsString("SuppressNonmaxSize"));
}

BOOST_AUTO_TEST_SUITE_END()
