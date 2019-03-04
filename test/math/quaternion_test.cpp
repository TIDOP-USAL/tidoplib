#include <gtest/gtest.h>

#include <tidop/math/algebra/quaternion.h>
#include <tidop/math/algebra/rotation_matrix.h>

using namespace tl::math;


class QuaternionTest 
  : public testing::Test
{
public:

  QuaternionTest(){}

protected:

  virtual void SetUp() override
  {
    q_cero = Quaternionf::zero();
    q_identity = Quaternionf::identity();
    q_i = Quaternionf::i();
    q_j = Quaternionf::j();
    q_k = Quaternionf::k();
    q = Quaternionf(0.f, 1.f, -3.f, 2.f);
    q2 = Quaternionf(1.f, 3.f, -5.f, 1.f);

    RotationMatrix<3, float> rot;
    rot.at(0, 0) = -0.8888889f;
    rot.at(0, 1) = 0.4444444f;
    rot.at(0, 2) = -0.1111111f;
    rot.at(1, 0) = -0.1111111f;
    rot.at(1, 1) = -0.4444444f;
    rot.at(1, 2) = -0.8888889f;
    rot.at(2, 0) = -0.4444444f;
    rot.at(2, 1) = -0.7777778f;
    rot.at(2, 2) =  0.4444444f;
    
    //rot.at(0, 0) = 1.19209e-007;
    //rot.at(0, 1) = 0.f;
    //rot.at(0, 2) = 1.f;
    //rot.at(1, 0) = 0.968583f;
    //rot.at(1, 2) = -0.24869f;
    //rot.at(1, 2) = 0.f;
    //rot.at(2, 0) = 0.24869f;
    //rot.at(2, 1) = 0.968583f;
    //rot.at(2, 2) = 1.19209e-007;

    q_rot = new Quaternionf(rot);
  }
 
  virtual void TearDown() override
  {
    if (q_rot) {
      delete q_rot;
      q_rot = nullptr;
    }
  }

  Quaternionf q_uninitialized;
  Quaternionf q_cero;
  Quaternionf q_identity;
  Quaternionf q_i;
  Quaternionf q_j;
  Quaternionf q_k;
  Quaternionf q;
  Quaternionf q2;
  Quaternionf *q_rot;
};

/* Clase Quaternion */

TEST_F(QuaternionTest, DefaultConstructor)
{
  EXPECT_EQ(-std::numeric_limits<float>().max(), q_uninitialized.w);
  EXPECT_EQ(-std::numeric_limits<float>().max(), q_uninitialized.x);
  EXPECT_EQ(-std::numeric_limits<float>().max(), q_uninitialized.y);
  EXPECT_EQ(-std::numeric_limits<float>().max(), q_uninitialized.z);
}

TEST_F(QuaternionTest, RotationMatrixConstructor)
{
  EXPECT_NEAR(-0.1666667f, q_rot->x, 0.0001);
	EXPECT_NEAR(-0.5f, q_rot->y, 0.0001);
	EXPECT_NEAR(0.8333333f, q_rot->z, 0.0001);
	EXPECT_NEAR(-0.1666667f, q_rot->w, 0.0001);
}

TEST_F(QuaternionTest, cero)
{
  EXPECT_EQ(0.f, q_cero.w);
  EXPECT_EQ(0.f, q_cero.x);
  EXPECT_EQ(0.f, q_cero.y);
  EXPECT_EQ(0.f, q_cero.z);
}

TEST_F(QuaternionTest, identity)
{
  EXPECT_EQ(1.f, q_identity.w);
  EXPECT_EQ(0.f, q_identity.x);
  EXPECT_EQ(0.f, q_identity.y);
  EXPECT_EQ(0.f, q_identity.z);
}

TEST_F(QuaternionTest, i)
{
  EXPECT_EQ(0.f, q_i.w);
  EXPECT_EQ(1.f, q_i.x);
  EXPECT_EQ(0.f, q_i.y);
  EXPECT_EQ(0.f, q_i.z);
}

TEST_F(QuaternionTest, j)
{
  EXPECT_EQ(0.f, q_j.w);
  EXPECT_EQ(0.f, q_j.x);
  EXPECT_EQ(1.f, q_j.y);
  EXPECT_EQ(0.f, q_j.z);
}

TEST_F(QuaternionTest, k)
{
  EXPECT_EQ(0.f, q_k.w);
  EXPECT_EQ(0.f, q_k.x);
  EXPECT_EQ(0.f, q_k.y);
  EXPECT_EQ(1.f, q_k.z);
}

TEST_F(QuaternionTest, parameters)
{
  EXPECT_EQ(2.f, q.w);
  EXPECT_EQ(0.f, q.x);
  EXPECT_EQ(1.f, q.y);
  EXPECT_EQ(-3.f, q.z);
}

TEST_F(QuaternionTest, normalize)
{
  q.normalize();
  EXPECT_NEAR(0.0f, q.x, 0.001);
  EXPECT_NEAR(0.2672612f, q.y, 0.001);
  EXPECT_NEAR(-0.8017837f, q.z, 0.001);
  EXPECT_NEAR(0.5345225f, q.w, 0.001);
}

TEST_F(QuaternionTest, conjugate)
{
  Quaternionf conjugate = q.conjugate();
  EXPECT_EQ(2.f, conjugate.w);
  EXPECT_EQ(0.f, conjugate.x);
  EXPECT_EQ(-1.f, conjugate.y);
  EXPECT_EQ(3.f, conjugate.z);
}

TEST_F(QuaternionTest, norm)
{
  float norm = q.norm();
  EXPECT_NEAR(3.741657f, norm, 0.001);
}

TEST_F(QuaternionTest, inverse)
{
  Quaternionf inverse = q.inverse();
  EXPECT_NEAR( 0.0f,          inverse.x, 0.001);
  EXPECT_NEAR(-0.0714285714f, inverse.y, 0.001);
  EXPECT_NEAR( 0.2142857142f, inverse.z, 0.001);
  EXPECT_NEAR( 0.1428571428f, inverse.w, 0.001);
}

/* Operaciones unarias */

TEST_F(QuaternionTest, neg)
{
  Quaternionf neg = -q;
  EXPECT_EQ( 0.f, neg.x);
  EXPECT_EQ(-1.f, neg.y);
  EXPECT_EQ( 3.f, neg.z);
  EXPECT_EQ(-2.f, neg.w);
}

/* */

TEST_F(QuaternionTest, multiplication)
{
  Quaternionf multi = q * q2;
  EXPECT_EQ(  6.f, multi.x);
  EXPECT_EQ(  4.f, multi.y);
  EXPECT_EQ(-14.f, multi.z);
  EXPECT_EQ(-16.f, multi.w);
}

TEST_F(QuaternionTest, sum)
{
  Quaternionf sum = q + q2;
  EXPECT_EQ( 1.f, sum.x);
  EXPECT_EQ( 4.f, sum.y);
  EXPECT_EQ(-8.f, sum.z);
  EXPECT_EQ( 3.f, sum.w);
}

//TEST_F(QuaternionTest, subtraction)
//{
//  Quaternionf subtraction = q - q2;
//  EXPECT_EQ(  6.f, subtraction.x);
//  EXPECT_EQ(  4.f, subtraction.y);
//  EXPECT_EQ(-14.f, subtraction.z);
//  EXPECT_EQ(-16.f, subtraction.w);
//}


//Quaternion<T> operator*(const Quaternion<T> &quaternion, T scalar)
//Quaternion<T> operator*(T scalar, const Quaternion<T> &quaternion)
//Quaternion<T> operator / (const Quaternion<T> &quaternion, T scalar)
//Quaternion<T> &operator += (Quaternion<T> &quat1, const Quaternion<T> &quat2)
//Quaternion<T> &operator -= (Quaternion<T> &quat1, const Quaternion<T> &quat2)
//Quaternion<T> &operator *= (Quaternion<T> &quaternion, T scalar)
//Quaternion<T> &operator /= (Quaternion<T> &quaternion, T scalar)
//T dot(const Quaternion<T> &quat1, const Quaternion<T> &quat2)