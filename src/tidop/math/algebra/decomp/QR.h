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

/*! \file QR.h
 * \brief QR decomposition using Householder transformations or LAPACK.
 *
 * This file defines the `QRDecomposition` class, which factorizes a matrix \( A \)
 * into an orthogonal matrix \( Q \) and an upper triangular matrix \( R \) such that
 * \( A = Q R \). The decomposition is useful for solving linear systems (especially
 * overdetermined or underdetermined), least squares problems, and eigenvalue algorithms.
 *
 * The implementation uses Householder reflections by default, with an option to use
 * LAPACK (OpenBLAS) when available for better performance.
 *
 * \ingroup Decomposition
 * \see tl::LuDecomposition, tl::CholeskyDecomposition, tl::SingularValueDecomposition
 */

#pragma once

#include <algorithm>

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/base/Lapack.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */


/*!
 * \class QRDecomposition
 * \brief QR decomposition of a matrix.
 *
 * \tparam Mat A type satisfying the `DenseMatrix` concept (e.g., `Matrix<T, Rows, Cols>`).
 *             The element type must be floating‑point (float, double).
 *
 * The decomposition is \( A = Q R \), where:
 * - \( Q \) is an orthogonal matrix (\( Q^T Q = I \)) of size `rows × rows`.
 * - \( R \) is an upper triangular matrix of size `rows × cols`.
 *
 * The class computes the decomposition at construction time and provides methods
 * to solve linear systems, retrieve \( Q \) and \( R \), and check singularity.
 *
 * ### Example
 * \code
 * Matrix<double, 4, 2> A = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
 * QRDecomposition<Matrix<double, 4, 3>> qr(A);
 * Vector<double, 4> b = {1, 1, 1, 1};
 * Vector<double, 2> x = qr.solve(b);   // least squares solution
 * auto Q = qr.q();
 * auto R = qr.r();
 * \endcode
 */
template<DenseMatrix Mat>
class QRDecomposition
{

public:

    using value_type = typename matrix_traits<Mat>::value_type;
    static constexpr size_t rows = matrix_traits<Mat>::rows;
    static constexpr size_t cols = matrix_traits<Mat>::cols;

private:

    Matrix<value_type, rows, rows> Q;      /*!< Orthogonal matrix Q. */
    Matrix<value_type, rows, cols> R;      /*!< Upper triangular matrix R. */
    bool singular;                         /*!< True if the matrix is (numerically) rank‑deficient. */
    size_t mRows;                          /*!< Number of rows. */
    size_t mCols;                          /*!< Number of columns. */

public:

    /*!
     * \brief Constructs the QR decomposition of a matrix.
     * \param[in] a The matrix to decompose.
     * \pre The element type must be floating‑point.
     */
    QRDecomposition(const Mat &a);

    /*!
     * \brief Solves the linear system \( A x = b \) (or least squares).
     * \tparam Vec A type satisfying `VectorExpr`.
     * \param[in] b The right‑hand side vector.
     * \return The solution vector \( x \) (size `cols()`).
     * \pre `b.size() == rows()`.
     * \throws `tl::Exception` if the matrix is singular or the solution fails.
     *
     * For overdetermined systems (rows > cols), this computes the least‑squares solution.
     * For underdetermined systems (rows < cols), the solution is not unique; this method
     * returns one particular solution (the minimum‑norm solution is not guaranteed).
     */
    template<VectorExpr Vec>
    auto solve(const Vec &b) -> Vector<value_type, cols>;

    /*!
     * \brief Returns the orthogonal matrix \( Q \).
     * \return A square matrix of size `rows × rows`.
     */
    auto q() const -> Matrix<value_type, rows, rows>;

    /*!
     * \brief Returns the upper triangular matrix \( R \).
     * \return A matrix of size `rows × cols`.
     */
    auto r() const -> Matrix<value_type, rows, cols>;

private:

    /*!
     * \brief Performs QR decomposition using Householder transformations (fallback).
     */
    void decompose();

#ifdef TL_HAVE_OPENBLAS
    /*!
     * \brief Performs QR decomposition using LAPACK's `geqrf` and `orgqr`.
     */
    void lapackeDecompose();
#endif // TL_HAVE_OPENBLAS

};


template<DenseMatrix Mat>
QRDecomposition<Mat>::QRDecomposition(const Mat &a)
  : Q(Matrix<value_type, rows, rows>::identity(a.rows(), a.rows())),
    R(a),
    singular(false),
    mRows(a.rows()),
    mCols(a.cols())
{
    static_assert(std::is_floating_point<value_type>::value, "Integral type not supported");

#ifdef TL_HAVE_OPENBLAS
    this->lapackeDecompose();
#else
    this->decompose();
#endif // TL_HAVE_OPENBLAS

}

template<DenseMatrix Mat>
void QRDecomposition<Mat>::decompose()
{
    try {

        size_t minRowsCols = std::min(mRows, mCols);

        // Vectores dinamicos para poder tomar el tamaño minimo
        Vector<value_type> v(minRowsCols);
        Vector<value_type> diagonal(minRowsCols);

        for (size_t k = 0; k < minRowsCols - 1; k++) {

            value_type scale = consts::zero<value_type>;

            for (size_t i = k; i < mRows; i++) {
                scale = std::max(scale, std::abs(R[i][k]));
            }

            if (isNearlyZero(scale)) {

                singular = true;
                v[k] = diagonal[k] = consts::zero<value_type>;

            } else {

                for (size_t i = k; i < mRows; i++) {
                    R[i][k] /= scale;
                }

                value_type aux{0};

                for (size_t i = k; i < mRows; i++) {
                    aux += R[i][k] * R[i][k];
                }

                value_type sigma = std::copysign(sqrt(aux), R[k][k]);
                R.at(k, k) += sigma;
                v[k] = sigma * R[k][k];
                diagonal[k] = -scale * sigma;

                for (size_t j = k + 1; j < mCols; j++) {

                    value_type aux{0};

                    for (size_t i = k; i < mRows; i++) {
                        aux += R[i][k] * R[i][j];
                    }

                    value_type tau = aux / v[k];

                    for (size_t i = k; i < mRows; i++) {
                        R[i][j] -= tau * R[i][k];
                    }

                }
            }
        }

        diagonal[minRowsCols - 1] = R[minRowsCols - 1][minRowsCols - 1];
        singular = (diagonal[minRowsCols - 1] == consts::zero<value_type>);

        for (size_t k = 0; k < minRowsCols - 1; k++) {

            if (v[k] != consts::zero<value_type>) {

                for (size_t i = 0; i < mRows; i++) {

                    value_type aux{0};
                    for (size_t j = k; j < mRows; j++) {
                        aux += R[j][k] * Q[i][j];
                    }

                    aux /= v[k];

                    for (size_t j = k; j < mRows; j++) {
                        Q[i][j] -= aux * R[j][k];
                    }
                }
            }
        }

        for (size_t r = 0; r < minRowsCols; r++) {
            R[r][r] = diagonal[r];
            for (size_t c = 0; c < r; c++) {
                R[r][c] = consts::zero<value_type>;
            }
        }

        for (size_t r = minRowsCols; r < mRows; r++) {
            for (size_t c = 0; c < mCols; c++) {
                if (r > c) R[r][c] = consts::zero<value_type>;
            }
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Lapack decompose exception");
    }
}

template<DenseMatrix Mat>
template<VectorExpr Vec>
auto QRDecomposition<Mat>::solve(const Vec &b) -> Vector<value_type, cols>
{
    TL_ASSERT(b.size() == mRows, "QRDecomposition::solve bad sizes");
    TL_ASSERT(!singular, "Singular");

    Vector<value_type, rows> y = Q.transpose() * b;
    Vector<value_type, cols> x(mCols);

    try {

#ifdef TL_HAVE_OPENBLAS 

        for (size_t i = 0; i < mCols; ++i) x[i] = y[i];

        bool isSingular = lapack::trtrs(lapack::Order::row_major,
                                        lapack::TriangularForm::upper,
                                        lapack::Transpose::no_trans,
                                        lapack::Diagonal::non_unit,
                                        mCols, 1, // n, nrhs
                                        R.data(), mCols, // lda es el número de columnas de R
                                        x.data(), 1);    // ldb suele ser 1 para un solo vector columna
        if (isSingular) singular = true;
#else

        Vector<value_type, rows> y = Q.transpose() * b;

        for (int i = mCols - 1; i >= 0; --i) {
            value_type sum = 0;
            for (int j = i + 1; j < mCols; ++j) {
                sum += R(i, j) * x[j];
            }
            x[i] = (y[i] - sum) / R(i, i);
        }

#endif

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error when trying to solve a system of linear equations");
    }

    return x;
}

template<DenseMatrix Mat>
auto QRDecomposition<Mat>::q() const -> Matrix<value_type, rows, rows>
{
    return Q;
}

template<DenseMatrix Mat>
auto QRDecomposition<Mat>::r() const -> Matrix<value_type, rows, cols>
{
    return R;
}

#ifdef TL_HAVE_OPENBLAS

template<DenseMatrix Mat>
void QRDecomposition<Mat>::lapackeDecompose()
{
    try {

        Matrix<value_type, rows, cols> QR(R);

        size_t m = QR.rows();
        size_t n = QR.cols();
        // En row-major order, cada fila está almacenada de forma contigua en memoria, 
        // por lo que la leading dimension es el número de columnas de A.
        size_t lda = QR.cols();
        size_t min_mn = std::min(m, n);
        std::vector<value_type> tau(min_mn);

        // Factorización QR usando geqrf
        lapack::geqrf(lapack::Order::row_major, m, n, QR.data(), lda, tau.data());

        singular = false;
        for (size_t i = 0; i < std::min(m, n); i++) {
            if (std::abs(QR(i, i)) < std::numeric_limits<value_type>::epsilon()) {
                singular = true;
                break;
            }
        }

        // Extraer R (es la parte superior de QR)
        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < n; j++) {
                if (i > j) R(i, j) = consts::zero<value_type>;
                else R(i, j) = QR(i, j);
            }
        }

        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < min_mn; j++) {
                Q(i, j) = QR(i, j);  // Copiar la parte de A que contiene Q
            }
        }

        // Calcular Q a partir de QR usando orgqr
        lapack::orgqr(lapack::Order::row_major, m, m, min_mn, Q.data(), m, tau.data());
    
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Lapack decompose exception");
    }
}

#endif // TL_HAVE_OPENBLAS


/*! \} */

} // End namespace tl
