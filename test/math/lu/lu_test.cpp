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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop lu test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/lu.h>

using namespace tl::math;


BOOST_AUTO_TEST_SUITE(LuTestSuite)

struct LuTest
{

  LuTest()
    : lu(nullptr),
      lu2(nullptr)
  {}
  ~LuTest()
  {
    delete lu;
    delete lu2;
  }

  void setup()
  {
    A = Matrix<double>(3,3);
    A[0][0] = 12.;
    A[0][1] = -51.;
    A[0][2] = 4.;
    A[1][0] = 6.;
    A[1][1] = 167.;
    A[1][2] = -68.;
    A[2][0] = -4.;
    A[2][1] = 24.;
    A[2][2] = -41.;

    B = Vector<double>(3);
    B[0] = 1;
    B[1] = 3;
    B[2] = 2;

    lu = new LuDecomposition<Matrix<double>>(A);

    A2 = Matrix<double>(3,3);
    A2[0][0] = 1.;
    A2[0][1] = 2.;
    A2[0][2] = 3.;
    A2[1][0] = 4.;
    A2[1][1] = 5.;
    A2[1][2] = 6.;
    A2[2][0] = 7.;
    A2[2][1] = 8.;
    A2[2][2] = 10.;

    B2 = Vector<double>(3);
    B2[0] = 3;
    B2[1] = 3;
    B2[2] = 4;

    lu2 = new LuDecomposition<Matrix<double>>(A2);

    SA;
    SA[0][0] = 12.;
    SA[0][1] = -51.;
    SA[0][2] = 4.;
    SA[1][0] = 6.;
    SA[1][1] = 167.;
    SA[1][2] = -68.;
    SA[2][0] = -4.;
    SA[2][1] = 24.;
    SA[2][2] = -41.;

    SB;
    SB[0] = 1;
    SB[1] = 3;
    SB[2] = 2;

    slu = new LuDecomposition<Matrix<double, 3, 3>>(SA);
  }

  void teardown()
  {
    
  }

  Matrix<double> A;
  Vector<double> B;
  LuDecomposition<Matrix<double>> *lu;
  Matrix<double> A2;
  Vector<double> B2;
  LuDecomposition<Matrix<double>> *lu2;

  Matrix<double, 3, 3> SA;
  Vector<double, 3> SB;
  LuDecomposition<Matrix<double, 3, 3>> *slu;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, LuTest)
{
  
  Matrix<double> _lu = lu->lu();
  
  //BOOST_CHECK_CLOSE(-0.85714285714285721, _lu.at(0, 0), 0.1);
  //BOOST_CHECK_CLOSE(0.39428571428571429, _lu.at(0, 1), 0.1);
  //BOOST_CHECK_CLOSE(0.33142857142857141, _lu.at(0, 2), 0.1);
  //BOOST_CHECK_CLOSE(-0.42857142857142855, _lu.at(1, 0), 0.1);
  //BOOST_CHECK_CLOSE(-0.902857142857142915, _lu.at(1, 1), 0.1);
  //BOOST_CHECK_CLOSE(-0.034285714285714280, _lu.at(1, 2), 0.1);
  //BOOST_CHECK_CLOSE(0.28571428571428570, _lu.at(2, 0), 0.1);
  //BOOST_CHECK_CLOSE(-0.17142857142857137, _lu.at(2, 1), 0.1);
  //BOOST_CHECK_CLOSE(0.94285714285714295, _lu.at(2, 2), 0.1);

}

BOOST_FIXTURE_TEST_CASE(solve, LuTest)
{
  auto C = lu->solve(B);
  BOOST_CHECK_CLOSE(0.065306122448979584, C[0], 0.1);
  BOOST_CHECK_CLOSE(-0.0089795918367346957, C[1], 0.1);
  BOOST_CHECK_CLOSE(-0.060408163265306125, C[2], 0.1);

  C = lu2->solve(B2);
  BOOST_CHECK_CLOSE(-2., C[0], 0.1);
  BOOST_CHECK_CLOSE(1., C[1], 0.1);
  BOOST_CHECK_CLOSE(1., C[2], 0.1);

  auto SC = slu->solve(SB);
  BOOST_CHECK_CLOSE(0.065306122448979584, SC[0], 0.1);
  BOOST_CHECK_CLOSE(-0.0089795918367346957, SC[1], 0.1);
  BOOST_CHECK_CLOSE(-0.060408163265306125, SC[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(solve_matrix, LuTest)
{
  {
    Matrix<double> a(2, 2);
    a[0][0] = 2;
    a[0][1] = -1;
    a[1][0] = -1;
    a[1][1] = 3;

    Matrix<double> b(2, 2);
    b[0][0] = 1;
    b[0][1] = 2;
    b[1][0] = 3;
    b[1][1] = 1;

    LuDecomposition<Matrix<double>> decomp(a);

    auto C = decomp.solve(b);
    BOOST_CHECK_CLOSE(1.2, C[0][0], 0.1);
    BOOST_CHECK_CLOSE(1.4, C[0][1], 0.1);
    BOOST_CHECK_CLOSE(1.4, C[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.8, C[1][1], 0.1);

  }


  // Matrices estáticas
  //{
  //  Matrix<double, 2, 2> a{ 2, -1,
  //                         -1,  3 };
  //  Matrix<double, 2, 2> b{ 1, 2,
  //                          3, 1 };

  //  LuDecomposition<Matrix<double, 2, 2>> decomp(a);

  //  auto C = decomp.solve(b);
  //  BOOST_CHECK_CLOSE(1.2, C.at(0, 0), 0.1);
  //  BOOST_CHECK_CLOSE(1.4, C.at(0, 1), 0.1);
  //  BOOST_CHECK_CLOSE(1.4, C.at(1, 0), 0.1);
  //  BOOST_CHECK_CLOSE(0.8, C.at(1, 1), 0.1);

  //}


}

BOOST_AUTO_TEST_SUITE_END()
