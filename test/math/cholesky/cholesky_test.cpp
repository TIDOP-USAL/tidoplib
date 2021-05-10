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
    A.at(0,0) = 4.;
    A.at(0,1) = 12.;
    A.at(0,2) = -16.;
    A.at(1,0) = 12.;
    A.at(1,1) = 37.;
    A.at(1,2) = -43.;
    A.at(2,0) = -16.;
    A.at(2,1) = -43.;
    A.at(2,2) = 98.;

    B = Vector<double>(3);
    B.at(0) = 1;
    B.at(1) = 3;
    B.at(2) = 2;

    cholesky = new CholeskyDecomposition<Matrix<double>>(A);

    A2 = Matrix<double>(3,3);
    A2.at(0,0) = 1.;
    A2.at(0,1) = 2.;
    A2.at(0,2) = 3.;
    A2.at(1,0) = 4.;
    A2.at(1,1) = 5.;
    A2.at(1,2) = 6.;
    A2.at(2,0) = 7.;
    A2.at(2,1) = 8.;
    A2.at(2,2) = 10.;

    B2 = Vector<double>(3);
    B2.at(0) = 3;
    B2.at(1) = 3;
    B2.at(2) = 4;

    cholesky2 = new CholeskyDecomposition<Matrix<double>>(A2);
  }

  void teardown()
  {
    
  }

  Matrix<double> A;
  Vector<double> B;
  CholeskyDecomposition<Matrix<double>> *cholesky;

  Matrix<double> A2;
  Vector<double> B2;
  CholeskyDecomposition<Matrix<double>> *cholesky2;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, CholeskyTest)
{
  
  Matrix<double> l = cholesky->l();
  
  BOOST_CHECK_CLOSE(2., l.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(0., l.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0., l.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(6., l.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(1., l.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(0., l.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(-8., l.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(5., l.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(3., l.at(2, 2), 0.1);

  l = cholesky2->l();
  BOOST_CHECK_CLOSE(1., l.at(0, 0), 0.1);
  BOOST_CHECK_CLOSE(0., l.at(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0., l.at(0, 2), 0.1);
  BOOST_CHECK_CLOSE(2., l.at(1, 0), 0.1);
  BOOST_CHECK_CLOSE(1., l.at(1, 1), 0.1);
  BOOST_CHECK_CLOSE(0., l.at(1, 2), 0.1);
  BOOST_CHECK_CLOSE(3., l.at(2, 0), 0.1);
  BOOST_CHECK_CLOSE(0., l.at(2, 1), 0.1);
  BOOST_CHECK_CLOSE(1., l.at(2, 2), 0.1);

}

BOOST_FIXTURE_TEST_CASE(solve, CholeskyTest)
{
/*   auto C = cholesky->solve(B);
  BOOST_CHECK_CLOSE(0., C.at(0), 0.1);
  BOOST_CHECK_CLOSE(0., C.at(1), 0.1);
  BOOST_CHECK_CLOSE(0., C.at(2), 0.1); */

  //auto C = cholesky2->solve(B2);
  //BOOST_CHECK_CLOSE(-2., C.at(0), 0.1);
  //BOOST_CHECK_CLOSE(1., C.at(1), 0.1);
  //BOOST_CHECK_CLOSE(1., C.at(2), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
