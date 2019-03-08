#include <gtest/gtest.h>

#include <tidop/math/matrix.h>

using namespace tl::math;


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

    _mat_4x4_d = new Matrix<4, 4>();
    _mat_4x4_d->at(0, 0) = 4.5;    _mat_4x4_d->at(0, 1) = 2.7;  _mat_4x4_d->at(0, 2) = 5.5;  _mat_4x4_d->at(0, 3) = 4.98;
    _mat_4x4_d->at(1, 0) = 1.36;   _mat_4x4_d->at(1, 1) = 7.62;	_mat_4x4_d->at(1, 2) = 78.3; _mat_4x4_d->at(1, 3) = 45.5;
    _mat_4x4_d->at(2, 0) = 14.3;   _mat_4x4_d->at(2, 1) = 45.3;	_mat_4x4_d->at(2, 2) = 5.;   _mat_4x4_d->at(2, 3) = 45.;
    _mat_4x4_d->at(3, 0) = 12.374; _mat_4x4_d->at(3, 1) = 41.6;	_mat_4x4_d->at(3, 2) = 1.3;  _mat_4x4_d->at(3, 3) = 12.7;

    mat_zero = Matrix<3, 3>::zero();
    mat_ones = Matrix<3, 3>::ones();
    mat_identity = Matrix<3, 3>::identity();

    _mat_5x5_d.at(0, 0) = 6;
    _mat_5x5_d.at(0, 1) = 8;
    _mat_5x5_d.at(0, 2) = 6;
    _mat_5x5_d.at(0, 3) = 7;
    _mat_5x5_d.at(0, 4) = 3;
    _mat_5x5_d.at(1, 0) = 9;
    _mat_5x5_d.at(1, 1) = 6;
    _mat_5x5_d.at(1, 2) = 2;
    _mat_5x5_d.at(1, 3) = 3;
    _mat_5x5_d.at(1, 4) = 3;
    _mat_5x5_d.at(2, 0) = 8;
    _mat_5x5_d.at(2, 1) = 3;
    _mat_5x5_d.at(2, 2) = 2;
    _mat_5x5_d.at(2, 3) = 3;
    _mat_5x5_d.at(2, 4) = 3;
    _mat_5x5_d.at(3, 0) = 5;
    _mat_5x5_d.at(3, 1) = 3;
    _mat_5x5_d.at(3, 2) = 3;
    _mat_5x5_d.at(3, 3) = 7;
    _mat_5x5_d.at(3, 4) = 6;
    _mat_5x5_d.at(4, 0) = 5;
    _mat_5x5_d.at(4, 1) = 5;
    _mat_5x5_d.at(4, 2) = 7;
    _mat_5x5_d.at(4, 3) = 4;
    _mat_5x5_d.at(4, 4) = 7;


    _mat_2x3_i.at(0, 0) = 6;
    _mat_2x3_i.at(0, 1) = 8;
    _mat_2x3_i.at(0, 2) = 6;
    _mat_2x3_i.at(1, 0) = 9;
    _mat_2x3_i.at(1, 1) = 6;
    _mat_2x3_i.at(1, 2) = 2;

    
    _cofactor_matrix.at(0, 0) = 1;
    _cofactor_matrix.at(0, 1) = 2;
    _cofactor_matrix.at(0, 2) = 3;
    _cofactor_matrix.at(1, 0) = 0;
    _cofactor_matrix.at(1, 1) = 4;
    _cofactor_matrix.at(1, 2) = 5;
    _cofactor_matrix.at(2, 0) = 1;
    _cofactor_matrix.at(2, 1) = 0;
    _cofactor_matrix.at(2, 2) = 6;
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
  Matrix<5, 5> _mat_5x5_d;
  Matrix<2, 3, int> _mat_2x3_i;
  Matrix3x3i _cofactor_matrix;
  Matrix<3, 3> mat_zero;
  Matrix<3, 3> mat_ones;
  Matrix<3, 3> mat_identity;

};


TEST_F(MatrixTest, DefaultConstructor)
{
  double ini_value = -std::numeric_limits<double>().max();

  for (size_t r = 0; r < _mat_2x2_d.rows(); r++){
    for (size_t c = 0; c < _mat_2x2_d.cols(); c++){
      EXPECT_EQ(ini_value, _mat_2x2_d.at(r, c));
    }
  }

}

TEST_F(MatrixTest, at)
{
  EXPECT_EQ(1.5, _mat_3x3_d.at(0, 0));
  EXPECT_EQ(0.0, _mat_3x3_d.at(0, 1));
  EXPECT_EQ(2.5, _mat_3x3_d.at(0, 2));
  EXPECT_EQ(1.0, _mat_3x3_d.at(1, 0));
  EXPECT_EQ(1.0, _mat_3x3_d.at(1, 1));
  EXPECT_EQ(1.2, _mat_3x3_d.at(1, 2));
  EXPECT_EQ(1.3, _mat_3x3_d.at(2, 0));
  EXPECT_EQ(2.6, _mat_3x3_d.at(2, 1));
  EXPECT_EQ(0.3, _mat_3x3_d.at(2, 2));
}

TEST_F(MatrixTest, rows)
{
  EXPECT_EQ(2, _mat_2x2_d.rows());
  EXPECT_EQ(3, _mat_3x3_d.rows());
  EXPECT_EQ(3, _mat_3x3_f.rows());
}

TEST_F(MatrixTest, cols)
{
  EXPECT_EQ(2, _mat_2x2_d.cols());
  EXPECT_EQ(3, _mat_3x3_d.cols());
  EXPECT_EQ(3, _mat_3x3_f.cols());
}

TEST_F(MatrixTest, inverse2x2)
{
  Matrix<2, 2> _mat_2x2;
  _mat_2x2.at(0, 0) = 2;
  _mat_2x2.at(0, 1) = 3;
  _mat_2x2.at(1, 0) = 1;
  _mat_2x2.at(1, 1) = 4;
  bool invertible;
  Matrix<2, 2> inv_mat = _mat_2x2.inverse(&invertible);
  EXPECT_TRUE(invertible);
  EXPECT_NEAR(0.8, inv_mat.at(0, 0), 0.001);
  EXPECT_NEAR(-0.6, inv_mat.at(0, 1), 0.001);
  EXPECT_NEAR(-0.2, inv_mat.at(1, 0), 0.001);
  EXPECT_NEAR(0.4, inv_mat.at(1, 1), 0.001);
}

TEST_F(MatrixTest, inverse3x3)
{
  bool invertible;
  Matrix<3, 3> inv_mat = _mat_3x3_d.inverse(&invertible);
  EXPECT_TRUE(invertible);
  EXPECT_NEAR( 2.877551, inv_mat.at(0, 0), 0.00001);
  EXPECT_NEAR(-6.632653, inv_mat.at(0, 1), 0.00001);
  EXPECT_NEAR( 2.551020, inv_mat.at(0, 2), 0.00001);
  EXPECT_NEAR(-1.285714, inv_mat.at(1, 0), 0.00001);
  EXPECT_NEAR( 2.857143, inv_mat.at(1, 1), 0.00001);
  EXPECT_NEAR(-0.714286, inv_mat.at(1, 2), 0.00001);
  EXPECT_NEAR(-1.326531, inv_mat.at(2, 0), 0.00001);
  EXPECT_NEAR( 3.979592, inv_mat.at(2, 1), 0.00001);
  EXPECT_NEAR(-1.530612, inv_mat.at(2, 2), 0.00001);
}

TEST_F(MatrixTest, inverse4x4)
{
  bool invertible;
  Matrix<4, 4> inv_mat = _mat_4x4_d->inverse(&invertible);
  EXPECT_TRUE(invertible);
  EXPECT_NEAR( 0.268435, inv_mat.at(0, 0), 0.01);
  EXPECT_NEAR(-0.018133, inv_mat.at(0, 1), 0.01);
  EXPECT_NEAR(-0.010673, inv_mat.at(0, 2), 0.01);
  EXPECT_NEAR(-0.002479, inv_mat.at(0, 3), 0.01);
  EXPECT_NEAR(-0.077647, inv_mat.at(1, 0), 0.01);
  EXPECT_NEAR( 0.005298, inv_mat.at(1, 1), 0.01);
  EXPECT_NEAR(-0.006780, inv_mat.at(1, 2), 0.01);
  EXPECT_NEAR( 0.035491, inv_mat.at(1, 3), 0.01);
  EXPECT_NEAR( 0.007528, inv_mat.at(2, 0), 0.01);
  EXPECT_NEAR( 0.013172, inv_mat.at(2, 1), 0.01);
  EXPECT_NEAR(-0.019248, inv_mat.at(2, 2), 0.01);
  EXPECT_NEAR( 0.018059, inv_mat.at(2, 3), 0.01);
  EXPECT_NEAR(-0.007974, inv_mat.at(3, 0), 0.01);
  EXPECT_NEAR(-0.001035, inv_mat.at(3, 1), 0.01);
  EXPECT_NEAR( 0.034578, inv_mat.at(3, 2), 0.01);	
  EXPECT_NEAR(-0.036946, inv_mat.at(3, 3), 0.01);
}

TEST_F(MatrixTest, inverse)
{

}

TEST_F(MatrixTest, transposeSquared)
{
  Matrix<3, 3> transp_mat = _mat_3x3_d.transpose();

  EXPECT_EQ(1.5, transp_mat.at(0, 0));
  EXPECT_EQ(1, transp_mat.at(0, 1));
  EXPECT_EQ(1.3, transp_mat.at(0, 2));
  EXPECT_EQ(0, transp_mat.at(1, 0));
  EXPECT_EQ(1, transp_mat.at(1, 1));
  EXPECT_EQ(2.6, transp_mat.at(1, 2));
  EXPECT_EQ(2.5, transp_mat.at(2, 0));
  EXPECT_EQ(1.2, transp_mat.at(2, 1));
  EXPECT_EQ(0.3, transp_mat.at(2, 2));
}


/// Transpuesta de matriz no cuadrada
TEST_F(MatrixTest, transpose)
{
  Matrix<3, 2, int> transp_mat = _mat_2x3_i.transpose();

  _mat_2x3_i.at(0, 0) = 6;
  _mat_2x3_i.at(0, 1) = 8;
  _mat_2x3_i.at(0, 2) = 6;
  _mat_2x3_i.at(1, 0) = 9;
  _mat_2x3_i.at(1, 1) = 6;
  _mat_2x3_i.at(1, 2) = 2;

  EXPECT_EQ(6, transp_mat.at(0, 0));
  EXPECT_EQ(9, transp_mat.at(0, 1));
  EXPECT_EQ(8, transp_mat.at(1, 0));
  EXPECT_EQ(6, transp_mat.at(1, 1));
  EXPECT_EQ(6, transp_mat.at(2, 0));
  EXPECT_EQ(2, transp_mat.at(2, 1));
}

TEST_F(MatrixTest, determinant2x2)
{
  Matrix<2, 2> _mat_2x2;
  _mat_2x2.at(0, 0) = 2;
  _mat_2x2.at(0, 1) = 3;
  _mat_2x2.at(1, 0) = 1;
  _mat_2x2.at(1, 1) = 4;
  EXPECT_EQ(5, _mat_2x2.determinant());
}

TEST_F(MatrixTest, determinant3x3)
{
  EXPECT_NEAR(-0.98, _mat_3x3_d.determinant(),0.01);
}

TEST_F(MatrixTest, determinant4x4)
{
  EXPECT_NEAR(353100.53, _mat_4x4_d->determinant(), 0.01);
}

TEST_F(MatrixTest, determinantnxn)
{
  EXPECT_NEAR(-2877.99, _mat_5x5_d.determinant(), 0.01);
}

/// http://www.mathwords.com/c/cofactor_matrix.htm
TEST_F(MatrixTest, cofactorMatrix)
{
  Matrix3x3i cofactor_matrix = _cofactor_matrix.cofactorMatrix();

  EXPECT_EQ( 24, cofactor_matrix.at(0, 0));
  EXPECT_EQ(  5, cofactor_matrix.at(0, 1));
  EXPECT_EQ( -4, cofactor_matrix.at(0, 2));
  EXPECT_EQ(-12, cofactor_matrix.at(1, 0));
  EXPECT_EQ(  3, cofactor_matrix.at(1, 1));
  EXPECT_EQ(  2, cofactor_matrix.at(1, 2));
  EXPECT_EQ( -2, cofactor_matrix.at(2, 0));
  EXPECT_EQ( -5, cofactor_matrix.at(2, 1));
  EXPECT_EQ(  4, cofactor_matrix.at(2, 2));
}

TEST_F(MatrixTest, firstMinor)
{
  int first_minor = _cofactor_matrix.firstMinor(1, 1);

  EXPECT_EQ(3, first_minor);

  first_minor = _cofactor_matrix.firstMinor(0, 1);

  EXPECT_EQ(-5, first_minor);
}

TEST_F(MatrixTest, cofactor)
{
  int cofactor = _cofactor_matrix.cofactor(1,1);

  EXPECT_EQ(3, cofactor);

  cofactor = _cofactor_matrix.cofactor(0, 1);

  EXPECT_EQ(5, cofactor);
}

TEST_F(MatrixTest, zero)
{
  for (size_t r = 0; r < mat_identity.rows(); r++){
    for (size_t c = 0; c < mat_identity.cols(); c++){
      EXPECT_EQ(0, mat_zero.at(r, c));
    }
  }
}

TEST_F(MatrixTest, ones)
{
  for (size_t r = 0; r < mat_identity.rows(); r++){
    for (size_t c = 0; c < mat_identity.cols(); c++){
      EXPECT_EQ(1, mat_ones.at(r, c));
    }
  }
}

TEST_F(MatrixTest, identity)
{
  for (size_t r = 0; r < mat_identity.rows(); r++){
    for (size_t c = 0; c < mat_identity.cols(); c++){
      if (r == c) {
        EXPECT_EQ(1, mat_identity.at(r, c));
      } else {
        EXPECT_EQ(0, mat_identity.at(r, c));
      }
    }
  }
}

