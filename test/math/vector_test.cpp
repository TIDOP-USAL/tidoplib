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

TEST_F(VectorTest, value_at)
{
  EXPECT_EQ(1.1, _vect_2_d[0]);
  EXPECT_EQ(3.5, _vect_2_d[1]);
}

TEST_F(VectorTest, zero)
{
  Vector<3, int> _zero = Vector<3, int>::zero();

  EXPECT_EQ(0, _zero[0]);
  EXPECT_EQ(0, _zero[1]);
  EXPECT_EQ(0, _zero[2]);
}