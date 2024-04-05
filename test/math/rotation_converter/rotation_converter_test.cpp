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

#define BOOST_TEST_MODULE Tidop rotation converter test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/rotation_convert.h>

using namespace tl;

BOOST_AUTO_TEST_SUITE(RotationConverterTestSuite)

struct RotationConverterTest
{

    RotationConverterTest() {}
    ~RotationConverterTest() {}

    void setup()
    {
        q_cero = Quaterniond::zero();
        q_identity = Quaterniond::identity();
        q_i = Quaterniond::i();
        q_j = Quaterniond::j();
        q_k = Quaterniond::k();
        q = Quaterniond(0., 1., -3., 2.);

        rot_zero = RotationMatrix<double>::zero();
        rot_ones = RotationMatrix<double>::ones();
        rot_identity = RotationMatrix<double>::identity();

        rot.at(0, 0) = -0.8888889;
        rot.at(0, 1) = 0.4444444;
        rot.at(0, 2) = -0.1111111;
        rot.at(1, 0) = -0.1111111;
        rot.at(1, 1) = -0.4444444;
        rot.at(1, 2) = -0.8888889;
        rot.at(2, 0) = -0.4444444;
        rot.at(2, 1) = -0.7777778;
        rot.at(2, 2) = 0.4444444;

        aa.setAngle(1);
        aa.setAxis({1,1,1});

        aa_x.setAngle(1);
        aa_x.setAxis({1,0,0});

        aa_y.setAngle(2);
        aa_y.setAxis({0,1,0});

        aa_z.setAngle(1.5);
        aa_z.setAxis({0,0,1});

        aa_1.setAngle(1.32);
        aa_1.setAxis({3.5, 23.1, 45.3});
    }

    void teardown()
    {

    }

    Quaterniond q_cero;
    Quaterniond q_identity;
    Quaterniond q_i;
    Quaterniond q_j;
    Quaterniond q_k;
    Quaterniond q;

    RotationMatrix<double> rot_zero;
    RotationMatrix<double> rot_ones;
    RotationMatrix<double> rot_identity;
    RotationMatrix<double> rot;

    AxisAngle<double> aa;
    AxisAngle<double> aa_x;
    AxisAngle<double> aa_y;
    AxisAngle<double> aa_z;
    AxisAngle<double> aa_1;
};

/// Quaternion to Matrix

BOOST_FIXTURE_TEST_CASE(quaternion_cero_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = q_cero;
    BOOST_CHECK_CLOSE(1., rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_identity_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = q_identity;
    BOOST_CHECK_CLOSE(1., rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_i_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = q_i;
    BOOST_CHECK_CLOSE(1., rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-1., rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-1., rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_j_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = q_j;
    BOOST_CHECK_CLOSE(-1., rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-1., rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_k_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = q_k;
    BOOST_CHECK_CLOSE(-1., rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-1., rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = q;

    BOOST_CHECK_CLOSE(-19., rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(12., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(4., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(-12, rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-17, rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-6, rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-4., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(-6., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-1., rot_mat.at(2, 2), 0.1);

    rot_mat = q.normalize();
    BOOST_CHECK_CLOSE(-0.4285714, rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.8571429, rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.2857143, rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.8571429, rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.2857143, rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.4285714, rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.2857143, rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.4285714, rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(0.8571429, rot_mat.at(2, 2), 0.1);

}


/// MatrixToQuaternion


BOOST_FIXTURE_TEST_CASE(matrix_zero_to_quaternion, RotationConverterTest)
{

    Quaterniond q_rot = rot_zero;
    BOOST_CHECK_CLOSE(0.5, q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(0., q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(0., q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(0., q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_ones_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = rot_ones;
    BOOST_CHECK_CLOSE(q_identity.x, q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(q_identity.y, q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(q_identity.z, q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(q_identity.w, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_identity_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = rot_identity;
    BOOST_CHECK_CLOSE(q_identity.x, q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(q_identity.y, q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(q_identity.z, q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(q_identity.w, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = rot;
    BOOST_CHECK_CLOSE(-0.1666667, q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(-0.5, q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(0.8333333, q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(-0.1666667, q_rot.w, 0.1);
}


/// QuaternionToAxisAngle

BOOST_FIXTURE_TEST_CASE(quaternion_cero_To_axis_Angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = q_cero;
    BOOST_CHECK_CLOSE(0, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_identity_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = q_identity;
    BOOST_CHECK_CLOSE(0., axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_i_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = q_i;
    BOOST_CHECK_CLOSE(3.1415926535897931, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_j_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = q_j;
    BOOST_CHECK_CLOSE(3.1415927, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_k_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = q_k;
    BOOST_CHECK_CLOSE(3.1415927, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = q;
    BOOST_CHECK_CLOSE(0., axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.3162278, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(-0.9486833, axis_angle.axis(2), 0.1);

    axis_angle = q.normalize();
    BOOST_CHECK_CLOSE(2.0137074, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.3162278, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(-0.9486833, axis_angle.axis(2), 0.1);
}

/// AxisAngle To Quaternion

BOOST_FIXTURE_TEST_CASE(axis_angle_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = aa;
    BOOST_CHECK_CLOSE(0.2767965, q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(0.2767965, q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(0.2767965, q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(0.8775826, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = aa_x;
    BOOST_CHECK_CLOSE(0.4794255, q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(0., q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(0., q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(0.8775826, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_y_angle2_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = aa_y;
    BOOST_CHECK_CLOSE(0., q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(0.841471, q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(0., q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(0.5403023, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_z_angle_1_5_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = aa_z;
    BOOST_CHECK_CLOSE(0., q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(0., q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(0.6816388, q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(0.7316889, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_1_angle_to_quaternion, RotationConverterTest)
{
    Quaterniond q_rot = aa_1;
    BOOST_CHECK_CLOSE(0.0421013, q_rot.x, 0.1);
    BOOST_CHECK_CLOSE(0.2778688, q_rot.y, 0.1);
    BOOST_CHECK_CLOSE(0.5449116, q_rot.z, 0.1);
    BOOST_CHECK_CLOSE(0.7899922, q_rot.w, 0.1);
}

/// Rotation Matrix to Axis Angle

BOOST_FIXTURE_TEST_CASE(matrix_zero_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = rot_zero;
    BOOST_CHECK_CLOSE(3.1415926535897931, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_ones_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = rot_ones;
    BOOST_CHECK_CLOSE(0., axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_identity_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = rot_identity;
    BOOST_CHECK_CLOSE(0, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(1., axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0., axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_to_axis_angle, RotationConverterTest)
{
    AxisAngle<double> axis_angle = rot;
    BOOST_CHECK_CLOSE(3.4764888, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(-0.1690308, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(-0.5070926, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.8451542, axis_angle.axis(2), 0.1);
}


/// Axis Angle to Rotation Matrix


BOOST_FIXTURE_TEST_CASE(axis_angle_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = aa;
    BOOST_CHECK_CLOSE(0.6935349, rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.3325909, rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.6390561, rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.6390561, rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.6935349, rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.3325909, rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.3325909, rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.6390561, rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(0.6935349, rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_to_matrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = aa_x;
    BOOST_CHECK_CLOSE(1., rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.5403023, rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.8414710, rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.8414710, rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(0.5403023, rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(AxisYAngle2ToMatrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = aa_y;
    BOOST_CHECK_CLOSE(-0.4161468, rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.9092974, rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.9092974, rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.4161468, rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(AxisZAngle1_5ToMatrix, RotationConverterTest)
{
    RotationMatrix<double> rot_mat = aa_z;
    BOOST_CHECK_CLOSE(0.0707372, rot_mat.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.9974950, rot_mat.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.9974950, rot_mat.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.0707372, rot_mat.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(1., rot_mat.at(2, 2), 0.1);
}


/// Euler Angles to Rotation Matrix

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyz_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xyz*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.5773503, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.5773503, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.5773503, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.2113249, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.7886751, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.5773503, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.7886751, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.2113249, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(0.5773503, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzy_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xzy*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.5773503, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.211325, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.788675, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.788675, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.211325, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yxz_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yxz*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.211325, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.788675, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.788675, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.211325, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzx_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yzx*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.788675, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.211325, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.211325, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.788675, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxy_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::zxy*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.788675, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.211325, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.211325, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.788675, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyx_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::zyx*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(0.57735, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.788675, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.211325, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.211325, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.788675, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.57735, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyx_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xyx*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.816497, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.0917516, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.908248, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.908248, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.0917517, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzx_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xzx*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.816497, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.408248, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.408248, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.0917517, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.908248, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.908248, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.0917516, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yxy_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yxy*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.0917516, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.908248, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.816497, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.408248, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.908248, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.408248, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.0917517, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzy_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yzy*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.0917517, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.908248, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.816497, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.908248, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.0917516, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxz_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::zxz*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.0917516, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.908248, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.908248, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.0917517, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.816497, rotation_matrix.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyz_to_RotationMatrix, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::zyz*/);
    RotationMatrix<double> rotation_matrix = eulerAngles;
    BOOST_CHECK_CLOSE(-0.0917517, rotation_matrix.at(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.908248, rotation_matrix.at(0, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.408248, rotation_matrix.at(0, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.908248, rotation_matrix.at(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.0917516, rotation_matrix.at(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(1, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.408248, rotation_matrix.at(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0.408248, rotation_matrix.at(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-0.816497, rotation_matrix.at(2, 2), 0.1);
}


/// EulerAngles to Quaternion


BOOST_FIXTURE_TEST_CASE(EulerAngles_xyz_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xyz*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.398113, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.22985, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.857813, quaternion.z, 0.1);
    BOOST_CHECK_CLOSE(-0.22985, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzy_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xzy*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.118979, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(-0.769095, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.444037, quaternion.z, 0.1);
    BOOST_CHECK_CLOSE(0.444037, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yxz_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yxz*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.444037, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.118979, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(-0.769095, quaternion.z, 0.1);
    BOOST_CHECK_CLOSE(0.444037, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzx_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yzx*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.857813, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.398113, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.22985, quaternion.z, 0.1);
    BOOST_CHECK_CLOSE(-0.22985, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxy_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::zxy*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.22985, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.857813, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.398113, quaternion.z, 0.1);
    BOOST_CHECK_CLOSE(-0.22985, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyx_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::zyx*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(-0.769095, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.444037, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.118979, quaternion.z, 0.1);
    BOOST_CHECK_CLOSE(0.444037, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyx_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xyx*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.302905, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.673887, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.673887, quaternion.z, 0.1);
    /// Número muy pequeño, lo cual da problemas con el test normal
    double abs_w = std::abs(quaternion.w);
    bool check = abs_w < 0.0000001;
    BOOST_CHECK(check);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzx_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::xzx*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.302905, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(-0.673887, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.673887, quaternion.z, 0.1);
    /// Número muy pequeño, lo cual da problemas con el test normal
    double abs_w = std::abs(quaternion.w);
    bool check = abs_w < 0.0000001;
    BOOST_CHECK(check);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yxy_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yxy*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.673887, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.302905, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(-0.673887, quaternion.z, 0.1);
    /// Número muy pequeño, lo cual da problemas con el test normal
    double abs_w = std::abs(quaternion.w);
    bool check = abs_w < 0.0000001;
    BOOST_CHECK(check);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzy_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483/*, EulerAngles<double>::Axes::yzy*/);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.673887, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.302905, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.673887, quaternion.z, 0.1);
    /// Número muy pequeño, lo cual da problemas con el test normal
    double abs_w = std::abs(quaternion.w);
    bool check = abs_w < 0.0000001;
    BOOST_CHECK(check);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxz_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(0.673887, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.673887, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.302905, quaternion.z, 0.1);
    /// Número muy pequeño, lo cual da problemas con el test normal
    double abs_w = std::abs(quaternion.w);
    bool check = abs_w < 0.0000001;
    BOOST_CHECK(check);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyz_to_Quaternion, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    Quaterniond quaternion = eulerAngles;
    BOOST_CHECK_CLOSE(-0.673887, quaternion.x, 0.1);
    BOOST_CHECK_CLOSE(0.673887, quaternion.y, 0.1);
    BOOST_CHECK_CLOSE(0.302905, quaternion.z, 0.1);
    /// Número muy pequeño, lo cual da problemas con el test normal
    double abs_w = std::abs(quaternion.w);
    bool check = abs_w < 0.0000001;
    BOOST_CHECK(check);
}


/// EulerAngles to AxisAngle

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyz_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.6054406, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.4090649, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.2361737, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.8814124, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzy_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(2.2213944, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.1327882, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(-0.858356, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.495572, axis_angle.axis(2), 0.1);
}
  
BOOST_FIXTURE_TEST_CASE(EulerAngles_yxz_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(2.2213944, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.495572, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.132788, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(-0.8583563, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzx_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.6054406, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.881412, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.409065, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.236174, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxy_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.6054406, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.236174, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.881412, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.409065, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyx_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(2.22139, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(-0.858356, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.495572, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.132788, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyx_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.14159, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.302905, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzx_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.14159, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.302905, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(-0.673887, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yxy_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.14159, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.302905, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(-0.673887, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzy_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.14159, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.302905, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxz_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.14159, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.302905, axis_angle.axis(2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyz_to_AxisAngle, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483);
    AxisAngle<double> axis_angle = eulerAngles;
    BOOST_CHECK_CLOSE(3.14159, axis_angle.angle(), 0.1);
    BOOST_CHECK_CLOSE(-0.673887, axis_angle.axis(0), 0.1);
    BOOST_CHECK_CLOSE(0.673887, axis_angle.axis(1), 0.1);
    BOOST_CHECK_CLOSE(0.302905, axis_angle.axis(2), 0.1);
}




/// Matrix to Euler Angles

/// https://www.andre-gaschler.com/rotationconverter/
/// Con Eigen sale distinto
BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles; 
    eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::half_pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::half_pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::half_pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::half_pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::half_pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = rot_zero;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::half_pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);

}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = rot_identity;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixOnesToEulerAngles, RotationConverterTest)
{
  //EulerAngles<double> eulerAngles = rot_ones;
  //BOOST_CHECK_CLOSE(2.356194490192345, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(2.5261129449194057, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0.7853981633974483, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(-0.61547970867038726, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(-0.61547970867038726, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(2.5261129449194057, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(2.5261129449194057, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(-0.61547970867038726, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(-0.78539816339744828, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(-0.78539816339744828, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(-0.78539816339744828, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.x, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.y, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = rot;
    BOOST_CHECK_CLOSE(1.1071487, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.111341, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.6779451, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-2.0899424, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.460554, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-3.0172377, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-0.2449786, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1.0949141, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.896614, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = rot;
    BOOST_CHECK_CLOSE(2.6779451, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.111341, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(2.0344439, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-2.3561945, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.8911225, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0.7853982, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-3.0172377, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0.460554, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-1.0516502, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-0.124354980700608, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1.11024238472747, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.62244659472789, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-0.244978663126864, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(2.66571042818294, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(1.81577498992176, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-2.0899423856568, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(2.03135026886233, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-1.6951513074955, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-1.6951513074955045, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1.1102423847274674, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-1.0516502679329891, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = rot;
    BOOST_CHECK_CLOSE(-1.8157749899217608, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(2.6657104281829396, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.8966139904629289, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = rot;
    BOOST_CHECK_CLOSE(2.6224465947278857, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(2.0313502688623259, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-0.12435498070060774, eulerAngles.z, 0.1);
}



/// Quaternion to Euler angles

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = q_cero;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIdentityToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

//  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
//  RotationConverter<double>::convert(q_identity, eulerAngles);
//  BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
//  BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
//}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = q_i;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}


BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = q_j;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0., eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = q_k;
    BOOST_CHECK_CLOSE(0., eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(consts::pi<double>, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesXYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(0.46364760900080615, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0.28975170143604750, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.0344439357957027, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesXZY, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(-2.1587989303424644, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-1.0296968008377509, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(2.5535900500422257, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesYXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(0.32175055439664224, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0.44291104407363896, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-1.8925468811915389, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesYZX, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(2.5535900500422257, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-1.0296968008377509, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(2.1587989303424644, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesZXY, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(-1.8925468811915389, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.44291104407363896, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0.32175055439664224, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesZYX, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(-2.0344439357957027, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0.28975170143604750, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-0.46364760900080615, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesZXZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(0.58800260354756761, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0.54109952595714583, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.5535900500422257, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesXYX, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(-1.2490457723982544, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(2.0137073708685356, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(1.2490457723982544, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesYZY, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(-2.6779450445889870, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1.8605480282309441, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.6779450445889870, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesZYZ, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(-0.98279372324732905, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0.54109952595714583, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-0.98279372324732905, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesXZX, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(-2.8198420991931510, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(2.0137073708685356, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(2.8198420991931510, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAnglesYXY, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = q.normalize();
    BOOST_CHECK_CLOSE(2.0344439357957027, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1.8605480282309441, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-1.1071487177940904, eulerAngles.z, 0.1);
}


/// AxisAngle to EulerAngles



BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_xyz, RotationConverterTest)
{
    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
    EulerAngles<double, Axes::xyz> eulerAngles = axis_angle;
    BOOST_CHECK_CLOSE(-0.785398007, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0.6154796765, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.35619448, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_xzy, RotationConverterTest)
{
    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
    EulerAngles<double, Axes::xzy> eulerAngles = axis_angle;
    BOOST_CHECK_CLOSE(2.8797933085288911, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.61547961750338809, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(2.3561944384602875, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_yxz, RotationConverterTest)
{
    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
    EulerAngles<double, Axes::yxz> eulerAngles = axis_angle;
    BOOST_CHECK_CLOSE(0.7853981, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.6154796, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.8797933, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_yzx, RotationConverterTest)
{
    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
    EulerAngles<double, Axes::yzx> eulerAngles = axis_angle;
    BOOST_CHECK_CLOSE(-2.2027107, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(-0.2129303, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-2.5096784, eulerAngles.z, 0.1);
}

//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_zxy, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::zxy> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}

//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_zyx, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::zyx> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}

//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_zxz, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::zxz> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_xyx, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::xyx> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_yzy, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::yzy> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_zyz, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::zyz> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_xzx, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::xzx> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles_yxy, RotationConverterTest)
//{
//    AxisAngle<double> axis_angle(3.6054406, {0.4090649, 0.2361737, 0.8814124});
//    EulerAngles<double, Axes::yxy> eulerAngles = axis_angle;
//    BOOST_CHECK_CLOSE(, eulerAngles.x, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.y, 0.1);
//    BOOST_CHECK_CLOSE(, eulerAngles.z, 0.1);
//}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_xyz, RotationConverterTest)
{
    EulerAngles<double, Axes::xyz> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(1, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_xzy, RotationConverterTest)
{
    EulerAngles<double, Axes::xzy> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(1, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_yxz, RotationConverterTest)
{
    EulerAngles<double, Axes::yxz> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_yzx, RotationConverterTest)
{
    EulerAngles<double, Axes::yzx> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_zxy, RotationConverterTest)
{
    EulerAngles<double, Axes::zxy> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_zyx, RotationConverterTest)
{
    EulerAngles<double, Axes::zyx> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_zxz, RotationConverterTest)
{
    EulerAngles<double, Axes::zxz> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_xyx, RotationConverterTest)
{
    EulerAngles<double, Axes::xyx> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_yzy, RotationConverterTest)
{
    EulerAngles<double, Axes::yzy> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(1.5707963267948966, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(-1.5707963267948966, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_zyz, RotationConverterTest)
{
    EulerAngles<double, Axes::zyz> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(-1.5707963267948966, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1., eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(1.5707963267948966, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_xzx, RotationConverterTest)
{
    EulerAngles<double, Axes::xzx> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.z, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_euler_angles_yxy, RotationConverterTest)
{
    EulerAngles<double, Axes::yxy> eulerAngles = aa_x;
    BOOST_CHECK_CLOSE(0, eulerAngles.x, 0.1);
    BOOST_CHECK_CLOSE(1, eulerAngles.y, 0.1);
    BOOST_CHECK_CLOSE(0, eulerAngles.z, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
