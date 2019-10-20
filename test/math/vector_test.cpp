#include <gtest/gtest.h>

#include <tidop/math/algebra/vector.h>

using namespace tl::math;


class VectorTest
  : public testing::Test
{
public:

  VectorTest() {}

protected:

  virtual void SetUp() override
  {
    _vect_2_d[0] = 1.1;
    _vect_2_d[1] = 3.5;

    _vect_3_d[0] = 3.4;
    _vect_3_d[1] = 5.7;
    _vect_3_d[2] = -3.4;

    _vect_4_d[0] = 3.4;
    _vect_4_d[1] = 5.7;
    _vect_4_d[2] = -3.4;
    _vect_4_d[3] = 5.4;

  }

  virtual void TearDown() override
  {
    
  }

  Vector<3> _vect_def;
  Vector<2> _vect_2_d;
  Vector<3> _vect_3_d;
  Vector<4> _vect_4_d;
};


TEST_F(VectorTest, DefaultConstructor)
{
  double ini_value = -std::numeric_limits<double>().max();

  for (size_t i = 0; i < _vect_def.size(); i++){
    EXPECT_EQ(ini_value, _vect_def[i]);
  }
}

TEST_F(VectorTest, CopyConstructor)
{
  Vector<3, double> copy(_vect_3_d);
  for (size_t i = 0; i < _vect_def.size(); i++){
    EXPECT_EQ(_vect_3_d[i], copy[i]);
  }
}

TEST_F(VectorTest, MoveConstructor)
{
  Vector<3, double> move(Vector<3, double>({1.,2.,3.}));
  EXPECT_EQ(1., move[0]);
  EXPECT_EQ(2., move[1]);
  EXPECT_EQ(3., move[2]);
}

TEST_F(VectorTest, initializer_list)
{
  Vector<4, double> vect{{1.,2.,3.,4.}};
  EXPECT_EQ(1., vect[0]);
  EXPECT_EQ(2., vect[1]);
  EXPECT_EQ(3., vect[2]);
  EXPECT_EQ(4., vect[3]);
}

TEST_F(VectorTest, size)
{
  EXPECT_EQ(3, _vect_def.size());
  EXPECT_EQ(2, _vect_2_d.size());
}

TEST_F(VectorTest, iterator)
{
  Vector<4>::iterator it = _vect_4_d.begin();
  EXPECT_EQ( 3.4, *it++);
  EXPECT_EQ( 5.7, *it++);
  EXPECT_EQ(-3.4, *it++);
  EXPECT_EQ( 5.4, *it++);
  EXPECT_TRUE(it == _vect_4_d.end());
}

TEST_F(VectorTest, value_at)
{
  EXPECT_EQ(1.1, _vect_2_d[0]);
  EXPECT_EQ(3.5, _vect_2_d[1]);

  EXPECT_EQ(1.1, _vect_2_d.at(0));
  EXPECT_EQ(3.5, _vect_2_d.at(1));
}

TEST_F(VectorTest, module)
{
  ///TODO
}

TEST_F(VectorTest, normalize)
{
  ///TODO
}

TEST_F(VectorTest, zero)
{
  Vector<3, int> _zero = Vector<3, int>::zero();

  EXPECT_EQ(0, _zero[0]);
  EXPECT_EQ(0, _zero[1]);
  EXPECT_EQ(0, _zero[2]);
}


/* Operaciones unarias */

TEST_F(VectorTest, plus)
{
  Vector<4> v4 = +_vect_4_d;

  EXPECT_EQ( 3.4, v4[0]);
  EXPECT_EQ( 5.7, v4[1]);
  EXPECT_EQ(-3.4, v4[2]);
  EXPECT_EQ( 5.4, v4[3]);
}

TEST_F(VectorTest, minus)
{
  Vector<4> v4 = -_vect_4_d;

  EXPECT_EQ(-3.4, v4[0]);
  EXPECT_EQ(-5.7, v4[1]);
  EXPECT_EQ( 3.4, v4[2]);
  EXPECT_EQ(-5.4, v4[3]);
}

/* Operaciones binarias entre vectores */

/// Suma de vectores

TEST_F(VectorTest, addition)
{
  Vector<3, int> v1 = {1, 0, 3};
  Vector<3, int> v2 = {-1, 4, 2};
  Vector<3, int> v3 = v1 + v2;

  EXPECT_EQ(0, v3[0]);
  EXPECT_EQ(4, v3[1]);
  EXPECT_EQ(5, v3[2]);

  v1 += v2;

  EXPECT_EQ(0, v1[0]);
  EXPECT_EQ(4, v1[1]);
  EXPECT_EQ(5, v1[2]);
}

TEST_F(VectorTest, subtraction)
{
  Vector<3, int> v1 = {1, 0, 3};
  Vector<3, int> v2 = {-1, 4, 2};
  Vector<3, int> v3 = v1 - v2;

  EXPECT_EQ( 2, v3[0]);
  EXPECT_EQ(-4, v3[1]);
  EXPECT_EQ( 1, v3[2]);

  v1 -= v2;

  EXPECT_EQ( 2, v1[0]);
  EXPECT_EQ(-4, v1[1]);
  EXPECT_EQ( 1, v1[2]);
}

/// Producto

TEST_F(VectorTest, multiplication)
{
  Vector<3, int> v1 = {1, 0, 3};
  Vector<3, int> v2 = {-1, 4, 2};
  Vector<3, int> v3 = v1 * v2;

  EXPECT_EQ(-1, v3[0]);
  EXPECT_EQ( 0, v3[1]);
  EXPECT_EQ( 6, v3[2]);

  v1 *= v2;

  EXPECT_EQ(-1, v1[0]);
  EXPECT_EQ( 0, v1[1]);
  EXPECT_EQ( 6, v1[2]);

}


TEST_F(VectorTest, VectorScalar)
{
  Vector<3, int> v1 = {1, 0, 3};
  Vector<3, int> v2 = v1 * 10;

  EXPECT_EQ(10, v2[0]);
  EXPECT_EQ( 0, v2[1]);
  EXPECT_EQ(30, v2[2]);

  v1 *= 10;

  EXPECT_EQ(10, v1[0]);
  EXPECT_EQ( 0, v1[1]);
  EXPECT_EQ(30, v1[2]);
}


TEST_F(VectorTest, ScalarVector)
{
  Vector<3, int> v1 = {1, 0, 3};
  Vector<3, int> v2 = 10 * v1;

  EXPECT_EQ(10, v2[0]);
  EXPECT_EQ( 0, v2[1]);
  EXPECT_EQ(30, v2[2]);

}

TEST_F(VectorTest, dotProduct)
{

}
