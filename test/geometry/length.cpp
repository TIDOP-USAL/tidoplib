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
 
#define BOOST_TEST_MODULE Tidop algorithms length test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/algorithms/measurement/Length.h>
#include <tidop/geometry/primitives/Segment.h>


using namespace tl; 
using namespace geometry;


BOOST_AUTO_TEST_SUITE(LengthAlgorithmsTestSuite)

struct LengthAlgorithms
{
    LengthAlgorithms()
    {

    }

    ~LengthAlgorithms()
    {

    }

    void setup()
    {
        segment = Segment<geometry::Point2d>(geometry::Point2d(56.23, 123.5), geometry::Point2d(96.2, 34.4));
    }

    void teardown()
    {

    }

    geometry::Point2d point;
    Segment<geometry::Point2d> segment;
};


BOOST_FIXTURE_TEST_CASE(distance, LengthAlgorithms)
{
    BOOST_CHECK_CLOSE(0., tl::geometry::length(point), 0.1);
    BOOST_CHECK_CLOSE(97.6545, tl::geometry::length(segment), 0.1);
}


BOOST_AUTO_TEST_SUITE_END()