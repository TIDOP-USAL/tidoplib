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
 
#define BOOST_TEST_MODULE Tidop translation test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/translation.h>
#include <tidop/math/algebra/matrix.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(TranslationTestSuite)

struct TranslationTest
{

    TranslationTest() {}

    ~TranslationTest() {}

    void setup()
    {

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

    std::vector<Point<double>> src_points_utm;
    std::vector<Point<double>> dst_points_utm;
    std::vector<Vector2d> src_vector_utm;
    std::vector<Vector2d> dst_vector_utm;
    Matrix<double> src_matrix_utm;
    Matrix<double> dst_matrix_utm;

};


BOOST_FIXTURE_TEST_CASE(default_constructor, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_constructor_tx_ty, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_constructor_tx_ty_tz, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_constructor_vector_2d, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_constructor_vector_3d, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_inverse, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_per_translation, TranslationTest)
{
    Translation<double, 3> translation_d3(10., 5., -15.);
    Translation<double, 3> translation_v3({1., 2., 3.});
    auto translation_concatenate = translation_d3 * translation_v3;
    BOOST_CHECK_EQUAL(translation_concatenate.x(), 11.);
    BOOST_CHECK_EQUAL(translation_concatenate.y(), 7.);
    BOOST_CHECK_EQUAL(translation_concatenate.z(), -12.);
}

BOOST_FIXTURE_TEST_CASE(translation_transform_point2d, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_transform_vector, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(translation_transform_matrix, TranslationTest)
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

BOOST_FIXTURE_TEST_CASE(estimate_points, TranslationTest)
{
    auto translation = TranslationEstimator<double, 2>::estimate(src_points_utm, dst_points_utm);

    BOOST_CHECK_CLOSE(150.0, translation.x(), 0.1);
    BOOST_CHECK_CLOSE(75.0, translation.y(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(estimate_matrix, TranslationTest)
{
    auto translation = TranslationEstimator<double, 2>::estimate(src_matrix_utm, dst_matrix_utm);

    BOOST_CHECK_CLOSE(150.0, translation.x(), 0.1);
    BOOST_CHECK_CLOSE(75.0, translation.y(), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()