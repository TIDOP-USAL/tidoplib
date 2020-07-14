#define BOOST_TEST_MODULE Tidop matrix test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/matrix.h>

using namespace tl::math;

BOOST_AUTO_TEST_SUITE(MatrixTestSuite)

struct MatrixTest 
{
  MatrixTest() 
    : _mat_4x4_d(new Matrix<4, 4>())
  {}
  ~MatrixTest()
  { 
    if (_mat_4x4_d) delete _mat_4x4_d, _mat_4x4_d = nullptr;
  }
  
  void setup()
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
 
  void teardown()
  {

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


BOOST_FIXTURE_TEST_CASE(default_constructor, MatrixTest)
{
  double ini_value = -std::numeric_limits<double>().max();

  for (size_t r = 0; r < _mat_2x2_d.rows; r++){
    for (size_t c = 0; c < _mat_2x2_d.cols; c++){
      BOOST_CHECK_EQUAL(ini_value, _mat_2x2_d.at(r, c));
    }
  }

}

BOOST_FIXTURE_TEST_CASE(iterator_list_constructor, MatrixTest)
{

  Matrix<2, 2, int> it_list2 {{
    {
      {0, 1},
      {2, 3}
    }
  }};

  BOOST_CHECK_EQUAL(0, it_list2.at(0, 0));
  BOOST_CHECK_EQUAL(1, it_list2.at(0, 1));
  BOOST_CHECK_EQUAL(2, it_list2.at(1, 0));
  BOOST_CHECK_EQUAL(3, it_list2.at(1, 1));

  Matrix<2, 2, int> it_list3 = {
    {
      { 0, 1 },
      {2, 3}
    }
  };

  BOOST_CHECK_EQUAL(0, it_list3.at(0, 0));
  BOOST_CHECK_EQUAL(1, it_list3.at(0, 1));
  BOOST_CHECK_EQUAL(2, it_list3.at(1, 0));
  BOOST_CHECK_EQUAL(3, it_list3.at(1, 1));

  Matrix<2, 3, int> it_list2x3 = {
    {
      {0, 1, 2},
      {3, 4, 5}
    }
  };

  BOOST_CHECK_EQUAL(0, it_list2x3.at(0, 0));
  BOOST_CHECK_EQUAL(1, it_list2x3.at(0, 1));
  BOOST_CHECK_EQUAL(2, it_list2x3.at(0, 2));
  BOOST_CHECK_EQUAL(3, it_list2x3.at(1, 0));
  BOOST_CHECK_EQUAL(4, it_list2x3.at(1, 1));
  BOOST_CHECK_EQUAL(5, it_list2x3.at(1, 2));

  //Matrix<3, 3, int> it_list3x3 = {
  //  {
  //    {0, 1, 2},
  //    {3, 4, 5},
  //    {6, 7, 8}
  //  }
  //};

  //BOOST_CHECK_EQUAL(0, it_list3x3.at(0, 0));
  //BOOST_CHECK_EQUAL(1, it_list3x3.at(0, 1));
  //BOOST_CHECK_EQUAL(2, it_list3x3.at(0, 2));

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

BOOST_FIXTURE_TEST_CASE(at, MatrixTest)
{
  BOOST_CHECK_EQUAL(1.5, _mat_3x3_d.at(0, 0));
  BOOST_CHECK_EQUAL(0.0, _mat_3x3_d.at(0, 1));
  BOOST_CHECK_EQUAL(2.5, _mat_3x3_d.at(0, 2));
  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d.at(1, 0));
  BOOST_CHECK_EQUAL(1.0, _mat_3x3_d.at(1, 1));
  BOOST_CHECK_EQUAL(1.2, _mat_3x3_d.at(1, 2));
  BOOST_CHECK_EQUAL(1.3, _mat_3x3_d.at(2, 0));
  BOOST_CHECK_EQUAL(2.6, _mat_3x3_d.at(2, 1));
  BOOST_CHECK_EQUAL(0.3, _mat_3x3_d.at(2, 2));
}

BOOST_FIXTURE_TEST_CASE(rows, MatrixTest)
{
  BOOST_CHECK_EQUAL(2, _mat_2x2_d.rows);
  BOOST_CHECK_EQUAL(3, _mat_3x3_d.rows);
  BOOST_CHECK_EQUAL(3, _mat_3x3_f.rows);
}

BOOST_FIXTURE_TEST_CASE(cols, MatrixTest)
{
  BOOST_CHECK_EQUAL(2, _mat_2x2_d.cols);
  BOOST_CHECK_EQUAL(3, _mat_3x3_d.cols);
  BOOST_CHECK_EQUAL(3, _mat_3x3_f.cols);
}

BOOST_FIXTURE_TEST_CASE(inverse2x2, MatrixTest)
{
  Matrix<2, 2> _mat_2x2;
  _mat_2x2.at(0, 0) = 2;
  _mat_2x2.at(0, 1) = 3;
  _mat_2x2.at(1, 0) = 1;
  _mat_2x2.at(1, 1) = 4;
  bool invertible;
  Matrix<2, 2> inv_mat = _mat_2x2.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE(0.8, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-0.6, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.2, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.4, inv_mat.at(1, 1), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse3x3, MatrixTest)
{
  bool invertible;
  Matrix<3, 3> inv_mat = _mat_3x3_d.inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_CLOSE( 2.877551, inv_mat.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(-6.632653, inv_mat.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE( 2.551020, inv_mat.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.285714, inv_mat.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE( 2.857143, inv_mat.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(-0.714286, inv_mat.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(-1.326531, inv_mat.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE( 3.979592, inv_mat.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(-1.530612, inv_mat.at(2, 2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse4x4, MatrixTest)
{
  bool invertible;
  Matrix<4, 4> inv_mat = _mat_4x4_d->inverse(&invertible);
  BOOST_CHECK(invertible);
  BOOST_CHECK_SMALL( 0.268435, inv_mat.at(0, 0));
  BOOST_CHECK_SMALL(-0.018133, inv_mat.at(0, 1));
  BOOST_CHECK_SMALL(-0.010673, inv_mat.at(0, 2));
  BOOST_CHECK_SMALL(-0.002479, inv_mat.at(0, 3));
  BOOST_CHECK_SMALL(-0.077647, inv_mat.at(1, 0));
  BOOST_CHECK_SMALL( 0.005298, inv_mat.at(1, 1));
  BOOST_CHECK_SMALL(-0.006780, inv_mat.at(1, 2));
  BOOST_CHECK_SMALL( 0.035491, inv_mat.at(1, 3));
  BOOST_CHECK_SMALL( 0.007528, inv_mat.at(2, 0));
  BOOST_CHECK_SMALL( 0.013172, inv_mat.at(2, 1));
  BOOST_CHECK_SMALL(-0.019248, inv_mat.at(2, 2));
  BOOST_CHECK_SMALL( 0.018059, inv_mat.at(2, 3));
  BOOST_CHECK_SMALL(-0.007974, inv_mat.at(3, 0));
  BOOST_CHECK_SMALL(-0.001035, inv_mat.at(3, 1));
  BOOST_CHECK_SMALL( 0.034578, inv_mat.at(3, 2));
  BOOST_CHECK_SMALL(-0.036946, inv_mat.at(3, 3));
}

BOOST_FIXTURE_TEST_CASE(inverse, MatrixTest)
{
  ///TODO: Terminar método inverse
}

BOOST_FIXTURE_TEST_CASE(transpose_squared, MatrixTest)
{
  Matrix<3, 3> transp_mat = _mat_3x3_d.transpose();

  BOOST_CHECK_EQUAL(1.5, transp_mat.at(0, 0));
  BOOST_CHECK_EQUAL(1, transp_mat.at(0, 1));
  BOOST_CHECK_EQUAL(1.3, transp_mat.at(0, 2));
  BOOST_CHECK_EQUAL(0, transp_mat.at(1, 0));
  BOOST_CHECK_EQUAL(1, transp_mat.at(1, 1));
  BOOST_CHECK_EQUAL(2.6, transp_mat.at(1, 2));
  BOOST_CHECK_EQUAL(2.5, transp_mat.at(2, 0));
  BOOST_CHECK_EQUAL(1.2, transp_mat.at(2, 1));
  BOOST_CHECK_EQUAL(0.3, transp_mat.at(2, 2));
}


/// Transpuesta de matriz no cuadrada
BOOST_FIXTURE_TEST_CASE(transpose, MatrixTest)
{
  Matrix<3, 2, int> transp_mat = _mat_2x3_i.transpose();

  _mat_2x3_i.at(0, 0) = 6;
  _mat_2x3_i.at(0, 1) = 8;
  _mat_2x3_i.at(0, 2) = 6;
  _mat_2x3_i.at(1, 0) = 9;
  _mat_2x3_i.at(1, 1) = 6;
  _mat_2x3_i.at(1, 2) = 2;

  BOOST_CHECK_EQUAL(6, transp_mat.at(0, 0));
  BOOST_CHECK_EQUAL(9, transp_mat.at(0, 1));
  BOOST_CHECK_EQUAL(8, transp_mat.at(1, 0));
  BOOST_CHECK_EQUAL(6, transp_mat.at(1, 1));
  BOOST_CHECK_EQUAL(6, transp_mat.at(2, 0));
  BOOST_CHECK_EQUAL(2, transp_mat.at(2, 1));
}

BOOST_FIXTURE_TEST_CASE(determinant2x2, MatrixTest)
{
  Matrix<2, 2> _mat_2x2;
  _mat_2x2.at(0, 0) = 2;
  _mat_2x2.at(0, 1) = 3;
  _mat_2x2.at(1, 0) = 1;
  _mat_2x2.at(1, 1) = 4;
  BOOST_CHECK_EQUAL(5, _mat_2x2.determinant());
}

BOOST_FIXTURE_TEST_CASE(determinant3x3, MatrixTest)
{
  BOOST_CHECK_CLOSE(-0.98, _mat_3x3_d.determinant(),0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant4x4, MatrixTest)
{
  BOOST_CHECK_CLOSE(353100.53, _mat_4x4_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinantnxn, MatrixTest)
{
  BOOST_CHECK_CLOSE(-2877.99, _mat_5x5_d.determinant(), 0.1);
}

/// http://www.mathwords.com/c/cofactor_matrix.htm
BOOST_FIXTURE_TEST_CASE(cofactor_matrix, MatrixTest)
{
  Matrix3x3i cofactor_matrix = _cofactor_matrix.cofactorMatrix();

  BOOST_CHECK_EQUAL( 24, cofactor_matrix.at(0, 0));
  BOOST_CHECK_EQUAL(  5, cofactor_matrix.at(0, 1));
  BOOST_CHECK_EQUAL( -4, cofactor_matrix.at(0, 2));
  BOOST_CHECK_EQUAL(-12, cofactor_matrix.at(1, 0));
  BOOST_CHECK_EQUAL(  3, cofactor_matrix.at(1, 1));
  BOOST_CHECK_EQUAL(  2, cofactor_matrix.at(1, 2));
  BOOST_CHECK_EQUAL( -2, cofactor_matrix.at(2, 0));
  BOOST_CHECK_EQUAL( -5, cofactor_matrix.at(2, 1));
  BOOST_CHECK_EQUAL(  4, cofactor_matrix.at(2, 2));
}

BOOST_FIXTURE_TEST_CASE(first_minor, MatrixTest)
{
  int first_minor = _cofactor_matrix.firstMinor(1, 1);

  BOOST_CHECK_EQUAL(3, first_minor);

  first_minor = _cofactor_matrix.firstMinor(0, 1);

  BOOST_CHECK_EQUAL(-5, first_minor);
}

BOOST_FIXTURE_TEST_CASE(cofactor, MatrixTest)
{
  int cofactor = _cofactor_matrix.cofactor(1,1);

  BOOST_CHECK_EQUAL(3, cofactor);

  cofactor = _cofactor_matrix.cofactor(0, 1);

  BOOST_CHECK_EQUAL(5, cofactor);
}

BOOST_FIXTURE_TEST_CASE(zero, MatrixTest)
{
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(0, mat_zero.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(ones, MatrixTest)
{
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(1, mat_ones.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(identity, MatrixTest)
{
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      if (r == c) {
        BOOST_CHECK_EQUAL(1, mat_identity.at(r, c));
      } else {
        BOOST_CHECK_EQUAL(0, mat_identity.at(r, c));
      }
    }
  }
}

/* Operaciones unarias */

BOOST_FIXTURE_TEST_CASE(plus, MatrixTest)
{
  Matrix3x3d mat = +_mat_3x3_d;
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(mat.at(r,c), _mat_3x3_d.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(minus, MatrixTest)
{
  Matrix3x3d mat = -_mat_3x3_d;
  for (size_t r = 0; r < mat_identity.rows; r++){
    for (size_t c = 0; c < mat_identity.cols; c++){
      BOOST_CHECK_EQUAL(-mat.at(r, c), _mat_3x3_d.at(r, c));
    }
  }
}

/* Operaciones binarias entre matrices */

/// Suma o adición de matrices

BOOST_FIXTURE_TEST_CASE(addition, MatrixTest)
{
  Matrix3x3d mat = mat_ones + _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat.at(2, 2));

  mat_ones += _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat_ones.at(0, 0));
  BOOST_CHECK_EQUAL(1.0, mat_ones.at(0, 1));
  BOOST_CHECK_EQUAL(3.5, mat_ones.at(0, 2));
  BOOST_CHECK_EQUAL(2.0, mat_ones.at(1, 0));
  BOOST_CHECK_EQUAL(2.0, mat_ones.at(1, 1));
  BOOST_CHECK_EQUAL(2.2, mat_ones.at(1, 2));
  BOOST_CHECK_EQUAL(2.3, mat_ones.at(2, 0));
  BOOST_CHECK_EQUAL(3.6, mat_ones.at(2, 1));
  BOOST_CHECK_EQUAL(1.3, mat_ones.at(2, 2));
}

/// Resta de matrices

BOOST_FIXTURE_TEST_CASE(subtraction, MatrixTest)
{
  Matrix3x3d mat = mat_identity - _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat.at(2, 2));

  mat_identity -= _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat_identity.at(0, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_identity.at(0, 1));
  BOOST_CHECK_EQUAL(-2.5, mat_identity.at(0, 2));
  BOOST_CHECK_EQUAL(-1.0, mat_identity.at(1, 0));
  BOOST_CHECK_EQUAL( 0.0, mat_identity.at(1, 1));
  BOOST_CHECK_EQUAL(-1.2, mat_identity.at(1, 2));
  BOOST_CHECK_EQUAL(-1.3, mat_identity.at(2, 0));
  BOOST_CHECK_EQUAL(-2.6, mat_identity.at(2, 1));
  BOOST_CHECK_EQUAL( 0.7, mat_identity.at(2, 2));
}

/// Multiplicación de matrices

BOOST_FIXTURE_TEST_CASE(multiplication, MatrixTest)
{
  Matrix<2,3,int> mat = _mat_2x3_i * _cofactor_matrix;

  BOOST_CHECK_EQUAL(12, mat.at(0, 0));
  BOOST_CHECK_EQUAL(44, mat.at(0, 1));
  BOOST_CHECK_EQUAL(94, mat.at(0, 2));
  BOOST_CHECK_EQUAL(11, mat.at(1, 0));
  BOOST_CHECK_EQUAL(42, mat.at(1, 1));
  BOOST_CHECK_EQUAL(69, mat.at(1, 2));

}

/// Multiplicación de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(MatrixScalar, MatrixTest)
{
  Matrix<2,3,int> mat = _mat_2x3_i * 10;

  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat.at(1, 2));

  _mat_2x3_i *= 10;

  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(0, 0));
  BOOST_CHECK_EQUAL(80, _mat_2x3_i.at(0, 1));
  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(0, 2));
  BOOST_CHECK_EQUAL(90, _mat_2x3_i.at(1, 0));
  BOOST_CHECK_EQUAL(60, _mat_2x3_i.at(1, 1));
  BOOST_CHECK_EQUAL(20, _mat_2x3_i.at(1, 2));
}

/// Multiplicación de un escalar por una matriz

BOOST_FIXTURE_TEST_CASE(scalar_matrix, MatrixTest)
{
  Matrix<2,3,int> mat = 10 * _mat_2x3_i;

  BOOST_CHECK_EQUAL(60, mat.at(0, 0));
  BOOST_CHECK_EQUAL(80, mat.at(0, 1));
  BOOST_CHECK_EQUAL(60, mat.at(0, 2));
  BOOST_CHECK_EQUAL(90, mat.at(1, 0));
  BOOST_CHECK_EQUAL(60, mat.at(1, 1));
  BOOST_CHECK_EQUAL(20, mat.at(1, 2));
}

// División de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(div_matrix_scalar, MatrixTest)
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

  BOOST_CHECK_EQUAL(.15, mat.at(0, 0));
  BOOST_CHECK_EQUAL(.0, mat.at(0, 1));
  BOOST_CHECK_EQUAL(.25, mat.at(0, 2));
  BOOST_CHECK_EQUAL(.10, mat.at(1, 0));
  BOOST_CHECK_EQUAL(.10, mat.at(1, 1));
  BOOST_CHECK_EQUAL(.12, mat.at(1, 2));
  BOOST_CHECK_EQUAL(.13, mat.at(2, 0));
  BOOST_CHECK_EQUAL(.26, mat.at(2, 1));
  BOOST_CHECK_EQUAL(.03, mat.at(2, 2));
}

BOOST_AUTO_TEST_SUITE_END()