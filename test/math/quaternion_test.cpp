#define BOOST_TEST_MODULE Tidop quaternion test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/quaternion.h>
#include <tidop/math/algebra/rotation_matrix.h>

using namespace tl::math;

BOOST_AUTO_TEST_SUITE(QuaternionTestSuite)

struct QuaternionTest 
{

  QuaternionTest()
  {}
   
  ~QuaternionTest()
  {
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

    q_d = Quaterniond(1., 3., -5., 1.);
  }
 
  void teardown()
  {

  }

  Quaternionf q_uninitialized;
  Quaterniond q_d_uninitialized;
  Quaternionf q_cero;
  Quaternionf q_identity;
  Quaternionf q_i;
  Quaternionf q_j;
  Quaternionf q_k;
  Quaternionf q;
  Quaternionf q2;
  Quaternionf *q_rot;
  Quaterniond q_d;
};

/* Clase Quaternion */

BOOST_FIXTURE_TEST_CASE(default_constructor, QuaternionTest)
{
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.w);
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.x);
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.y);
  BOOST_CHECK_EQUAL(-std::numeric_limits<float>().max(), q_uninitialized.z);

  BOOST_CHECK_EQUAL(-std::numeric_limits<double>().max(), q_d_uninitialized.w);
  BOOST_CHECK_EQUAL(-std::numeric_limits<double>().max(), q_d_uninitialized.x);
  BOOST_CHECK_EQUAL(-std::numeric_limits<double>().max(), q_d_uninitialized.y);
  BOOST_CHECK_EQUAL(-std::numeric_limits<double>().max(), q_d_uninitialized.z);
}

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

BOOST_FIXTURE_TEST_CASE(rotationType, QuaternionTest)
{
  BOOST_CHECK(Rotation::Type::quaternion == q_uninitialized.rotationType());
  BOOST_CHECK(Rotation::Type::quaternion == q_d_uninitialized.rotationType());
  BOOST_CHECK(Rotation::Type::quaternion == q_cero.rotationType());
  BOOST_CHECK(Rotation::Type::quaternion == q_identity.rotationType());
  BOOST_CHECK(Rotation::Type::quaternion == q_i.rotationType());
  BOOST_CHECK(Rotation::Type::quaternion == q_j.rotationType());
  BOOST_CHECK(Rotation::Type::quaternion == q_k.rotationType());
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

  q_cero.normalize();
  BOOST_CHECK_EQUAL(0.f, q_cero.w);
  BOOST_CHECK_EQUAL(0.f, q_cero.x);
  BOOST_CHECK_EQUAL(0.f, q_cero.y);
  BOOST_CHECK_EQUAL(0.f, q_cero.z);
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

  Quaternionf quaternion_inverse = q_cero.inverse();
  BOOST_CHECK_CLOSE(0.0f, quaternion_inverse.x, 0.1);
  BOOST_CHECK_CLOSE(0.0f, quaternion_inverse.y, 0.1);
  BOOST_CHECK_CLOSE(0.0f, quaternion_inverse.z, 0.1);
  BOOST_CHECK_CLOSE(0.0f, quaternion_inverse.w, 0.1);

}

/* Operaciones unarias */

BOOST_FIXTURE_TEST_CASE(neg, QuaternionTest)
{
  Quaternionf neg = -q;
  BOOST_CHECK_EQUAL( 0.f, neg.x);
  BOOST_CHECK_EQUAL(-1.f, neg.y);
  BOOST_CHECK_EQUAL( 3.f, neg.z);
  BOOST_CHECK_EQUAL(-2.f, neg.w);

  Quaterniond neg_d = -q_d;
  BOOST_CHECK_EQUAL(-1., neg_d.x);
  BOOST_CHECK_EQUAL(-3., neg_d.y);
  BOOST_CHECK_EQUAL(5., neg_d.z);
  BOOST_CHECK_EQUAL(-1., neg_d.w);
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
BOOST_FIXTURE_TEST_CASE(sum2, QuaternionTest)
{
  Quaternionf sum = q;
  sum += q2;
  BOOST_CHECK_EQUAL(1.f, sum.x);
  BOOST_CHECK_EQUAL(4.f, sum.y);
  BOOST_CHECK_EQUAL(-8.f, sum.z);
  BOOST_CHECK_EQUAL(3.f, sum.w);

}
BOOST_FIXTURE_TEST_CASE(subtraction, QuaternionTest)
{
  Quaternionf subtraction = q - q2;
  BOOST_CHECK_EQUAL(-1.f, subtraction.x);
  BOOST_CHECK_EQUAL(-2.f, subtraction.y);
  BOOST_CHECK_EQUAL(2.f, subtraction.z);
  BOOST_CHECK_EQUAL(1.f, subtraction.w);
}
BOOST_FIXTURE_TEST_CASE(subtraction2, QuaternionTest)
{
  Quaternionf subtraction = q;
  subtraction -= q2;
  BOOST_CHECK_EQUAL(-1.f, subtraction.x);
  BOOST_CHECK_EQUAL(-2.f, subtraction.y);
  BOOST_CHECK_EQUAL(2.f, subtraction.z);
  BOOST_CHECK_EQUAL(1.f, subtraction.w);

}
BOOST_FIXTURE_TEST_CASE(quaternion_by_scalar, QuaternionTest)
{
  Quaternionf multi = q * 2.f;
  BOOST_CHECK_EQUAL(0.f, multi.x);
  BOOST_CHECK_EQUAL(2.f, multi.y);
  BOOST_CHECK_EQUAL(-6.f, multi.z);
  BOOST_CHECK_EQUAL(4.f, multi.w);
}

BOOST_FIXTURE_TEST_CASE(scalar_by_quaternion, QuaternionTest)
{
  Quaternionf multi = 2.f * q;
  BOOST_CHECK_EQUAL(0.f, multi.x);
  BOOST_CHECK_EQUAL(2.f, multi.y);
  BOOST_CHECK_EQUAL(-6.f, multi.z);
  BOOST_CHECK_EQUAL(4.f, multi.w);
}

BOOST_FIXTURE_TEST_CASE(quaternion_divided_by_scalar, QuaternionTest)
{
  Quaternionf multi = q / 2.f;
  BOOST_CHECK_EQUAL(0.f, multi.x);
  BOOST_CHECK_EQUAL(0.5f, multi.y);
  BOOST_CHECK_EQUAL(-1.5f, multi.z);
  BOOST_CHECK_EQUAL(1.f, multi.w);

  multi = q / 0.f;
  BOOST_CHECK_EQUAL(0.f, multi.x);
  BOOST_CHECK_EQUAL(0.f, multi.y);
  BOOST_CHECK_EQUAL(0.f, multi.z);
  BOOST_CHECK_EQUAL(0.f, multi.w);
}

BOOST_FIXTURE_TEST_CASE(scalar_divided_by_quaternion, QuaternionTest)
{
  Quaternionf multi = 2.f / q;
  BOOST_CHECK_EQUAL(0.f, multi.x);
  BOOST_CHECK_EQUAL(0.5f, multi.y);
  BOOST_CHECK_EQUAL(-1.5f, multi.z);
  BOOST_CHECK_EQUAL(1.f, multi.w);

  multi = 0.f / q;
  BOOST_CHECK_EQUAL(0.f, multi.x);
  BOOST_CHECK_EQUAL(0.f, multi.y);
  BOOST_CHECK_EQUAL(0.f, multi.z);
  BOOST_CHECK_EQUAL(0.f, multi.w);
}


//T dot(const Quaternion<T> &quat1, const Quaternion<T> &quat2)


BOOST_AUTO_TEST_SUITE_END()