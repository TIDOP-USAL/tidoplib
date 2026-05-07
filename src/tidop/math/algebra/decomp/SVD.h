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

/*! \file SVD.h
 * \brief Singular Value Decomposition (SVD) of a matrix.
 *
 * This file defines the `SingularValueDecomposition` class, which factorizes a matrix
 * \( A \) into \( A = U \Sigma V^T \), where \( U \) and \( V \) are orthogonal matrices
 * and \( \Sigma \) is a diagonal matrix containing the singular values. The SVD is a
 * fundamental tool for solving linear systems (including least squares), matrix
 * approximation (compression), principal component analysis, and many other applications.
 *
 * The implementation can use LAPACK (OpenBLAS) when available, otherwise a built‑in
 * Golub‑Reinsch algorithm is used.
 *
 * \ingroup Decomposition
 * \see tl::QRDecomposition, tl::LuDecomposition, tl::CholeskyDecomposition
 */

#pragma once


#include <algorithm>

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/base/Lapack.h"
#include "tidop/core/base/flags.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */

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
 * \class SingularValueDecomposition
 * \brief Singular Value Decomposition of a matrix.
 *
 * \tparam Mat A type satisfying the `DenseMatrix` concept (e.g., `Matrix<T, Rows, Cols>`).
 *             The element type must be floating‑point (float, double).
 *
 * For an \( m \times n \) matrix \( A \), the decomposition is:
 * \f[
 * A = U \Sigma V^T
 * \f]
 * where:
 * - \( U \) is \( m \times m \) (or reduced size),
 * - \( \Sigma \) is an \( m \times n \) diagonal matrix with the singular values on the diagonal,
 * - \( V \) is \( n \times n \) (or reduced size).
 *
 * The singular values are stored in descending order. The class allows fine‑grained control
 * over the computation of \( U \) and \( V \) via the `ConfigSVD` flags.
 *
 * ### Example
 * \code
 * Matrix<double, 4, 2> A = {{1,2},{3,4},{5,6},{7,8}};
 * SingularValueDecomposition svd(A, ConfigSVD::full_u | ConfigSVD::full_v);
 * auto U = svd.u();     // 4×4 orthogonal
 * auto S = svd.w();     // singular values vector
 * auto V = svd.v();     // 2×2 orthogonal
 * Vector<double, 4> b = {1,1,1,1};
 * auto x = svd.solve(b); // least‑squares solution
 * \endcode
 */
template<DenseMatrix Mat>
class SingularValueDecomposition
{
public:

    using value_type = typename matrix_traits<Mat>::value_type;
    static constexpr size_t rows = matrix_traits<Mat>::rows;
    static constexpr size_t cols = matrix_traits<Mat>::cols;

private:

    Matrix<value_type, rows, cols> A;                               /*!< Original matrix (modified during decomposition). */
    Matrix<value_type, DynamicData, DynamicData> U;                 /*!< Left orthogonal matrix (size depends on flags). */
    Matrix<value_type, DynamicData, DynamicData> V;                 /*!< Right orthogonal matrix (size depends on flags). */
    Vector<value_type, cols> W;                                     /*!< Singular values (descending order). */
    EnumFlags<ConfigSVD> mConfig;                                   /*!< Configuration flags. */
    size_t mRows;                                                   /*!< Number of rows. */
    size_t mCols;                                                   /*!< Number of columns. */

public:

    /*!
     * \brief Constructs the SVD decomposition.
     * \param[in] a      The matrix to decompose.
     * \param[in] config Configuration flags (default: compute reduced U and V).
     *
     * The decomposition is performed immediately. If LAPACK is available, it uses
     * `gesvd`; otherwise it uses a built‑in Golub‑Reinsch algorithm.
     */
    SingularValueDecomposition(const Mat &a, ConfigSVD config = static_cast<ConfigSVD>(0));

    /*!
     * \brief Solves the linear system \( A x = b \) (or least squares).
     * \tparam Vec A type satisfying `VectorExpr`.
     * \param[in] B The right‑hand side vector.
     * \return The solution vector \( x \) of length `cols()`.
     * \pre `U` and `V` must have been computed (i.e., `ConfigSVD::no_u` and `ConfigSVD::no_v` must not be set).
     * \throws `tl::Exception` if `U` or `V` are missing, or if the solve fails.
     *
     * For overdetermined systems (rows > cols) this gives the least‑squares solution.
     * For underdetermined systems it returns the minimum‑norm solution (since the SVD
     * inverts only the non‑zero singular values).
     */
    template<VectorExpr Vec>
    auto solve(const Vec &b) -> Vector<value_type, cols>;

    /*!
     * \brief Returns the left orthogonal matrix \( U \) (if computed).
     * \return The matrix \( U \) (size depends on configuration).
     * \throws `tl::Exception` if `ConfigSVD::no_u` was set in the constructor.
     */
    auto u() const -> Matrix<value_type, DynamicData, DynamicData>;

    /*!
     * \brief Returns the right orthogonal matrix \( V \) (if computed).
     * \return The matrix \( V \) (size depends on configuration).
     * \throws `tl::Exception` if `ConfigSVD::no_v` was set in the constructor.
     */
    auto v() const -> Matrix<value_type, DynamicData, DynamicData>;

    /*!
     * \brief Returns the singular values.
     * \return A vector containing the singular values in descending order.
     */
    auto w() const -> Vector<value_type, cols>;

private:

    void decompose();
    void reorder();
#ifdef TL_HAVE_OPENBLAS
    void lapackDecompose();
#endif // TL_HAVE_OPENBLAS

};



template<DenseMatrix Mat>
SingularValueDecomposition<Mat>::SingularValueDecomposition(const Mat &a,
                                                            ConfigSVD config)
  : A(a),
    mConfig(config),
    mRows(a.rows()),
    mCols(a.cols())
{
    static_assert(std::is_floating_point<value_type>::value, "Integral type not supported");

    U = Matrix<value_type, DynamicData, DynamicData>(mRows, mConfig.isEnabled(ConfigSVD::full_u) ? mRows : std::min(mRows, mCols));
    V = Matrix<value_type, DynamicData, DynamicData>(mConfig.isEnabled(ConfigSVD::full_v) ? mCols : std::min(mRows, mCols), mCols);
    W = Vector<value_type, cols>(mCols);

#ifdef TL_HAVE_OPENBLAS
    this->lapackDecompose();
#else
    this->decompose();
    this->reorder();
#endif // TL_HAVE_OPENBLAS
}


template<DenseMatrix Mat>
template<VectorExpr Vec>
auto SingularValueDecomposition<Mat>::solve(const Vec &B) -> Vector<value_type, cols>
{
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_u), "Attempted to solve the system, but matrix U was not computed. Disable ConfigSVD::no_u in the constructor.");
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_v), "Attempted to solve the system, but matrix V was not computed. Disable ConfigSVD::no_v in the constructor.");

    Vector<value_type, cols> C(mCols);
    value_type tsh = consts::one_half<value_type> *std::sqrt(mRows + mCols + consts::one<value_type>) * W[0] * std::numeric_limits<value_type>::epsilon();
    value_type s;
    Vector<value_type, cols> tmp(mCols);

    for (size_t j = 0; j < mCols; j++) {
        s = consts::zero<value_type>;
        if (W[j] > tsh) {
            for (size_t i = 0; i < mRows; i++)
                s += U[i][j] * B[i];
            s /= W[j];
        }
        tmp[j] = s;
    }

    for (size_t j = 0; j < mCols; j++) {
        s = consts::zero<value_type>;
        for (size_t k = 0; k < mCols; k++)
            s += V[j][k] * tmp[k];
        C[j] = s;
    }

    return C;
}


template<DenseMatrix Mat>
void SingularValueDecomposition<Mat>::decompose()
{
    try {
        if (A.cols() > U.cols())
            U = A.block(0, 0, A.rows(), U.cols());
        else
            U.block(0, 0, A.rows(), A.cols()) = A;

        value_type tol = static_cast<value_type>(1e-10);

        Vector<value_type, cols> e(mCols, 0);

        value_type g = consts::zero<value_type>;
        value_type x = consts::zero<value_type>;
        value_type scale = consts::zero<value_type>;
        bool with_u = mConfig.isDisabled(ConfigSVD::no_u);
        bool with_v = mConfig.isDisabled(ConfigSVD::no_v);

        // Householder's reduction to bidiagonal form

        for (size_t i = 0; i < mCols; ++i) {

            e[i] = scale * g;
            value_type s = consts::zero<value_type>;
            scale = consts::zero<value_type>;
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
                        g = consts::zero<value_type>;
                    } else {

                        value_type f = U[i][i];

                        g = -std::copysign(sqrt(s), f);
                        value_type h = f * g - s;
                        U[i][i] = f - g;

                        for (size_t j = l; j < U.cols(); ++j) {

                            s = consts::zero<value_type>;

                            for (size_t k = i; k < U.rows(); ++k)
                                s += U[k][i] * U[k][j];

                            value_type f = s / h;

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

                        value_type f = U[i][i + 1];
                        g = -std::copysign(sqrt(s), f);
                        value_type h = f * g - s;
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
            for (int i = static_cast<int>(mCols) - 1; i >= 0; --i) {

                if (l != -1) {

                    if (g != 0.0) {

                        value_type h = U[i][l] * g;

                        for (size_t j = l; j < mCols; ++j)
                            V[j][i] = U[i][j] / h;

                        for (size_t j = l; j < mCols; ++j) {

                            value_type s = consts::zero<value_type>;

                            for (size_t k = l; k < mCols; ++k)
                                s += U[i][k] * V[k][j];

                            for (size_t k = l; k < mCols; ++k)
                                V[k][j] += s * V[k][i];
                        }
                    }

                    for (size_t j = l; j < mCols; ++j)
                        V[i][j] = V[j][i] = consts::zero<value_type>;
                }

                V[i][i] = consts::one<value_type>;
                g = e[i];
                l = i;
            }
        }

        if (with_u) {

            if (mConfig.isEnabled(ConfigSVD::full_u)) {
                for (size_t i = mCols; i < mRows; ++i) {
                    for (size_t j = mCols; j < mRows; ++j) {
                        U[i][j] = (i == j) ? consts::one<value_type> : consts::zero<value_type>;
                    }
                }
            }

            for (int i = static_cast<int>(std::min(mRows, mCols)) - 1; i >= 0; --i) {

                size_t l = i + 1;
                g = W[i];

                for (size_t j = l; j < U.cols(); ++j)
                    U[i][j] = 0.0;

                if (g != 0.0) {

                    g = consts::one<value_type> / g;
                    value_type h = (consts::one<value_type> / U[i][i]) * g;

                    //Para matrices completas -> mRows. En ese caso U.cols() == A.rows()
                    for (size_t j = l; j < U.cols(); ++j) {

                        value_type s = consts::zero<value_type>;

                        for (size_t k = l; k < mRows; ++k)
                            s += U[k][i] * U[k][j];

                        value_type f = s * h;

                        for (size_t k = i; k < mRows; ++k)
                            U[k][j] += f * U[k][i];

                    }

                    for (size_t j = i; j < mRows; ++j)
                        U[j][i] *= g;

                } else {
                    for (size_t j = i; j < mRows; ++j)
                        U[j][i] = consts::zero<value_type>;
                }
                ++U[i][i];
            }
        }

        value_type convergenge = std::numeric_limits<value_type>::epsilon() * x;

        // Diagonalización de la forma bidiagonal

        for (int k = static_cast<int>(mCols) - 1; k >= 0; --k) {

            for (size_t its = 0; its < max_svg_iterations; its++) {

                int l;

                //Test for splitting.
                for (l = k; l >= 0; --l) {

                    if (l == 0 || std::abs(e[l]) <= convergenge) break; // -> Test for convergence

                    if (std::abs(W[l - 1]) <= convergenge) {
                        // Cancellation of e[l] if l > 1
                        value_type c = consts::zero<value_type>;
                        value_type s = consts::one<value_type>;
                        int l1 = l - 1;
                        for (int i = l; i <= k; ++i) {
                            value_type f = s * e[i];
                            e[i] = c * e[i];
                            if (std::abs(f) <= convergenge) break;
                            value_type g = W[i];
                            W[i] = module(f, g);
                            value_type h = consts::one<value_type> / W[i];
                            c = g * h;
                            s = -f * h;

                            if (with_u) {
                                for (size_t j = 0; j < mRows; ++j) {
                                    value_type y = U[j][l1];
                                    value_type z = U[j][i];
                                    U[j][l1] = y * c + z * s;
                                    U[j][i] = -y * s + z * c;
                                }
                            }
                        }
                        break;
                    }
                }

                // Test for convergence
                value_type z = W[k];
                if (l == k) {
                    if (z < consts::zero<value_type>) {  //Singular value is made nonnegative.
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
                value_type x = W[l], y = W[k - 1];
                value_type g = e[k - 1], h = e[k];
                value_type f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2 * h * y);
                g = module(f, consts::one<value_type>);
                f = ((x - z) * (x + z) + h * ((y / (f + std::copysign(g, f))) - h)) / x;

                // Next QR transformation
                value_type c = 1, s = 1;
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
                        z = consts::one<value_type> / W[i - 1];
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

                e[l] = consts::zero<value_type>;
                e[k] = f;
                W[k] = x;

            }

        }
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("SVD decompose");
    }
}

template<DenseMatrix Mat>
void SingularValueDecomposition<Mat>::reorder()
{
    size_t inc = 1;

    bool with_u = mConfig.isDisabled(ConfigSVD::no_u);
    bool with_v = mConfig.isDisabled(ConfigSVD::no_v);

    do { inc *= 3; inc++; } while (inc <= mCols);

    do {
        inc /= 3;
        for (size_t i = inc; i < mCols; i++) {
            value_type sw = W[i];
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
                if (U[r][c] < consts::zero<value_type>)
                    s++;
        }

        if (with_v) {
            for (size_t r = 0; r < mCols; r++)
                if (V[r][c] < consts::zero<value_type>)
                    s++;
        }

        if (s > (mRows + mCols) / 2) {
            if (with_u) U.col(c) = -U.col(c);
            if (with_v) V.col(c) = -V.col(c);
        }
    }
}

#ifdef TL_HAVE_OPENBLAS

template<DenseMatrix Mat>
void SingularValueDecomposition<Mat>::lapackDecompose()
{
    try {

        auto lda = mCols;
        auto ldu = mConfig.isEnabled(ConfigSVD::full_u) ? mRows : std::min(mRows, mCols);
        auto ldvt = mConfig.isEnabled(ConfigSVD::full_v) ? mCols : std::min(mRows, mCols);
        std::vector<value_type> superb(std::min(mRows, mCols) - 1);

        lapack::SVDMode jobu = lapack::SVDMode::none;
        if (mConfig.isDisabled(ConfigSVD::no_u)) {
            jobu = mConfig.isEnabled(ConfigSVD::full_u) ? lapack::SVDMode::full : lapack::SVDMode::reduced;
        }

        lapack::SVDMode jobvt = lapack::SVDMode::none;
        if (mConfig.isDisabled(ConfigSVD::no_v)) {
            jobvt = mConfig.isEnabled(ConfigSVD::full_v) ? lapack::SVDMode::full : lapack::SVDMode::reduced;
        }

        lapack::gesvd(lapack::Order::row_major,
                      jobu, jobvt,
                      mRows, mCols,
                      A.data(), lda, W.data(), U.data(),
                      ldu, V.data(), ldvt, superb.data());

        if (mConfig.isDisabled(ConfigSVD::no_v))
            V = V.transpose();

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Lapack SVD decompose");
    }
}

#endif // TL_HAVE_OPENBLAS

template<DenseMatrix Mat>
auto SingularValueDecomposition<Mat>::u() const -> Matrix<value_type, DynamicData, DynamicData>
{
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_u), 
              "Attempted to access matrix U, but it was not computed. Disable ConfigSVD::no_u in the constructor.");

    return U;
}

template<DenseMatrix Mat>
auto SingularValueDecomposition<Mat>::v() const -> Matrix<value_type, DynamicData, DynamicData>
{
    TL_ASSERT(mConfig.isDisabled(ConfigSVD::no_v), "Attempted to access matrix V, but it was not computed. Disable ConfigSVD::no_v in the constructor.");

    return V;
}

template<DenseMatrix Mat>
auto SingularValueDecomposition<Mat>::w() const -> Vector<value_type, cols>
{
    return W;
}


/*! \} */


} // End namespace tl
