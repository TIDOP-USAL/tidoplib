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
 
#define BOOST_TEST_MODULE Tidop algorithms envelope test
#include <boost/test/unit_test.hpp>
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

using namespace tl; 

BOOST_AUTO_TEST_SUITE(EnvelopeAlgorithmTest)


struct EnvelopeTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(-3.5, -4.2);
        point3d1 = Point3d(1.0, 2.0, 3.0);
        point3d2 = Point3d(4.0, 5.0, 6.0);
        point2dm1 = Point2dm(2.0, 3.0, 10.0);
        point2dm2 = Point2dm(3.0, 4.0, 20.0);
		point2i = Point2i(5, 10);
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
	Point2i point2i;
};

BOOST_FIXTURE_TEST_CASE(Envelope_Point2D, EnvelopeTestFixture)
{
    auto bbox = envelope(point2d1);
    
    // El envelope de un punto es una bounding box degenerada
    BOOST_TEST(bbox.min().x() == point2d1.x());
    BOOST_TEST(bbox.min().y() == point2d1.y());
    BOOST_TEST(bbox.max().x() == point2d1.x());
    BOOST_TEST(bbox.max().y() == point2d1.y());
    
    // Verificar dimensiones
    BOOST_TEST(bbox.width() == 0.0);
    BOOST_TEST(bbox.height() == 0.0);
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point3D, EnvelopeTestFixture)
{
    auto bbox = envelope(point3d1);
    
    // Para 3D, también es degenerado
    BOOST_TEST(bbox.min().x() == point3d1.x());
    BOOST_TEST(bbox.min().y() == point3d1.y());
    BOOST_TEST(bbox.min().z() == point3d1.z());
    BOOST_TEST(bbox.max().x() == point3d1.x());
    BOOST_TEST(bbox.max().y() == point3d1.y());
    BOOST_TEST(bbox.max().z() == point3d1.z());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point2D_WithMeasure, EnvelopeTestFixture)
{
    auto bbox = envelope(point2dm1);
    
    // El envelope no debe incluir la medida
    BOOST_TEST(bbox.min().x() == point2dm1.x());
    BOOST_TEST(bbox.min().y() == point2dm1.y());
    BOOST_TEST(bbox.max().x() == point2dm1.x());
    BOOST_TEST(bbox.max().y() == point2dm1.y());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point_Integer, EnvelopeTestFixture)
{
    auto bbox = envelope(point2i);
    
    BOOST_TEST(bbox.min().x() == point2i.x());
    BOOST_TEST(bbox.min().y() == point2i.y());
    BOOST_TEST(bbox.max().x() == point2i.x());
    BOOST_TEST(bbox.max().y() == point2i.y());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point_NegativeCoordinates, EnvelopeTestFixture)
{
    auto bbox = envelope(point2d2);
    
    BOOST_TEST(bbox.min().x() == point2d2.x());
    BOOST_TEST(bbox.min().y() == point2d2.y());
    BOOST_TEST(bbox.max().x() == point2d2.x());
    BOOST_TEST(bbox.max().y() == point2d2.y());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point_Zero, EnvelopeTestFixture)
{
    Point2d p{0.0, 0.0};
    
    auto bbox = envelope(p);
    
    BOOST_TEST(bbox.min().x() == 0.0);
    BOOST_TEST(bbox.min().y() == 0.0);
    BOOST_TEST(bbox.max().x() == 0.0);
    BOOST_TEST(bbox.max().y() == 0.0);
}

BOOST_AUTO_TEST_SUITE_END()