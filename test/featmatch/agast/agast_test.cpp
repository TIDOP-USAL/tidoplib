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

#define BOOST_TEST_MODULE Tidop AgastDetector test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/features/agast.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(AgastDetectorTestSuite)

struct AgastDetectorTest
{
    AgastDetectorTest()
    { }

    ~AgastDetectorTest()
    {
        delete agastDetector;
    }

    void setup()
    {
        agastProperties.setThreshold(15);
        agastProperties.setNonmaxSuppression(true);
        agastProperties.setDetectorType("AGAST_7_12s");

        agastDetector = new AgastDetector(agastProperties);
    }

    void teardown()
    {
    }

    AgastProperties agastDefaultProperties;
    AgastProperties agastProperties;
    AgastDetector *agastDetector;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, AgastDetectorTest)
{
    BOOST_CHECK_EQUAL(10, agastDefaultProperties.threshold());
    BOOST_CHECK_EQUAL("OAST_9_16", agastDefaultProperties.detectorType());
    BOOST_CHECK_EQUAL(true, agastDefaultProperties.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(constructor, AgastDetectorTest)
{
    BOOST_CHECK_EQUAL(15, agastProperties.threshold());
    BOOST_CHECK_EQUAL("AGAST_7_12s", agastProperties.detectorType());
    BOOST_CHECK_EQUAL(true, agastProperties.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, AgastDetectorTest)
{
    AgastProperties agast_copy(agastProperties);
    BOOST_CHECK_EQUAL(15, agast_copy.threshold());
    BOOST_CHECK_EQUAL("AGAST_7_12s", agast_copy.detectorType());
    BOOST_CHECK_EQUAL(true, agast_copy.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(assign, AgastDetectorTest)
{
    AgastProperties agast_assign = agastProperties;
    BOOST_CHECK_EQUAL(15, agast_assign.threshold());
    BOOST_CHECK_EQUAL("AGAST_7_12s", agast_assign.detectorType());
    BOOST_CHECK_EQUAL(true, agast_assign.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(type, AgastDetectorTest)
{
    BOOST_CHECK(Feature::Type::agast == agastProperties.type());
}

BOOST_FIXTURE_TEST_CASE(name, AgastDetectorTest)
{
    BOOST_CHECK_EQUAL("AGAST", agastProperties.name());
}

BOOST_FIXTURE_TEST_CASE(threshold, AgastDetectorTest)
{
    AgastProperties agast;
    agast.setThreshold(7);
    BOOST_CHECK_EQUAL(7, agast.threshold());
}

BOOST_FIXTURE_TEST_CASE(detectorType, AgastDetectorTest)
{
    AgastProperties agast;
    agast.setDetectorType("AGAST_5_8");
    BOOST_CHECK_EQUAL("AGAST_5_8", agast.detectorType());
}

BOOST_FIXTURE_TEST_CASE(nonmaxSuppression, AgastDetectorTest)
{
    AgastProperties agast;
    agast.setNonmaxSuppression(false);
    BOOST_CHECK_EQUAL(false, agast.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(reset, AgastDetectorTest)
{
    AgastProperties agast;
    agast.setNonmaxSuppression(false);
    agast.setDetectorType("AGAST_5_8");
    agast.setThreshold(7);
    agast.reset();
    BOOST_CHECK_EQUAL(10, agast.threshold());
    BOOST_CHECK_EQUAL("OAST_9_16", agast.detectorType());
    BOOST_CHECK_EQUAL(true, agast.nonmaxSuppression());
}

BOOST_FIXTURE_TEST_CASE(agast_detector_properties, AgastDetectorTest)
{
    auto &properties = agastDetector->properties();

    BOOST_CHECK_EQUAL(15, properties.getProperty<int>("Threshold"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("NonmaxSuppression"));
    BOOST_CHECK_EQUAL("AGAST_7_12s", properties.getProperty<std::string>("DetectorType"));

    BOOST_CHECK_EQUAL("15", properties.getPropertyAsString("Threshold"));
    BOOST_CHECK_EQUAL("true", properties.getPropertyAsString("NonmaxSuppression"));
    BOOST_CHECK_EQUAL("AGAST_7_12s", properties.getPropertyAsString("DetectorType"));
}

BOOST_AUTO_TEST_SUITE_END()
