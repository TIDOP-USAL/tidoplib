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
 
#define BOOST_TEST_MODULE Tidop svd test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/svd.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(SvdTestSuite)

struct SvdTest
{

  SvdTest()
    : svd(nullptr),
      svd2(nullptr)
  {}
  ~SvdTest()
  {
    delete svd;
    delete svd2;
  }

  void setup()
  {
    A = Matrix<float>(3,3);
    A[0][0] = 12.;
    A[0][1] = -51.;
    A[0][2] = 4.;
    A[1][0] = 6.;
    A[1][1] = 167.;
    A[1][2] = -68.;
    A[2][0] = -4.;
    A[2][1] = 24.;
    A[2][2] = -41.;

    B = Vector<float>(3);
    B[0] = 1;
    B[1] = 3;
    B[2] = 2;

    svd = new SingularValueDecomposition<Matrix<float>>(A);

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

    svd2 = new SingularValueDecomposition<Matrix<double>>(A2);

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

    s_svd = new SingularValueDecomposition<Matrix<float, 3, 3>>(SA);
  }

  void teardown()
  {
    
  }

  Matrix<float> A;
  Vector<float> B;
  SingularValueDecomposition<Matrix<float>> *svd;
  Matrix<double> A2;
  Vector<double> B2;
  SingularValueDecomposition<Matrix<double>> *svd2;

  Matrix<float, 3, 3> SA;
  Vector<float, 3> SB;
  SingularValueDecomposition<Matrix<float, 3, 3>> *s_svd;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, SvdTest)
{
  Matrix<float> u = svd->u();
  Matrix<float> v = svd->v();
  Vector<float> w = svd->w();

  BOOST_CHECK_CLOSE(-0.254378, u[0][0], 0.1);
  BOOST_CHECK_CLOSE(-0.513984, u[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.819215, u[0][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.819215, u[0][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.94641, u[1][0], 0.1);
  BOOST_CHECK_CLOSE(0.0419982, u[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.320224, u[1][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.320224, u[1][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.198995, u[2][0], 0.1);
  BOOST_CHECK_CLOSE(-0.856771, u[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(0.475756, u[2][2], 0.1);
#else
  BOOST_CHECK_CLOSE(-0.475756, u[2][2], 0.1);
#endif

  BOOST_CHECK_CLOSE(0.00960263, v[0][0], 0.1);
  BOOST_CHECK_CLOSE(-0.0757445, v[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.997081, v[0][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.997081, v[0][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.922508, v[1][0], 0.1);
  BOOST_CHECK_CLOSE(0.38544, v[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.020396, v[1][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.020396, v[1][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(-0.38586, v[2][0], 0.1);
  BOOST_CHECK_CLOSE(0.919619, v[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.0735761, v[2][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.0735761, v[2][2], 0.1);
#endif


  Matrix<double> u2 = svd2->u();
  Matrix<double> v2 = svd2->v();
  Vector<double> w2 = svd2->w();

#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.209337, u2[0][0], 0.1);
#else
  BOOST_CHECK_CLOSE(0.209337, u2[0][0], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.964385, u2[0][1], 0.1); 
  BOOST_CHECK_CLOSE(0.161676, u2[0][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.503849, u2[1][0], 0.1);
#else
  BOOST_CHECK_CLOSE(0.503849, u2[1][0], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.0353216, u2[1][1], 0.1);
  BOOST_CHECK_CLOSE(-0.863069, u2[1][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.838042, u2[2][0], 0.1);
#else
  BOOST_CHECK_CLOSE(0.838042, u2[2][0], 0.1);
#endif
  BOOST_CHECK_CLOSE(-0.262133, u2[2][1], 0.1);
  BOOST_CHECK_CLOSE(0.47851, u2[2][2], 0.1);

#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.464668, v2[0][0], 0.1);
#else
  BOOST_CHECK_CLOSE(0.464668, v2[0][0], 0.1);
#endif
  BOOST_CHECK_CLOSE(-0.833287, v2[0][1], 0.1);
  BOOST_CHECK_CLOSE(0.299528, v2[0][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.553755, v2[1][0], 0.1);
#else
  BOOST_CHECK_CLOSE(0.553755, v2[1][0], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.00950048, v2[1][1], 0.1);
  BOOST_CHECK_CLOSE(-0.832626, v2[1][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.69097, v2[2][0], 0.1);
#else
  BOOST_CHECK_CLOSE(0.69097, v2[2][0], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.552759, v2[2][1], 0.1);
  BOOST_CHECK_CLOSE(0.465851, v2[2][2], 0.1);

  Matrix<float, 3, 3> u3 = s_svd->u();
  Matrix<float, 3, 3> v3 = s_svd->v();
  Vector<float, 3> w3 = s_svd->w();

  BOOST_CHECK_CLOSE(-0.254378, u3[0][0], 0.1);
  BOOST_CHECK_CLOSE(-0.513984, u3[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.819215, u3[0][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.819215, u3[0][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.0419982, u3[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.320224, u3[1][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.320224, u3[1][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.198995, u3[2][0], 0.1);
  BOOST_CHECK_CLOSE(-0.856771, u3[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(0.475756, u3[2][2], 0.1);
#else
  BOOST_CHECK_CLOSE(-0.475756, u3[2][2], 0.1);
#endif

  BOOST_CHECK_CLOSE(0.00960263, v3[0][0], 0.1);
  BOOST_CHECK_CLOSE(-0.0757445, v3[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.997081, v3[0][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.997081, v3[0][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(0.922508, v3[1][0], 0.1);
  BOOST_CHECK_CLOSE(0.38544, v3[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.020396, v3[1][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.020396, v3[1][2], 0.1);
#endif
  BOOST_CHECK_CLOSE(-0.38586, v3[2][0], 0.1);
  BOOST_CHECK_CLOSE(0.919619, v3[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
  BOOST_CHECK_CLOSE(-0.0735761, v3[2][2], 0.1);
#else
  BOOST_CHECK_CLOSE(0.0735761, v3[2][2], 0.1);
#endif
}

BOOST_FIXTURE_TEST_CASE(solve, SvdTest)
{
  auto C = svd->solve(B);
  BOOST_CHECK_CLOSE(0.0653061, C[0], 0.1);
  BOOST_CHECK_CLOSE(-0.00897959, C[1], 0.1);
  BOOST_CHECK_CLOSE(-0.0604082, C[2], 0.1);
  
  auto C2 = svd2->solve(B2);
  BOOST_CHECK_CLOSE(-2., C2[0], 0.1);
  BOOST_CHECK_CLOSE(1., C2[1], 0.1);
  BOOST_CHECK_CLOSE(1., C2[2], 0.1);
  
  auto SC = s_svd->solve(SB);
  BOOST_CHECK_CLOSE(0.065306122448979584, SC[0], 0.1);
  BOOST_CHECK_CLOSE(-0.0089795918367346957, SC[1], 0.1);
  BOOST_CHECK_CLOSE(-0.060408163265306125, SC[2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(solve_matrix, SvdTest)
{
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
