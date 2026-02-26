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
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/algorithms/analysis/Equals.h"

using namespace tl; 

BOOST_AUTO_TEST_SUITE(EqualsAlgorithmTest)

struct EqualsTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(3.0, 4.0);
        point2d_near = Point2d(1.0 + 1e-13, 2.0 - 1e-13);   // Dentro de tolerancia
        point2d_far = Point2d(1.0 + 1e-8, 2.0 - 1e-8);    // Fuera de tolerancia

        point2dm1 = Point2dm(1.0, 2.0, 10.0);
        point2dm2 = Point2dm(1.0, 2.0, 20.0);
        point2f_near = Point2f(1.0f + FLT_EPSILON / 2, 2.0f - FLT_EPSILON / 2);
        point2f_far = Point2f(1.0f + FLT_EPSILON * 200, 2.0f - FLT_EPSILON * 200);

        point2i1 = Point2i(1, 2);
        point2i2 = Point2i(1, 3);

        // Segmentos 2D
        seg2d_same1 = Segment<Point2d>(point2d1, point2d2);
        seg2d_same2 = Segment<Point2d>(point2d1, point2d2);
        seg2d_reversed = Segment<Point2d>(point2d2, point2d1);
        seg2d_different = Segment<Point2d>(point2d1, point2d_near); // diferente
        seg2d_near = Segment<Point2d>(point2d1, point2d_near);
        seg2d_far = Segment<Point2d>(point2d1, point2d_far);
        seg2d_zero1 = Segment<Point2d>(point2d1, point2d1);
        seg2d_zero2 = Segment<Point2d>(point2d1, point2d1);

        // Segmentos con medida
        seg2dm_same1 = Segment<Point2dm>(point2dm1, point2dm2);
        seg2dm_same2 = Segment<Point2dm>(point2dm1, point2dm2);
        seg2dm_reversed = Segment<Point2dm>(point2dm2, point2dm1);
        seg2dm_different = Segment<Point2dm>(point2dm1, point2dm1); // mismo punto

        // Segmentos enteros
        seg2i_same1 = Segment<Point2i>(point2i1, point2i2);
        seg2i_same2 = Segment<Point2i>(point2i1, point2i2);
        seg2i_different = Segment<Point2i>(point2i1, point2i1);
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

    // Segmentos 2D
    Segment<Point2d> seg2d_same1;
    Segment<Point2d> seg2d_same2;
    Segment<Point2d> seg2d_reversed;
    Segment<Point2d> seg2d_different;
    Segment<Point2d> seg2d_near;
    Segment<Point2d> seg2d_far;
    Segment<Point2d> seg2d_zero1;
    Segment<Point2d> seg2d_zero2;

    // Segmentos con medida
    Segment<Point2dm> seg2dm_same1;
    Segment<Point2dm> seg2dm_same2;
    Segment<Point2dm> seg2dm_reversed;
    Segment<Point2dm> seg2dm_different;

    // Segmentos enteros
    Segment<Point2i> seg2i_same1;
    Segment<Point2i> seg2i_same2;
    Segment<Point2i> seg2i_different;
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

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_Same, EqualsTestFixture)
{
    BOOST_CHECK(equals(seg2d_same1, seg2d_same2));
    BOOST_CHECK(equals(seg2d_same2, seg2d_same1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_SameReversed, EqualsTestFixture)
{
    BOOST_CHECK(equals(seg2d_same1, seg2d_reversed));
    BOOST_CHECK(equals(seg2d_reversed, seg2d_same1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_Different, EqualsTestFixture)
{
    BOOST_CHECK(!equals(seg2d_same1, seg2d_different));
    BOOST_CHECK(!equals(seg2d_different, seg2d_same1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_WithinTolerance, EqualsTestFixture)
{
    // point2d_near está dentro de la tolerancia por defecto (1e-12)
    Segment<Point2d> seg1(point2d1, point2d2);
    Segment<Point2d> seg2(point2d_near, point2d2);
    BOOST_CHECK(equals(seg1, seg2));
    BOOST_CHECK(equals(seg2, seg1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_OutsideTolerance, EqualsTestFixture)
{
    Segment<Point2d> seg1(point2d1, point2d2);
    Segment<Point2d> seg2(point2d1, point2d_far);
    BOOST_CHECK(!equals(seg1, seg2));
    BOOST_CHECK(!equals(seg2, seg1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_ZeroLength, EqualsTestFixture)
{
    Segment<Point2d> zeroA(point2d1, point2d1);
    Segment<Point2d> zeroB(point2d1, point2d1);
    BOOST_CHECK(equals(zeroA, zeroB));
    BOOST_CHECK(equals(zeroB, zeroA));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_WithMeasure, EqualsTestFixture)
{
    // Misma coordenada espacial, diferente medida → deben ser iguales
    Segment<Point2dm> seg1(point2dm1, point2dm2); // (1,2,10) y (1,2,20)
    Segment<Point2dm> seg2(point2dm1, point2dm2);
    BOOST_CHECK(equals(seg1, seg2));
    BOOST_CHECK(equals(seg2, seg1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_MixedMeasure, EqualsTestFixture)
{
    // Un segmento con puntos sin medida, otro con puntos con medida (misma coordenada)
    Point2d p1(1.0, 2.0);
    Point2d p2(3.0, 4.0);
    Point2dm pm1(1.0, 2.0, 10.0);
    Point2dm pm2(3.0, 4.0, 20.0);

    Segment<Point2d> seg_no_m(p1, p2);
    Segment<Point2dm> seg_with_m(pm1, pm2);

    BOOST_CHECK(equals(seg_no_m, seg_with_m));
    BOOST_CHECK(equals(seg_with_m, seg_no_m));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2i_Same, EqualsTestFixture)
{
    BOOST_CHECK(equals(seg2i_same1, seg2i_same2));
    BOOST_CHECK(equals(seg2i_same2, seg2i_same1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2i_Different, EqualsTestFixture)
{
    BOOST_CHECK(!equals(seg2i_same1, seg2i_different));
    BOOST_CHECK(!equals(seg2i_different, seg2i_same1));
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_CustomTolerance, EqualsTestFixture)
{
    Point2d base(1.0, 2.0);
    Point2d close_enough(1.0 + 1e-9, 2.0 - 1e-9); // 1e-9
    Point2d too_far(1.0 + 1e-7, 2.0 - 1e-7);      // 1e-7

    Segment<Point2d> segA(base, point2d2);
    Segment<Point2d> segB(close_enough, point2d2);
    Segment<Point2d> segC(too_far, point2d2);

    // Tolerancia 1e-8: close_enough está dentro, too_far está fuera
    BOOST_CHECK(equals(segA, segB, 1e-8));
    BOOST_CHECK(!equals(segA, segC, 1e-8));

    // Tolerancia 1e-6: ambos dentro
    BOOST_CHECK(equals(segA, segB, 1e-6));
    BOOST_CHECK(equals(segA, segC, 1e-6));
}

// Casos que NO deben compilar (comentados)
// Descomentar solo para comprobar que no compilan

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_Segment3d_ShouldNotCompile, EqualsTestFixture)
{
    Segment<Point2d> seg2d(point2d1, point2d2);
    Segment<Point3d> seg3d(Point3d(1,2,0), Point3d(3,4,0));
    // equals(seg2d, seg3d); // Error: different spatial dimension
}

BOOST_FIXTURE_TEST_CASE(Equals_Segment2d_Segment2f_ShouldNotCompile, EqualsTestFixture)
{
    Segment<Point2d> seg_double(point2d1, point2d2);
    Segment<Point2f> seg_float(Point2f(1,2), Point2f(3,4));
    //equals(seg_double, seg_float); // Error: different coordinate type
}



BOOST_AUTO_TEST_SUITE_END()