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
 
#define BOOST_TEST_MODULE Tidop algorithms equals test
#include <boost/test/unit_test.hpp>
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/analysis/Equals.h"

using namespace tl; 

BOOST_AUTO_TEST_SUITE(EqualsAlgorithmTest)

struct EqualsTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(3.0, 4.0);
        point2d_near = Point2d(1.0 + 1e-12, 2.0 - 1e-12);   // Dentro de tolerancia
        point2d_far = Point2d(1.0 + 1e-8, 2.0 - 1e-8);    // Fuera de tolerancia

        point2dm1 = Point2dm(1.0, 2.0, 10.0);
        point2dm2 = Point2dm(1.0, 2.0, 20.0);
        point2f_near = Point2f(1.0f + FLT_EPSILON / 2, 2.0f - FLT_EPSILON / 2);
        point2f_far = Point2f(1.0f + FLT_EPSILON * 200, 2.0f - FLT_EPSILON * 200);

        point2i1 = Point2i(1, 2);
        point2i2 = Point2i(1, 3);
    }

    void teardown()
    {

    }

    Point2d point2d1;
    Point2d point2d2;
    Point2d point2d_near;
    Point2d point2d_far;
    Point2f point2f1;
    Point2f point2f2;
    Point2f point2f_near;
    Point2f point2f_far;
    Point2dm point2dm1;
    Point2dm point2dm2;
    Point2i point2i1;
    Point2i point2i2;
};

BOOST_FIXTURE_TEST_CASE(Equals_Point2d_SamePoints, EqualsTestFixture)
{
    BOOST_CHECK(equals(point2d1, point2d1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Point2d_DifferentPoints, EqualsTestFixture)
{
    BOOST_CHECK(!equals(point2d1, point2d2));
    BOOST_CHECK(!equals(point2d2, point2d1));
}

BOOST_FIXTURE_TEST_CASE(Equals_PointWithMeasure, EqualsTestFixture)
{    
    BOOST_CHECK(equals(point2dm1, point2dm1));
    BOOST_CHECK(equals(point2dm1, point2dm2)); // Diferente medida (se ignora)
}

BOOST_FIXTURE_TEST_CASE(Equals_PointIntegerTypes, EqualsTestFixture)
{
    BOOST_CHECK(equals(point2i1, point2i1));
    BOOST_CHECK(!equals(point2i1, point2i2));
}

BOOST_FIXTURE_TEST_CASE(Equals_Point2d_OutsideTolerance, EqualsTestFixture)
{
    BOOST_CHECK(!equals(point2d1, point2d_far));
}

BOOST_FIXTURE_TEST_CASE(Equals_Point2f_OutsideTolerance, EqualsTestFixture)
{
    BOOST_CHECK(!equals(point2f1, point2f_far));
}

BOOST_FIXTURE_TEST_CASE(Equals_Point2f_LargeValues, EqualsTestFixture)
{
    // Tolerancia relativa debe adaptarse a números grandes
    Point<float, xy_tag> big{1e6f, 2e6f};
    Point<float, xy_tag> big_near{1e6f + 1e-2f, 2e6f - 1e-2f}; // ~1e-2 en 1e6 = 1e-8 relativo
    BOOST_CHECK(equals(big, big_near));
}

BOOST_FIXTURE_TEST_CASE(Equals_Point2d_IndustrialPrecision, EqualsTestFixture)
{
    TolerancePolicy industrial(CoordinateUnit::meters, 0.000001, 0.000002, 0.000001, 0.000002);

    Point2d nominal{1000.123456, 2000.123456};
    Point2d medido_ok{1000.123457, 2000.123455};   // diferencia ≈ 0.001 mm (1 micra)
    Point2d medido_ko{1000.123500, 2000.123400};   // diferencia ≈ 0.044 mm (44 micras)

    // Tolerancia = 0.002 mm (2 micras)
    BOOST_CHECK(equals(nominal, medido_ok, industrial));
    BOOST_CHECK(!equals(nominal, medido_ko, industrial));

    // El usuario puede ajustar la tolerancia explícitamente
    BOOST_CHECK(equals(nominal, medido_ko, 0.0001));
}

BOOST_FIXTURE_TEST_CASE(Equals_Point2d_Cadastral_1_500, EqualsTestFixture)
{
    TolerancePolicy cadastral(CoordinateUnit::meters, 0.025, 0.025, 0.025, 0.025);

    Point2d p1{100.00, 200.00};
    Point2d p2{100.02, 200.03};

    BOOST_CHECK(!equals(p1, p2, cadastral));
}

BOOST_FIXTURE_TEST_CASE(Equals_PointGeographic_WGS84, EqualsTestFixture)
{
    TolerancePolicy wgs84 = TolerancePolicy::geographicDefault();

    Point<double, xy_tag> p1{-3.70000000, 40.50000000};
    Point<double, xy_tag> p2{-3.70000001, 40.50000001};

    BOOST_CHECK(equals(p1, p2, wgs84));
}

BOOST_AUTO_TEST_SUITE_END()