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

#include "tidop/math/math.h"
#include "tidop/core/base/exception.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/base/lapack.h"
#include "tidop/math/base/cuda.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */

 /// \cond

template<typename T>
class LuDecomposition;

/// \endcond

/*!
 * \brief LU Decomposition with Partial and Full Pivoting
 *
 * The LU decomposition factorizes a matrix \( A \) into a lower triangular matrix \( L \)
 * and an upper triangular matrix \( U \), optionally applying row and column pivoting.
 * This decomposition is useful for solving systems of linear equations, computing the determinant,
 * and finding the inverse of a matrix.
 *
 * The decomposition is given by:
 * \f[ P A Q = L U \f]
 * where:
 *  - \( P \) is a permutation matrix representing row swaps.
 *  - \( Q \) is a permutation matrix representing column swaps (only in full pivoting).
 *  - \( L \) is a lower triangular matrix with ones on the diagonal.
 *  - \( U \) is an upper triangular matrix.
 *
 * \tparam Matrix_t The matrix type (e.g., `Matrix`).
 * \tparam T The data type of elements in the matrix (e.g., `double`).
 * \tparam _rows The number of rows in the matrix.
 * \tparam _cols The number of columns in the matrix.
 */
template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
class LuDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

    /*!
     * \brief Constructs an LU Decomposition of a given matrix
     *
     * This constructor performs the LU decomposition of matrix \( A \), storing the result
     * in a compact form where \( L \) and \( U \) share the same storage.
     *
     * \param[in] a The matrix \( A \) to decompose.
     * \param[in] fullPivot If true, full pivoting (row and column swapping) is used.
     */
    LuDecomposition(const Matrix_t<T, _rows, _cols> &a, bool fullPivot = false);

    /*!
     * \brief Solves the system of equations \( A \cdot x = b \) using the LU decomposition
     *
     * Using the decomposed matrix \( LU \), this method solves the system of linear equations
     * \( A \cdot x = b \), where \( A \) is the matrix and \( b \) is the right-hand side vector.
     *
     * \param[in] b The right-hand side vector \( b \).
     * \return The solution vector \( x \).
     */
    auto solve(const Vector<T, _rows> &b) const -> Vector<T, _rows>;

    /*!
     * \brief Solves the system of equations \( A \cdot X = B \) using the LU decomposition
     *
     * Using the LU decomposition, this method solves the system of linear equations
     * \( A \cdot X = B \), where \( A \) is the matrix and \( B \) is the matrix of right-hand side vectors.
     *
     * \param[in] b The right-hand side matrix \( B \).
     * \return The solution matrix \( X \).
     */
    template<typename Matrix2_t>
    auto solve(const Matrix2_t &b) const -> Matrix2_t;

    /*!
     * \brief Gets the LU decomposition matrix
     *
     * This method returns the matrix \( LU \), which contains both the lower triangular matrix \( L \)
     * and the upper triangular matrix \( U \).
     *
     * \return The matrix \( LU \), which contains both \( L \) and \( U \).
     */
    auto lu() const -> Matrix_t<T, _rows, _cols>;

    /*!
     * \brief Computes the determinant of the original matrix \( A \)
     *
     * The determinant is given by:
     * \f[
     * \det(A) = d \cdot \prod_{i} U(i, i)
     * \f]
     * where \( d \) accounts for row and column permutations.
     *
     * \return The determinant of \( A \).
     */
    // Con Lapack no se esta calculando
    //auto determinant() const->T;

    /*!
     * \brief Computes the inverse of the matrix \( A \) using LU decomposition.
     *
     * This method solves \( A X = I \) using LU factorization, where \( X \) is the inverse of \( A \).
     * If the matrix is singular (non-invertible), an exception is thrown.
     *
     * \return The inverse matrix \( A^{-1} \).
     */
    auto inverse() const -> Matrix_t<T, _rows, _cols>;

    /*!
     * \brief Computes the rank of the matrix \( A \).
     *
     * The rank is the number of nonzero pivots in the upper triangular matrix \( U \).
     *
     * \return The rank of \( A \).
     */
    auto rank() const -> size_t;

    /*!
     * \brief Checks if the matrix \( A \) is singular.
     *
     * A matrix is singular if it has a determinant of zero, or equivalently, if any diagonal element of \( U \) is zero.
     *
     * \return True if the matrix is singular, false otherwise.
     */
    //auto isSingular() const -> bool;

    /*!
     * \brief Extracts the lower triangular matrix \( L \).
     *
     * The matrix \( L \) is returned with ones on its diagonal.
     *
     * \return The lower triangular matrix \( L \).
     */
    auto lower() const -> Matrix_t<T, _rows, _cols>;

    /*!
     * \brief Extracts the upper triangular matrix \( U \).
     *
     * The matrix \( U \) is returned as an upper triangular matrix.
     *
     * \return The upper triangular matrix \( U \).
     */
    auto upper() const -> Matrix_t<T, _rows, _cols>;

private:

    /*!
     * \brief Performs the LU decomposition.
     *
     * This method factorizes the matrix \( A \) into \( L \) and \( U \), applying
     * row and optionally column pivoting.
     */
    void decompose();

private:

    Matrix_t<T, _rows, _cols> LU;
    bool mFullPivot;
    Vector<int, _rows> mPivotIndexRow;
    Vector<int, _rows> mPivotIndexCol;
    T d;
    size_t mRows;
    size_t mCols;
    Vector<int, _rows > mColPer;
};


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
LuDecomposition<Matrix_t<T, _rows, _cols>>::LuDecomposition(const Matrix_t<T, _rows, _cols> &a, bool fullPivot)
  : LU(a),
    mFullPivot(fullPivot),
    mPivotIndexRow(a.rows()),
    mPivotIndexCol(a.rows()),
    d(consts::one<T>),
    mRows(a.rows()),
    mCols(a.cols()),
    mColPer(mRows)
{
    static_assert(Rows == Cols, "Non-Square Matrix");
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
    TL_ASSERT(mRows == cols, "Non-Square Matrix");

    for (size_t i = 0; i < mRows; i++) {
        mColPer[i] = i;
    }

    this->decompose();
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b) const -> Vector<T, _rows>
{
    TL_ASSERT(b.size() == mRows, "LuDecomposition::solve bad sizes");

    Vector<T, _rows> x(b);

#ifdef TL_HAVE_OPENBLAS 
    if (!mFullPivot) {
        lapack_int nrhs = 1;
        lapack_int lda = static_cast<int>(mRows);
        lapack_int ldb = 1;
        lapack_int info = lapack::getrs(lapack::Order::row_major,
            lapack::LUTransposeMode::NoTrans,
            static_cast<int>(mRows), nrhs, const_cast<T *>(LU.data()), lda, const_cast<int *>(mPivotIndexRow.data()), x.data(), ldb);
    } else {
#endif

        T sum;
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
            } else if (sum != consts::zero<T>) {
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
            Vector<T, _rows> y = x;

            // Reordenamos la solución aplicando la permutación de columnas: 
            for (size_t i = 0; i < mRows; i++) {
                x[mColPer[i]] = y[i];
            }
        }
#ifdef TL_HAVE_OPENBLAS 
    }
#endif

    return x;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
template<typename Matrix2_t>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Matrix2_t &b) const -> Matrix2_t
///Por ahora solo funciona con matrizes dinamicas
{
    TL_ASSERT(b.rows() == mRows, "LuDecomposition::solve bad sizes");

    Matrix2_t x(b);

#ifdef TL_HAVE_OPENBLAS 
    if (!mFullPivot) {
        lapack_int info;
        lapack_int nrhs = static_cast<lapack_int>(b.cols());
        lapack_int lda = static_cast<lapack_int>(mRows);
        lapack_int ldb = static_cast<lapack_int>(b.cols());

        info = lapack::getrs(lapack::Order::row_major, lapack::LUTransposeMode::NoTrans, mRows, nrhs, const_cast<T *>(LU.data()), lda, const_cast<int *>(mPivotIndexRow.data()), x.data(), ldb);

    } else {
#endif

        Vector<T, _rows> xx(mRows);

        for (size_t j = 0; j < x.cols(); j++) {

            for (size_t i = 0; i < mRows; i++) {
                xx[i] = b[i][j];
            }

            xx = this->solve(xx);

            for (size_t i = 0; i < mRows; i++) {
                x[i][j] = xx[i];
            }
        }

#ifdef TL_HAVE_OPENBLAS 
    }
#endif

    return x;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
void LuDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
#ifdef TL_HAVE_OPENBLAS 
    if (!mFullPivot) {
        lapack_int info;
        lapack_int lda = mRows;

        info = lapack::getrf(lapack::Order::row_major, mRows, mRows, LU.data(), lda, mPivotIndexRow.data());

        TL_ASSERT(info == 0, "The algorithm computing LU failed to converge.");

    } else {
#endif

        for (size_t k = 0; k < mRows; k++) {

            T big = consts::zero<T>;
            size_t pivot_row = k;
            size_t pivot_col = k;

            for (size_t i = k; i < mRows; i++) {

                if (mFullPivot) {
                    for (size_t j = k; j < mRows; j++) {
                        T temp = std::abs(LU[i][j]);
                        if (temp > big) {
                            big = temp;
                            pivot_row = i;
                            pivot_col = j;
                        }
                    }
                } else {
                    T temp = std::abs(LU[i][k]);
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

            mPivotIndexRow[k] = pivot_row;

            // Intercambio de columnas
            if (mFullPivot) {
                if (k != pivot_col) {
                    LU.swapCols(pivot_col, k);
                    std::swap(mColPer[k], mColPer[pivot_col]);
                }
                mPivotIndexCol[k] = pivot_col;
            }

            if (isNearlyZero(LU[k][k]))
                LU[k][k] = std::numeric_limits<T>::epsilon();

            T llkk = LU[k][k];

            // Actualización de la submatriz
            for (size_t i = k + 1; i < mRows; i++) {

                T temp = LU[i][k];
                temp /= llkk;
                LU[i][k] = temp;

                for (size_t j = k + 1; j < mRows; j++)
                    LU[i][j] -= temp * LU[k][j];
            }
        }

#ifdef TL_HAVE_OPENBLAS 
    }
#endif
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::lu() const -> Matrix_t<T, _rows, _cols>
{
    return LU;
}

//template<
//    template<typename, size_t, size_t>
//class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//auto LuDecomposition<Matrix_t<T, _rows, _cols>>::determinant() const -> T
//{
//    T det = this->d;
//
//    for (size_t i = 0; i < mRows; i++)
//        det *= LU[i][i];
//
//    return det;
//}


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::inverse() const -> Matrix_t<T, _rows, _cols>
{
    Matrix_t<T, _rows, _cols> I = Matrix_t<T, _rows, _cols>::identity(mRows, mCols);
    return solve(I);
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::rank() const -> size_t
{
    size_t r = 0;
    for (size_t i = 0; i < mRows; i++) {
        if (!isNearlyZero(LU(i, i))) {
            r++;
        }
    }
    return r;
}

//template<
//    template<typename, size_t, size_t>
//class Matrix_t, typename T, size_t _rows, size_t _cols
//>
//auto LuDecomposition<Matrix_t<T, _rows, _cols>>::isSingular() const -> bool
//{
//    for (size_t i = 0; i < mRows; i++) {
//        if (isNearlyZero(LU(i, i))) {
//            return true;
//        }
//    }
//    return false;
//}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::lower() const -> Matrix_t<T, _rows, _cols>
{
    Matrix_t<T, _rows, _cols> L = Matrix_t<T, _rows, _cols>::identity(mRows, mCols);
    for (size_t i = 0; i < mRows; i++) {
        for (size_t j = 0; j < i; j++) {
            L(i, j) = LU(i, j);
        }
    }
    return L;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto LuDecomposition<Matrix_t<T, _rows, _cols>>::upper() const -> Matrix_t<T, _rows, _cols>
{
    Matrix_t<T, _rows, _cols> U = Matrix_t<T, _rows, _cols>::zero(mRows, mCols);
    for (size_t i = 0; i < mRows; i++) {
        for (size_t j = i; j < mRows; j++) {
            U(i, j) = LU(i, j);
        }
    }
    return U;
}

/*! \} */

} // End namespace tl
