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
 
#define BOOST_TEST_MODULE Tidop algorithms overlaps test
#include <boost/test/unit_test.hpp>
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/analysis/Overlaps.h"

using namespace tl; 

BOOST_AUTO_TEST_SUITE(OverlapsAlgorithmTest)


struct OverlapsTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(3.0, 4.0);
        point3d1 = Point3d(1.0, 2.0, 3.0);
        point3d2 = Point3d(4.0, 5.0, 6.0);
        point2dm1 = Point2dm(1.0, 2.0, 10.0);
        point2dm2 = Point2dm(3.0, 4.0, 20.0);
    }

    void teardown()
    {

    }

    Point2d point2d1;
    Point2d point2d2;
    Point3d point3d1;
    Point3d point3d2;
    Point2dm point2dm1;
    Point2dm point2dm2;
};


BOOST_FIXTURE_TEST_CASE(Overlaps_Point_Point_Different, OverlapsTestFixture)
{
    BOOST_TEST(!overlaps(point2d1, point2d2));
    BOOST_TEST(!overlaps(point2d2, point2d1)); // Simetría
}

BOOST_FIXTURE_TEST_CASE(Overlaps_Point_Point_Same, OverlapsTestFixture)
{
    // Un punto no se superpone consigo mismo según OGC
    // (overlaps requiere que las geometrías tengan la misma dimensión
    // y que su intersección tenga dimensión menor que ambas)
    BOOST_TEST(!overlaps(point2d1, point2d1));
}

BOOST_FIXTURE_TEST_CASE(Overlaps_Point3D, OverlapsTestFixture)
{
	static constexpr double EPSILON = 1e-10;
	
    Point3d p2{1.0, 2.0, 3.0 + EPSILON/2};
    
    // Puntos casi iguales pero no superpuestos
    BOOST_TEST(!overlaps(point3d1, p2));
}

// Este caso no tiene que estar permitido pero si un punto tiene medidas si
//BOOST_FIXTURE_TEST_CASE(Overlaps_DifferentDimensions, OverlapsTestFixture)
//{
//    Point2D p2d{1.0, 2.0};
//    Point3D p3d{1.0, 2.0, 0.0};
//    
//    // Geometrías con dimensiones diferentes no pueden superponerse
//    BOOST_TEST(!algorithms::overlaps(p2d, p3d));
//}

BOOST_AUTO_TEST_SUITE_END()