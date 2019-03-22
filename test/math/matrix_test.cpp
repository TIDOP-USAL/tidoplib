#include <gtest/gtest.h>

#include <tidop/math/algebra/matrix.h>

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

TEST_F(MatrixTest, iteratorList)
{

  Matrix<2, 2, int> it_list2 {{
    {
      {0, 1},
      {2, 3}
    }
  }};

  EXPECT_EQ(0, it_list2.at(0, 0));
  EXPECT_EQ(1, it_list2.at(0, 1));
  EXPECT_EQ(2, it_list2.at(1, 0));
  EXPECT_EQ(3, it_list2.at(1, 1));

  Matrix<2, 2, int> it_list3 = {
    {
      { 0, 1 },
      {2, 3}
    }
  };

  EXPECT_EQ(0, it_list3.at(0, 0));
  EXPECT_EQ(1, it_list3.at(0, 1));
  EXPECT_EQ(2, it_list3.at(1, 0));
  EXPECT_EQ(3, it_list3.at(1, 1));

  Matrix<2, 3, int> it_list2x3 = {
    {
      {0, 1, 2},
      {3, 4, 5}
    }
  };

  EXPECT_EQ(0, it_list2x3.at(0, 0));
  EXPECT_EQ(1, it_list2x3.at(0, 1));
  EXPECT_EQ(2, it_list2x3.at(0, 2));
  EXPECT_EQ(3, it_list2x3.at(1, 0));
  EXPECT_EQ(4, it_list2x3.at(1, 1));
  EXPECT_EQ(5, it_list2x3.at(1, 2));

  //Matrix<3, 3, int> it_list3x3 = {
  //  {
  //    {0, 1, 2},
  //    {3, 4, 5},
  //    {6, 7, 8}
  //  }
  //};

  //EXPECT_EQ(0, it_list3x3.at(0, 0));
  //EXPECT_EQ(1, it_list3x3.at(0, 1));
  //EXPECT_EQ(2, it_list3x3.at(0, 2));

  //Matrix<5, 5, int> it_list5 = {
  //  {
  //    {6, 8,6,7,3},
  //    {9,6,2,3,3},
  //    {8,3,2,3,3},
  //    {5,3,3,7,6},
  //    {5,5,7,4,7}
  //  }
  //};
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
  ///TODO: Terminar método inverse
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

/* Operaciones unarias */

TEST_F(MatrixTest, plus)
{
  Matrix3x3d mat = +_mat_3x3_d;
  for (size_t r = 0; r < mat_identity.rows(); r++){
    for (size_t c = 0; c < mat_identity.cols(); c++){
      EXPECT_EQ(mat.at(r,c), _mat_3x3_d.at(r, c));
    }
  }
}

TEST_F(MatrixTest, minus)
{
  Matrix3x3d mat = -_mat_3x3_d;
  for (size_t r = 0; r < mat_identity.rows(); r++){
    for (size_t c = 0; c < mat_identity.cols(); c++){
      EXPECT_EQ(-mat.at(r, c), _mat_3x3_d.at(r, c));
    }
  }
}

/* Operaciones binarias entre matrices */

/// Suma o adición de matrices

TEST_F(MatrixTest, addition)
{
  Matrix3x3d mat = mat_ones + _mat_3x3_d;

  EXPECT_EQ(2.5, mat.at(0, 0));
  EXPECT_EQ(1.0, mat.at(0, 1));
  EXPECT_EQ(3.5, mat.at(0, 2));
  EXPECT_EQ(2.0, mat.at(1, 0));
  EXPECT_EQ(2.0, mat.at(1, 1));
  EXPECT_EQ(2.2, mat.at(1, 2));
  EXPECT_EQ(2.3, mat.at(2, 0));
  EXPECT_EQ(3.6, mat.at(2, 1));
  EXPECT_EQ(1.3, mat.at(2, 2));

  mat_ones += _mat_3x3_d;

  EXPECT_EQ(2.5, mat_ones.at(0, 0));
  EXPECT_EQ(1.0, mat_ones.at(0, 1));
  EXPECT_EQ(3.5, mat_ones.at(0, 2));
  EXPECT_EQ(2.0, mat_ones.at(1, 0));
  EXPECT_EQ(2.0, mat_ones.at(1, 1));
  EXPECT_EQ(2.2, mat_ones.at(1, 2));
  EXPECT_EQ(2.3, mat_ones.at(2, 0));
  EXPECT_EQ(3.6, mat_ones.at(2, 1));
  EXPECT_EQ(1.3, mat_ones.at(2, 2));
}

/// Resta de matrices

TEST_F(MatrixTest, subtraction)
{
  Matrix3x3d mat = mat_identity - _mat_3x3_d;

  EXPECT_EQ(-0.5, mat.at(0, 0));
  EXPECT_EQ( 0.0, mat.at(0, 1));
  EXPECT_EQ(-2.5, mat.at(0, 2));
  EXPECT_EQ(-1.0, mat.at(1, 0));
  EXPECT_EQ( 0.0, mat.at(1, 1));
  EXPECT_EQ(-1.2, mat.at(1, 2));
  EXPECT_EQ(-1.3, mat.at(2, 0));
  EXPECT_EQ(-2.6, mat.at(2, 1));
  EXPECT_EQ( 0.7, mat.at(2, 2));

  mat_identity -= _mat_3x3_d;

  EXPECT_EQ(-0.5, mat_identity.at(0, 0));
  EXPECT_EQ( 0.0, mat_identity.at(0, 1));
  EXPECT_EQ(-2.5, mat_identity.at(0, 2));
  EXPECT_EQ(-1.0, mat_identity.at(1, 0));
  EXPECT_EQ( 0.0, mat_identity.at(1, 1));
  EXPECT_EQ(-1.2, mat_identity.at(1, 2));
  EXPECT_EQ(-1.3, mat_identity.at(2, 0));
  EXPECT_EQ(-2.6, mat_identity.at(2, 1));
  EXPECT_EQ( 0.7, mat_identity.at(2, 2));
}

/// Multiplicación de matrices

TEST_F(MatrixTest, multiplication)
{
  Matrix<2,3,int> mat = _mat_2x3_i * _cofactor_matrix;

  EXPECT_EQ(12, mat.at(0, 0));
  EXPECT_EQ(44, mat.at(0, 1));
  EXPECT_EQ(94, mat.at(0, 2));
  EXPECT_EQ(11, mat.at(1, 0));
  EXPECT_EQ(42, mat.at(1, 1));
  EXPECT_EQ(69, mat.at(1, 2));

}

/// Multiplicación de una matriz por un escalar

TEST_F(MatrixTest, MatrixScalar)
{
  Matrix<2,3,int> mat = _mat_2x3_i * 10;

  EXPECT_EQ(60, mat.at(0, 0));
  EXPECT_EQ(80, mat.at(0, 1));
  EXPECT_EQ(60, mat.at(0, 2));
  EXPECT_EQ(90, mat.at(1, 0));
  EXPECT_EQ(60, mat.at(1, 1));
  EXPECT_EQ(20, mat.at(1, 2));

  _mat_2x3_i *= 10;

  EXPECT_EQ(60, _mat_2x3_i.at(0, 0));
  EXPECT_EQ(80, _mat_2x3_i.at(0, 1));
  EXPECT_EQ(60, _mat_2x3_i.at(0, 2));
  EXPECT_EQ(90, _mat_2x3_i.at(1, 0));
  EXPECT_EQ(60, _mat_2x3_i.at(1, 1));
  EXPECT_EQ(20, _mat_2x3_i.at(1, 2));
}

/// Multiplicación de un escalar por una matriz

TEST_F(MatrixTest, ScalarMatrix)
{
  Matrix<2,3,int> mat = 10 * _mat_2x3_i;

  EXPECT_EQ(60, mat.at(0, 0));
  EXPECT_EQ(80, mat.at(0, 1));
  EXPECT_EQ(60, mat.at(0, 2));
  EXPECT_EQ(90, mat.at(1, 0));
  EXPECT_EQ(60, mat.at(1, 1));
  EXPECT_EQ(20, mat.at(1, 2));
}

// División de una matriz por un escalar

TEST_F(MatrixTest, DivMatrixScalar)
{
  Matrix<3,3,double> mat = _mat_3x3_d / 10.;

  _mat_3x3_d.at(0, 0) = 1.5;
  _mat_3x3_d.at(0, 1) = 0.0;
  _mat_3x3_d.at(0, 2) = 2.5;
  _mat_3x3_d.at(1, 0) = 1.0;
  _mat_3x3_d.at(1, 1) = 1.0;
  _mat_3x3_d.at(1, 2) = 1.2;
  _mat_3x3_d.at(2, 0) = 1.3;
  _mat_3x3_d.at(2, 1) = 2.6;
  _mat_3x3_d.at(2, 2) = 0.3;

  EXPECT_EQ(.15, mat.at(0, 0));
  EXPECT_EQ(.0, mat.at(0, 1));
  EXPECT_EQ(.25, mat.at(0, 2));
  EXPECT_EQ(.10, mat.at(1, 0));
  EXPECT_EQ(.10, mat.at(1, 1));
  EXPECT_EQ(.12, mat.at(1, 2));
  EXPECT_EQ(.13, mat.at(2, 0));
  EXPECT_EQ(.26, mat.at(2, 1));
  EXPECT_EQ(.03, mat.at(2, 2));
}
