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
 
#define BOOST_TEST_MODULE Tidop qr test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/decomp/QR.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(QrTestSuite)

struct QrTest
{

    QrTest()
        : qr(nullptr),
        qr2(nullptr)
    {
    }
    ~QrTest()
    {
        delete qr;
        delete qr2;
    }

    void setup()
    {
        A = Matrix<double>(3, 3);
        A.at(0, 0) = 12.;
        A.at(0, 1) = -51.;
        A.at(0, 2) = 4.;
        A.at(1, 0) = 6.;
        A.at(1, 1) = 167.;
        A.at(1, 2) = -68.;
        A.at(2, 0) = -4.;
        A.at(2, 1) = 24.;
        A.at(2, 2) = -41.;

        B = Vector<double>(3);
        B.at(0) = 1;
        B.at(1) = 3;
        B.at(2) = 2;

        qr = new QRDecomposition<Matrix<double>>(A);

        A2 = Matrix<double>(3, 3);
        A2.at(0, 0) = 1.;
        A2.at(0, 1) = 2.;
        A2.at(0, 2) = 3.;
        A2.at(1, 0) = 4.;
        A2.at(1, 1) = 5.;
        A2.at(1, 2) = 6.;
        A2.at(2, 0) = 7.;
        A2.at(2, 1) = 8.;
        A2.at(2, 2) = 10.;

        B2 = Vector<double>(3);
        B2.at(0) = 3;
        B2.at(1) = 3;
        B2.at(2) = 4;

        qr2 = new QRDecomposition<Matrix<double>>(A2);
    }

    void teardown()
    {

    }

    Matrix<double> A;
    Vector<double> B;
    QRDecomposition<Matrix<double>> *qr;
    Matrix<double> A2;
    Vector<double> B2;
    QRDecomposition<Matrix<double>> *qr2;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, QrTest)
{

    Matrix<double> q = qr->q();
    Matrix<double> r = qr->r();

    BOOST_CHECK_CLOSE(-0.85714285714285721, q.at(0, 0), 0.01);
    BOOST_CHECK_CLOSE(0.39428571428571429, q.at(0, 1), 0.01);
    BOOST_CHECK_CLOSE(0.33142857142857141, q.at(0, 2), 0.01);
    BOOST_CHECK_CLOSE(-0.42857142857142855, q.at(1, 0), 0.01);
    BOOST_CHECK_CLOSE(-0.902857142857142915, q.at(1, 1), 0.01);
    BOOST_CHECK_CLOSE(-0.034285714285714280, q.at(1, 2), 0.01);
    BOOST_CHECK_CLOSE(0.28571428571428570, q.at(2, 0), 0.01);
    BOOST_CHECK_CLOSE(-0.17142857142857137, q.at(2, 1), 0.01);
    BOOST_CHECK_CLOSE(0.94285714285714295, q.at(2, 2), 0.01);


    BOOST_CHECK_CLOSE(-14, r.at(0, 0), 0.01);
    BOOST_CHECK_CLOSE(-21, r.at(0, 1), 0.01);
    BOOST_CHECK_CLOSE(14, r.at(0, 2), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(1, 0), 0.01);
    BOOST_CHECK_CLOSE(-175, r.at(1, 1), 0.01);
    BOOST_CHECK_CLOSE(70, r.at(1, 2), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(2, 0), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(2, 1), 0.01);
    BOOST_CHECK_CLOSE(-35, r.at(2, 2), 0.01);

}

BOOST_FIXTURE_TEST_CASE(rectangular_matrix, QrTest)
{
    Matrix<double, 4, 3> A = {1, 2, 3,
                              4, 5, 6,
                              7, 8, 9,
                              10, 11, 12};

    QRDecomposition<Matrix<double, 4, 3>> qr(A);
    Matrix<double> q = qr.q();
    Matrix<double> r = qr.r();

// Sale algo diferente con LAPACK

    BOOST_CHECK_CLOSE(-0.0776151, q.at(0, 0), 0.01);
    BOOST_CHECK_CLOSE(-0.833052, q.at(0, 1), 0.01);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(0.540521, q.at(0, 2), 0.01);
    BOOST_CHECK_CLOSE(-0.088526, q.at(0, 3), 0.01);
#else
    BOOST_CHECK_CLOSE(-0.392045, q.at(0, 2), 0.01);
    BOOST_CHECK_CLOSE(-0.382492, q.at(0, 3), 0.01);
#endif // TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.31046, q.at(1, 0), 0.01);
    BOOST_CHECK_CLOSE(-0.451237, q.at(1, 1), 0.01);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.654711, q.at(1, 2), 0.01);
    BOOST_CHECK_CLOSE(0.520916, q.at(1, 3), 0.01);
#else
    BOOST_CHECK_CLOSE(0.237634, q.at(1, 2), 0.01);
    BOOST_CHECK_CLOSE(0.802203, q.at(1, 3), 0.01);
#endif // TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.543305, q.at(2, 0), 0.01);
    BOOST_CHECK_CLOSE(-0.069421, q.at(2, 1), 0.01);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.312114, q.at(2, 2), 0.01);
    BOOST_CHECK_CLOSE(-0.776252, q.at(2, 3), 0.01);
#else
    BOOST_CHECK_CLOSE(0.700868, q.at(2, 2), 0.01);
    BOOST_CHECK_CLOSE(-0.456929, q.at(2, 3), 0.01);
#endif // TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(-0.776151, q.at(3, 0), 0.01);
    BOOST_CHECK_CLOSE(0.312395, q.at(3, 1), 0.01);
#ifdef TL_HAVE_OPENBLAS
    BOOST_CHECK_CLOSE(0.426331, q.at(3, 2), 0.01);
    BOOST_CHECK_CLOSE(0.343862, q.at(3, 3), 0.01);
#else
    BOOST_CHECK_CLOSE(-0.546457, q.at(3, 2), 0.01);
    BOOST_CHECK_CLOSE(0.0372184, q.at(3, 3), 0.01);
#endif // TL_HAVE_OPENBLAS


    BOOST_CHECK_CLOSE(-12.8841, r.at(0, 0), 0.01);
    BOOST_CHECK_CLOSE(-14.5916, r.at(0, 1), 0.01);
    BOOST_CHECK_CLOSE(-16.2992, r.at(0, 2), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(1, 0), 0.01);
    BOOST_CHECK_CLOSE(-1.04132, r.at(1, 1), 0.01);
    BOOST_CHECK_CLOSE(-2.08263, r.at(1, 2), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(2, 0), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(2, 1), 0.01);
    // Valor muy pequeño que cambia de usar LAPACK a usar el código c++
    //BOOST_CHECK_CLOSE(-1.11022e-16, r.at(2, 2), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(3, 0), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(3, 1), 0.01);
    BOOST_CHECK_CLOSE(0, r.at(3, 2), 0.01);
     
    Matrix<double> qtq = q.transpose() * q;

    BOOST_CHECK_CLOSE(1., qtq.at(0, 0), 0.01);
    BOOST_CHECK_CLOSE(1., qtq.at(1, 1), 0.01);
    BOOST_CHECK_CLOSE(1., qtq.at(2, 2), 0.01);
    BOOST_CHECK_CLOSE(1., qtq.at(3, 3), 0.01);

    Matrix<double> q_r = q * r;

    BOOST_CHECK_CLOSE(1, q_r.at(0, 0), 0.01);
    BOOST_CHECK_CLOSE(2, q_r.at(0, 1), 0.01);
    BOOST_CHECK_CLOSE(3, q_r.at(0, 2), 0.01);
    BOOST_CHECK_CLOSE(4, q_r.at(1, 0), 0.01);
    BOOST_CHECK_CLOSE(5, q_r.at(1, 1), 0.01);
    BOOST_CHECK_CLOSE(6, q_r.at(1, 2), 0.01);
    BOOST_CHECK_CLOSE(7, q_r.at(2, 0), 0.01);
    BOOST_CHECK_CLOSE(8, q_r.at(2, 1), 0.01);
    BOOST_CHECK_CLOSE(9, q_r.at(2, 2), 0.01);
    BOOST_CHECK_CLOSE(10, q_r.at(3, 0), 0.01);
    BOOST_CHECK_CLOSE(11, q_r.at(3, 1), 0.01);
    BOOST_CHECK_CLOSE(12, q_r.at(3, 2), 0.01);


    //Vector<double, 4> b = {1, 2, 3, 4};
    //Vector<double, 3> x = qr.solve(b);

    //BOOST_CHECK_CLOSE(2.16667, x[0], 0.01);
    //BOOST_CHECK_CLOSE(-4.33333, x[1], 0.01);
    //BOOST_CHECK_CLOSE(2.5, x[2], 0.01);

    //[-0.0466022, 0.0932043, 0.286731]
}

BOOST_FIXTURE_TEST_CASE(solve, QrTest)
{
    Vector<double> C = qr->solve(B);
    BOOST_CHECK_CLOSE(0.065306122448979584, C.at(0), 0.01);
    BOOST_CHECK_CLOSE(-0.0089795918367346957, C.at(1), 0.01);
    BOOST_CHECK_CLOSE(-0.060408163265306125, C.at(2), 0.01);

    C = qr2->solve(B2);
    BOOST_CHECK_CLOSE(-2., C.at(0), 0.01);
    BOOST_CHECK_CLOSE(1., C.at(1), 0.01);
    BOOST_CHECK_CLOSE(1., C.at(2), 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
