#include <gtest/gtest.h>

#include <tidop/math/algebra/rotations.h>

using namespace tl::math;


class RotationConverterTest
  : public testing::Test
{
public:

  RotationConverterTest(){}

protected:

  virtual void SetUp() override
  {
    q_cero = Quaternionf::zero();
    q_identity = Quaternionf::identity();
    q_i = Quaternionf::i();
    q_j = Quaternionf::j();
    q_k = Quaternionf::k();
    q = Quaternionf(0.f, 1.f, -3.f, 2.f);

    rot.at(0, 0) = -0.8888889f;
    rot.at(0, 1) = 0.4444444f;
    rot.at(0, 2) = -0.1111111f;
    rot.at(1, 0) = -0.1111111f;
    rot.at(1, 1) = -0.4444444f;
    rot.at(1, 2) = -0.8888889f;
    rot.at(2, 0) = -0.4444444f;
    rot.at(2, 1) = -0.7777778f;
    rot.at(2, 2) =  0.4444444f;
  }

  virtual void TearDown() override
  {

  }

  Quaternionf q_cero;
  Quaternionf q_identity;
  Quaternionf q_i;
  Quaternionf q_j;
  Quaternionf q_k;
  Quaternionf q;

  RotationMatrix<float> rot;

};


//TEST_F(RotationConverterTest, QuaternionToMatrix)
//{
//  RotationMatrix<float> rot_mat;
//  RotationConverter<float>::convert(q, rot_mat);
//}


TEST_F(RotationConverterTest, MatrixToQuaternion)
{
  Quaternionf q_rot;
  RotationConverter<float>::convert(rot, q_rot);
  EXPECT_NEAR(-0.1666667f, q_rot.x, 0.0001);
  EXPECT_NEAR(-0.5f, q_rot.y, 0.0001);
  EXPECT_NEAR(0.8333333f, q_rot.z, 0.0001);
  EXPECT_NEAR(-0.1666667f, q_rot.w, 0.0001);
}
