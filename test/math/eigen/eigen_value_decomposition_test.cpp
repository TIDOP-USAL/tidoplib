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
#include <tidop/math/algebra/decomp/eigen.h>
#include <tidop/math/algebra/matrix.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(EigenDecompositionTestSuite)

struct EigenDecompositionTest
{

    EigenDecompositionTest()
      : eigen_decomp(nullptr),
        eigen_decomp_1(nullptr),
        eigen_decomp_2(nullptr),
        eigen_decomp_3(nullptr) 
    {
    }

    ~EigenDecompositionTest()
    {
        delete eigen_decomp;
        delete eigen_decomp_1;
        delete eigen_decomp_2;
        delete eigen_decomp_3;
    }

    void setup()
    {
        symmetric = {{4, 1, -2},
                     {1, 3, 0},
                     {-2, 0, 2}};

        eigen_decomp = new EigenDecomposition<Matrix<double>>(symmetric);

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

        eigen_decomp_1 = new EigenDecomposition<Matrix<double>>(A);

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

        eigen_decomp_2 = new EigenDecomposition<Matrix<double>>(A2);

        SA[0][0] = 12.;
        SA[0][1] = -51.;
        SA[0][2] = 4.;
        SA[1][0] = 6.;
        SA[1][1] = 167.;
        SA[1][2] = -68.;
        SA[2][0] = -4.;
        SA[2][1] = 24.;
        SA[2][2] = -41.;

        eigen_decomp_3 = new EigenDecomposition<Matrix<double, 3, 3>>(SA);
    }

    void teardown()
    {

    }

    Matrix<double> symmetric;
    EigenDecomposition<Matrix<double>> *eigen_decomp;

    Matrix<double> A;
    EigenDecomposition<Matrix<double>> *eigen_decomp_1;
    Matrix<double> A2;
    EigenDecomposition<Matrix<double>> *eigen_decomp_2;

    Matrix<double, 3, 3> SA;
    EigenDecomposition<Matrix<double, 3, 3>> *eigen_decomp_3;

    // Matriz > 10
    // Desactivar OpenBLAS
};

BOOST_FIXTURE_TEST_CASE(compute_symmetric, EigenDecompositionTest)
{
    auto real_val = eigen_decomp->eigenvaluesReal();
    auto img_val = eigen_decomp->eigenvaluesImag();
    auto vectors = eigen_decomp->eigenvectors();

    BOOST_CHECK_CLOSE(0.638531, real_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(2.83255, real_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(5.52892, real_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.547398, vectors[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.15351, vectors[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.822673, vectors[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.231804, vectors[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.916757, vectors[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.325306, vectors[1][2], 0.1);
    BOOST_CHECK_CLOSE(0.804128, vectors[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.368771, vectors[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.466246, vectors[2][2], 0.1);

    BOOST_CHECK_CLOSE(9, real_val.sum(), 0.1);

    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);

}

BOOST_FIXTURE_TEST_CASE(test_1, EigenDecompositionTest)
{
    auto real_val = eigen_decomp_1->eigenvaluesReal();
    auto img_val = eigen_decomp_1->eigenvaluesImag();
    auto vectors = eigen_decomp_1->eigenvectors();
  
    BOOST_CHECK_CLOSE(156.137, real_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(16.06, real_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(- 34.1967, real_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.328147, vectors[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.990526, vectors[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.254758, vectors[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.936881, vectors[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.0871754, vectors[1][1], 0.1);
    BOOST_CHECK_CLOSE(0.302793, vectors[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.120717, vectors[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.106104, vectors[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.918376, vectors[2][2], 0.1);

    BOOST_CHECK_CLOSE(138, real_val.sum(), 0.1);

    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(test_2, EigenDecompositionTest)
{
    auto real_val = eigen_decomp_2->eigenvaluesReal();
    auto img_val = eigen_decomp_2->eigenvaluesImag();
    auto vectors = eigen_decomp_2->eigenvectors();
    
    BOOST_CHECK_CLOSE(16.7075, real_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(-0.90574, real_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.198247, real_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(-0.223513, vectors[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.865846, vectors[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.278296, vectors[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.503946, vectors[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.0856512, vectors[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.831847, vectors[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.834314, vectors[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.492925, vectors[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.48019, vectors[2][2], 0.1);

    BOOST_CHECK_CLOSE(16, real_val.sum(), 0.1);

    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(test_3, EigenDecompositionTest)
{
    auto real_val = eigen_decomp_3->eigenvaluesReal();
    auto img_val = eigen_decomp_3->eigenvaluesImag();
    auto vectors = eigen_decomp_3->eigenvectors();

    BOOST_CHECK_CLOSE(156.137, real_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(16.06, real_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(-34.1967, real_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.0, img_val.at(0), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(1), 0.1);
    BOOST_CHECK_CLOSE(0.0, img_val.at(2), 0.1);

    BOOST_CHECK_CLOSE(0.328147, vectors[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.990526, vectors[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.254758, vectors[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.936881, vectors[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.0871754, vectors[1][1], 0.1);
    BOOST_CHECK_CLOSE(0.302793, vectors[1][2], 0.1);
    BOOST_CHECK_CLOSE(-0.120717, vectors[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.106104, vectors[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.918376, vectors[2][2], 0.1);

    BOOST_CHECK_CLOSE(138, real_val.sum(), 0.1);

    BOOST_CHECK_CLOSE(1, vectors.col(0).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(1).module(), 0.1);
    BOOST_CHECK_CLOSE(1, vectors.col(2).module(), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
