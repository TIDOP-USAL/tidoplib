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

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/base/lapack.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */


/// \cond

// http://www.ehu.eus/izaballa/Cursos/valores_singulares.pdf
// https://www.researchgate.net/publication/263583897_La_descomposicion_en_valores_singulares_SVD_y_algunas_de_sus_aplicaciones

template<typename T>
class SingularValueDecomposition;

/// \endcond

/*!
 * \brief Singular Value Decomposition (SVD)
 *
 * In linear algebra, the Singular Value Decomposition (SVD) of a matrix \( A \) is a factorization
 * of the matrix that has many applications, such as solving linear systems, signal processing, and data
 * compression. The SVD of a matrix allows it to be expressed as a product of three matrices: an orthogonal
 * matrix \( U \), a diagonal matrix \( \Sigma \), and the transpose of another orthogonal matrix \( V \).
 *
 * Given an \( m \times n \) matrix \( A \), the SVD is given by:
 * \f[ A = U \cdot \Sigma \cdot V^T \f]
 * where:
 * - \( U \) is an orthogonal matrix of size \( m \times m \),
 * - \( \Sigma \) is a diagonal matrix of size \( m \times n \),
 * - \( V \) is an orthogonal matrix of size \( n \times n \).
 *
 * The SVD provides a powerful tool for analyzing and solving linear systems, especially in cases
 * where the matrix is non-square or ill-conditioned.
 *
 * \tparam Matrix_t The type of the matrix (e.g., `Matrix`).
 * \tparam T The type of the elements in the matrix (e.g., `double`).
 * \tparam Rows The number of rows in the matrix.
 * \tparam Cols The number of columns in the matrix.
 */
template<
    template<typename, size_t, size_t>
    class Matrix_t, typename T, size_t Rows, size_t Cols>
class SingularValueDecomposition<Matrix_t<T, Rows, Cols>>
{

public:

    /*!
     * \brief Constructs the Singular Value Decomposition (SVD) of a matrix
     *
     * This constructor performs the SVD of the matrix \( A \), storing the resulting orthogonal matrices
     * \( U \) and \( V \), as well as the singular values in the vector \( W \).
     *
     * \param[in] a The matrix \( A \) to decompose.
     */
    SingularValueDecomposition(const Matrix_t<T, Rows, Cols> &a);

    /*!
     * \brief Solves the system of equations \( A \cdot x = b \) using the SVD
     *
     * Using the SVD, this method solves the system of linear equations \( A \cdot x = b \), where \( A \)
     * is the matrix and \( b \) is the right-hand side vector. This is done by using the properties of
     * the matrices \( U \), \( \Sigma \), and \( V \) from the SVD.
     *
     * \param[in] b The right-hand side vector \( b \).
     * \return The solution vector \( x \).
     */
    auto solve(const Vector<T, Rows>& b) -> Vector<T, Cols>;

    /*!
     * \brief Gets the orthogonal matrix \( U \)
     *
     * This method returns the orthogonal matrix \( U \) from the SVD decomposition.
     *
     * \return The matrix \( U \), which is orthogonal.
     */
    auto u() const -> Matrix<T, Rows, Cols>;

    /*!
     * \brief Gets the orthogonal matrix \( V \)
     *
     * This method returns the orthogonal matrix \( V \) from the SVD decomposition.
     *
     * \return The matrix \( V \), which is orthogonal.
     */
    auto v() const -> Matrix<T, Cols, Cols>;

    /*!
     * \brief Gets the singular values as a vector \( W \)
     *
     * This method returns the singular values of the matrix as a vector \( W \).
     *
     * \return The vector of singular values \( W \).
     */
    auto w() const -> Vector<T, Cols>;

    /*!
     * \brief Gets the maximum number of iterations for the decomposition
     *
     * This method returns the maximum number of iterations that will be performed during the decomposition.
     *
     * \return The maximum number of iterations.
     */
    auto maxIterations() const -> int;

    /*!
     * \brief Sets the maximum number of iterations for the decomposition
     *
     * This method allows the user to set the maximum number of iterations to be performed during the
     * decomposition.
     *
     * \param[in] maxIterations The maximum number of iterations.
     */
    void setMaxIterations(int maxIterations);

private:

    void decompose();
    void reorder();
#ifdef TL_HAVE_OPENBLAS
    void lapackDecompose();
#endif // TL_HAVE_OPENBLAS

private:

    Matrix<T, Rows, Cols> A;
    Matrix<T, Rows, Cols> U;
    Matrix<T, Cols, Cols> V;
    Vector<T, Cols> W;
    int mIterationMax;
    T eps;
    T tsh;
    size_t mRows;
    size_t mCols;
};


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::SingularValueDecomposition(const Matrix_t<T, Rows, Cols> &a)
    : A(a),
    mIterationMax(30),
    mRows(a.rows()),
    mCols(a.cols())
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    U = Matrix<T, Rows, Cols>(mRows, mCols);
    V = Matrix<T, Cols, Cols>(mCols, mCols);
    W = Vector<T, Cols>(mCols);

#ifdef TL_HAVE_OPENBLAS
    this->lapackDecompose();
#else
    eps = std::numeric_limits<T>::epsilon();
    this->decompose();
    this->reorder();
    tsh = consts::one_half<T> *std::sqrt(mRows + mCols + consts::one<T>) * W[0] * eps;
#endif // TL_HAVE_OPENBLAS

}


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::solve(const Vector<T, Rows>& B) -> Vector<T, Cols>
{
    Vector<T, Cols> C(mCols);

    T s;
    Vector<T, Cols> tmp(mCols);

    for (size_t j = 0; j < mCols; j++) {
        s = consts::zero<T>;
        if (W[j] > tsh) {
            for (size_t i = 0; i < mRows; i++)
                s += U[i][j] * B[i];
            s /= W[j];
        }
        tmp[j] = s;
    }

    for (size_t j = 0; j < mCols; j++) {
        s = consts::zero<T>;
        for (size_t k = 0; k < mCols; k++)
            s += V[j][k] * tmp[k];
        C[j] = s;
    }

    return C;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
void SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::decompose()
{
    this->U = this->A;
    Vector<T, Cols> rv1(mCols);
    int i;
    int j;
    int k;
    int l;
    T anorm;
    T f;
    T g;
    T h;
    T s;
    T scale;
    g = scale = anorm = consts::zero<T>; //Householder reduction to bidiagonal form.

    //int one = 1;

    for (i = 0; i < static_cast<int>(mCols); i++) {
        l = i + 2;
        rv1[i] = scale * g;
        g = s = scale = 0.0;

        if (i < static_cast<int>(mRows)) {
            for (k = i; k < static_cast<int>(mRows); k++) scale += std::abs(U[k][i]);
            if (scale != 0.) {
                for (k = i; k < static_cast<int>(mRows); k++) {
                    U[k][i] /= scale;
                    s += U[k][i] * U[k][i];
                }
                f = U[i][i];
                g = -std::copysign(sqrt(s), f);
                h = f * g - s;
                U[i][i] = f - g;
                for (j = l - consts::one<int>; j < static_cast<int>(mCols); j++) {
                    for (s = 0.0, k = i; k < static_cast<int>(mRows); k++)
                        s += U[k][i] * U[k][j];
                    f = s / h;
                    for (k = i; k < mRows; k++)
                        U[k][j] += f * U[k][i];
                }
                for (k = i; k < mRows; k++)
                    U[k][i] *= scale;
            }
        }

        W[i] = scale * g;
        g = s = scale = 0.0;
        if (i + consts::one<int> <= mRows && i + consts::one<int> != mCols) {
            for (k = l - consts::one<int>; k < mCols; k++) scale += fabs(U[i][k]);
            if (scale != 0.) {
                for (k = l - consts::one<int>; k < mCols; k++) {
                    U[i][k] /= scale;
                    s += U[i][k] * U[i][k];
                }
                f = U[i][l - consts::one<int>];
                g = -std::copysign(sqrt(s), f);
                h = f * g - s;
                U[i][l - consts::one<int>] = f - g;

                for (k = l - consts::one<int>; k < mCols; k++)
                    rv1[k] = U[i][k] / h;

                for (j = l - consts::one<int>; j < mRows; j++) {
                    for (s = 0.0, k = l - consts::one<int>; k < mCols; k++)
                        s += U[j][k] * U[i][k];
                    for (k = l - consts::one<int>; k < mCols; k++)
                        U[j][k] += s * rv1[k];
                }

                for (k = l - consts::one<int>; k < mCols; k++)
                    U[i][k] *= scale;
            }
        }
        anorm = std::max(anorm, std::abs(W[i]) + std::abs(rv1[i]));
    }

    for (i = static_cast<int>(mCols) - consts::one<int>; i >= 0; i--) { //Accumulation of right-hand transformations.
        if (i < mCols - consts::one<int>) {
            if (g != 0.) {
                for (j = l; j < mCols; j++) //Double division to avoid possible underflow.
                    V[j][i] = (U[i][j] / U[i][l]) / g;
                for (j = l; j < mCols; j++) {
                    for (s = 0.0, k = l; k < mCols; k++)
                        s += U[i][k] * V[k][j];
                    for (k = l; k < mCols; k++)
                        V[k][j] += s * V[k][i];
                }
            }

            for (j = l; j < mCols; j++)
                V[i][j] = V[j][i] = consts::zero<T>;
        }

        V[i][i] = consts::one<T>;
        g = rv1[i];
        l = i;
    }

    for (i = static_cast<int>(std::min(mRows, mCols)) - consts::one<int>; i >= 0; i--) { //Accumulation of left-hand transformations.
        l = i + consts::one<int>;
        g = W[i];
        for (j = l; j < mCols; j++) this->U.at(i, j) = 0.0;
        if (g != consts::zero<T>) {
            g = consts::one<T> / g;
            for (j = l; j < mCols; j++) {
                for (s = consts::zero<T>, k = l; k < mRows; k++)
                    s += U[k][i] * U[k][j];
                f = (s / U[i][i]) * g;
                for (k = i; k < mRows; k++)
                    U[k][j] += f * U[k][i];
            }
            for (j = i; j < mRows; j++)
                U[j][i] *= g;
        } else {
            for (j = i; j < mRows; j++)
                U[j][i] = consts::zero<T>;
        }
        ++U[i][i];
    }

    bool flag;
    int its;
    size_t nm;
    T c;
    T x;
    T y;
    T z;
    for (k = static_cast<int>(mCols) - consts::one<int>; k >= 0; k--) { //Diagonalization of the bidiagonal form: Loop over
        for (its = 0; its < mIterationMax; its++) { // singular values, and over allowed iterations.

            flag = true;

            for (l = k; l >= 0; l--) { //Test for splitting.
                nm = static_cast<size_t>(l) - consts::one<int>; //Note that rv1[1] is always zero.
                if (l == 0 || std::abs(rv1[l]) <= eps * anorm) {
                    flag = false;
                    break;
                }
                if (std::abs(W[nm]) <= eps * anorm) break;
            }

            if (flag) {
                c = consts::zero<T>; //Cancellation of rv1[l], if l > 1.
                s = consts::one<T>;
                for (i = l; i < k + consts::one<int>; i++) {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if (std::abs(f) <= eps * anorm) break;
                    g = W[i];
                    h = module(f, g);
                    W[i] = h;
                    h = consts::one<T> / h;
                    c = g * h;
                    s = -f * h;
                    for (j = 0; j < mRows; j++) {
                        y = U[j][nm];
                        z = U[j][i];
                        U[j][nm] = y * c + z * s;
                        U[j][i] = z * c - y * s;
                    }
                }
            }
            z = W[k];
            if (l == k) { //Convergence.
                if (z < consts::zero<T>) { //Singular value is made nonnegative.
                    W[k] = -z;
                    for (j = 0; j < mCols; j++)
                        V[j][k] = -V[j][k];
                }
                break;
            }

            if (its == 29) throw std::runtime_error("no convergence in 30 iterations");
            x = W[l]; //Shift from bottom 2-by-2 minor.
            nm = static_cast<size_t>(k - consts::one<int>);
            y = W[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (static_cast<T>(2) * h * y);
            g = module(f, consts::one<T>);
            f = ((x - z) * (x + z) + h * ((y / (f + std::copysign(g, f))) - h)) / x;
            c = s = consts::one<T>; //Next QR transformation:

            for (j = l; j <= nm; j++) {
                i = j + consts::one<int>;
                g = rv1[i];
                y = W[i];
                h = s * g;
                g = c * g;
                z = module(f, h);
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;
                for (size_t jj = 0; jj < mCols; jj++) {
                    x = V[jj][j];
                    z = V[jj][i];
                    V[jj][j] = x * c + z * s;
                    V[jj][i] = z * c - x * s;
                }
                z = module(f, h);
                W[j] = z; //Rotation can be arbitrary if z = 0.
                if (z != 0.) {
                    z = consts::one<T> / z;
                    c = f * z;
                    s = h * z;
                }
                f = c * g + s * y;
                x = c * y - s * g;
                for (size_t jj = 0; jj < mRows; jj++) {
                    y = U[jj][j];
                    z = U[jj][i];
                    U[jj][j] = y * c + z * s;
                    U[jj][i] = z * c - y * s;
                }
            }

            rv1[l] = consts::zero<T>;
            rv1[k] = f;
            W[k] = x;
        }
    }
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
void SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::reorder()
{
    int i;
    int j;
    size_t k;

    int inc = 1;
    T sw;
    Vector<T, Rows> su(mRows);
    Vector<T, Cols> sv(mCols);

    do { inc *= 3; inc++; } while (inc <= mCols);

    do {
        inc /= 3;
        for (i = inc; i < mCols; i++) {
            sw = W[i];
            for (k = 0; k < mRows; k++) su[k] = U.at(k, i);
            for (k = 0; k < mCols; k++) sv[k] = V.at(k, i);
            j = i;
            while (W[j - inc] < sw) {
                W[j] = W[j - inc];
                for (k = 0; k < mRows; k++) U.at(k, j) = U.at(k, j - inc);
                for (k = 0; k < mCols; k++) V.at(k, j) = V.at(k, j - inc);
                j -= inc;
                if (j < inc) break;
            }
            W[j] = sw;
            for (k = 0; k < mRows; k++) U.at(k, j) = su[k];
            for (k = 0; k < mCols; k++) V.at(k, j) = sv[k];

        }
    } while (inc > 1);

    size_t s;
    for (size_t c = 0; c < mCols; c++) {
        s = 0;

        for (size_t r = 0; r < mRows; r++)
            if (U[r][c] < consts::zero<T>)
                s++;

        for (size_t r = 0; r < mCols; r++)
            if (V[r][c] < consts::zero<T>)
                s++;

        if (s > (mRows + mCols) / 2) {
            U.col(c) = -U.col(c);
            V.col(c) = -V.col(c);
        }
    }
}

#ifdef TL_HAVE_OPENBLAS

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
inline void SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::lapackDecompose()
{
    lapack_int info;
    lapack_int lda = static_cast<int>(mCols);
    lapack_int ldu = static_cast<int>(mRows);
    lapack_int ldvt = static_cast<int>(mCols);
    T *superb = new T[std::min(mRows, mCols) - 1];

    info = lapack::gesvd(static_cast<int>(mRows), static_cast<int>(mCols), A.data(), lda, W.data(), U.data(), ldu, V.data(), ldvt, superb);
    V = V.transpose();

    delete[] superb;

    TL_ASSERT(info >= 0, "The algorithm computing SVD failed to converge.");
}

#endif // TL_HAVE_OPENBLAS

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::u() const -> Matrix<T, Rows, Cols>
{
    return U;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::v() const -> Matrix<T, Cols, Cols>
{
    return V;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::w() const -> Vector<T, Cols>
{
    return W;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::maxIterations() const -> int
{
    return mIterationMax;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
void SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::setMaxIterations(int maxIterations)
{
    mIterationMax = maxIterations;
}


/*! \} */ 


} // End namespace tl
