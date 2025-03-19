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
#include <boost/test/debug.hpp>

#include <tidop/math/algebra/decomp/svd.h>
#include <tidop/math/algebra/matrix.h>

using namespace tl;



BOOST_AUTO_TEST_SUITE(SvdTestSuite)

struct SvdTest
{

    SvdTest()
    {
    }
    ~SvdTest()
    {
    }

    void setup()
    {
        boost::debug::detect_memory_leaks(0);

        A = Matrix<float>(3, 3);
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

        A8x5 = {22, 10,  2,   3,  7,
                14,  7, 10,   0,  8,
                -1, 13, -1, -11,  3,
                -3, -2, 13,  -2,  4,
                 9,  8,  1,  -2,  4,
                 9,  1, -7,   5, -1,
                 2, -6,  6,   5,  1,
                 4,  5,  0,  -2,  2};

    }

    void teardown()
    {

    }

    Matrix<float> A;
    Vector<float> B;

    Matrix<double> A2;
    Vector<double> B2;

    Matrix<float, 3, 3> SA;
    Vector<float, 3> SB;

    Matrix<double, 8, 5> A8x5;
};


BOOST_FIXTURE_TEST_CASE(squared_matrix_float, SvdTest)
{
    SingularValueDecomposition<Matrix<float>> svd(A);

    Matrix<float> u = svd.u();
    Matrix<float> v = svd.v();
    Vector<float> w = svd.w();

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


    BOOST_CHECK_CLOSE(190.567276, w[0], 0.1);
    BOOST_CHECK_CLOSE(32.8568878, w[1], 0.1);
    BOOST_CHECK_CLOSE(13.6949205, w[2], 0.1);

    BOOST_CHECK(w[0] >= w[1]);
    BOOST_CHECK(w[1] >= w[2]);

    // Orthogonality verification of U and V

    auto I = u.transpose() * u;

    BOOST_CHECK_CLOSE(1., I[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I[2][2], 0.1);

    for (size_t i = 0; i < I.rows(); i++) {
        for (size_t j = 0; j < I.cols(); j++) {
            if (i == j) {
                BOOST_CHECK_CLOSE(1., I[i][j], 0.1);  // Diagonal ≈ 1
            } else {
                BOOST_CHECK_SMALL(I[i][j], 1e-5f);  // Fuera de la diagonal ≈ 0
            }
        }
    }

    auto I2 = v.transpose() * v;

    BOOST_CHECK_CLOSE(1., I2[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I2[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I2[2][2], 0.1);
}
 
BOOST_FIXTURE_TEST_CASE(squared_matrix_double, SvdTest)
{
    SingularValueDecomposition<Matrix<double>> svd(A2);

    Matrix<double> U = svd.u();
    Matrix<double> V = svd.v();
    Vector<double> W = svd.w();

#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.209337, U[0][0], 0.1);
#else
    BOOST_CHECK_CLOSE(0.209337, U[0][0], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.964385, U[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.161676, U[0][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.503849, U[1][0], 0.1);
#else
    BOOST_CHECK_CLOSE(0.503849, U[1][0], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.0353216, U[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.863069, U[1][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.838042, U[2][0], 0.1);
#else
    BOOST_CHECK_CLOSE(0.838042, U[2][0], 0.1);
#endif
    BOOST_CHECK_CLOSE(-0.262133, U[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.47851, U[2][2], 0.1);

#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.464668, V[0][0], 0.1);
#else
    BOOST_CHECK_CLOSE(0.464668, V[0][0], 0.1);
#endif
    BOOST_CHECK_CLOSE(-0.833287, V[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.299528, V[0][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.553755, V[1][0], 0.1);
#else
    BOOST_CHECK_CLOSE(0.553755, V[1][0], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.00950048, V[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.832626, V[1][2], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.69097, V[2][0], 0.1);
#else
    BOOST_CHECK_CLOSE(0.69097, V[2][0], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.552759, V[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.465851, V[2][2], 0.1);

    BOOST_CHECK_CLOSE(17.4125, W[0], 0.1);
    BOOST_CHECK_CLOSE(0.87516, W[1], 0.1);
    BOOST_CHECK_CLOSE(0.19686, W[2], 0.1);

    BOOST_CHECK(W[0] >= W[1]);
    BOOST_CHECK(W[1] >= W[2]);

    // Orthogonality verification of U and V

    auto I = U.transpose() * U;

    BOOST_CHECK_CLOSE(1., I[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I[2][2], 0.1);

    auto I2 = V.transpose() * V;

    BOOST_CHECK_CLOSE(1., I2[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I2[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I2[2][2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(squared_matrix_static, SvdTest)
{
    SingularValueDecomposition<Matrix<float, 3, 3>> svd(SA);

    Matrix<float, 3, 3> U = svd.u();
    Matrix<float, 3, 3> V = svd.v();
    Vector<float, 3> W = svd.w();

    BOOST_CHECK_CLOSE(-0.254378, U[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.513984, U[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.819215, U[0][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.819215, U[0][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.0419982, U[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.320224, U[1][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.320224, U[1][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.198995, U[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.856771, U[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(0.475756, U[2][2], 0.1);
#else
    BOOST_CHECK_CLOSE(-0.475756, U[2][2], 0.1);
#endif

    BOOST_CHECK_CLOSE(0.00960263, V[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.0757445, V[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.997081, V[0][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.997081, V[0][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.922508, V[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.38544, V[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.020396, V[1][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.020396, V[1][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(-0.38586, V[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.919619, V[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.0735761, V[2][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.0735761, V[2][2], 0.1);
#endif

    BOOST_CHECK_CLOSE(190.665, W[0], 0.1);
    BOOST_CHECK_CLOSE(32.8569, W[1], 0.1);
    BOOST_CHECK_CLOSE(13.6949, W[2], 0.1);

    BOOST_CHECK(W[0] >= W[1]);
    BOOST_CHECK(W[1] >= W[2]);

    // Orthogonality verification of U and V

    auto I = U.transpose() * U;

    BOOST_CHECK_CLOSE(1., I[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I[2][2], 0.1);

    auto I2 = V.transpose() * V;

    BOOST_CHECK_CLOSE(1., I2[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I2[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I2[2][2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(rect_matrix, SvdTest)
{
    Matrix<double, 4, 3> rect_mat = {12, -51,   4,
                                      6,  167, -68,
                                     -4,   24, -41,
                                      2,   12,  13};
    SingularValueDecomposition<Matrix<double, 4, 3>> svd(rect_mat, true);
    auto U = svd.u();

    BOOST_CHECK_CLOSE(-0.254467, U[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.441243, U[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.858115, U[0][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.858115, U[0][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.0647305, U[0][3], 0.1);
    BOOST_CHECK_CLOSE(0.945955, U[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.0288187, U[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.303645, U[1][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.303645, U[1][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(-0.11017, U[1][3], 0.1);
    BOOST_CHECK_CLOSE(0.198453, U[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.775938, U[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(0.37533, U[2][2], 0.1);
#else
    BOOST_CHECK_CLOSE(-0.37533, U[2][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.466545, U[2][3], 0.1);
    BOOST_CHECK_CLOSE(0.0321069, U[3][0], 0.1);
    BOOST_CHECK_CLOSE(0.449884, U[3][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.174831, U[3][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.174831, U[3][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.875219, U[3][3], 0.1);

    auto V = svd.v();

    BOOST_CHECK_CLOSE(0.00992593, V[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.0304234, V[0][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.999488, V[0][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.999488, V[0][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(0.923614, V[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.383317, V[1][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.00249534, V[1][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.00249534, V[1][2], 0.1);
#endif
    BOOST_CHECK_CLOSE(-0.383196, V[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.923116, V[2][1], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.0319042, V[2][2], 0.1);
#else
    BOOST_CHECK_CLOSE(0.0319042, V[2][2], 0.1);
#endif

    auto W = svd.w();

    BOOST_CHECK_CLOSE(190.665, W[0], 0.1);
    BOOST_CHECK_CLOSE(36.7638, W[1], 0.1);
    BOOST_CHECK_CLOSE(13.9774, W[2], 0.1);

    BOOST_CHECK(W[0] >= W[1]);
    BOOST_CHECK(W[1] >= W[2]);

    // Orthogonality verification of U and V

    auto I = U.transpose() * U;

    BOOST_CHECK_CLOSE(1., I[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I[2][2], 0.1);
    BOOST_CHECK_CLOSE(1., I[3][3], 0.1);

    auto I2 = V.transpose() * V;

    BOOST_CHECK_CLOSE(1., I2[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I2[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I2[2][2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(matrix_8x5, SvdTest)
{
    SingularValueDecomposition<Matrix<double>> svd(A8x5);

    auto U = svd.u();
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.707107, U[0][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.176777, U[0][2], 0.1);
    BOOST_CHECK_CLOSE(0.538426, U[0][3], 0.1);
    BOOST_CHECK_CLOSE(0.298828, U[0][4], 0.1);

    BOOST_CHECK_CLOSE(-0.53033, U[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[1][1], 0.1);
    BOOST_CHECK_CLOSE(0.353553, U[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.69694, U[1][3], 0.1);
    BOOST_CHECK_CLOSE(-0.00668641, U[1][4], 0.1);

    BOOST_CHECK_CLOSE(-0.176777, U[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.790569, U[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.176777, U[2][2], 0.1);
    BOOST_CHECK_CLOSE(0.197802, U[2][3], 0.1);
    BOOST_CHECK_CLOSE(-0.225688, U[2][4], 0.1);

    BOOST_CHECK_CLOSE(-8.73582e-18, U[3][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[3][1], 0.1);
    BOOST_CHECK_CLOSE(0.707107, U[3][2], 0.1);
    BOOST_CHECK_CLOSE(0.33884, U[3][3], 0.1);
    BOOST_CHECK_CLOSE(-0.438889, U[3][4], 0.1);

    BOOST_CHECK_CLOSE(-0.353553, U[4][0], 0.1);
    BOOST_CHECK_CLOSE(-0.158114, U[4][1], 0.1);
    BOOST_CHECK_CLOSE(-1.0636e-15, U[4][2], 0.1);
    BOOST_CHECK_CLOSE(-0.0900541, U[4][3], 0.1);
    BOOST_CHECK_CLOSE(-0.151281, U[4][4], 0.1);

    BOOST_CHECK_CLOSE(-0.176777, U[5][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[5][1], 0.1);
    BOOST_CHECK_CLOSE(-0.53033, U[5][2], 0.1);
    BOOST_CHECK_CLOSE(-0.045398, U[5][3], 0.1);
    BOOST_CHECK_CLOSE(-0.787301, U[5][4], 0.1);

    BOOST_CHECK_CLOSE(-2.48245e-17, U[6][0], 0.1);
    BOOST_CHECK_CLOSE(0.474342, U[6][1], 0.1);
    BOOST_CHECK_CLOSE(0.176777, U[6][2], 0.1);
    BOOST_CHECK_CLOSE(0.24295, U[6][3], 0.1);
    BOOST_CHECK_CLOSE(-0.0684578, U[6][4], 0.1);

    BOOST_CHECK_CLOSE(-0.176777, U[7][0], 0.1);
    BOOST_CHECK_CLOSE(-0.158114, U[7][1], 0.1);
    BOOST_CHECK_CLOSE(-1.08123e-15, U[7][2], 0.1);
    BOOST_CHECK_CLOSE(-0.0351785, U[7][3], 0.1);
    BOOST_CHECK_CLOSE(0.140299, U[7][4], 0.1);
#endif

    auto W = svd.w();

    BOOST_CHECK_CLOSE(35.327, W[0], 0.1);
    BOOST_CHECK_CLOSE(20, W[1], 0.1);
    BOOST_CHECK_CLOSE(19.5959, W[2], 0.1);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(2.47564e-15, W[3], 0.1);
    BOOST_CHECK_CLOSE(1.08689e-15, W[4], 0.1);
#endif

    BOOST_CHECK(W[0] >= W[1]);
    BOOST_CHECK(W[1] >= W[2]);
    BOOST_CHECK(W[2] >= W[3]);
    BOOST_CHECK(W[3] >= W[4]);

    auto V = svd.v();

#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.800641, V[0][0], 0.1);
    BOOST_CHECK_CLOSE(0.316228, V[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.288675, V[0][2], 0.1);
    BOOST_CHECK_CLOSE(0.248978, V[0][3], 0.1);
    BOOST_CHECK_CLOSE(0.337122, V[0][4], 0.1);

    BOOST_CHECK_CLOSE(-0.480384, V[1][0], 0.1);
    BOOST_CHECK_CLOSE(-0.632456, V[1][1], 0.1);
    BOOST_CHECK_CLOSE(-4.45791e-15, V[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.598381, V[1][3], 0.1);
    BOOST_CHECK_CLOSE(-0.105694, V[1][4], 0.1);

    BOOST_CHECK_CLOSE(-0.160128, V[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.316228, V[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.866025, V[2][2], 0.1);
    BOOST_CHECK_CLOSE(-0.249673, V[2][3], 0.1);
    BOOST_CHECK_CLOSE(0.249043, V[2][4], 0.1);

    BOOST_CHECK_CLOSE(1.21655e-18, V[3][0], 0.1);
    BOOST_CHECK_CLOSE(0.632456, V[3][1], 0.1);
    BOOST_CHECK_CLOSE(-0.288675, V[3][2], 0.1);
    BOOST_CHECK_CLOSE(-0.598034, V[3][3], 0.1);
    BOOST_CHECK_CLOSE(-0.398776, V[3][4], 0.1);

    BOOST_CHECK_CLOSE(-0.320256, V[4][0], 0.1);
    BOOST_CHECK_CLOSE(-2.01712e-15, V[4][1], 0.1);
    BOOST_CHECK_CLOSE(0.288675, V[4][2], 0.1);
    BOOST_CHECK_CLOSE(0.399962, V[4][3], 0.1);
    BOOST_CHECK_CLOSE(-0.808785, V[4][4], 0.1);
#endif

    // Orthogonality verification of U and V

    auto I = U.transpose() * U;

    //BOOST_CHECK_CLOSE(1., I[0][0], 0.1);
    //BOOST_CHECK_CLOSE(1., I[1][1], 0.1);
    //BOOST_CHECK_CLOSE(1., I[2][2], 0.1);
    //BOOST_CHECK_CLOSE(1., I[3][3], 0.1);
    //BOOST_CHECK_CLOSE(1., I[4][4], 0.1);
    for (size_t i = 0; i < I.rows(); i++) {
        for (size_t j = 0; j < I.cols(); j++) {
            if (i == j) {
                BOOST_CHECK_CLOSE(1., I[i][j], 0.1);  // Diagonal ≈ 1
            } else {
                BOOST_CHECK_SMALL(I[i][j], 1e-10);  // Fuera de la diagonal ≈ 0
            }
        }
    }

    auto I2 = V.transpose() * V;

    //BOOST_CHECK_CLOSE(1., I2[0][0], 0.1);
    //BOOST_CHECK_CLOSE(1., I2[1][1], 0.1);
    //BOOST_CHECK_CLOSE(1., I2[2][2], 0.1);
    //BOOST_CHECK_CLOSE(1., I2[3][3], 0.1);
    //BOOST_CHECK_CLOSE(1., I2[4][4], 0.1);

    for (size_t i = 0; i < I2.rows(); i++) {
        for (size_t j = 0; j < I2.cols(); j++) {
            if (i == j) {
                BOOST_CHECK_CLOSE(1., I2[i][j], 0.1);  // Diagonal ≈ 1
            } else {
                BOOST_CHECK_SMALL(I2[i][j], 1e-10);  // Fuera de la diagonal ≈ 0
            }
        }
    }
}

BOOST_FIXTURE_TEST_CASE(matrix_8x5_full_u, SvdTest)
{
    SingularValueDecomposition<Matrix<double>> svd(A8x5, true);

    auto U = svd.u();

    // La matriz U tiene diferencias según se use LAPACK o no, aunque lo importante 
    // es que sea ortogonal que se comprueba posteriormente
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.707107, U[0][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.176777, U[0][2], 0.1);
    BOOST_CHECK_CLOSE(0.538426, U[0][3], 0.1);
    BOOST_CHECK_CLOSE(0.298828, U[0][4], 0.1);
    BOOST_CHECK_CLOSE(0.147827, U[0][5], 0.1);
    BOOST_CHECK_CLOSE(-0.141366, U[0][6], 0.1);
    BOOST_CHECK_CLOSE(-0.150706, U[0][7], 0.1);

    BOOST_CHECK_CLOSE(-0.53033, U[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[1][1], 0.1);
    BOOST_CHECK_CLOSE(0.353553, U[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.69694, U[1][3], 0.1);
    BOOST_CHECK_CLOSE(-0.00668641, U[1][4], 0.1);
    BOOST_CHECK_CLOSE(0.224181, U[1][5], 0.1);
    BOOST_CHECK_CLOSE(0.107973, U[1][6], 0.1);
    BOOST_CHECK_CLOSE(-0.145134, U[1][7], 0.1);

    BOOST_CHECK_CLOSE(-0.176777, U[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.790569, U[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.176777, U[2][2], 0.1);
    BOOST_CHECK_CLOSE(0.197802, U[2][3], 0.1);
    BOOST_CHECK_CLOSE(-0.225688, U[2][4], 0.1);
    BOOST_CHECK_CLOSE(0.158901, U[2][5], 0.1);
    BOOST_CHECK_CLOSE(0.40808, U[2][6], 0.1);
    BOOST_CHECK_CLOSE(-0.1751, U[2][7], 0.1);

    BOOST_CHECK_CLOSE(-8.73582e-18, U[3][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[3][1], 0.1);
    BOOST_CHECK_CLOSE(0.707107, U[3][2], 0.1);
    BOOST_CHECK_CLOSE(0.33884, U[3][3], 0.1);
    BOOST_CHECK_CLOSE(-0.438889, U[3][4], 0.1);
    BOOST_CHECK_CLOSE(0.071945, U[3][5], 0.1);
    BOOST_CHECK_CLOSE(-0.387447, U[3][6], 0.1);
    BOOST_CHECK_CLOSE(0.110781, U[3][7], 0.1);

    BOOST_CHECK_CLOSE(-0.353553, U[4][0], 0.1);
    BOOST_CHECK_CLOSE(-0.158114, U[4][1], 0.1);
    BOOST_CHECK_CLOSE(-1.0636e-15, U[4][2], 0.1);
    BOOST_CHECK_CLOSE(-0.0900541, U[4][3], 0.1);
    BOOST_CHECK_CLOSE(-0.151281, U[4][4], 0.1);
    BOOST_CHECK_CLOSE(-0.892936, U[4][5], 0.1);
    BOOST_CHECK_CLOSE(-0.109636, U[4][6], 0.1);
    BOOST_CHECK_CLOSE(0.0982322, U[4][7], 0.1);

    BOOST_CHECK_CLOSE(-0.176777, U[5][0], 0.1);
    BOOST_CHECK_CLOSE(0.158114, U[5][1], 0.1);
    BOOST_CHECK_CLOSE(-0.53033, U[5][2], 0.1);
    BOOST_CHECK_CLOSE(-0.045398, U[5][3], 0.1);
    BOOST_CHECK_CLOSE(-0.787301, U[5][4], 0.1);
    BOOST_CHECK_CLOSE(0.18749, U[5][5], 0.1);
    BOOST_CHECK_CLOSE(0.00464994, U[5][6], 0.1);
    BOOST_CHECK_CLOSE(0.0736393, U[5][7], 0.1);

    BOOST_CHECK_CLOSE(-2.48245e-17, U[6][0], 0.1);
    BOOST_CHECK_CLOSE(0.474342, U[6][1], 0.1);
    BOOST_CHECK_CLOSE(0.176777, U[6][2], 0.1);
    BOOST_CHECK_CLOSE(0.24295, U[6][3], 0.1);
    BOOST_CHECK_CLOSE(-0.0684578, U[6][4], 0.1);
    BOOST_CHECK_CLOSE(-0.184748, U[6][5], 0.1);
    BOOST_CHECK_CLOSE(0.798347, U[6][6], 0.1);
    BOOST_CHECK_CLOSE(0.0924579, U[6][7], 0.1);

    BOOST_CHECK_CLOSE(-0.176777, U[7][0], 0.1);
    BOOST_CHECK_CLOSE(-0.158114, U[7][1], 0.1);
    BOOST_CHECK_CLOSE(-1.08123e-15, U[7][2], 0.1);
    BOOST_CHECK_CLOSE(-0.0351785, U[7][3], 0.1);
    BOOST_CHECK_CLOSE(0.140299, U[7][4], 0.1);
    BOOST_CHECK_CLOSE(0.17563, U[7][5], 0.1);
    BOOST_CHECK_CLOSE(0.0480867, U[7][6], 0.1);
    BOOST_CHECK_CLOSE(0.943223, U[7][7], 0.1);
#endif

    auto W = svd.w();

    BOOST_CHECK_CLOSE(35.327, W[0], 0.1);
    BOOST_CHECK_CLOSE(20, W[1], 0.1);
    BOOST_CHECK_CLOSE(19.5959, W[2], 0.1);
    BOOST_CHECK_CLOSE(2.47564e-15, W[3], 0.1);
    BOOST_CHECK_CLOSE(1.08689e-15, W[4], 0.1);

    BOOST_CHECK(W[0] >= W[1]);
    BOOST_CHECK(W[1] >= W[2]);
    BOOST_CHECK(W[2] >= W[3]);
    BOOST_CHECK(W[3] >= W[4]);

    auto V = svd.v();

    BOOST_CHECK_CLOSE(-0.800641, V[0][0], 0.1);
    BOOST_CHECK_CLOSE(0.316228, V[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.288675, V[0][2], 0.1);
    BOOST_CHECK_CLOSE(0.248978, V[0][3], 0.1);
    BOOST_CHECK_CLOSE(0.337122, V[0][4], 0.1);

    BOOST_CHECK_CLOSE(-0.480384, V[1][0], 0.1);
    BOOST_CHECK_CLOSE(-0.632456, V[1][1], 0.1);
    BOOST_CHECK_CLOSE(-4.45791e-15, V[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.598381, V[1][3], 0.1);
    BOOST_CHECK_CLOSE(-0.105694, V[1][4], 0.1);

    BOOST_CHECK_CLOSE(-0.160128, V[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.316228, V[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.866025, V[2][2], 0.1);
    BOOST_CHECK_CLOSE(-0.249673, V[2][3], 0.1);
    BOOST_CHECK_CLOSE(0.249043, V[2][4], 0.1);

    BOOST_CHECK_CLOSE(1.21655e-18, V[3][0], 0.1);
    BOOST_CHECK_CLOSE(0.632456, V[3][1], 0.1);
    BOOST_CHECK_CLOSE(-0.288675, V[3][2], 0.1);
    BOOST_CHECK_CLOSE(-0.598034, V[3][3], 0.1);
    BOOST_CHECK_CLOSE(-0.398776, V[3][4], 0.1);

    BOOST_CHECK_CLOSE(-0.320256, V[4][0], 0.1);
    BOOST_CHECK_CLOSE(-2.01712e-15, V[4][1], 0.1);
    BOOST_CHECK_CLOSE(0.288675, V[4][2], 0.1);
    BOOST_CHECK_CLOSE(0.399962, V[4][3], 0.1);
    BOOST_CHECK_CLOSE(-0.808785, V[4][4], 0.1);


    // Orthogonality verification of U and V

    auto I = U.transpose() * U;

    BOOST_CHECK_CLOSE(1., I[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I[2][2], 0.1);
    BOOST_CHECK_CLOSE(1., I[3][3], 0.1);
    BOOST_CHECK_CLOSE(1., I[4][4], 0.1);
    BOOST_CHECK_CLOSE(1., I[5][5], 0.1);
    BOOST_CHECK_CLOSE(1., I[6][6], 0.1);
    BOOST_CHECK_CLOSE(1., I[7][7], 0.1);

    auto I2 = V.transpose() * V;

    BOOST_CHECK_CLOSE(1., I2[0][0], 0.1);
    BOOST_CHECK_CLOSE(1., I2[1][1], 0.1);
    BOOST_CHECK_CLOSE(1., I2[2][2], 0.1);
    BOOST_CHECK_CLOSE(1., I2[3][3], 0.1);
    BOOST_CHECK_CLOSE(1., I2[4][4], 0.1);
}

BOOST_FIXTURE_TEST_CASE(solve, SvdTest)
{
    SingularValueDecomposition<Matrix<float>> svd(A);
    auto C = svd.solve(B);
    BOOST_CHECK_CLOSE(0.0653061, C[0], 0.1);
    BOOST_CHECK_CLOSE(-0.00897959, C[1], 0.1);
    BOOST_CHECK_CLOSE(-0.0604082, C[2], 0.1);

    SingularValueDecomposition<Matrix<double>> svd2(A2);
    auto C2 = svd2.solve(B2);
    BOOST_CHECK_CLOSE(-2., C2[0], 0.1);
    BOOST_CHECK_CLOSE(1., C2[1], 0.1);
    BOOST_CHECK_CLOSE(1., C2[2], 0.1);

    SingularValueDecomposition<Matrix<float, 3, 3>> svd3(SA);
    auto SC = svd3.solve(SB);
    BOOST_CHECK_CLOSE(0.065306122, SC[0], 0.1);
    BOOST_CHECK_CLOSE(-0.00897959, SC[1], 0.1);
    BOOST_CHECK_CLOSE(-0.06040816, SC[2], 0.1);

    Vector<double, 8> B8{1, 2, 3, 4, 5, 6, 7, 8};

    SingularValueDecomposition<Matrix<double, 8, 5>> svd4(A8x5, true);

    auto x = svd4.solve(B8);

    BOOST_CHECK_CLOSE(0.134591, x[0], 0.1);
    BOOST_CHECK_CLOSE(0.0589423, x[1], 0.1);
    BOOST_CHECK_CLOSE(0.130585, x[2], 0.1);
    BOOST_CHECK_CLOSE(0.00135417, x[3], 0.1);
    BOOST_CHECK_CLOSE(0.0879407, x[4], 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
