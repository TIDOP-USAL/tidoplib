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

/*! \file MatrixBase.h
 * \brief Base class for matrix operations with expression templates.
 *
 * This file defines the `MatrixBase` class, which provides common matrix operations
 * (addition, multiplication, transpose, inverse, norms, etc.) using expression
 * templates for lazy evaluation. Many arithmetic operations and `transpose()`
 * return **expression objects** that represent the computation without actually
 * performing it. These expressions are lightweight and efficient, and they are
 * automatically evaluated when assigned to a concrete matrix or used in a context
 * that requires values (e.g., construction, assignment, printing).
 *
 * The typical usage is to perform operations and directly assign the result to a
 * matrix object. The evaluation happens automatically inside the assignment
 * operator or constructor:
 *
 * \code
 * Matrix<double, 3, 3> A = ...;
 * Matrix<double, 3, 3> B = ...;
 *
 * // Automatic evaluation: C becomes a concrete matrix
 * Matrix<double, 3, 3> C = A + B;
 *
 * // Transpose and assign
 * Matrix<double, 3, 3> D = A.transpose();
 * \endcode
 *
 * In rare cases where you need to store an expression for later use without
 * evaluating it immediately (e.g., in a lambda or a generic context), you can
 * use `auto` – but remember that the expression will be re‑evaluated each time
 * it is used, unless you call `.eval()` to obtain a concrete copy.
 *
 * \ingroup Matrix
 * \see tl::Matrix, tl::MatrixExpr, tl::BaseExpr
 */

#pragma once

#include "tidop/math/base/Data.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/BaseExpr.h"
#include "tidop/math/algebra/expr/MatBinaryExpr.h"
#include "tidop/math/algebra/expr/MatScalarExpr.h"
#include "tidop/math/algebra/expr/MatUnaryMinusExpr.h"
#include "tidop/math/algebra/expr/MatMulExpr.h"
#include "tidop/math/algebra/expr/TransposeExpr.h"
#include "tidop/math/algebra/matrix/detail/AdjugateMatrix.h"
#include "tidop/math/algebra/matrix/detail/Cofactor.h"
#include "tidop/math/algebra/matrix/detail/CofactorMatrix.h"
#include "tidop/math/algebra/matrix/detail/Determinant.h"
#include "tidop/math/algebra/matrix/detail/FirstMinor.h"
#include "tidop/math/algebra/matrix/detail/Inverse.h"
#include "tidop/math/algebra/matrix/detail/ReducedRowEchelonForm.h"
#include "tidop/math/algebra/matrix/detail/RowEchelonForm.h"
#include "tidop/math/algebra/matrix/detail/MatEqual.h"

namespace tl
{

/*! \addtogroup Matrix
 *  \{
 */

/*!
 * \class MatrixBase
 * \brief CRTP base class for matrix operations with lazy evaluation.
 *
 * \tparam Derived The derived matrix type (e.g., `Matrix<T, Rows, Cols>`).
 *
 * This class implements common matrix arithmetic and linear algebra operations.
 * Many operations (like `transpose()`, `operator+`, etc.) return **expression
 * objects**, which defer the actual computation until the result is assigned to
 * a concrete matrix or otherwise evaluated. This eliminates unnecessary temporary
 * matrices and enables optimizations like expression fusion.
 *
 * **How to use:**
 * - For normal matrix arithmetic, assign the result directly to a `Matrix` object.
 *   The expression will be evaluated automatically during assignment.
 * - If you need to store an intermediate expression for later use, you can use
 *   `auto`. However, be aware that the expression will be re‑evaluated every time
 *   it is used. To get a standalone copy, call `.eval()`.
 * - Some functions (e.g., `inverse()`, `determinant()`, `adjugate()`,
 *   `rowEchelonForm()`) return concrete matrices or scalars directly because they
 *   involve expensive computations that are always evaluated.
 *
 * ### Examples
 * \code
 * Matrix<double, 3, 3> A = {{1,2,3},{4,5,6},{7,8,9}};
 * Matrix<double, 3, 3> B = {{9,8,7},{6,5,4},{3,2,1}};
 *
 * // Automatic evaluation: C is a concrete matrix
 * Matrix<double, 3, 3> C = A + B;
 *
 * // Transpose and assign
 * Matrix<double, 3, 3> D = A.transpose();
 *
 * // Storing an expression (re‑computed each time it is used)
 * auto expr = A * B;
 * // ... later ...
 * Matrix<double, 3, 3> E = expr;   // evaluated here
 *
 * // Force evaluation into a concrete matrix
 * auto concrete = (A + B).eval();
 * \endcode
 */
template<typename Derived>
class MatrixBase
  : public BaseExpr<Derived>
{

public:

    using derived_type = Derived;
    using value_type = typename matrix_traits<Derived>::value_type;

public:

    /*!
     * \brief Computes the inverse of the matrix.
     *
     * Only square and invertible matrices have an inverse. This function
     * evaluates internally and returns a concrete matrix.
     *
     * \param[out] invertibility Optional pointer to a boolean that will be set to
     *                           `true` if the matrix is invertible, `false` otherwise.
     * \return The inverse matrix (if invertible); otherwise, the returned matrix
     *         is unspecified (usually a zero matrix or unchanged).
     *
     * \note For non‑square matrices, this function is not available (compile‑time error).
     * \see isInvertible()
     * 
     * ### Example
     * \code
     * Matrix<double, 2, 2> mat_2x2{2., 3.,
     *                              1., 4.};
     * bool invertible;
     * Matrix<double, 2, 2> inv_mat = mat_2x2.inverse(&invertible);
     * \endcode
     */
    auto inverse(bool *invertibility = nullptr) const;

    /*!
     * \brief Returns the transpose of the matrix (expression).
     *
     * \f[
     * A=\begin{bmatrix}
     * 1 & 2 & 3 \\
     * 4 & 5 & 6 \\
     * 7 & 8 & 9 \\
     * \end{bmatrix}
     * \f]
     *
     * \f[
     * A^{T}=\begin{bmatrix}
     * 1 & 4 & 7 \\
     * 2 & 5 & 8 \\
     * 3 & 6 & 9 \\
     * \end{bmatrix}
     * \f]
     *
     * \note To obtain a concrete matrix, assign the result to a `Matrix` object:
     *       `Matrix<double,3,3> B = A.transpose();` The evaluation happens
     *       automatically during assignment.
     *
     * \return A `TransposeExpr` representing the transpose.
     */
    constexpr auto transpose() const;

    /*!
     * \brief Computes the determinant of a square matrix (scalar result).
     *
     * \return The determinant as `value_type`.
     * \throws std::invalid_argument if the matrix is not square.
     */
    auto determinant() const;

    /*!
     * \brief Computes the adjugate (classical adjoint) matrix (concrete result).
     * 
     * The adjugate is the transpose of the cofactor matrix.
     * \f[ adj(A) = C^T \f]
     *
     * \return The adjugate as a concrete matrix.
     */
    auto adjugate() const;

    /*!
     * \brief Calculates the cofactor matrix (concrete result).
     * \return The cofactor matrix
     */
    auto cofactorMatrix() const;

    /*!
     * \brief Computes the cofactor of a specific element (scalar result).
     *
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return The cofactor value.
     */
    auto cofactor(size_t r, size_t c) const -> value_type;

    /*!
     * \brief Computes the trace.
     * Sum of the elements on the main diagonal of a square matrix
     * \f[ tr(A) = a_{11} + ... +  a_{nn} \f]
     * \return The trace.
     */
    auto trace() const -> value_type;

    /*!
     * \brief Checks whether the matrix is invertible.
     * \return `true` if square and determinant non‑zero.
     */
    auto isInvertible() const -> bool;

    /*!
     * \brief Checks whether the matrix is singular.
     * \return `true` if square and determinant zero.
     */
    auto isSingular() const -> bool;

    /*!
     * \brief Checks whether the matrix is square.
     * \return `true` if rows == cols.
     */
    constexpr auto isSquare() const -> bool;

    /*!
     * \brief Checks whether the matrix is symmetric.
     * \return `true` if square and \f$ A = A^T \f$.
     */
    auto isSymmetric() const -> bool;

    /*!
     * \brief Checks whether the matrix is diagonal.
     * \return `true` if all off‑diagonal elements are zero.
     */
    auto isDiagonal() const -> bool;

    /*!
     * \brief Checks if the matrix is an identity matrix.
     *
     * A matrix is an identity matrix if it is square, all diagonal elements are 1,
     * and all non-diagonal elements are 0.
     *
     * \return true if the matrix is an identity matrix, false otherwise.
     */
    auto isIdentity() const -> bool;

    /*!
     * \brief Checks if the matrix is upper triangular.
     *
     * A matrix is upper triangular if all elements below the main diagonal are zero.
     *
     * \return true if the matrix is upper triangular, false otherwise.
     */
    auto isUpperTriangular() const -> bool;

    /*!
     * \brief Checks if the matrix is lower triangular.
     *
     * A matrix is lower triangular if all elements above the main diagonal are zero.
     *
     * \return true if the matrix is lower triangular, false otherwise.
     */
    auto isLowerTriangular() const -> bool;

    /*!
     * \brief Checks if the matrix is triangular.
     *
     * A matrix is triangular if it is either upper triangular or lower triangular.
     *
     * A matrix is:
     * - **Upper triangular** if all elements below the main diagonal are zero.
     * - **Lower triangular** if all elements above the main diagonal are zero.
     *
     * This method returns `true` if the matrix satisfies either of these conditions.
     *
     * \return true if the matrix is triangular (upper or lower), false otherwise.
     */
    auto isTriangular() const -> bool;

    /*!
     * \brief Computes the first minor
     * 
     * A minor of a square matrix A is the determinant of some of the
     * submatrices obtained from the elimination of a row and a column.
     * They are used for calculating the cofactor matrix.
     *
     * \f[
     * A=\begin{bmatrix}
     * a1 & a2 & a3 \\
     * a4 & a5 & a6 \\
     * a7 & a8 & a9 \\
     * \end{bmatrix}
     * \f]
     *
     * \f[ M_{23} = a8*a1-a2*a7 \f]
     *
     * \return The determinant of the submatrix obtained by removing row `r` and column `c`.
     */
    auto firstMinor(size_t r, size_t c) const;

    /*!
     * \brief Computes the row echelon form (REF) (concrete result).
     * \return A concrete matrix in REF.
     */
    auto rowEchelonForm() const;

    /*!
     * \brief Computes the reduced row echelon form (RREF) (concrete result).
     * \return A concrete matrix in RREF.
     */
    auto reducedRowEchelonForm() const;

    /*!
     * \brief Computes the rank of the matrix (scalar result).
     * \return The rank as an integer.
     */
    auto rank() const -> int;

    /*!
     * \brief Extracts the diagonal elements as a vector (concrete result).
     * \return A vector containing the diagonal entries.
     */
    //auto diagonal() const -> Vector<value_type>;

    /*!
     * \brief Computes the Frobenius norm of the matrix.
     *
     * The Frobenius norm is defined as:
     * \f[
     * \|A\|_F = \sqrt{\sum_{i,j} |a_{ij}|^2}
     * \f]
     *
     * \return The Frobenius norm of the matrix.
     */
    auto frobeniusNorm() const -> value_type;

    /*!
     * \brief Computes the L1 norm of the matrix (maximum absolute column sum).
     *
     * The L1 norm is defined as:
     * \f[
     * \|A\|_1 = \max_j \sum_i |a_{ij}|
     * \f]
     *
     * \return The L1 norm of the matrix.
     */
    auto l1Norm() const -> value_type;

    /*!
     * \brief Computes the L2 norm of the matrix (approximate spectral norm).
     *
     * The L2 norm is approximated as:
     * \f[
     * \|A\|_2 = \max_i \sqrt{\sum_j |a_{ij}|^2}
     * \f]
     *
     * This is an upper bound approximation of the true spectral norm.
     *
     * \return The approximate L2 norm of the matrix.
     */
    auto l2Norm() const -> value_type;

    /*!
     * \brief Returns the number of rows.
     */
    constexpr auto rows() const noexcept -> size_t { return this->derived().rows(); }

    /*!
     * \brief Returns the number of columns.
     */
    constexpr auto cols() const noexcept -> size_t { return this->derived().cols(); }

    /* Unary arithmetic operators */

    /*!
     * \brief Unary plus (identity).
     * \return Const reference to the matrix itself.
     */
    auto operator+() const noexcept -> const Derived & { return this->derived(); }

    /*!
     * \brief Unary minus (negation) – returns an expression.
     * \return A `MatUnaryMinusExpr` representing the negated matrix.
     */
    auto operator-() const { return MatUnaryMinusExpr<Derived>(this->derived()); }

    /* Binary arithmetic operators */

    /*!
     * \brief Matrix addition (returns expression).
     * \tparam RHS A type satisfying `MatrixExpr`.
     * \param[in] rhs The matrix to add.
     * \return A `MatBinaryExpr` representing the sum.
     */
    template<MatrixExpr RHS>
    auto operator +(const RHS &rhs) const;

    /*!
     * \brief Matrix subtraction (returns expression).
     * \tparam RHS A type satisfying `MatrixExpr`.
     * \param[in] rhs The matrix to subtract.
     * \return A `MatBinaryExpr` representing the difference.
     */
    template<MatrixExpr RHS>
    auto operator -(const RHS &rhs) const;

    /*!
     * \brief Scalar multiplication (returns expression).
     * \param[in] scalar The scalar value.
     * \return A `MatScalarExpr` representing the scaled matrix.
     */
    auto operator *(value_type scalar) const;

    /*!
     * \brief Scalar division (returns expression).
     * \param[in] scalar The scalar divisor.
     * \return A `MatScalarExpr` representing the matrix divided by the scalar.
     */
    auto operator /(value_type scalar) const;

    /* Compound assignment operators */

    /*!
     * \brief Addition assignment (evaluated in‑place).
     * \param[in] expr The matrix expression to add.
     * \return Reference to the modified matrix.
     */
    template<MatrixExpr Expr>
        requires (matrix_traits<Derived>::is_mutable)
    auto operator+=(const Expr &expr) -> Derived &;

    /*!
     * \brief Subtraction assignment (evaluated in‑place).
     * \param[in] expr The matrix expression to subtract.
     * \return Reference to the modified matrix.
     */
    template<MatrixExpr Expr>
        requires (matrix_traits<Derived>::is_mutable)
    auto operator-=(const Expr &expr) -> Derived &;

    /*!
     * \brief Scalar multiplication assignment (in‑place).
     * \param[in] scalar The scalar multiplier.
     * \return Reference to the modified matrix.
     */
    template<typename Scalar>
    auto operator *=(Scalar scalar) -> Derived &
        requires (matrix_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>);

    /*!
     * \brief Scalar division assignment (in‑place).
     * \param[in] scalar The scalar divisor.
     * \return Reference to the modified matrix.
     */
    template<typename Scalar>
    auto operator /=(Scalar scalar) -> Derived &
        requires (matrix_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>);

    /*!
     * \brief Evaluates the expression and returns a concrete matrix.
     *
     * If the derived type is already a plain matrix, returns a copy.
     * Otherwise, constructs a new matrix from the expression.
     *
     * \return A concrete `Matrix` object.
     *
     * \note This method is rarely needed because assignment to a `Matrix`
     *       automatically evaluates the expression. It is useful when you need
     *       to store an evaluated copy of an expression, e.g., when using `auto`
     *       and you want to avoid re‑evaluation.
     */
    constexpr auto eval() const;

};


/* MatrixBase implementation */

template<typename Derived>
auto MatrixBase<Derived>::inverse(bool *invertibility) const
{
    auto evaluated_mat = this->eval();

    return detail::invert_matrix(evaluated_mat, invertibility);
}

template<typename Derived>
constexpr auto MatrixBase<Derived>::transpose() const
{
    return TransposeExpr<Derived>(this->derived());
}

template<typename Derived>
auto MatrixBase<Derived>::determinant() const
{
    auto evaluated_mat = this->eval();
    return detail::matrix_determinant(evaluated_mat);
}

template<typename Derived>
auto MatrixBase<Derived>::adjugate() const
{
    auto evaluated_mat = this->eval();
    return detail::matrix_adjoint(evaluated_mat);
}

template<typename Derived>
auto MatrixBase<Derived>::cofactorMatrix() const
{
    auto evaluated_mat = this->eval();
    return detail::matrix_cofactor_matrix(evaluated_mat);
}

template<typename Derived>
auto MatrixBase<Derived>::cofactor(size_t r, size_t c) const -> value_type
{
    auto evaluated_mat = this->eval();
    return detail::matrix_cofactor(evaluated_mat, r, c);
}

template<typename Derived>
auto MatrixBase<Derived>::trace() const -> value_type
{
    static_assert(matrix_traits<Derived>::rows == DynamicData ||
        matrix_traits<Derived>::cols == DynamicData ||
        matrix_traits<Derived>::rows == matrix_traits<Derived>::cols,
        "Static matrices must be square for trace");

    size_t r = this->rows();
    TL_ASSERT(r == this->cols(), "Non-Square Matrix");

    value_type sum = consts::zero<value_type>;

    auto eval = make_evaluator(this->derived());

    for (size_t i = 0; i < r; ++i) {
        sum += eval.coeff(i, i);
    }

    return sum;
}

template<typename Derived>
auto MatrixBase<Derived>::isInvertible() const -> bool
{
    return (this->determinant() != consts::zero<value_type>);
}

template<typename Derived>
auto MatrixBase<Derived>::isSingular() const -> bool
{
    return (this->determinant() == consts::zero<value_type>);
}

template<typename Derived>
constexpr auto MatrixBase<Derived>::isSquare() const -> bool
{
    return this->rows() == this->cols();
}

template<typename Derived>
auto MatrixBase<Derived>::isSymmetric() const -> bool
{
    if (!this->isSquare()) return false;

    auto eval = make_evaluator(this->derived());
    size_t r = this->rows();

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = i + 1; j < r; ++j) {
            if (!isNearlyEqual(eval.coeff(i, j), eval.coeff(j, i))) {
                return false;
            }
        }
    }
    return true;
}

template<typename Derived>
auto MatrixBase<Derived>::isDiagonal() const -> bool
{
    if (!this->isSquare()) return false;

    auto eval = make_evaluator(this->derived());
    size_t r = this->rows();

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < r; ++j) {
            if (i != j && !isNearlyZero(eval.coeff(i, j))) {
                return false;
            }
        }
    }
    return true;
}

template<typename Derived>
auto MatrixBase<Derived>::isIdentity() const -> bool
{
    if (!this->isSquare()) return false;

    auto eval = make_evaluator(this->derived());
    size_t r = this->rows();

    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < r; ++j) {
            if (i == j) {
                if (!isNearlyEqual(eval.coeff(i, j), consts::one<value_type>)) {
                    return false;
                }
            } else {
                if (!isNearlyZero(eval.coeff(i, j))) {
                    return false;
                }
            }
        }
    }
    return true;
}

template<typename Derived>
auto MatrixBase<Derived>::isUpperTriangular() const -> bool
{
    if (!this->isSquare()) return false;

    auto eval = make_evaluator(this->derived());

    for (size_t r = 1; r < this->rows(); ++r) {
        for (size_t c = 0; c < r; ++c) {
            if (!isNearlyZero(eval.coeff(r, c))) {
                return false;
            }
        }
    }

    return true;
}

template<typename Derived>
auto MatrixBase<Derived>::isLowerTriangular() const -> bool
{
    if (!this->isSquare()) return false;

    auto eval = make_evaluator(this->derived());

    for (size_t r = 0; r < this->rows(); ++r) {
        for (size_t c = r + 1; c < this->cols(); ++c) {
            if (!isNearlyZero(eval.coeff(r, c))) {
                return false;
            }
        }
    }

    return true;
}

template<typename Derived>
auto MatrixBase<Derived>::isTriangular() const -> bool
{
    return this->isUpperTriangular() || this->isLowerTriangular();
}

template<typename Derived>
auto MatrixBase<Derived>::firstMinor(size_t r, size_t c) const
{
    auto evaluated_mat = this->eval();
    return detail::matrix_first_minor(evaluated_mat, r, c);
}

template<typename Derived>
auto MatrixBase<Derived>::rowEchelonForm() const
{
    auto evaluated_mat = this->eval();
    return detail::matrix_row_echelon_form(evaluated_mat);
}

template<typename Derived>
auto MatrixBase<Derived>::reducedRowEchelonForm() const
{
    auto evaluated_mat = this->eval();
    return detail::matrix_reduced_row_echelon_form(evaluated_mat);
}

template<typename Derived>
auto MatrixBase<Derived>::rank() const -> int
{
    auto ref_matrix = this->rowEchelonForm();

    int rnk = 0;
    size_t r_count = ref_matrix.rows();
    size_t c_count = ref_matrix.cols();

    for (size_t r = 0; r < r_count; r++) {
        for (size_t c = r; c < c_count; c++) {
            if (!isNearlyZero(ref_matrix(r, c))) {
                rnk++;
                break;
            }
        }
    }

    return rnk;
}

//template<typename Derived>
//auto MatrixBase<Derived>::diagonal() const -> Vector<value_type>
//{
//    //TODO: Implementar diagonal() para matrices mutables y const, usando 
//    // MatrixDiagonal como vista para evitar copias innecesarias
//    auto &derived = this->derived();
//    size_t size = std::min(derived.rows(), derived.cols());
//    Vector<value_type> diag_vector(size);
//
//    auto eval = make_evaluator(derived);
//
//    for (size_t i = 0; i < size; ++i) {
//        diag_vector[i] = eval.coeff(i, i);
//    }
//
//    return diag_vector;
//}

template<typename Derived>
auto MatrixBase<Derived>::frobeniusNorm() const -> value_type
{
    size_t r_count = this->rows();
    size_t c_count = this->cols();

    value_type sum = consts::zero<value_type>;

    auto eval = make_evaluator(this->derived());

    for (size_t r = 0; r < r_count; ++r) {
        for (size_t c = 0; c < c_count; ++c) {
            value_type val = eval.coeff(r, c);
            sum += val * val;
        }
    }

    return std::sqrt(sum);
}

template<typename Derived>
auto MatrixBase<Derived>::l1Norm() const -> value_type
{
    size_t r_count = this->rows();
    size_t c_count = this->cols();

    value_type max_sum = consts::zero<value_type>;

    auto eval = make_evaluator(this->derived());

    for (size_t c = 0; c < c_count; ++c) {
        value_type col_sum = consts::zero<value_type>;
        for (size_t r = 0; r < r_count; ++r) {
            col_sum += std::abs(eval.coeff(r, c));
        }
        max_sum = std::max(max_sum, col_sum);
    }

    return max_sum;
}

template<typename Derived>
auto MatrixBase<Derived>::l2Norm() const -> value_type
{
    auto evaluated_mat = this->eval();

    SingularValueDecomposition<decltype(evaluated_mat)> svd(evaluated_mat);

    auto w = svd.w();
    return *std::max_element(w.begin(), w.end());
}

template<typename Derived>
template<MatrixExpr RHS>
auto MatrixBase<Derived>::operator +(const RHS &rhs) const
{
    return MatBinaryExpr<Derived, RHS, AddOp>(this->derived(), rhs);
}

template<typename Derived>
template<MatrixExpr RHS>
auto MatrixBase<Derived>::operator -(const RHS &rhs) const
{
    return MatBinaryExpr<Derived, RHS, SubOp>(this->derived(), rhs);
}

template<typename Derived>
auto MatrixBase<Derived>::operator *(value_type scalar) const
{
    return MatScalarExpr<Derived, value_type, MulOp>(this->derived(), scalar);
}

template<typename Derived>
auto MatrixBase<Derived>::operator /(value_type scalar) const
{
    return MatScalarExpr<Derived, value_type, DivOp>(this->derived(), scalar);
}

template<typename Derived>
template<MatrixExpr Expr>
    requires (matrix_traits<Derived>::is_mutable)
auto MatrixBase<Derived>::operator+=(const Expr &expr) -> Derived &
{
    std::move(this->derived()) = this->derived() + expr;

    return this->derived();
}

template<typename Derived>
template<MatrixExpr Expr>
    requires (matrix_traits<Derived>::is_mutable)
auto MatrixBase<Derived>::operator-=(const Expr &expr) -> Derived &
{
    std::move(this->derived()) = this->derived() - expr;

    return this->derived();
}

template<typename Derived>
template<typename Scalar>
auto MatrixBase<Derived>::operator *=(Scalar scalar) -> Derived &
    requires (matrix_traits<Derived>::is_mutable &&
              std::is_convertible_v<Scalar, value_type>)
{
    std::move(this->derived()) = this->derived() * scalar;

    return this->derived();
}

template<typename Derived>
template<typename Scalar>
auto MatrixBase<Derived>::operator /=(Scalar scalar) -> Derived &
    requires (matrix_traits<Derived>::is_mutable &&
              std::is_convertible_v<Scalar, value_type>)
{
    std::move(this->derived()) = this->derived() / scalar;

    return this->derived();
}

template<typename Derived>
constexpr auto MatrixBase<Derived>::eval() const
{
    if constexpr (matrix_traits<Derived>::is_plain) {
        return this->derived();
    } else {

        using T = std::remove_cv_t<typename matrix_traits<Derived>::value_type>;
        constexpr size_t R = matrix_traits<Derived>::rows;
        constexpr size_t C = matrix_traits<Derived>::cols;

        return Matrix<T, R, C>(this->derived());
    }
}

template<typename DerivedL, typename DerivedR>
auto operator==(const MatrixBase<DerivedL> &lhs, 
                const MatrixBase<DerivedR> &rhs) -> bool
{
    static_assert(!matrix_traits<DerivedL>::is_expression &&
        !matrix_traits<DerivedR>::is_expression,
        "Expressions cannot be compared directly. Use .eval().");

    return detail::matrix_equal(lhs.derived(), rhs.derived());
}

template<typename DerivedL, typename DerivedR>
auto operator!=(const MatrixBase<DerivedL> &lhs,
                const MatrixBase<DerivedR> &rhs) -> bool
{
    return !(lhs == rhs);
}


template<typename Scalar, MatrixExpr Expr>
    requires std::is_convertible_v<Scalar, typename matrix_traits<Expr>::value_type>
auto operator*(const Scalar &scalar, const Expr &expr)
{
    return expr * scalar;
}

template<MatrixExpr LHS, MatrixExpr RHS>
auto operator*(const LHS &lhs, const RHS &rhs)
{
    return MatMulExpr<LHS, RHS>(lhs, rhs);
}


template<MatrixExpr Mat>
    requires (matrix_traits<Mat>::is_mutable)
auto operator<<(std::ostream &os, const Mat &mat) -> std::ostream &
{
    for (size_t r = 0; r < mat.rows(); r++) {
        for (size_t c = 0; c < mat.cols(); c++) {
            os << std::left << std::setw(12) << mat(r, c) << " ";
        }
        os << "\n";
    }
    return os;
}


/*! \} */





} // namespace tl