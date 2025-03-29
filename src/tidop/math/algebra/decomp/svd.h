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

#include "tidop/math/algebra/vector.h"
#include "tidop/math/base/lapack.h"
#include "tidop/core/base/flags.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */


 /// \cond

template<typename T>
class SingularValueDecomposition;

/// \endcond



/*!
 * \enum ConfigSVD
 * \brief Configuration flags for the Singular Value Decomposition (SVD).
 *
 * This enumeration defines options for controlling the computation of
 * the U and V matrices in the Singular Value Decomposition (SVD).
 *
 * The flags can be combined using bitwise operations to customize the
 * behavior of the decomposition.
 */
enum class ConfigSVD : int8_t
{
    /*!
     * \brief Compute the full U matrix.
     *
     * When set, the decomposition will compute the full \( m \times m \)
     * orthogonal matrix \( U \). If not set, a reduced version may be computed
     * depending on other flags.
     */
    full_u = 1 << 0,

    /*!
     * \brief Compute the full V matrix.
     *
     * When set, the decomposition will compute the full \( n \times n \)
     * orthogonal matrix \( V \). If not set, a reduced version may be computed
     * depending on other flags.
     */
    full_v = 1 << 1,

    /*!
     * \brief Disable computation of the U matrix.
     *
     * When set, the U matrix will not be computed. This flag overrides
     * both `full_u` and any other option related to U.
     */
    no_u = 1 << 2,

    /*!
     * \brief Disable computation of the V matrix.
     *
     * When set, the V matrix will not be computed. This flag overrides
     * both `full_v` and any other option related to V.
     */
    no_v = 1 << 3
};
ALLOW_BITWISE_FLAG_OPERATIONS(ConfigSVD)

constexpr unsigned int max_svg_iterations = 30;



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
 * - \( U \) is an orthogonal matrix of size \( m \times m \) (or a reduced version if configured),
 * - \( \Sigma \) is a diagonal matrix of size \( m \times n \),
 * - \( V \) is an orthogonal matrix of size \( n \times n \) (or a reduced version if configured).
 *
 * The SVD provides a powerful tool for analyzing and solving linear systems, especially in cases
 * where the matrix is non-square or ill-conditioned.
 *
 * This class supports computing the full or reduced versions of \( U \) and \( V \), as well as disabling
 * their computation entirely through the \ref ConfigSVD configuration flags.
 *
 * \note
 * If **LAPACK** is available (`TL_HAVE_OPENBLAS` is defined), the decomposition is performed using LAPACK's
 * optimized routines for higher efficiency and numerical stability.
 * Otherwise, the decomposition is computed using the **Golub-Reinsch algorithm**, which follows
 * an iterative bidiagonalization and QR algorithm approach.
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

private:

    Matrix_t<T, Rows, Cols> A;
    Matrix_t<T, DynamicData, DynamicData> U;
    Matrix_t<T, DynamicData, DynamicData> V;
    Vector<T, Cols> W;
    EnumFlags<ConfigSVD> mConfig;
    size_t mRows;
    size_t mCols;

public:

    /*!
     * \brief Constructs the Singular Value Decomposition (SVD) of a matrix
     *
     * This constructor performs the SVD of the matrix \( A \), storing the resulting orthogonal matrices
     * \( U \) and \( V \), as well as the singular values in the vector \( W \).
     *
     * The computation of \( U \) and \( V \) can be controlled through the \ref ConfigSVD parameter.
     * By default, both \( U \) and \( V \) are computed in reduced form.
     *
     * \param[in] a The matrix \( A \) to decompose.
     * \param[in] config Configuration flags to control computation of \( U \) and \( V \).
     */
    SingularValueDecomposition(const Matrix_t<T, Rows, Cols> &a, ConfigSVD config = static_cast<ConfigSVD>(0));

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
    /*!
     * \brief Solves the system of equations \( A \cdot x = b \) using the SVD
     *
     * Using the SVD, this method solves the system of linear equations \( A \cdot x = b \), where \( A \)
     * is the matrix and \( b \) is the right-hand side vector. This is done by using the properties of
     * the matrices \( U \), \( \Sigma \), and \( V \) from the SVD.
     *
     * \throws Exception if \( U \) was not computed (ConfigSVD::no_u was enabled).
     * \throws Exception if \( V \) was not computed (ConfigSVD::no_v was enabled).
     *
     * \param[in] B The right-hand side vector \( b \).
     * \return The solution vector \( x \).
     */
    auto solve(const Vector<T, Rows> &b) -> Vector<T, Cols>;

    /*!
     * \brief Gets the orthogonal matrix \( U \), if computed
     *
     * This method returns the orthogonal matrix \( U \) from the SVD decomposition.
     * If \( U \) was disabled in the configuration, an exception is thrown.
     *
     * \throws Exception if \( U \) was not computed (ConfigSVD::no_u was enabled).
     *
     * \return The matrix \( U \), which is orthogonal.
     */
    auto u() const -> Matrix_t<T, DynamicData, DynamicData>;

    /*!
     * \brief Gets the orthogonal matrix \( V \), if computed
     *
     * This method returns the orthogonal matrix \( V \) from the SVD decomposition.
     * If \( V \) was disabled in the configuration, an exception is thrown.
     *
     * \throws Exception if \( V \) was not computed (ConfigSVD::no_v was enabled).
     *
     * \return The matrix \( V \), which is orthogonal.
     */
    auto v() const -> Matrix_t<T, DynamicData, DynamicData>;

    /*!
     * \brief Gets the singular values as a vector \( W \)
     *
     * This method returns the singular values of the matrix as a vector \( W \).
     *
     * \return The vector of singular values \( W \).
     */
    auto w() const -> Vector<T, Cols>;

private:

    void decompose();
    void reorder();
#ifdef TL_HAVE_OPENBLAS
    void lapackDecompose();
#endif // TL_HAVE_OPENBLAS

};



template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::SingularValueDecomposition(const Matrix_t<T, Rows, Cols> &a,
                                                                                ConfigSVD config)
  : A(a),
    mConfig(config),
    mRows(a.rows()),
    mCols(a.cols())
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    U = Matrix_t<T, DynamicData, DynamicData>(mRows, mConfig.isEnabled(ConfigSVD::full_u) ? mRows : std::min(mRows, mCols));
    V = Matrix_t<T, DynamicData, DynamicData>(mConfig.isEnabled(ConfigSVD::full_v) ? mCols : std::min(mRows, mCols), mCols);
    W = Vector<T, Cols>(mCols);

#ifdef TL_HAVE_OPENBLAS
    this->lapackDecompose();
#else
    this->decompose();
    this->reorder();
#endif // TL_HAVE_OPENBLAS
}


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::solve(const Vector<T, Rows> &B) -> Vector<T, Cols>
{
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_u), "Attempted to solve the system, but matrix U was not computed. Disable ConfigSVD::no_u in the constructor.");
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_v), "Attempted to solve the system, but matrix V was not computed. Disable ConfigSVD::no_v in the constructor.");

    Vector<T, Cols> C(mCols);
    T tsh = consts::one_half<T> *std::sqrt(mRows + mCols + consts::one<T>) * W[0] * std::numeric_limits<T>::epsilon();
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
    if (A.cols() > U.cols())
        U = A.block(0, A.rows() - 1, 0, U.cols() - 1);
    else
        U.block(0, A.rows() - 1, 0, A.cols() - 1) = A;

    T tol = static_cast<T>(1e-10);

    Vector<T, Cols> e(mCols, 0);

    T g = consts::zero<T>;
    T x = consts::zero<T>;
    T scale = consts::zero<T>;
    bool with_u = mConfig.isDisabled(ConfigSVD::no_u);
    bool with_v = mConfig.isDisabled(ConfigSVD::no_v);

    // Householder's reduction to bidiagonal form

    for (size_t i = 0; i < mCols; ++i) {

        e[i] = scale * g;
        T s = consts::zero<T>;
        scale = consts::zero<T>;
        size_t l = i + 1;

        if (i < mRows) {

            for (size_t k = i; k < mRows; k++)
                scale += std::abs(U[k][i]);

            if (scale != 0.) {

                for (size_t j = i; j < mRows; ++j) {
                    U[j][i] /= scale;
                    s += U[j][i] * U[j][i];
                }

                if (s < tol) {
                    g = consts::zero<T>;
                } else {

                    T f = U[i][i];

                    g = -std::copysign(sqrt(s), f);
                    T h = f * g - s;
                    U[i][i] = f - g;

                    for (size_t j = l; j < U.cols(); ++j) {

                        s = consts::zero<T>;

                        for (size_t k = i; k < U.rows(); ++k)
                            s += U[k][i] * U[k][j];

                        T f = s / h;

                        for (size_t k = i; k < U.rows(); ++k)
                            U[k][j] += f * U[k][i];
                    }

                    for (size_t k = i; k < mRows; k++)
                        U[k][i] *= scale;
                }
            }
        }

        W[i] = scale * g;
        s = scale = 0.0;

        if (i < mRows && i + consts::one<int> != mCols) {

            for (size_t k = l; k < mCols; k++)
                scale += fabs(U[i][k]);

            if (scale != 0.) {

                for (size_t j = l; j < mCols; ++j) {
                    U[i][j] /= scale;
                    s += U[i][j] * U[i][j];
                }

                if (s < tol)
                    g = 0.0;
                else {

                    T f = U[i][i + 1];
                    g = -std::copysign(sqrt(s), f);
                    T h = f * g - s;
                    U[i][i + 1] = f - g;

                    for (size_t j = l; j < mCols; ++j)
                        e[j] = U[i][j] / h;

                    for (size_t j = l; j < mRows; ++j) {

                        s = 0.0;

                        for (size_t k = l; k < mCols; ++k)
                            s += U[j][k] * U[i][k];

                        for (size_t k = l; k < mCols; ++k)
                            U[j][k] += s * e[k];
                    }

                    for (size_t k = l; k < mCols; k++)
                        U[i][k] *= scale;
                }
            }
        }

        x = std::max(x, abs(W[i]) + abs(e[i]));

    }

    // Acumulación de las transformaciones de la derecha
    if (with_v) {

        int l = -1;
        for (int i = mCols - 1; i >= 0; --i) {

            if (l != -1) {

                if (g != 0.0) {

                    T h = U[i][l] * g;

                    for (size_t j = l; j < mCols; ++j)
                        V[j][i] = U[i][j] / h;

                    for (size_t j = l; j < mCols; ++j) {

                        T s = consts::zero<T>;

                        for (size_t k = l; k < mCols; ++k)
                            s += U[i][k] * V[k][j];

                        for (size_t k = l; k < mCols; ++k)
                            V[k][j] += s * V[k][i];
                    }
                }

                for (size_t j = l; j < mCols; ++j)
                    V[i][j] = V[j][i] = consts::zero<T>;
            }

            V[i][i] = consts::one<T>;
            g = e[i];
            l = i;
        }
    }

    if (with_u) {

        if (mConfig.isEnabled(ConfigSVD::full_u)){
            for (size_t i = mCols; i < mRows; ++i) {
                for (size_t j = mCols; j < mRows; ++j) {
                    U[i][j] = (i == j) ? consts::one<T> : consts::zero<T>;
                }
            }
        }

        for (int i = std::min(mRows, mCols) - 1; i >= 0; --i) {

            size_t l = i + 1;
            g = W[i];

            for (size_t j = l; j < U.cols(); ++j)
                U[i][j] = 0.0;

            if (g != 0.0) {

                g = consts::one<T> / g;
                T h = (consts::one<T> / U[i][i]) * g;

                //Para matrices completas -> mRows. En ese caso U.cols() == A.rows()
                for (size_t j = l; j < U.cols(); ++j) {

                    T s = consts::zero<T>;

                    for (size_t k = l; k < mRows; ++k)
                        s += U[k][i] * U[k][j];

                    T f = s * h;

                    for (size_t k = i; k < mRows; ++k)
                        U[k][j] += f * U[k][i];

                }

                for (size_t j = i; j < mRows; ++j)
                    U[j][i] *= g;

            } else {
                for (size_t j = i; j < mRows; ++j)
                    U[j][i] = consts::zero<T>;
            }
            ++U[i][i];
        }
    }

    T convergenge = std::numeric_limits<T>::epsilon() * x;

    // Diagonalización de la forma bidiagonal

    for (int k = static_cast<int>(mCols) - 1; k >= 0; --k) {

        for (size_t its = 0; its < max_svg_iterations; its++) {

            int l;

            //Test for splitting.
            for (l = k; l >= 0; --l) {

                if (l == 0 || std::abs(e[l]) <= convergenge) break; // -> Test for convergence

                if (std::abs(W[l - 1]) <= convergenge) {
                    // Cancellation of e[l] if l > 1
                    T c = consts::zero<T>;
                    T s = consts::one<T>;
                    int l1 = l - 1;
                    for (int i = l; i <= k; ++i) {
                        T f = s * e[i];
                        e[i] = c * e[i];
                        if (std::abs(f) <= convergenge) break;
                        T g = W[i];
                        W[i] = module(f, g);
                        T h = consts::one<T> / W[i];
                        c = g * h;
                        s = -f * h;

                        if (with_u) {
                            for (size_t j = 0; j < mRows; ++j) {
                                T y = U[j][l1];
                                T z = U[j][i];
                                U[j][l1] = y * c + z * s;
                                U[j][i] = -y * s + z * c;
                            }
                        }
                    }
                    break;
                }
            }

            // Test for convergence
            T z = W[k];
            if (l == k) {
                if (z < consts::zero<T>) {  //Singular value is made nonnegative.
                    W[k] = -z;
                    if (with_v) {
                        for (int j = 0; j < mCols; ++j) {
                            V[j][k] = -V[j][k];
                        }
                    }
                }
                continue;
            }

            TL_ASSERT(its < max_svg_iterations, "SVD no convergence in {} iterations", max_svg_iterations);

            // Shift from bottom 2x2 minor
            T x = W[l], y = W[k - 1];
            T g = e[k - 1], h = e[k];
            T f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2 * h * y);
            g = module(f, consts::one<T>);
            f = ((x - z) * (x + z) + h * ((y / (f + std::copysign(g, f))) - h)) / x;

            // Next QR transformation
            T c = 1, s = 1;
            for (int i = l + 1; i <= k; ++i) {
                g = e[i];
                y = W[i];
                h = s * g;
                g = c * g;
                e[i - 1] = z = module(f, h);
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = -x * s + g * c;
                h = y * s;
                y *= c;

                if (with_v) {
                    for (size_t j = 0; j < mCols; ++j) {
                        x = V[j][i - 1];
                        z = V[j][i];
                        V[j][i - 1] = x * c + z * s;
                        V[j][i] = -x * s + z * c;
                    }
                }

                W[i - 1] = module(f, h);
                //Rotation can be arbitrary if W[i - 1] = 0.
                if (W[i - 1] != 0.) {
                    z = consts::one<T> / W[i - 1];
                    c = f * z;
                    s = h * z;
                }
                f = c * g + s * y;
                x = -s * g + c * y;

                if (with_u) {
                    for (size_t j = 0; j < mRows; ++j) {
                        y = U[j][i - 1];
                        z = U[j][i];
                        U[j][i - 1] = y * c + z * s;
                        U[j][i] = -y * s + z * c;
                    }
                }
            }

            e[l] = consts::zero<T>;
            e[k] = f;
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
    size_t inc = 1;

    bool with_u = mConfig.isDisabled(ConfigSVD::no_u);
    bool with_v = mConfig.isDisabled(ConfigSVD::no_v);

    do { inc *= 3; inc++; } while (inc <= mCols);

    do {
        inc /= 3;
        for (size_t i = inc; i < mCols; i++) {
            T sw = W[i];
            size_t j = i;
            while (W[j - inc] < sw) {
                std::swap(W[j], W[j - inc]);
                if (with_u) U.swapCols(j, j - inc);
                if (with_v) V.swapCols(j, j - inc);
                j -= inc;
                if (j < inc) break;
            }

        }
    } while (inc > 1);

    size_t s;
    for (size_t c = 0; c < mCols; c++) {
        s = 0;

        if (with_u) {
            for (size_t r = 0; r < mRows; r++)
                if (U[r][c] < consts::zero<T>)
                    s++;
        }

        if (with_v) {
            for (size_t r = 0; r < mCols; r++)
                if (V[r][c] < consts::zero<T>)
                    s++;
        }

        if (s > (mRows + mCols) / 2) {
            if (with_u) U.col(c) = -U.col(c);
            if (with_v) V.col(c) = -V.col(c);
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
    auto lda = mCols;
    auto ldu = mConfig.isEnabled(ConfigSVD::full_u) ? mRows : std::min(mRows, mCols);
    auto ldvt = mConfig.isEnabled(ConfigSVD::full_v) ? mCols : std::min(mRows, mCols);
    T *superb = new T[std::min(mRows, mCols) - 1];

    lapack::SVDMode jobu = lapack::SVDMode::none;
    if (mConfig.isDisabled(ConfigSVD::no_u)) {
        jobu = mConfig.isEnabled(ConfigSVD::full_u) ? lapack::SVDMode::full : lapack::SVDMode::reduced;
    }

    lapack::SVDMode jobvt = lapack::SVDMode::none;
    if (mConfig.isDisabled(ConfigSVD::no_v)) {
        jobvt = mConfig.isEnabled(ConfigSVD::full_v) ? lapack::SVDMode::full : lapack::SVDMode::reduced;
    }

    auto info = lapack::gesvd(lapack::Order::row_major,
                              jobu, jobvt,
                              mRows, mCols,
                              A.data(), lda, W.data(), U.data(),
                              ldu, V.data(), ldvt, superb);

    if (mConfig.isDisabled(ConfigSVD::no_v))
        V = V.transpose();

    delete[] superb;

    TL_ASSERT(info >= 0, "The algorithm computing SVD failed to converge.");
}

#endif // TL_HAVE_OPENBLAS

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::u() const -> Matrix_t<T, DynamicData, DynamicData>
{
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_u), 
              "Attempted to access matrix U, but it was not computed. Disable ConfigSVD::no_u in the constructor.");

    return U;
}

template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
auto SingularValueDecomposition<Matrix_t<T, Rows, Cols>>::v() const -> Matrix_t<T, DynamicData, DynamicData>
{
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_v), "Attempted to access matrix V, but it was not computed. Disable ConfigSVD::no_v in the constructor.");

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


/*! \} */


} // End namespace tl
