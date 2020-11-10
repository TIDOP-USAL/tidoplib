#define BOOST_TEST_MODULE Tidop rotation converter test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/rotation_convert.h>

using namespace tl::math;

BOOST_AUTO_TEST_SUITE(RotationConverterTestSuite)

struct RotationConverterTest
{

  RotationConverterTest(){}
  ~RotationConverterTest(){}

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
    rot.at(2, 2) =  0.4444444;

    aa.angle = 1;
    aa.axis = {1,1,1};

    aa_x.angle = 1;
    aa_x.axis = {1,0,0};

    aa_y.angle = 2;
    aa_y.axis = {0,1,0};

    aa_z.angle = 1.5;
    aa_z.axis = {0,0,1};

    aa_1.angle = 1.32;
    aa_1.axis = {3.5, 23.1, 45.3};
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
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(q_cero, rot_mat);
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
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(q_identity, rot_mat);
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
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(q_i, rot_mat);
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
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(q_j, rot_mat);
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
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(q_k, rot_mat);
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
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(q, rot_mat);

  BOOST_CHECK_CLOSE(-19., rot_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-12., rot_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-4., rot_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(12, rot_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(-17, rot_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-6, rot_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(4., rot_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(-6., rot_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-1., rot_mat.at(2, 2), 0.1);

  q.normalize();
  RotationConverter<double>::convert(q, rot_mat);
  BOOST_CHECK_CLOSE(-0.4285714, rot_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.8571429, rot_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.2857143, rot_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(0.8571429, rot_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.2857143, rot_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.4285714, rot_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0.2857143, rot_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.4285714, rot_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(0.8571429, rot_mat.at(2, 2), 0.1);
}


/// MatrixToQuaternion


BOOST_FIXTURE_TEST_CASE(matrix_zero_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(rot_zero, q_rot);
  BOOST_CHECK_CLOSE(0.5, q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(0., q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(0., q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(0., q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_ones_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(rot_ones, q_rot);
  BOOST_CHECK_CLOSE(q_identity.x, q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(q_identity.y, q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(q_identity.z, q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(q_identity.w, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_identity_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(rot_identity, q_rot);
  BOOST_CHECK_CLOSE(q_identity.x, q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(q_identity.y, q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(q_identity.z, q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(q_identity.w, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(rot, q_rot);
  BOOST_CHECK_CLOSE(-0.1666667, q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(-0.5, q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(0.8333333, q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(-0.1666667, q_rot.w, 0.1);
}


/// QuaternionToAxisAngle

BOOST_FIXTURE_TEST_CASE(quaternion_cero_To_axis_Angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(q_cero, axis_angle);
  BOOST_CHECK_CLOSE(0, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_identity_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(q_identity, axis_angle);
  BOOST_CHECK_CLOSE(0., axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_i_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(q_i, axis_angle);
  BOOST_CHECK_CLOSE(3.1415926535897931, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_j_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(q_j, axis_angle);
  BOOST_CHECK_CLOSE(3.1415927, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_k_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(q_k, axis_angle);
  BOOST_CHECK_CLOSE(3.1415927, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(quaternion_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(q, axis_angle);
  BOOST_CHECK_CLOSE(0., axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.3162278, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(-0.9486833, axis_angle.axis[2], 0.1);

  q.normalize();
  RotationConverter<double>::convert(q, axis_angle);
  BOOST_CHECK_CLOSE(2.0137074, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.3162278, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(-0.9486833, axis_angle.axis[2], 0.1);
}

/// AxisAngle To Quaternion

BOOST_FIXTURE_TEST_CASE(axis_angle_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(aa, q_rot);
  BOOST_CHECK_CLOSE(0.4794255, q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(0.4794255, q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(0.4794255, q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(0.8775826, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(aa_x, q_rot);
  BOOST_CHECK_CLOSE(0.4794255, q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(0., q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(0., q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(0.8775826, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_y_angle2_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(aa_y, q_rot);
  BOOST_CHECK_CLOSE(0., q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(0.841471, q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(0., q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(0.5403023, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_z_angle_1_5_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(aa_z, q_rot);
  BOOST_CHECK_CLOSE(0., q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(0., q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(0.6816388, q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(0.7316889, q_rot.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_1_angle_to_quaternion, RotationConverterTest)
{
  Quaterniond q_rot;
  RotationConverter<double>::convert(aa_1, q_rot);
  BOOST_CHECK_CLOSE(2.1459089819070183, q_rot.x, 0.1);
  BOOST_CHECK_CLOSE(14.162999280586321, q_rot.y, 0.1);
  BOOST_CHECK_CLOSE(27.774193394396548, q_rot.z, 0.1);
  BOOST_CHECK_CLOSE(0.78999223149736508, q_rot.w, 0.1);
}

/// Rotation Matrix to Axis Angle

BOOST_FIXTURE_TEST_CASE(matrix_zero_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(rot_zero, axis_angle);
  BOOST_CHECK_CLOSE(3.1415926535897931, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_ones_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(rot_ones, axis_angle);
  BOOST_CHECK_CLOSE(0., axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_identity_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(rot_identity, axis_angle);
  BOOST_CHECK_CLOSE(0, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(1., axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0., axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_to_axis_angle, RotationConverterTest)
{
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(rot, axis_angle);
  BOOST_CHECK_CLOSE(3.4764888, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(-0.1690308, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(-0.5070926, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.8451542, axis_angle.axis[2], 0.1);
}


/// Axis Angle to Rotation Matrix


BOOST_FIXTURE_TEST_CASE(axis_angle_to_matrix, RotationConverterTest)
{
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(aa, rot_mat);
  BOOST_CHECK_CLOSE(1., rot_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(1.3011686789397567, rot_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.38177329067603627, rot_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.38177329067603627, rot_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(1, rot_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(1.3011686789397567, rot_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(1.3011686789397567, rot_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.38177329067603627, rot_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(1., rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(axis_x_angle1_to_matrix, RotationConverterTest)
{
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(aa_x, rot_mat);
  BOOST_CHECK_CLOSE(1., rot_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.5403023, rot_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(0.8414710, rot_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.8414710, rot_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(0.5403023, rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(AxisYAngle2ToMatrix, RotationConverterTest)
{
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(aa_y, rot_mat);
  BOOST_CHECK_CLOSE(-0.4161468, rot_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.9092974, rot_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(1., rot_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0.9092974, rot_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.4161468, rot_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(AxisZAngle1_5ToMatrix, RotationConverterTest)
{
  RotationMatrix<double> rot_mat;
  RotationConverter<double>::convert(aa_z, rot_mat);
  BOOST_CHECK_CLOSE(0.0707372, rot_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(0.9974950, rot_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-0.9974950, rot_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.0707372, rot_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(0., rot_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(1., rot_mat.at(2, 2), 0.1);
}


/// Euler Angles to Rotation Matrix

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyz_to_RotationMatrix, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xyz);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xzy);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yxz);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yzx);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zxy);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zyx);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xyx);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xzx);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yxy);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yzy);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zxz);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zyz);
  RotationMatrix<double> rotation_matrix;
  RotationConverter<double>::convert(eulerAngles, rotation_matrix);
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
  
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xyz);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
  BOOST_CHECK_CLOSE(0.398113, quaternion.x, 0.1);
  BOOST_CHECK_CLOSE(0.22985, quaternion.y, 0.1);
  BOOST_CHECK_CLOSE(0.857813, quaternion.z, 0.1);
  BOOST_CHECK_CLOSE(-0.22985, quaternion.w, 0.1);
}
BOOST_FIXTURE_TEST_CASE(EulerAngles_xzy_to_Quaternion, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xzy);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
  BOOST_CHECK_CLOSE(0.118979, quaternion.x, 0.1);
  BOOST_CHECK_CLOSE(-0.769095, quaternion.y, 0.1);
  BOOST_CHECK_CLOSE(0.444037, quaternion.z, 0.1);
  BOOST_CHECK_CLOSE(0.444037, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yxz_to_Quaternion, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yxz);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
  BOOST_CHECK_CLOSE(0.444037, quaternion.x, 0.1);
  BOOST_CHECK_CLOSE(0.118979, quaternion.y, 0.1);
  BOOST_CHECK_CLOSE(-0.769095, quaternion.z, 0.1);
  BOOST_CHECK_CLOSE(0.444037, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzx_to_Quaternion, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yzx);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
  BOOST_CHECK_CLOSE(0.857813, quaternion.x, 0.1);
  BOOST_CHECK_CLOSE(0.398113, quaternion.y, 0.1);
  BOOST_CHECK_CLOSE(0.22985, quaternion.z, 0.1);
  BOOST_CHECK_CLOSE(-0.22985, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxy_to_Quaternion, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zxy);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
  BOOST_CHECK_CLOSE(0.22985, quaternion.x, 0.1);
  BOOST_CHECK_CLOSE(0.857813, quaternion.y, 0.1);
  BOOST_CHECK_CLOSE(0.398113, quaternion.z, 0.1);
  BOOST_CHECK_CLOSE(-0.22985, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyx_to_Quaternion, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zyx);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
  BOOST_CHECK_CLOSE(-0.769095, quaternion.x, 0.1);
  BOOST_CHECK_CLOSE(0.444037, quaternion.y, 0.1);
  BOOST_CHECK_CLOSE(0.118979, quaternion.z, 0.1);
  BOOST_CHECK_CLOSE(0.444037, quaternion.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyx_to_Quaternion, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xyx);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xzx);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yxy);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yzy);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zxz);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zyz);
  Quaterniond quaternion;
  RotationConverter<double>::convert(eulerAngles, quaternion);
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
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xyz);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.6054406, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.4090649, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.2361737, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.8814124, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzy_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xzy);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(2.2213944, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.1327882, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(-0.858356, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.495572, axis_angle.axis[2], 0.1);
} 
  
BOOST_FIXTURE_TEST_CASE(EulerAngles_yxz_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yxz);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(2.2213944, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.495572, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.132788, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(-0.8583563, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzx_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yzx);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.6054406, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.881412, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.409065, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.236174, axis_angle.axis[2], 0.1); 
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxy_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zxy);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.6054406, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.236174, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.881412, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.409065, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyx_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zyx);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(2.22139, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(-0.858356, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.495572, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.132788, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xyx_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xyx);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.14159, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.302905, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_xzx_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::xzx);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.14159, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.302905, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(-0.673887, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yxy_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yxy);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.14159, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.302905, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(-0.673887, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_yzy_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::yzy);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.14159, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.302905, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zxz_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zxz);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.14159, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.302905, axis_angle.axis[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(EulerAngles_zyz_to_AxisAngle, RotationConverterTest)
{
  EulerAngles<double> eulerAngles(2.356194490192345, 2.5261129449194057, 0.7853981633974483, EulerAngles<double>::Axes::zyz);
  AxisAngle<double> axis_angle;
  RotationConverter<double>::convert(eulerAngles, axis_angle);
  BOOST_CHECK_CLOSE(3.14159, axis_angle.angle, 0.1);
  BOOST_CHECK_CLOSE(-0.673887, axis_angle.axis[0], 0.1);
  BOOST_CHECK_CLOSE(0.673887, axis_angle.axis[1], 0.1);
  BOOST_CHECK_CLOSE(0.302905, axis_angle.axis[2], 0.1);
}






/// Matrix to Euler Angles

/// https://www.andre-gaschler.com/rotationconverter/
/// Con Eigen sale distinto
BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI_2, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI_2, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI_2, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI_2, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI_2, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(rot_zero, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI_2, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(rot_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixOnesToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(rot_ones, eulerAngles);

  //BOOST_CHECK_CLOSE(2.356194490192345, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(2.5261129449194057, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0.7853981633974483, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(-0.61547970867038726, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(-0.61547970867038726, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(2.5261129449194057, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(2.5261129449194057, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(-0.61547970867038726, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(-0.78539816339744828, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(-0.78539816339744828, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(2.3561944901923448, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(-0.78539816339744828, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  //RotationConverter<double>::convert(rot_ones, eulerAngles);
  //BOOST_CHECK_CLOSE(0.78539816339744828, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(0.95531661812450930, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);
}

BOOST_FIXTURE_TEST_CASE(MatrixToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(rot, eulerAngles);

  BOOST_CHECK_CLOSE(1.1071487, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.111341, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.6779451, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-2.0899424, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.460554, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-3.0172377, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-0.2449786, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(1.0949141, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.896614, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(2.6779451, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.111341, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(2.0344439, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-2.3561945, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.8911225, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0.7853982, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-3.0172377, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0.460554, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-1.0516502, eulerAngles.kappa, 0.1);


  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-0.124354980700608, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(1.11024238472747, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.62244659472789, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-0.244978663126864, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(2.66571042818294, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(1.81577498992176, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-2.0899423856568, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(2.03135026886233, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-1.6951513074955, eulerAngles.kappa, 0.1);
  
  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-1.6951513074955045, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(1.1102423847274674, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-1.0516502679329891, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-1.8157749899217608, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(2.6657104281829396, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.8966139904629289, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(2.6224465947278857, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(2.0313502688623259, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-0.12435498070060774, eulerAngles.kappa, 0.1);
}



/// Quaternion to Euler angles

BOOST_FIXTURE_TEST_CASE(QuaternionCeroToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(q_cero, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIdentityToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(q_identity, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);
}


BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(q_j, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(q_k, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(TL_PI, eulerAngles.kappa, 0.1);
}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAngles, RotationConverterTest)
{
  q.normalize();
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(0.46364760900080615, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.28975170143604750, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(2.0344439357957027, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(-2.1587989303424644, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(1.0296968008377509, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.5535900500422257, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(-0.32175055439664224, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0.44291104407363896, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(1.8925468811915389, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(-2.5535900500422257, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(1.0296968008377509, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(2.1587989303424644, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(1.8925468811915389, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.44291104407363896, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-0.32175055439664224, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(2.0344439357957027, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.28975170143604750, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-0.46364760900080615, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(-0.58800260354756761, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0.54109952595714583, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(2.5535900500422257, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(1.8925468811915389, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(2.0137073708685356, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-1.8925468811915389, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(-0.46364760900080609, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(1.8605480282309441, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-0.46364760900080609, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(-2.1587989303424644, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0.54109952595714583, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.1587989303424644, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(0.32175055439664219, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(2.0137073708685356, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-0.32175055439664219, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  RotationConverter<double>::convert(q, eulerAngles);
  BOOST_CHECK_CLOSE(-2.0344439357957027, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(1.8605480282309441, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(1.1071487177940904, eulerAngles.kappa, 0.1);
}


/// AxisAngle to EulerAngles



BOOST_FIXTURE_TEST_CASE(axis_angle_to_euler_angles, RotationConverterTest)
{
//  EulerAngles<double> eulerAngles;
//  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(1.1071488836573031, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(-0.11134090351284165, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(-2.6779450961087030, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::xyx;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yzy;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::zyz;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::xzx;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
//
//  eulerAngles.axes = EulerAngles<double>::Axes::yxy;
//  RotationConverter<double>::convert(aa, eulerAngles);
//  BOOST_CHECK_CLOSE(, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(, eulerAngles.kappa, 0.1);
}


BOOST_AUTO_TEST_SUITE_END()