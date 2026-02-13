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
 
#define BOOST_TEST_MODULE Tidop algorithms within test
#include <boost/test/unit_test.hpp>
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/analysis/Within.h"

using namespace tl; 

BOOST_AUTO_TEST_SUITE(WithinAlgorithmTest)


struct WithinTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(3.0, 4.0);
    }

    void teardown()
    {

    }

    Point2d point2d1;
    Point2d point2d2;
};


BOOST_FIXTURE_TEST_CASE(Within_Point2D_SamePoint, WithinTestFixture)
{    
    BOOST_CHECK(within(point2d1, point2d1));
}

BOOST_FIXTURE_TEST_CASE(Within_Point2D_DifferentPoints, WithinTestFixture)
{   
    BOOST_CHECK(!within(point2d1, point2d2));
    BOOST_CHECK(!within(point2d2, point2d1));
}

BOOST_AUTO_TEST_SUITE_END()