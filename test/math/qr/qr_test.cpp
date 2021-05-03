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
 
#define BOOST_TEST_MODULE Tidop qr test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/qr.h>

using namespace tl::math;


BOOST_AUTO_TEST_SUITE(QrTestSuite)

struct QrTest
{

  QrTest()
  {}
  ~QrTest()
  {
  }

  void setup()
  {
    A = Matrix<double>(3,3);
    A.at(0,0) = 12.;
    A.at(0,1) = -51.;
    A.at(0,2) = 4.;
    A.at(1,0) = 6.;
    A.at(1,1) = 167.;
    A.at(1,2) = -68.;
    A.at(2,0) = -4.;
    A.at(2,1) = 24.;
    A.at(2,2) = -41.;

    B = Vector<double>(3);
    B.at(0) = 1;
    B.at(1) = 3;
    B.at(2) = 2;
  }

  void teardown()
  {
    
  }

  Matrix<double> A;
  Vector<double> B;

};


BOOST_FIXTURE_TEST_CASE(default_constructor, QrTest)
{
 QRDecomposition<Matrix<double>> qr(A);
 Matrix<double> q = qr.q();
 Matrix<double> r = qr.r();

 BOOST_CHECK_CLOSE(-14, r.at(0, 0), 0.1);
 BOOST_CHECK_CLOSE(-21, r.at(0, 1), 0.1);
 BOOST_CHECK_CLOSE(14, r.at(0, 2), 0.1);
 BOOST_CHECK_CLOSE(0, r.at(1, 0), 0.1);
 BOOST_CHECK_CLOSE(-175, r.at(1, 1), 0.1);
 BOOST_CHECK_CLOSE(70, r.at(1, 2), 0.1);
 BOOST_CHECK_CLOSE(0, r.at(2, 0), 0.1);
 BOOST_CHECK_CLOSE(0, r.at(2, 1), 0.1);
 BOOST_CHECK_CLOSE(-35, r.at(2, 2), 0.1);

}

BOOST_AUTO_TEST_SUITE_END()
