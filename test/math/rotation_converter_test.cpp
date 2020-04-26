#define BOOST_TEST_MODULE Tidop rotation converter test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/rotations.h>

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


/// Hasta aqui comprobado con eigen

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
}

BOOST_FIXTURE_TEST_CASE(QuaternionIToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(-3.1415927, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::xzy;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(3.1415927, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(3.1415927, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(3.1415927, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(-3.1415927, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(0., eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-3.1415927, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-3.1415927, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(q_i, eulerAngles);
  BOOST_CHECK_CLOSE(3.1415927, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0., eulerAngles.kappa, 0.1);
}


BOOST_FIXTURE_TEST_CASE(QuaternionJToEulerAngles, RotationConverterTest)
{

}

BOOST_FIXTURE_TEST_CASE(QuaternionKToEulerAngles, RotationConverterTest)
{

}

BOOST_FIXTURE_TEST_CASE(QuaternionToEulerAngles, RotationConverterTest)
{

}



/// Matrix to Euler Angles

BOOST_FIXTURE_TEST_CASE(MatrixZeroToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(rot_zero, eulerAngles);

  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);

}

//BOOST_FIXTURE_TEST_CASE(MatrixOnesToEulerAngles, RotationConverterTest)
//{
//  EulerAngles<double> eulerAngles;
//  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
//  RotationConverter<double>::convert(rot_ones, eulerAngles);

//  BOOST_CHECK_CLOSE(2.356194490192345, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(2.5261129449194057, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(0.7853981633974483, eulerAngles.kappa, 0.1);
//}

BOOST_FIXTURE_TEST_CASE(MatrixIdentityToEulerAngles, RotationConverterTest)
{
  EulerAngles<double> eulerAngles;
  eulerAngles.axes = EulerAngles<double>::Axes::xyz;
  RotationConverter<double>::convert(rot_identity, eulerAngles);

  BOOST_CHECK_CLOSE(0, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(0, eulerAngles.kappa, 0.1);
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
  BOOST_CHECK_CLOSE(-3.0172377, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-0.460554, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yxz;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(1.0949141, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(-0.2449786, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.896614, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::yzx;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(2.0344439, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(2.6779451, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-0.111341, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zxy;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-0.8911225, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0.7853982, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-2.3561945, eulerAngles.kappa, 0.1);

  eulerAngles.axes = EulerAngles<double>::Axes::zyx;
  RotationConverter<double>::convert(rot, eulerAngles);
  BOOST_CHECK_CLOSE(-1.0516502, eulerAngles.omega, 0.1);
  BOOST_CHECK_CLOSE(0.460554, eulerAngles.phi, 0.1);
  BOOST_CHECK_CLOSE(-3.0172377, eulerAngles.kappa, 0.1);


  eulerAngles.axes = EulerAngles<double>::Axes::zxz;
  RotationConverter<double>::convert(rot, eulerAngles);
//  BOOST_CHECK_CLOSE(3.0172376728891854, eulerAngles.omega, 0.1);
//  BOOST_CHECK_CLOSE(-1.1102423792148137, eulerAngles.phi, 0.1);
//  BOOST_CHECK_CLOSE(0.5191460588619076, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xyx;
  //RotationConverter<double>::convert(rot, eulerAngles);
  //BOOST_CHECK_CLOSE(2.896613990462929, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(-2.665710449741662, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(-1.3258177107268567, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yzy;
  //RotationConverter<double>::convert(rot, eulerAngles);
  //BOOST_CHECK_CLOSE(1.051650267932989, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(-2.0313502798876333, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(1.446441367632749, eulerAngles.kappa, 0.1);
  //
  //eulerAngles.axes = EulerAngles<double>::Axes::zyz;
  //RotationConverter<double>::convert(rot, eulerAngles);
  //BOOST_CHECK_CLOSE(1.4464413460942886, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(-1.1102423792148137, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(2.089942385656804, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::xzx;
  //RotationConverter<double>::convert(rot, eulerAngles);
  //BOOST_CHECK_CLOSE(1.3258176636680323, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(-2.665710449741662, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(0.24497861606804, eulerAngles.kappa, 0.1);

  //eulerAngles.axes = EulerAngles<double>::Axes::yxy;
  //RotationConverter<double>::convert(rot, eulerAngles);
  //BOOST_CHECK_CLOSE(2.6224465947278857, eulerAngles.omega, 0.1);
  //BOOST_CHECK_CLOSE(2.0313502798876333, eulerAngles.phi, 0.1);
  //BOOST_CHECK_CLOSE(-0.12435495916214766, eulerAngles.kappa, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()