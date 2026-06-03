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

#include <type_traits>
#include <cstddef>

#include "tidop/config.h"

namespace tl
{

/*!
 * \brief Constant used to signify that the size of a structure (Matrix, Vector)
 * is determined at runtime rather than at compile-time.
 */
constexpr std::size_t DynamicData = std::numeric_limits<std::size_t>::max();



template<typename T, size_t Rows, size_t Cols> class Matrix;
template<typename T> class MatrixBlock;
template<typename T> class MatrixRow;
template<typename T> class MatrixCol;
template<typename T> class MatrixDiagonal;
template<typename T, size_t Size> class Vector;
template<typename T> class RotationMatrix;

template<typename LHS, typename RHS, typename Op> class MatBinaryExpr;
template<typename LHS, typename Scalar, typename Op> class MatScalarExpr;
template<typename LHS, typename RHS, typename Op> class VecBinaryExpr;
template<typename LHS, typename Scalar, typename Op> class VecScalarExpr;
template<typename LHS, typename RHS> class MatMulExpr;
template<typename Expr> class MatUnaryMinusExpr;
template<typename Expr> class TransposeExpr;
template<typename Expr> class VecUnaryMinusExpr;
template<typename Mat, typename Vec> class MatVecMulExpr;
template<typename Expr> class DiagonalMatrixExpr;

template<typename P> struct PackedTraits;
template<typename T> class Packed;


template<typename T>
struct matrix_traits
{
    static constexpr bool is_mutable = false;
    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_element_wise = false;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename T, size_t R, size_t C>
struct matrix_traits<Matrix<T, R, C>>
{
    using value_type = T;
    static constexpr size_t rows = R;
    static constexpr size_t cols = C;
    static constexpr bool is_mutable = true;
    static constexpr bool has_contiguous_memory = true;
    static constexpr bool is_element_wise = true;
    static constexpr bool is_expression = false;
    static constexpr bool is_plain = true;
};

template<typename T>
struct matrix_traits<MatrixBlock<T>>
{
    using value_type = std::remove_cv_t<T>;
    static constexpr size_t rows = std::numeric_limits<size_t>::max();
    static constexpr size_t cols = std::numeric_limits<size_t>::max();
    static constexpr bool is_mutable = !std::is_const_v<T>;
    static constexpr bool is_view = true;
    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_element_wise = true;
    static constexpr bool is_expression = false;
    static constexpr bool is_plain = false;
};

template<typename LHS, typename RHS, typename Op>
struct matrix_traits<MatBinaryExpr<LHS, RHS, Op>>
{
    static_assert(std::is_same_v<
        std::remove_cv_t<typename matrix_traits<LHS>::value_type>,
        std::remove_cv_t<typename matrix_traits<RHS>::value_type>>,
        "Mixed types not supported in matrix operations");

    using value_type = std::remove_cv_t<typename matrix_traits<LHS>::value_type>;
    static constexpr size_t rows = matrix_traits<LHS>::rows;
    static constexpr size_t cols = matrix_traits<LHS>::cols;
    static constexpr bool has_contiguous_memory = matrix_traits<LHS>::has_contiguous_memory &&
                                                  matrix_traits<RHS>::has_contiguous_memory;
    static constexpr bool is_element_wise = matrix_traits<LHS>::is_element_wise &&
                                            matrix_traits<RHS>::is_element_wise;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename LHS, typename Scalar, typename Op>
struct matrix_traits<MatScalarExpr<LHS, Scalar, Op>>
{
    static_assert(std::is_same_v<
        std::remove_cv_t<typename matrix_traits<LHS>::value_type>,
        std::remove_cv_t<Scalar>>,
        "Mixed types not supported in matrix operations");

    using value_type = std::remove_cv_t<typename matrix_traits<LHS>::value_type>;
    static constexpr size_t rows = matrix_traits<LHS>::rows;
    static constexpr size_t cols = matrix_traits<LHS>::cols;
    static constexpr bool has_contiguous_memory = matrix_traits<LHS>::has_contiguous_memory;
    static constexpr bool is_element_wise = matrix_traits<LHS>::is_element_wise;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename LHS, typename RHS>
struct matrix_traits<MatMulExpr<LHS, RHS>>
{
    using value_type = std::remove_cv_t<typename matrix_traits<LHS>::value_type>;

    static_assert(std::is_same_v<value_type, std::remove_cv_t<typename matrix_traits<RHS>::value_type>>,
        "Mixed types not supported in matrix multiplication");

    static constexpr size_t rows = matrix_traits<LHS>::rows;
    static constexpr size_t cols = matrix_traits<RHS>::cols;

    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_mutable = false;
    static constexpr bool is_element_wise = false;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename Expr>
struct matrix_traits<MatUnaryMinusExpr<Expr>>
{
    using value_type = std::remove_cv_t<typename matrix_traits<Expr>::value_type>;

    static_assert(std::is_signed_v<value_type>, "Unary minus requires a signed type");

    static constexpr size_t rows = matrix_traits<Expr>::rows;
    static constexpr size_t cols = matrix_traits<Expr>::cols;
    static constexpr bool has_contiguous_memory = matrix_traits<Expr>::has_contiguous_memory;
    static constexpr bool is_element_wise = matrix_traits<Expr>::is_element_wise;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename Expr>
struct matrix_traits<TransposeExpr<Expr>>
{
    using value_type = std::remove_cv_t<typename matrix_traits<Expr>::value_type>;

    static constexpr size_t rows = matrix_traits<Expr>::cols;
    static constexpr size_t cols = matrix_traits<Expr>::rows;

    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_mutable = false;
    static constexpr bool is_element_wise = false;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename T>
struct matrix_traits<RotationMatrix<T>>
{
    using value_type = std::remove_cv_t<typename RotationMatrix<T>::value_type>;

    static constexpr size_t rows = 3;
    static constexpr size_t cols = 3;
    static constexpr bool is_mutable = true;
    static constexpr bool has_contiguous_memory = true;
    static constexpr bool is_element_wise = true;
    static constexpr bool is_expression = false;
    static constexpr bool is_plain = true;
};



template<typename D>
struct vector_traits
{
    static constexpr bool is_mutable = false;
    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename T, size_t Size>
struct vector_traits<Vector<T, Size>>
{
    using value_type = T;
    static constexpr size_t size = Size;
    static constexpr bool is_mutable = true;
    static constexpr bool has_contiguous_memory = true;
    static constexpr bool is_expression = false;
    static constexpr bool is_plain = true;
};

template<typename T>
struct vector_traits<MatrixRow<T>>
{
    using value_type = std::remove_cv_t<T>;
    static constexpr size_t size = std::numeric_limits<size_t>::max();
    static constexpr bool is_view = true;
    static constexpr bool is_mutable = !std::is_const_v<T>;
    static constexpr bool is_element_wise = false;
    static constexpr bool has_contiguous_memory = true;
    static constexpr bool is_expression = false;
    static constexpr bool is_plain = false;
};

template<typename T>
struct vector_traits<MatrixCol<T>>
{
    using value_type = std::remove_cv_t<T>;
    static constexpr size_t size = std::numeric_limits<size_t>::max();
    static constexpr bool is_view = true;
    static constexpr bool is_mutable = !std::is_const_v<T>;
    static constexpr bool is_element_wise = false;
    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_expression = false;
    static constexpr bool is_plain = false;
};

template<typename T>
struct vector_traits<MatrixDiagonal<T>>
{
    using value_type = std::remove_cv_t<T>;
    static constexpr size_t size = std::numeric_limits<size_t>::max();
    static constexpr bool is_view = true;
    static constexpr bool is_mutable = !std::is_const_v<T>;
    static constexpr bool is_element_wise = false;
    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_expression = false;
    static constexpr bool is_plain = false;
};

template<typename LHS, typename RHS, typename Op>
struct vector_traits<VecBinaryExpr<LHS, RHS, Op>>
{
    static_assert(std::is_same_v<
        std::remove_cv_t<typename vector_traits<LHS>::value_type>,
        std::remove_cv_t<typename vector_traits<RHS>::value_type>>,
        "Mixed types not supported in vector operations");

    using value_type = std::remove_cv_t<typename vector_traits<LHS>::value_type>;
    static constexpr size_t size = vector_traits<LHS>::size;
    static constexpr bool has_contiguous_memory = vector_traits<LHS>::has_contiguous_memory && 
                                                  vector_traits<RHS>::has_contiguous_memory;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename LHS, typename Scalar, typename Op>
struct vector_traits<VecScalarExpr<LHS, Scalar, Op>>
{
    static_assert(std::is_same_v<
        std::remove_cv_t<typename vector_traits<LHS>::value_type>,
        std::remove_cv_t<Scalar>>,
        "Mixed types not supported in vector operations");

    using value_type = std::remove_cv_t<typename vector_traits<LHS>::value_type>;
    static constexpr size_t size = vector_traits<LHS>::size;
    static constexpr bool has_contiguous_memory = vector_traits<LHS>::has_contiguous_memory;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename Mat, typename Vec>
struct vector_traits<MatVecMulExpr<Mat, Vec>>
{
    static_assert(std::is_same_v<
        std::remove_cv_t<typename matrix_traits<Mat>::value_type>,
        std::remove_cv_t<typename vector_traits<Vec>::value_type>>,
        "Mixed types not supported in vector operations");

    using value_type = std::remove_cv_t<typename vector_traits<Vec>::value_type>;
    static constexpr size_t size = matrix_traits<Mat>::rows;
    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename Expr>
struct vector_traits<VecUnaryMinusExpr<Expr>>
{
    using value_type = std::remove_cv_t<typename vector_traits<Expr>::value_type>;

    static_assert(std::is_signed_v<value_type>, "Unary minus requires a signed type");

    static constexpr size_t size = vector_traits<Expr>::size;
    static constexpr bool has_contiguous_memory = vector_traits<Expr>::has_contiguous_memory;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename Expr>
struct matrix_traits<DiagonalMatrixExpr<Expr>>
{
    using value_type = std::remove_cv_t<typename vector_traits<Expr>::value_type>;

    static constexpr size_t rows = vector_traits<Expr>::size;
    static constexpr size_t cols = vector_traits<Expr>::size;

    static constexpr bool has_contiguous_memory = false;
    static constexpr bool is_mutable = false;
    static constexpr bool is_element_wise = false;
    static constexpr bool is_expression = true;
    static constexpr bool is_plain = false;
};

template<typename D>
struct is_vector : std::false_type {};

template<typename Scalar, size_t Size>
struct is_vector<Vector<Scalar, Size>> : std::true_type {};

template<typename T>
struct is_vector<MatrixRow<T>> : std::true_type {};

template<typename T>
struct is_vector<MatrixCol<T>> : std::true_type {};

template<typename T>
struct is_vector<MatrixDiagonal<T>> : std::true_type {};



template<typename T>
struct is_matrix_product : std::false_type {};

template<typename LHS, typename RHS>
struct is_matrix_product<MatMulExpr<LHS, RHS>> : std::true_type {};

template<typename T>
inline constexpr bool is_matrix_product_v = is_matrix_product<std::remove_cvref_t<T>>::value;


template<typename T>
struct is_matvec_product : std::false_type {};

template<typename LHS, typename RHS>
struct is_matvec_product<MatVecMulExpr<LHS, RHS>> : std::true_type {};

template<typename T>
inline constexpr bool is_matvec_product_v = is_matvec_product<std::remove_cvref_t<T>>::value;




template<typename T>
struct is_blas_compatible : std::false_type {};

template<typename T, size_t R, size_t C>
struct is_blas_compatible<Matrix<T, R, C>>
    : std::bool_constant<std::is_floating_point_v<T>>
{
};

template<typename T>
inline constexpr bool is_blas_compatible_v = is_blas_compatible<std::remove_cvref_t<T>>::value;


template<typename T>
struct is_cublas_compatible : std::false_type {};

template<typename T, size_t R, size_t C>
struct is_cublas_compatible<Matrix<T, R, C>>
    : std::bool_constant<std::is_floating_point_v<T>>
{
};

template<typename T>
inline constexpr bool is_cublas_compatible_v = is_cublas_compatible<std::remove_cvref_t<T>>::value;


template<typename T>
struct is_packed : std::false_type {};

template<typename T>
struct is_packed<Packed<T>> : std::true_type {};

}