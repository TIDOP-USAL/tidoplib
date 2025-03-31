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
#include <tidop/math/algebra/decomp/cholesky.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(CholeskyTestSuite)

struct CholeskyTest
{

    CholeskyTest()
      : cholesky(nullptr),
        cholesky2(nullptr),
        cholesky3(nullptr)
    {
    }
    ~CholeskyTest()
    {
        delete cholesky;
        delete cholesky2;
        delete cholesky3;
    }

    void setup()
    {
        //https://atozmath.com/MatrixEv.aspx?q=choleskydecomp
        A = Matrix<double>(3, 3);
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

        A_4x4 = {4, 12, -16, 2,
                 12, 37, -43, 5,
                -16, -43, 98, -6,
                  2, 5, -6, 10};

        b_4 = {1, 2, 3, 4};

        cholesky3 = new CholeskyDecomposition<Matrix<double, 4, 4>>(A_4x4);
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

    Matrix<double, 4, 4> A_4x4;

    Vector<double, 4> b_4;

    // **Descomposición de Cholesky en TidopLib**
    CholeskyDecomposition<Matrix<double, 4, 4>> *cholesky3;
};


BOOST_FIXTURE_TEST_CASE(lower_matrix, CholeskyTest)
{
    Matrix<double> l = cholesky->lower();
    BOOST_CHECK_CLOSE(2.44949, l[0][0], 0.1);
    BOOST_CHECK_CLOSE(0., l[0][1], 0.1);
    BOOST_CHECK_CLOSE(0., l[0][2], 0.1);
    BOOST_CHECK_CLOSE(6.12372, l[1][0], 0.1);
    BOOST_CHECK_CLOSE(4.1833, l[1][1], 0.1);
    BOOST_CHECK_CLOSE(0., l[1][2], 0.1);
    BOOST_CHECK_CLOSE(22.4537, l[2][0], 0.1);
    BOOST_CHECK_CLOSE(20.9165, l[2][1], 0.1);
    BOOST_CHECK_CLOSE(6.1101, l[2][2], 0.1);

    Matrix<double, 3, 3> l2 = cholesky2->lower();
    BOOST_CHECK_CLOSE(2.44949, l2[0][0], 0.1);
    BOOST_CHECK_CLOSE(0., l2[0][1], 0.1);
    BOOST_CHECK_CLOSE(0., l2[0][2], 0.1);
    BOOST_CHECK_CLOSE(6.12372, l2[1][0], 0.1);
    BOOST_CHECK_CLOSE(4.1833, l2[1][1], 0.1);
    BOOST_CHECK_CLOSE(0., l2[1][2], 0.1);
    BOOST_CHECK_CLOSE(22.4537, l2[2][0], 0.1);
    BOOST_CHECK_CLOSE(20.9165, l2[2][1], 0.1);
    BOOST_CHECK_CLOSE(6.1101, l2[2][2], 0.1);

    auto l4x4 = cholesky3->lower();
    BOOST_CHECK_CLOSE(2., l4x4[0][0], 0.1);
    BOOST_CHECK_CLOSE(0., l4x4[0][1], 0.1);
    BOOST_CHECK_CLOSE(0., l4x4[0][2], 0.1);
    BOOST_CHECK_CLOSE(0., l4x4[0][3], 0.1);
    BOOST_CHECK_CLOSE(6., l4x4[1][0], 0.1);
    BOOST_CHECK_CLOSE(1., l4x4[1][1], 0.1);
    BOOST_CHECK_CLOSE(0., l4x4[1][2], 0.1);
    BOOST_CHECK_CLOSE(0., l4x4[1][3], 0.1);
    BOOST_CHECK_CLOSE(-8., l4x4[2][0], 0.1);
    BOOST_CHECK_CLOSE(5., l4x4[2][1], 0.1);
    BOOST_CHECK_CLOSE(3., l4x4[2][2], 0.1);
    BOOST_CHECK_CLOSE(0., l4x4[2][3], 0.1);
    BOOST_CHECK_CLOSE(1., l4x4[3][0], 0.1);
    BOOST_CHECK_CLOSE(-1., l4x4[3][1], 0.1);
    BOOST_CHECK_CLOSE(2.33333, l4x4[3][2], 0.1);
    BOOST_CHECK_CLOSE(1.59861, l4x4[3][3], 0.1);
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

    auto C3 = cholesky3->solve(b_4);
    BOOST_CHECK_CLOSE(77.4565, C3[0], 0.1);
    BOOST_CHECK_CLOSE(-20.7391, C3[1], 0.1);
    BOOST_CHECK_CLOSE(3.41304, C3[2], 0.1);
    BOOST_CHECK_CLOSE(-2.67391, C3[3], 0.1);
}

BOOST_FIXTURE_TEST_CASE(l_lt, CholeskyTest)
{
    Matrix<double> l = cholesky->lower();
    auto llt = l *l.transpose();
    for (size_t r = 0; r < A.rows(); ++r) {
        for (size_t c = 0; c < A.cols(); ++c) {
            BOOST_CHECK_CLOSE(A[r][c], llt[r][c], 0.1);
        }
    }

    Matrix<double, 3, 3> l2 = cholesky2->lower();
    auto llt2 = l2 * l2.transpose();
    for (size_t r = 0; r < A2.rows(); ++r) {
        for (size_t c = 0; c < A2.cols(); ++c) {
            BOOST_CHECK_CLOSE(A2[r][c], llt2[r][c], 0.1);
        }
    }

    auto l3 = cholesky3->lower();
    auto llt3 = l3 * l3.transpose();
    for (size_t r = 0; r < A_4x4.rows(); ++r) {
        for (size_t c = 0; c < A_4x4.cols(); ++c) {
            BOOST_CHECK_CLOSE(A_4x4[r][c], llt3[r][c], 0.1);
        }
    }
}

BOOST_FIXTURE_TEST_CASE(inverse, CholeskyTest)
{
    Matrix<double, 3, 3> inverse3x3 = cholesky2->inverse();


    auto inverse = cholesky3->inverse();
    BOOST_CHECK_CLOSE(180.087, inverse[0][0], 0.1);
    BOOST_CHECK_CLOSE(-48.5217, inverse[0][1], 0.1);
    BOOST_CHECK_CLOSE(7.67391, inverse[0][2], 0.1);
    BOOST_CHECK_CLOSE(-7.15217, inverse[0][3], 0.1);
    BOOST_CHECK_CLOSE(-48.5217, inverse[1][0], 0.1);
    BOOST_CHECK_CLOSE(13.1304, inverse[1][1], 0.1);
    BOOST_CHECK_CLOSE(-2.04348, inverse[1][2], 0.1);
    BOOST_CHECK_CLOSE(1.91304, inverse[1][3], 0.1);
    BOOST_CHECK_CLOSE(7.67391, inverse[2][0], 0.1);
    BOOST_CHECK_CLOSE(-2.04348, inverse[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.347826, inverse[2][2], 0.1);
    BOOST_CHECK_CLOSE(-0.304348, inverse[2][3], 0.1);
    BOOST_CHECK_CLOSE(-7.15217, inverse[3][0], 0.1);
    BOOST_CHECK_CLOSE(1.91304, inverse[3][1], 0.1);
    BOOST_CHECK_CLOSE(-0.304348, inverse[3][2], 0.1);
    BOOST_CHECK_CLOSE(0.391304, inverse[3][3], 0.1);  
}

BOOST_FIXTURE_TEST_CASE(determinant, CholeskyTest)
{
    BOOST_CHECK_CLOSE(92., cholesky3->determinant(), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
