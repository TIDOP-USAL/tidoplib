#define BOOST_TEST_MODULE Tidop quaternion test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/quaternion.h>
#include <tidop/math/algebra/rotation_matrix.h>

using namespace tl::math;

BOOST_AUTO_TEST_SUITE(QuaternionTestSuite)

struct QuaternionTest 
{

  QuaternionTest()
    //: q_rot()
  {}
   
  ~QuaternionTest()
  {
    //if (q_rot) {
    //  delete q_rot;
    //  q_rot = nullptr;
    //}
  }

  void setup()
  {
    q_cero = Quaternionf::zero();
    q_identity = Quaternionf::identity();
    q_i = Quaternionf::i();
    q_j = Quaternionf::j();
    q_k = Quaternionf::k();
    q = Quaternionf(0.f, 1.f, -3.f, 2.f);
    q2 = Quaternionf(1.f, 3.f, -5.f, 1.f);

    //RotationMatrix<float> rot;
    //rot.at(0, 0) = -0.8888889f;
    //rot.at(0, 1) = 0.4444444f;
    //rot.at(0, 2) = -0.1111111f;
    //rot.at(1, 0) = -0.1111111f;
    //rot.at(1, 1) = -0.4444444f;
    //rot.at(1, 2) = -0.8888889f;
    //rot.at(2, 0) = -0.4444444f;
    //rot.at(2, 1) = -0.7777778f;
    //rot.at(2, 2) =  0.4444444f;

    //q_rot = new Quaternionf(rot);
  }
 
  void teardown()
  {

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

BOOST_FIXTURE_TEST_CASE(default_constructor, QuaternionTest)
{
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.w);
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.x);
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.y);
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.z);
}

//BOOST_FIXTURE_TEST_CASE(QuaternionTest, RotationMatrixConstructor)
//{
//  BOOST_CHECK_CLOSE(-0.1666667f, q_rot->x, 0.0001);
//	BOOST_CHECK_CLOSE(-0.5f, q_rot->y, 0.0001);
//	BOOST_CHECK_CLOSE(0.8333333f, q_rot->z, 0.0001);
//	BOOST_CHECK_CLOSE(-0.1666667f, q_rot->w, 0.0001);
//}

BOOST_FIXTURE_TEST_CASE(cero, QuaternionTest)
{
  BOOST_CHECK_EQUAL(0.f, q_cero.w);
  BOOST_CHECK_EQUAL(0.f, q_cero.x);
  BOOST_CHECK_EQUAL(0.f, q_cero.y);
  BOOST_CHECK_EQUAL(0.f, q_cero.z);
}

BOOST_FIXTURE_TEST_CASE(identity, QuaternionTest)
{
  BOOST_CHECK_EQUAL(1.f, q_identity.w);
  BOOST_CHECK_EQUAL(0.f, q_identity.x);
  BOOST_CHECK_EQUAL(0.f, q_identity.y);
  BOOST_CHECK_EQUAL(0.f, q_identity.z);
}

BOOST_FIXTURE_TEST_CASE(i, QuaternionTest)
{
  BOOST_CHECK_EQUAL(0.f, q_i.w);
  BOOST_CHECK_EQUAL(1.f, q_i.x);
  BOOST_CHECK_EQUAL(0.f, q_i.y);
  BOOST_CHECK_EQUAL(0.f, q_i.z);
}

BOOST_FIXTURE_TEST_CASE(j, QuaternionTest)
{
  BOOST_CHECK_EQUAL(0.f, q_j.w);
  BOOST_CHECK_EQUAL(0.f, q_j.x);
  BOOST_CHECK_EQUAL(1.f, q_j.y);
  BOOST_CHECK_EQUAL(0.f, q_j.z);
}

BOOST_FIXTURE_TEST_CASE(k, QuaternionTest)
{
  BOOST_CHECK_EQUAL(0.f, q_k.w);
  BOOST_CHECK_EQUAL(0.f, q_k.x);
  BOOST_CHECK_EQUAL(0.f, q_k.y);
  BOOST_CHECK_EQUAL(1.f, q_k.z);
}

BOOST_FIXTURE_TEST_CASE(parameters, QuaternionTest)
{
  BOOST_CHECK_EQUAL(2.f, q.w);
  BOOST_CHECK_EQUAL(0.f, q.x);
  BOOST_CHECK_EQUAL(1.f, q.y);
  BOOST_CHECK_EQUAL(-3.f, q.z);
}

BOOST_FIXTURE_TEST_CASE(normalize, QuaternionTest)
{
  q.normalize();
  BOOST_CHECK_CLOSE(0.0f, q.x, 0.1);
  BOOST_CHECK_CLOSE(0.2672612f, q.y, 0.1);
  BOOST_CHECK_CLOSE(-0.8017837f, q.z, 0.1);
  BOOST_CHECK_CLOSE(0.5345225f, q.w, 0.1);
}

BOOST_FIXTURE_TEST_CASE(conjugate, QuaternionTest)
{
  Quaternionf conjugate = q.conjugate();
  BOOST_CHECK_EQUAL(2.f, conjugate.w);
  BOOST_CHECK_EQUAL(0.f, conjugate.x);
  BOOST_CHECK_EQUAL(-1.f, conjugate.y);
  BOOST_CHECK_EQUAL(3.f, conjugate.z);
}

BOOST_FIXTURE_TEST_CASE(norm, QuaternionTest)
{
  float norm = q.norm();
  BOOST_CHECK_CLOSE(3.741657f, norm, 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse, QuaternionTest)
{
  Quaternionf inverse = q.inverse();
  BOOST_CHECK_CLOSE( 0.0f,          inverse.x, 0.1);
  BOOST_CHECK_CLOSE(-0.0714285714f, inverse.y, 0.1);
  BOOST_CHECK_CLOSE( 0.2142857142f, inverse.z, 0.1);
  BOOST_CHECK_CLOSE( 0.1428571428f, inverse.w, 0.1);
}

/* Operaciones unarias */

BOOST_FIXTURE_TEST_CASE(neg, QuaternionTest)
{
  Quaternionf neg = -q;
  BOOST_CHECK_EQUAL( 0.f, neg.x);
  BOOST_CHECK_EQUAL(-1.f, neg.y);
  BOOST_CHECK_EQUAL( 3.f, neg.z);
  BOOST_CHECK_EQUAL(-2.f, neg.w);
}

/* */

BOOST_FIXTURE_TEST_CASE(multiplication, QuaternionTest)
{
  Quaternionf multi = q * q2;
  BOOST_CHECK_EQUAL(  6.f, multi.x);
  BOOST_CHECK_EQUAL(  4.f, multi.y);
  BOOST_CHECK_EQUAL(-14.f, multi.z);
  BOOST_CHECK_EQUAL(-16.f, multi.w);
}

BOOST_FIXTURE_TEST_CASE(sum, QuaternionTest)
{
  Quaternionf sum = q + q2;
  BOOST_CHECK_EQUAL( 1.f, sum.x);
  BOOST_CHECK_EQUAL( 4.f, sum.y);
  BOOST_CHECK_EQUAL(-8.f, sum.z);
  BOOST_CHECK_EQUAL( 3.f, sum.w);
}

//BOOST_FIXTURE_TEST_CASE(QuaternionTest, subtraction)
//{
//  Quaternionf subtraction = q - q2;
//  BOOST_CHECK_EQUAL(  6.f, subtraction.x);
//  BOOST_CHECK_EQUAL(  4.f, subtraction.y);
//  BOOST_CHECK_EQUAL(-14.f, subtraction.z);
//  BOOST_CHECK_EQUAL(-16.f, subtraction.w);
//}


//Quaternion<T> operator*(const Quaternion<T> &quaternion, T scalar)
//Quaternion<T> operator*(T scalar, const Quaternion<T> &quaternion)
//Quaternion<T> operator / (const Quaternion<T> &quaternion, T scalar)
//Quaternion<T> &operator += (Quaternion<T> &quat1, const Quaternion<T> &quat2)
//Quaternion<T> &operator -= (Quaternion<T> &quat1, const Quaternion<T> &quat2)
//Quaternion<T> &operator *= (Quaternion<T> &quaternion, T scalar)
//Quaternion<T> &operator /= (Quaternion<T> &quaternion, T scalar)
//T dot(const Quaternion<T> &quat1, const Quaternion<T> &quat2)


BOOST_AUTO_TEST_SUITE_END()