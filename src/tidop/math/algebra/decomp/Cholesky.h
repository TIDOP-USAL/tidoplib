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

/*! \file Cholesky.h
 * \brief Cholesky decomposition of symmetric positive‑definite matrices.
 *
 * This file defines the `CholeskyDecomposition` class, which computes the Cholesky
 * decomposition \( A = L L^T \) of a symmetric positive‑definite matrix \( A \).
 * The lower triangular matrix \( L \) is stored and can be used to solve linear
 * systems, compute the inverse, or evaluate the determinant.
 *
 * The implementation uses LAPACK (OpenBLAS) when available; otherwise a fallback
 * implementation is used.
 *
 * \ingroup Decomposition
 * \see tl::QRDecomposition, tl::LuDecomposition, tl::SingularValueDecomposition
 */

#pragma once

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/base/Lapack.h"


namespace tl
{


/*! \addtogroup Decomposition
 *  \{
 */

/*!
 * \class CholeskyDecomposition
 * \brief Cholesky decomposition of a symmetric positive‑definite matrix.
 *
 * \tparam Mat A type satisfying the `DenseMatrix` concept (e.g., `Matrix<T, Rows, Cols>`).
 *             The matrix must be square and have a floating‑point element type.
 *
 * This class performs the Cholesky decomposition \( A = L L^T \) on a symmetric
 * positive‑definite matrix \( A \). The decomposition is computed at construction
 * time and stored as the lower triangular matrix \( L \). The class provides
 * methods to solve linear systems, compute the inverse, and obtain the determinant.
 *
 * ### Example
 * \code
 * Matrix<double, 3, 3> A = {{4, 12, -16},
 *                           {12, 37, -43},
 *                           {-16, -43, 98}};
 * CholeskyDecomposition<Matrix<double, 3, 3>> cholesky(A);
 * Vector<double, 3> b = {1, 2, 3};
 * Vector<double, 3> x = cholesky.solve(b);
 * double det = cholesky.determinant();
 * auto inv = cholesky.inverse();
 * \endcode
 */
template<DenseMatrix Mat>
class CholeskyDecomposition
{

public:

    using value_type = typename matrix_traits<Mat>::value_type;
    static constexpr size_t rows = matrix_traits<Mat>::rows;
    static constexpr size_t cols = matrix_traits<Mat>::cols;

protected:

    Mat L;          /*!< Lower triangular matrix from the decomposition. */
    size_t mRows;   /*!< Number of rows (equals number of columns). */

public:

    /*!
     * \brief Constructor that performs the Cholesky decomposition.
     * \param[in] a The symmetric positive‑definite matrix to decompose.
     * \pre `a.rows() == a.cols()` (square matrix).
     * \pre The element type of `a` must be floating‑point (e.g., `float`, `double`).
     * \throws tl::Exception if the matrix is not positive‑definite or if the
     *         decomposition fails (e.g., due to non‑square input).
     */
    CholeskyDecomposition(const Mat &a);

    /*!
     * \brief Solves the linear system \( A x = b \) for the vector \( b \).
     * \tparam Vec A type satisfying `VectorExpr`.
     * \param[in] b The right‑hand side vector.
     * \return The solution vector \( x \) of length `rows`.
     * \pre `b.size() == rows()`.
     * \throws tl::Exception if the solve fails (e.g., due to a singular matrix).
     */
    template<VectorExpr Vec>
    auto solve(const Vec &b) const -> Vector<value_type, rows>;

    /*!
     * \brief Solves the linear system \( A X = B \) for the matrix \( B \).
     * \tparam MatExpr A type satisfying `MatrixExpr`.
     * \param[in] B The right‑hand side matrix (each column is a system).
     * \return The solution matrix \( X \) of size `rows` × `cols(B)`.
     * \pre `B.rows() == rows()`.
     * \throws tl::Exception if the solve fails.
     */
    template<MatrixExpr MatExpr>
    auto solve(const MatExpr &B) const -> Matrix<value_type, rows, cols>;

    /*!
     * \brief Returns the lower triangular matrix \( L \).
     * \return A square matrix of size `rows` containing the lower triangular factor.
     */
    auto lower() const -> Matrix<value_type, rows, cols>;

    /*!
     * \brief Computes the inverse of the original matrix \( A \).
     * \return The inverse matrix \( A^{-1} \) of size `rows` × `cols`.
     * \note The inverse is computed using the Cholesky decomposition and is
     *       only valid for positive‑definite matrices.
     */
    auto inverse() const -> Matrix<value_type, rows, cols>;

    /*!
     * \brief Computes the determinant of the original matrix \( A \).
     * \return The determinant (positive value).
     * \note The determinant is computed as \( (\prod_{i} L_{ii})^2 \).
     */
    auto determinant() const -> value_type;

private:

    /*!
     * \brief Internal routine that performs the Cholesky decomposition.
     * \post The matrix `L` is overwritten with the lower triangular factor.
     * \throws tl::Exception if the decomposition fails (non‑positive‑definite).
     */
    void decompose();

};


template<DenseMatrix Mat>
CholeskyDecomposition<Mat>::CholeskyDecomposition(const Mat &a)
  : L(a),
    mRows(a.rows())
{
    static_assert(rows == cols, "Cholesky decomposition requires a square matrix.");
    static_assert(std::is_floating_point<value_type>::value, "Integral type not supported");
    TL_ASSERT(a.rows() == a.cols(), "Cholesky decomposition requires a square matrix.");

    this->decompose();

    for (size_t i = 0; i < mRows; i++) {
        for (size_t j = 0; j < i; j++) {
            L[j][i] = consts::zero<value_type>;
        }
    }
}



template<DenseMatrix Mat>
template<VectorExpr Vec>
auto CholeskyDecomposition<Mat>::solve(const Vec &b) const -> Vector<value_type, rows>
{
    TL_ASSERT(b.size() == mRows, "bad lengths in Cholesky");

    Vector<value_type, rows> x(b);

    try {

#ifdef TL_HAVE_OPENBLAS 

        size_t nrhs = 1;  // Un único vector de términos constantes b
        // lda y ldb son el número de filas de las matrices L y X (que es igual a mRows)
        size_t lda = L.cols();
        size_t ldb = 1;

        lapack::potrs(lapack::Order::row_major,          // Orden de la matriz
                      lapack::TriangularForm::lower,     // L es triangular inferior
                      mRows,                             // Número de filas de L
                      nrhs,                              // Número de términos constantes (1 en este caso)
                      L.data(),                          // Matriz L
                      lda,                               // Paso entre filas de L
                      x.data(),                          // La solución del sistema
                      ldb);                              // Paso entre filas de X

#else

        value_type sum;

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

#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error when trying to solve a system of linear equations");
    }

    return x;
}

template<DenseMatrix Mat>
template<MatrixExpr MatExpr>
auto CholeskyDecomposition<Mat>::solve(const MatExpr &B) const -> Matrix<value_type, rows, cols>
{
    TL_ASSERT(B.rows() == mRows, "Cholesky::solve - Bad matrix dimensions");

    Matrix<value_type, rows, cols> X(B);

    try {

#ifdef TL_HAVE_OPENBLAS 

        // Número de columnas en la matriz B (equivale a "nrhs" en LAPACK, ya que cada columna es un sistema independiente)
        size_t nrhs = B.cols();
        // lda: En RowMajor, es el número de columnas de la matriz L
        size_t lda = L.cols();  // Leading dimension de L
        // ldb: En RowMajor, es el número de columnas de la matriz X (que es MatB::cols)
        size_t ldb = X.cols();  // Leading dimension de X

        lapack::potrs(lapack::Order::row_major,          // Orden de la matriz
                      lapack::TriangularForm::lower,     // L es triangular inferior
                      mRows,                             // Número de filas de L
                      nrhs,                              // Número de columnas de B (número de sistemas a resolver)
                      L.data(),                          // Matriz L
                      lda,                               // Leading dimension de L
                      X.data(),                          // Matriz X con las soluciones
                      ldb);                              // Leading dimension de X

#else

        for (size_t j = 0; j < B.cols(); j++) {
            Vector<value_type, rows> temp = B.col(j);
            X.col(j) = this->solve(temp);
        }

#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error when trying to solve a system of linear equations");
    }

    return X;
}

template<DenseMatrix Mat>
void CholeskyDecomposition<Mat>::decompose()
{
    try {

#ifdef TL_HAVE_OPENBLAS
        
        lapack::potrf(lapack::Order::row_major, lapack::TriangularForm::lower, L.rows(), L.data(), L.cols());

#else
        for (size_t i = 0; i < mRows; i++) {

            for (size_t j = i; j < mRows; j++) {

                value_type sum = L[i][j];

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
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Cholesky decompose exception");
    }
}

template<DenseMatrix Mat>
auto CholeskyDecomposition<Mat>::lower() const -> Matrix<value_type, rows, cols>
{
    return L;
}

template<DenseMatrix Mat>
auto CholeskyDecomposition<Mat>::inverse() const -> Matrix<value_type, rows, cols>
{
    Matrix<value_type, rows, cols> inv = L;

    try {
#ifdef TL_HAVE_OPENBLAS
        size_t lda = inv.cols();

        lapack::potri(lapack::Order::row_major,
                      lapack::TriangularForm::lower,
                      mRows,
                      inv.data(),
                      lda);

        // potri solo llena la parte triangular inferior (en este caso).
        // Debemos reflejarla para tener la matriz completa.
        for (size_t i = 0; i < mRows; ++i) {
            for (size_t j = i + 1; j < mRows; ++j) {
                inv(i, j) = inv(j, i);
            }
        }
#else
        inv = solve(Matrix<value_type, rows, cols>::identity(mRows, mRows));
#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error computing Cholesky inverse");
    }

    return inv;
}


template<DenseMatrix Mat>
auto CholeskyDecomposition<Mat>::determinant() const -> value_type
{
    value_type det = 1;
    for (size_t i = 0; i < mRows; i++) {
        det *= L[i][i];
    }
    return det * det; 
}

/*! \} */

} // End namespace tl
