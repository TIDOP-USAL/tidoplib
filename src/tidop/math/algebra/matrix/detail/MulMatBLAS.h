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

#pragma once

#include "tidop/math/base/blas.h"

namespace tl
{

namespace detail
{

#ifdef TL_HAVE_OPENBLAS

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_blas(const LHS &matrix1, const RHS &matrix2, OutMat &matrix)
{
    using T = typename matrix_traits<OutMat>::value_type;

    bool mat1_is_symmetric = matrix1.isSymmetric();
    bool mat2_is_symmetric = matrix2.isSymmetric();
    bool mat1_is_upper = matrix1.isUpperTriangular();
    bool mat1_is_lower = matrix1.isLowerTriangular();
    bool mat2_is_upper = matrix2.isUpperTriangular();
    bool mat2_is_lower = matrix2.isLowerTriangular();
    bool mat1_is_triangular = mat1_is_upper || mat1_is_lower;
    bool mat2_is_triangular = mat2_is_upper || mat2_is_lower;

    //if (matrix1.rows() == matrix1.cols() && (mat1_is_symmetric || mat2_is_symmetric)) {
        //T alpha = 1.0;
        //T beta = 0.0;
        //auto lda = matrix1.rows();
        //auto ldb = matrix2.cols();
        //auto ldc = matrix2.cols();
    //    blas::symm(blas::Order::row_major, 
    //               mat1_is_symmetric ? blas::Side::left : blas::Side::right,
    //               blas::TriangularForm::upper,
    //               matrix1.rows(), 
    //               matrix2.cols(), 
    //               alpha,
    //               matrix1.data(), lda
    //               matrix2.data(), ldb,
    //               beta,
    //               matrix.data(), ldc);

    //} else if (matrix1.rows() == matrix1.cols() && (mat1_is_triangular || mat2_is_triangular)) {

    //    blas::Side side;
    //    blas::TriangularForm form;

    //    if (mat1_is_triangular) {
    //        side = blas::Side::left;
    //        form = mat1_is_upper ? blas::TriangularForm::upper : blas::TriangularForm::lower;
    //    } else {
    //        side = blas::Side::right;
    //        form = mat1_is_upper ? blas::TriangularForm::upper : blas::TriangularForm::lower;
    //    }

    //    T alpha = 1.0;
    //    auto lda = m;
    //    auto ldb = n;
    //    matrix = matrix2;
    //    blas::trmm(blas::Order::row_major, 
    //               side, 
    //               form, 
    //               blas::TransposeMode::no_transpose,  
    //               matrix1.rows(),
    //               matrix2.cols(),
    //               alpha,
    //               matrix1.data(), lda,
    //               matrix.data(), ldb);

    //} else {
    T alpha = 1.;
    T beta = 0.;
    auto lda = matrix1.cols();
    auto ldb = matrix2.cols();
    auto ldc = matrix2.cols();
    blas::gemm(blas::Order::row_major,
        blas::TransposeMode::no_transpose,
        blas::TransposeMode::no_transpose,
        matrix1.rows(),
        matrix2.cols(),
        matrix1.cols(),
        alpha,
        matrix1.data(), lda,
        matrix2.data(), ldb,
        beta,
        matrix.data(), ldc);
    //}
}

#endif // TL_HAVE_OPENBLAS

} // namespace detail

} // namespace tl