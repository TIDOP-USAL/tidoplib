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

#define BOOST_TEST_MODULE Tidop matrix test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/matrix.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(MatrixTestSuite)

struct MatrixTest
{
    MatrixTest()
        : _mat_dyn_default_constructor(new Matrix<double>()),
        _mat_dyn_2x2_constructor(new Matrix<double>(2, 2)),
        _mat_dyn_2x2(new Matrix<double>(2, 2)),
        _mat_dyn_3x3_d(new Matrix<double>(3, 3)),
        _mat_dyn_4x4_d(new Matrix<double>(4, 4)),
        _mat_dyn_5x5_d(new Matrix<double>(5, 5)),
        _mat_dyn_2x3_i(new Matrix<int>(2, 3)),
        _cofactor_matrix_dyn(new Matrix<int>(3, 3))
    {
    }

    ~MatrixTest()
    {
        if (_mat_dyn_default_constructor) delete _mat_dyn_default_constructor, _mat_dyn_default_constructor = nullptr;
        if (_mat_dyn_2x2_constructor) delete _mat_dyn_2x2_constructor, _mat_dyn_2x2_constructor = nullptr;
        if (_mat_dyn_2x2) delete _mat_dyn_2x2, _mat_dyn_2x2 = nullptr;
        if (_mat_dyn_3x3_d) delete _mat_dyn_3x3_d, _mat_dyn_3x3_d = nullptr;
        if (_mat_dyn_4x4_d) delete _mat_dyn_4x4_d, _mat_dyn_4x4_d = nullptr;
        if (_mat_dyn_2x3_i) delete _mat_dyn_2x3_i, _mat_dyn_2x3_i = nullptr;
        if (_cofactor_matrix_dyn) delete _cofactor_matrix_dyn, _cofactor_matrix_dyn = nullptr;
    }

    void setup()
    {

        _mat_2x2[0][0] = 2.;
        _mat_2x2[0][1] = 3.;
        _mat_2x2[1][0] = 1.;
        _mat_2x2[1][1] = 4.;

        _mat_dyn_2x2->at(0, 0) = 2.;
        _mat_dyn_2x2->at(0, 1) = 3.;
        _mat_dyn_2x2->at(1, 0) = 1.;
        _mat_dyn_2x2->at(1, 1) = 4.;

        _mat_3x3_d[0][0] = 1.5;
        _mat_3x3_d[0][1] = 0.0;
        _mat_3x3_d[0][2] = 2.5;
        _mat_3x3_d[1][0] = 1.0;
        _mat_3x3_d[1][1] = 1.0;
        _mat_3x3_d[1][2] = 1.2;
        _mat_3x3_d[2][0] = 1.3;
        _mat_3x3_d[2][1] = 2.6;
        _mat_3x3_d[2][2] = 0.3;

        _mat_dyn_3x3_d->at(0, 0) = 1.5;
        _mat_dyn_3x3_d->at(0, 1) = 0.0;
        _mat_dyn_3x3_d->at(0, 2) = 2.5;
        _mat_dyn_3x3_d->at(1, 0) = 1.0;
        _mat_dyn_3x3_d->at(1, 1) = 1.0;
        _mat_dyn_3x3_d->at(1, 2) = 1.2;
        _mat_dyn_3x3_d->at(2, 0) = 1.3;
        _mat_dyn_3x3_d->at(2, 1) = 2.6;
        _mat_dyn_3x3_d->at(2, 2) = 0.3;

        _mat_4x4_d[0][0] = 4.5;    _mat_4x4_d[0][1] = 2.7;  _mat_4x4_d[0][2] = 5.5;  _mat_4x4_d[0][3] = 4.98;
        _mat_4x4_d[1][0] = 1.36;   _mat_4x4_d[1][1] = 7.62;	_mat_4x4_d[1][2] = 78.3; _mat_4x4_d[1][3] = 45.5;
        _mat_4x4_d[2][0] = 14.3;   _mat_4x4_d[2][1] = 45.3;	_mat_4x4_d[2][2] = 5.;   _mat_4x4_d[2][3] = 45.;
        _mat_4x4_d[3][0] = 12.374; _mat_4x4_d[3][1] = 41.6;	_mat_4x4_d[3][2] = 1.3;  _mat_4x4_d[3][3] = 12.7;

        _mat_dyn_4x4_d->at(0, 0) = 4.5;    _mat_dyn_4x4_d->at(0, 1) = 2.7;  _mat_dyn_4x4_d->at(0, 2) = 5.5;  _mat_dyn_4x4_d->at(0, 3) = 4.98;
        _mat_dyn_4x4_d->at(1, 0) = 1.36;   _mat_dyn_4x4_d->at(1, 1) = 7.62;	_mat_dyn_4x4_d->at(1, 2) = 78.3; _mat_dyn_4x4_d->at(1, 3) = 45.5;
        _mat_dyn_4x4_d->at(2, 0) = 14.3;   _mat_dyn_4x4_d->at(2, 1) = 45.3;	_mat_dyn_4x4_d->at(2, 2) = 5.;   _mat_dyn_4x4_d->at(2, 3) = 45.;
        _mat_dyn_4x4_d->at(3, 0) = 12.374; _mat_dyn_4x4_d->at(3, 1) = 41.6;	_mat_dyn_4x4_d->at(3, 2) = 1.3;  _mat_dyn_4x4_d->at(3, 3) = 12.7;

        mat_zero = Matrix<double, 3, 3>::zero();
        mat_dyn_zero = Matrix<double>::zero(3, 3);
        mat_ones = Matrix<double, 3, 3>::ones();
        mat_dyn_ones = Matrix<double>::ones(3, 3);
        mat_identity = Matrix<double, 3, 3>::identity();
        mat_dyn_identity = Matrix<double>::identity(3, 3);

        _mat_5x5_d[0][0] = 6;
        _mat_5x5_d[0][1] = 8;
        _mat_5x5_d[0][2] = 6;
        _mat_5x5_d[0][3] = 7;
        _mat_5x5_d[0][4] = 3;
        _mat_5x5_d[1][0] = 9;
        _mat_5x5_d[1][1] = 6;
        _mat_5x5_d[1][2] = 2;
        _mat_5x5_d[1][3] = 3;
        _mat_5x5_d[1][4] = 3;
        _mat_5x5_d[2][0] = 8;
        _mat_5x5_d[2][1] = 3;
        _mat_5x5_d[2][2] = 2;
        _mat_5x5_d[2][3] = 3;
        _mat_5x5_d[2][4] = 3;
        _mat_5x5_d[3][0] = 5;
        _mat_5x5_d[3][1] = 3;
        _mat_5x5_d[3][2] = 3;
        _mat_5x5_d[3][3] = 7;
        _mat_5x5_d[3][4] = 6;
        _mat_5x5_d[4][0] = 5;
        _mat_5x5_d[4][1] = 5;
        _mat_5x5_d[4][2] = 7;
        _mat_5x5_d[4][3] = 4;
        _mat_5x5_d[4][4] = 7;

        _mat_dyn_5x5_d->at(0, 0) = 6;
        _mat_dyn_5x5_d->at(0, 1) = 8;
        _mat_dyn_5x5_d->at(0, 2) = 6;
        _mat_dyn_5x5_d->at(0, 3) = 7;
        _mat_dyn_5x5_d->at(0, 4) = 3;
        _mat_dyn_5x5_d->at(1, 0) = 9;
        _mat_dyn_5x5_d->at(1, 1) = 6;
        _mat_dyn_5x5_d->at(1, 2) = 2;
        _mat_dyn_5x5_d->at(1, 3) = 3;
        _mat_dyn_5x5_d->at(1, 4) = 3;
        _mat_dyn_5x5_d->at(2, 0) = 8;
        _mat_dyn_5x5_d->at(2, 1) = 3;
        _mat_dyn_5x5_d->at(2, 2) = 2;
        _mat_dyn_5x5_d->at(2, 3) = 3;
        _mat_dyn_5x5_d->at(2, 4) = 3;
        _mat_dyn_5x5_d->at(3, 0) = 5;
        _mat_dyn_5x5_d->at(3, 1) = 3;
        _mat_dyn_5x5_d->at(3, 2) = 3;
        _mat_dyn_5x5_d->at(3, 3) = 7;
        _mat_dyn_5x5_d->at(3, 4) = 6;
        _mat_dyn_5x5_d->at(4, 0) = 5;
        _mat_dyn_5x5_d->at(4, 1) = 5;
        _mat_dyn_5x5_d->at(4, 2) = 7;
        _mat_dyn_5x5_d->at(4, 3) = 4;
        _mat_dyn_5x5_d->at(4, 4) = 7;

        _mat_2x3_i[0][0] = 6;
        _mat_2x3_i[0][1] = 8;
        _mat_2x3_i[0][2] = 6;
        _mat_2x3_i[1][0] = 9;
        _mat_2x3_i[1][1] = 6;
        _mat_2x3_i[1][2] = 2;

        _mat_dyn_2x3_i->at(0, 0) = 6;
        _mat_dyn_2x3_i->at(0, 1) = 8;
        _mat_dyn_2x3_i->at(0, 2) = 6;
        _mat_dyn_2x3_i->at(1, 0) = 9;
        _mat_dyn_2x3_i->at(1, 1) = 6;
        _mat_dyn_2x3_i->at(1, 2) = 2;


        _cofactor_matrix[0][0] = 1;
        _cofactor_matrix[0][1] = 2;
        _cofactor_matrix[0][2] = 3;
        _cofactor_matrix[1][0] = 0;
        _cofactor_matrix[1][1] = 4;
        _cofactor_matrix[1][2] = 5;
        _cofactor_matrix[2][0] = 1;
        _cofactor_matrix[2][1] = 0;
        _cofactor_matrix[2][2] = 6;

        _cofactor_matrix_dyn->at(0, 0) = 1;
        _cofactor_matrix_dyn->at(0, 1) = 2;
        _cofactor_matrix_dyn->at(0, 2) = 3;
        _cofactor_matrix_dyn->at(1, 0) = 0;
        _cofactor_matrix_dyn->at(1, 1) = 4;
        _cofactor_matrix_dyn->at(1, 2) = 5;
        _cofactor_matrix_dyn->at(2, 0) = 1;
        _cofactor_matrix_dyn->at(2, 1) = 0;
        _cofactor_matrix_dyn->at(2, 2) = 6;

        mat_singular[0][0] = 1;
        mat_singular[0][1] = 2;
        mat_singular[0][2] = 3;
        mat_singular[1][0] = 4;
        mat_singular[1][1] = 5;
        mat_singular[1][2] = 6;
        mat_singular[2][0] = 7;
        mat_singular[2][1] = 8;
        mat_singular[2][2] = 9;

        std::array<double, 10 * 10> a1 = {0.81, 7.45, 1.17, 3.44, 4.13, 5.67, 4.57, 2.03, 5.53, 5.25,
                                        6.69, 7.87, 1.70, 4.32, 6.33, 0.67, 8.99, 7.63, 7.08, 7.24,
                                        6.39, 3.54, 6.47, 7.15, 0.37, 5.52, 3.21, 8.52, 3.20, 5.83,
                                        7.86, 5.01, 5.82, 0.94, 8.94, 6.53, 2.04, 5.91, 5.94, 5.18,
                                        0.11, 3.99, 1.09, 2.89, 5.71, 6.51, 6.84, 4.33, 3.46, 8.86,
                                        0.78, 2.50, 2.20, 5.76, 4.13, 2.64, 6.41, 4.38, 3.75, 8.01,
                                        4.52, 0.05, 1.16, 2.97, 2.71, 1.97, 8.81, 3.89, 2.37, 3.86,
                                        6.65, 2.05, 8.86, 3.24, 0.83, 7.56, 8.70, 6.93, 0.93, 0.79,
                                        2.09, 4.23, 6.07, 8.07, 7.64, 0.80, 7.43, 0.15, 0.07, 7.02,
                                        2.24, 3.95, 5.79, 3.45, 3.43, 4.62, 5.88, 2.01, 3.04, 3.06};
        matrix1 = Matrix<double>(a1.data(), 10, 10);

        std::array<double, 10 * 10> a2 = {1.48, 1.19, 1.71, 7.35, 2.66, 8.25, 8.28, 3.86, 1.82, 2.69,
                                          2.19, 5.53, 2.06, 6.51, 6.39, 1.18, 8.83, 3.41, 6.19, 5.41,
                                          8.52, 4.30, 0.60, 0.11, 7.88, 2.41, 3.63, 7.20, 5.96, 5.61,
                                          6.35, 0.27, 5.50, 4.55, 2.46, 3.81, 3.20, 6.69, 5.08, 3.68,
                                          8.14, 1.13, 5.17, 0.64, 4.79, 6.49, 4.00, 1.22, 6.89, 0.20,
                                          2.10, 6.48, 5.23, 1.51, 2.13, 2.42, 7.35, 7.44, 0.24, 3.89,
                                          6.93, 8.66, 0.94, 3.01, 3.90, 7.55, 0.08, 8.03, 3.37, 7.25,
                                          2.42, 6.20, 3.22, 8.21, 7.63, 2.82, 0.79, 7.68, 7.07, 3.24,
                                          2.89, 3.61, 5.30, 2.00, 4.53, 2.41, 7.54, 0.27, 3.02, 4.24,
                                          0.58, 0.69, 5.86, 3.62, 5.66, 7.92, 2.53, 3.01, 6.05, 0.82};
        matrix2 = Matrix<double>(a2.data(), 10, 10);
    }

    void teardown()
    {

    }

    Matrix<double> *_mat_dyn_default_constructor;
    Matrix<double, 2, 2> _mat_2x2_default_constructor;
    Matrix<double> *_mat_dyn_2x2_constructor;

    Matrix<double, 2, 2> _mat_2x2;
    Matrix<double> *_mat_dyn_2x2;

    Matrix<double, 3, 3> _mat_3x3_d;
    Matrix<double> *_mat_dyn_3x3_d;

    Matrix<double, 4, 4> _mat_4x4_d;
    Matrix<double> *_mat_dyn_4x4_d;

    Matrix<double, 5, 5> _mat_5x5_d;
    Matrix<double> *_mat_dyn_5x5_d;

    //Matrix<2, 2, float> _mat_2x2_f;
    //Matrix<3, 3, float> _mat_3x3_f;
    //Matrix<4, 4, float> _mat_4x4_f;

    Matrix<int, 2, 3> _mat_2x3_i;
    Matrix<int> *_mat_dyn_2x3_i;
    Matrix<int, 3, 3> _cofactor_matrix;
    Matrix<int> *_cofactor_matrix_dyn;
    Matrix<double, 3, 3> mat_zero;
    Matrix<double> mat_dyn_zero;
    Matrix<double, 3, 3> mat_ones;
    Matrix<double> mat_dyn_ones;
    Matrix<double, 3, 3> mat_identity;
    Matrix<double> mat_dyn_identity;

    Matrix<int, 3, 3> mat_singular;

    Matrix<double> matrix1;
    Matrix<double> matrix2;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, MatrixTest)
{
    BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->rows());
    BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->cols());

    constexpr double ini_value = -std::numeric_limits<double>().max();

    for (size_t r = 0; r < _mat_2x2_default_constructor.rows(); r++) {
        for (size_t c = 0; c < _mat_2x2_default_constructor.cols(); c++) {
            BOOST_CHECK_EQUAL(static_cast<double>(ini_value), _mat_2x2_default_constructor.at(r, c));
        }
    }
    BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.rows());
    BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.cols());
}

BOOST_FIXTURE_TEST_CASE(rows_cols_constructor, MatrixTest)
{
    BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->rows());
    BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->cols());

    constexpr double ini_value = -std::numeric_limits<double>().max();

    for (size_t r = 0; r < _mat_dyn_2x2_constructor->rows(); r++) {
        for (size_t c = 0; c < _mat_dyn_2x2_constructor->cols(); c++) {
            BOOST_CHECK_EQUAL(ini_value, _mat_dyn_2x2_constructor->at(r, c));
        }
    }
}

BOOST_FIXTURE_TEST_CASE(iterator_list_constructor, MatrixTest)
{
    Matrix<int, 2, 2> it_list2{0, 1,
                                2, 3};

    BOOST_CHECK_EQUAL(0, it_list2[0][0]);
    BOOST_CHECK_EQUAL(1, it_list2[0][1]);
    BOOST_CHECK_EQUAL(2, it_list2[1][0]);
    BOOST_CHECK_EQUAL(3, it_list2[1][1]);

    Matrix<int, 2, 2> it_list3{{ 0, 1 },
                                 {2, 3}};

    BOOST_CHECK_EQUAL(0, it_list3[0][0]);
    BOOST_CHECK_EQUAL(1, it_list3[0][1]);
    BOOST_CHECK_EQUAL(2, it_list3[1][0]);
    BOOST_CHECK_EQUAL(3, it_list3[1][1]);

    Matrix<int, 2, 2> it_list4{0, 1,
                                2, 3, 4};

    BOOST_CHECK_EQUAL(0, it_list4[0][0]);
    BOOST_CHECK_EQUAL(1, it_list4[0][1]);
    BOOST_CHECK_EQUAL(2, it_list4[1][0]);
    BOOST_CHECK_EQUAL(3, it_list4[1][1]);

    Matrix<int, 2, 3> it_list2x3{{0, 1, 2},
                                   {3, 4, 5}};

    BOOST_CHECK_EQUAL(0, it_list2x3[0][0]);
    BOOST_CHECK_EQUAL(1, it_list2x3[0][1]);
    BOOST_CHECK_EQUAL(2, it_list2x3[0][2]);
    BOOST_CHECK_EQUAL(3, it_list2x3[1][0]);
    BOOST_CHECK_EQUAL(4, it_list2x3[1][1]);
    BOOST_CHECK_EQUAL(5, it_list2x3[1][2]);

    Matrix<int, 3, 3> it_list3x3{{0, 1, 2},
                                  {3, 4, 5},
                                  {6, 7, 8}};

    BOOST_CHECK_EQUAL(0, it_list3x3[0][0]);
    BOOST_CHECK_EQUAL(1, it_list3x3[0][1]);
    BOOST_CHECK_EQUAL(2, it_list3x3[0][2]);
    BOOST_CHECK_EQUAL(3, it_list3x3[1][0]);
    BOOST_CHECK_EQUAL(4, it_list3x3[1][1]);
    BOOST_CHECK_EQUAL(5, it_list3x3[1][2]);
    BOOST_CHECK_EQUAL(6, it_list3x3[2][0]);
    BOOST_CHECK_EQUAL(7, it_list3x3[2][1]);
    BOOST_CHECK_EQUAL(8, it_list3x3[2][2]);

    Matrix<int, 5, 5> it_list5{{6, 8,6,7,3},
                                {9,6,2,3,3},
                                {8,3,2,3,3},
                                {5,3,3,7,6},
                                {5,5,7,4,7}};

    BOOST_CHECK_EQUAL(6, it_list5[0][0]);
    BOOST_CHECK_EQUAL(8, it_list5[0][1]);
    BOOST_CHECK_EQUAL(6, it_list5[0][2]);
    BOOST_CHECK_EQUAL(7, it_list5[0][3]);
    BOOST_CHECK_EQUAL(3, it_list5[0][4]);
    BOOST_CHECK_EQUAL(9, it_list5[1][0]);
    BOOST_CHECK_EQUAL(6, it_list5[1][1]);
    BOOST_CHECK_EQUAL(2, it_list5[1][2]);
    BOOST_CHECK_EQUAL(3, it_list5[1][3]);


    Matrix<int> it_list3x3_d{{0, 1, 2},
                              {3, 4, 5},
                              {6, 7, 8}};

    BOOST_CHECK_EQUAL(0, it_list3x3_d[0][0]);
    BOOST_CHECK_EQUAL(1, it_list3x3_d[0][1]);
    BOOST_CHECK_EQUAL(2, it_list3x3_d[0][2]);
    BOOST_CHECK_EQUAL(3, it_list3x3_d[1][0]);
    BOOST_CHECK_EQUAL(4, it_list3x3_d[1][1]);
    BOOST_CHECK_EQUAL(5, it_list3x3_d[1][2]);
    BOOST_CHECK_EQUAL(6, it_list3x3_d[2][0]);
    BOOST_CHECK_EQUAL(7, it_list3x3_d[2][1]);
    BOOST_CHECK_EQUAL(8, it_list3x3_d[2][2]);
}

BOOST_FIXTURE_TEST_CASE(assing, MatrixTest)
{
    // Asignación de estática a dinámica

    Matrix<int, 5, 5> mat1{{6,8,6,7,3},
                            {9,6,2,3,3},
                            {8,3,2,3,3},
                            {5,3,3,7,6},
                            {5,5,7,4,7}};

    Matrix<int> mat2 = mat1;

    BOOST_CHECK_EQUAL(6, mat2[0][0]);
    BOOST_CHECK_EQUAL(8, mat2[0][1]);
    BOOST_CHECK_EQUAL(6, mat2[0][2]);
    BOOST_CHECK_EQUAL(7, mat2[0][3]);
    BOOST_CHECK_EQUAL(3, mat2[0][4]);
    BOOST_CHECK_EQUAL(9, mat2[1][0]);
    BOOST_CHECK_EQUAL(6, mat2[1][1]);
    BOOST_CHECK_EQUAL(2, mat2[1][2]);
    BOOST_CHECK_EQUAL(3, mat2[1][3]);
    BOOST_CHECK_EQUAL(3, mat2[1][4]);
    BOOST_CHECK_EQUAL(8, mat2[2][0]);
    BOOST_CHECK_EQUAL(3, mat2[2][1]);
    BOOST_CHECK_EQUAL(2, mat2[2][2]);
    BOOST_CHECK_EQUAL(3, mat2[2][3]);
    BOOST_CHECK_EQUAL(3, mat2[2][4]);
    BOOST_CHECK_EQUAL(5, mat2[3][0]);
    BOOST_CHECK_EQUAL(3, mat2[3][1]);
    BOOST_CHECK_EQUAL(3, mat2[3][2]);
    BOOST_CHECK_EQUAL(7, mat2[3][3]);
    BOOST_CHECK_EQUAL(6, mat2[3][4]);
    BOOST_CHECK_EQUAL(5, mat2[4][0]);
    BOOST_CHECK_EQUAL(5, mat2[4][1]);
    BOOST_CHECK_EQUAL(7, mat2[4][2]);
    BOOST_CHECK_EQUAL(4, mat2[4][3]);
    BOOST_CHECK_EQUAL(7, mat2[4][4]);

    // Asignación de dinámica a estática

    Matrix<int> mat3{{6,8,6,7,3},
                      {9,6,2,3,3},
                      {8,3,2,3,3},
                      {5,3,3,7,6},
                      {5,5,7,4,7}};

    Matrix<int, 5, 5> mat4 = mat3;

    BOOST_CHECK_EQUAL(6, mat4[0][0]);
    BOOST_CHECK_EQUAL(8, mat4[0][1]);
    BOOST_CHECK_EQUAL(6, mat4[0][2]);
    BOOST_CHECK_EQUAL(7, mat4[0][3]);
    BOOST_CHECK_EQUAL(3, mat4[0][4]);
    BOOST_CHECK_EQUAL(9, mat4[1][0]);
    BOOST_CHECK_EQUAL(6, mat4[1][1]);
    BOOST_CHECK_EQUAL(2, mat4[1][2]);
    BOOST_CHECK_EQUAL(3, mat4[1][3]);
    BOOST_CHECK_EQUAL(3, mat4[1][4]);
    BOOST_CHECK_EQUAL(8, mat4[2][0]);
    BOOST_CHECK_EQUAL(3, mat4[2][1]);
    BOOST_CHECK_EQUAL(2, mat4[2][2]);
    BOOST_CHECK_EQUAL(3, mat4[2][3]);
    BOOST_CHECK_EQUAL(3, mat4[2][4]);
    BOOST_CHECK_EQUAL(5, mat4[3][0]);
    BOOST_CHECK_EQUAL(3, mat4[3][1]);
    BOOST_CHECK_EQUAL(3, mat4[3][2]);
    BOOST_CHECK_EQUAL(7, mat4[3][3]);
    BOOST_CHECK_EQUAL(6, mat4[3][4]);
    BOOST_CHECK_EQUAL(5, mat4[4][0]);
    BOOST_CHECK_EQUAL(5, mat4[4][1]);
    BOOST_CHECK_EQUAL(7, mat4[4][2]);
    BOOST_CHECK_EQUAL(4, mat4[4][3]);
    BOOST_CHECK_EQUAL(7, mat4[4][4]);

    bool exception_catch = false;

    try {
        // Se lanza una excepción al intentar asignar una matriz de diferente tamaño a una matriz estática
        Matrix<int, 3, 3> mat5 = mat1;
    } catch (...) {
        exception_catch = true;
    }

    BOOST_CHECK(exception_catch);

    // redimensión de matriz dinámica
    Matrix<int> mat6(6, 6);
    mat6 = mat3;

    BOOST_CHECK_EQUAL(5, mat6.rows());
    BOOST_CHECK_EQUAL(5, mat6.cols());
    BOOST_CHECK_EQUAL(6, mat6[0][0]);
    BOOST_CHECK_EQUAL(8, mat6[0][1]);
    BOOST_CHECK_EQUAL(6, mat6[0][2]);
    BOOST_CHECK_EQUAL(7, mat6[0][3]);
    BOOST_CHECK_EQUAL(3, mat6[0][4]);
    BOOST_CHECK_EQUAL(9, mat6[1][0]);
    BOOST_CHECK_EQUAL(6, mat6[1][1]);
    BOOST_CHECK_EQUAL(2, mat6[1][2]);
    BOOST_CHECK_EQUAL(3, mat6[1][3]);
    BOOST_CHECK_EQUAL(3, mat6[1][4]);
    BOOST_CHECK_EQUAL(8, mat6[2][0]);
    BOOST_CHECK_EQUAL(3, mat6[2][1]);
    BOOST_CHECK_EQUAL(2, mat6[2][2]);
    BOOST_CHECK_EQUAL(3, mat6[2][3]);
    BOOST_CHECK_EQUAL(3, mat6[2][4]);
    BOOST_CHECK_EQUAL(5, mat6[3][0]);
    BOOST_CHECK_EQUAL(3, mat6[3][1]);
    BOOST_CHECK_EQUAL(3, mat6[3][2]);
    BOOST_CHECK_EQUAL(7, mat6[3][3]);
    BOOST_CHECK_EQUAL(6, mat6[3][4]);
    BOOST_CHECK_EQUAL(5, mat6[4][0]);
    BOOST_CHECK_EQUAL(5, mat6[4][1]);
    BOOST_CHECK_EQUAL(7, mat6[4][2]);
    BOOST_CHECK_EQUAL(4, mat6[4][3]);
    BOOST_CHECK_EQUAL(7, mat6[4][4]);

    // Bloque a matriz estática

    Matrix<int, 3, 3> mat7 = mat1.block(0, 2, 0, 2);

    BOOST_CHECK_EQUAL(6, mat7[0][0]);
    BOOST_CHECK_EQUAL(8, mat7[0][1]);
    BOOST_CHECK_EQUAL(6, mat7[0][2]);
    BOOST_CHECK_EQUAL(9, mat7[1][0]);
    BOOST_CHECK_EQUAL(6, mat7[1][1]);
    BOOST_CHECK_EQUAL(2, mat7[1][2]);
    BOOST_CHECK_EQUAL(8, mat7[2][0]);
    BOOST_CHECK_EQUAL(3, mat7[2][1]);
    BOOST_CHECK_EQUAL(2, mat7[2][2]);

    // Bloque a matriz dinámica

    Matrix<int> mat8 = mat1.block(0, 2, 0, 2);

    BOOST_CHECK_EQUAL(3, mat8.rows());
    BOOST_CHECK_EQUAL(3, mat8.cols());
    BOOST_CHECK_EQUAL(6, mat8[0][0]);
    BOOST_CHECK_EQUAL(8, mat8[0][1]);
    BOOST_CHECK_EQUAL(6, mat8[0][2]);
    BOOST_CHECK_EQUAL(9, mat8[1][0]);
    BOOST_CHECK_EQUAL(6, mat8[1][1]);
    BOOST_CHECK_EQUAL(2, mat8[1][2]);
    BOOST_CHECK_EQUAL(8, mat8[2][0]);
    BOOST_CHECK_EQUAL(3, mat8[2][1]);
    BOOST_CHECK_EQUAL(2, mat8[2][2]);

}

BOOST_FIXTURE_TEST_CASE(array_index_operator, MatrixTest)
{
    BOOST_CHECK_EQUAL(1.5, _mat_3x3_d[0][0]);
    BOOST_CHECK_EQUAL(0.0, _mat_3x3_d[0][1]);
    BOOST_CHECK_EQUAL(2.5, _mat_3x3_d[0][2]);
    BOOST_CHECK_EQUAL(1.0, _mat_3x3_d[1][0]);
    BOOST_CHECK_EQUAL(1.0, _mat_3x3_d[1][1]);
    BOOST_CHECK_EQUAL(1.2, _mat_3x3_d[1][2]);
    BOOST_CHECK_EQUAL(1.3, _mat_3x3_d[2][0]);
    BOOST_CHECK_EQUAL(2.6, _mat_3x3_d[2][1]);
    BOOST_CHECK_EQUAL(0.3, _mat_3x3_d[2][2]);
}

BOOST_FIXTURE_TEST_CASE(at, MatrixTest)
{
    BOOST_CHECK_EQUAL(1.5, _mat_dyn_3x3_d->at(0, 0));
    BOOST_CHECK_EQUAL(0.0, _mat_dyn_3x3_d->at(0, 1));
    BOOST_CHECK_EQUAL(2.5, _mat_dyn_3x3_d->at(0, 2));
    BOOST_CHECK_EQUAL(1.0, _mat_dyn_3x3_d->at(1, 0));
    BOOST_CHECK_EQUAL(1.0, _mat_dyn_3x3_d->at(1, 1));
    BOOST_CHECK_EQUAL(1.2, _mat_dyn_3x3_d->at(1, 2));
    BOOST_CHECK_EQUAL(1.3, _mat_dyn_3x3_d->at(2, 0));
    BOOST_CHECK_EQUAL(2.6, _mat_dyn_3x3_d->at(2, 1));
    BOOST_CHECK_EQUAL(0.3, _mat_dyn_3x3_d->at(2, 2));
}

BOOST_FIXTURE_TEST_CASE(rows, MatrixTest)
{
    BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->rows());
    BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.rows());
    BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->rows());
    BOOST_CHECK_EQUAL(2, _mat_dyn_2x2->rows());
    BOOST_CHECK_EQUAL(3, _mat_3x3_d.rows());
    BOOST_CHECK_EQUAL(3, _mat_dyn_3x3_d->rows());
}

BOOST_FIXTURE_TEST_CASE(cols, MatrixTest)
{
    BOOST_CHECK_EQUAL(0, _mat_dyn_default_constructor->cols());
    BOOST_CHECK_EQUAL(2, _mat_2x2_default_constructor.cols());
    BOOST_CHECK_EQUAL(2, _mat_dyn_2x2_constructor->cols());
    BOOST_CHECK_EQUAL(2, _mat_dyn_2x2->cols());
    BOOST_CHECK_EQUAL(3, _mat_3x3_d.cols());
    BOOST_CHECK_EQUAL(3, _mat_dyn_3x3_d->cols());
}


BOOST_FIXTURE_TEST_CASE(determinant2x2, MatrixTest)
{
    BOOST_CHECK_EQUAL(5., _mat_2x2.determinant());
    BOOST_CHECK_EQUAL(5., _mat_dyn_2x2->determinant());
}

BOOST_FIXTURE_TEST_CASE(determinant3x3, MatrixTest)
{
    BOOST_CHECK_CLOSE(-0.98, _mat_3x3_d.determinant(), 0.1);
    BOOST_CHECK_CLOSE(-0.98, _mat_dyn_3x3_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant4x4, MatrixTest)
{
    BOOST_CHECK_CLOSE(353100.53, _mat_4x4_d.determinant(), 0.1);
    BOOST_CHECK_CLOSE(353100.53, _mat_dyn_4x4_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinantnxn, MatrixTest)
{
    BOOST_CHECK_CLOSE(-2877.99, _mat_5x5_d.determinant(), 0.1);
    BOOST_CHECK_CLOSE(-2877.99, _mat_dyn_5x5_d->determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant_singular, MatrixTest)
{
    BOOST_CHECK_EQUAL(0, mat_singular.determinant());
}

BOOST_FIXTURE_TEST_CASE(diagonal, MatrixTest)
{
    auto _diagonal = _mat_5x5_d.diagonal();
    BOOST_CHECK_EQUAL(6, _diagonal[0]);
    BOOST_CHECK_EQUAL(6, _diagonal[1]);
    BOOST_CHECK_EQUAL(2, _diagonal[2]);
    BOOST_CHECK_EQUAL(7, _diagonal[3]);
    BOOST_CHECK_EQUAL(7, _diagonal[4]);
}

BOOST_FIXTURE_TEST_CASE(inverse2x2, MatrixTest)
{
    bool invertible;
    Matrix<double, 2, 2> inv_mat = _mat_2x2.inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(0.8, inv_mat[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.6, inv_mat[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.2, inv_mat[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.4, inv_mat[1][1], 0.1);

    Matrix<double> inv_mat_dyn = _mat_dyn_2x2->inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(0.8, inv_mat_dyn[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.6, inv_mat_dyn[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.2, inv_mat_dyn[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.4, inv_mat_dyn[1][1], 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse3x3, MatrixTest)
{
    bool invertible;
    Matrix<double, 3, 3> inv_mat = _mat_3x3_d.inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(2.877551, inv_mat[0][0], 0.1);
    BOOST_CHECK_CLOSE(-6.632653, inv_mat[0][1], 0.1);
    BOOST_CHECK_CLOSE(2.551020, inv_mat[0][2], 0.1);
    BOOST_CHECK_CLOSE(-1.285714, inv_mat[1][0], 0.1);
    BOOST_CHECK_CLOSE(2.857143, inv_mat[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.714286, inv_mat[1][2], 0.1);
    BOOST_CHECK_CLOSE(-1.326531, inv_mat[2][0], 0.1);
    BOOST_CHECK_CLOSE(3.979592, inv_mat[2][1], 0.1);
    BOOST_CHECK_CLOSE(-1.530612, inv_mat[2][2], 0.1);


    Matrix<double> inv_mat2 = _mat_dyn_3x3_d->inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(2.877551, inv_mat2[0][0], 0.1);
    BOOST_CHECK_CLOSE(-6.632653, inv_mat2[0][1], 0.1);
    BOOST_CHECK_CLOSE(2.551020, inv_mat2[0][2], 0.1);
    BOOST_CHECK_CLOSE(-1.285714, inv_mat2[1][0], 0.1);
    BOOST_CHECK_CLOSE(2.857143, inv_mat2[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.714286, inv_mat2[1][2], 0.1);
    BOOST_CHECK_CLOSE(-1.326531, inv_mat2[2][0], 0.1);
    BOOST_CHECK_CLOSE(3.979592, inv_mat2[2][1], 0.1);
    BOOST_CHECK_CLOSE(-1.530612, inv_mat2[2][2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse4x4, MatrixTest)
{
    bool invertible;
    Matrix<double, 4, 4> inv_mat = _mat_4x4_d.inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(0.268435, inv_mat[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.018133, inv_mat[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.010673, inv_mat[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.002479, inv_mat[0][3], 0.1);
    BOOST_CHECK_CLOSE(-0.077647, inv_mat[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.005298, inv_mat[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.006780, inv_mat[1][2], 0.1);
    BOOST_CHECK_CLOSE(0.035491, inv_mat[1][3], 0.1);
    BOOST_CHECK_CLOSE(0.007528, inv_mat[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.013172, inv_mat[2][1], 0.1);
    BOOST_CHECK_CLOSE(-0.019248, inv_mat[2][2], 0.1);
    BOOST_CHECK_CLOSE(0.018059, inv_mat[2][3], 0.1);
    BOOST_CHECK_CLOSE(-0.007974, inv_mat[3][0], 0.1);
    BOOST_CHECK_CLOSE(-0.001035, inv_mat[3][1], 0.1);
    BOOST_CHECK_CLOSE(0.034578, inv_mat[3][2], 0.1);
    BOOST_CHECK_CLOSE(-0.036946, inv_mat[3][3], 0.1);

    Matrix<double> inv_mat2 = _mat_dyn_4x4_d->inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(0.268435, inv_mat2[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.018133, inv_mat2[0][1], 0.1);
    BOOST_CHECK_CLOSE(-0.010673, inv_mat2[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.002479, inv_mat2[0][3], 0.1);
    BOOST_CHECK_CLOSE(-0.077647, inv_mat2[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.005298, inv_mat2[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.006780, inv_mat2[1][2], 0.1);
    BOOST_CHECK_CLOSE(0.035491, inv_mat2[1][3], 0.1);
    BOOST_CHECK_CLOSE(0.007528, inv_mat2[2][0], 0.1);
    BOOST_CHECK_CLOSE(0.013172, inv_mat2[2][1], 0.1);
    BOOST_CHECK_CLOSE(-0.019248, inv_mat2[2][2], 0.1);
    BOOST_CHECK_CLOSE(0.018059, inv_mat2[2][3], 0.1);
    BOOST_CHECK_CLOSE(-0.007974, inv_mat2[3][0], 0.1);
    BOOST_CHECK_CLOSE(-0.001035, inv_mat2[3][1], 0.1);
    BOOST_CHECK_CLOSE(0.034578, inv_mat2[3][2], 0.1);
    BOOST_CHECK_CLOSE(-0.036946, inv_mat2[3][3], 0.1);
}

BOOST_FIXTURE_TEST_CASE(inverse, MatrixTest)
{
    bool invertible;
    Matrix<double, 5, 5> inv_mat = _mat_5x5_d.inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(-0.00208479, inv_mat[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.0799166, inv_mat[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.273801, inv_mat[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.066713, inv_mat[0][3], 0.1);
    BOOST_CHECK_CLOSE(-0.0250174, inv_mat[0][4], 0.1);
    BOOST_CHECK_CLOSE(0.000694945, inv_mat[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.359972, inv_mat[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.4246, inv_mat[1][2], 0.1);
    BOOST_CHECK_CLOSE(0.0222376, inv_mat[1][3], 0.1);
    BOOST_CHECK_CLOSE(0.00833914, inv_mat[1][4], 0.1);
    BOOST_CHECK_CLOSE(0.134468, inv_mat[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.345379, inv_mat[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.339819, inv_mat[2][2], 0.1);
    BOOST_CHECK_CLOSE(-0.197012, inv_mat[2][3], 0.1);
    BOOST_CHECK_CLOSE(0.113621, inv_mat[2][4], 0.1);
    BOOST_CHECK_CLOSE(0.113621, inv_mat[3][0], 0.1);
    BOOST_CHECK_CLOSE(-0.144545, inv_mat[3][1], 0.1);
    BOOST_CHECK_CLOSE(0.0778318, inv_mat[3][2], 0.1);
    BOOST_CHECK_CLOSE(0.135858, inv_mat[3][3], 0.1);
    BOOST_CHECK_CLOSE(-0.136553, inv_mat[3][4], 0.1);
    BOOST_CHECK_CLOSE(-0.198402, inv_mat[4][0], 0.1);
    BOOST_CHECK_CLOSE(0.227936, inv_mat[4][1], 0.1);
    BOOST_CHECK_CLOSE(-0.276581, inv_mat[4][2], 0.1);
    BOOST_CHECK_CLOSE(0.151147, inv_mat[4][3], 0.1);
    BOOST_CHECK_CLOSE(0.11918, inv_mat[4][4], 0.1);


    Matrix<double> inv_mat2 = _mat_dyn_5x5_d->inverse(&invertible);
    BOOST_CHECK(invertible);
    BOOST_CHECK_CLOSE(-0.00208479, inv_mat2[0][0], 0.1);
    BOOST_CHECK_CLOSE(-0.0799166, inv_mat2[0][1], 0.1);
    BOOST_CHECK_CLOSE(0.273801, inv_mat2[0][2], 0.1);
    BOOST_CHECK_CLOSE(-0.066713, inv_mat2[0][3], 0.1);
    BOOST_CHECK_CLOSE(-0.0250174, inv_mat2[0][4], 0.1);
    BOOST_CHECK_CLOSE(0.000694945, inv_mat2[1][0], 0.1);
    BOOST_CHECK_CLOSE(0.359972, inv_mat2[1][1], 0.1);
    BOOST_CHECK_CLOSE(-0.4246, inv_mat2[1][2], 0.1);
    BOOST_CHECK_CLOSE(0.0222376, inv_mat2[1][3], 0.1);
    BOOST_CHECK_CLOSE(0.00833914, inv_mat2[1][4], 0.1);
    BOOST_CHECK_CLOSE(0.134468, inv_mat2[2][0], 0.1);
    BOOST_CHECK_CLOSE(-0.345379, inv_mat2[2][1], 0.1);
    BOOST_CHECK_CLOSE(0.339819, inv_mat2[2][2], 0.1);
    BOOST_CHECK_CLOSE(-0.197012, inv_mat2[2][3], 0.1);
    BOOST_CHECK_CLOSE(0.113621, inv_mat2[2][4], 0.1);
    BOOST_CHECK_CLOSE(0.113621, inv_mat2[3][0], 0.1);
    BOOST_CHECK_CLOSE(-0.144545, inv_mat2[3][1], 0.1);
    BOOST_CHECK_CLOSE(0.0778318, inv_mat2[3][2], 0.1);
    BOOST_CHECK_CLOSE(0.135858, inv_mat2[3][3], 0.1);
    BOOST_CHECK_CLOSE(-0.136553, inv_mat2[3][4], 0.1);
    BOOST_CHECK_CLOSE(-0.198402, inv_mat2[4][0], 0.1);
    BOOST_CHECK_CLOSE(0.227936, inv_mat2[4][1], 0.1);
    BOOST_CHECK_CLOSE(-0.276581, inv_mat2[4][2], 0.1);
    BOOST_CHECK_CLOSE(0.151147, inv_mat2[4][3], 0.1);
    BOOST_CHECK_CLOSE(0.11918, inv_mat2[4][4], 0.1);
}

BOOST_FIXTURE_TEST_CASE(transpose_squared, MatrixTest)
{
    Matrix<double, 3, 3> transp_mat = _mat_3x3_d.transpose();

    BOOST_CHECK_EQUAL(1.5, transp_mat[0][0]);
    BOOST_CHECK_EQUAL(1, transp_mat[0][1]);
    BOOST_CHECK_EQUAL(1.3, transp_mat[0][2]);
    BOOST_CHECK_EQUAL(0, transp_mat[1][0]);
    BOOST_CHECK_EQUAL(1, transp_mat[1][1]);
    BOOST_CHECK_EQUAL(2.6, transp_mat[1][2]);
    BOOST_CHECK_EQUAL(2.5, transp_mat[2][0]);
    BOOST_CHECK_EQUAL(1.2, transp_mat[2][1]);
    BOOST_CHECK_EQUAL(0.3, transp_mat[2][2]);

    Matrix<double> transp_mat2 = _mat_dyn_3x3_d->transpose();
    BOOST_CHECK_EQUAL(1.5, transp_mat2[0][0]);
    BOOST_CHECK_EQUAL(1, transp_mat2[0][1]);
    BOOST_CHECK_EQUAL(1.3, transp_mat2[0][2]);
    BOOST_CHECK_EQUAL(0, transp_mat2[1][0]);
    BOOST_CHECK_EQUAL(1, transp_mat2[1][1]);
    BOOST_CHECK_EQUAL(2.6, transp_mat2[1][2]);
    BOOST_CHECK_EQUAL(2.5, transp_mat2[2][0]);
    BOOST_CHECK_EQUAL(1.2, transp_mat2[2][1]);
    BOOST_CHECK_EQUAL(0.3, transp_mat2[2][2]);

    {
        Matrix<float, 4, 4> matrix{3.f, 6.f, -5.f, 0.f,
                                    1.f, 1.f, 2.f, 9.f,
                                    2.f, 4.f, -3.f, 1.f,
                                    3.5f, 7.5f, 8.3f, 0.5f};

        std::cout << matrix << std::endl;

        Packed<float> packed_1;
        Packed<float> packed_2;
        Packed<float> packed_3;
        Packed<float> packed_4;

        packed_1.loadUnaligned(&matrix(0, 0));
        packed_2.loadUnaligned(&matrix(1, 0));
        packed_3.loadUnaligned(&matrix(2, 0));
        packed_4.loadUnaligned(&matrix(3, 0));

        transposeMatrix4x4(packed_1, packed_2, packed_3, packed_4);

        _MM_TRANSPOSE4_PS(packed_1, packed_2, packed_3, packed_4);

        packed_1.storeUnaligned(&matrix(0, 0));
        packed_2.storeUnaligned(&matrix(1, 0));
        packed_3.storeUnaligned(&matrix(2, 0));
        packed_4.storeUnaligned(&matrix(3, 0));

        std::cout << matrix << std::endl;

        BOOST_CHECK_EQUAL(3.f, matrix[0][0]);
        BOOST_CHECK_EQUAL(1.f, matrix[0][1]);
        BOOST_CHECK_EQUAL(2.f, matrix[0][2]);
        BOOST_CHECK_EQUAL(3.5f, matrix[0][3]);
        BOOST_CHECK_EQUAL(6.f, matrix[1][0]);
        BOOST_CHECK_EQUAL(1.f, matrix[1][1]);
        BOOST_CHECK_EQUAL(4.f, matrix[1][2]);
        BOOST_CHECK_EQUAL(7.5f, matrix[1][3]);
        BOOST_CHECK_EQUAL(-5.f, matrix[2][0]);
        BOOST_CHECK_EQUAL(2.f, matrix[2][1]);
        BOOST_CHECK_EQUAL(-3.f, matrix[2][2]);
        BOOST_CHECK_EQUAL(8.3f, matrix[2][3]);
        BOOST_CHECK_EQUAL(0.f, matrix[3][0]);
        BOOST_CHECK_EQUAL(9.f, matrix[3][1]);
        BOOST_CHECK_EQUAL(1.f, matrix[3][2]);
        BOOST_CHECK_EQUAL(0.5f, matrix[3][3]);
    }

    //{
    //    Matrix<double, 4, 4> matrix{3., 6., -5., 0.,
    //                                1., 1., 2., 9.,
    //                                2., 4., -3., 1.,
    //                                3.5, 7.5, 8.3, 0.5};

    //    std::cout << matrix << std::endl;

    //    Packed<double> packed_1;
    //    Packed<double> packed_2;
    //    Packed<double> packed_3;
    //    Packed<double> packed_4;

    //    packed_1.loadUnaligned(&matrix(0, 0));
    //    packed_2.loadUnaligned(&matrix(1, 0));
    //    packed_3.loadUnaligned(&matrix(2, 0));
    //    packed_4.loadUnaligned(&matrix(3, 0));

    //    transposeMatrix4x4(packed_1, packed_2, packed_3, packed_4);

    //    packed_1.storeUnaligned(&matrix(0, 0));
    //    packed_2.storeUnaligned(&matrix(1, 0));
    //    packed_3.storeUnaligned(&matrix(2, 0));
    //    packed_4.storeUnaligned(&matrix(3, 0));

    //    std::cout << matrix << std::endl;

    //    BOOST_CHECK_EQUAL(3., matrix[0][0]);
    //    BOOST_CHECK_EQUAL(1., matrix[0][1]);
    //    BOOST_CHECK_EQUAL(2., matrix[0][2]);
    //    BOOST_CHECK_EQUAL(3.5, matrix[0][3]);
    //    BOOST_CHECK_EQUAL(6., matrix[1][0]);
    //    BOOST_CHECK_EQUAL(1., matrix[1][1]);
    //    BOOST_CHECK_EQUAL(4., matrix[1][2]);
    //    BOOST_CHECK_EQUAL(7.5, matrix[1][3]);
    //    BOOST_CHECK_EQUAL(-5., matrix[2][0]);
    //    BOOST_CHECK_EQUAL(2., matrix[2][1]);
    //    BOOST_CHECK_EQUAL(-3., matrix[2][2]);
    //    BOOST_CHECK_EQUAL(8.3, matrix[2][3]);
    //    BOOST_CHECK_EQUAL(0., matrix[3][0]);
    //    BOOST_CHECK_EQUAL(9., matrix[3][1]);
    //    BOOST_CHECK_EQUAL(1., matrix[3][2]);
    //    BOOST_CHECK_EQUAL(0.5, matrix[3][3]);
    //}
}

/// Transpuesta de matriz no cuadrada
BOOST_FIXTURE_TEST_CASE(transpose, MatrixTest)
{
    Matrix<int, 3, 2> transp_mat = _mat_2x3_i.transpose();

    BOOST_CHECK_EQUAL(6, transp_mat[0][0]);
    BOOST_CHECK_EQUAL(9, transp_mat[0][1]);
    BOOST_CHECK_EQUAL(8, transp_mat[1][0]);
    BOOST_CHECK_EQUAL(6, transp_mat[1][1]);
    BOOST_CHECK_EQUAL(6, transp_mat[2][0]);
    BOOST_CHECK_EQUAL(2, transp_mat[2][1]);

    Matrix<int> transp_mat2 = _mat_dyn_2x3_i->transpose();

    BOOST_CHECK_EQUAL(6, transp_mat2[0][0]);
    BOOST_CHECK_EQUAL(9, transp_mat2[0][1]);
    BOOST_CHECK_EQUAL(8, transp_mat2[1][0]);
    BOOST_CHECK_EQUAL(6, transp_mat2[1][1]);
    BOOST_CHECK_EQUAL(6, transp_mat2[2][0]);
    BOOST_CHECK_EQUAL(2, transp_mat2[2][1]);
}

/// http://www.mathwords.com/c/cofactor_matrix.htm
BOOST_FIXTURE_TEST_CASE(cofactor_matrix, MatrixTest)
{
  Matrix<int,3,3> cofactor_matrix = _cofactor_matrix.cofactorMatrix();

  BOOST_CHECK_EQUAL( 24, cofactor_matrix[0][0]);
  BOOST_CHECK_EQUAL(  5, cofactor_matrix[0][1]);
  BOOST_CHECK_EQUAL( -4, cofactor_matrix[0][2]);
  BOOST_CHECK_EQUAL(-12, cofactor_matrix[1][0]);
  BOOST_CHECK_EQUAL(  3, cofactor_matrix[1][1]);
  BOOST_CHECK_EQUAL(  2, cofactor_matrix[1][2]);
  BOOST_CHECK_EQUAL( -2, cofactor_matrix[2][0]);
  BOOST_CHECK_EQUAL( -5, cofactor_matrix[2][1]);
  BOOST_CHECK_EQUAL(  4, cofactor_matrix[2][2]);

  Matrix<int> cofactor_matrix_2 = _cofactor_matrix_dyn->cofactorMatrix();

  BOOST_CHECK_EQUAL( 24, cofactor_matrix_2[0][0]);
  BOOST_CHECK_EQUAL(  5, cofactor_matrix_2[0][1]);
  BOOST_CHECK_EQUAL( -4, cofactor_matrix_2[0][2]);
  BOOST_CHECK_EQUAL(-12, cofactor_matrix_2[1][0]);
  BOOST_CHECK_EQUAL(  3, cofactor_matrix_2[1][1]);
  BOOST_CHECK_EQUAL(  2, cofactor_matrix_2[1][2]);
  BOOST_CHECK_EQUAL( -2, cofactor_matrix_2[2][0]);
  BOOST_CHECK_EQUAL( -5, cofactor_matrix_2[2][1]);
  BOOST_CHECK_EQUAL(  4, cofactor_matrix_2[2][2]);
}

BOOST_FIXTURE_TEST_CASE(first_minor, MatrixTest)
{
  int first_minor = _cofactor_matrix.firstMinor(1, 1);
  BOOST_CHECK_EQUAL(3, first_minor);

  first_minor = _cofactor_matrix.firstMinor(0, 1);
  BOOST_CHECK_EQUAL(-5, first_minor);

  first_minor = _cofactor_matrix_dyn->firstMinor(1, 1);
  BOOST_CHECK_EQUAL(3, first_minor);

  first_minor = _cofactor_matrix_dyn->firstMinor(0, 1);
  BOOST_CHECK_EQUAL(-5, first_minor);
}

BOOST_FIXTURE_TEST_CASE(cofactor, MatrixTest)
{
  int cofactor = _cofactor_matrix.cofactor(1,1);
  BOOST_CHECK_EQUAL(3, cofactor);

  cofactor = _cofactor_matrix.cofactor(0, 1);
  BOOST_CHECK_EQUAL(5, cofactor);

  cofactor = _cofactor_matrix_dyn->cofactor(1,1);
  BOOST_CHECK_EQUAL(3, cofactor);

  cofactor = _cofactor_matrix_dyn->cofactor(0, 1);
  BOOST_CHECK_EQUAL(5, cofactor);
}

BOOST_FIXTURE_TEST_CASE(zero, MatrixTest)
{
  for (size_t r = 0; r < mat_zero.rows(); r++){
    for (size_t c = 0; c < mat_zero.cols(); c++){
      BOOST_CHECK_EQUAL(0, mat_zero.at(r, c));
    }
  }

  for (size_t r = 0; r < mat_dyn_zero.rows(); r++){
    for (size_t c = 0; c < mat_dyn_zero.cols(); c++){
      BOOST_CHECK_EQUAL(0, mat_dyn_zero.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(ones, MatrixTest)
{
  for (size_t r = 0; r < mat_ones.rows(); r++){
    for (size_t c = 0; c < mat_ones.cols(); c++){
      BOOST_CHECK_EQUAL(1, mat_ones.at(r, c));
    }
  }

  for (size_t r = 0; r < mat_dyn_ones.rows(); r++){
    for (size_t c = 0; c < mat_dyn_ones.cols(); c++){
      BOOST_CHECK_EQUAL(1, mat_dyn_ones.at(r, c));
    }
  }
}

BOOST_FIXTURE_TEST_CASE(identity, MatrixTest)
{
  for (size_t r = 0; r < mat_identity.rows(); r++){
    for (size_t c = 0; c < mat_identity.cols(); c++){
      if (r == c) {
        BOOST_CHECK_EQUAL(1, mat_identity.at(r, c));
      } else {
        BOOST_CHECK_EQUAL(0, mat_identity.at(r, c));
      }
    }
  }

  for (size_t r = 0; r < mat_dyn_identity.rows(); r++){
    for (size_t c = 0; c < mat_dyn_identity.cols(); c++){
      if (r == c) {
        BOOST_CHECK_EQUAL(1, mat_dyn_identity.at(r, c));
      } else {
        BOOST_CHECK_EQUAL(0, mat_dyn_identity.at(r, c));
      }
    }
  }
}

BOOST_FIXTURE_TEST_CASE(randon, MatrixTest)
{
  {
    Matrix<float, 3, 3> randon_matrix = Matrix<float, 3, 3>::randon();

    for (size_t r = 0; r < randon_matrix.rows(); r++) {
      for (size_t c = 0; c < randon_matrix.cols(); c++) {
        BOOST_TEST(0.f <= randon_matrix[r][c]);
        BOOST_TEST(99.f >= randon_matrix[r][c]);
      }
    }
  }

  {
    Matrix<int> randon_matrix = Matrix<int>::randon(3,3);

    for (size_t r = 0; r < randon_matrix.rows(); r++) {
      for (size_t c = 0; c < randon_matrix.cols(); c++) {
        BOOST_TEST(0 <= randon_matrix[r][c]);
        BOOST_TEST(99 >= randon_matrix[r][c]);
      }
    }
  }
}

BOOST_FIXTURE_TEST_CASE(rowEchelonForm, MatrixTest)
{
  Matrix<float, 3, 4> matrix{ 3.f, 6.f, -5.f, 0.f,
                              1.f, 1.f, 2.f, 9.f,
                              2.f, 4.f, -3.f, 1.f };

  Matrix<float, 3, 4> ref = matrix.rowEchelonForm();

  BOOST_CHECK_EQUAL(1.f, ref[0][0]);
  BOOST_CHECK_EQUAL(2.f, ref[0][1]);
  BOOST_CHECK_CLOSE(-1.66667f, ref[0][2], 0.01);
  BOOST_CHECK_EQUAL(0.f, ref[0][3]);
  BOOST_CHECK_EQUAL(0.f, ref[1][0]);
  BOOST_CHECK_EQUAL(1.f, ref[1][1]);
  BOOST_CHECK_CLOSE(-11/3.f, ref[1][2], 0.01);
  BOOST_CHECK_EQUAL(-9.f, ref[1][3]);
  BOOST_CHECK_EQUAL(0.f, ref[2][0]);
  BOOST_CHECK_EQUAL(0.f, ref[2][1]);
  BOOST_CHECK_EQUAL(1.f, ref[2][2]);
  BOOST_CHECK_CLOSE(3.f, ref[2][3], 0.01);

  Matrix<float, 3, 3> matrix2{1.f, 2.f, 3.f,
                              0.f, 0.f, 1.f,
                              0.f, 0.f, 2.f};
  auto ref2 = matrix2.rowEchelonForm();
  BOOST_CHECK_EQUAL(1.f, ref2[0][0]);
  BOOST_CHECK_EQUAL(2.f, ref2[0][1]);
  BOOST_CHECK_EQUAL(3.f, ref2[0][2]);
  BOOST_CHECK_EQUAL(0.f, ref2[1][0]);
  BOOST_CHECK_EQUAL(0.f, ref2[1][1]);
  BOOST_CHECK_EQUAL(1.f, ref2[1][2]);
  BOOST_CHECK_EQUAL(0.f, ref2[2][0]);
  BOOST_CHECK_EQUAL(0.f, ref2[2][1]);
  BOOST_CHECK_EQUAL(0.f, ref2[2][2]);

}

BOOST_FIXTURE_TEST_CASE(reducedRowEchelonForm, MatrixTest)
{
  Matrix<float, 3, 4> matrix{ 3.f, 6.f, -5.f, 0.f,
                              1.f, 1.f, 2.f, 9.f,
                              2.f, 4.f, -3.f, 1.f };

  Matrix<float, 3, 4> ref = matrix.reducedRowEchelonForm();

  BOOST_CHECK_CLOSE(1.f, ref[0][0], 0.01);
  BOOST_CHECK_CLOSE(0.f, ref[0][1], 0.01);
  BOOST_CHECK_CLOSE(0.f, ref[0][2], 0.01);
  BOOST_CHECK_CLOSE(1.f, ref[0][3], 0.01);
  BOOST_CHECK_CLOSE(0.f, ref[1][0], 0.01);
  BOOST_CHECK_CLOSE(1.f, ref[1][1], 0.01);
  BOOST_CHECK_CLOSE(0.f, ref[1][2], 0.01);
  BOOST_CHECK_CLOSE(2.f, ref[1][3], 0.01);
  BOOST_CHECK_CLOSE(0.f, ref[2][0], 0.01);
  BOOST_CHECK_CLOSE(0.f, ref[2][1], 0.01);
  BOOST_CHECK_CLOSE(1.f, ref[2][2], 0.01);
  BOOST_CHECK_CLOSE(3.f, ref[2][3], 0.01);


  Matrix<float, 3, 3> matrix2{1.f, 2.f, 3.f,
                              0.f, 0.f, 1.f,
                              0.f, 0.f, 2.f};

  auto ref2 = matrix2.reducedRowEchelonForm();
  BOOST_CHECK_EQUAL(1.f, ref2[0][0]);
  BOOST_CHECK_EQUAL(2.f, ref2[0][1]);
  BOOST_CHECK_EQUAL(0.f, ref2[0][2]);
  BOOST_CHECK_EQUAL(0.f, ref2[1][0]);
  BOOST_CHECK_EQUAL(0.f, ref2[1][1]);
  BOOST_CHECK_EQUAL(1.f, ref2[1][2]);
  BOOST_CHECK_EQUAL(0.f, ref2[2][0]);
  BOOST_CHECK_EQUAL(0.f, ref2[2][1]);
  BOOST_CHECK_EQUAL(0.f, ref2[2][2]);

}

BOOST_FIXTURE_TEST_CASE(rank, MatrixTest)
{
  Matrix<float, 3, 4> matrix{3.f, 6.f, -5.f, 0.f,
                             1.f, 1.f, 2.f, 9.f,
                             2.f, 4.f, -3.f, 1.f};

  int rank = matrix.rank();
  BOOST_CHECK_EQUAL(3, rank);

  Matrix<float, 3, 3> matrix2{1, 2, 3,
                              0, 0, 1,
                              0, 0, 2};

  rank = matrix2.rank();
  BOOST_CHECK_EQUAL(2, rank);

}

BOOST_FIXTURE_TEST_CASE(trace, MatrixTest)
{
  BOOST_CHECK_EQUAL(6, _mat_2x2.trace());
  BOOST_CHECK_CLOSE(2.7999, _mat_3x3_d.trace(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(invertible, MatrixTest)
{
  BOOST_CHECK(_mat_2x2.invertible());
}

BOOST_FIXTURE_TEST_CASE(singular, MatrixTest)
{
  BOOST_CHECK(mat_singular.singular());
}

BOOST_FIXTURE_TEST_CASE(adjoint2x2, MatrixTest)
{
  Matrix<double, 2, 2> adjoint2x2 = _mat_2x2.adjugate();

  BOOST_CHECK_CLOSE(4., adjoint2x2[0][0], 0.1);
  BOOST_CHECK_CLOSE(-3., adjoint2x2[0][1], 0.1);
  BOOST_CHECK_CLOSE(-1., adjoint2x2[1][0], 0.1);
  BOOST_CHECK_CLOSE(2., adjoint2x2[1][1], 0.1);

  Matrix<double> adjoint2x2_dyn = _mat_dyn_2x2->adjugate();
  BOOST_CHECK_CLOSE(4., adjoint2x2_dyn[0][0], 0.1);
  BOOST_CHECK_CLOSE(-3., adjoint2x2_dyn[0][1], 0.1);
  BOOST_CHECK_CLOSE(-1., adjoint2x2_dyn[1][0], 0.1);
  BOOST_CHECK_CLOSE(2., adjoint2x2_dyn[1][1], 0.1);
}

BOOST_FIXTURE_TEST_CASE(adjoint3x3, MatrixTest)
{
  Matrix<double, 3, 3> adjoint3x3 = _mat_3x3_d.adjugate();
  BOOST_CHECK_CLOSE(-2.82, adjoint3x3[0][0], 0.1);
  BOOST_CHECK_CLOSE(6.5, adjoint3x3[0][1], 0.1);
  BOOST_CHECK_CLOSE(-2.5, adjoint3x3[0][2], 0.1);
  BOOST_CHECK_CLOSE(1.26, adjoint3x3[1][0], 0.1);
  BOOST_CHECK_CLOSE(-2.8, adjoint3x3[1][1], 0.1);
  BOOST_CHECK_CLOSE(0.7, adjoint3x3[1][2], 0.1);
  BOOST_CHECK_CLOSE(1.3, adjoint3x3[2][0], 0.1);
  BOOST_CHECK_CLOSE(-3.9, adjoint3x3[2][1], 0.1);
  BOOST_CHECK_CLOSE(1.5, adjoint3x3[2][2], 0.1);

  Matrix<double> adjoint3x3_dyn = _mat_dyn_3x3_d->adjugate();
  BOOST_CHECK_CLOSE(-2.82, adjoint3x3_dyn[0][0], 0.1);
  BOOST_CHECK_CLOSE(6.5, adjoint3x3_dyn[0][1], 0.1);
  BOOST_CHECK_CLOSE(-2.5, adjoint3x3_dyn[0][2], 0.1);
  BOOST_CHECK_CLOSE(1.26, adjoint3x3_dyn[1][0], 0.1);
  BOOST_CHECK_CLOSE(-2.8, adjoint3x3_dyn[1][1], 0.1);
  BOOST_CHECK_CLOSE(0.7, adjoint3x3_dyn[1][2], 0.1);
  BOOST_CHECK_CLOSE(1.3, adjoint3x3_dyn[2][0], 0.1);
  BOOST_CHECK_CLOSE(-3.9, adjoint3x3_dyn[2][1], 0.1);
  BOOST_CHECK_CLOSE(1.5, adjoint3x3_dyn[2][2], 0.1);
}

BOOST_FIXTURE_TEST_CASE(adjoint4x4, MatrixTest)
{
  Matrix<double, 4, 4> adjoint4x4 = _mat_4x4_d.adjugate();
  BOOST_CHECK_CLOSE(94784.4, adjoint4x4[0][0], 0.1);
  BOOST_CHECK_CLOSE(-6402.73, adjoint4x4[0][1], 0.1);
  BOOST_CHECK_CLOSE(-3768.58, adjoint4x4[0][2], 0.1);
  BOOST_CHECK_CLOSE(-875.303, adjoint4x4[0][3], 0.1);
  BOOST_CHECK_CLOSE(-27417.3, adjoint4x4[1][0], 0.1);
  BOOST_CHECK_CLOSE(1870.688, adjoint4x4[1][1], 0.1);
  BOOST_CHECK_CLOSE(-2394.03, adjoint4x4[1][2], 0.1);
  BOOST_CHECK_CLOSE(12531.8, adjoint4x4[1][3], 0.1);
  BOOST_CHECK_CLOSE(2658.05, adjoint4x4[2][0], 0.1);
  BOOST_CHECK_CLOSE(4651.01, adjoint4x4[2][1], 0.1);
  BOOST_CHECK_CLOSE(-6796.42, adjoint4x4[2][2], 0.1);
  BOOST_CHECK_CLOSE(6376.45, adjoint4x4[2][3], 0.1);
  BOOST_CHECK_CLOSE(-2815.65, adjoint4x4[3][0], 0.1);
  BOOST_CHECK_CLOSE(-365.281, adjoint4x4[3][1], 0.1);
  BOOST_CHECK_CLOSE(12209.4, adjoint4x4[3][2], 0.1);
  BOOST_CHECK_CLOSE(-13045.7, adjoint4x4[3][3], 0.1);

  Matrix<double> adjoint4x4_dyn = _mat_dyn_4x4_d->adjugate();
  BOOST_CHECK_CLOSE(94784.4, adjoint4x4_dyn[0][0], 0.1);
  BOOST_CHECK_CLOSE(-6402.73, adjoint4x4_dyn[0][1], 0.1);
  BOOST_CHECK_CLOSE(-3768.58, adjoint4x4_dyn[0][2], 0.1);
  BOOST_CHECK_CLOSE(-875.303, adjoint4x4_dyn[0][3], 0.1);
  BOOST_CHECK_CLOSE(-27417.3, adjoint4x4_dyn[1][0], 0.1);
  BOOST_CHECK_CLOSE(1870.688, adjoint4x4_dyn[1][1], 0.1);
  BOOST_CHECK_CLOSE(-2394.03, adjoint4x4_dyn[1][2], 0.1);
  BOOST_CHECK_CLOSE(12531.8, adjoint4x4_dyn[1][3], 0.1);
  BOOST_CHECK_CLOSE(2658.05, adjoint4x4_dyn[2][0], 0.1);
  BOOST_CHECK_CLOSE(4651.01, adjoint4x4_dyn[2][1], 0.1);
  BOOST_CHECK_CLOSE(-6796.42, adjoint4x4_dyn[2][2], 0.1);
  BOOST_CHECK_CLOSE(6376.45, adjoint4x4_dyn[2][3], 0.1);
  BOOST_CHECK_CLOSE(-2815.65, adjoint4x4_dyn[3][0], 0.1);
  BOOST_CHECK_CLOSE(-365.281, adjoint4x4_dyn[3][1], 0.1);
  BOOST_CHECK_CLOSE(12209.4, adjoint4x4_dyn[3][2], 0.1);
  BOOST_CHECK_CLOSE(-13045.7, adjoint4x4_dyn[3][3], 0.1);
}

BOOST_FIXTURE_TEST_CASE(adjointnxn, MatrixTest)
{
  Matrix<double, 5, 5> adjointnxn = _mat_5x5_d.adjugate();
  BOOST_CHECK_CLOSE(6, adjointnxn[0][0], 0.1);
  BOOST_CHECK_CLOSE(230, adjointnxn[0][1], 0.1);
  BOOST_CHECK_CLOSE(-788, adjointnxn[0][2], 0.1);
  BOOST_CHECK_CLOSE(192, adjointnxn[0][3], 0.1);
  BOOST_CHECK_CLOSE(72, adjointnxn[0][4], 0.1);
  BOOST_CHECK_CLOSE(-2, adjointnxn[1][0], 0.1);
  BOOST_CHECK_CLOSE(-1036, adjointnxn[1][1], 0.1);
  BOOST_CHECK_CLOSE(1222, adjointnxn[1][2], 0.1);
  BOOST_CHECK_CLOSE(-64, adjointnxn[1][3], 0.1);
  BOOST_CHECK_CLOSE(-24, adjointnxn[1][4], 0.1);
  BOOST_CHECK_CLOSE(-387, adjointnxn[2][0], 0.1);
  BOOST_CHECK_CLOSE(994, adjointnxn[2][1], 0.1);
  BOOST_CHECK_CLOSE(-978, adjointnxn[2][2], 0.1);
  BOOST_CHECK_CLOSE(567, adjointnxn[2][3], 0.1);
  BOOST_CHECK_CLOSE(-327, adjointnxn[2][4], 0.1);
  BOOST_CHECK_CLOSE(-327, adjointnxn[3][0], 0.1);
  BOOST_CHECK_CLOSE(416, adjointnxn[3][1], 0.1);
  BOOST_CHECK_CLOSE(-224, adjointnxn[3][2], 0.1);
  BOOST_CHECK_CLOSE(-391, adjointnxn[3][3], 0.1);
  BOOST_CHECK_CLOSE(393, adjointnxn[3][4], 0.1);
  BOOST_CHECK_CLOSE(571, adjointnxn[4][0], 0.1);
  BOOST_CHECK_CLOSE(-656, adjointnxn[4][1], 0.1);
  BOOST_CHECK_CLOSE(796, adjointnxn[4][2], 0.1);
  BOOST_CHECK_CLOSE(-435, adjointnxn[4][3], 0.1);
  BOOST_CHECK_CLOSE(-343, adjointnxn[4][4], 0.1);

  Matrix<double> adjointnxn_dyn = _mat_dyn_5x5_d->adjugate();
  BOOST_CHECK_CLOSE(6, adjointnxn_dyn[0][0], 0.1);
  BOOST_CHECK_CLOSE(230, adjointnxn_dyn[0][1], 0.1);
  BOOST_CHECK_CLOSE(-788, adjointnxn_dyn[0][2], 0.1);
  BOOST_CHECK_CLOSE(192, adjointnxn_dyn[0][3], 0.1);
  BOOST_CHECK_CLOSE(72, adjointnxn_dyn[0][4], 0.1);
  BOOST_CHECK_CLOSE(-2, adjointnxn_dyn[1][0], 0.1);
  BOOST_CHECK_CLOSE(-1036, adjointnxn_dyn[1][1], 0.1);
  BOOST_CHECK_CLOSE(1222, adjointnxn_dyn[1][2], 0.1);
  BOOST_CHECK_CLOSE(-64, adjointnxn_dyn[1][3], 0.1);
  BOOST_CHECK_CLOSE(-24, adjointnxn_dyn[1][4], 0.1);
  BOOST_CHECK_CLOSE(-387, adjointnxn_dyn[2][0], 0.1);
  BOOST_CHECK_CLOSE(994, adjointnxn_dyn[2][1], 0.1);
  BOOST_CHECK_CLOSE(-978, adjointnxn_dyn[2][2], 0.1);
  BOOST_CHECK_CLOSE(567, adjointnxn_dyn[2][3], 0.1);
  BOOST_CHECK_CLOSE(-327, adjointnxn_dyn[2][4], 0.1);
  BOOST_CHECK_CLOSE(-327, adjointnxn_dyn[3][0], 0.1);
  BOOST_CHECK_CLOSE(416, adjointnxn_dyn[3][1], 0.1);
  BOOST_CHECK_CLOSE(-224, adjointnxn_dyn[3][2], 0.1);
  BOOST_CHECK_CLOSE(-391, adjointnxn_dyn[3][3], 0.1);
  BOOST_CHECK_CLOSE(393, adjointnxn_dyn[3][4], 0.1);
  BOOST_CHECK_CLOSE(571, adjointnxn_dyn[4][0], 0.1);
  BOOST_CHECK_CLOSE(-656, adjointnxn_dyn[4][1], 0.1);
  BOOST_CHECK_CLOSE(796, adjointnxn_dyn[4][2], 0.1);
  BOOST_CHECK_CLOSE(-435, adjointnxn_dyn[4][3], 0.1);
  BOOST_CHECK_CLOSE(-343, adjointnxn_dyn[4][4], 0.1);

}

/* Unary arithmetic operators */

BOOST_FIXTURE_TEST_CASE(unary_plus, MatrixTest)
{
  Matrix<double,3,3> mat = +_mat_3x3_d;
  for (size_t r = 0; r < mat.rows(); r++){
    for (size_t c = 0; c < mat.cols(); c++){
      BOOST_CHECK_EQUAL(mat.at(r,c), _mat_3x3_d.at(r, c));
    }
  }

  Matrix<double> mat2 = +(*_mat_dyn_3x3_d);
  for (size_t r = 0; r < mat2.rows(); r++){
    for (size_t c = 0; c < mat2.cols(); c++){
      BOOST_CHECK_EQUAL(mat2.at(r,c), _mat_dyn_3x3_d->at(r, c));
    }
  }

  /// R Value move
  Matrix<int, 3, 3> mat3 = +Matrix<int, 3, 3>::ones();
  for (size_t r = 0; r < mat3.rows(); r++) {
    for (size_t c = 0; c < mat3.cols(); c++) {
      BOOST_CHECK_EQUAL(mat3.at(r, c), 1);
    }
  }

}

BOOST_FIXTURE_TEST_CASE(unary_minus, MatrixTest)
{
  Matrix<double,3,3> mat = -_mat_3x3_d;
  for (size_t r = 0; r < mat.rows(); r++){
    for (size_t c = 0; c < mat.cols(); c++){
      BOOST_CHECK_EQUAL(-mat.at(r, c), _mat_3x3_d.at(r, c));
    }
  }

  Matrix<double> mat2 = -(*_mat_dyn_3x3_d);
  for (size_t r = 0; r < mat2.rows(); r++){
    for (size_t c = 0; c < mat2.cols(); c++){
      BOOST_CHECK_EQUAL(-mat2.at(r,c), _mat_dyn_3x3_d->at(r, c));
    }
  }

  /// R Value move
  Matrix<int, 3, 3> mat3 = -Matrix<int, 3, 3>::ones();
  for (size_t r = 0; r < mat3.rows(); r++) {
    for (size_t c = 0; c < mat3.cols(); c++) {
      BOOST_CHECK_EQUAL(mat3.at(r, c), -1);
    }
  }

  // Prueba de que da error de compilación con unsigned
  //Matrix<unsigned int, 2, 2> mat3 = {1, 2,
  //                                   3, 4};

  //Matrix<unsigned int, 2, 2> mat4 = -mat3; // error C2338: Requires signed type
}

/* Operaciones binarias entre matrices */

/// Suma o adición de matrices


BOOST_FIXTURE_TEST_CASE(addition, MatrixTest)
{

  Matrix<double,3,3> mat = mat_ones + _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat[0][0]);
  BOOST_CHECK_EQUAL(1.0, mat[0][1]);
  BOOST_CHECK_EQUAL(3.5, mat[0][2]);
  BOOST_CHECK_EQUAL(2.0, mat[1][0]);
  BOOST_CHECK_EQUAL(2.0, mat[1][1]);
  BOOST_CHECK_EQUAL(2.2, mat[1][2]);
  BOOST_CHECK_EQUAL(2.3, mat[2][0]);
  BOOST_CHECK_EQUAL(3.6, mat[2][1]);
  BOOST_CHECK_EQUAL(1.3, mat[2][2]);

  mat_ones += _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat_ones[0][0]);
  BOOST_CHECK_EQUAL(1.0, mat_ones[0][1]);
  BOOST_CHECK_EQUAL(3.5, mat_ones[0][2]);
  BOOST_CHECK_EQUAL(2.0, mat_ones[1][0]);
  BOOST_CHECK_EQUAL(2.0, mat_ones[1][1]);
  BOOST_CHECK_EQUAL(2.2, mat_ones[1][2]);
  BOOST_CHECK_EQUAL(2.3, mat_ones[2][0]);
  BOOST_CHECK_EQUAL(3.6, mat_ones[2][1]);
  BOOST_CHECK_EQUAL(1.3, mat_ones[2][2]);

  Matrix<double> mat2 = mat_dyn_ones + (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(2.5, mat2[0][0]);
  BOOST_CHECK_EQUAL(1.0, mat2[0][1]);
  BOOST_CHECK_EQUAL(3.5, mat2[0][2]);
  BOOST_CHECK_EQUAL(2.0, mat2[1][0]);
  BOOST_CHECK_EQUAL(2.0, mat2[1][1]);
  BOOST_CHECK_EQUAL(2.2, mat2[1][2]);
  BOOST_CHECK_EQUAL(2.3, mat2[2][0]);
  BOOST_CHECK_EQUAL(3.6, mat2[2][1]);
  BOOST_CHECK_EQUAL(1.3, mat2[2][2]);

  mat_dyn_ones += (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(2.5, mat_dyn_ones[0][0]);
  BOOST_CHECK_EQUAL(1.0, mat_dyn_ones[0][1]);
  BOOST_CHECK_EQUAL(3.5, mat_dyn_ones[0][2]);
  BOOST_CHECK_EQUAL(2.0, mat_dyn_ones[1][0]);
  BOOST_CHECK_EQUAL(2.0, mat_dyn_ones[1][1]);
  BOOST_CHECK_EQUAL(2.2, mat_dyn_ones[1][2]);
  BOOST_CHECK_EQUAL(2.3, mat_dyn_ones[2][0]);
  BOOST_CHECK_EQUAL(3.6, mat_dyn_ones[2][1]);
  BOOST_CHECK_EQUAL(1.3, mat_dyn_ones[2][2]);

  Matrix<double> mat3 = matrix1 + matrix2;

  BOOST_CHECK_CLOSE(mat3[0][0], 2.29, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][1], 8.64, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][2], 2.88, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][3], 10.79, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][4], 6.79, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][5], 13.92, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][6], 12.85, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][7], 5.89, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][8], 7.35, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][9], 7.94, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][0], 8.88, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][1], 13.4, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][2], 3.76, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][3], 10.83, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][4], 12.72, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][5], 1.85, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][6], 17.82, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][7], 11.04, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][8], 13.27, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][9], 12.65, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][0], 14.91, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][1], 7.84, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][2], 7.07, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][3], 7.26, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][4], 8.25, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][5], 7.93, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][6], 6.84, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][7], 15.72, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][8], 9.16, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][9], 11.44, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][0], 14.21, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][1], 5.28, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][2], 11.32, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][3], 5.49, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][4], 11.4, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][5], 10.34, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][6], 5.24, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][7], 12.6, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][8], 11.02, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][9], 8.86, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][0], 8.25, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][1], 5.12, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][2], 6.26, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][3], 3.53, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][4], 10.5, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][5], 13, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][6], 10.84, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][7], 5.55, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][8], 10.35, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][9], 9.06, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][0], 2.88, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][1], 8.98, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][2], 7.43, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][3], 7.27, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][4], 6.26, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][5], 5.06, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][6], 13.76, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][7], 11.82, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][8], 3.99, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][9], 11.9, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][0], 11.45, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][1], 8.71, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][2], 2.1, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][3], 5.98, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][4], 6.61, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][5], 9.52, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][6], 8.89, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][7], 11.92, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][8], 5.74, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][9], 11.11, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][0], 9.07, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][1], 8.25, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][2], 12.08, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][3], 11.45, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][4], 8.46, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][5], 10.38, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][6], 9.49, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][7], 14.61, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][8], 8, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][9], 4.03, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][0], 4.98, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][1], 7.84, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][2], 11.37, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][3], 10.07, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][4], 12.17, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][5], 3.21, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][6], 14.97, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][7], 0.42, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][8], 3.09, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][9], 11.26, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][0], 2.82, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][1], 4.64, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][2], 11.65, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][3], 7.07, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][4], 9.09, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][5], 12.54, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][6], 8.41, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][7], 5.02, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][8], 9.09, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][9], 3.88, 0.01);


  Matrix<double, 3, 3> mat4 = Matrix<double, 3, 3>::ones() + mat_identity;
  BOOST_CHECK_EQUAL(2., mat4[0][0]);
  BOOST_CHECK_EQUAL(1., mat4[0][1]);
  BOOST_CHECK_EQUAL(1., mat4[0][2]);
  BOOST_CHECK_EQUAL(1., mat4[1][0]);
  BOOST_CHECK_EQUAL(2., mat4[1][1]);
  BOOST_CHECK_EQUAL(1., mat4[1][2]);
  BOOST_CHECK_EQUAL(1., mat4[2][0]);
  BOOST_CHECK_EQUAL(1., mat4[2][1]);
  BOOST_CHECK_EQUAL(2., mat4[2][2]);

  auto ones = Matrix<double, 3, 3>::ones();
  mat4 = ones + Matrix<double, 3, 3>::identity();
  BOOST_CHECK_EQUAL(2., mat4[0][0]);
  BOOST_CHECK_EQUAL(1., mat4[0][1]);
  BOOST_CHECK_EQUAL(1., mat4[0][2]);
  BOOST_CHECK_EQUAL(1., mat4[1][0]);
  BOOST_CHECK_EQUAL(2., mat4[1][1]);
  BOOST_CHECK_EQUAL(1., mat4[1][2]);
  BOOST_CHECK_EQUAL(1., mat4[2][0]);
  BOOST_CHECK_EQUAL(1., mat4[2][1]);
  BOOST_CHECK_EQUAL(2., mat4[2][2]);

  mat4 = Matrix<double, 3, 3>::ones() + Matrix<double, 3, 3>::identity();
  BOOST_CHECK_EQUAL(2., mat4[0][0]);
  BOOST_CHECK_EQUAL(1., mat4[0][1]);
  BOOST_CHECK_EQUAL(1., mat4[0][2]);
  BOOST_CHECK_EQUAL(1., mat4[1][0]);
  BOOST_CHECK_EQUAL(2., mat4[1][1]);
  BOOST_CHECK_EQUAL(1., mat4[1][2]);
  BOOST_CHECK_EQUAL(1., mat4[2][0]);
  BOOST_CHECK_EQUAL(1., mat4[2][1]);
  BOOST_CHECK_EQUAL(2., mat4[2][2]);

  // Tipos de matriz diferentes
  // Por ahora es muy poco eficiente ya que convierte de tipo de matriz en lugar de realizar la operación automaticamente
  auto dyn_ones = Matrix<double>::ones(3, 3);
  Matrix<double,3,3> mat5 = dyn_ones + _mat_3x3_d;

  BOOST_CHECK_EQUAL(2.5, mat5[0][0]);
  BOOST_CHECK_EQUAL(1.0, mat5[0][1]);
  BOOST_CHECK_EQUAL(3.5, mat5[0][2]);
  BOOST_CHECK_EQUAL(2.0, mat5[1][0]);
  BOOST_CHECK_EQUAL(2.0, mat5[1][1]);
  BOOST_CHECK_EQUAL(2.2, mat5[1][2]);
  BOOST_CHECK_EQUAL(2.3, mat5[2][0]);
  BOOST_CHECK_EQUAL(3.6, mat5[2][1]);
  BOOST_CHECK_EQUAL(1.3, mat5[2][2]);

  mat5 = _mat_3x3_d + Matrix<double>::ones(3, 3);

  BOOST_CHECK_EQUAL(2.5, mat5[0][0]);
  BOOST_CHECK_EQUAL(1.0, mat5[0][1]);
  BOOST_CHECK_EQUAL(3.5, mat5[0][2]);
  BOOST_CHECK_EQUAL(2.0, mat5[1][0]);
  BOOST_CHECK_EQUAL(2.0, mat5[1][1]);
  BOOST_CHECK_EQUAL(2.2, mat5[1][2]);
  BOOST_CHECK_EQUAL(2.3, mat5[2][0]);
  BOOST_CHECK_EQUAL(3.6, mat5[2][1]);
  BOOST_CHECK_EQUAL(1.3, mat5[2][2]);

  //Matrix<double, 5, 5> a = Matrix<double, 5, 5>::randon();
  //Matrix<double, 5, 5> b = Matrix<double, 5, 5>::randon();
  //Matrix<double, 5, 5> c = Matrix<double, 5, 5>::randon();
  //Matrix<double, 5, 5> d = Matrix<double, 5, 5>::randon();
  //Matrix<double, 5, 5> e = Matrix<double, 5, 5>::randon();
  //auto f = a + b;
  //auto g = a + b + c;
  //auto h = a + b + c + d;
  //auto i = a + b + c + d + e;

}

/// Resta de matrices

BOOST_FIXTURE_TEST_CASE(subtraction, MatrixTest)
{
  Matrix<double,3,3> mat = mat_identity - _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat[0][0]);
  BOOST_CHECK_EQUAL( 0.0, mat[0][1]);
  BOOST_CHECK_EQUAL(-2.5, mat[0][2]);
  BOOST_CHECK_EQUAL(-1.0, mat[1][0]);
  BOOST_CHECK_EQUAL( 0.0, mat[1][1]);
  BOOST_CHECK_EQUAL(-1.2, mat[1][2]);
  BOOST_CHECK_EQUAL(-1.3, mat[2][0]);
  BOOST_CHECK_EQUAL(-2.6, mat[2][1]);
  BOOST_CHECK_EQUAL( 0.7, mat[2][2]);

  mat_identity -= _mat_3x3_d;

  BOOST_CHECK_EQUAL(-0.5, mat_identity[0][0]);
  BOOST_CHECK_EQUAL( 0.0, mat_identity[0][1]);
  BOOST_CHECK_EQUAL(-2.5, mat_identity[0][2]);
  BOOST_CHECK_EQUAL(-1.0, mat_identity[1][0]);
  BOOST_CHECK_EQUAL( 0.0, mat_identity[1][1]);
  BOOST_CHECK_EQUAL(-1.2, mat_identity[1][2]);
  BOOST_CHECK_EQUAL(-1.3, mat_identity[2][0]);
  BOOST_CHECK_EQUAL(-2.6, mat_identity[2][1]);
  BOOST_CHECK_EQUAL( 0.7, mat_identity[2][2]);


  Matrix<double> mat2 = mat_dyn_identity - (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(-0.5, mat2[0][0]);
  BOOST_CHECK_EQUAL( 0.0, mat2[0][1]);
  BOOST_CHECK_EQUAL(-2.5, mat2[0][2]);
  BOOST_CHECK_EQUAL(-1.0, mat2[1][0]);
  BOOST_CHECK_EQUAL( 0.0, mat2[1][1]);
  BOOST_CHECK_EQUAL(-1.2, mat2[1][2]);
  BOOST_CHECK_EQUAL(-1.3, mat2[2][0]);
  BOOST_CHECK_EQUAL(-2.6, mat2[2][1]);
  BOOST_CHECK_EQUAL( 0.7, mat2[2][2]);

  mat_dyn_identity -= (*_mat_dyn_3x3_d);

  BOOST_CHECK_EQUAL(-0.5, mat_dyn_identity[0][0]);
  BOOST_CHECK_EQUAL( 0.0, mat_dyn_identity[0][1]);
  BOOST_CHECK_EQUAL(-2.5, mat_dyn_identity[0][2]);
  BOOST_CHECK_EQUAL(-1.0, mat_dyn_identity[1][0]);
  BOOST_CHECK_EQUAL( 0.0, mat_dyn_identity[1][1]);
  BOOST_CHECK_EQUAL(-1.2, mat_dyn_identity[1][2]);
  BOOST_CHECK_EQUAL(-1.3, mat_dyn_identity[2][0]);
  BOOST_CHECK_EQUAL(-2.6, mat_dyn_identity[2][1]);
  BOOST_CHECK_EQUAL( 0.7, mat_dyn_identity[2][2]);


  Matrix<double> mat3 = matrix1 - matrix2;

  BOOST_CHECK_CLOSE(mat3[0][0], -0.67, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][1], 6.26, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][2], -0.54, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][3], -3.91, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][4], 1.47, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][5], -2.58, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][6], -3.71, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][7], -1.83, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][8], 3.71, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][9], 2.56, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][0], 4.5, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][1], 2.34, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][2], -0.36, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][3], -2.19, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][4], -0.06, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][5], -0.51, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][6], 0.16, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][7], 4.22, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][8], 0.89, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][9], 1.83, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][0], -2.13, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][1], -0.76, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][2], 5.87, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][3], 7.04, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][4], -7.51, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][5], 3.11, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][6], -0.42, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][7], 1.32, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][8], -2.76, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][9], 0.22, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][0], 1.51, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][1], 4.74, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][2], 0.32, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][3], -3.61, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][4], 6.48, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][5], 2.72, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][6], -1.16, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][7], -0.78, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][8], 0.86, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][9], 1.5, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][0], -8.03, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][1], 2.86, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][2], -4.08, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][3], 2.25, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][4], 0.92, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][5], 0.02, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][6], 2.84, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][7], 3.11, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][8], -3.43, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][9], 8.66, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][0], -1.32, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][1], -3.98, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][2], -3.03, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][3], 4.25, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][4], 2, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][5], 0.22, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][6], -0.94, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][7], -3.06, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][8], 3.51, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][9], 4.12, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][0], -2.41, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][1], -8.61, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][2], 0.22, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][3], -0.04, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][4], -1.19, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][5], -5.58, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][6], 8.73, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][7], -4.14, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][8], -1, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][9], -3.39, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][0], 4.23, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][1], -4.15, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][2], 5.64, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][3], -4.97, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][4], -6.8, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][5], 4.74, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][6], 7.91, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][7], -0.75, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][8], -6.14, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][9], -2.45, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][0], -0.8, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][1], 0.62, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][2], 0.77, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][3], 6.07, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][4], 3.11, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][5], -1.61, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][6], -0.11, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][7], -0.12, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][8], -2.95, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][9], 2.78, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][0], 1.66, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][1], 3.26, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][2], -0.07, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][3], -0.17, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][4], -2.23, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][5], -3.3, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][6], 3.35, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][7], -1, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][8], -3.01, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][9], 2.24, 0.01);
  
  auto identity = Matrix<double, 3, 3>::identity();
  Matrix<double, 3, 3> mat4 = Matrix<double, 3, 3>::ones() - identity;
  BOOST_CHECK_EQUAL(0., mat4[0][0]);
  BOOST_CHECK_EQUAL(1., mat4[0][1]);
  BOOST_CHECK_EQUAL(1., mat4[0][2]);
  BOOST_CHECK_EQUAL(1., mat4[1][0]);
  BOOST_CHECK_EQUAL(0., mat4[1][1]);
  BOOST_CHECK_EQUAL(1., mat4[1][2]);
  BOOST_CHECK_EQUAL(1., mat4[2][0]);
  BOOST_CHECK_EQUAL(1., mat4[2][1]);
  BOOST_CHECK_EQUAL(0., mat4[2][2]);

  auto ones = Matrix<double, 3, 3>::ones();
  mat4 = ones - Matrix<double, 3, 3>::identity();
  BOOST_CHECK_EQUAL(0., mat4[0][0]);
  BOOST_CHECK_EQUAL(1., mat4[0][1]);
  BOOST_CHECK_EQUAL(1., mat4[0][2]);
  BOOST_CHECK_EQUAL(1., mat4[1][0]);
  BOOST_CHECK_EQUAL(0., mat4[1][1]);
  BOOST_CHECK_EQUAL(1., mat4[1][2]);
  BOOST_CHECK_EQUAL(1., mat4[2][0]);
  BOOST_CHECK_EQUAL(1., mat4[2][1]);
  BOOST_CHECK_EQUAL(0., mat4[2][2]);

  mat4 = Matrix<double, 3, 3>::ones() - Matrix<double, 3, 3>::identity();
  BOOST_CHECK_EQUAL(0., mat4[0][0]);
  BOOST_CHECK_EQUAL(1., mat4[0][1]);
  BOOST_CHECK_EQUAL(1., mat4[0][2]);
  BOOST_CHECK_EQUAL(1., mat4[1][0]);
  BOOST_CHECK_EQUAL(0., mat4[1][1]);
  BOOST_CHECK_EQUAL(1., mat4[1][2]);
  BOOST_CHECK_EQUAL(1., mat4[2][0]);
  BOOST_CHECK_EQUAL(1., mat4[2][1]);
  BOOST_CHECK_EQUAL(0., mat4[2][2]);
}

/// Multiplicación de matrices

template<typename T> inline
Matrix<T> mulmat3(const Matrix<T> &matrix1,
                  const Matrix<T> &matrix2)
{

  size_t rows = matrix1.rows();
  size_t dim1 = matrix1.cols();
  size_t cols = matrix2.cols();
  size_t dim2 = matrix2.rows();

  TL_ASSERT(dim1 == dim2, "A columns != B rows");

  Matrix<T> matrix = Matrix<T>::zero(rows, cols);

  static const size_t num_unroll_r = 5;
  static const size_t num_unroll_k = 5;

  const size_t r_max_for_unrolled_loop = matrix1.rows() / num_unroll_r * num_unroll_r;

  const size_t j_max = matrix2.cols();
  const size_t k_max_for_unrolled_loop = matrix2.rows() / num_unroll_k * num_unroll_k;
  const size_t k_max = matrix2.rows();

  for (size_t r = 0; r < r_max_for_unrolled_loop; r += num_unroll_r) {

    auto a_row0 = matrix1[r];
    auto a_row1 = matrix1[r + 1];
    auto a_row2 = matrix1[r + 2];
    auto a_row3 = matrix1[r + 3];
    auto a_row4 = matrix1[r + 4];

    for (size_t k = 0; k < k_max_for_unrolled_loop; k += num_unroll_k) {

      T a00 = a_row0[k];
      T a01 = a_row0[k + 1];
      T a02 = a_row0[k + 2];
      T a03 = a_row0[k + 3];
      T a04 = a_row0[k + 4];
      T a10 = a_row1[k];
      T a11 = a_row1[k + 1];
      T a12 = a_row1[k + 2];
      T a13 = a_row1[k + 3];
      T a14 = a_row1[k + 4];
      T a20 = a_row2[k];
      T a21 = a_row2[k + 1];
      T a22 = a_row2[k + 2];
      T a23 = a_row2[k + 3];
      T a24 = a_row2[k + 4];
      T a30 = a_row3[k];
      T a31 = a_row3[k + 1];
      T a32 = a_row3[k + 2];
      T a33 = a_row3[k + 3];
      T a34 = a_row3[k + 4];
      T a40 = a_row4[k];
      T a41 = a_row4[k + 1];
      T a42 = a_row4[k + 2];
      T a43 = a_row4[k + 3];
      T a44 = a_row4[k + 4];

      for (int c = 0; c < cols; ++c) {
        T b0 = matrix2[k][c];
        T b1 = matrix2[k + 1][c];
        T b2 = matrix2[k + 2][c];
        T b3 = matrix2[k + 3][c];
        T b4 = matrix2[k + 4][c];

        T sum0;
        sum0 = a00 * b0;
        sum0 += a01 * b1;
        sum0 += a02 * b2;
        sum0 += a03 * b3;
        sum0 += a04 * b4;
        matrix[r][c] += sum0;
        T sum1 = a10 * b0;
        sum1 += a11 * b1;
        sum1 += a12 * b2;
        sum1 += a13 * b3;
        sum1 += a14 * b4;
        matrix[r + 1][c] += sum1;
        T sum2 = a20 * b0;
        sum2 += a21 * b1;
        sum2 += a22 * b2;
        sum2 += a23 * b3;
        sum2 += a24 * b4;
        matrix[r + 2][c] += sum2;
        T sum3 = a30 * b0;
        sum3 += a31 * b1;
        sum3 += a32 * b2;
        sum3 += a33 * b3;
        sum3 += a34 * b4;
        matrix[r + 3][c] += sum3;
        T sum4 = a40 * b0;
        sum4 += a41 * b1;
        sum4 += a42 * b2;
        sum4 += a43 * b3;
        sum4 += a44 * b4;
        matrix[r + 4][c] += sum4;
      }
    }
    for (size_t k = k_max_for_unrolled_loop; k < k_max; ++k) {
      //const T a = matrix1[r][k];
      for (size_t c = 0; c < cols; ++c) {
        matrix[r][c] += a_row0[k] * matrix2[k][c];
        matrix[r + 1][c] += a_row1[k] * matrix2[k][c];
        matrix[r + 2][c] += a_row2[k] * matrix2[k][c];
        matrix[r + 3][c] += a_row3[k] * matrix2[k][c];
        matrix[r + 4][c] += a_row4[k] * matrix2[k][c];
      }
    }
  }

  for (size_t r = r_max_for_unrolled_loop; r < rows; ++r) {
    auto a_row = matrix1[r];
    for (size_t k = 0; k < k_max_for_unrolled_loop; k += num_unroll_k) {

      T a00 = a_row[k];
      T a01 = a_row[k + 1];
      T a02 = a_row[k + 2];
      T a03 = a_row[k + 3];
      T a04 = a_row[k + 4];

      for (int c = 0; c < cols; ++c) {

        T b0 = matrix2[k][c];
        T b1 = matrix2[k + 1][c];
        T b2 = matrix2[k + 2][c];
        T b3 = matrix2[k + 3][c];
        T b4 = matrix2[k + 4][c];

        T sum;
        sum = a00 * b0;
        sum += a01 * b1;
        sum += a02 * b2;
        sum += a03 * b3;
        sum += a04 * b4;
        matrix[r][c] += sum;
      }
    }
    for (size_t k = k_max_for_unrolled_loop; k < k_max; ++k) {
      const T a = matrix1[r][k];
      for (size_t c = 0; c < cols; ++c) {
        matrix[r][c] += a * matrix2[k][c];
      }
    }
  }
  return matrix;
}

BOOST_FIXTURE_TEST_CASE(multiplication, MatrixTest)
{
  Matrix<int, 2, 3> mat = _mat_2x3_i * _cofactor_matrix;

  BOOST_CHECK_EQUAL(12, mat[0][0]);
  BOOST_CHECK_EQUAL(44, mat[0][1]);
  BOOST_CHECK_EQUAL(94, mat[0][2]);
  BOOST_CHECK_EQUAL(11, mat[1][0]);
  BOOST_CHECK_EQUAL(42, mat[1][1]);
  BOOST_CHECK_EQUAL(69, mat[1][2]);

  Matrix<int> mat2 = (*_mat_dyn_2x3_i) * (*_cofactor_matrix_dyn);

  BOOST_CHECK_EQUAL(12, mat2[0][0]);
  BOOST_CHECK_EQUAL(44, mat2[0][1]);
  BOOST_CHECK_EQUAL(94, mat2[0][2]);
  BOOST_CHECK_EQUAL(11, mat2[1][0]);
  BOOST_CHECK_EQUAL(42, mat2[1][1]);
  BOOST_CHECK_EQUAL(69, mat2[1][2]);

  // Matriz estática por dinámica

  Matrix<int> mat4 = _mat_2x3_i * (*_cofactor_matrix_dyn);

  BOOST_CHECK_EQUAL(12, mat4[0][0]);
  BOOST_CHECK_EQUAL(44, mat4[0][1]);
  BOOST_CHECK_EQUAL(94, mat4[0][2]);
  BOOST_CHECK_EQUAL(11, mat4[1][0]);
  BOOST_CHECK_EQUAL(42, mat4[1][1]);
  BOOST_CHECK_EQUAL(69, mat4[1][2]);

  // Matriz dinámica por estática

  Matrix<int> mat5 = (*_mat_dyn_2x3_i) * _cofactor_matrix;

  BOOST_CHECK_EQUAL(12, mat5[0][0]);
  BOOST_CHECK_EQUAL(44, mat5[0][1]);
  BOOST_CHECK_EQUAL(94, mat5[0][2]);
  BOOST_CHECK_EQUAL(11, mat5[1][0]);
  BOOST_CHECK_EQUAL(42, mat5[1][1]);
  BOOST_CHECK_EQUAL(69, mat5[1][2]);
    
  //Matrix<double> mat3 = mulmat3(matrix1, matrix2);
  Matrix<double> mat3 = matrix1 * matrix2;
  BOOST_CHECK_CLOSE(mat3[0][0], 150.46129999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][1], 165.27870000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][2], 158.26670000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][3], 141.92560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][4], 187.37970000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][5], 167.06009999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][6], 202.88790000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][7], 176.77520000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][8], 180.07070000000002, 0.01);
  BOOST_CHECK_CLOSE(mat3[0][9], 152.05050000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][0], 227.41139999999996, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][1], 227.16690000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][2], 201.63189999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][3], 255.38209999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][4], 290.18440000000004, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][5], 291.53309999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][6], 253.57210000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][7], 261.00040000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][8], 286.16719999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[1][9], 215.73440000000005, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][0], 187.83359999999996, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][1], 189.31679999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][2], 173.78440000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][3], 218.94399999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][4], 246.74090000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][5], 217.63609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][6], 218.45099999999994, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][7], 282.29669999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][8], 228.28930000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[2][9], 189.72190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][0], 213.25510000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][1], 194.08109999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][2], 195.57950000000002, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][3], 196.17829999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][4], 267.10329999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][5], 249.59680000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][6], 279.93429999999995, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][7], 234.07190000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][8], 245.87960000000004, 0.01);
  BOOST_CHECK_CLOSE(mat3[3][9], 174.91820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][0], 169.70760000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][1], 180.98440000000002, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][2], 179.15429999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][3], 148.66820000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][4], 208.23980000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][5], 214.42800000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][6], 172.50779999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][7], 212.39560000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][8], 204.69629999999995, 0.01);
  BOOST_CHECK_CLOSE(mat3[4][9], 150.65260000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][0], 171.61580000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][1], 149.27350000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][2], 161.58570000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][3], 146.83769999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][4], 195.70380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][5], 203.04889999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][6], 143.38870000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][7], 200.82369999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][8], 200.71049999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[5][9], 143.38950000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][0], 141.29339999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][1], 138.90479999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][2], 105.16480000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][3], 129.06590000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][4], 142.59010000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][5], 187.58380000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][6], 108.31489999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][7], 176.67999999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][8], 137.38419999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[6][9], 127.76190000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][0], 213.23240000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][1], 230.35989999999998, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][2], 122.61140000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][3], 177.68850000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][4], 224.14439999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][5], 208.38609999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][6], 189.76090000000002, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][7], 301.09850000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][8], 187.49260000000004, 0.01);
  BOOST_CHECK_CLOSE(mat3[7][9], 210.30039999999997, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][0], 235.31420000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][1], 138.34640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][2], 152.97290000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][3], 135.53080000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][4], 208.74430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][5], 231.41550000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][6], 157.95550000000003, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][7], 217.42079999999996, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][8], 228.77390000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[8][9], 157.30339999999995, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][0], 176.99920000000000, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][1], 160.61970000000002, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][2], 122.35509999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][3], 119.04239999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][4], 180.94049999999999, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][5], 165.30430000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][6], 165.88210000000004, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][7], 198.12640000000002, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][8], 167.02330000000001, 0.01);
  BOOST_CHECK_CLOSE(mat3[9][9], 155.77199999999999, 0.01);

}

/// Multiplicación de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(matrix_scalar, MatrixTest)
{
  Matrix<int,2,3> mat = _mat_2x3_i * 10;

  BOOST_CHECK_EQUAL(60, mat[0][0]);
  BOOST_CHECK_EQUAL(80, mat[0][1]);
  BOOST_CHECK_EQUAL(60, mat[0][2]);
  BOOST_CHECK_EQUAL(90, mat[1][0]);
  BOOST_CHECK_EQUAL(60, mat[1][1]);
  BOOST_CHECK_EQUAL(20, mat[1][2]);

  _mat_2x3_i *= 10;

  BOOST_CHECK_EQUAL(60, _mat_2x3_i[0][0]);
  BOOST_CHECK_EQUAL(80, _mat_2x3_i[0][1]);
  BOOST_CHECK_EQUAL(60, _mat_2x3_i[0][2]);
  BOOST_CHECK_EQUAL(90, _mat_2x3_i[1][0]);
  BOOST_CHECK_EQUAL(60, _mat_2x3_i[1][1]);
  BOOST_CHECK_EQUAL(20, _mat_2x3_i[1][2]);

  _mat_2x3_i[1] *= -1;
  BOOST_CHECK_EQUAL(-90, _mat_2x3_i[1][0]);
  BOOST_CHECK_EQUAL(-60, _mat_2x3_i[1][1]);
  BOOST_CHECK_EQUAL(-20, _mat_2x3_i[1][2]);

  Matrix<int> mat2 = (*_mat_dyn_2x3_i) * 10;
  
  BOOST_CHECK_EQUAL(60, mat2[0][0]);
  BOOST_CHECK_EQUAL(80, mat2[0][1]);
  BOOST_CHECK_EQUAL(60, mat2[0][2]);
  BOOST_CHECK_EQUAL(90, mat2[1][0]);
  BOOST_CHECK_EQUAL(60, mat2[1][1]);
  BOOST_CHECK_EQUAL(20, mat2[1][2]);

  (*_mat_dyn_2x3_i) *= 10;

  BOOST_CHECK_EQUAL(60, _mat_dyn_2x3_i->at(0, 0));
  BOOST_CHECK_EQUAL(80, _mat_dyn_2x3_i->at(0, 1));
  BOOST_CHECK_EQUAL(60, _mat_dyn_2x3_i->at(0, 2));
  BOOST_CHECK_EQUAL(90, _mat_dyn_2x3_i->at(1, 0));
  BOOST_CHECK_EQUAL(60, _mat_dyn_2x3_i->at(1, 1));
  BOOST_CHECK_EQUAL(20, _mat_dyn_2x3_i->at(1, 2));

  (*_mat_dyn_2x3_i)[0] *= -1;
  BOOST_CHECK_EQUAL(-60, _mat_dyn_2x3_i->at(0, 0));
  BOOST_CHECK_EQUAL(-80, _mat_dyn_2x3_i->at(0, 1));
  BOOST_CHECK_EQUAL(-60, _mat_dyn_2x3_i->at(0, 2));

  Matrix<int, 2, 2> mat3 = Matrix<int, 2, 2>::ones() * 10;
  BOOST_CHECK_EQUAL(10, mat3[0][0]);
  BOOST_CHECK_EQUAL(10, mat3[0][1]);
  BOOST_CHECK_EQUAL(10, mat3[1][0]);
  BOOST_CHECK_EQUAL(10, mat3[1][1]);

  Matrix<int> mat4 = Matrix<int>::ones(2, 2) * 20;
  BOOST_CHECK_EQUAL(20, mat4[0][0]);
  BOOST_CHECK_EQUAL(20, mat4[0][1]);
  BOOST_CHECK_EQUAL(20, mat4[1][0]);
  BOOST_CHECK_EQUAL(20, mat4[1][1]);
}

/// Multiplicación de un escalar por una matriz

BOOST_FIXTURE_TEST_CASE(scalar_matrix, MatrixTest)
{
  Matrix<int,2,3> mat = 10 * _mat_2x3_i;

  BOOST_CHECK_EQUAL(60, mat[0][0]);
  BOOST_CHECK_EQUAL(80, mat[0][1]);
  BOOST_CHECK_EQUAL(60, mat[0][2]);
  BOOST_CHECK_EQUAL(90, mat[1][0]);
  BOOST_CHECK_EQUAL(60, mat[1][1]);
  BOOST_CHECK_EQUAL(20, mat[1][2]);

  Matrix<int> mat2 = 10 * (*_mat_dyn_2x3_i);

  BOOST_CHECK_EQUAL(60, mat2[0][0]);
  BOOST_CHECK_EQUAL(80, mat2[0][1]);
  BOOST_CHECK_EQUAL(60, mat2[0][2]);
  BOOST_CHECK_EQUAL(90, mat2[1][0]);
  BOOST_CHECK_EQUAL(60, mat2[1][1]);
  BOOST_CHECK_EQUAL(20, mat2[1][2]);

  Matrix<int, 2, 2> mat3 = 10 * Matrix<int, 2, 2>::ones();
  BOOST_CHECK_EQUAL(10, mat3[0][0]);
  BOOST_CHECK_EQUAL(10, mat3[0][1]);
  BOOST_CHECK_EQUAL(10, mat3[1][0]);
  BOOST_CHECK_EQUAL(10, mat3[1][1]);

  Matrix<int> mat4 = 20 * Matrix<int>::ones(2, 2);
  BOOST_CHECK_EQUAL(20, mat4[0][0]);
  BOOST_CHECK_EQUAL(20, mat4[0][1]);
  BOOST_CHECK_EQUAL(20, mat4[1][0]);
  BOOST_CHECK_EQUAL(20, mat4[1][1]);
}

/// Multiplicación de un vector por una matriz

BOOST_FIXTURE_TEST_CASE(vector_matrix, MatrixTest)
{
    Vector<int, 3> vect{ 1, 2, 3 };
    Vector<int, 2> vect2 = _mat_2x3_i * vect;

    BOOST_CHECK_EQUAL(40, vect2[0]);
    BOOST_CHECK_EQUAL(27, vect2[1]);

    Vector<int> vect3{ 1, 2, 3 };
    Vector<int> vect4 = (*_mat_dyn_2x3_i) * vect3;

    BOOST_CHECK_EQUAL(40, vect4[0]);
    BOOST_CHECK_EQUAL(27, vect4[1]);

    Vector<double, 5> vect5{ 1, 2, 3, 4, 5};
    Vector<double, 5> vect6 = _mat_5x5_d * vect5;

    BOOST_CHECK_EQUAL(83., vect6[0]);
    BOOST_CHECK_EQUAL(54., vect6[1]);
    BOOST_CHECK_EQUAL(47., vect6[2]);
    BOOST_CHECK_EQUAL(78., vect6[3]);
    BOOST_CHECK_EQUAL(87., vect6[4]);

    Vector<double> vect7{ 1, 2, 3, 4, 5};
    Vector<double> vect8 = *_mat_dyn_5x5_d * vect7;

    BOOST_CHECK_EQUAL(83., vect8[0]);
    BOOST_CHECK_EQUAL(54., vect8[1]);
    BOOST_CHECK_EQUAL(47., vect8[2]);
    BOOST_CHECK_EQUAL(78., vect8[3]);
    BOOST_CHECK_EQUAL(87., vect8[4]);

    Matrix<float, 8, 8> matrix_f{1, 2, 3, 4, 5, 6, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8};
    Vector<float, 8> vectf{ 1, 2, 3, 4, 5, 6, 7, 8};
    Vector<float, 8> vect_result = matrix_f * vectf;

    BOOST_CHECK_EQUAL(204.f, vect_result[0]);
    BOOST_CHECK_EQUAL(204.f, vect_result[1]);
    BOOST_CHECK_EQUAL(204.f, vect_result[2]);
    BOOST_CHECK_EQUAL(204.f, vect_result[3]);
    BOOST_CHECK_EQUAL(204.f, vect_result[4]);
    BOOST_CHECK_EQUAL(204.f, vect_result[5]);
    BOOST_CHECK_EQUAL(204.f, vect_result[6]);
    BOOST_CHECK_EQUAL(204.f, vect_result[7]);

    // Matriz estática por vector dinámico

    Vector<float> vectf2{ 1, 2, 3, 4, 5, 6, 7, 8};
    vect_result = matrix_f * vectf2;

    BOOST_CHECK_EQUAL(204.f, vect_result[0]);
    BOOST_CHECK_EQUAL(204.f, vect_result[1]);
    BOOST_CHECK_EQUAL(204.f, vect_result[2]);
    BOOST_CHECK_EQUAL(204.f, vect_result[3]);
    BOOST_CHECK_EQUAL(204.f, vect_result[4]);
    BOOST_CHECK_EQUAL(204.f, vect_result[5]);
    BOOST_CHECK_EQUAL(204.f, vect_result[6]);
    BOOST_CHECK_EQUAL(204.f, vect_result[7]);

    // Matriz dinámica por vector estático

    Matrix<float> matrix_din{{1, 2, 3, 4, 5, 6, 7, 8},
                             {1, 2, 3, 4, 5, 6, 7, 8},
                             {1, 2, 3, 4, 5, 6, 7, 8},
                             {1, 2, 3, 4, 5, 6, 7, 8},
                             {1, 2, 3, 4, 5, 6, 7, 8},
                             {1, 2, 3, 4, 5, 6, 7, 8},
                             {1, 2, 3, 4, 5, 6, 7, 8},
                             {1, 2, 3, 4, 5, 6, 7, 8}};

    auto vect_result_2 = matrix_din * vectf;

    BOOST_CHECK_EQUAL(204.f, vect_result_2[0]);
    BOOST_CHECK_EQUAL(204.f, vect_result_2[1]);
    BOOST_CHECK_EQUAL(204.f, vect_result_2[2]);
    BOOST_CHECK_EQUAL(204.f, vect_result_2[3]);
    BOOST_CHECK_EQUAL(204.f, vect_result_2[4]);
    BOOST_CHECK_EQUAL(204.f, vect_result_2[5]);
    BOOST_CHECK_EQUAL(204.f, vect_result_2[6]);
    BOOST_CHECK_EQUAL(204.f, vect_result_2[7]);
}


// División de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(div_matrix_scalar, MatrixTest)
{
  Matrix<double,3,3> mat = _mat_3x3_d / 10.;

  BOOST_CHECK_CLOSE(.15, mat[0][0], 0.01);
  BOOST_CHECK_EQUAL(.0, mat[0][1]);
  BOOST_CHECK_EQUAL(.25, mat[0][2]);
  BOOST_CHECK_EQUAL(.10, mat[1][0]);
  BOOST_CHECK_EQUAL(.10, mat[1][1]);
  BOOST_CHECK_EQUAL(.12, mat[1][2]);
  BOOST_CHECK_EQUAL(.13, mat[2][0]);
  BOOST_CHECK_EQUAL(.26, mat[2][1]);
  BOOST_CHECK_EQUAL(.03, mat[2][2]);

  Matrix<double> mat2 = (*_mat_dyn_3x3_d) / 10.;

  BOOST_CHECK_CLOSE(.15, mat2[0][0], 0.01);
  BOOST_CHECK_EQUAL(.0, mat2[0][1]);
  BOOST_CHECK_EQUAL(.25, mat2[0][2]);
  BOOST_CHECK_EQUAL(.10, mat2[1][0]);
  BOOST_CHECK_EQUAL(.10, mat2[1][1]);
  BOOST_CHECK_EQUAL(.12, mat2[1][2]);
  BOOST_CHECK_EQUAL(.13, mat2[2][0]);
  BOOST_CHECK_EQUAL(.26, mat2[2][1]);
  BOOST_CHECK_EQUAL(.03, mat2[2][2]);

  Matrix<double, 2, 2> mat3 = Matrix<double, 2, 2>::ones() / 10.;
  BOOST_CHECK_EQUAL(0.1, mat3[0][0]);
  BOOST_CHECK_EQUAL(0.1, mat3[0][1]);
  BOOST_CHECK_EQUAL(0.1, mat3[1][0]);
  BOOST_CHECK_EQUAL(0.1, mat3[1][1]);

  Matrix<double> mat4 = Matrix<double>::ones(2, 2) / 20.;
  BOOST_CHECK_EQUAL(0.05, mat4[0][0]);
  BOOST_CHECK_EQUAL(0.05, mat4[0][1]);
  BOOST_CHECK_EQUAL(0.05, mat4[1][0]);
  BOOST_CHECK_EQUAL(0.05, mat4[1][1]);
}


// Vector

BOOST_FIXTURE_TEST_CASE(col, MatrixTest)
{
  auto col_0 = _mat_dyn_3x3_d->col(0);
  auto col_1 = _mat_dyn_3x3_d->col(1);
  auto col_2 = _mat_dyn_3x3_d->col(2);

  BOOST_CHECK_EQUAL(1.5, col_0[0]);
  BOOST_CHECK_EQUAL(1.0, col_0[1]);
  BOOST_CHECK_EQUAL(1.3, col_0[2]);
  BOOST_CHECK_EQUAL(0.0, col_1[0]);
  BOOST_CHECK_EQUAL(1.0, col_1[1]);
  BOOST_CHECK_EQUAL(2.6, col_1[2]);
  BOOST_CHECK_EQUAL(2.5, col_2[0]);
  BOOST_CHECK_EQUAL(1.2, col_2[1]);
  BOOST_CHECK_EQUAL(0.3, col_2[2]);
}

BOOST_FIXTURE_TEST_CASE(row, MatrixTest)
{
  auto row_0 = _mat_dyn_3x3_d->row(0);
  auto row_1 = _mat_dyn_3x3_d->row(1);
  auto row_2 = _mat_dyn_3x3_d->row(2);

  BOOST_CHECK_EQUAL(1.5, row_0[0]);
  BOOST_CHECK_EQUAL(0.0, row_0[1]);
  BOOST_CHECK_EQUAL(2.5, row_0[2]);
  BOOST_CHECK_EQUAL(1.0, row_1[0]);
  BOOST_CHECK_EQUAL(1.0, row_1[1]);
  BOOST_CHECK_EQUAL(1.2, row_1[2]);
  BOOST_CHECK_EQUAL(1.3, row_2[0]);
  BOOST_CHECK_EQUAL(2.6, row_2[1]);
  BOOST_CHECK_EQUAL(0.3, row_2[2]);
}

BOOST_FIXTURE_TEST_CASE(block, MatrixTest)
{
  auto block_0 = _mat_5x5_d.block(0, 3, 3, 4);

  BOOST_CHECK_EQUAL(4, block_0.rows());
  BOOST_CHECK_EQUAL(2, block_0.cols());
  BOOST_CHECK_EQUAL(7, block_0(0,0));
  BOOST_CHECK_EQUAL(3, block_0(0,1));
  BOOST_CHECK_EQUAL(3, block_0(1,0));
  BOOST_CHECK_EQUAL(3, block_0(1,1));
  BOOST_CHECK_EQUAL(3, block_0(2,0));
  BOOST_CHECK_EQUAL(3, block_0(2,1));
  BOOST_CHECK_EQUAL(7, block_0(3,0));
  BOOST_CHECK_EQUAL(6, block_0(3,1));


  auto block_1 = _mat_5x5_d.rowBlock(2, 4);

  BOOST_CHECK_EQUAL(3, block_1.rows());
  BOOST_CHECK_EQUAL(5, block_1.cols());
  BOOST_CHECK_EQUAL(8, block_1(0, 0));
  BOOST_CHECK_EQUAL(3, block_1(0, 1));
  BOOST_CHECK_EQUAL(2, block_1(0, 2));
  BOOST_CHECK_EQUAL(3, block_1(0, 3));
  BOOST_CHECK_EQUAL(3, block_1(0, 4));
  BOOST_CHECK_EQUAL(5, block_1(1, 0));
  BOOST_CHECK_EQUAL(3, block_1(1, 1));
  BOOST_CHECK_EQUAL(3, block_1(1, 2));
  BOOST_CHECK_EQUAL(7, block_1(1, 3));
  BOOST_CHECK_EQUAL(6, block_1(1, 4));
  BOOST_CHECK_EQUAL(5, block_1(2, 0));
  BOOST_CHECK_EQUAL(5, block_1(2, 1));
  BOOST_CHECK_EQUAL(7, block_1(2, 2));
  BOOST_CHECK_EQUAL(4, block_1(2, 3));
  BOOST_CHECK_EQUAL(7, block_1(2, 4));

  auto block_2 = _mat_5x5_d.colBlock(1, 2);
  BOOST_CHECK_EQUAL(5, block_2.rows());
  BOOST_CHECK_EQUAL(2, block_2.cols());
  BOOST_CHECK_EQUAL(8, block_2(0, 0));
  BOOST_CHECK_EQUAL(6, block_2(0, 1));
  BOOST_CHECK_EQUAL(6, block_2(1, 0));
  BOOST_CHECK_EQUAL(2, block_2(1, 1));
  BOOST_CHECK_EQUAL(3, block_2(2, 0));
  BOOST_CHECK_EQUAL(2, block_2(2, 1));
  BOOST_CHECK_EQUAL(3, block_2(3, 0));
  BOOST_CHECK_EQUAL(3, block_2(3, 1));
  BOOST_CHECK_EQUAL(5, block_2(4, 0));
  BOOST_CHECK_EQUAL(7, block_2(4, 1));
}

BOOST_FIXTURE_TEST_CASE(compare, MatrixTest)
{
  Matrix<double, 2, 2> mat1;
  mat1[0][0] = 2.;
  mat1[0][1] = 3.;
  mat1[1][0] = 1.;
  mat1[1][1] = 4.;

  Matrix<double, 2, 2> mat2(mat1);

  Matrix<double, 2, 2> mat3;
  mat3[0][0] = 1.;
  mat3[0][1] = 5.;
  mat3[1][0] = 2.;
  mat3[1][1] = 3.;


  BOOST_CHECK(mat1 == mat2);
  BOOST_CHECK(mat1 != mat3);
}

BOOST_AUTO_TEST_SUITE_END()

/* */


BOOST_AUTO_TEST_SUITE(MatrixRowTestSuite)

struct MatrixRowTest
{
  MatrixRowTest()
    : _mat_dyn_2x2(new Matrix<double>(2, 2))
  {}
  ~MatrixRowTest()
  {
  }

  void setup()
  {
    _mat_dyn_2x2->at(0, 0) = 2.;
    _mat_dyn_2x2->at(0, 1) = 3.;
    _mat_dyn_2x2->at(1, 0) = 1.;
    _mat_dyn_2x2->at(1, 1) = 4.;

    _mat_3x3_d[0][0] = 1.5;
    _mat_3x3_d[0][1] = 0.0;
    _mat_3x3_d[0][2] = 2.5;
    _mat_3x3_d[1][0] = 1.0;
    _mat_3x3_d[1][1] = 1.0;
    _mat_3x3_d[1][2] = 1.2;
    _mat_3x3_d[2][0] = 1.3;
    _mat_3x3_d[2][1] = 2.6;
    _mat_3x3_d[2][2] = 0.3;
  }

  void teardown()
  {

  }

  Matrix<double> *_mat_dyn_2x2;
  Matrix<double, 3, 3> _mat_3x3_d;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, MatrixRowTest)
{
  auto v1 = (*_mat_dyn_2x2)[1][0];
  (*_mat_dyn_2x2)[1][0] = 2.;
  auto v2 = (*_mat_dyn_2x2)[1][0];

  auto v3 = _mat_3x3_d[1][1];
  _mat_3x3_d[1][1] = 10.;
  auto v4 = _mat_3x3_d[1][1];
}

BOOST_FIXTURE_TEST_CASE(iterate, MatrixRowTest)
{
  {
    auto r0 = _mat_3x3_d[0];
    auto it0 = r0.begin();
    BOOST_CHECK_EQUAL(1.5, *it0);
    it0++;
    BOOST_CHECK_EQUAL(0.0, *it0);
    it0++;
    BOOST_CHECK_EQUAL(2.5, *it0);
    it0++;
    BOOST_CHECK(it0 == r0.end());

    auto r1 = _mat_3x3_d[1];
    auto it1 = r1.begin();
    BOOST_CHECK_EQUAL(1.0, *it1);
    it1++;
    BOOST_CHECK_EQUAL(1.0, *it1);
    it1++;
    BOOST_CHECK_EQUAL(1.2, *it1);
    it1++;
    BOOST_CHECK(it1 == r1.end());

    auto r2 = _mat_3x3_d[2];
    auto it2 = r2.begin();
    BOOST_CHECK_EQUAL(1.3, *it2);
    it2++;
    BOOST_CHECK_EQUAL(2.6, *it2);
    it2++;
    BOOST_CHECK_EQUAL(0.3, *it2);
    it2++;
    BOOST_CHECK(it2 == r2.end());

  }

  {
    auto r0 = (*_mat_dyn_2x2)[0];
    auto it0 = r0.begin();
    BOOST_CHECK_EQUAL(2., *it0);
    it0++;
    BOOST_CHECK_EQUAL(3., *it0);
    it0++;
    BOOST_CHECK(it0 == r0.end());

    auto r1 = (*_mat_dyn_2x2)[1];
    auto it1 = r1.begin();
    BOOST_CHECK_EQUAL(1.0, *it1);
    it1++;
    BOOST_CHECK_EQUAL(4.0, *it1);
    it1++;
    BOOST_CHECK(it1 == r1.end());
  }

}

BOOST_FIXTURE_TEST_CASE(size, MatrixRowTest)
{
  auto r0 = (*_mat_dyn_2x2)[0];
  BOOST_CHECK_EQUAL(2, r0.size());

  auto r1 = (*_mat_dyn_2x2)[1];
  BOOST_CHECK_EQUAL(2, r1.size());
}

BOOST_FIXTURE_TEST_CASE(value_at, MatrixRowTest)
{
  {
    auto r0 = _mat_3x3_d[0];

    BOOST_CHECK_EQUAL(1.5, r0[0]);
    BOOST_CHECK_EQUAL(0.0, r0[1]);
    BOOST_CHECK_EQUAL(2.5, r0[2]);

    auto r1 = _mat_3x3_d[1];

    BOOST_CHECK_EQUAL(1.0, r1[0]);
    BOOST_CHECK_EQUAL(1.0, r1[1]);
    BOOST_CHECK_EQUAL(1.2, r1[2]);

    auto r2 = _mat_3x3_d[2];

    BOOST_CHECK_EQUAL(1.3, r2[0]);
    BOOST_CHECK_EQUAL(2.6, r2[1]);
    BOOST_CHECK_EQUAL(0.3, r2[2]);

  }

  {
    auto r0 = (*_mat_dyn_2x2)[0];

    BOOST_CHECK_EQUAL(2., r0[0]);
    BOOST_CHECK_EQUAL(3., r0[1]);

    auto r1 = (*_mat_dyn_2x2)[1];

    BOOST_CHECK_EQUAL(1.0, r1[0]);
    BOOST_CHECK_EQUAL(4.0, r1[1]);

  }
}

BOOST_FIXTURE_TEST_CASE(asign, MatrixRowTest)
{
  Matrix<int, 5, 5> a{ {6, 8,6,7,3},
                       {9,6,2,3,3},
                       {8,3,2,3,3},
                       {5,3,3,7,6},
                       {5,5,7,4,7} };

  a[0] = 0;
  a[1] = 1;
  a[2] = 2;
  a[3] = 3;
  a[4] = 4;

  BOOST_CHECK_EQUAL(0, a[0][0]);
  BOOST_CHECK_EQUAL(0, a[0][1]);
  BOOST_CHECK_EQUAL(0, a[0][2]);
  BOOST_CHECK_EQUAL(0, a[0][3]);
  BOOST_CHECK_EQUAL(0, a[0][4]);
  BOOST_CHECK_EQUAL(1, a[1][0]);
  BOOST_CHECK_EQUAL(1, a[1][1]);
  BOOST_CHECK_EQUAL(1, a[1][2]);
  BOOST_CHECK_EQUAL(1, a[1][3]);
  BOOST_CHECK_EQUAL(1, a[1][4]);
  BOOST_CHECK_EQUAL(2, a[2][0]);
  BOOST_CHECK_EQUAL(2, a[2][1]);
  BOOST_CHECK_EQUAL(2, a[2][2]);
  BOOST_CHECK_EQUAL(2, a[2][3]);
  BOOST_CHECK_EQUAL(2, a[2][4]);
  BOOST_CHECK_EQUAL(3, a[3][0]);
  BOOST_CHECK_EQUAL(3, a[3][1]);
  BOOST_CHECK_EQUAL(3, a[3][2]);
  BOOST_CHECK_EQUAL(3, a[3][3]);
  BOOST_CHECK_EQUAL(3, a[3][4]);
  BOOST_CHECK_EQUAL(4, a[4][0]);
  BOOST_CHECK_EQUAL(4, a[4][1]);
  BOOST_CHECK_EQUAL(4, a[4][2]);
  BOOST_CHECK_EQUAL(4, a[4][3]);
  BOOST_CHECK_EQUAL(4, a[4][4]);

}

/* Operaciones unarias */

BOOST_FIXTURE_TEST_CASE(plus, MatrixRowTest)
{
  {
    auto r0 = +_mat_3x3_d[0];

    BOOST_CHECK_EQUAL(1.5, r0[0]);
    BOOST_CHECK_EQUAL(0.0, r0[1]);
    BOOST_CHECK_EQUAL(2.5, r0[2]);

    auto r1 = +_mat_3x3_d[1];

    BOOST_CHECK_EQUAL(1.0, r1[0]);
    BOOST_CHECK_EQUAL(1.0, r1[1]);
    BOOST_CHECK_EQUAL(1.2, r1[2]);

    auto r2 = +_mat_3x3_d[2];

    BOOST_CHECK_EQUAL(1.3, r2[0]);
    BOOST_CHECK_EQUAL(2.6, r2[1]);
    BOOST_CHECK_EQUAL(0.3, r2[2]);

  }

  {
    auto r0 = +(*_mat_dyn_2x2)[0];

    BOOST_CHECK_EQUAL(2., r0[0]);
    BOOST_CHECK_EQUAL(3., r0[1]);

    auto r1 = +(*_mat_dyn_2x2)[1];

    BOOST_CHECK_EQUAL(1.0, r1[0]);
    BOOST_CHECK_EQUAL(4.0, r1[1]);

  }
}

BOOST_FIXTURE_TEST_CASE(minus, MatrixRowTest)
{
  {
    auto r0 = -_mat_3x3_d[0];

    BOOST_CHECK_EQUAL(-1.5, r0[0]);
    BOOST_CHECK_EQUAL(-0.0, r0[1]);
    BOOST_CHECK_EQUAL(-2.5, r0[2]);

    auto r1 = -_mat_3x3_d[1];

    BOOST_CHECK_EQUAL(-1.0, r1[0]);
    BOOST_CHECK_EQUAL(-1.0, r1[1]);
    BOOST_CHECK_EQUAL(-1.2, r1[2]);

    auto r2 = -_mat_3x3_d[2];

    BOOST_CHECK_EQUAL(-1.3, r2[0]);
    BOOST_CHECK_EQUAL(-2.6, r2[1]);
    BOOST_CHECK_EQUAL(-0.3, r2[2]);

  }

  {
    auto r0 = -(*_mat_dyn_2x2)[0];

    BOOST_CHECK_EQUAL(-2., r0[0]);
    BOOST_CHECK_EQUAL(-3., r0[1]);

    auto r1 = -(*_mat_dyn_2x2)[1];

    BOOST_CHECK_EQUAL(-1.0, r1[0]);
    BOOST_CHECK_EQUAL(-4.0, r1[1]);

  }
}


/* Operaciones binarias */

BOOST_FIXTURE_TEST_CASE(addition, MatrixRowTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a[0] + a[1];

  BOOST_CHECK_EQUAL(15, v[0]);
  BOOST_CHECK_EQUAL(14, v[1]);
  BOOST_CHECK_EQUAL(8, v[2]);
  BOOST_CHECK_EQUAL(10, v[3]);
  BOOST_CHECK_EQUAL(6, v[4]);

  a[0] += a[1];

  BOOST_CHECK_EQUAL(15, a[0][0]);
  BOOST_CHECK_EQUAL(14, a[0][1]);
  BOOST_CHECK_EQUAL(8, a[0][2]);
  BOOST_CHECK_EQUAL(10, a[0][3]);
  BOOST_CHECK_EQUAL(6, a[0][4]);

}

BOOST_FIXTURE_TEST_CASE(subtraction, MatrixRowTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a[0] - a[1];

  BOOST_CHECK_EQUAL(-3, v[0]);
  BOOST_CHECK_EQUAL(2, v[1]);
  BOOST_CHECK_EQUAL(4, v[2]);
  BOOST_CHECK_EQUAL(4, v[3]);
  BOOST_CHECK_EQUAL(0, v[4]);

  a[0] -= a[1];

  BOOST_CHECK_EQUAL(-3, a[0][0]);
  BOOST_CHECK_EQUAL(2, a[0][1]);
  BOOST_CHECK_EQUAL(4, a[0][2]);
  BOOST_CHECK_EQUAL(4, a[0][3]);
  BOOST_CHECK_EQUAL(0, a[0][4]);
}

BOOST_FIXTURE_TEST_CASE(multiplication, MatrixRowTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a[0] * a[1];

  BOOST_CHECK_EQUAL(54, v[0]);
  BOOST_CHECK_EQUAL(48, v[1]);
  BOOST_CHECK_EQUAL(12, v[2]);
  BOOST_CHECK_EQUAL(21, v[3]);
  BOOST_CHECK_EQUAL(9, v[4]);

  a[0] *= a[1];

  BOOST_CHECK_EQUAL(54, a[0][0]);
  BOOST_CHECK_EQUAL(48, a[0][1]);
  BOOST_CHECK_EQUAL(12, a[0][2]);
  BOOST_CHECK_EQUAL(21, a[0][3]);
  BOOST_CHECK_EQUAL(9, a[0][4]);

  Matrix<double, 5, 5> a_d{{6., 8., 6., 7., 3.},
                           {9., 6., 2., 3., 3.},
                           {8., 3., 2., 3., 3.},
                           {5., 3., 3., 7., 6.},
                           {5., 5., 7., 4., 7.}};

  auto v_d = a_d[0] * a_d[1];

  BOOST_CHECK_EQUAL(54., v_d[0]);
  BOOST_CHECK_EQUAL(48., v_d[1]);
  BOOST_CHECK_EQUAL(12., v_d[2]);
  BOOST_CHECK_EQUAL(21., v_d[3]);
  BOOST_CHECK_EQUAL(9.,  v_d[4]);

}

BOOST_FIXTURE_TEST_CASE(division, MatrixRowTest)
{
  Matrix<double, 5, 5> a{{6, 8, 6, 7, 3},
                         {9, 6, 2, 3, 3},
                         {8, 3, 2, 3, 3},
                         {5, 3, 3, 7, 6},
                         {5, 5, 7, 4, 7}};

  auto v = a[0] / a[1];

  BOOST_CHECK_CLOSE(0.6666666, v[0], 0.01);
  BOOST_CHECK_CLOSE(1.3333333, v[1], 0.01);
  BOOST_CHECK_CLOSE(3, v[2], 0.01);
  BOOST_CHECK_CLOSE(2.3333333, v[3], 0.01);
  BOOST_CHECK_CLOSE(1, v[4], 0.01);

  a[0] /= a[1];

  BOOST_CHECK_CLOSE(0.6666666, a[0][0], 0.01);
  BOOST_CHECK_CLOSE(1.3333333, a[0][1], 0.01);
  BOOST_CHECK_CLOSE(3, a[0][2], 0.01);
  BOOST_CHECK_CLOSE(2.3333333, a[0][3], 0.01);
  BOOST_CHECK_CLOSE(1, a[0][4], 0.01);
}

BOOST_FIXTURE_TEST_CASE(multiplication_row_scalar, MatrixRowTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a[2] * 10;

  BOOST_CHECK_EQUAL(80, v[0]);
  BOOST_CHECK_EQUAL(30, v[1]);
  BOOST_CHECK_EQUAL(20, v[2]);
  BOOST_CHECK_EQUAL(30, v[3]);
  BOOST_CHECK_EQUAL(30, v[4]);

  auto v2 = 10 * a[2];

  BOOST_CHECK_EQUAL(80, v2[0]);
  BOOST_CHECK_EQUAL(30, v2[1]);
  BOOST_CHECK_EQUAL(20, v2[2]);
  BOOST_CHECK_EQUAL(30, v2[3]);
  BOOST_CHECK_EQUAL(30, v2[4]);

  a[3] *= 10;

  BOOST_CHECK_EQUAL(50, a[3][0]);
  BOOST_CHECK_EQUAL(30, a[3][1]);
  BOOST_CHECK_EQUAL(30, a[3][2]);
  BOOST_CHECK_EQUAL(70, a[3][3]);
  BOOST_CHECK_EQUAL(60, a[3][4]);
}

BOOST_FIXTURE_TEST_CASE(division_row_scalar, MatrixRowTest)
{
  Matrix<double, 5, 5> a{{6, 8, 6, 7, 3},
                         {9, 6, 2, 3, 3},
                         {8, 3, 2, 3, 3},
                         {5, 3, 3, 7, 6},
                         {5, 5, 7, 4, 7}};

  auto v = a[2] / 10.;

  BOOST_CHECK_CLOSE(0.8, v[0], 0.01);
  BOOST_CHECK_CLOSE(0.3, v[1], 0.01);
  BOOST_CHECK_CLOSE(0.2, v[2], 0.01);
  BOOST_CHECK_CLOSE(0.3, v[3], 0.01);
  BOOST_CHECK_CLOSE(0.3, v[4], 0.01);


  a[3] /= 10.;

  BOOST_CHECK_CLOSE(0.5, a[3][0], 0.01);
  BOOST_CHECK_CLOSE(0.3, a[3][1], 0.01);
  BOOST_CHECK_CLOSE(0.3, a[3][2], 0.01);
  BOOST_CHECK_CLOSE(0.7, a[3][3], 0.01);
  BOOST_CHECK_CLOSE(0.6, a[3][4], 0.01);
}

BOOST_FIXTURE_TEST_CASE(asign_vector, MatrixRowTest)
{
  Matrix<int, 5, 5> a{{6,8,6,7,3},
                      {9,6,2,3,3},
                      {8,3,2,3,3},
                      {5,3,3,7,6},
                      {5,5,7,4,7}};

  Vector<int> v{1, 2, 3, 4, 5};

  a.row(0) = v;

  BOOST_CHECK_EQUAL(1, a.row(0)[0]);
  BOOST_CHECK_EQUAL(2, a.row(0)[1]);
  BOOST_CHECK_EQUAL(3, a.row(0)[2]);
  BOOST_CHECK_EQUAL(4, a.row(0)[3]);
  BOOST_CHECK_EQUAL(5, a.row(0)[4]);
}

BOOST_AUTO_TEST_SUITE_END()


/* */



BOOST_AUTO_TEST_SUITE(MatrixColTestSuite)

struct MatrixColTest
{
  MatrixColTest()
    : _mat_dyn_2x2(new Matrix<double>(2, 2))
  {
  }
  ~MatrixColTest()
  {
  }

  void setup()
  {
    _mat_dyn_2x2->at(0, 0) = 2.;
    _mat_dyn_2x2->at(0, 1) = 3.;
    _mat_dyn_2x2->at(1, 0) = 1.;
    _mat_dyn_2x2->at(1, 1) = 4.;

    _mat_3x3_d[0][0] = 1.5;
    _mat_3x3_d[0][1] = 0.0;
    _mat_3x3_d[0][2] = 2.5;
    _mat_3x3_d[1][0] = 1.0;
    _mat_3x3_d[1][1] = 1.0;
    _mat_3x3_d[1][2] = 1.2;
    _mat_3x3_d[2][0] = 1.3;
    _mat_3x3_d[2][1] = 2.6;
    _mat_3x3_d[2][2] = 0.3;
  }

  void teardown()
  {

  }

  Matrix<double> *_mat_dyn_2x2;
  Matrix<double, 3, 3> _mat_3x3_d;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, MatrixColTest)
{
  auto v1 = (*_mat_dyn_2x2)[1][0];
  (*_mat_dyn_2x2)[1][0] = 2.;
  auto v2 = (*_mat_dyn_2x2)[1][0];

  auto v3 = _mat_3x3_d[1][1];
  _mat_3x3_d[1][1] = 10.;
  auto v4 = _mat_3x3_d[1][1];
}

BOOST_FIXTURE_TEST_CASE(iterate, MatrixColTest)
{
  {
    auto c0 = _mat_3x3_d.col(0);
    auto it0 = c0.begin();
    BOOST_CHECK_EQUAL(1.5, *it0);
    it0++;
    BOOST_CHECK_EQUAL(1.0, *it0);
    it0++;
    BOOST_CHECK_EQUAL(1.3, *it0);
    it0++;
    BOOST_CHECK(it0 == c0.end());

    auto c1 = _mat_3x3_d.col(1);
    auto it1 = c1.begin();
    BOOST_CHECK_EQUAL(0.0, *it1);
    it1++;
    BOOST_CHECK_EQUAL(1.0, *it1);
    it1++;
    BOOST_CHECK_EQUAL(2.6, *it1);
    it1++;
    BOOST_CHECK(it1 == c1.end());

    auto c2 = _mat_3x3_d.col(2);
    auto it2 = c2.begin();
    BOOST_CHECK_EQUAL(2.5, *it2);
    it2++;
    BOOST_CHECK_EQUAL(1.2, *it2);
    it2++;
    BOOST_CHECK_EQUAL(0.3, *it2);
    it2++;
    BOOST_CHECK(it2 == c2.end());

  }

  {
    auto c0 = _mat_dyn_2x2->col(0);
    auto it0 = c0.begin();
    BOOST_CHECK_EQUAL(2., *it0);
    it0++;
    BOOST_CHECK_EQUAL(1., *it0);
    it0++;
    BOOST_CHECK(it0 == c0.end());

    auto c1 = _mat_dyn_2x2->col(1);
    auto it1 = c1.begin();
    BOOST_CHECK_EQUAL(3.0, *it1);
    it1++;
    BOOST_CHECK_EQUAL(4.0, *it1);
    it1++;
    BOOST_CHECK(it1 == c1.end());
  }

}

BOOST_FIXTURE_TEST_CASE(size, MatrixColTest)
{
  auto c0 = _mat_3x3_d.col(0);
  BOOST_CHECK_EQUAL(3, c0.size());
  auto c1 = _mat_3x3_d.col(1);
  BOOST_CHECK_EQUAL(3, c1.size());
}

BOOST_FIXTURE_TEST_CASE(value_at, MatrixColTest)
{
  {
    auto c0 = _mat_3x3_d.col(0);

    BOOST_CHECK_EQUAL(1.5, c0[0]);
    BOOST_CHECK_EQUAL(1.0, c0[1]);
    BOOST_CHECK_EQUAL(1.3, c0[2]);

    auto c1 = _mat_3x3_d.col(1);

    BOOST_CHECK_EQUAL(0.0, c1[0]);
    BOOST_CHECK_EQUAL(1.0, c1[1]);
    BOOST_CHECK_EQUAL(2.6, c1[2]);

    auto c2 = _mat_3x3_d.col(2);

    BOOST_CHECK_EQUAL(2.5, c2[0]);
    BOOST_CHECK_EQUAL(1.2, c2[1]);
    BOOST_CHECK_EQUAL(0.3, c2[2]);

  }

  {
    auto c0 = _mat_dyn_2x2->col(0);

    BOOST_CHECK_EQUAL(2., c0[0]);
    BOOST_CHECK_EQUAL(1., c0[1]);

    auto c1 = _mat_dyn_2x2->col(1);

    BOOST_CHECK_EQUAL(3.0, c1[0]);
    BOOST_CHECK_EQUAL(4.0, c1[1]);

  }
}

BOOST_FIXTURE_TEST_CASE(asign, MatrixColTest)
{
  Matrix<int, 5, 5> a{ {6, 8,6,7,3},
                       {9,6,2,3,3},
                       {8,3,2,3,3},
                       {5,3,3,7,6},
                       {5,5,7,4,7} };

  a.col(0) = 0;
  a.col(1) = 1;
  a.col(2) = 2;
  a.col(3) = 3;
  a.col(4) = 4;

  BOOST_CHECK_EQUAL(0, a[0][0]);
  BOOST_CHECK_EQUAL(0, a[1][0]);
  BOOST_CHECK_EQUAL(0, a[2][0]);
  BOOST_CHECK_EQUAL(0, a[3][0]);
  BOOST_CHECK_EQUAL(0, a[4][0]);
  BOOST_CHECK_EQUAL(1, a[0][1]);
  BOOST_CHECK_EQUAL(1, a[1][1]);
  BOOST_CHECK_EQUAL(1, a[2][1]);
  BOOST_CHECK_EQUAL(1, a[3][1]);
  BOOST_CHECK_EQUAL(1, a[4][1]);
  BOOST_CHECK_EQUAL(2, a[0][2]);
  BOOST_CHECK_EQUAL(2, a[1][2]);
  BOOST_CHECK_EQUAL(2, a[2][2]);
  BOOST_CHECK_EQUAL(2, a[3][2]);
  BOOST_CHECK_EQUAL(2, a[4][2]);
  BOOST_CHECK_EQUAL(3, a[0][3]);
  BOOST_CHECK_EQUAL(3, a[1][3]);
  BOOST_CHECK_EQUAL(3, a[2][3]);
  BOOST_CHECK_EQUAL(3, a[3][3]);
  BOOST_CHECK_EQUAL(3, a[4][3]);
  BOOST_CHECK_EQUAL(4, a[0][4]);
  BOOST_CHECK_EQUAL(4, a[1][4]);
  BOOST_CHECK_EQUAL(4, a[2][4]);
  BOOST_CHECK_EQUAL(4, a[3][4]);
  BOOST_CHECK_EQUAL(4, a[4][4]);

}

BOOST_FIXTURE_TEST_CASE(plus, MatrixColTest)
{
  {
    Vector<double> c0 = +_mat_3x3_d.col(0);

    BOOST_CHECK_EQUAL(1.5, c0[0]);
    BOOST_CHECK_EQUAL(1.0, c0[1]);
    BOOST_CHECK_EQUAL(1.3, c0[2]);

    auto c1 = +_mat_3x3_d.col(1);

    BOOST_CHECK_EQUAL(0.0, c1[0]);
    BOOST_CHECK_EQUAL(1.0, c1[1]);
    BOOST_CHECK_EQUAL(2.6, c1[2]);

    auto c2 = +_mat_3x3_d.col(2);

    BOOST_CHECK_EQUAL(2.5, c2[0]);
    BOOST_CHECK_EQUAL(1.2, c2[1]);
    BOOST_CHECK_EQUAL(0.3, c2[2]);

  }

  {
    auto c0 = +_mat_dyn_2x2->col(0);

    BOOST_CHECK_EQUAL(2., c0[0]);
    BOOST_CHECK_EQUAL(1., c0[1]);

    auto c1 = +_mat_dyn_2x2->col(1);

    BOOST_CHECK_EQUAL(3.0, c1[0]);
    BOOST_CHECK_EQUAL(4.0, c1[1]);
  }
}

BOOST_FIXTURE_TEST_CASE(minus, MatrixColTest)
{
  {
    auto c0 = -_mat_3x3_d.col(0);

    BOOST_CHECK_EQUAL(-1.5, c0[0]);
    BOOST_CHECK_EQUAL(-1.0, c0[1]);
    BOOST_CHECK_EQUAL(-1.3, c0[2]);

    auto c1 = -_mat_3x3_d.col(1);

    BOOST_CHECK_EQUAL(-0.0, c1[0]);
    BOOST_CHECK_EQUAL(-1.0, c1[1]);
    BOOST_CHECK_EQUAL(-2.6, c1[2]);

    auto c2 = -_mat_3x3_d.col(2);

    BOOST_CHECK_EQUAL(-2.5, c2[0]);
    BOOST_CHECK_EQUAL(-1.2, c2[1]);
    BOOST_CHECK_EQUAL(-0.3, c2[2]);

  }

  {
    auto c0 = -_mat_dyn_2x2->col(0);

    BOOST_CHECK_EQUAL(-2., c0[0]);
    BOOST_CHECK_EQUAL(-1., c0[1]);

    auto c1 = -_mat_dyn_2x2->col(1);

    BOOST_CHECK_EQUAL(-3.0, c1[0]);
    BOOST_CHECK_EQUAL(-4.0, c1[1]);
  }
}

BOOST_FIXTURE_TEST_CASE(addition, MatrixColTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a.col(0) + a.col(1);

  BOOST_CHECK_EQUAL(14, v[0]);
  BOOST_CHECK_EQUAL(15, v[1]);
  BOOST_CHECK_EQUAL(11, v[2]);
  BOOST_CHECK_EQUAL(8, v[3]);
  BOOST_CHECK_EQUAL(10, v[4]);

  a.col(0) += a.col(1);

  BOOST_CHECK_EQUAL(14, a.col(0)[0]);
  BOOST_CHECK_EQUAL(15, a.col(0)[1]);
  BOOST_CHECK_EQUAL(11, a.col(0)[2]);
  BOOST_CHECK_EQUAL(8, a.col(0)[3]);
  BOOST_CHECK_EQUAL(10, a.col(0)[4]);

}

BOOST_FIXTURE_TEST_CASE(subtraction, MatrixColTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a.col(0) - a.col(1);

  BOOST_CHECK_EQUAL(-2, v[0]);
  BOOST_CHECK_EQUAL(3, v[1]);
  BOOST_CHECK_EQUAL(5, v[2]);
  BOOST_CHECK_EQUAL(2, v[3]);
  BOOST_CHECK_EQUAL(0, v[4]);

  a.col(0) -= a.col(1);

  BOOST_CHECK_EQUAL(-2, a.col(0)[0]);
  BOOST_CHECK_EQUAL(3, a.col(0)[1]);
  BOOST_CHECK_EQUAL(5, a.col(0)[2]);
  BOOST_CHECK_EQUAL(2, a.col(0)[3]);
  BOOST_CHECK_EQUAL(0, a.col(0)[4]);

}

BOOST_FIXTURE_TEST_CASE(multiplication, MatrixColTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a.col(0) * a.col(1);

  BOOST_CHECK_EQUAL(48, v[0]);
  BOOST_CHECK_EQUAL(54, v[1]);
  BOOST_CHECK_EQUAL(24, v[2]);
  BOOST_CHECK_EQUAL(15, v[3]);
  BOOST_CHECK_EQUAL(25, v[4]);

  a.col(0) *= a.col(1);

  BOOST_CHECK_EQUAL(48, a.col(0)[0]);
  BOOST_CHECK_EQUAL(54, a.col(0)[1]);
  BOOST_CHECK_EQUAL(24, a.col(0)[2]);
  BOOST_CHECK_EQUAL(15, a.col(0)[3]);
  BOOST_CHECK_EQUAL(25, a.col(0)[4]);

}

BOOST_FIXTURE_TEST_CASE(division, MatrixColTest)
{
  Matrix<double, 5, 5> a{{6, 8, 6, 7, 3},
                         {9, 6, 2, 3, 3},
                         {8, 3, 2, 3, 3},
                         {5, 3, 3, 7, 6},
                         {5, 5, 7, 4, 7}};

  auto v = a.col(0) / a.col(1);

  BOOST_CHECK_EQUAL(6 / 8., v[0]);
  BOOST_CHECK_EQUAL(9 / 6., v[1]);
  BOOST_CHECK_EQUAL(8 / 3., v[2]);
  BOOST_CHECK_EQUAL(5 / 3., v[3]);
  BOOST_CHECK_EQUAL(1, v[4]);

  a.col(0) /= a.col(1);

  BOOST_CHECK_EQUAL(6 / 8., a.col(0)[0]);
  BOOST_CHECK_EQUAL(9 / 6., a.col(0)[1]);
  BOOST_CHECK_EQUAL(8 / 3., a.col(0)[2]);
  BOOST_CHECK_EQUAL(5 / 3., a.col(0)[3]);
  BOOST_CHECK_EQUAL(1, a.col(0)[4]);

}

BOOST_FIXTURE_TEST_CASE(multiplication_row_scalar, MatrixColTest)
{
  Matrix<int, 5, 5> a{{6, 8, 6, 7, 3},
                      {9, 6, 2, 3, 3},
                      {8, 3, 2, 3, 3},
                      {5, 3, 3, 7, 6},
                      {5, 5, 7, 4, 7}};

  auto v = a.col(0) * 10;

  BOOST_CHECK_EQUAL(60, v[0]);
  BOOST_CHECK_EQUAL(90, v[1]);
  BOOST_CHECK_EQUAL(80, v[2]);
  BOOST_CHECK_EQUAL(50, v[3]);
  BOOST_CHECK_EQUAL(50, v[4]);

  auto v2 = 10 * a.col(1);

  BOOST_CHECK_EQUAL(80, v2[0]);
  BOOST_CHECK_EQUAL(60, v2[1]);
  BOOST_CHECK_EQUAL(30, v2[2]);
  BOOST_CHECK_EQUAL(30, v2[3]);
  BOOST_CHECK_EQUAL(50, v2[4]);

  a.col(2) *= 10;

  BOOST_CHECK_EQUAL(60, a.col(2)[0]);
  BOOST_CHECK_EQUAL(20, a.col(2)[1]);
  BOOST_CHECK_EQUAL(20, a.col(2)[2]);
  BOOST_CHECK_EQUAL(30, a.col(2)[3]);
  BOOST_CHECK_EQUAL(70, a.col(2)[4]);
}

BOOST_FIXTURE_TEST_CASE(division_row_scalar, MatrixColTest)
{
  Matrix<double, 5, 5> a{{6, 8, 6, 7, 3},
                         {9, 6, 2, 3, 3},
                         {8, 3, 2, 3, 3},
                         {5, 3, 3, 7, 6},
                         {5, 5, 7, 4, 7}};

  auto v = a.col(0) / 10.;

  BOOST_CHECK_EQUAL(6 / 10., v[0]);
  BOOST_CHECK_EQUAL(9 / 10., v[1]);
  BOOST_CHECK_EQUAL(8 / 10., v[2]);
  BOOST_CHECK_EQUAL(5 / 10., v[3]);
  BOOST_CHECK_EQUAL(5 / 10., v[4]);

  a.col(1) /= 10.;

  BOOST_CHECK_EQUAL(8 / 10., a.col(1)[0]);
  BOOST_CHECK_EQUAL(6 / 10., a.col(1)[1]);
  BOOST_CHECK_EQUAL(3 / 10., a.col(1)[2]);
  BOOST_CHECK_EQUAL(3 / 10., a.col(1)[3]);
  BOOST_CHECK_EQUAL(5 / 10., a.col(1)[4]);

}

BOOST_FIXTURE_TEST_CASE(asign_vector, MatrixColTest)
{
  Matrix<int, 5, 5> a{{6,8,6,7,3},
                      {9,6,2,3,3},
                      {8,3,2,3,3},
                      {5,3,3,7,6},
                      {5,5,7,4,7}};

  Vector<int> v{1, 2, 3, 4, 5};

  a.col(0) = v;

  BOOST_CHECK_EQUAL(1, a.col(0)[0]);
  BOOST_CHECK_EQUAL(2, a.col(0)[1]);
  BOOST_CHECK_EQUAL(3, a.col(0)[2]);
  BOOST_CHECK_EQUAL(4, a.col(0)[3]);
  BOOST_CHECK_EQUAL(5, a.col(0)[4]);
}

BOOST_AUTO_TEST_SUITE_END()


/* */


BOOST_AUTO_TEST_SUITE(MatrixBlockTestSuite)

struct MatrixBlockTest
{
    MatrixBlockTest()
        : block(nullptr),
        block2(nullptr)
    {
    }
    ~MatrixBlockTest()
    {
        if (block) {
            delete block;
        }

        if (block2)
            delete block2;
    }

    void setup()
    {
        mat_3x3_i = Matrix<int, 3, 3>::identity();
        block = new internal::MatrixBlock<int>(mat_3x3_i.data(), mat_3x3_i.rows(), mat_3x3_i.cols(), 1, 2, 1, 2);

        m_5x5 = {{6, 8, 6, 7, 3},
                  {9, 6, 2, 3, 3},
                  {8, 3, 2, 3, 3},
                  {5, 3, 3, 7, 6},
                  {5, 5, 7, 4, 7}};
        block2 = new internal::MatrixBlock<int>(m_5x5.data(), m_5x5.rows(), m_5x5.cols(), 2, 4, 1, 4);

        mat_10x10_d = {0.81, 7.45, 1.17, 3.44, 4.13, 5.67, 4.57, 2.03, 5.53, 5.25,
                       6.69, 7.87, 1.70, 4.32, 6.33, 0.67, 8.99, 7.63, 7.08, 7.24,
                       6.39, 3.54, 6.47, 7.15, 0.37, 5.52, 3.21, 8.52, 3.20, 5.83,
                       7.86, 5.01, 5.82, 0.94, 8.94, 6.53, 2.04, 5.91, 5.94, 5.18,
                       0.11, 3.99, 1.09, 2.89, 5.71, 6.51, 6.84, 4.33, 3.46, 8.86,
                       0.78, 2.50, 2.20, 5.76, 4.13, 2.64, 6.41, 4.38, 3.75, 8.01,
                       4.52, 0.05, 1.16, 2.97, 2.71, 1.97, 8.81, 3.89, 2.37, 3.86,
                       6.65, 2.05, 8.86, 3.24, 0.83, 7.56, 8.70, 6.93, 0.93, 0.79,
                       2.09, 4.23, 6.07, 8.07, 7.64, 0.80, 7.43, 0.15, 0.07, 7.02,
                       2.24, 3.95, 5.79, 3.45, 3.43, 4.62, 5.88, 2.01, 3.04, 3.06};

        mat_5x5_d = {7.35, 2.66, 8.25, 8.28, 3.86,
                     6.51, 6.39, 1.18, 8.83, 3.41,
                     0.11, 7.88, 2.41, 3.63, 7.20,
                     4.55, 2.46, 3.81, 3.20, 6.69,
                     0.64, 4.79, 6.49, 4.00, 1.22,
                     1.51, 2.13, 2.42, 7.35, 7.44,
                     3.01, 3.90, 7.55, 0.08, 8.03,
                     8.21, 7.63, 2.82, 0.79, 7.68,
                     2.00, 4.53, 2.41, 7.54, 0.27,
                     3.62, 5.66, 7.92, 2.53, 3.01};

        //block_2 = new internal::MatrixBlock2<Matrix<double, 10, 10>>(&mat_10x10_d,0, 3, 0, 3);
    }

    void teardown()
    {

    }

    Matrix<int, 3, 3> mat_3x3_i;
    Matrix<int, 5, 5> m_5x5;
    Matrix<double, 10, 10> mat_10x10_d;
    Matrix<double, 5, 5> mat_5x5_d;
    internal::MatrixBlock<int> *block;
    internal::MatrixBlock<int> *block2;

    //internal::MatrixBlock2<Matrix<double, 10, 10>> *block_2;
};


BOOST_FIXTURE_TEST_CASE(constructor, MatrixBlockTest)
{

  BOOST_CHECK_EQUAL(1, (*block)(0, 0));
  BOOST_CHECK_EQUAL(0, (*block)(0, 1));
  BOOST_CHECK_EQUAL(0, (*block)(1, 0));
  BOOST_CHECK_EQUAL(1, (*block)(1, 1));

  BOOST_CHECK_EQUAL(3, (*block2)(0, 0));
  BOOST_CHECK_EQUAL(2, (*block2)(0, 1));
  BOOST_CHECK_EQUAL(3, (*block2)(0, 2));
  BOOST_CHECK_EQUAL(3, (*block2)(0, 3));
  BOOST_CHECK_EQUAL(3, (*block2)(1, 0));
  BOOST_CHECK_EQUAL(3, (*block2)(1, 1));
  BOOST_CHECK_EQUAL(7, (*block2)(1, 2));
  BOOST_CHECK_EQUAL(6, (*block2)(1, 3));
  BOOST_CHECK_EQUAL(5, (*block2)(2, 0));
  BOOST_CHECK_EQUAL(7, (*block2)(2, 1));
  BOOST_CHECK_EQUAL(4, (*block2)(2, 2));
  BOOST_CHECK_EQUAL(7, (*block2)(2, 3));
}

//BOOST_FIXTURE_TEST_CASE(array_index_operator, MatrixBlockTest)
//{
//  BOOST_CHECK_EQUAL(1, (*block)[0][0]);
//  BOOST_CHECK_EQUAL(0, (*block)[0][1]);
//  BOOST_CHECK_EQUAL(0, (*block)[1][0]);
//  BOOST_CHECK_EQUAL(1, (*block)[1][1]);
//
//  BOOST_CHECK_EQUAL(3, (*block2)[0][0]);
//  BOOST_CHECK_EQUAL(2, (*block2)[0][1]);
//  BOOST_CHECK_EQUAL(3, (*block2)[0][2]);
//  BOOST_CHECK_EQUAL(3, (*block2)[0][3]);
//  BOOST_CHECK_EQUAL(3, (*block2)[1][0]);
//  BOOST_CHECK_EQUAL(3, (*block2)[1][1]);
//  BOOST_CHECK_EQUAL(7, (*block2)[1][2]);
//  BOOST_CHECK_EQUAL(6, (*block2)[1][3]);
//  BOOST_CHECK_EQUAL(5, (*block2)[2][0]);
//  BOOST_CHECK_EQUAL(7, (*block2)[2][1]);
//  BOOST_CHECK_EQUAL(4, (*block2)[2][2]);
//  BOOST_CHECK_EQUAL(7, (*block2)[2][3]);
//}

BOOST_FIXTURE_TEST_CASE(assing_matrix, MatrixBlockTest)
{
  (*block) = Matrix<int, 2, 2>::ones();
  BOOST_CHECK_EQUAL(1, block->at(0, 0));
  BOOST_CHECK_EQUAL(1, block->at(0, 1));
  BOOST_CHECK_EQUAL(1, block->at(1, 0));
  BOOST_CHECK_EQUAL(1, block->at(1, 1));

  (*block) = Matrix<double>::ones(2, 2);
  BOOST_CHECK_EQUAL(1, block->at(0, 0));
  BOOST_CHECK_EQUAL(1, block->at(0, 1));
  BOOST_CHECK_EQUAL(1, block->at(1, 0));
  BOOST_CHECK_EQUAL(1, block->at(1, 1));

  m_5x5.block(0, 1, 0, 1) = m_5x5.block(2, 3, 2, 3);
  BOOST_CHECK_EQUAL(2, m_5x5.block(0, 1, 0, 1)(0, 0));
  BOOST_CHECK_EQUAL(3, m_5x5.block(0, 1, 0, 1)(0, 1));
  BOOST_CHECK_EQUAL(3, m_5x5.block(0, 1, 0, 1)(1, 0));
  BOOST_CHECK_EQUAL(7, m_5x5.block(0, 1, 0, 1)(1, 1));

  m_5x5.block(0, 2, 0, 2) = m_5x5.block(2, 4, 2, 4);

  BOOST_CHECK_EQUAL(2, m_5x5.block(0, 2, 0, 2)(0, 0));
  BOOST_CHECK_EQUAL(3, m_5x5.block(0, 2, 0, 2)(0, 1));
  BOOST_CHECK_EQUAL(3, m_5x5.block(0, 2, 0, 2)(0, 2));
  BOOST_CHECK_EQUAL(3, m_5x5.block(0, 2, 0, 2)(1, 0));
  BOOST_CHECK_EQUAL(7, m_5x5.block(0, 2, 0, 2)(1, 1));
  BOOST_CHECK_EQUAL(6, m_5x5.block(0, 2, 0, 2)(1, 2));
  BOOST_CHECK_EQUAL(7, m_5x5.block(0, 2, 0, 2)(2, 0));
  BOOST_CHECK_EQUAL(4, m_5x5.block(0, 2, 0, 2)(2, 1));
  BOOST_CHECK_EQUAL(7, m_5x5.block(0, 2, 0, 2)(2, 2));

  m_5x5.block(2, 4, 2, 4) = m_5x5.block(1, 3, 1, 3);

  BOOST_CHECK_EQUAL(7, m_5x5.block(2, 4, 2, 4)(0, 0));
  BOOST_CHECK_EQUAL(6, m_5x5.block(2, 4, 2, 4)(0, 1));
  BOOST_CHECK_EQUAL(3, m_5x5.block(2, 4, 2, 4)(0, 2));
  BOOST_CHECK_EQUAL(4, m_5x5.block(2, 4, 2, 4)(1, 0));
  BOOST_CHECK_EQUAL(7, m_5x5.block(2, 4, 2, 4)(1, 1));
  BOOST_CHECK_EQUAL(3, m_5x5.block(2, 4, 2, 4)(1, 2));
  BOOST_CHECK_EQUAL(3, m_5x5.block(2, 4, 2, 4)(2, 0));
  BOOST_CHECK_EQUAL(3, m_5x5.block(2, 4, 2, 4)(2, 1));
  BOOST_CHECK_EQUAL(7, m_5x5.block(2, 4, 2, 4)(2, 2));

}

BOOST_FIXTURE_TEST_CASE(at, MatrixBlockTest)
{
  BOOST_CHECK_EQUAL(1, block->at(0, 0));
  BOOST_CHECK_EQUAL(0, block->at(0, 1));
  BOOST_CHECK_EQUAL(0, block->at(1, 0));
  BOOST_CHECK_EQUAL(1, block->at(1, 1));

  BOOST_CHECK_EQUAL(3, block2->at(0, 0));
  BOOST_CHECK_EQUAL(2, block2->at(0, 1));
  BOOST_CHECK_EQUAL(3, block2->at(0, 2));
  BOOST_CHECK_EQUAL(3, block2->at(0, 3));
  BOOST_CHECK_EQUAL(3, block2->at(1, 0));
  BOOST_CHECK_EQUAL(3, block2->at(1, 1));
  BOOST_CHECK_EQUAL(7, block2->at(1, 2));
  BOOST_CHECK_EQUAL(6, block2->at(1, 3));
  BOOST_CHECK_EQUAL(5, block2->at(2, 0));
  BOOST_CHECK_EQUAL(7, block2->at(2, 1));
  BOOST_CHECK_EQUAL(4, block2->at(2, 2));
  BOOST_CHECK_EQUAL(7, block2->at(2, 3));
}

BOOST_FIXTURE_TEST_CASE(rows, MatrixBlockTest)
{
  BOOST_CHECK_EQUAL(2, block->rows());
  BOOST_CHECK_EQUAL(3, block2->rows());
}

BOOST_FIXTURE_TEST_CASE(cols, MatrixBlockTest)
{
  BOOST_CHECK_EQUAL(2, block->cols());
  BOOST_CHECK_EQUAL(4, block2->cols());
}

BOOST_FIXTURE_TEST_CASE(determinant2x2, MatrixBlockTest)
{
  auto block = mat_10x10_d.block(0, 1, 0, 1);
  BOOST_CHECK_CLOSE(-43.4658, block.determinant(), 0.1);

  block = mat_10x10_d.block(5, 6, 6, 7);
  BOOST_CHECK_CLOSE(-13.65289, block.determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant3x3, MatrixBlockTest)
{
  auto block = mat_10x10_d.block(0, 2, 0, 2);
  BOOST_CHECK_CLOSE(-236.298795, block.determinant(), 0.1);

  block = mat_10x10_d.block(5, 7, 5, 7);
  BOOST_CHECK_CLOSE(-43.821873, block.determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinant4x4, MatrixBlockTest)
{
  auto block = mat_10x10_d.block(1, 4, 1, 4);
  BOOST_CHECK_CLOSE(-869.83954904, block.determinant(), 0.1);

  block = mat_10x10_d.block(6, 9, 6, 9);
  BOOST_CHECK_CLOSE(-14.51687716, block.determinant(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(determinantnxn, MatrixBlockTest)
{
  auto block = mat_10x10_d.block(1, 6, 1, 6);
  BOOST_CHECK_CLOSE(51766.061607744239, block.determinant(), 0.1);

  block = mat_10x10_d.block(1, 6, 3, 8);
  BOOST_CHECK_CLOSE(-4932.993342108, block.determinant(), 0.1);
}

/* Unary arithmetic operators */

BOOST_FIXTURE_TEST_CASE(unary_plus, MatrixBlockTest)
{
  auto _block = +(*block);
  BOOST_CHECK_EQUAL(1, _block(0, 0));
  BOOST_CHECK_EQUAL(0, _block(0, 1));
  BOOST_CHECK_EQUAL(0, _block(1, 0));
  BOOST_CHECK_EQUAL(1, _block(1, 1));

  _block = +(*block2);

  BOOST_CHECK_EQUAL(3, _block(0, 0));
  BOOST_CHECK_EQUAL(2, _block(0, 1));
  BOOST_CHECK_EQUAL(3, _block(0, 2));
  BOOST_CHECK_EQUAL(3, _block(0, 3));
  BOOST_CHECK_EQUAL(3, _block(1, 0));
  BOOST_CHECK_EQUAL(3, _block(1, 1));
  BOOST_CHECK_EQUAL(7, _block(1, 2));
  BOOST_CHECK_EQUAL(6, _block(1, 3));
  BOOST_CHECK_EQUAL(5, _block(2, 0));
  BOOST_CHECK_EQUAL(7, _block(2, 1));
  BOOST_CHECK_EQUAL(4, _block(2, 2));
  BOOST_CHECK_EQUAL(7, _block(2, 3));

  auto _block2 = +mat_10x10_d.block(2, 4, 0, 2);

  BOOST_CHECK_EQUAL(6.39, _block2(0, 0));
  BOOST_CHECK_EQUAL(3.54, _block2(0, 1));
  BOOST_CHECK_EQUAL(7.86, _block2(1, 0));
  BOOST_CHECK_EQUAL(5.01, _block2(1, 1));
  BOOST_CHECK_EQUAL(0.11, _block2(2, 0));
  BOOST_CHECK_EQUAL(3.99, _block2(2, 1));
}

BOOST_FIXTURE_TEST_CASE(unary_minus, MatrixBlockTest)
{
  auto _block = -(*block);
  BOOST_CHECK_EQUAL(-1, _block(0, 0));
  BOOST_CHECK_EQUAL(-0, _block(0, 1));
  BOOST_CHECK_EQUAL(-0, _block(1, 0));
  BOOST_CHECK_EQUAL(-1, _block(1, 1));

  _block = -(*block2);

  BOOST_CHECK_EQUAL(-3, _block(0, 0));
  BOOST_CHECK_EQUAL(-2, _block(0, 1));
  BOOST_CHECK_EQUAL(-3, _block(0, 2));
  BOOST_CHECK_EQUAL(-3, _block(0, 3));
  BOOST_CHECK_EQUAL(-3, _block(1, 0));
  BOOST_CHECK_EQUAL(-3, _block(1, 1));
  BOOST_CHECK_EQUAL(-7, _block(1, 2));
  BOOST_CHECK_EQUAL(-6, _block(1, 3));
  BOOST_CHECK_EQUAL(-5, _block(2, 0));
  BOOST_CHECK_EQUAL(-7, _block(2, 1));
  BOOST_CHECK_EQUAL(-4, _block(2, 2));
  BOOST_CHECK_EQUAL(-7, _block(2, 3));

  auto _block2 = -mat_10x10_d.block(2, 4, 0, 2);

  BOOST_CHECK_EQUAL(-6.39, _block2(0, 0));
  BOOST_CHECK_EQUAL(-3.54, _block2(0, 1));
  BOOST_CHECK_EQUAL(-7.86, _block2(1, 0));
  BOOST_CHECK_EQUAL(-5.01, _block2(1, 1));
  BOOST_CHECK_EQUAL(-0.11, _block2(2, 0));
  BOOST_CHECK_EQUAL(-3.99, _block2(2, 1));
}


/* Operaciones binarias entre matrices */

/// Suma o adición de matrices

  //mat_10x10_d = {0.81, 7.45, 1.17, 3.44, 4.13, 5.67, 4.57, 2.03, 5.53, 5.25,
  //               6.69, 7.87, 1.70, 4.32, 6.33, 0.67, 8.99, 7.63, 7.08, 7.24,
  //               6.39, 3.54, 6.47, 7.15, 0.37, 5.52, 3.21, 8.52, 3.20, 5.83,
  //               7.86, 5.01, 5.82, 0.94, 8.94, 6.53, 2.04, 5.91, 5.94, 5.18,
  //               0.11, 3.99, 1.09, 2.89, 5.71, 6.51, 6.84, 4.33, 3.46, 8.86,
  //               0.78, 2.50, 2.20, 5.76, 4.13, 2.64, 6.41, 4.38, 3.75, 8.01,
  //               4.52, 0.05, 1.16, 2.97, 2.71, 1.97, 8.81, 3.89, 2.37, 3.86,
  //               6.65, 2.05, 8.86, 3.24, 0.83, 7.56, 8.70, 6.93, 0.93, 0.79,
  //               2.09, 4.23, 6.07, 8.07, 7.64, 0.80, 7.43, 0.15, 0.07, 7.02,
  //               2.24, 3.95, 5.79, 3.45, 3.43, 4.62, 5.88, 2.01, 3.04, 3.06};

BOOST_FIXTURE_TEST_CASE(addition, MatrixBlockTest)
{
  {
    auto block1 = mat_5x5_d.block(0, 2, 0, 3);
    auto block2 = mat_10x10_d.block(3, 5, 4, 7);                   

    Matrix<double> mat = mat_5x5_d.block(0, 2, 0, 3) + mat_10x10_d.block(3, 5, 4, 7);

    BOOST_CHECK_CLOSE(16.29, mat(0, 0), 0.1);
    BOOST_CHECK_CLOSE(9.19, mat(0, 1), 0.1);
    BOOST_CHECK_CLOSE(10.29, mat(0, 2), 0.1);
    BOOST_CHECK_CLOSE(14.19, mat(0, 3), 0.1);
    BOOST_CHECK_CLOSE(12.22, mat(1, 0), 0.1);
    BOOST_CHECK_CLOSE(12.9, mat(1, 1), 0.1);
    BOOST_CHECK_CLOSE(8.02, mat(1, 2), 0.1);
    BOOST_CHECK_CLOSE(13.16, mat(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.24, mat(2, 0), 0.1);
    BOOST_CHECK_CLOSE(10.52, mat(2, 1), 0.1);
    BOOST_CHECK_CLOSE(8.82, mat(2, 2), 0.1);
    BOOST_CHECK_CLOSE(8.01, mat(2, 3), 0.1);

    // Se comprueba que los bloques no se han modificado

    BOOST_CHECK_CLOSE(7.35, block1(0, 0), 0.1);
    BOOST_CHECK_CLOSE(2.66, block1(0, 1), 0.1);
    BOOST_CHECK_CLOSE(8.25, block1(0, 2), 0.1);
    BOOST_CHECK_CLOSE(8.28, block1(0, 3), 0.1);
    BOOST_CHECK_CLOSE(6.51, block1(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.39, block1(1, 1), 0.1);
    BOOST_CHECK_CLOSE(1.18, block1(1, 2), 0.1);
    BOOST_CHECK_CLOSE(8.83, block1(1, 3), 0.1);
    BOOST_CHECK_CLOSE(0.11, block1(2, 0), 0.1);
    BOOST_CHECK_CLOSE(7.88, block1(2, 1), 0.1);
    BOOST_CHECK_CLOSE(2.41, block1(2, 2), 0.1);
    BOOST_CHECK_CLOSE(3.63, block1(2, 3), 0.1);
                           
    BOOST_CHECK_CLOSE(8.94, block2(0, 0), 0.1);
    BOOST_CHECK_CLOSE(6.53, block2(0, 1), 0.1);
    BOOST_CHECK_CLOSE(2.04, block2(0, 2), 0.1);
    BOOST_CHECK_CLOSE(5.91, block2(0, 3), 0.1);
    BOOST_CHECK_CLOSE(5.71, block2(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.51, block2(1, 1), 0.1);
    BOOST_CHECK_CLOSE(6.84, block2(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.33, block2(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.13, block2(2, 0), 0.1);
    BOOST_CHECK_CLOSE(2.64, block2(2, 1), 0.1);
    BOOST_CHECK_CLOSE(6.41, block2(2, 2), 0.1);
    BOOST_CHECK_CLOSE(4.38, block2(2, 3), 0.1);

    mat = block1 + block2;

    BOOST_CHECK_CLOSE(16.29, mat(0, 0), 0.1);
    BOOST_CHECK_CLOSE(9.19, mat(0, 1), 0.1);
    BOOST_CHECK_CLOSE(10.29, mat(0, 2), 0.1);
    BOOST_CHECK_CLOSE(14.19, mat(0, 3), 0.1);
    BOOST_CHECK_CLOSE(12.22, mat(1, 0), 0.1);
    BOOST_CHECK_CLOSE(12.9, mat(1, 1), 0.1);
    BOOST_CHECK_CLOSE(8.02, mat(1, 2), 0.1);
    BOOST_CHECK_CLOSE(13.16, mat(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.24, mat(2, 0), 0.1);
    BOOST_CHECK_CLOSE(10.52, mat(2, 1), 0.1);
    BOOST_CHECK_CLOSE(8.82, mat(2, 2), 0.1);
    BOOST_CHECK_CLOSE(8.01, mat(2, 3), 0.1);

    // Se comprueba que los bloques no se han modificado

    BOOST_CHECK_CLOSE(7.35, block1(0, 0), 0.1);
    BOOST_CHECK_CLOSE(2.66, block1(0, 1), 0.1);
    BOOST_CHECK_CLOSE(8.25, block1(0, 2), 0.1);
    BOOST_CHECK_CLOSE(8.28, block1(0, 3), 0.1);
    BOOST_CHECK_CLOSE(6.51, block1(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.39, block1(1, 1), 0.1);
    BOOST_CHECK_CLOSE(1.18, block1(1, 2), 0.1);
    BOOST_CHECK_CLOSE(8.83, block1(1, 3), 0.1);
    BOOST_CHECK_CLOSE(0.11, block1(2, 0), 0.1);
    BOOST_CHECK_CLOSE(7.88, block1(2, 1), 0.1);
    BOOST_CHECK_CLOSE(2.41, block1(2, 2), 0.1);
    BOOST_CHECK_CLOSE(3.63, block1(2, 3), 0.1);
                           
    BOOST_CHECK_CLOSE(8.94, block2(0, 0), 0.1);
    BOOST_CHECK_CLOSE(6.53, block2(0, 1), 0.1);
    BOOST_CHECK_CLOSE(2.04, block2(0, 2), 0.1);
    BOOST_CHECK_CLOSE(5.91, block2(0, 3), 0.1);
    BOOST_CHECK_CLOSE(5.71, block2(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.51, block2(1, 1), 0.1);
    BOOST_CHECK_CLOSE(6.84, block2(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.33, block2(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.13, block2(2, 0), 0.1);
    BOOST_CHECK_CLOSE(2.64, block2(2, 1), 0.1);
    BOOST_CHECK_CLOSE(6.41, block2(2, 2), 0.1);
    BOOST_CHECK_CLOSE(4.38, block2(2, 3), 0.1);

    Matrix<double> mat2 = block2;
    mat = block1 + mat2;

    BOOST_CHECK_CLOSE(16.29, mat(0, 0), 0.1);
    BOOST_CHECK_CLOSE(9.19, mat(0, 1), 0.1);
    BOOST_CHECK_CLOSE(10.29, mat(0, 2), 0.1);
    BOOST_CHECK_CLOSE(14.19, mat(0, 3), 0.1);
    BOOST_CHECK_CLOSE(12.22, mat(1, 0), 0.1);
    BOOST_CHECK_CLOSE(12.9, mat(1, 1), 0.1);
    BOOST_CHECK_CLOSE(8.02, mat(1, 2), 0.1);
    BOOST_CHECK_CLOSE(13.16, mat(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.24, mat(2, 0), 0.1);
    BOOST_CHECK_CLOSE(10.52, mat(2, 1), 0.1);
    BOOST_CHECK_CLOSE(8.82, mat(2, 2), 0.1);
    BOOST_CHECK_CLOSE(8.01, mat(2, 3), 0.1);

    // Se comprueba que los bloques no se han modificado

    BOOST_CHECK_CLOSE(7.35, block1(0, 0), 0.1);
    BOOST_CHECK_CLOSE(2.66, block1(0, 1), 0.1);
    BOOST_CHECK_CLOSE(8.25, block1(0, 2), 0.1);
    BOOST_CHECK_CLOSE(8.28, block1(0, 3), 0.1);
    BOOST_CHECK_CLOSE(6.51, block1(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.39, block1(1, 1), 0.1);
    BOOST_CHECK_CLOSE(1.18, block1(1, 2), 0.1);
    BOOST_CHECK_CLOSE(8.83, block1(1, 3), 0.1);
    BOOST_CHECK_CLOSE(0.11, block1(2, 0), 0.1);
    BOOST_CHECK_CLOSE(7.88, block1(2, 1), 0.1);
    BOOST_CHECK_CLOSE(2.41, block1(2, 2), 0.1);
    BOOST_CHECK_CLOSE(3.63, block1(2, 3), 0.1);

    mat = mat2 + block1;

    BOOST_CHECK_CLOSE(16.29, mat(0, 0), 0.1);
    BOOST_CHECK_CLOSE(9.19, mat(0, 1), 0.1);
    BOOST_CHECK_CLOSE(10.29, mat(0, 2), 0.1);
    BOOST_CHECK_CLOSE(14.19, mat(0, 3), 0.1);
    BOOST_CHECK_CLOSE(12.22, mat(1, 0), 0.1);
    BOOST_CHECK_CLOSE(12.9, mat(1, 1), 0.1);
    BOOST_CHECK_CLOSE(8.02, mat(1, 2), 0.1);
    BOOST_CHECK_CLOSE(13.16, mat(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.24, mat(2, 0), 0.1);
    BOOST_CHECK_CLOSE(10.52, mat(2, 1), 0.1);
    BOOST_CHECK_CLOSE(8.82, mat(2, 2), 0.1);
    BOOST_CHECK_CLOSE(8.01, mat(2, 3), 0.1);

    // Se comprueba que los bloques no se han modificado
                           
    BOOST_CHECK_CLOSE(8.94, block2(0, 0), 0.1);
    BOOST_CHECK_CLOSE(6.53, block2(0, 1), 0.1);
    BOOST_CHECK_CLOSE(2.04, block2(0, 2), 0.1);
    BOOST_CHECK_CLOSE(5.91, block2(0, 3), 0.1);
    BOOST_CHECK_CLOSE(5.71, block2(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.51, block2(1, 1), 0.1);
    BOOST_CHECK_CLOSE(6.84, block2(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.33, block2(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.13, block2(2, 0), 0.1);
    BOOST_CHECK_CLOSE(2.64, block2(2, 1), 0.1);
    BOOST_CHECK_CLOSE(6.41, block2(2, 2), 0.1);
    BOOST_CHECK_CLOSE(4.38, block2(2, 3), 0.1);
  }

  //{
  //  auto block1 = mat_5x5_d.block(0, 2, 0, 3);
  //  auto block2 = mat_10x10_d.block(3, 5, 4, 7);
  //  block1 += block2;

  //  BOOST_CHECK_CLOSE(16.29, block1(0, 0), 0.1);
  //  BOOST_CHECK_CLOSE(9.19, block1(0, 1), 0.1);
  //  BOOST_CHECK_CLOSE(10.29, block1(0, 2), 0.1);
  //  BOOST_CHECK_CLOSE(14.19, block1(0, 3), 0.1);
  //  BOOST_CHECK_CLOSE(12.22, block1(1, 0), 0.1);
  //  BOOST_CHECK_CLOSE(12.9, block1(1, 1), 0.1);
  //  BOOST_CHECK_CLOSE(8.02, block1(1, 2), 0.1);
  //  BOOST_CHECK_CLOSE(13.16, block1(1, 3), 0.1);
  //  BOOST_CHECK_CLOSE(4.24, block1(2, 0), 0.1);
  //  BOOST_CHECK_CLOSE(10.52, block1(2, 1), 0.1);
  //  BOOST_CHECK_CLOSE(8.82, block1(2, 2), 0.1);
  //  BOOST_CHECK_CLOSE(8.01, block1(2, 3), 0.1);
  //}

  {
    auto block1 = mat_5x5_d.block(0, 2, 0, 4);
    auto block2 = mat_10x10_d.block(3, 5, 4, 8);

    block1 += block2;

    BOOST_CHECK_CLOSE(16.29, block1(0, 0), 0.01);
    BOOST_CHECK_CLOSE(9.19, block1(0, 1), 0.01);
    BOOST_CHECK_CLOSE(10.29, block1(0, 2), 0.01);
    BOOST_CHECK_CLOSE(14.19, block1(0, 3), 0.01);
    BOOST_CHECK_CLOSE(9.8, block1(0, 4), 0.01);
    BOOST_CHECK_CLOSE(12.22, block1(1, 0), 0.01);
    BOOST_CHECK_CLOSE(12.9, block1(1, 1), 0.01);
    BOOST_CHECK_CLOSE(8.02, block1(1, 2), 0.01);
    BOOST_CHECK_CLOSE(13.16, block1(1, 3), 0.01);
    BOOST_CHECK_CLOSE(6.87, block1(1, 4), 0.01);
    BOOST_CHECK_CLOSE(4.24, block1(2, 0), 0.01);
    BOOST_CHECK_CLOSE(10.52, block1(2, 1), 0.01);
    BOOST_CHECK_CLOSE(8.82, block1(2, 2), 0.01);
    BOOST_CHECK_CLOSE(8.01, block1(2, 3), 0.01);
    BOOST_CHECK_CLOSE(10.9499, block1(2, 4), 0.01);
  }
}

/// Resta de matrices

BOOST_FIXTURE_TEST_CASE(subtraction, MatrixBlockTest)
{
  {
    auto block1 = mat_5x5_d.block(0, 2, 0, 3);
    auto block2 = mat_10x10_d.block(3, 5, 4, 7);
    
    Matrix<double> mat = mat_5x5_d.block(0, 2, 0, 3) - mat_10x10_d.block(3, 5, 4, 7);

    BOOST_CHECK_CLOSE(-1.59, mat(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-3.87, mat(0, 1), 0.1);
    BOOST_CHECK_CLOSE(6.21, mat(0, 2), 0.1);
    BOOST_CHECK_CLOSE(2.37, mat(0, 3), 0.1);
    BOOST_CHECK_CLOSE(0.8, mat(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.12, mat(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-5.66, mat(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.5, mat(1, 3), 0.1);
    BOOST_CHECK_CLOSE(-4.02, mat(2, 0), 0.1);
    BOOST_CHECK_CLOSE(5.24, mat(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-4, mat(2, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.75, mat(2, 3), 0.1);

    BOOST_CHECK_CLOSE(7.35, block1(0, 0), 0.1);
    BOOST_CHECK_CLOSE(2.66, block1(0, 1), 0.1);
    BOOST_CHECK_CLOSE(8.25, block1(0, 2), 0.1);
    BOOST_CHECK_CLOSE(8.28, block1(0, 3), 0.1);
    BOOST_CHECK_CLOSE(6.51, block1(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.39, block1(1, 1), 0.1);
    BOOST_CHECK_CLOSE(1.18, block1(1, 2), 0.1);
    BOOST_CHECK_CLOSE(8.83, block1(1, 3), 0.1);
    BOOST_CHECK_CLOSE(0.11, block1(2, 0), 0.1);
    BOOST_CHECK_CLOSE(7.88, block1(2, 1), 0.1);
    BOOST_CHECK_CLOSE(2.41, block1(2, 2), 0.1);
    BOOST_CHECK_CLOSE(3.63, block1(2, 3), 0.1);
                           
    BOOST_CHECK_CLOSE(8.94, block2(0, 0), 0.1);
    BOOST_CHECK_CLOSE(6.53, block2(0, 1), 0.1);
    BOOST_CHECK_CLOSE(2.04, block2(0, 2), 0.1);
    BOOST_CHECK_CLOSE(5.91, block2(0, 3), 0.1);
    BOOST_CHECK_CLOSE(5.71, block2(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.51, block2(1, 1), 0.1);
    BOOST_CHECK_CLOSE(6.84, block2(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.33, block2(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.13, block2(2, 0), 0.1);
    BOOST_CHECK_CLOSE(2.64, block2(2, 1), 0.1);
    BOOST_CHECK_CLOSE(6.41, block2(2, 2), 0.1);
    BOOST_CHECK_CLOSE(4.38, block2(2, 3), 0.1);

    mat = block1 - block2;

    BOOST_CHECK_CLOSE(-1.59, mat(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-3.87, mat(0, 1), 0.1);
    BOOST_CHECK_CLOSE(6.21, mat(0, 2), 0.1);
    BOOST_CHECK_CLOSE(2.37, mat(0, 3), 0.1);
    BOOST_CHECK_CLOSE(0.8, mat(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.12, mat(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-5.66, mat(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.5, mat(1, 3), 0.1);
    BOOST_CHECK_CLOSE(-4.02, mat(2, 0), 0.1);
    BOOST_CHECK_CLOSE(5.24, mat(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-4, mat(2, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.75, mat(2, 3), 0.1);

    BOOST_CHECK_CLOSE(7.35, block1(0, 0), 0.1);
    BOOST_CHECK_CLOSE(2.66, block1(0, 1), 0.1);
    BOOST_CHECK_CLOSE(8.25, block1(0, 2), 0.1);
    BOOST_CHECK_CLOSE(8.28, block1(0, 3), 0.1);
    BOOST_CHECK_CLOSE(6.51, block1(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.39, block1(1, 1), 0.1);
    BOOST_CHECK_CLOSE(1.18, block1(1, 2), 0.1);
    BOOST_CHECK_CLOSE(8.83, block1(1, 3), 0.1);
    BOOST_CHECK_CLOSE(0.11, block1(2, 0), 0.1);
    BOOST_CHECK_CLOSE(7.88, block1(2, 1), 0.1);
    BOOST_CHECK_CLOSE(2.41, block1(2, 2), 0.1);
    BOOST_CHECK_CLOSE(3.63, block1(2, 3), 0.1);
                           
    BOOST_CHECK_CLOSE(8.94, block2(0, 0), 0.1);
    BOOST_CHECK_CLOSE(6.53, block2(0, 1), 0.1);
    BOOST_CHECK_CLOSE(2.04, block2(0, 2), 0.1);
    BOOST_CHECK_CLOSE(5.91, block2(0, 3), 0.1);
    BOOST_CHECK_CLOSE(5.71, block2(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.51, block2(1, 1), 0.1);
    BOOST_CHECK_CLOSE(6.84, block2(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.33, block2(1, 3), 0.1);
    BOOST_CHECK_CLOSE(4.13, block2(2, 0), 0.1);
    BOOST_CHECK_CLOSE(2.64, block2(2, 1), 0.1);
    BOOST_CHECK_CLOSE(6.41, block2(2, 2), 0.1);
    BOOST_CHECK_CLOSE(4.38, block2(2, 3), 0.1);

    Matrix<double> mat2 = block2;
    mat = block1 - mat2;

    BOOST_CHECK_CLOSE(-1.59, mat(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-3.87, mat(0, 1), 0.1);
    BOOST_CHECK_CLOSE(6.21, mat(0, 2), 0.1);
    BOOST_CHECK_CLOSE(2.37, mat(0, 3), 0.1);
    BOOST_CHECK_CLOSE(0.8, mat(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.12, mat(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-5.66, mat(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.5, mat(1, 3), 0.1);
    BOOST_CHECK_CLOSE(-4.02, mat(2, 0), 0.1);
    BOOST_CHECK_CLOSE(5.24, mat(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-4, mat(2, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.75, mat(2, 3), 0.1);

    BOOST_CHECK_CLOSE(7.35, block1(0, 0), 0.1);
    BOOST_CHECK_CLOSE(2.66, block1(0, 1), 0.1);
    BOOST_CHECK_CLOSE(8.25, block1(0, 2), 0.1);
    BOOST_CHECK_CLOSE(8.28, block1(0, 3), 0.1);
    BOOST_CHECK_CLOSE(6.51, block1(1, 0), 0.1);
    BOOST_CHECK_CLOSE(6.39, block1(1, 1), 0.1);
    BOOST_CHECK_CLOSE(1.18, block1(1, 2), 0.1);
    BOOST_CHECK_CLOSE(8.83, block1(1, 3), 0.1);
    BOOST_CHECK_CLOSE(0.11, block1(2, 0), 0.1);
    BOOST_CHECK_CLOSE(7.88, block1(2, 1), 0.1);
    BOOST_CHECK_CLOSE(2.41, block1(2, 2), 0.1);
    BOOST_CHECK_CLOSE(3.63, block1(2, 3), 0.1);

  }

  {
    auto block1 = mat_5x5_d.block(0, 2, 0, 3);
    auto block2 = mat_10x10_d.block(3, 5, 4, 7);
    block1 -= block2;

    BOOST_CHECK_CLOSE(-1.59, block1(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-3.87, block1(0, 1), 0.1);
    BOOST_CHECK_CLOSE(6.21, block1(0, 2), 0.1);
    BOOST_CHECK_CLOSE(2.37, block1(0, 3), 0.1);
    BOOST_CHECK_CLOSE(0.8, block1(1, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.12, block1(1, 1), 0.1);
    BOOST_CHECK_CLOSE(-5.66, block1(1, 2), 0.1);
    BOOST_CHECK_CLOSE(4.5, block1(1, 3), 0.1);
    BOOST_CHECK_CLOSE(-4.02, block1(2, 0), 0.1);
    BOOST_CHECK_CLOSE(5.24, block1(2, 1), 0.1);
    BOOST_CHECK_CLOSE(-4, block1(2, 2), 0.1);
    BOOST_CHECK_CLOSE(-0.75, block1(2, 3), 0.1);
  }
}

// 7.35, 2.66, 8.25, 8.28
// 6.51, 6.39, 1.18, 8.83
// 0.11, 7.88, 2.41, 3.63

// 8.94, 6.53, 2.04, 5.91
// 5.71, 6.51, 6.84, 4.33
// 4.13, 2.64, 6.41, 4.38

BOOST_FIXTURE_TEST_CASE(multiplication, MatrixBlockTest)
{

}

/// Multiplicación de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(block_matrix_scalar, MatrixBlockTest)
{
  auto block1 = mat_5x5_d.block(0, 2, 0, 3);
  block1 *= 25.2;

  BOOST_CHECK_CLOSE(185.22, block1(0, 0), 0.1);
  BOOST_CHECK_CLOSE(67.032, block1(0, 1), 0.1);
  BOOST_CHECK_CLOSE(207.9, block1(0, 2), 0.1);
  BOOST_CHECK_CLOSE(208.656, block1(0, 3), 0.1);
  BOOST_CHECK_CLOSE(164.052, block1(1, 0), 0.1);
  BOOST_CHECK_CLOSE(161.028, block1(1, 1), 0.1);
  BOOST_CHECK_CLOSE(29.736, block1(1, 2), 0.1);
  BOOST_CHECK_CLOSE(222.516, block1(1, 3), 0.1);
  BOOST_CHECK_CLOSE(2.772, block1(2, 0), 0.1);
  BOOST_CHECK_CLOSE(198.576, block1(2, 1), 0.1);
  BOOST_CHECK_CLOSE(60.732, block1(2, 2), 0.1);
  BOOST_CHECK_CLOSE(91.476, block1(2, 3), 0.1);
}

BOOST_FIXTURE_TEST_CASE(block_matrix_scalar2, MatrixBlockTest)
{
  auto mat = mat_5x5_d.block(0, 2, 0, 3) * 25.2;

  BOOST_CHECK_CLOSE(185.22, mat(0, 0), 0.1);
  BOOST_CHECK_CLOSE(67.032, mat(0, 1), 0.1);
  BOOST_CHECK_CLOSE(207.9, mat(0, 2), 0.1);
  BOOST_CHECK_CLOSE(208.656, mat(0, 3), 0.1);
  BOOST_CHECK_CLOSE(164.052, mat(1, 0), 0.1);
  BOOST_CHECK_CLOSE(161.028, mat(1, 1), 0.1);
  BOOST_CHECK_CLOSE(29.736, mat(1, 2), 0.1);
  BOOST_CHECK_CLOSE(222.516, mat(1, 3), 0.1);
  BOOST_CHECK_CLOSE(2.772, mat(2, 0), 0.1);
  BOOST_CHECK_CLOSE(198.576, mat(2, 1), 0.1);
  BOOST_CHECK_CLOSE(60.732, mat(2, 2), 0.1);
  BOOST_CHECK_CLOSE(91.476, mat(2, 3), 0.1);
}

/// Multiplicación de un escalar por una matriz

BOOST_FIXTURE_TEST_CASE(scalar_block_matrix, MatrixBlockTest)
{
  auto mat =  25.2 * mat_5x5_d.block(0, 2, 0, 3);

  BOOST_CHECK_CLOSE(185.22, mat(0, 0), 0.1);
  BOOST_CHECK_CLOSE(67.032, mat(0, 1), 0.1);
  BOOST_CHECK_CLOSE(207.9, mat(0, 2), 0.1);
  BOOST_CHECK_CLOSE(208.656, mat(0, 3), 0.1);
  BOOST_CHECK_CLOSE(164.052, mat(1, 0), 0.1);
  BOOST_CHECK_CLOSE(161.028, mat(1, 1), 0.1);
  BOOST_CHECK_CLOSE(29.736, mat(1, 2), 0.1);
  BOOST_CHECK_CLOSE(222.516, mat(1, 3), 0.1);
  BOOST_CHECK_CLOSE(2.772, mat(2, 0), 0.1);
  BOOST_CHECK_CLOSE(198.576, mat(2, 1), 0.1);
  BOOST_CHECK_CLOSE(60.732, mat(2, 2), 0.1);
  BOOST_CHECK_CLOSE(91.476, mat(2, 3), 0.1);
}

/// Multiplicación de un vector por una matriz

BOOST_FIXTURE_TEST_CASE(vector_block_matrix, MatrixBlockTest)
{
  //Matrix<double, 3, 4> m{7.35, 2.66, 8.25, 8.28,
//                       6.51, 6.39, 1.18, 8.83,
//                       0.11, 7.88, 2.41, 3.63};

// auto m2 = 25.2 * m;

//std::cout << m << std::endl;  
}


// División de una matriz por un escalar

BOOST_FIXTURE_TEST_CASE(div_block_matrix_scalar, MatrixBlockTest)
{
  auto block1 = mat_5x5_d.block(0, 2, 0, 3);
  block1 /= 25.2;

  BOOST_CHECK_CLOSE(0.291667, block1(0, 0), 0.1);
  BOOST_CHECK_CLOSE(0.105556, block1(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0.327381, block1(0, 2), 0.1);
  BOOST_CHECK_CLOSE(0.328571, block1(0, 3), 0.1);
  BOOST_CHECK_CLOSE(0.258333, block1(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.253571, block1(1, 1), 0.1);
  BOOST_CHECK_CLOSE(0.046825, block1(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0.350397, block1(1, 3), 0.1);
  BOOST_CHECK_CLOSE(0.004365, block1(2, 0), 0.1);
  BOOST_CHECK_CLOSE(0.312698, block1(2, 1), 0.1);
  BOOST_CHECK_CLOSE(0.095635, block1(2, 2), 0.1);
  BOOST_CHECK_CLOSE(0.144048, block1(2, 3), 0.1);
}

BOOST_FIXTURE_TEST_CASE(div_block_matrix_scalar2, MatrixBlockTest)
{
  auto block1 = mat_5x5_d.block(0, 2, 0, 3);
  auto mat = block1 / 25.2;

  BOOST_CHECK_CLOSE(0.291667, mat(0, 0), 0.1);
  BOOST_CHECK_CLOSE(0.105556, mat(0, 1), 0.1);
  BOOST_CHECK_CLOSE(0.327381, mat(0, 2), 0.1);
  BOOST_CHECK_CLOSE(0.328571, mat(0, 3), 0.1);
  BOOST_CHECK_CLOSE(0.258333, mat(1, 0), 0.1);
  BOOST_CHECK_CLOSE(0.253571, mat(1, 1), 0.1);
  BOOST_CHECK_CLOSE(0.046825, mat(1, 2), 0.1);
  BOOST_CHECK_CLOSE(0.350397, mat(1, 3), 0.1);
  BOOST_CHECK_CLOSE(0.004365, mat(2, 0), 0.1);
  BOOST_CHECK_CLOSE(0.312698, mat(2, 1), 0.1);
  BOOST_CHECK_CLOSE(0.095635, mat(2, 2), 0.1);
  BOOST_CHECK_CLOSE(0.144048, mat(2, 3), 0.1);
}

// Vector

BOOST_FIXTURE_TEST_CASE(col, MatrixBlockTest)
{

}

BOOST_FIXTURE_TEST_CASE(row, MatrixBlockTest)
{

}

BOOST_FIXTURE_TEST_CASE(block, MatrixBlockTest)
{

}

BOOST_AUTO_TEST_SUITE_END()