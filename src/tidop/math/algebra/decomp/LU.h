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

/*! \file LU.h
 * \brief LU decomposition with partial or full pivoting.
 *
 * This file defines the `LuDecomposition` class, which performs LU decomposition
 * of a square matrix \( A \) with optional row pivoting (partial) or row and column
 * pivoting (full). The decomposition is stored compactly in a single matrix \( LU \)
 * where the lower part (excluding diagonal) contains the multipliers of \( L \) and
 * the upper part contains the elements of \( U \). The diagonal of \( L \) is implicitly 1.
 *
 * The class provides methods to solve linear systems, compute the determinant,
 * inverse, rank, and to extract the \( L \) and \( U \) factors.
 *
 * \ingroup Decomposition
 * \see tl::CholeskyDecomposition, tl::QRDecomposition, tl::SingularValueDecomposition
 */

#pragma once

#include "tidop/math/math.h"
#include "tidop/core/base/Exception.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/base/Lapack.h"
#include "tidop/math/base/Cuda.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */

 /*!
  * \class LuDecomposition
  * \brief LU decomposition with partial or full pivoting.
  *
  * \tparam Mat A type satisfying the `DenseMatrix` concept (e.g., `Matrix<T, Rows, Cols>`).
  *             The matrix must be square and have a floating‑point element type.
  *
  * The decomposition is \( P A Q = L U \), where:
  * - \( P \) is a row permutation matrix (partial pivoting) or both \( P \) and \( Q \) (full pivoting).
  * - \( L \) is lower triangular with ones on the diagonal.
  * - \( U \) is upper triangular.
  *
  * The compact storage `LU` contains the strict lower part of \( L \) (the multipliers)
  * and the whole \( U \) (including its diagonal). The diagonal of \( L \) is assumed to be 1.
  *
  * ### Example
  * \code
  * Matrix<double, 3, 3> A = {{2, -1, -2}, {-4, 6, 3}, {-4, -2, 8}};
  * LuDecomposition<Matrix<double, 3, 3>> lu(A); // partial pivoting by default
  * Vector<double, 3> b = {2, 2, 2};
  * auto x = lu.solve(b);
  * double det = lu.determinant();
  * auto inv = lu.inverse();
  * auto L = lu.lower();
  * auto U = lu.upper();
  * \endcode
  */
template<DenseMatrix Mat>
class LuDecomposition
{

public:

    using value_type = typename matrix_traits<Mat>::value_type;
    static constexpr size_t rows = matrix_traits<Mat>::rows;
    static constexpr size_t cols = matrix_traits<Mat>::cols;

private:

    Matrix<value_type, rows, cols> LU;      /*!< Compact storage: L (strict lower) + U (upper). */
    bool mFullPivot;                        /*!< If true, full pivoting (row+column); else partial. */
    Vector<int, rows> mPivotIndexRow;       /*!< Row permutation indices (0‑based). */
    Vector<int, rows> mPivotIndexCol;       /*!< Column permutation indices (full pivoting only). */
    value_type d;                           /*!< Sign of the permutation (±1). */
    size_t mRows;                           /*!< Number of rows (actual dimension). */
    size_t mCols;                           /*!< Number of columns (equal to mRows for square). */
    Vector<int, rows> mColPer;              /*!< Column permutation for solution reconstruction. */
    bool mIsSingular;                       /*!< True if the matrix is (numerically) singular. */

public:

    /*!
     * \brief Constructs the LU decomposition.
     * \param[in] a The square matrix to decompose.
     * \param[in] fullPivot If `true`, perform full pivoting (row and column);
     *                      otherwise, only partial (row) pivoting.
     * \pre `a.rows() == a.cols()` (square matrix).
     * \pre The element type must be floating‑point.
     */
    LuDecomposition(const Mat &a, bool fullPivot = false);

    /*!
     * \brief Solves \( A x = b \) for a single right‑hand side vector.
     * \tparam Vec A type satisfying `VectorExpr`.
     * \param[in] b The right‑hand side vector.
     * \return The solution vector \( x \) of length `rows`.
     * \pre `b.size() == rows()`.
     * \throws `tl::Exception` if the matrix is singular or the solve fails.
     */
    template<VectorExpr Vec>
    auto solve(const Vec &b) const -> Vector<value_type, rows>;

    /*!
     * \brief Solves \( A X = B \) for multiple right‑hand sides.
     * \tparam MatExpr A type satisfying `MatrixExpr`.
     * \param[in] b The right‑hand side matrix (each column is a separate system).
     * \return The solution matrix \( X \) of size `rows` × `cols(b)`.
     * \pre `b.rows() == rows()`.
     * \throws `tl::Exception` if the matrix is singular or the solve fails.
     */
    template<MatrixExpr MatExpr>
    auto solve(const MatExpr &b) const/* -> Matrix<value_type, rows, cols>*/;

    /*!
     * \brief Returns the compact LU matrix.
     * \return The matrix `LU` containing the strict lower part of \( L \) and the whole \( U \).
     */
    auto lu() const -> Matrix<value_type, rows, cols>;

    /*!
     * \brief Computes the determinant of the original matrix \( A \).
     * \return The determinant.
     * \note The sign `d` is taken into account based on row (and column) swaps.
     */
    auto determinant() const -> value_type;

    /*!
     * \brief Computes the inverse of the original matrix \( A \) using the LU decomposition.
     * \return The inverse matrix \( A^{-1} \).
     * \throws `tl::Exception` if the matrix is singular.
     */
    auto inverse() const -> Matrix<value_type, rows, cols>;

    /*!
     * \brief Returns the rank of the original matrix.
     * \return The rank, computed as the number of non‑zero pivots in \( U \)
     *         (using a tolerance for near‑zero).
     */
    auto rank() const -> size_t;

    /*!
     * \brief Checks whether the matrix is (numerically) singular.
     * \return `true` if any pivot is zero (within a small epsilon), `false` otherwise.
     */
    auto isSingular() const -> bool;

    /*!
     * \brief Extracts the lower triangular matrix \( L \).
     * \return A matrix with ones on the diagonal and the multipliers from the compact `LU`.
     */
    auto lower() const -> Matrix<value_type, rows, cols>;

    /*!
     * \brief Extracts the upper triangular matrix \( U \).
     * \return The upper triangular matrix from the compact `LU`.
     */
    auto upper() const -> Matrix<value_type, rows, cols>;

private:

    /*!
     * \brief Performs the actual LU decomposition (partial or full pivoting).
     * \post `LU`, `mPivotIndexRow`, `mPivotIndexCol`, `d`, and `mIsSingular` are set.
     */
    void decompose();

};


template<DenseMatrix Mat>
LuDecomposition<Mat>::LuDecomposition(const Mat &a, bool fullPivot)
  : LU(a),
    mFullPivot(fullPivot),
    mPivotIndexRow(a.rows()),
    mPivotIndexCol(a.rows()),
    d(consts::one<value_type>),
    mRows(a.rows()),
    mCols(a.cols()),
    mColPer(mRows),
    mIsSingular(false)
{
    static_assert(rows == cols, "Non-Square Matrix");
    static_assert(std::is_floating_point<value_type>::value, "Integral type not supported");
    TL_ASSERT(mRows == mCols, "Non-Square Matrix");

    for (size_t i = 0; i < mRows; i++) {
        mColPer[i] = static_cast<int>(i);
    }

    this->decompose();
}

template<DenseMatrix Mat>
template<VectorExpr Vec>
auto LuDecomposition<Mat>::solve(const Vec &b) const -> Vector<value_type, rows>
{
    TL_ASSERT(b.size() == mRows, "LuDecomposition::solve bad sizes");
    TL_ASSERT(!mIsSingular, "The matrix is singular and cannot be solved.");

    Vector<value_type, rows> x(b);

    try {

#ifdef TL_HAVE_OPENBLAS 

        if (!mFullPivot) {

            size_t nrhs = 1;
            size_t lda = mRows;
            size_t ldb = 1;
            lapack::getrs(lapack::Order::row_major,
                          lapack::Transpose::no_trans,
                          mRows, nrhs, LU.data(), lda, 
                          mPivotIndexRow.data(), x.data(), ldb);

        } else {

#endif

            value_type sum;
            size_t ii = 0;

            // 1. Sustitución hacia adelante (L * y = Pb)
            for (size_t i = 0; i < mRows; i++) {

                size_t pivot_index = mPivotIndexRow[i];
                sum = x[pivot_index];
                x[pivot_index] = x[i];

                if (ii != 0) {
                    for (size_t j = ii - 1; j < i; j++) {
                        sum -= LU(i, j) * x[j];
                    }
                } else if (sum != consts::zero<value_type>) {
                    ii = i + 1;
                }

                x[i] = sum;
            }

            // 2. Sustitución hacia atrás (U * x = y)
            for (int i = static_cast<int>(mRows - 1); i >= 0; i--) {

                sum = x[i];

                for (size_t j = static_cast<size_t>(i + 1); j < mRows; j++)
                    sum -= LU(i, j) * x[j];

                x[i] = sum / LU(i, i);
            }

            // 3. Si se usó pivotación completa, reordenar la solución aplicando la permutación inversa de columnas.
            if (mFullPivot) {
                Vector<value_type, rows> y = x;

                // Reordenamos la solución aplicando la permutación de columnas: 
                for (size_t i = 0; i < mRows; i++) {
                    x[mColPer[i]] = y[i];
                }
            }
#ifdef TL_HAVE_OPENBLAS 
        }
#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error when trying to solve a system of linear equations");
    }

    return x;
}

template<DenseMatrix Mat>
template<MatrixExpr MatExpr>
auto LuDecomposition<Mat>::solve(const MatExpr &b) const/* -> Matrix<value_type, rows, cols>*/
{
    TL_ASSERT(b.rows() == mRows, "LuDecomposition::solve bad sizes");
    TL_ASSERT(!mIsSingular, "The matrix is singular and cannot be solved.");

    Matrix<value_type> x(b);

    try {

#ifdef TL_HAVE_OPENBLAS 

        if (!mFullPivot) {

            lapack_int nrhs = static_cast<lapack_int>(b.cols());
            lapack_int lda = static_cast<lapack_int>(mRows);
            lapack_int ldb = static_cast<lapack_int>(b.cols());

            lapack::getrs(lapack::Order::row_major, 
                          lapack::Transpose::no_trans,
                          mRows, nrhs, LU.data(), lda,
                          mPivotIndexRow.data(), x.data(), ldb);

        } else {

#endif

            for (size_t j = 0; j < x.cols(); j++) {
                Vector<value_type, rows> temp = b.col(j);
                x.col(j) = this->solve(temp);
            }

#ifdef TL_HAVE_OPENBLAS 
        }
#endif

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error when trying to solve a system of linear equations");
    }

    return x;
}

template<DenseMatrix Mat>
void LuDecomposition<Mat>::decompose()
{
    try {
#ifdef TL_HAVE_OPENBLAS 
        if (!mFullPivot) {

            auto lda = mRows;
            mIsSingular = lapack::getrf(lapack::Order::row_major, mRows, mRows, LU.data(), lda, mPivotIndexRow.data());

            int rowSwaps = 0;
            for (int i = 0; i < mRows; ++i) {
                if (mPivotIndexRow[i] != i + 1) {
                    rowSwaps++;
                }
            }

            this->d = (rowSwaps % 2 == 0) ? value_type(1) : value_type(-1);

        } else {
#endif

            for (size_t k = 0; k < mRows; k++) {

                value_type big = consts::zero<value_type>;
                size_t pivot_row = k;
                size_t pivot_col = k;

                for (size_t i = k; i < mRows; i++) {

                    if (mFullPivot) {
                        for (size_t j = k; j < mRows; j++) {
                            value_type temp = std::abs(LU[i][j]);
                            if (temp > big) {
                                big = temp;
                                pivot_row = i;
                                pivot_col = j;
                            }
                        }
                    } else {
                        value_type temp = std::abs(LU[i][k]);
                        if (temp > big) {
                            big = temp;
                            pivot_row = i;
                        }
                    }
                }

                // Intercambio de filas
                if (k != pivot_row) {
                    LU.swapRows(pivot_row, k);
                    this->d = -this->d;
                }

                mPivotIndexRow[k] = static_cast<int>(pivot_row);

                // Intercambio de columnas
                if (mFullPivot) {
                    if (k != pivot_col) {
                        LU.swapCols(pivot_col, k);
                        std::swap(mColPer[k], mColPer[pivot_col]);
                    }
                    mPivotIndexCol[k] = static_cast<int>(pivot_col);
                }

                if (isNearlyZero(LU[k][k])) {
                    mIsSingular = true;
                    LU[k][k] = std::numeric_limits<value_type>::epsilon();
                }

                value_type llkk = LU[k][k];

                // Actualización de la submatriz
                for (size_t i = k + 1; i < mRows; i++) {

                    value_type temp = LU[i][k];
                    temp /= llkk;
                    LU[i][k] = temp;

                    for (size_t j = k + 1; j < mRows; j++)
                        LU[i][j] -= temp * LU[k][j];
                }
            }

#ifdef TL_HAVE_OPENBLAS 
        }
#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("LU decompose error");
    }
}

template<DenseMatrix Mat>
auto LuDecomposition<Mat>::lu() const -> Matrix<value_type, rows, cols>
{
    return LU;
}

template<DenseMatrix Mat>
auto LuDecomposition<Mat>::determinant() const -> value_type
{
    value_type det = this->d;

    for (size_t i = 0; i < mRows; i++)
        det *= LU(i,i);

    return det;
}

template<DenseMatrix Mat>
auto LuDecomposition<Mat>::inverse() const -> Matrix<value_type, rows, cols>
{
    return solve(Matrix<value_type, rows, cols>::identity(mRows, mCols));
}

template<DenseMatrix Mat>
auto LuDecomposition<Mat>::rank() const -> size_t
{
    size_t r = 0;
    for (size_t i = 0; i < mRows; i++) {
        if (!isNearlyZero(LU(i, i))) {
            r++;
        }
    }
    return r;
}

template<DenseMatrix Mat>
auto LuDecomposition<Mat>::isSingular() const -> bool
{
    return mIsSingular;
}

template<DenseMatrix Mat>
auto LuDecomposition<Mat>::lower() const -> Matrix<value_type, rows, cols>
{
    auto L = Matrix<value_type, rows, cols>::identity(mRows, mCols);
    for (size_t i = 0; i < mRows; i++) {
        for (size_t j = 0; j < i; j++) {
            L(i, j) = LU(i, j);
        }
    }
    return L;
}

template<DenseMatrix Mat>
auto LuDecomposition<Mat>::upper() const -> Matrix<value_type, rows, cols>
{
    auto U = Matrix<value_type, rows, cols>::zero(mRows, mCols);
    for (size_t i = 0; i < mRows; i++) {
        for (size_t j = i; j < mRows; j++) {
            U(i, j) = LU(i, j);
        }
    }
    return U;
}

/*! \} */

} // End namespace tl
