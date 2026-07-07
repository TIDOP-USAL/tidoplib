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

#define BOOST_TEST_MODULE Tidop graphic polygon test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/entities/GPolygon.h>
#include <tidop/graphic/entities/GPolygon3D.h>
#include <tidop/graphic/entities/GMultiPolygon.h>
#include <tidop/graphic/entities/GMultiPolygon3D.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(GPolygonTestSuite)

struct GPolygonTest
{

    GPolygonTest()
    {
    }

    ~GPolygonTest()
    {
    }

    GPolygon polygon;
    GPolygon3D polygon_3d;
    GMultiPolygon multi_polygon;
    GMultiPolygon3D multi_polygon_3d;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, GPolygonTest)
{
    BOOST_CHECK(polygon.type() == GraphicEntity::Type::polygon_2d);
    BOOST_CHECK(polygon_3d.type() == GraphicEntity::Type::polygon_3d);
    BOOST_CHECK(multi_polygon.type() == GraphicEntity::Type::multipolygon_2d);
    BOOST_CHECK(multi_polygon_3d.type() == GraphicEntity::Type::multipolygon_3d);
}


BOOST_AUTO_TEST_SUITE_END()