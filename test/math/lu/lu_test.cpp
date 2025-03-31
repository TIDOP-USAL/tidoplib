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
 
#define BOOST_TEST_MODULE Tidop lu test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/decomp/lu.h>
#include <tidop/math/algebra/matrix.h>

using namespace tl;

constexpr double small_values = 1e-14;

BOOST_AUTO_TEST_SUITE(LuTestSuite)

struct LuTest
{

    LuTest()
      : lu(nullptr),
        lu2(nullptr),
        slu(nullptr)
    {
    }
    ~LuTest()
    {
        delete lu;
        delete lu2;
        delete slu;
    }

    void setup()
    {
        A = Matrix<double>(3, 3);
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

        A2 = Matrix<double>(3, 3);
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

        SA[0][0] = 12.;
        SA[0][1] = -51.;
        SA[0][2] = 4.;
        SA[1][0] = 6.;
        SA[1][1] = 167.;
        SA[1][2] = -68.;
        SA[2][0] = -4.;
        SA[2][1] = 24.;
        SA[2][2] = -41.;

        SB[0] = 1;
        SB[1] = 3;
        SB[2] = 2;

        slu = new LuDecomposition<Matrix<double, 3, 3>>(SA);

        A_4x4 = {2, 1, 1, 0,
                 4, 3, 3, 1,
                 8, 7, 9, 5,
                 6, 7, 9, 8};

        b_4x2 = {1, 2,
                 2, 3,
                 3, 4,
                 4, 5};
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

    Matrix<double, 4, 4> A_4x4;
    Matrix<double, 4, 2> b_4x2;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, LuTest)
{

    Matrix<double> _lu = lu->lu();

    BOOST_CHECK_CLOSE(12, _lu[0][0], 0.1);
    BOOST_CHECK_CLOSE(-51, _lu[0][1], 0.1);
    BOOST_CHECK_CLOSE(4, _lu[0][2], 0.1);
    BOOST_CHECK_CLOSE(0.5, _lu[1][0], 0.1);
    BOOST_CHECK_CLOSE(192.5, _lu[1][1], 0.1);
    BOOST_CHECK_CLOSE(-70, _lu[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.333333, _lu[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.0363636, _lu[2][1], 0.1);
    BOOST_CHECK_CLOSE(-37.1212, _lu[2][2], 0.1);

    Matrix<double> _lu2 = lu2->lu();

    BOOST_CHECK_CLOSE(7, _lu2[0][0], 0.1);
    BOOST_CHECK_CLOSE(8, _lu2[0][1], 0.1);
    BOOST_CHECK_CLOSE(10, _lu2[0][2], 0.1);
    BOOST_CHECK_CLOSE(0.142857, _lu2[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.857143, _lu2[1][1], 0.1);
    BOOST_CHECK_CLOSE(1.57143, _lu2[1][2], 0.1);
    BOOST_CHECK_CLOSE(0.571429, _lu2[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.5, _lu2[2][1], 0.1);
    BOOST_CHECK_CLOSE(-0.5, _lu2[2][2], 0.1);
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

BOOST_FIXTURE_TEST_CASE(solve_matrix_partial, LuTest)
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

    Matrix<double> mb(3, 2);
    mb[0][0] = 3;
    mb[0][1] = 4;
    mb[1][0] = 6;
    mb[1][1] = 54;
    mb[2][0] = 9;
    mb[2][1] = 13;

    LuDecomposition<Matrix<double>> decomp2(A);

    auto mc = decomp2.solve(mb);
    BOOST_CHECK_CLOSE(0.0281633, mc[0][0], 0.1);
    BOOST_CHECK_CLOSE(1.0751, mc[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.0729796, mc[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.148245, mc[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.26498, mc[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.335184, mc[2][1], 0.1);

    LuDecomposition<Matrix<double, 4, 4>> slu(A_4x4);
    auto x = slu.solve(b_4x2);

    BOOST_CHECK_CLOSE(1., x[0][0], 0.1);
    BOOST_CHECK_CLOSE(2.5, x[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.5, x[1][0], 0.1);
    BOOST_CHECK_CLOSE(-0.5, x[1][1], 0.1);
    BOOST_CHECK_CLOSE(-1.5, x[2][0], 0.1);
    BOOST_CHECK_CLOSE(-2.5, x[2][1], 0.1);
    BOOST_CHECK_CLOSE(1., x[3][0], 0.1);
    BOOST_CHECK_CLOSE(2., x[3][1], 0.1);
}

BOOST_FIXTURE_TEST_CASE(solve_matrix_full_pivot, LuTest)
{
    LuDecomposition<Matrix<double, 4, 4>> slu(A_4x4, true);
    auto x = slu.solve(b_4x2);

    BOOST_CHECK_CLOSE(1., x[0][0], 0.1);
    BOOST_CHECK_CLOSE(2.5, x[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.5, x[1][0], 0.1);
    BOOST_CHECK_CLOSE(-0.5, x[1][1], 0.1);
    BOOST_CHECK_CLOSE(-1.5, x[2][0], 0.1);
    BOOST_CHECK_CLOSE(-2.5, x[2][1], 0.1);
    BOOST_CHECK_CLOSE(1., x[3][0], 0.1);
    BOOST_CHECK_CLOSE(2., x[3][1], 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse, LuTest)
{

    LuDecomposition<Matrix<double, 4, 4>> lu(A_4x4);
    Matrix<double, 4, 4> A_inv = lu.inverse();

    BOOST_CHECK_CLOSE(2.25, A_inv[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.75, A_inv[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.25, A_inv[0][2], 0.1);
    BOOST_CHECK_CLOSE(0.25, A_inv[0][3], 0.1);
    BOOST_CHECK_CLOSE(-3., A_inv[1][0], 0.1);
    BOOST_CHECK_CLOSE(2.5, A_inv[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.5, A_inv[1][2], 0.1);
    BOOST_CHECK_SMALL(A_inv[1][3], small_values);
    BOOST_CHECK_CLOSE(-0.5, A_inv[2][0], 0.1);
    BOOST_CHECK_CLOSE(-1, A_inv[2][1], 0.1);
    BOOST_CHECK_CLOSE(1., A_inv[2][2], 0.1);
    BOOST_CHECK_CLOSE(-0.5, A_inv[2][3], 0.1);
    BOOST_CHECK_CLOSE(1.5, A_inv[3][0], 0.1);
    BOOST_CHECK_CLOSE(-0.5, A_inv[3][1], 0.1);
    BOOST_CHECK_CLOSE(-0.5, A_inv[3][2], 0.1);
    BOOST_CHECK_CLOSE(0.5, A_inv[3][3], 0.1);

    // Verificar que A * A_inv ≈ Identidad
    Matrix<double, 4, 4> I = A_4x4 * A_inv;


    BOOST_CHECK_CLOSE(1., I[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I[2][2], 0.1);
    BOOST_CHECK_CLOSE(1., I[3][3], 0.1);
}

BOOST_FIXTURE_TEST_CASE(rank, LuTest)
{
    LuDecomposition<Matrix<double, 4, 4>> lu(A_4x4);
    BOOST_CHECK_EQUAL(4, lu.rank());
}

BOOST_FIXTURE_TEST_CASE(lower, LuTest)
{
    LuDecomposition<Matrix<double, 4, 4>> lu(A_4x4, true);
    auto lower = lu.lower();
    BOOST_CHECK_CLOSE(1, lower[0][0], 0.1);
    BOOST_CHECK_CLOSE(0, lower[0][1], 0.1);
    BOOST_CHECK_CLOSE(0, lower[0][2], 0.1);
    BOOST_CHECK_CLOSE(0, lower[0][3], 0.1);
    BOOST_CHECK_CLOSE(1, lower[1][0], 0.1);
    BOOST_CHECK_CLOSE(1, lower[1][1], 0.1);
    BOOST_CHECK_CLOSE(0, lower[1][2], 0.1);
    BOOST_CHECK_CLOSE(0, lower[1][3], 0.1);
    BOOST_CHECK_CLOSE(0.333333, lower[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.222222, lower[2][1], 0.1);
    BOOST_CHECK_CLOSE(1., lower[2][2], 0.1);
    BOOST_CHECK_CLOSE(0., lower[2][3], 0.1);
    BOOST_CHECK_CLOSE(0.111111, lower[3][0], 0.1);
    BOOST_CHECK_CLOSE(-0.185185, lower[3][1], 0.1);
    BOOST_CHECK_CLOSE(0.833333, lower[3][2], 0.1);
    BOOST_CHECK_CLOSE(1., lower[3][3], 0.1);

}

BOOST_FIXTURE_TEST_CASE(upper, LuTest)
{
    LuDecomposition<Matrix<double, 4, 4>> lu(A_4x4, true);
    auto upper = lu.upper();
    BOOST_CHECK_CLOSE(9, upper[0][0], 0.1);
    BOOST_CHECK_CLOSE(5, upper[0][1], 0.1);
    BOOST_CHECK_CLOSE(8, upper[0][2], 0.1);
    BOOST_CHECK_CLOSE(7, upper[0][3], 0.1);
    BOOST_CHECK_CLOSE(0, upper[1][0], 0.1);
    BOOST_CHECK_CLOSE(3, upper[1][1], 0.1);
    BOOST_CHECK_CLOSE(-2, upper[1][2], 0.1);
    BOOST_CHECK_CLOSE(0, upper[1][3], 0.1);
    BOOST_CHECK_CLOSE(0, upper[2][0], 0.1);
    BOOST_CHECK_CLOSE(0, upper[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.888889, upper[2][2], 0.1);
    BOOST_CHECK_CLOSE(0.666667, upper[2][3], 0.1);
    BOOST_CHECK_CLOSE(0, upper[3][0], 0.1);
    BOOST_CHECK_CLOSE(0, upper[3][1], 0.1);
    BOOST_CHECK_CLOSE(0, upper[3][2], 0.1);
    BOOST_CHECK_CLOSE(-0.333333, upper[3][3], 0.1);

}

BOOST_FIXTURE_TEST_CASE(determinant, LuTest)
{
    LuDecomposition<Matrix<double, 4, 4>> lu(A_4x4);
    BOOST_CHECK_EQUAL(8, lu.determinant());
}


//BOOST_FIXTURE_TEST_CASE(rectangular, LuTest)
//{
//    Matrix<double, 3, 4> A = {
//        {2, 3, 1, 5},
//        {6, 13, 5, 19},
//        {2, 19, 10, 23}
//    };
//
//    LuDecomposition<Matrix<double, 3, 4>> lu(A);
//
//    Matrix<double, 3, 4> L = lu.lower();
//    Matrix<double, 3, 4> U = lu.upper();
//
//    // Verificar estructura de L (triangular inferior con 1s en la diagonal)
//    BOOST_TEST(L(0, 0) == 1.0);
//    BOOST_TEST(L(1, 1) == 1.0);
//    BOOST_TEST(L(2, 2) == 1.0);
//
//    // Verificar estructura de U (triangular superior)
//    BOOST_TEST(U(1, 0) == 0.0);
//    BOOST_TEST(U(2, 0) == 0.0);
//    BOOST_TEST(U(2, 1) == 0.0);
//
//    // Comprobar que \( L \times U \) aproxima \( A \)
//    auto LU_result = L * U;
//    BOOST_TEST(LU_result.isApproximatelyEqual(A, 1e-6));
//}

BOOST_AUTO_TEST_SUITE_END()

//BOOST_AUTO_TEST_CASE(LU_IsSingular_Test)
//{
//    Matrix<double, 3, 3> A = {
//        {1, 2, 3},
//        {4, 5, 6},
//        {7, 8, 9} // Matriz singular (det = 0)
//    };
//
//    LuDecomposition<Matrix<double, 3, 3>> lu(A, true);
//    BOOST_TEST(lu.isSingular() == true);
//
//    // Matriz no singular
//    Matrix<double, 3, 3> B = {
//        {4, 3, 2},
//        {2, 1, 3},
//        {3, 5, 1}
//    };
//
//    LuDecomposition<Matrix<double, 3, 3>> luB(B, true);
//    BOOST_TEST(luB.isSingular() == false);
//}

//BOOST_AUTO_TEST_CASE(LU_LeastSquares_Solution)
//{
//    Matrix<double, 4, 3> A = {
//        {2, 3, 1},
//        {6, 13, 5},
//        {2, 19, 10},
//        {4, 10, 7}
//    };
//
//    Vector<double, 4> b = {1, 2, 3, 4};
//
//    LuDecomposition<Matrix<double, 4, 3>> lu(A);
//    Vector<double, 3> x = lu.leastSquares(b);
//
//    // Comparación con solución obtenida por Eigen
//    Eigen::MatrixXd A_eigen(4, 3);
//    Eigen::VectorXd b_eigen(4);
//
//    for (int i = 0; i < 4; ++i) {
//        b_eigen(i) = b[i];
//        for (int j = 0; j < 3; ++j)
//            A_eigen(i, j) = A(i, j);
//    }
//
//    Eigen::VectorXd x_eigen = A_eigen.colPivHouseholderQr().solve(b_eigen);
//
//    BOOST_TEST(x.isApproximatelyEqual(x_eigen, 1e-6));
//}

//BOOST_AUTO_TEST_CASE(LU_Underdetermined_Solution)
//{
//    Matrix<double, 2, 3> A = {
//        {2, 3, 1},
//        {6, 13, 5}
//    };
//
//    Vector<double, 2> b = {1, 2};
//
//    LuDecomposition<Matrix<double, 2, 3>> lu(A);
//    Vector<double, 3> x = lu.solve(b);
//
//    // Verificar que Ax ≈ b
//    Vector<double, 2> Ax = A * x;
//    BOOST_TEST(Ax.isApproximatelyEqual(b, 1e-6));
//}

