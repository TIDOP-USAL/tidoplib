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

#include <algorithm>

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

 //https://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf
 //https://rosettacode.org/wiki/QR_decomposition#C.2B.2B

template<typename T>
class QRDecomposition;

/// \endcond

/*!
 * \brief QR Decomposition
 *
 * The QR decomposition of a matrix is a factorization of the matrix into the product of two matrices:
 * an orthogonal matrix \( Q \) and an upper triangular matrix \( R \). The QR decomposition is often
 * used to solve linear systems, compute eigenvalues, and perform least squares fitting.
 *
 * For a matrix \( A \), the QR decomposition is given by:
 * \f[ A = Q \cdot R \f]
 * where \( Q \) is an orthogonal matrix, and \( R \) is an upper triangular matrix. The orthogonality
 * of \( Q \) is given by:
 * \f[ Q^T \cdot Q = I \f]
 * where \( Q^T \) is the transpose of \( Q \) and \( I \) is the identity matrix.
 *
 * The QR decomposition can be computed using different algorithms, such as Householder transformations.
 * This class implements the Householder method for QR decomposition.
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
class QRDecomposition<Matrix_t<T, _rows, _cols>>
{

public:

    /*!
     * \brief Constructs the QR decomposition from a given matrix
     *
     * This constructor performs the QR decomposition of the matrix \( A \), storing the resulting
     * orthogonal matrix \( Q \) and the upper triangular matrix \( R \).
     *
     * \param[in] a The matrix \( A \) to decompose.
     */
    QRDecomposition(const Matrix_t<T, _rows, _cols> &a);

    /*!
     * \brief Solves the system of equations \( A \cdot x = b \) using the QR decomposition
     *
     * Using the QR decomposition, this method solves the system of linear equations \( A \cdot x = b \),
     * where \( A \) is the matrix and \( b \) is the right-hand side vector.
     *
     * \param[in] b The right-hand side vector \( b \).
     * \return The solution vector \( x \).
     */
    auto solve(const Vector<T, _rows> &b) -> Vector<T, _cols>;

    /*!
     * \brief Gets the orthogonal matrix \( Q \)
     *
     * This method returns the orthogonal matrix \( Q \) from the QR decomposition.
     *
     * \return The matrix \( Q \), which is orthogonal.
     */
    auto q() const->Matrix<T, _rows, _rows>;

    /*!
     * \brief Gets the upper triangular matrix \( R \)
     *
     * This method returns the upper triangular matrix \( R \) from the QR decomposition.
     *
     * \return The matrix \( R \), which is upper triangular.
     */
    auto r() const->Matrix<T, _rows, _cols>;

private:

    /*!
     * \brief Performs the QR decomposition using Householder transformations
     *
     * This private method performs the QR decomposition of the matrix using the Householder method,
     * which generates orthogonal transformations to zero out the lower triangular part of the matrix.
     */
    void decompose();

#ifdef TL_HAVE_OPENBLAS
    void lapackeDecompose();
#endif // TL_HAVE_OPENBLAS

private:

    Matrix<T, _rows, _rows> Q;
    Matrix<T, _rows, _cols> R;
    bool singular;
    size_t mRows;
    size_t mCols;
};


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
QRDecomposition<Matrix_t<T, _rows, _cols>>::QRDecomposition(const Matrix_t<T, _rows, _cols> &a)
  : Q(Matrix<T, _rows, _rows>::identity(a.rows(), a.rows())),
    R(a),
    singular(false),
    mRows(a.rows()),
    mCols(a.cols())
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

#ifdef TL_HAVE_OPENBLAS
    this->lapackeDecompose();
#else
    this->decompose();
#endif // TL_HAVE_OPENBLAS

}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
void QRDecomposition<Matrix_t<T, _rows, _cols>>::decompose()
{
    size_t minRowsCols = std::min(mRows, mCols);

    // Vectores dinamicos para poder tomar el tamaño minimo
    Vector<T> v(minRowsCols);
    Vector<T> diagonal(minRowsCols);

    for (size_t k = 0; k < minRowsCols - 1; k++) {

        T scale = consts::zero<T>;

        for (size_t i = k; i < mRows; i++) {
            scale = std::max(scale, std::abs(R[i][k]));
        }
        
        if (isNearlyZero(scale)) {

            singular = true;
            v[k] = diagonal[k] = consts::zero<T>;

        } else {

            for (size_t i = k; i < mRows; i++) {
                R[i][k] /= scale;
            }

            T aux{0};

            for (size_t i = k; i < mRows; i++) {
                aux += R[i][k] * R[i][k];
            }

            T sigma = std::copysign(sqrt(aux), R[k][k]);
            R.at(k, k) += sigma;
            v[k] = sigma * R[k][k];
            diagonal[k] = -scale * sigma;

            for (size_t j = k + 1; j < mCols; j++) {

                T aux{0};

                for (size_t i = k; i < mRows; i++) {
                    aux += R[i][k] * R[i][j];
                }

                T tau = aux / v[k];

                for (size_t i = k; i < mRows; i++) {
                    R[i][j] -= tau * R[i][k];
                }

            }
        }
    }

    diagonal[minRowsCols - 1] = R[minRowsCols - 1][minRowsCols - 1];
    singular = (diagonal[minRowsCols - 1] == consts::zero<T>);

    for (size_t k = 0; k < minRowsCols - 1; k++) {

        if (v[k] != consts::zero<T>) {

            for (size_t i = 0; i < mRows; i++) {

                T aux{0};
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
            R[r][c] = consts::zero<T>;
        }
    }

    for (size_t r = minRowsCols; r < mRows; r++) {
        for (size_t c = 0; c < mCols; c++) {
            if (r > c) R[r][c] = consts::zero<T>;
        }
    }
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto QRDecomposition<Matrix_t<T, _rows, _cols>>::solve(const Vector<T, _rows> &b) -> Vector<T, _cols>
{
    TL_ASSERT(b.size() == mRows, "QRDecomposition::solve bad sizes");
    TL_ASSERT(!singular, "Singular");

    Vector<T, _rows> y = Q.transpose() * b;
    Vector<T, _cols> x(mCols);

    for (int i = mCols - 1; i >= 0; --i) {
        T sum = 0;
        for (int j = i + 1; j < mCols; ++j) {
            sum += R(i, j) * x[j];
        }
        x[i] = (y[i] - sum) / R(i, i);
    }

    return x;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto QRDecomposition<Matrix_t<T, _rows, _cols>>::q() const -> Matrix<T, _rows, _rows>
{
    return Q;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
auto QRDecomposition<Matrix_t<T, _rows, _cols>>::r() const -> Matrix<T, _rows, _cols>
{
    return R;
}

#ifdef TL_HAVE_OPENBLAS

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t _rows, size_t _cols
>
inline void QRDecomposition<Matrix_t<T, _rows, _cols>>::lapackeDecompose()
{
    Matrix_t<T, _rows, _cols> QR(R);

    lapack_int info;
    lapack_int m = QR.rows();
    lapack_int n = QR.cols();
    // En row-major order, cada fila está almacenada de forma contigua en memoria, 
    // por lo que la leading dimension es el número de columnas de A.
    lapack_int lda = QR.cols(); 

    std::vector<T> tau(std::min(m, n));

    // Factorización QR usando geqrf
    info = lapack::geqrf(lapack::Order::row_major, m, n, QR.data(), lda, tau.data());

    TL_ASSERT(info >= 0, "LAPACKE_geqrf failed.");

    singular = false;
    for (size_t i = 0; i < std::min(m, n); i++) {
        if (std::abs(QR(i, i)) < std::numeric_limits<T>::epsilon()) {
            singular = true;
            break;
        }
    }

    // Extraer R (es la parte superior de QR)
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            if (i > j) R(i, j) = consts::zero<T>;
            else R(i, j) = QR(i, j);
        }
    }

    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            Q(i,j) = QR(i, j);  // Copiar la parte de A que contiene Q
        }
    }

    // Calcular Q a partir de QR usando orgqr
    info = lapack::orgqr(lapack::Order::row_major, m, m, tau.size(), Q.data(), m, tau.data());
    TL_ASSERT(info >= 0, "LAPACKE_orgqr failed.");
}

#endif // TL_HAVE_OPENBLAS


/*! \} */

} // End namespace tl
