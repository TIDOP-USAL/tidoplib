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

/*! \file EigenDecomposition.h
 * \brief Eigenvalue decomposition of a square matrix.
 *
 * This file defines the `EigenDecomposition` class, which computes the eigenvalues
 * and eigenvectors of a square matrix \( A \). The decomposition is \( A = V \Lambda V^{-1} \),
 * where \( V \) is the matrix of eigenvectors and \( \Lambda \) is a diagonal matrix
 * of eigenvalues. For symmetric matrices, the eigenvectors are orthogonal and the
 * decomposition is particularly robust and efficient.
 *
 * The implementation uses different strategies depending on the matrix properties
 * and the availability of LAPACK:
 * - For symmetric matrices: LAPACK's `syev` (if available), otherwise a Jacobi method
 *   for small matrices (< 10×10) or the QR algorithm with Wilkinson shift for larger ones.
 * - For general (non‑symmetric) matrices: LAPACK's `geev` (if available), otherwise
 *   the basic QR algorithm (without shifts), which works for real eigenvalues but may
 *   struggle with complex pairs.
 *
 * \ingroup Decomposition
 * \see tl::QRDecomposition, tl::SVD, tl::LuDecomposition
 */

#pragma once


#include <algorithm>

#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/decomp/QR.h"
#include "tidop/math/base/Lapack.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */

/*!
 * \class EigenDecomposition
 * \brief Eigenvalue decomposition of a square matrix.
 *
 * \tparam Mat A type satisfying the `DenseMatrix` concept (e.g., `Matrix<T, Rows, Cols>`).
 *             The matrix must be square and have a floating‑point element type.
 *
 * For a square matrix \( A \), the decomposition is:
 * \f[
 * A = V \Lambda V^{-1}
 * \f]
 * where:
 * - \( \Lambda \) is a diagonal matrix whose diagonal entries are the eigenvalues,
 * - \( V \) is a matrix whose columns are the eigenvectors (right eigenvectors).
 *
 * The class stores the eigenvalues (real and imaginary parts) and the eigenvectors
 * as a matrix where each column corresponds to an eigenvector (in the same order as
 * the eigenvalues). For symmetric matrices, the eigenvectors are orthonormal and the
 * imaginary parts are zero.
 *
 * ### Example
 * \code
 * Matrix<double, 3, 3> A = {{4, 1, -2}, {1, 3, 0}, {-2, 0, 2}};
 * EigenDecomposition<Matrix<double, 3, 3>> eig(A);
 * auto real = eig.eigenvaluesReal();
 * auto imag = eig.eigenvaluesImag();
 * auto V = eig.eigenvectors();
 * // Verify A * V ≈ V * diag(λ)
 * \endcode
 */
template<DenseMatrix Mat>
class EigenDecomposition
{

public:

    using value_type = typename matrix_traits<Mat>::value_type;
    static constexpr size_t rows = matrix_traits<Mat>::rows;
    static constexpr size_t cols = matrix_traits<Mat>::cols;

private:

    Matrix<value_type, rows, cols> mMatrix;          /*!< Input matrix (copied). */
    Vector<value_type, rows> mEigenvaluesReal;       /*!< Real parts of eigenvalues. */
    Vector<value_type, rows> mEigenvaluesImag;       /*!< Imaginary parts (zero for symmetric). */
    Matrix<value_type, rows, cols> mEigenvectors;    /*!< Matrix of eigenvectors (columnwise). */
    size_t mSize;                                    /*!< Dimension of the matrix (rows×rows). */

public:

    /*!
     * \brief Constructs the eigenvalue decomposition.
     * \param[in] a The square matrix to decompose.
     * \pre `a.rows() == a.cols()`.
     * \pre The element type must be floating‑point.
     * \throws `tl::Exception` if the matrix is not square or if computation fails.
     *
     * The decomposition is performed immediately. The method used depends on
     * whether the matrix is detected as symmetric (via `isSymmetric()`) and on
     * the availability of LAPACK.
     */
    EigenDecomposition(const Mat &a);

    /*!
     * \brief Returns the real parts of the eigenvalues.
     * \return Vector of length `rows` containing the real parts.
     */
    auto eigenvaluesReal() const -> Vector<value_type, rows> { return mEigenvaluesReal; }

    /*!
     * \brief Returns the imaginary parts of the eigenvalues.
     * \return Vector of length `rows` containing the imaginary parts (zero for symmetric matrices).
     */
    auto eigenvaluesImag() const -> Vector<value_type, rows> { return mEigenvaluesImag; }

    /*!
     * \brief Returns the eigenvectors.
     * \return Matrix of size `rows × rows` where each column is an eigenvector,
     *         in the same order as the eigenvalues.
     */
    auto eigenvectors() const -> Matrix<value_type, rows, cols> { return mEigenvectors; }

private:

    /*!
     * \brief Internal dispatcher that calls either `computeSymmetric` or `computeGeneral`.
     */
    void decompose();

    /*!
     * \brief Computes eigenvalues and eigenvectors for a symmetric matrix.
     *
     * Uses LAPACK's `syev` when available (most efficient). Otherwise:
     * - For sizes < 10, uses the Jacobi method (iterative, robust for small matrices).
     * - For sizes ≥ 10, uses the QR algorithm with Wilkinson shift for faster convergence.
     *
     * The results are stored in `mEigenvaluesReal` and `mEigenvectors` (orthonormal).
     */
    void computeSymmetric();

    /*!
     * \brief Computes eigenvalues and eigenvectors for a general (non‑symmetric) matrix.
     *
     * Uses LAPACK's `geev` when available. Otherwise, a basic QR iteration is performed
     * (without explicit handling of complex eigenvalues – only real eigenvalues are supported).
     *
     * The results are stored in `mEigenvaluesReal`, `mEigenvaluesImag` and `mEigenvectors`.
     */
    void computeGeneral();

};


template<DenseMatrix Mat>
EigenDecomposition<Mat>::EigenDecomposition(const Mat &a)
  : mMatrix(a), 
    mSize(a.rows())
{
	static_assert(std::is_floating_point<value_type>::value, "Integral type not supported");

    TL_ASSERT(mMatrix.isSquare(), "Matrix must be square.");

    mEigenvaluesReal = Vector<value_type, cols>::zero(mSize);
    mEigenvaluesImag = Vector<value_type, cols>::zero(mSize);
    mEigenvectors = Matrix<value_type, cols, cols>::zero(mSize, mSize);

    decompose();

}

template<DenseMatrix Mat>
void EigenDecomposition<Mat>::decompose()
{
    try {
        if (mMatrix.isSymmetric()) {
            computeSymmetric();
        } else {
            computeGeneral();
        }
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Eigen decomposition exception");
    }
}


template<DenseMatrix Mat>
void EigenDecomposition<Mat>::computeSymmetric()
{
    try {
#ifdef TL_HAVE_OPENBLAS

        mEigenvectors = mMatrix;

        lapack::syev(lapack::Order::row_major,
                     lapack::EigenVectors::compute,
                     lapack::TriangularForm::upper,
                     mSize, mEigenvectors.data(),
                     mSize, mEigenvaluesReal.data());

#else
        if (mSize < 10) {
            constexpr size_t maxIterations = 100;
            constexpr value_type tolerance = static_cast<value_type>(1e-10);

            Matrix<value_type, rows, cols> A_copy = mMatrix;
            Matrix<value_type, rows, cols> V = Matrix<value_type, rows, cols>::identity(mSize, mSize);

            for (size_t iter = 0; iter < maxIterations; ++iter) {
                // Buscar el mayor elemento fuera de la diagonal
                size_t p = 0, q = 1;
                value_type maxOffDiagonal = 0;
                for (size_t i = 0; i < mSize; ++i) {
                    for (size_t j = i + 1; j < mSize; ++j) {
                        if (std::abs(A_copy(i, j)) > maxOffDiagonal) {
                            maxOffDiagonal = std::abs(A_copy(i, j));
                            p = i;
                            q = j;
                        }
                    }
                }

                // Criterio de convergencia: si el mayor valor fuera de la diagonal es menor que la tolerancia, terminamos
                if (maxOffDiagonal < tolerance) break;

                // Calcular el ángulo de rotación Jacobi
                value_type theta = (A_copy(q, q) - A_copy(p, p)) / (2 * A_copy(p, q));
                value_type t = std::copysign(1.0, theta) / (std::abs(theta) + std::sqrt(theta * theta + 1.0));
                value_type c = 1.0 / std::sqrt(t * t + 1.);
                value_type s = t * c;
                value_type tau = s / (1.0 + c);

                // Aplicar la rotación a A_copy
                for (size_t i = 0; i < mSize; ++i) {
                    if (i != p && i != q) {
                        value_type Ap = A_copy(i, p);
                        value_type Aq = A_copy(i, q);
                        A_copy(i, p) = A_copy(p, i) = Ap * c - Aq * s;
                        A_copy(i, q) = A_copy(q, i) = Ap * s + Aq * c;
                    }
                }

                value_type A_pp = A_copy(p, p);
                value_type A_qq = A_copy(q, q);
                value_type A_pq = A_copy(p, q);

                A_copy(p, p) = A_pp - t * A_pq;
                A_copy(q, q) = A_qq + t * A_pq;
                A_copy(p, q) = A_copy(q, p) = 0.0;

                // Acumular la rotación en la matriz de autovectores
                for (size_t i = 0; i < mSize; ++i) {
                    value_type Vp = V(i, p);
                    value_type Vq = V(i, q);
                    V(i, p) = Vp - s * (Vq + Vp * tau);
                    V(i, q) = Vq + s * (Vp - Vq * tau);
                }
            }

            // Extraer los autovalores y autovectores
            for (size_t i = 0; i < mSize; ++i) {
                mEigenvaluesReal[i] = A_copy(i, i);
            }

            mEigenvaluesImag = Vector<value_type, rows>::zero(mSize); // No hay parte imaginaria en matrices simétricas
            mEigenvectors = V;

            // Ordenar por selección (ascendente) para coincidir con LAPACK/Eigen
            //for (size_t i = 0; i < mSize - 1; ++i) {
            //    size_t k = i;
            //    for (size_t j = i + 1; j < mSize; ++j) {
            //        if (mEigenvaluesReal[j] < mEigenvaluesReal[k]) {
            //            k = j;
            //        }
            //    }
            //    if (k != i) {
            //        // Intercambiar autovalores
            //        std::swap(mEigenvaluesReal[i], mEigenvaluesReal[k]);
            //        // Intercambiar las columnas correspondientes en la matriz de autovectores
            //        for (size_t row = 0; row < mSize; ++row) {
            //            std::swap(mEigenvectors(row, i), mEigenvectors(row, k));
            //        }
            //    }
            //}


        } else {
            constexpr size_t maxIterations = 100;
            constexpr value_type tolerance = static_cast<value_type>(1e-10);

            Matrix<value_type, rows, cols> A_copy = mMatrix;
            mEigenvectors = Matrix<value_type, rows, cols>::identity(mSize, mSize);

            for (size_t iter = 0; iter < maxIterations; ++iter) {
                // Verificamos convergencia: si la matriz es casi diagonal, terminamos
                value_type offDiagNorm = 0;
                for (size_t i = 0; i < mSize; ++i) {
                    for (size_t j = 0; j < mSize; ++j) {
                        if (i != j) offDiagNorm += A_copy(i, j) * A_copy(i, j);
                    }
                }
                if (std::sqrt(offDiagNorm) < tolerance) break;

                // Desplazamiento de Wilkinson para acelerar convergencia
                size_t n = mSize - 1;
                value_type d = (A_copy(n - 1, n - 1) - A_copy(n, n)) / 2.0;
                value_type mu = A_copy(n, n) - (d / std::abs(d)) * (A_copy(n - 1, n) * A_copy(n - 1, n)) / (std::abs(d) + std::sqrt(d * d + A_copy(n - 1, n) * A_copy(n - 1, n)));

                // Restamos mu * I a la matriz
                for (size_t i = 0; i < mSize; ++i) A_copy(i, i) -= mu;

                // Descomposición QR de la matriz desplazada
                QRDecomposition<Matrix<value_type, rows, cols>> qr(A_copy);
                Matrix<value_type, rows, cols> Q = qr.q();
                Matrix<value_type, rows, cols> R = qr.r();

                // Nueva iteración: A' = R * Q + mu * I
                A_copy = R * Q;
                for (size_t i = 0; i < rows; ++i) A_copy(i, i) += mu;

                // Acumulamos las transformaciones en los autovectores
                mEigenvectors = mEigenvectors * Q;
            }

            // Extraer los autovalores de la diagonal de A_copy
            for (size_t i = 0; i < mSize; ++i) {
                mEigenvaluesReal[i] = A_copy(i, i);
            }

            mEigenvaluesImag = Vector<value_type, rows>::zero(mSize); // No hay parte imaginaria
        }
#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Exception when calculating the eigenvalues and eigenvectors of a symmetric matrix");
    }
}


template<DenseMatrix Mat>
void EigenDecomposition<Mat>::computeGeneral()
{
    try {
#ifdef TL_HAVE_OPENBLAS

        Matrix<value_type, rows, cols> A_copy = mMatrix;
        lapack::geev(lapack::Order::row_major,
                     lapack::EigenVectors::none,   // No queremos autovectores izquierdos
                     lapack::EigenVectors::compute,// Se calculan autovectores derechos
                     mSize, A_copy.data(),
                     mSize, mEigenvaluesReal.data(),
                     mEigenvaluesImag.data(), static_cast<value_type *>(nullptr),
                     mSize, mEigenvectors.data(), mSize); // Matriz de autovectores derechos

#else
        constexpr size_t maxIterations = 1000;
        constexpr value_type tolerance = static_cast<value_type>(1e-10);

        Matrix<value_type, rows, cols> A_copy = mMatrix;
        Matrix<value_type, rows, cols> Q_total = Matrix<value_type, rows, cols>::identity(mSize, mSize);

        for (size_t iter = 0; iter < maxIterations; ++iter) {
            QRDecomposition<Matrix<value_type, rows, cols>> qr(A_copy);
            Matrix<value_type, rows, cols> Q = qr.q();
            Matrix<value_type, rows, cols> R = qr.r();

            A_copy = R * Q;   // A_{k+1} = R_k * Q_k
            Q_total = Q_total * Q;  // Acumulamos la transformación

            // Convergencia: verificar si la matriz se ha vuelto casi diagonal
            bool converged = true;
            for (size_t i = 0; i < mSize; ++i) {
                for (size_t j = 0; j < mSize; ++j) {
                    if (i != j && std::abs(A_copy(i, j)) > tolerance) {
                        converged = false;
                        break;
                    }
                }
                if (!converged) break;
            }

            if (converged) break;
        }

        for (size_t i = 0; i < mSize; ++i) {
            mEigenvaluesReal[i] = A_copy(i, i);  // Los autovalores están en la diagonal
        }

        mEigenvaluesImag = Vector<value_type, rows>::zero(mSize); // Solo manejamos autovalores reales
        mEigenvectors = Q_total;  // Los autovectores son las columnas acumuladas de Q

        // Solución 2

        //constexpr size_t maxIterations = 1000;
        //constexpr value_type tolerance = static_cast<value_type>(1e-10);

        //Matrix<value_type, rows, cols> A_copy = mMatrix;
        //Matrix<value_type, rows, cols> Q_total = Matrix<value_type, rows, cols>::identity(mSize, mSize);

        //// Reducción a forma de Hessenberg
        //for (size_t k = 0; k < mSize - 2; ++k) {
        //    for (size_t i = k + 2; i < mSize; ++i) {
        //        value_type x = A_copy(i, k);
        //        value_type y = A_copy(k + 1, k);
        //        if (std::abs(x) < tolerance) continue;

        //        // Crear la rotación de Givens
        //        value_type r = std::hypot(y, x);
        //        value_type c = y / r;
        //        value_type s = -x / r;

        //        // Aplicar la rotación a la matriz A
        //        for (size_t j = k; j < mSize; ++j) {
        //            value_type temp1 = c * A_copy(k + 1, j) - s * A_copy(i, j);
        //            value_type temp2 = s * A_copy(k + 1, j) + c * A_copy(i, j);
        //            A_copy(k + 1, j) = temp1;
        //            A_copy(i, j) = temp2;
        //        }

        //        // Aplicar la rotación a la matriz Q_total
        //        for (size_t j = 0; j < mSize; ++j) {
        //            value_type temp1 = c * Q_total(j, k + 1) - s * Q_total(j, i);
        //            value_type temp2 = s * Q_total(j, k + 1) + c * Q_total(j, i);
        //            Q_total(j, k + 1) = temp1;
        //            Q_total(j, i) = temp2;
        //        }
        //    }
        //}

        //// Iteraciones QR con detección de bloques 2x2
        //for (size_t iter = 0; iter < maxIterations; ++iter) {
        //    QRDecomposition<Matrix<value_type, rows, cols>> qr(A_copy);
        //    Matrix<value_type, rows, cols> Q = qr.q();
        //    Matrix<value_type, rows, cols> R = qr.r();

        //    A_copy = R * Q;   // A_{k+1} = R_k * Q_k
        //    Q_total = Q_total * Q;  // Acumulamos la transformación

        //    // Detectar bloques 2x2 en la diagonal
        //    bool converged = true;
        //    for (size_t i = 0; i < mSize - 1; ++i) {
        //        if (std::abs(A_copy(i + 1, i)) > tolerance) {
        //            converged = false;
        //            break;
        //        }
        //    }
        //    if (converged) break;
        //}

        //// Extraer autovalores reales y complejos
        //for (size_t i = 0; i < mSize; ++i) {
        //    if (i < mSize - 1 && std::abs(A_copy(i + 1, i)) > tolerance) {
        //        // Bloque 2x2 -> calcular autovalores complejos
        //        value_type a = A_copy(i, i);
        //        value_type b = A_copy(i, i + 1);
        //        value_type c = A_copy(i + 1, i);
        //        value_type d = A_copy(i + 1, i + 1);
        //        value_type trace = a + d;
        //        value_type det = a * d - b * c;
        //        value_type discriminant = trace * trace - 4 * det;

        //        if (discriminant < 0) {  // Autovalores complejos
        //            mEigenvaluesReal[i] = trace / 2;
        //            mEigenvaluesImag[i] = std::sqrt(-discriminant) / 2;
        //            mEigenvaluesReal[i + 1] = trace / 2;
        //            mEigenvaluesImag[i + 1] = -std::sqrt(-discriminant) / 2;
        //            i++;  // Saltar al siguiente índice
        //        } else {  // Autovalores reales
        //            mEigenvaluesReal[i] = (trace + std::sqrt(discriminant)) / 2;
        //            mEigenvaluesReal[i + 1] = (trace - std::sqrt(discriminant)) / 2;
        //            mEigenvaluesImag[i] = 0;
        //            mEigenvaluesImag[i + 1] = 0;
        //        }
        //    } else {
        //        // Autovalores reales
        //        mEigenvaluesReal[i] = A_copy(i, i);
        //        mEigenvaluesImag[i] = 0;
        //    }
        //}

        //mEigenvectors = Q_total;  // Los autovectores son las columnas acumuladas de Q

        // Solución 3
        //constexpr size_t maxIterations = 1000;
        //constexpr value_type tolerance = static_cast<value_type>(1e-10);

        //Matrix<value_type, rows, cols> A_copy = mMatrix;
        //Matrix<value_type, rows, cols> Q_total = Matrix<value_type, rows, cols>::identity(mSize, mSize);

        //// Reducción a Hessenberg
        //for (size_t k = 0; k < mSize - 2; ++k) {
        //    for (size_t i = k + 2; i < mSize; ++i) {
        //        value_type x = A_copy(i, k);
        //        value_type y = A_copy(k + 1, k);
        //        if (std::abs(x) < tolerance) continue;

        //        // Rotación de Givens
        //        value_type r = std::hypot(y, x);
        //        value_type c = y / r;
        //        value_type s = -x / r;

        //        // Aplicar rotación en A
        //        for (size_t j = 0; j < mSize; ++j) {
        //            value_type temp1 = c * A_copy(k + 1, j) - s * A_copy(i, j);
        //            value_type temp2 = s * A_copy(k + 1, j) + c * A_copy(i, j);
        //            A_copy(k + 1, j) = temp1;
        //            A_copy(i, j) = temp2;
        //        }

        //        // Aplicar en Q_total
        //        for (size_t j = 0; j < mSize; ++j) {
        //            value_type temp1 = c * Q_total(j, k + 1) - s * Q_total(j, i);
        //            value_type temp2 = s * Q_total(j, k + 1) + c * Q_total(j, i);
        //            Q_total(j, k + 1) = temp1;
        //            Q_total(j, i) = temp2;
        //        }
        //    }
        //}

        //// Iteraciones QR con Shift
        //for (size_t iter = 0; iter < maxIterations; ++iter) {
        //    // Wilkinson Shift para mejorar convergencia
        //    value_type mu = A_copy(mSize - 1, mSize - 1);
        //    Matrix<value_type, rows, cols> I = Matrix<value_type, rows, cols>::identity(mSize, mSize);
        //    Matrix<value_type, rows, cols> A_shifted = A_copy - mu * I;

        //    QRDecomposition<Matrix<value_type, rows, cols>> qr(A_shifted);
        //    Matrix<value_type, rows, cols> Q = qr.q();
        //    Matrix<value_type, rows, cols> R = qr.r();

        //    TL_TODO("Cuando se implemente Evaluator quitar el eval(). Se hace para seguir compilando")
        //    A_copy = (R * Q).eval() + mu * I; // Aplicar el shift
        //    Q_total = Q_total * Q; // Acumular transformaciones

        //    // Detección de convergencia
        //    bool converged = true;
        //    for (size_t i = 0; i < mSize - 1; ++i) {
        //        if (std::abs(A_copy(i + 1, i)) > tolerance) {
        //            converged = false;
        //            break;
        //        }
        //    }
        //    if (converged) break;
        //}

        //// Extraer autovalores (reales o complejos)
        //for (size_t i = 0; i < mSize; ++i) {
        //    if (i < mSize - 1 && std::abs(A_copy(i + 1, i)) > tolerance) {
        //        // Bloque 2x2 → calcular autovalores complejos
        //        value_type a = A_copy(i, i);
        //        value_type b = A_copy(i, i + 1);
        //        value_type c = A_copy(i + 1, i);
        //        value_type d = A_copy(i + 1, i + 1);
        //        value_type trace = a + d;
        //        value_type det = a * d - b * c;
        //        value_type discriminant = trace * trace - 4 * det;

        //        if (discriminant < 0) {  // Autovalores complejos
        //            mEigenvaluesReal[i] = trace / 2;
        //            mEigenvaluesImag[i] = std::sqrt(-discriminant) / 2;
        //            mEigenvaluesReal[i + 1] = trace / 2;
        //            mEigenvaluesImag[i + 1] = -std::sqrt(-discriminant) / 2;
        //            i++;  // Saltar índice
        //        } else {  // Autovalores reales
        //            mEigenvaluesReal[i] = (trace + std::sqrt(discriminant)) / 2;
        //            mEigenvaluesReal[i + 1] = (trace - std::sqrt(discriminant)) / 2;
        //            mEigenvaluesImag[i] = 0;
        //            mEigenvaluesImag[i + 1] = 0;
        //        }
        //    } else {
        //        // Autovalores reales
        //        mEigenvaluesReal[i] = A_copy(i, i);
        //        mEigenvaluesImag[i] = 0;
        //    }
        //}

        //mEigenvectors = Q_total;  // Autovectores = acumulación de Q

#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Exception when calculating the eigenvalues and eigenvectors of a general matrix.");
    }
}

/*! \} */ 

} // End namespace tl
