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

#include "tidop/math/base/data.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/BaseExpr.h"
#include "tidop/math/algebra/expr/MatAddExpr.h"
#include "tidop/math/algebra/expr/MatSubExpr.h"
#include "tidop/math/algebra/expr/MatMulScalarExpr.h"
#include "tidop/math/algebra/expr/MatDivScalarExpr.h"
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


template<typename Derived>
class MatrixBase
  : public BaseExpr<Derived>
{

public:

    using derived_type = Derived;
    using value_type = typename matrix_traits<Derived>::value_type;

public:

    /*!
     * \brief Inverse matrix
     * A square and invertible matrix A has an inverse matrix \f[ A^{-1} \f]
     * \param[out] invertibility Checks if the matrix is invertible
     * \return The inverse matrix
     * <h4>Example</h4>
     * \code
     * Matrix<double, 2, 2> mat_2x2{2., 3.,
     *                              1., 4.};
     * bool invertible;
     * Matrix<double, 2, 2> inv_mat = mat_2x2.inverse(&invertible);
     * \endcode
     */
    auto inverse(bool *invertibility = nullptr) const
    {
        auto evaluated_mat = this->eval();

        return detail::invert_matrix(evaluated_mat, invertibility);
    }

    /*!
     * \brief Calculates the transpose matrix
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
     * <h4>Example</h4>
     * \code
     * Matrix<double, 2, 2> mat_2x2{2., 3.,
     *                               1., 4.};
     * Matrix<double, 2, 2> transpose_mat = mat_2x2.transpose();
     * std::cout << transpose_mat << std::endl;
     * \endcode
     *
     * \return The transpose matrix
     */
    constexpr auto transpose() const
    {
        return TransposeExpr<Derived>(this->derived());
    }

    /*!
     * \brief Determinant of the matrix
     * \return Determinant
     */
    auto determinant() const
    {
        auto evaluated_mat = this->eval();
        return detail::matrix_determinant(evaluated_mat);
    }

    /*!
     * \brief Calculates the adjugate matrix
     * \f[ adj(A) = C^T \f]
     * \return The adjugate matrix
     */
    auto adjugate() const
    {
        auto evaluated_mat = this->eval();
        return detail::matrix_adjoint(evaluated_mat);
    }

    /*!
     * \brief Calculates the cofactor matrix
     * \return The cofactor matrix
     */
    auto cofactorMatrix() const
    {
        auto evaluated_mat = this->eval();
        return detail::matrix_cofactor_matrix(evaluated_mat);
    }

    /*!
     * \brief Cofactor
     * The determinant obtained by removing the row and column of a given element from a matrix or determinant.
     * The cofactor is preceded by a sign + or - depending on whether the element is in a + or - position.
     * \f[ (-)^{r+j} \f]
     * \return The cofactor
     */
    auto cofactor(size_t r, size_t c) const -> value_type
    {
        auto evaluated_mat = this->eval();
        return detail::matrix_cofactor(evaluated_mat, r, c);
    }

    /*!
     * \brief Trace of a square matrix
     * Sum of the elements on the main diagonal of a square matrix
     * \f[ tr(A) = a_{11} + ... +  a_{nn} \f]
     * \return The trace of the matrix
     */
    auto trace() const -> value_type
    {
        static_assert(matrix_traits<Derived>::rows == DynamicData ||
                      matrix_traits<Derived>::cols == DynamicData ||
                      matrix_traits<Derived>::rows == matrix_traits<Derived>::cols,
            "Static matrices must be square for trace");

        size_t r = this->rows();
        TL_ASSERT(r == this->cols(), "Non-Square Matrix");

        value_type sum = consts::zero<value_type>;

        //auto &expr = this->derived();
        auto eval = make_evaluator(this->derived());

        for (size_t i = 0; i < r; ++i) {
            sum += eval.coeff(i, i);
        }

        return sum;
    }

    /*!
     * \brief Checks if the matrix is invertible
     * \return True if the matrix is invertible
     */
    auto isInvertible() const -> bool
    {
        return (this->determinant() != consts::zero<value_type>);
    }

    /*!
     * \brief Checks if the matrix is singular
     * A square matrix that has no inverse is singular. The
     * determinant of a singular matrix is 0
     * \return True if the matrix is singular
     */
    auto isSingular() const -> bool
    {
        return (this->determinant() == consts::zero<value_type>);
    }

    /*!
     * \brief Checks if the matrix is square (i.e., same number of rows and columns)
     * \return True if the matrix is square, false otherwise
     */
    constexpr auto isSquare() const -> bool
    {
        return this->rows() == this->cols();
    }

    /*!
     * \brief Checks if the matrix is symmetric
     * \return True if the matrix is symmetric, false otherwise
     */
    auto isSymmetric() const -> bool
    {
        if (!this->isSquare()) return false;

        //auto &expr = this->derived();
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

    /*!
     * \brief Checks if the matrix is a diagonal matrix.
     *
     * A matrix is diagonal if all non-diagonal elements are zero.
     * Only square matrices can be diagonal.
     *
     * \return true if the matrix is diagonal, false otherwise.
     */
    auto isDiagonal() const -> bool 
    {
        if (!this->isSquare()) return false;

        //auto &expr = this->derived();
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

    /*!
     * \brief Checks if the matrix is an identity matrix.
     *
     * A matrix is an identity matrix if it is square, all diagonal elements are 1,
     * and all non-diagonal elements are 0.
     *
     * \return true if the matrix is an identity matrix, false otherwise.
     */
    auto isIdentity() const -> bool
    {
        if (!this->isSquare()) return false;

        //auto &expr = this->derived();
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

    /*!
     * \brief Checks if the matrix is upper triangular.
     *
     * A matrix is upper triangular if all elements below the main diagonal are zero.
     *
     * \return true if the matrix is upper triangular, false otherwise.
     */
    auto isUpperTriangular() const -> bool
    {
        if (!this->isSquare()) return false;

        //auto &expr = this->derived();
        auto eval = make_evaluator(this->derived());

        for (size_t r = 1; r < this->rows(); ++r) {
            for (size_t c = 0; c < r; ++c) {
                if (!isNearlyZero(eval.coeff(r,c))) {
                    return false;
                }
            }
        }

        return true;
    }

    /*!
     * \brief Checks if the matrix is lower triangular.
     *
     * A matrix is lower triangular if all elements above the main diagonal are zero.
     *
     * \return true if the matrix is lower triangular, false otherwise.
     */
    auto isLowerTriangular() const -> bool
    {
        if (!this->isSquare()) return false;

        //auto &expr = this->derived();
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
    auto isTriangular() const -> bool
    {
        return this->isUpperTriangular() || this->isLowerTriangular();
    }

    /*!
     * \brief First minor
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
     * \return The first minor
     */
    auto firstMinor(size_t r, size_t c) const
    {
        auto evaluated_mat = this->eval();
        return detail::matrix_first_minor(evaluated_mat, r, c);
    }

    /*!
     * \brief Row echelon form
     */
    auto rowEchelonForm() const
    {
        auto evaluated_mat = this->eval();
        return detail::matrix_row_echelon_form(evaluated_mat);
    }

    /*!
     * \brief Reduced row echelon form
     */
    auto reducedRowEchelonForm() const
    {
        auto evaluated_mat = this->eval();
        return detail::matrix_reduced_row_echelon_form(evaluated_mat);
    }

    /*!
     * \brief Rank of a matrix
     */
    auto rank() const -> int
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

    //TODO: Implementar diagonal() para matrices mutables y const, usando 
    //      MatrixDiagonal como vista para evitar copias innecesarias
    auto diagonal() const -> Vector<value_type>
    {
        auto &derived = this->derived();
        size_t size = std::min(derived.rows(), derived.cols());
        Vector<value_type> diag_vector(size);

        auto eval = make_evaluator(derived);

        for (size_t i = 0; i < size; ++i) {
            diag_vector[i] = eval.coeff(i, i);
        }

        return diag_vector;
    }

    // Para lectura/escritura en matrices mutables
    //auto diagonal() -> MatrixDiagonal<Derived> 
    //{
    //    return MatrixDiagonal<Derived>(this->derived());
    //}

    // Para solo lectura en matrices const
    //auto diagonal() const -> MatrixDiagonal<const Derived> 
    //{
    //    return MatrixDiagonal<const Derived>(this->derived());
    //}

    /*!
     * \brief Computes the Frobenius norm of the matrix.
     *
     * The Frobenius norm is defined as:
     * \f[
     * \|A\|_F = \sqrt{\sum_{i,j} |a_{ij}|^2}
     * \f]
     *
     * \return T The Frobenius norm of the matrix.
     */
    auto frobeniusNorm() const -> value_type
    {
        //auto &expr = this->derived();
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

    /*!
     * \brief Computes the L1 norm of the matrix (maximum column sum).
     *
     * The L1 norm is defined as:
     * \f[
     * \|A\|_1 = \max_j \sum_i |a_{ij}|
     * \f]
     *
     * \return T The L1 norm of the matrix.
     */
    auto l1Norm() const -> value_type
    {
        //auto &expr = this->derived();
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
     * \return T The approximate L2 norm of the matrix.
     */
    //auto l2Norm() const -> value_type
    //{
    //    auto evaluated_mat = this->eval();

    //    SingularValueDecomposition<decltype(evaluated_mat)> svd(evaluated_mat);

    //    auto w = svd.w();
    //    return *std::max_element(w.begin(), w.end());
    //}

    // Subido a BaseExpr para que esté disponible para vectores y matrices
    //constexpr auto derived() -> derived_type&
    //{
    //    return static_cast<Derived &>(*this); 
    //}

    //constexpr auto derived() const -> const derived_type&
    //{ 
    //    return static_cast<const Derived &>(*this);
    //}

    constexpr auto rows() const noexcept -> size_t { return this->derived().rows(); }
    constexpr auto cols() const noexcept -> size_t { return this->derived().cols(); }

    //decltype(auto) operator()(size_t i, size_t j)
    //{ 
    //    return this->derived()(i, j);
    //}

    //decltype(auto) operator()(size_t i, size_t j) const
    //{
    //    return this->derived()(i, j);
    //}

    //decltype(auto) operator()(size_t i) { return this->derived()(i); }
    //decltype(auto) operator()(size_t i) const { return this->derived()(i); }

    //constexpr auto packet(size_t i) const
    //    requires (matrix_traits<Derived>::has_contiguous_memory)
    //{ 
    //    return this->derived().packet(i);
    //}

    //auto aliases(const void *ptr) const -> bool { return derived().aliases(ptr); }

    /* Unary arithmetic operators */

    auto operator+() const noexcept -> const Derived &
    {
        return this->derived();
    }

    auto operator-() const
    {
        return MatUnaryMinusExpr<Derived>(this->derived());
    }

    /* Binary arithmetic operators */

    template<MatrixExpr RHS>
    auto operator +(const RHS &rhs) const
    {
        return MatAddExpr<Derived, RHS>(this->derived(), rhs);
    }

    template<MatrixExpr RHS>
    auto operator -(const RHS &rhs) const
    {
        return MatSubExpr<Derived, RHS>(this->derived(), rhs);
    }

    auto operator *(value_type scalar) const
    {
        return MatMulScalarExpr<Derived, value_type>(this->derived(), scalar);
    }

    auto operator /(value_type scalar) const
    {
        return MatDivScalarExpr<Derived, value_type>(this->derived(), scalar);
    }

    template<MatrixExpr Expr>
        requires (matrix_traits<Derived>::is_mutable)
    auto operator+=(const Expr &expr) -> Derived &
    {
        std::move(this->derived()) = this->derived() + expr;

        return this->derived();
    }

    template<MatrixExpr Expr>
        requires (matrix_traits<Derived>::is_mutable)
    auto operator-=(const Expr &expr) -> Derived &
    {
        std::move(this->derived()) = this->derived() - expr;

        return this->derived();
    }

    template<typename Scalar>
        requires (matrix_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>)
    auto operator *=(Scalar scalar) -> Derived &
    {
        std::move(this->derived()) = this->derived() * scalar;

        return this->derived();
    }


    template<typename Scalar>
        requires (matrix_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>)
    auto operator /=(Scalar scalar) -> Derived &
    {
        std::move(this->derived()) = this->derived() / scalar;

        return this->derived();
    }

    constexpr auto eval() const
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

    //template<typename OtherDerived>
    //bool operator==(const MatrixBase<OtherDerived> &other) const
    //{
    //    const auto &self = this->derived();
    //    const auto &rhs = other.derived();

    //    if (self.rows() != rhs.rows() || self.cols() != rhs.cols()) {
    //        return false;
    //    }

    //    size_t size = self.rows() * self.cols();
    //    for (size_t i = 0; i < size; ++i) {
    //        if (self(i) != rhs(i)) {
    //            return false;
    //        }
    //    }

    //    return true;
    //}

    //template<typename OtherDerived>
    //bool operator!=(const MatrixBase<OtherDerived> &other) const
    //{
    //    return !(*this == other);
    //}

//    template<typename Scalar>
//        requires (matrix_traits<Derived>::is_mutable &&
//                  std::is_convertible_v<Scalar, value_type>)
//    void fill(Scalar value)
//    {
//        auto &derived = this->derived();
//        size_t size = derived.rows() * derived.cols();
//        size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//        if (matrix_traits<Derived>::has_contiguous_memory) {
//            Packed<value_type> packed_val(value);
//            constexpr size_t packed_size = packed_val.size();
//            size_t max_size = size - size % packed_size;
//
//            for (; i < max_size; i += packed_size) {
//                packed_val.storeUnaligned(&derived(i));
//            }
//        }
//#endif
//
//        for (; i < size; i++) {
//            derived(i) = value;
//        }
//    }

};


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

} // End namespace tl