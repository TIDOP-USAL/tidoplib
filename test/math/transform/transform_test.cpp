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
 
#define BOOST_TEST_MODULE Tidop transform test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/transform.h>
#include <tidop/math/algebra/matrix.h>

#include <tidop/math/algebra/scaling.h>
#include <tidop/math/algebra/umeyama.h>

using namespace tl;
using namespace tl::math;

BOOST_AUTO_TEST_SUITE(TransformTestSuite)

struct TransformTest
{

    TransformTest() {}

    ~TransformTest() {}

    void setup()
    {

        src_mat = {{2441775.419, 799268.100, 5818729.162},
                   {3464655.838, 845749.989, 5270271.528},
                   {3309991.828, 828932.118, 5370882.280},
                   {3160763.338, 759160.187, 5469345.504},
                   {2248123.493, 865686.595, 5886425.596},
                   {3022573.157, 802945.690, 5540683.951},
                   {3104219.427, 998384.028, 5463290.505},
                   {2998189.685, 931451.634, 5533398.462},
                   {3199093.294, 932231.327, 5420322.483},
                   {3370658.823, 711876.990, 5349786.786},
                   {3341340.173, 957912.343, 5330003.236},
                   {2534031.166, 975174.455, 5752078.309},
                   {2838909.903, 903822.098, 5620660.184},
                   {2902495.079, 761455.843, 5609859.672},
                   {2682407.890, 950395.934, 5688993.082},
                   {2620258.868, 779138.041, 5743799.267},
                   {3246470.535, 1077900.355, 5365277.896},
                   {3249408.275, 692757.965, 5426396.948},
                   {2763885.496, 733247.387, 5682653.347},
                   {2368885.005, 994492.233, 5818478.154}};

        dst_mat = {{2441276.712, 799286.666, 5818162.025},
                   {3464161.275, 845805.461, 5269712.429},
                   {3309496.800, 828981.942, 5370322.060},
                   {3160269.913, 759204.574, 5468784.081},
                   {2247621.426, 865698.413, 5885856.498},
                   {3022077.340, 802985.055, 5540121.276},
                   {3103716.966, 998426.412, 5462727.814},
                   {2997689.029, 931490.201, 5532835.154},
                   {3198593.776, 932277.179, 5419760.966},
                   {3370168.626, 711928.884, 5349227.574},
                   {3340840.578, 957963.383, 5329442.724},
                   {2533526.497, 975196.347, 5751510.935},
                   {2838409.359, 903854.897, 5620095.593},
                   {2902000.172, 761490.908, 5609296.343},
                   {2681904.794, 950423.098, 5688426.909},
                   {2619761.810, 779162.964, 5743233.630},
                   {3245966.134, 1077947.976, 5364716.214},
                   {3248918.041, 692805.543, 5425836.841},
                   {2763390.878, 733277.458, 5682089.111},
                   {2368378.937, 994508.273, 5817909.286}};

        src_mat2 = {{74.19025779187848, 8.749411901636444, -18.612330072771144},
                    {74.36043674951422,8.749411901636464, -2.045092574570816},
                    {91.2480433111534, 8.749411901636464, -1.961327929447604},
                    {91.60235952827264, 8.749411901636464, -19.02572480319629}};

        dst_mat2 = {{65.76787270674683, -2.842170943040401e-14, -24.428301993342885},
                    {70.61906389289197, 0, 6.908026593137352},
                    {99.47031667189891, 0, 3.111022232462366},
                    {95.375769367781, -2.842170943040401e-14, -28.312814463968905}};

        src_points = {Point3<double>(2441775.419, 799268.100, 5818729.162),
                      Point3<double>(3464655.838, 845749.989, 5270271.528),
                      Point3<double>(3309991.828, 828932.118, 5370882.280),
                      Point3<double>(3160763.338, 759160.187, 5469345.504),
                      Point3<double>(2248123.493, 865686.595, 5886425.596),
                      Point3<double>(3022573.157, 802945.690, 5540683.951),
                      Point3<double>(3104219.427, 998384.028, 5463290.505),
                      Point3<double>(2998189.685, 931451.634, 5533398.462),
                      Point3<double>(3199093.294, 932231.327, 5420322.483),
                      Point3<double>(3370658.823, 711876.990, 5349786.786),
                      Point3<double>(3341340.173, 957912.343, 5330003.236),
                      Point3<double>(2534031.166, 975174.455, 5752078.309),
                      Point3<double>(2838909.903, 903822.098, 5620660.184),
                      Point3<double>(2902495.079, 761455.843, 5609859.672),
                      Point3<double>(2682407.890, 950395.934, 5688993.082),
                      Point3<double>(2620258.868, 779138.041, 5743799.267),
                      Point3<double>(3246470.535, 1077900.355, 5365277.896),
                      Point3<double>(3249408.275, 692757.965, 5426396.948),
                      Point3<double>(2763885.496, 733247.387, 5682653.347),
                      Point3<double>(2368885.005, 994492.233, 5818478.154)};

        dst_points = {Point3<double>(2441276.712, 799286.666, 5818162.025),
                      Point3<double>(3464161.275, 845805.461, 5269712.429),
                      Point3<double>(3309496.800, 828981.942, 5370322.060),
                      Point3<double>(3160269.913, 759204.574, 5468784.081),
                      Point3<double>(2247621.426, 865698.413, 5885856.498),
                      Point3<double>(3022077.340, 802985.055, 5540121.276),
                      Point3<double>(3103716.966, 998426.412, 5462727.814),
                      Point3<double>(2997689.029, 931490.201, 5532835.154),
                      Point3<double>(3198593.776, 932277.179, 5419760.966),
                      Point3<double>(3370168.626, 711928.884, 5349227.574),
                      Point3<double>(3340840.578, 957963.383, 5329442.724),
                      Point3<double>(2533526.497, 975196.347, 5751510.935),
                      Point3<double>(2838409.359, 903854.897, 5620095.593),
                      Point3<double>(2902000.172, 761490.908, 5609296.343),
                      Point3<double>(2681904.794, 950423.098, 5688426.909),
                      Point3<double>(2619761.810, 779162.964, 5743233.630),
                      Point3<double>(3245966.134, 1077947.976, 5364716.214),
                      Point3<double>(3248918.041, 692805.543, 5425836.841),
                      Point3<double>(2763390.878, 733277.458, 5682089.111),
                      Point3<double>(2368378.937, 994508.273, 5817909.286)};

        src_points_utm = {Point<double>(4157222.543, 664789.307),
                          Point<double>(4149043.336, 688836.443),
                          Point<double>(4172803.511, 690340.078),
                          Point<double>(4177148.376, 642997.635),
                          Point<double>(4137012.190, 671808.029),
                          Point<double>(4146292.729, 666952.887),
                          Point<double>(4138759.902, 702670.738)};

        dst_points_utm = {Point<double>(4157372.543, 664864.307),
                          Point<double>(4149193.336, 688911.443),
                          Point<double>(4172953.511, 690415.078),
                          Point<double>(4177298.376, 643072.635),
                          Point<double>(4137162.19, 671883.029),
                          Point<double>(4146442.729, 667027.887),
                          Point<double>(4138909.902, 702745.738) };

        src_vector_utm = {Vector2d{{4157222.543, 664789.307}},
                          Vector2d{{4149043.336, 688836.443}},
                          Vector2d{{4172803.511, 690340.078}},
                          Vector2d{{4177148.376, 642997.635}},
                          Vector2d{{4137012.190, 671808.029}},
                          Vector2d{{4146292.729, 666952.887}},
                          Vector2d{{4138759.902, 702670.738}}};
        
        dst_vector_utm = {Vector2d{{4157372.543, 664864.307}},
                          Vector2d{{4149193.336, 688911.443}},
                          Vector2d{{4172953.511, 690415.078}},
                          Vector2d{{4177298.376, 643072.635}},
                          Vector2d{{4137162.19, 671883.029}},
                          Vector2d{{4146442.729, 667027.887}},
                          Vector2d{{4138909.902, 702745.738}}};

        src_matrix_utm = {{4157222.543, 664789.307},
                          {4149043.336, 688836.443},
                          {4172803.511, 690340.078},
                          {4177148.376, 642997.635},
                          {4137012.190, 671808.029},
                          {4146292.729, 666952.887},
                          {4138759.902, 702670.738}};

        dst_matrix_utm = {{4157372.543, 664864.307},
                          {4149193.336, 688911.443},
                          {4172953.511, 690415.078},
                          {4177298.376, 643072.635},
                          {4137162.19, 671883.029},
                          {4146442.729, 667027.887},
                          {4138909.902, 702745.738}};

    }

    void teardown()
    {

    }


    Matrix<double> src_mat;
    Matrix<double> dst_mat;
    Matrix<double> src_mat2;
    Matrix<double> dst_mat2;
    std::vector<Point3<double>> src_points;
    std::vector<Point3<double>> dst_points;
    std::vector<Point<double>> src_points_utm;
    std::vector<Point<double>> dst_points_utm;
    std::vector<Vector2d> src_vector_utm;
    std::vector<Vector2d> dst_vector_utm;
    Matrix<double> src_matrix_utm;
    Matrix<double> dst_matrix_utm;

};


BOOST_FIXTURE_TEST_CASE(default_constructor, TransformTest)
{

    Transform<double, 3> transform;
        
    BOOST_CHECK_CLOSE(1., transform[0][0], 0.1);
    BOOST_CHECK_CLOSE(0., transform[0][1], 0.1);
    BOOST_CHECK_CLOSE(0., transform[0][2], 0.1);
    BOOST_CHECK_CLOSE(0., transform[0][3], 0.1);
    BOOST_CHECK_CLOSE(0., transform[1][0], 0.1);
    BOOST_CHECK_CLOSE(1., transform[1][1], 0.1);
    BOOST_CHECK_CLOSE(0., transform[1][2], 0.1);
    BOOST_CHECK_CLOSE(0., transform[1][3], 0.1);
    BOOST_CHECK_CLOSE(0., transform[2][0], 0.1);
    BOOST_CHECK_CLOSE(0., transform[2][1], 0.1);
    BOOST_CHECK_CLOSE(1., transform[2][2], 0.1);
    BOOST_CHECK_CLOSE(0., transform[2][3], 0.1);
    BOOST_CHECK_CLOSE(0., transform[3][0], 0.1);
    BOOST_CHECK_CLOSE(0., transform[3][1], 0.1);
    BOOST_CHECK_CLOSE(0., transform[3][2], 0.1);
    BOOST_CHECK_CLOSE(1., transform[3][3], 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_per_transform, TransformTest)
{

    Transform<double, 3> transform1;
    transform1.setScale(2.);
    Transform<double, 3> transform2;
    transform2.setTranslation({10., 30., 26.});
    auto transform3 = transform1 * transform2;

    BOOST_CHECK_CLOSE(2., transform3[0][0], 0.1);
    BOOST_CHECK_CLOSE(2., transform3[1][1], 0.1);
    BOOST_CHECK_CLOSE(2., transform3[2][2], 0.1);
    BOOST_CHECK_CLOSE(20., transform3[0][3], 0.1);
    BOOST_CHECK_CLOSE(60., transform3[1][3], 0.1);
    BOOST_CHECK_CLOSE(52., transform3[2][3], 0.1);
}

BOOST_FIXTURE_TEST_CASE(setScale, TransformTest)
{

    Transform<double, 3> transform;
    transform.setScale(2.);

    auto scale = transform.scale();

    BOOST_CHECK_CLOSE(2., scale[0], 0.1);
    BOOST_CHECK_CLOSE(2., scale[1], 0.1);
    BOOST_CHECK_CLOSE(2., scale[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(setTranslation, TransformTest)
{
    Transform<double, 3> transform;
    transform.setTranslation({10., 30., 26.});

    auto translation = transform.translation();

    BOOST_CHECK_CLOSE(10., translation[0], 0.1);
    BOOST_CHECK_CLOSE(30., translation[1], 0.1);
    BOOST_CHECK_CLOSE(26., translation[2], 0.1);
}

BOOST_AUTO_TEST_SUITE_END()