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
#include <tidop/math/algebra/translation.h>

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

        translate = Translation<double, 3>(10., 20., 30);

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

    Translation<double, 3> translate;
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

BOOST_FIXTURE_TEST_CASE(umeyama_compute, TransformTest)
{
    Transform<double, 3> transform = Umeyama<double,3>::compute(src_mat, dst_mat);
    BOOST_CHECK_CLOSE(1, transform[0][0], 0.1);
    BOOST_CHECK_CLOSE(-3.80747e-05, transform[0][1], 0.1);
    BOOST_CHECK_CLOSE(-8.79539e-06, transform[0][2], 0.1);
    BOOST_CHECK_CLOSE(-419.568, transform[0][3], 0.1);
    BOOST_CHECK_CLOSE(3.80748e-05, transform[1][0], 0.1);
    BOOST_CHECK_CLOSE(1, transform[1][1], 0.1);
    BOOST_CHECK_CLOSE(4.1215e-06, transform[1][2], 0.1);
    BOOST_CHECK_CLOSE(-99.246, transform[1][3], 0.1);
    BOOST_CHECK_CLOSE(8.79523e-06, transform[2][0], 0.1);
    BOOST_CHECK_CLOSE(-4.12183e-06, transform[2][1], 0.1);
    BOOST_CHECK_CLOSE(1, transform[2][2], 0.1);
    BOOST_CHECK_CLOSE(-591.456, transform[2][3], 0.1);
    BOOST_CHECK_CLOSE(0, transform[3][0], 0.1);
    BOOST_CHECK_CLOSE(0, transform[3][1], 0.1);
    BOOST_CHECK_CLOSE(0, transform[3][2], 0.1);
    BOOST_CHECK_CLOSE(1, transform[3][3], 0.1);
}

/* Translation */

BOOST_FIXTURE_TEST_CASE(translation_default_constructor, TransformTest)
{
    /* 2D */
    Translation<double, 2> translate;

    BOOST_CHECK_EQUAL(0, translate.x());
    BOOST_CHECK_EQUAL(0, translate.y());

    BOOST_CHECK_EQUAL(0, translate.toVector()[0]);
    BOOST_CHECK_EQUAL(0, translate.toVector()[1]);

    BOOST_CHECK_EQUAL(0, translate.at(0));
    BOOST_CHECK_EQUAL(0, translate.at(1));

    BOOST_CHECK_EQUAL(0, translate[0]);
    BOOST_CHECK_EQUAL(0, translate[1]);

    /* 3D */

    Translation<double, 3> translate3d;

    BOOST_CHECK_EQUAL(0, translate3d.x());
    BOOST_CHECK_EQUAL(0, translate3d.y());
    BOOST_CHECK_EQUAL(0, translate3d.z());

    BOOST_CHECK_EQUAL(0, translate3d.toVector()[0]);
    BOOST_CHECK_EQUAL(0, translate3d.toVector()[1]);
    BOOST_CHECK_EQUAL(0, translate3d.toVector()[2]);

    BOOST_CHECK_EQUAL(0, translate3d.at(0));
    BOOST_CHECK_EQUAL(0, translate3d.at(1));
    BOOST_CHECK_EQUAL(0, translate3d.at(2));
    
    BOOST_CHECK_EQUAL(0, translate3d[0]);
    BOOST_CHECK_EQUAL(0, translate3d[1]);
    BOOST_CHECK_EQUAL(0, translate3d[2]);
}

BOOST_FIXTURE_TEST_CASE(translation_constructor_tx_ty, TransformTest)
{
    Translation<double, 2> translate(25., 36.);

    BOOST_CHECK_EQUAL(25., translate.x());
    BOOST_CHECK_EQUAL(36., translate.y());

    BOOST_CHECK_EQUAL(25., translate.toVector()[0]);
    BOOST_CHECK_EQUAL(36., translate.toVector()[1]);

    BOOST_CHECK_EQUAL(25., translate.at(0));
    BOOST_CHECK_EQUAL(36., translate.at(1));

    BOOST_CHECK_EQUAL(25., translate[0]);
    BOOST_CHECK_EQUAL(36., translate[1]);
}

BOOST_FIXTURE_TEST_CASE(translation_constructor_tx_ty_tz, TransformTest)
{
    Translation<double, 3> translate(25., 36., 11.);

    BOOST_CHECK_EQUAL(25., translate.x());
    BOOST_CHECK_EQUAL(36., translate.y());
    BOOST_CHECK_EQUAL(11., translate.z());

    BOOST_CHECK_EQUAL(25., translate.toVector()[0]);
    BOOST_CHECK_EQUAL(36., translate.toVector()[1]);
    BOOST_CHECK_EQUAL(11., translate.toVector()[2]);

    BOOST_CHECK_EQUAL(25., translate.at(0));
    BOOST_CHECK_EQUAL(36., translate.at(1));
    BOOST_CHECK_EQUAL(11., translate.at(2));
    
    BOOST_CHECK_EQUAL(25., translate[0]);
    BOOST_CHECK_EQUAL(36., translate[1]);
    BOOST_CHECK_EQUAL(11., translate[2]);
}

BOOST_FIXTURE_TEST_CASE(translation_constructor_vector_2d, TransformTest)
{
    Translation<double, 2> translate({25., 36.});

    BOOST_CHECK_EQUAL(25., translate.x());
    BOOST_CHECK_EQUAL(36., translate.y());

    BOOST_CHECK_EQUAL(25., translate.toVector()[0]);
    BOOST_CHECK_EQUAL(36., translate.toVector()[1]);

    BOOST_CHECK_EQUAL(25., translate.at(0));
    BOOST_CHECK_EQUAL(36., translate.at(1));

    BOOST_CHECK_EQUAL(25., translate[0]);
    BOOST_CHECK_EQUAL(36., translate[1]);
}

BOOST_FIXTURE_TEST_CASE(translation_constructor_vector_3d, TransformTest)
{
    Translation<double, 3> translate({25., 36., 11.});

    BOOST_CHECK_EQUAL(25., translate.x());
    BOOST_CHECK_EQUAL(36., translate.y());
    BOOST_CHECK_EQUAL(11., translate.z());

    BOOST_CHECK_EQUAL(25., translate.toVector()[0]);
    BOOST_CHECK_EQUAL(36., translate.toVector()[1]);
    BOOST_CHECK_EQUAL(11., translate.toVector()[2]);

    BOOST_CHECK_EQUAL(25., translate.at(0));
    BOOST_CHECK_EQUAL(36., translate.at(1));
    BOOST_CHECK_EQUAL(11., translate.at(2));
    
    BOOST_CHECK_EQUAL(25., translate[0]);
    BOOST_CHECK_EQUAL(36., translate[1]);
    BOOST_CHECK_EQUAL(11., translate[2]);
}

BOOST_FIXTURE_TEST_CASE(translation_inverse, TransformTest)
{
    Translation<double, 3> translate({25., 36., 11.});
    auto translate_inv = translate.inverse();

    BOOST_CHECK_EQUAL(-25., translate_inv.x());
    BOOST_CHECK_EQUAL(-36., translate_inv.y());
    BOOST_CHECK_EQUAL(-11., translate_inv.z());

    BOOST_CHECK_EQUAL(-25., translate_inv.toVector()[0]);
    BOOST_CHECK_EQUAL(-36., translate_inv.toVector()[1]);
    BOOST_CHECK_EQUAL(-11., translate_inv.toVector()[2]);

    BOOST_CHECK_EQUAL(-25., translate_inv.at(0));
    BOOST_CHECK_EQUAL(-36., translate_inv.at(1));
    BOOST_CHECK_EQUAL(-11., translate_inv.at(2));

    BOOST_CHECK_EQUAL(-25., translate_inv[0]);
    BOOST_CHECK_EQUAL(-36., translate_inv[1]);
    BOOST_CHECK_EQUAL(-11., translate_inv[2]);
}

BOOST_FIXTURE_TEST_CASE(translation_transform_point2d, TransformTest)
{
    {
        Translation<double, 2> translate(25., 36.);

        Point<double> point(23.5, 21.1);
        auto point2 = translate.transform(point);

        BOOST_CHECK_EQUAL(48.5, point2.x);
        BOOST_CHECK_EQUAL(57.1, point2.y);
    }

    {
        Point<double> point(23.5, 21.1);
        auto point2 = Translation<double, 2>(25., 36.) * point;

        BOOST_CHECK_EQUAL(48.5, point2.x);
        BOOST_CHECK_EQUAL(57.1, point2.y);
    }

}

BOOST_FIXTURE_TEST_CASE(translation_transform_vector, TransformTest)
{
    {
        Translation<double, 3> translate(25., 36., 11.);

        Vector<double, 3> vector{23.5, 21.1, 63.8};
        auto vector2 = translate.transform(vector);

        BOOST_CHECK_EQUAL(48.5, vector2[0]);
        BOOST_CHECK_EQUAL(57.1, vector2[1]);
        BOOST_CHECK_EQUAL(74.8, vector2[2]);
    }

    {
        Vector<double, 3> vector{23.5, 21.1, 63.8};
        auto vector2 = Translation<double, 3>(25., 36., 11.) * vector;

        BOOST_CHECK_EQUAL(48.5, vector2[0]);
        BOOST_CHECK_EQUAL(57.1, vector2[1]);
        BOOST_CHECK_EQUAL(74.8, vector2[2]);
    }

}

BOOST_FIXTURE_TEST_CASE(translation_transform_matrix, TransformTest)
{
    {
        Translation<double, 2> translate(150.0, 75.);

        auto out = translate.transform(src_matrix_utm);

        BOOST_CHECK_EQUAL(4157372.543, out[0][0]);
        BOOST_CHECK_EQUAL(664864.307, out[0][1]);
        BOOST_CHECK_EQUAL(4149193.336, out[1][0]);
        BOOST_CHECK_EQUAL(688911.443, out[1][1]);
        BOOST_CHECK_EQUAL(4172953.511, out[2][0]);
        BOOST_CHECK_EQUAL(690415.078, out[2][1]);
        BOOST_CHECK_EQUAL(4177298.376, out[3][0]);
        BOOST_CHECK_EQUAL(643072.635, out[3][1]);
        BOOST_CHECK_EQUAL(4137162.19, out[4][0]);
        BOOST_CHECK_EQUAL(671883.029, out[4][1]);
        BOOST_CHECK_EQUAL(4146442.729, out[5][0]);
        BOOST_CHECK_EQUAL(667027.887, out[5][1]);
        BOOST_CHECK_EQUAL(4138909.902, out[6][0]);
        BOOST_CHECK_EQUAL(702745.738, out[6][1]);
    }

    {
        auto out = Translation<double, 2>(150.0, 75.) * src_matrix_utm;

        BOOST_CHECK_EQUAL(4157372.543, out[0][0]);
        BOOST_CHECK_EQUAL(664864.307, out[0][1]);
        BOOST_CHECK_EQUAL(4149193.336, out[1][0]);
        BOOST_CHECK_EQUAL(688911.443, out[1][1]);
        BOOST_CHECK_EQUAL(4172953.511, out[2][0]);
        BOOST_CHECK_EQUAL(690415.078, out[2][1]);
        BOOST_CHECK_EQUAL(4177298.376, out[3][0]);
        BOOST_CHECK_EQUAL(643072.635, out[3][1]);
        BOOST_CHECK_EQUAL(4137162.19, out[4][0]);
        BOOST_CHECK_EQUAL(671883.029, out[4][1]);
        BOOST_CHECK_EQUAL(4146442.729, out[5][0]);
        BOOST_CHECK_EQUAL(667027.887, out[5][1]);
        BOOST_CHECK_EQUAL(4138909.902, out[6][0]);
        BOOST_CHECK_EQUAL(702745.738, out[6][1]);
    }

}


/* Scaling */

BOOST_FIXTURE_TEST_CASE(scaling_default_constructor, TransformTest)
{
    /* 2D */
    {
        Scaling<double, 2> scale;

        BOOST_CHECK_EQUAL(1, scale.x());
        BOOST_CHECK_EQUAL(1, scale.y());

        BOOST_CHECK_EQUAL(1, scale.toVector()[0]);
        BOOST_CHECK_EQUAL(1, scale.toVector()[1]);

        BOOST_CHECK_EQUAL(1, scale.at(0));
        BOOST_CHECK_EQUAL(1, scale.at(1));

        BOOST_CHECK_EQUAL(1, scale[0]);
        BOOST_CHECK_EQUAL(1, scale[1]);
    }
    /* 3D */

    {
        Scaling<double, 3> scale;

        BOOST_CHECK_EQUAL(1, scale.x());
        BOOST_CHECK_EQUAL(1, scale.y());
        BOOST_CHECK_EQUAL(1, scale.z());

        BOOST_CHECK_EQUAL(1, scale.toVector()[0]);
        BOOST_CHECK_EQUAL(1, scale.toVector()[1]);
        BOOST_CHECK_EQUAL(1, scale.toVector()[2]);

        BOOST_CHECK_EQUAL(1, scale.at(0));
        BOOST_CHECK_EQUAL(1, scale.at(1));
        BOOST_CHECK_EQUAL(1, scale.at(2));

        BOOST_CHECK_EQUAL(1, scale[0]);
        BOOST_CHECK_EQUAL(1, scale[1]);
        BOOST_CHECK_EQUAL(1, scale[2]);
    }
}

BOOST_FIXTURE_TEST_CASE(scaling_constructor_uniform_scale, TransformTest)
{
    Scaling<double, 2> scaling(1.5);

    BOOST_CHECK_EQUAL(1.5, scaling.x());
    BOOST_CHECK_EQUAL(1.5, scaling.y());

    BOOST_CHECK_EQUAL(1.5, scaling.toVector()[0]);
    BOOST_CHECK_EQUAL(1.5, scaling.toVector()[1]);

    BOOST_CHECK_EQUAL(1.5, scaling.at(0));
    BOOST_CHECK_EQUAL(1.5, scaling.at(1));

    BOOST_CHECK_EQUAL(1.5, scaling[0]);
    BOOST_CHECK_EQUAL(1.5, scaling[1]);
}

BOOST_FIXTURE_TEST_CASE(scaling_constructor_sx_sy, TransformTest)
{
    Scaling<double, 2> scaling(1.2, 1.5);

    BOOST_CHECK_EQUAL(1.2, scaling.x());
    BOOST_CHECK_EQUAL(1.5, scaling.y());

    BOOST_CHECK_EQUAL(1.2, scaling.toVector()[0]);
    BOOST_CHECK_EQUAL(1.5, scaling.toVector()[1]);

    BOOST_CHECK_EQUAL(1.2, scaling.at(0));
    BOOST_CHECK_EQUAL(1.5, scaling.at(1));

    BOOST_CHECK_EQUAL(1.2, scaling[0]);
    BOOST_CHECK_EQUAL(1.5, scaling[1]);
}

BOOST_FIXTURE_TEST_CASE(scaling_constructor_sx_sy_sz, TransformTest)
{
    Scaling<double, 3> scaling(-1., -1., 1.);

    BOOST_CHECK_EQUAL(-1., scaling.x());
    BOOST_CHECK_EQUAL(-1., scaling.y());
    BOOST_CHECK_EQUAL(1., scaling.z());

    BOOST_CHECK_EQUAL(-1., scaling.toVector()[0]);
    BOOST_CHECK_EQUAL(-1., scaling.toVector()[1]);
    BOOST_CHECK_EQUAL(1., scaling.toVector()[2]);

    BOOST_CHECK_EQUAL(-1., scaling.at(0));
    BOOST_CHECK_EQUAL(-1., scaling.at(1));
    BOOST_CHECK_EQUAL(1., scaling.at(2));
    
    BOOST_CHECK_EQUAL(-1., scaling[0]);
    BOOST_CHECK_EQUAL(-1., scaling[1]);
    BOOST_CHECK_EQUAL(1., scaling[2]);
}

BOOST_FIXTURE_TEST_CASE(scaling_constructor_vector_2d, TransformTest)
{
    Scaling<double, 2> scaling({1.2, 1.5});

    BOOST_CHECK_EQUAL(1.2, scaling.x());
    BOOST_CHECK_EQUAL(1.5, scaling.y());

    BOOST_CHECK_EQUAL(1.2, scaling.toVector()[0]);
    BOOST_CHECK_EQUAL(1.5, scaling.toVector()[1]);

    BOOST_CHECK_EQUAL(1.2, scaling.at(0));
    BOOST_CHECK_EQUAL(1.5, scaling.at(1));

    BOOST_CHECK_EQUAL(1.2, scaling[0]);
    BOOST_CHECK_EQUAL(1.5, scaling[1]);
}

BOOST_FIXTURE_TEST_CASE(scaling_constructor_vector_3d, TransformTest)
{
    Scaling<double, 3> scaling({-1., -1., 1.});

    BOOST_CHECK_EQUAL(-1., scaling.x());
    BOOST_CHECK_EQUAL(-1., scaling.y());
    BOOST_CHECK_EQUAL(1., scaling.z());

    BOOST_CHECK_EQUAL(-1., scaling.toVector()[0]);
    BOOST_CHECK_EQUAL(-1., scaling.toVector()[1]);
    BOOST_CHECK_EQUAL(1., scaling.toVector()[2]);

    BOOST_CHECK_EQUAL(-1., scaling.at(0));
    BOOST_CHECK_EQUAL(-1., scaling.at(1));
    BOOST_CHECK_EQUAL(1., scaling.at(2));
    
    BOOST_CHECK_EQUAL(-1., scaling[0]);
    BOOST_CHECK_EQUAL(-1., scaling[1]);
    BOOST_CHECK_EQUAL(1., scaling[2]);
}

BOOST_FIXTURE_TEST_CASE(scaling_inverse, TransformTest)
{
    Scaling<double, 3> scaling({1.2, 1.2, 1.2});
    auto scaling_inv = scaling.inverse();

    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.x(), 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.y(), 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.z(), 0.01);

    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.toVector()[0], 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.toVector()[1], 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.toVector()[2], 0.01);

    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.at(0), 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.at(1), 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv.at(2), 0.01);

    BOOST_CHECK_CLOSE(0.83333333, scaling_inv[0], 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv[1], 0.01);
    BOOST_CHECK_CLOSE(0.83333333, scaling_inv[2], 0.01);
}

BOOST_FIXTURE_TEST_CASE(scaling_transform_point2d, TransformTest)
{
    {
        Scaling<double, 2> scaling(0.25);

        auto point2 = scaling.transform(src_points_utm[0]);

        BOOST_CHECK_CLOSE(1039305.636, point2.x, 0.01);
        BOOST_CHECK_CLOSE(166197.327, point2.y, 0.01);
    }

    {
        Point<double> point(23.5, 21.1);
        auto point2 = Scaling<double, 2>(1.5, 2.0) * point;

        BOOST_CHECK_CLOSE(35.25, point2.x, 0.01);
        BOOST_CHECK_CLOSE(42.2, point2.y, 0.01);
    }

}

BOOST_FIXTURE_TEST_CASE(scaling_transform_vector, TransformTest)
{
    {
        Scaling<double, 3> scaling(0.25);

        Vector<double, 3> vector{23.5, 21.1, 63.8};
        auto vector2 = scaling.transform(vector);

        BOOST_CHECK_CLOSE(5.875, vector2[0], 0.01);
        BOOST_CHECK_CLOSE(5.275, vector2[1], 0.01);
        BOOST_CHECK_CLOSE(15.95, vector2[2], 0.01);
    }

    {
        Vector<double, 3> vector{1., 1., 1.};
        auto vector2 = Scaling<double, 3>(0.5, 1.0, 1.5) * vector;

        BOOST_CHECK_CLOSE(0.5, vector2[0], 0.01);
        BOOST_CHECK_CLOSE(1.0, vector2[1], 0.01);
        BOOST_CHECK_CLOSE(1.5, vector2[2], 0.01);
    }

}



BOOST_FIXTURE_TEST_CASE(scaling_transform_matrix, TransformTest)
{
    {
        Scaling<double, 2> scaling(0.25);

        auto out = scaling.transform(src_matrix_utm);

        BOOST_CHECK_CLOSE(1039305.636, out[0][0], 0.1);
        BOOST_CHECK_CLOSE(166197.327, out[0][1], 0.1);
        BOOST_CHECK_CLOSE(1037260.834, out[1][0], 0.1);
        BOOST_CHECK_CLOSE(172209.111, out[1][1], 0.1);
        BOOST_CHECK_CLOSE(1043200.878, out[2][0], 0.1);
        BOOST_CHECK_CLOSE(172585.020, out[2][1], 0.1);
        BOOST_CHECK_CLOSE(1044287.094, out[3][0], 0.1);
        BOOST_CHECK_CLOSE(160749.409, out[3][1], 0.1);
        BOOST_CHECK_CLOSE(1034253.048, out[4][0], 0.1);
        BOOST_CHECK_CLOSE(167952.007, out[4][1], 0.1);
        BOOST_CHECK_CLOSE(1036573.182, out[5][0], 0.1);
        BOOST_CHECK_CLOSE(166738.222, out[5][1], 0.1);
        BOOST_CHECK_CLOSE(1034689.976, out[6][0], 0.1);
        BOOST_CHECK_CLOSE(175667.685, out[6][1], 0.1);
    }

    {
        auto out = Scaling<double, 2>(0.25) * src_matrix_utm;

        BOOST_CHECK_CLOSE(1039305.636, out[0][0], 0.1);
        BOOST_CHECK_CLOSE(166197.327, out[0][1], 0.1);
        BOOST_CHECK_CLOSE(1037260.834, out[1][0], 0.1);
        BOOST_CHECK_CLOSE(172209.111, out[1][1], 0.1);
        BOOST_CHECK_CLOSE(1043200.878, out[2][0], 0.1);
        BOOST_CHECK_CLOSE(172585.020, out[2][1], 0.1);
        BOOST_CHECK_CLOSE(1044287.094, out[3][0], 0.1);
        BOOST_CHECK_CLOSE(160749.409, out[3][1], 0.1);
        BOOST_CHECK_CLOSE(1034253.048, out[4][0], 0.1);
        BOOST_CHECK_CLOSE(167952.007, out[4][1], 0.1);
        BOOST_CHECK_CLOSE(1036573.182, out[5][0], 0.1);
        BOOST_CHECK_CLOSE(166738.222, out[5][1], 0.1);
        BOOST_CHECK_CLOSE(1034689.976, out[6][0], 0.1);
        BOOST_CHECK_CLOSE(175667.685, out[6][1], 0.1);
    }

}

BOOST_AUTO_TEST_SUITE_END()
