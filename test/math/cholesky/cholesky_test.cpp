/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop cholesky test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/cholesky.h>

using namespace tl::math;


BOOST_AUTO_TEST_SUITE(CholeskyTestSuite)

struct CholeskyTest
{

  CholeskyTest()
    : cholesky(nullptr),
      cholesky2(nullptr)
  {}
  ~CholeskyTest()
  {
    delete cholesky;
    delete cholesky2;
  }

  void setup()
  {
    A = Matrix<double>(3,3);
    A[0][0] = 6;
    A[0][1] = 15;
    A[0][2] = 55;
    A[1][0] = 15;
    A[1][1] = 55;
    A[1][2] = 225;
    A[2][0] = 55;
    A[2][1] = 225;
    A[2][2] = 979;

    B = Vector<double>(3);
    B[0] = 76;
    B[1] = 295;
    B[2] = 1259;

    cholesky = new CholeskyDecomposition<Matrix<double>>(A);

    A2 = Matrix<double, 3, 3>();
    A2[0][0] = 6;
    A2[0][1] = 15;
    A2[0][2] = 55;
    A2[1][0] = 15;
    A2[1][1] = 55;
    A2[1][2] = 225;
    A2[2][0] = 55;
    A2[2][1] = 225;
    A2[2][2] = 979;

    B2 = Vector<double, 3>();
    B2[0] = 76;
    B2[1] = 295;
    B2[2] = 1259;

    cholesky2 = new CholeskyDecomposition<Matrix<double, 3, 3>>(A2);
  }

  void teardown()
  {
    
  }

  Matrix<double> A;
  Vector<double> B;
  CholeskyDecomposition<Matrix<double>> *cholesky;

  Matrix<double, 3, 3> A2;
  Vector<double, 3> B2;
  CholeskyDecomposition<Matrix<double, 3, 3>> *cholesky2;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, CholeskyTest)
{
  
  Matrix<double> l = cholesky->l();
  BOOST_CHECK_CLOSE(2.44949, l[0][0], 0.1);
  BOOST_CHECK_CLOSE(0., l[0][1], 0.1);
  BOOST_CHECK_CLOSE(0., l[0][2], 0.1);
  BOOST_CHECK_CLOSE(6.12372, l[1][0], 0.1);
  BOOST_CHECK_CLOSE(4.1833, l[1][1], 0.1);
  BOOST_CHECK_CLOSE(0., l[1][2], 0.1);
  BOOST_CHECK_CLOSE(22.4537, l[2][0], 0.1);
  BOOST_CHECK_CLOSE(20.9165, l[2][1], 0.1);
  BOOST_CHECK_CLOSE(6.1101, l[2][2], 0.1);

  Matrix<double, 3, 3> l2 = cholesky2->l();
  BOOST_CHECK_CLOSE(2.44949, l2[0][0], 0.1);
  BOOST_CHECK_CLOSE(0., l2[0][1], 0.1);
  BOOST_CHECK_CLOSE(0., l2[0][2], 0.1);
  BOOST_CHECK_CLOSE(6.12372, l2[1][0], 0.1);
  BOOST_CHECK_CLOSE(4.1833, l2[1][1], 0.1);
  BOOST_CHECK_CLOSE(0., l2[1][2], 0.1);
  BOOST_CHECK_CLOSE(22.4537, l2[2][0], 0.1);
  BOOST_CHECK_CLOSE(20.9165, l2[2][1], 0.1);
  BOOST_CHECK_CLOSE(6.1101, l2[2][2], 0.1);

}

BOOST_FIXTURE_TEST_CASE(solve, CholeskyTest)
{
  auto C = cholesky->solve(B);
  BOOST_CHECK_CLOSE(1., C[0], 0.1);
  BOOST_CHECK_CLOSE(1., C[1], 0.1);
  BOOST_CHECK_CLOSE(1., C[2], 0.1);

  auto C2 = cholesky2->solve(B2);
  BOOST_CHECK_CLOSE(1., C2[0], 0.1);
  BOOST_CHECK_CLOSE(1., C2[1], 0.1);
  BOOST_CHECK_CLOSE(1., C2[2], 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
