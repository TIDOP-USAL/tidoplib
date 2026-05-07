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
 
#define BOOST_TEST_MODULE Tidop eigen test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/decomp/Eigen.h>
#include <tidop/math/algebra/matrix/Matrix.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(EigenDecompositionTestSuite)

struct EigenDecompositionTest
{

    EigenDecompositionTest()
    {
    }

    ~EigenDecompositionTest()
    {
    }

    void setup()
    {
        symmetric = {{4, 1, -2},
                     {1, 3, 0},
                     {-2, 0, 2}};

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


        SA[0][0] = 12.;
        SA[0][1] = -51.;
        SA[0][2] = 4.;
        SA[1][0] = 6.;
        SA[1][1] = 167.;
        SA[1][2] = -68.;
        SA[2][0] = -4.;
        SA[2][1] = 24.;
        SA[2][2] = -41.;

    }

    void teardown()
    {

    }

    Matrix<double> symmetric;
    Matrix<double> A;
    Matrix<double> A2;
    Matrix<double, 3, 3> SA;

    // Matriz > 10
    // Desactivar OpenBLAS
};

BOOST_FIXTURE_TEST_CASE(compute_symmetric, EigenDecompositionTest)
{
    EigenDecomposition<Matrix<double>> eigen_decomp(symmetric);

    Vector<double> real_val = eigen_decomp.eigenvaluesReal();
    Vector<double> img_val = eigen_decomp.eigenvaluesImag();
    Matrix<double> vectors = eigen_decomp.eigenvectors();

    // El orden puede variar al no usar Lapack pero no significa que este mal
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(0.638531, real_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(2.83255, real_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(5.52892, real_val.at(2), 0.1);
#endif // TL_HAVE_OPENBLAS

    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);

    // Estos valores pueden variar en función de si utiliza Lapack o no
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(0.547398, vectors[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.15351, vectors[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.822673, vectors[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.231804, vectors[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.916757, vectors[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.325306, vectors[1][2], 0.1);
    BOOST_CHECK_CLOSE(0.804128, vectors[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.368771, vectors[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.466246, vectors[2][2], 0.1);
#endif // TL_HAVE_OPENBLAS

    BOOST_CHECK_CLOSE(9, real_val.sum(), 0.1);

    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);

    // Ortogonalidad
    Matrix<double> QtQ = vectors.transpose() * vectors;
    Matrix<double> I = Matrix<double>::identity(3, 3);
    double error = (QtQ - I).frobeniusNorm();
    BOOST_CHECK(error < 1e-8);
}

BOOST_FIXTURE_TEST_CASE(test_1, EigenDecompositionTest)
{
    EigenDecomposition<Matrix<double>> eigen_decomp(A);

    Vector<double> real_val = eigen_decomp.eigenvaluesReal();
    Vector<double> img_val = eigen_decomp.eigenvaluesImag();
    Matrix<double> vectors = eigen_decomp.eigenvectors();

    BOOST_CHECK_CLOSE(156.137, real_val.at(0), 0.1);
    // Puede variar el orden
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(16.06, real_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(-34.1967, real_val.at(2), 0.1);
#endif // TL_HAVE_OPENBLAS

    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);

    // Estos valores pueden variar en función de si utiliza Lapack o no
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(0.328147, vectors[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.990526, vectors[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.254758, vectors[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.936881, vectors[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.0871754, vectors[1][1], 0.1);
    BOOST_CHECK_CLOSE(0.302793, vectors[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.120717, vectors[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.106104, vectors[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.918376, vectors[2][2], 0.1);
#endif // TL_HAVE_OPENBLAS

    BOOST_CHECK_CLOSE(138, real_val.sum(), 0.1);

    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);

    // Ortogonalidad
    // Aunque no coincidan los resultados (al usar Lapack o no) esto se tiene que cumplir
    Matrix<double> QtQ = vectors.transpose() * vectors;
    Matrix<double> I = Matrix<double>::identity(3, 3);
    double error = (QtQ - I).frobeniusNorm();
    BOOST_CHECK(error < 1e-8);
}

BOOST_FIXTURE_TEST_CASE(test_2, EigenDecompositionTest)
{
    EigenDecomposition<Matrix<double>> eigen_decomp(A2);

    Vector<double> real_val = eigen_decomp.eigenvaluesReal();
    Vector<double> img_val = eigen_decomp.eigenvaluesImag();
    Matrix<double> vectors = eigen_decomp.eigenvectors();

    BOOST_CHECK_CLOSE(16.7075, real_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(-0.90574, real_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.198247, real_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);

    // Estos valores pueden variar en función de si utiliza Lapack o no
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.223513, vectors[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.865846, vectors[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.278296, vectors[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.503946, vectors[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.0856512, vectors[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.831847, vectors[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.834314, vectors[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.492925, vectors[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.48019, vectors[2][2], 0.1);
#endif // TL_HAVE_OPENBLAS

    BOOST_CHECK_CLOSE(16, real_val.sum(), 0.1);

    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);

    // Ortogonalidad
    Matrix<double> QtQ = vectors.transpose() * vectors;
    Matrix<double> I = Matrix<double>::identity(3, 3);
    double error = (QtQ - I).frobeniusNorm();
    BOOST_CHECK(error < 1e-8);
}

BOOST_FIXTURE_TEST_CASE(test_10x10_symmetric, EigenDecompositionTest)
{
    // Crear una matriz simétrica de 10x10 (por ejemplo, una matriz diagonal con valores conocidos)
    Matrix<double, 10, 10> A;
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            A(i, j) = (i == j) ? static_cast<double>(i + 1) : 0.0;
        }
    }
    // Añadir algo de "ruido" simétrico para que no sea trivial
    for (size_t i = 0; i < 10; ++i) {
        for (size_t j = i + 1; j < 10; ++j) {
            double val = static_cast<double>(i * j) / 10.0;
            A(i, j) = val;
            A(j, i) = val;
        }
    }

    EigenDecomposition<Matrix<double, 10, 10>> eigen_decomp(A);

    Vector<double> real_vals = eigen_decomp.eigenvaluesReal();
    Vector<double> imag_vals = eigen_decomp.eigenvaluesImag();
    Matrix<double> vectors = eigen_decomp.eigenvectors();

    // 1. Verificar que los autovalores imaginarios son cero (matriz real simétrica)
    for (size_t i = 0; i < 10; ++i) {
        BOOST_CHECK_SMALL(imag_vals[i], 1e-10);
    }

    // 2. Verificar ortogonalidad de los autovectores: Q^T * Q = I
    Matrix<double> QtQ = vectors.transpose() * vectors;
    Matrix<double> I = Matrix<double>::identity(10, 10);
    double error_orth = (QtQ - I).frobeniusNorm();
    BOOST_CHECK(error_orth < 1e-8);
}

//BOOST_FIXTURE_TEST_CASE(test_3, EigenDecompositionTest)
//{
//    Vector3d real_val = eigen_decomp_3->eigenvaluesReal();
//    Vector3d img_val = eigen_decomp_3->eigenvaluesImag();
//    Matrix3x3d vectors = eigen_decomp_3->eigenvectors();
//
//    BOOST_CHECK_CLOSE(156.137, real_val.at(0), 0.1);
//    BOOST_CHECK_CLOSE(16.06, real_val.at(1), 0.1);
//    BOOST_CHECK_CLOSE(-34.1967, real_val.at(2), 0.1);
//
//    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
//    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
//    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);
//
//    BOOST_CHECK_CLOSE(0.328147, vectors[0][0], 0.1);
//    BOOST_CHECK_CLOSE(-0.990526, vectors[0][1], 0.1);
//    BOOST_CHECK_CLOSE(0.254758, vectors[0][2], 0.1);
//    BOOST_CHECK_CLOSE(-0.936881, vectors[1][0], 0.1);
//    BOOST_CHECK_CLOSE(0.0871754, vectors[1][1], 0.1);
//    BOOST_CHECK_CLOSE(0.302793, vectors[1][2], 0.1);
//    BOOST_CHECK_CLOSE(-0.120717, vectors[2][0], 0.1);
//    BOOST_CHECK_CLOSE(0.106104, vectors[2][1], 0.1);
//    BOOST_CHECK_CLOSE(0.918376, vectors[2][2], 0.1);
//
//    BOOST_CHECK_CLOSE(138, real_val.sum(), 0.1);
//
//    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
//    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
//    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);
//}

BOOST_AUTO_TEST_SUITE_END()
