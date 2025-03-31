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
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/base/lapack.h"


namespace tl
{


/*! \addtogroup Decomposition
 *  \{
 */

/// \cond

template<typename T>
class CholeskyDecomposition;

/// \endcond
/*!
 * \brief Cholesky Decomposition
 *
 * This class provides an implementation of the Cholesky decomposition, which is a method
 * for decomposing a positive-definite, symmetric matrix into the product of a lower triangular
 * matrix L and its transpose, such that \( A = L L^T \).
 *
 * Given a matrix \( A \), if it is symmetric and positive-definite, the Cholesky decomposition
 * can be used to find \( L \), the lower triangular matrix. The matrix \( A \) must be symmetric
 * and positive-definite for the decomposition to be applicable.
 *
 * The Cholesky decomposition is often used in numerical methods, such as solving systems of linear equations,
 * optimization, and inverting positive-definite matrices. The decomposition is numerically stable and efficient.
 *
 * \tparam Matrix_t The type of the matrix (e.g., `Matrix`).
 * \tparam T The type of the elements in the matrix (e.g., `double`).
 * \tparam _rows The number of rows in the matrix.
 * \tparam _cols The number of columns in the matrix.
 */
template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
class CholeskyDecomposition<Matrix_t<T, _rows, _cols>>
{

public:
    /*!
     * \brief Constructs a Cholesky Decomposition from a given matrix
     *
     * This constructor performs the Cholesky decomposition of the given matrix \( A \) and
     * stores the resulting lower triangular matrix \( L \).
     *
     * The matrix \( A \) must be symmetric and positive-definite for the decomposition to succeed.
     * If these conditions are not met, the decomposition will fail.
     *
     * \param[in] a The matrix \( A \) to decompose, which must be symmetric and positive-definite.
     */
    CholeskyDecomposition(const Matrix_t<T, _rows, _cols> &a);
    /*!
     * \brief Solves the system of equations \( A \cdot x = b \) using the Cholesky decomposition
     *
     * Using the decomposed matrix \( L \), this method solves the system of linear equations
     * \( A \cdot x = b \), where \( A \) is the matrix and \( b \) is the right-hand side vector.
     * The solution is obtained by first solving \( L \cdot y = b \), and then solving \( L^T \cdot x = y \).
     *
     * This method is more efficient than directly solving \( A \cdot x = b \) by using methods like Gaussian elimination,
     * as it takes advantage of the Cholesky decomposition.
     *
     * \param[in] b The right-hand side vector \( b \).
     * \return The solution vector \( x \).
     */
    auto solve(const Vector<T, _rows> &b) const -> Vector<T, _rows>;

    /*!
     * \brief Solves the system of equations \( A \cdot X = B \) using the Cholesky decomposition
     *
     * This method solves the system of linear equations \( A \cdot X = B \), where \( A \) is the matrix
     * and \( B \) is a matrix of right-hand side vectors. The solution is obtained by solving each column
     * of \( B \) individually using the Cholesky decomposition.
     *
     * \param[in] B The matrix \( B \) of right-hand side vectors.
     * \return The matrix \( X \) containing the solutions.
     */
    auto solve(const Matrix<T, _rows, _cols> &B) const->Matrix<T, _rows, _cols>;

    /*!
     * \brief Gets the lower triangular matrix \( L \)
     *
     * This method returns the lower triangular matrix \( L \) from the Cholesky decomposition,
     * such that \( A = L L^T \).
     *
     * \return The lower triangular matrix \( L \).
     */
    auto lower() const -> Matrix_t<T, _rows, _cols>;

    /*!
     * \brief Computes the inverse of the matrix using the Cholesky decomposition
     *
     * This method computes the inverse of the matrix \( A \) using the Cholesky decomposition.
     * It is applicable only if \( A \) is symmetric and positive-definite.
     *
     * The method uses the Cholesky decomposition to solve \( A \cdot X = I \), where \( I \) is the identity matrix.
     *
     * \return The inverse matrix of \( A \).
     */
    auto inverse() const -> Matrix_t<T, _rows, _cols>;

    /*!
     * \brief Computes the determinant of the matrix \(A\) using Cholesky decomposition.
     *
     * This method computes the determinant of a symmetric, positive-definite matrix \(A\)
     * using the Cholesky decomposition \(A = L \cdot L^T\). The determinant is the square
     * of the product of the diagonal elements of the lower triangular matrix \(L\).
     *
     * \return The determinant of the matrix \(A\).
     */
    auto determinant() const -> T;

private:

    /*!
     * \brief Performs the Cholesky decomposition
     *
     * This private method decomposes the matrix \( A \) into its lower triangular matrix \( L \).
     */
    void decompose();

protected:

    Matrix_t<T, _rows, _cols> L;
    size_t mRows;
};


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::CholeskyDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : L(a),
    mRows(a.rows())
{
    static_assert(_rows == _cols, "Cholesky decomposition requires a square matrix.");
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
    TL_ASSERT(a.rows() == a.cols(), "Cholesky decomposition requires a square matrix.");

    this->decompose();

    for (size_t i = 0; i < mRows; i++) {
        for (size_t j = 0; j < i; j++) {
            L[j][i] = consts::zero<T>;
        }
    }
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b) const -> Vector<T, _rows>
{
    TL_ASSERT(b.size() == mRows, "bad lengths in Cholesky");

    Vector<T, _rows> x(b);
//#ifdef TL_HAVE_OPENBLAS 
//
//    lapack_int info;
//    lapack_int nrhs = 1;  // Un único vector de términos constantes b
//    // lda y ldb son el número de filas de las matrices L y X (que es igual a mRows)
//    lapack_int lda = mRows;
//    lapack_int ldb = mRows;
//
//    info = lapack::potrs(lapack::Order::row_major,          // Orden de la matriz
//                         lapack::TriangularForm::lower,     // L es triangular inferior
//                         mRows,                             // Número de filas de L
//                         nrhs,                              // Número de términos constantes (1 en este caso)
//                         L.data(),                          // Matriz L
//                         lda,                               // Paso entre filas de L
//                         x.data(),                          // La solución del sistema
//                         ldb);                              // Paso entre filas de X
//
//    TL_ASSERT(info == 0, "LAPACK error in potrs");
//
//#else

    T sum;

    for (size_t r = 0; r < mRows; r++) {

        sum = b[r];

        for (size_t k = r; k > 0; k--)
            sum -= L[r][k - 1] * x[k - 1];

        x[r] = sum / L[r][r];

    }

    for (size_t i = mRows; i > 0; i--) {

        sum = x[i - 1];

        for (size_t k = i; k < mRows; k++)
            sum -= L[k][i - 1] * x[k];

        x[i - 1] = sum / L[i - 1][i - 1];

    }

//#endif

    return x;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Matrix<T, _rows, _cols> &B) const -> Matrix<T, _rows, _cols>
{
    TL_ASSERT(B.rows() == mRows, "Cholesky::solve - Bad matrix dimensions");

    Matrix<T, _rows, _cols> X(B);

//#ifdef TL_HAVE_OPENBLAS 
//
//    lapack_int info;
//
//    // Número de columnas en la matriz B (equivale a "nrhs" en LAPACK, ya que cada columna es un sistema independiente)
//    lapack_int nrhs = B.cols();
//    lapack_int lda = mRows;  // Leading dimension de L
//    lapack_int ldb = mRows;  // Leading dimension de X
//
//    info = lapack::potrs(lapack::Order::row_major,          // Orden de la matriz
//                         lapack::TriangularForm::lower,     // L es triangular inferior
//                         mRows,                             // Número de filas de L
//                         nrhs,                              // Número de columnas de B (número de sistemas a resolver)
//                         L.data(),                          // Matriz L
//                         lda,                               // Leading dimension de L
//                         X.data(),                          // Matriz X con las soluciones
//                         ldb);                              // Leading dimension de X
//    
//    TL_ASSERT(info == 0, "LAPACK error in potrs");
//
//#else

    for (size_t j = 0; j < B.cols(); j++) {
        Vector<T, _rows> temp = B.col(j);
        X.col(j) = this->solve(temp);
    }

//#endif

    return X;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
void CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
#ifdef TL_HAVE_OPENBLAS
    lapack_int info;

    info = lapack::potrf(lapack::Order::row_major, lapack::TriangularForm::lower , L.rows(), L.data(), L.cols());

    TL_ASSERT(info >= 0, "Cholesky decomposition failed");
#else
    for (size_t i = 0; i < mRows; i++) {

        for (size_t j = i; j < mRows; j++) {

            T sum = L[i][j];

            for (size_t k = i; k > 0; k--) {
                sum -= L[i][k - 1] * L[j][k - 1];
            }

            if (i == j) {
                TL_ASSERT(sum > 0.0, "Cholesky decomposition failed");
                L[i][i] = sqrt(sum);
            } else {
                L[j][i] = sum / L[i][i];
            }

        }
    }
#endif
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::lower() const -> Matrix_t<T, _rows, _cols>
{
    return L;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::inverse() const -> Matrix_t<T, _rows, _cols>
{
    return solve(Matrix_t<T, _rows, _cols>::identity(mRows, mRows));
}


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto CholeskyDecomposition<Matrix_t<T, _rows, _cols>>::determinant() const -> T
{
    T det = 1;
    for (size_t i = 0; i < mRows; i++) {
        det *= L[i][i];
    }
    return det * det; 
}

/*! \} */

} // End namespace tl
