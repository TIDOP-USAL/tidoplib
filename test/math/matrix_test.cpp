#include <gtest/gtest.h>

#include "math/matrix.h"

using namespace TL::math;


class MatrixTest 
  : public testing::Test
{
public:

  MatrixTest() : _mat_4x4_d(nullptr){}

protected:

  virtual void SetUp() override
  {
    _mat_3x3_d.at(0, 0) = 1.5;
    _mat_3x3_d.at(0, 1) = 0.0;
    _mat_3x3_d.at(0, 2) = 2.5;
    _mat_3x3_d.at(1, 0) = 1.0;
    _mat_3x3_d.at(1, 1) = 1.0;
    _mat_3x3_d.at(1, 2) = 1.2;
    _mat_3x3_d.at(2, 0) = 1.3;
    _mat_3x3_d.at(2, 1) = 2.6;
    _mat_3x3_d.at(2, 2) = 0.3;

    //_mat_2x2_f = FactoryMatrix::create(FactoryMatrix::Diag);
  }
 
  virtual void TearDown() override
  {
    if (_mat_4x4_d) delete _mat_4x4_d, _mat_4x4_d = nullptr;
  }

  Matrix<2, 2> _mat_2x2_d;
  Matrix<3, 3> _mat_3x3_d;
  Matrix<4, 4> *_mat_4x4_d;
  Matrix<2, 2, float> _mat_2x2_f;
  Matrix<3, 3, float> _mat_3x3_f;
  Matrix<4, 4, float> _mat_4x4_f;
};


TEST_F(MatrixTest, DefaultConstructor)
{
  EXPECT_EQ(2, _mat_2x2_d.rows());
  EXPECT_EQ(2, _mat_2x2_d.cols());
  for (size_t r = 0; r < _mat_2x2_d.rows(); r++){
    for (size_t c = 0; c < _mat_2x2_d.cols(); c++){
      EXPECT_EQ(0, _mat_2x2_d.at(r, c));
    }
  }

  EXPECT_EQ(3, _mat_3x3_d.rows());
  EXPECT_EQ(3, _mat_3x3_d.cols());
  EXPECT_EQ(1.5, _mat_3x3_d.at(0, 0));
  EXPECT_EQ(0.0, _mat_3x3_d.at(0, 1));
  EXPECT_EQ(2.5, _mat_3x3_d.at(0, 2));
  EXPECT_EQ(1.0, _mat_3x3_d.at(1, 0));
  EXPECT_EQ(1.0, _mat_3x3_d.at(1, 1));
  EXPECT_EQ(1.2, _mat_3x3_d.at(1, 2));
  EXPECT_EQ(1.3, _mat_3x3_d.at(2, 0));
  EXPECT_EQ(2.6, _mat_3x3_d.at(2, 1));
  EXPECT_EQ(0.3, _mat_3x3_d.at(2, 2));
  
  EXPECT_EQ(3, _mat_3x3_f.rows());
  EXPECT_EQ(3, _mat_3x3_f.cols());
  for (size_t r = 0; r < _mat_3x3_f.rows(); r++){
    for (size_t c = 0; c < _mat_3x3_f.cols(); c++){
      EXPECT_EQ(0.f, _mat_3x3_f.at(r, c));
    }
  }

}

TEST_F(MatrixTest, isEmpty)
{
  //EXPECT_TRUE(_mat_4x4_f.isEmpty());
  //EXPECT_FALSE(_mat_3x3_d.isEmpty());
}

TEST_F(MatrixTest, isDiag)
{

}

TEST_F(MatrixTest, isSquared)
{

}