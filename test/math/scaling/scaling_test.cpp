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
 
#define BOOST_TEST_MODULE Tidop scaling test
#include <boost/test/unit_test.hpp>
#include <tidop/math/geometry/scaling.h>
#include <tidop/math/algebra/matrix.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(ScalingTestSuite)

struct ScalingTest
{

    ScalingTest() {}

    ~ScalingTest() {}

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


BOOST_FIXTURE_TEST_CASE(default_constructor, ScalingTest)
{
    /* 2D */
    {
        Scaling<double, 2> scaling;

        BOOST_CHECK_EQUAL(1., scaling.x());
        BOOST_CHECK_EQUAL(1., scaling.y());

        BOOST_CHECK_EQUAL(1., scaling.toVector()[0]);
        BOOST_CHECK_EQUAL(1., scaling.toVector()[1]);

        BOOST_CHECK_EQUAL(1., scaling.at(0));
        BOOST_CHECK_EQUAL(1., scaling.at(1));

        BOOST_CHECK_EQUAL(1., scaling[0]);
        BOOST_CHECK_EQUAL(1., scaling[1]);
    }

    /* 3D */
    {
        Scaling<double, 3> scaling;

        BOOST_CHECK_EQUAL(1, scaling.x());
        BOOST_CHECK_EQUAL(1, scaling.y());
        BOOST_CHECK_EQUAL(1, scaling.z());

        BOOST_CHECK_EQUAL(1, scaling.toVector()[0]);
        BOOST_CHECK_EQUAL(1, scaling.toVector()[1]);
        BOOST_CHECK_EQUAL(1, scaling.toVector()[2]);

        BOOST_CHECK_EQUAL(1, scaling.at(0));
        BOOST_CHECK_EQUAL(1, scaling.at(1));
        BOOST_CHECK_EQUAL(1, scaling.at(2));

        BOOST_CHECK_EQUAL(1, scaling[0]);
        BOOST_CHECK_EQUAL(1, scaling[1]);
        BOOST_CHECK_EQUAL(1, scaling[2]);
    }
}

BOOST_FIXTURE_TEST_CASE(scaling_constructor_uniform_scale, ScalingTest)
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

BOOST_FIXTURE_TEST_CASE(scaling_constructor_sx_sy, ScalingTest)
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

BOOST_FIXTURE_TEST_CASE(scaling_constructor_sx_sy_sz, ScalingTest)
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

BOOST_FIXTURE_TEST_CASE(scaling_constructor_vector_2d, ScalingTest)
{
    Scaling<double, 2> scaling{1.2, 1.5};

    BOOST_CHECK_EQUAL(1.2, scaling.x());
    BOOST_CHECK_EQUAL(1.5, scaling.y());

    BOOST_CHECK_EQUAL(1.2, scaling.toVector()[0]);
    BOOST_CHECK_EQUAL(1.5, scaling.toVector()[1]);

    BOOST_CHECK_EQUAL(1.2, scaling.at(0));
    BOOST_CHECK_EQUAL(1.5, scaling.at(1));

    BOOST_CHECK_EQUAL(1.2, scaling[0]);
    BOOST_CHECK_EQUAL(1.5, scaling[1]);
}

BOOST_FIXTURE_TEST_CASE(translation_constructor_vector_3d, ScalingTest)
{
    Scaling<double, 3> scaling{-1., -1., 1.};

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

BOOST_FIXTURE_TEST_CASE(scaling_inverse, ScalingTest)
{
    Scaling<double, 3> scaling{1.2, 1.2, 1.2};
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

BOOST_FIXTURE_TEST_CASE(scaling_per_scaling, ScalingTest)
{
    Scaling<double, 3> scaling_d3(1., 1., 1.);
    Scaling<double, 3> scaling_v3{1., 2., 3.};
    auto scaling_concatenate = scaling_d3 * scaling_v3;
    BOOST_CHECK_EQUAL(scaling_concatenate.x(), 1.);
    BOOST_CHECK_EQUAL(scaling_concatenate.y(), 2.);
    BOOST_CHECK_EQUAL(scaling_concatenate.z(), 3.);
}

BOOST_FIXTURE_TEST_CASE(scaling_transform_point2d, ScalingTest)
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

BOOST_FIXTURE_TEST_CASE(scaling_transform_vector, ScalingTest)
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

BOOST_FIXTURE_TEST_CASE(scaling_transform_matrix, ScalingTest)
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


BOOST_AUTO_TEST_SUITE(ScalingEstimatorTestSuite)

struct ScalingTest
{

    ScalingTest()
    {

    }

    ~ScalingTest()
    {
    }

    void setup()
    {
        src_points = {
          Point<double>(4157222.543, 664789.307),
          Point<double>(4149043.336, 688836.443),
          Point<double>(4172803.511, 690340.078),
          Point<double>(4177148.376, 642997.635),
          Point<double>(4137012.190, 671808.029),
          Point<double>(4146292.729, 666952.887),
          Point<double>(4138759.902, 702670.738)};

        dst_points = {
          Point<double>(1039305.636,  166197.327),
          Point<double>(1037260.834,  172209.111),
          Point<double>(1043200.878,  172585.020),
          Point<double>(1044287.094,  160749.409),
          Point<double>(1034253.048,  167952.007),
          Point<double>(1036573.182,  166738.222),
          Point<double>(1034689.976,  175667.685)};

        src_mat = {{4157222.543, 664789.307},
                   {4149043.336, 688836.443},
                   {4172803.511, 690340.078},
                   {4177148.376, 642997.635},
                   {4137012.190, 671808.029},
                   {4146292.729, 666952.887},
                   {4138759.902, 702670.738}};

        dst_mat = {{1039305.636, 166197.327},
                   {1037260.834, 172209.111},
                   {1043200.878, 172585.020},
                   {1044287.094, 160749.409},
                   {1034253.048, 167952.007},
                   {1036573.182, 166738.222},
                   {1034689.976, 175667.685}};
    }

    void teardown()
    {
    }

    std::vector<Point<double>> src_points;
    std::vector<Point<double>> dst_points;
    Matrix<double> src_mat;
    Matrix<double> dst_mat;
};


BOOST_FIXTURE_TEST_CASE(estimate_points, ScalingTest)
{
    auto scaling = ScalingEstimator<double, 2>::estimate(src_points, dst_points);

    BOOST_CHECK_CLOSE(0.25, scaling.x(), 0.1);
    BOOST_CHECK_CLOSE(0.25, scaling.y(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(estimate_matrix, ScalingTest)
{
    auto scaling = ScalingEstimator<double, 2>::estimate(src_mat, dst_mat);

    BOOST_CHECK_CLOSE(0.25, scaling.x(), 0.1);
    BOOST_CHECK_CLOSE(0.25, scaling.y(), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()