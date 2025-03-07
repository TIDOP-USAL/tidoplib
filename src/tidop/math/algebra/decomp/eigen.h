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
#include "tidop/math/algebra/decomp/qr.h"
#include "tidop/math/base/lapack.h"

namespace tl
{

/*! \addtogroup Decomposition
 *  \{
 */


/// \cond

template<typename T>
class EigenDecomposition;

/// \endcond

/*!
 * \brief Eigenvalue Decomposition
 *
 * In linear algebra, the Eigenvalue Decomposition of a matrix \( A \) is a factorization of the matrix
 * that has many applications, such as solving linear systems, signal processing, and data compression.
 * The Eigenvalue Decomposition of a matrix allows it to be expressed as a product of three matrices:
 * an orthogonal matrix \( U \), a diagonal matrix \( \Sigma \), and the transpose of another orthogonal
 * matrix \( V \).
 *
 * Given an \( n \times n \) matrix \( A \), the Eigenvalue Decomposition is given by:
 *
 * \f[
 * A = V \cdot \Lambda \cdot V^{-1}
 * \f]
 *
 * where \( V \) is the matrix of eigenvectors, and \( \Lambda \) is the diagonal matrix of eigenvalues.
 *
 * \tparam Matrix_t The type of the matrix to decompose.
 */
template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols>
class EigenDecomposition<Matrix_t<T, Rows, Cols>>
{

public:

    /*!
     * \brief Constructs an eigenvalue decomposition object for a given square matrix.
     *
     * This constructor initializes the eigenvalue decomposition for a square matrix. It computes the eigenvalues and eigenvectors based on whether the matrix is symmetric or general (non-symmetric). If the matrix is symmetric, the decomposition uses a more efficient method; otherwise, a general eigenvalue decomposition method is used.
     *
     * - If the matrix is symmetric, the `computeSymmetric` method is used to compute the eigenvalues and eigenvectors.
     * - If the matrix is not symmetric, the `computeGeneral` method is used.
     *
     * \param a The input matrix for which eigenvalues and eigenvectors are to be computed. This matrix must be square.
     *
     * \throws std::invalid_argument if the matrix is not square.
     * \throws std::logic_error if the matrix is not of floating-point type.
     *
     * \note
     * - The matrix `a` must be square (i.e., the number of rows must equal the number of columns).
     * - Only floating-point matrix types (`float`, `double`, `long double`) are supported. Integral types (e.g., `int`, `long`) are not supported.
     */
    EigenDecomposition(const Matrix_t<T, Rows, Cols> &a);

    auto eigenvaluesReal() const -> Vector<T, Rows> { return mEigenvaluesReal; }
    auto eigenvaluesImag() const -> Vector<T, Rows> { return mEigenvaluesImag; }
    auto eigenvectors() const -> Matrix_t<T, Rows, Cols> { return mEigenvectors; }

private:

    /*!
     * \brief Computes the eigenvalues and eigenvectors of a symmetric matrix.
     *
     * This function computes the eigenvalues and eigenvectors of a symmetric matrix using different methods depending on the availability of LAPACK and the matrix size. If LAPACK is available, the optimized `syev` function from LAPACK is used to efficiently compute the results.
     * If LAPACK is not available and the matrix size is smaller than 10x10, the Jacobi method is used, which is an iterative and robust algorithm for small matrices. For matrices of size 10x10 or larger, the QR algorithm with Wilkinson's shift is used, which improves the convergence of the QR method for large matrices.
     *
     * - If LAPACK is available, its optimized function is used to calculate the eigenvalues and eigenvectors.
     * - If the matrix is smaller than 10x10, the Jacobi algorithm is used.
     * - If the matrix is 10x10 or larger, the QR algorithm with Wilkinson's shift is used.
     *
     * \note
     * - The Jacobi method is suitable for small matrices due to its high accuracy, although it is slower for larger matrices.
     * - The QR algorithm with Wilkinson's shift is more efficient for larger matrices, as it accelerates the convergence of the QR method by subtracting a shift (mu) from the matrix in each iteration.
     *
     * \param mMatrix The symmetric input matrix for which eigenvalues and eigenvectors are to be computed.
     *
     * \return No explicit return value, but the results of the eigenvalues are stored in `mEigenvaluesReal`, and the eigenvectors are stored in `mEigenvectors`.
     *
     * \note
     * - Ensure that the input matrix is square.
     * - The matrix must be symmetric for valid results.
     */
    void computeSymmetric()
    {
#ifdef TL_HAVE_OPENBLAS

        mEigenvectors = mMatrix;

        lapack_int info = lapack::syev('V', // 'V' para calcular autovectores
                                       'U', // Usamos la parte superior de la matriz
                                       static_cast<int>(mSize),
                                       mEigenvectors.data(),
                                       static_cast<int>(mSize),
                                       mEigenvaluesReal.data());

        TL_ASSERT(info >= 0, "Eigenvalue decomposition did not converge.");

#else
        if (mSize < 10) {
            constexpr size_t maxIterations = 100;
            constexpr T tolerance = static_cast<T>(1e-10);

            Matrix_t<T, Rows, Cols> A_copy = mMatrix;
            Matrix_t<T, Rows, Cols> V = Matrix_t<T, Rows, Cols>::identity(mSize, mSize);

            for (size_t iter = 0; iter < maxIterations; ++iter) {
                // Buscar el mayor elemento fuera de la diagonal
                size_t p = 0, q = 1;
                T maxOffDiagonal = 0;
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
                T theta = (A_copy(q, q) - A_copy(p, p)) / (2 * A_copy(p, q));
                T t = std::copysign(1, theta) / (std::abs(theta) + std::sqrt(theta * theta + 1));
                T c = 1 / std::sqrt(t * t + 1);
                T s = t * c;

                // Aplicar la rotación a A_copy
                for (size_t i = 0; i < mSize; ++i) {
                    if (i != p && i != q) {
                        T A_ip = A_copy(i, p);
                        T A_iq = A_copy(i, q);
                        A_copy(i, p) = A_ip * c - A_iq * s;
                        A_copy(p, i) = A_copy(i, p);
                        A_copy(i, q) = A_ip * s + A_iq * c;
                        A_copy(q, i) = A_copy(i, q);
                    }
                }

                T A_pp = A_copy(p, p);
                T A_qq = A_copy(q, q);
                T A_pq = A_copy(p, q);

                A_copy(p, p) = c * c * A_pp - 2 * s * c * A_pq + s * s * A_qq;
                A_copy(q, q) = s * s * A_pp + 2 * s * c * A_pq + c * c * A_qq;
                A_copy(p, q) = A_copy(q, p) = 0;

                // Acumular la rotación en la matriz de autovectores
                for (size_t i = 0; i < mSize; ++i) {
                    T V_ip = V(i, p);
                    T V_iq = V(i, q);
                    V(i, p) = V_ip * c - V_iq * s;
                    V(i, q) = V_ip * s + V_iq * c;
                }
            }

            // Extraer los autovalores y autovectores
            for (size_t i = 0; i < mSize; ++i) {
                mEigenvaluesReal[i] = A_copy(i, i);
            }

            mEigenvaluesImag = Vector<T, Rows>::zero(mSize); // No hay parte imaginaria en matrices simétricas
            mEigenvectors = V;
        } else {
            constexpr size_t maxIterations = 100;
            constexpr T tolerance = static_cast<T>(1e-10);

            Matrix_t<T, Rows, Cols> A_copy = mMatrix;
            mEigenvectors = Matrix_t<T, Rows, Cols>::identity(mSize, mSize);

            for (size_t iter = 0; iter < maxIterations; ++iter) {
                // Verificamos convergencia: si la matriz es casi diagonal, terminamos
                T offDiagNorm = 0;
                for (size_t i = 0; i < mSize; ++i) {
                    for (size_t j = 0; j < mSize; ++j) {
                        if (i != j) offDiagNorm += A_copy(i, j) * A_copy(i, j);
                    }
                }
                if (std::sqrt(offDiagNorm) < tolerance) break;

                // Desplazamiento de Wilkinson para acelerar convergencia
                size_t n = Rows - 1;
                T d = (A_copy(n - 1, n - 1) - A_copy(n, n)) / 2.0;
                T mu = A_copy(n, n) - (d / std::abs(d)) * (A_copy(n - 1, n) * A_copy(n - 1, n)) / (std::abs(d) + std::sqrt(d * d + A_copy(n - 1, n) * A_copy(n - 1, n)));

                // Restamos mu * I a la matriz
                for (size_t i = 0; i < mSize; ++i) A_copy(i, i) -= mu;

                // Descomposición QR de la matriz desplazada
                QRDecomposition<Matrix_t<T, Rows, Cols>> qr(A_copy);
                Matrix_t<T, Rows, Cols> Q = qr.q();
                Matrix_t<T, Rows, Cols> R = qr.r();

                // Nueva iteración: A' = R * Q + mu * I
                A_copy = R * Q;
                for (size_t i = 0; i < Rows; ++i) A_copy(i, i) += mu;

                // Acumulamos las transformaciones en los autovectores
                mEigenvectors = mEigenvectors * Q;
            }

            // Extraer los autovalores de la diagonal de A_copy
            for (size_t i = 0; i < mSize; ++i) {
                mEigenvaluesReal[i] = A_copy(i, i);
            }

            mEigenvaluesImag = Vector<T, Rows>::zero(mSize); // No hay parte imaginaria
        }
#endif
    }

    /*!
     * \brief Computes the eigenvalues and eigenvectors of a general matrix.
     *
     * This function computes the eigenvalues and eigenvectors of a general matrix (not necessarily symmetric) using different methods depending on the availability of LAPACK.
     * If LAPACK is available, the optimized `geev` function from LAPACK is used to compute the eigenvalues and right eigenvectors.
     * If LAPACK is not available, the QR algorithm is used iteratively to compute the eigenvalues and eigenvectors.
     *
     * - If LAPACK is available, its optimized function `geev` is used to calculate the eigenvalues and right eigenvectors.
     * - If LAPACK is not available, the QR algorithm is used iteratively to converge towards the eigenvalues and eigenvectors.
     *
     * The QR algorithm works by iteratively decomposing the matrix into a product of an orthogonal matrix (Q) and an upper triangular matrix (R), and updating the matrix as \( A_{k+1} = R_k \times Q_k \). The eigenvalues are extracted from the diagonal of the matrix when it becomes nearly diagonal.
     *
     * \note
     * - This method handles general matrices, meaning matrices that are not necessarily symmetric.
     * - The convergence criterion is when the off-diagonal elements of the matrix are sufficiently small (below a specified tolerance).
     *
     * \param mMatrix The input matrix for which eigenvalues and eigenvectors are to be computed.
     *
     * \return No explicit return value, but the eigenvalues are stored in `mEigenvaluesReal` and `mEigenvaluesImag` (the imaginary parts are zero for real eigenvalues), and the eigenvectors are stored in `mEigenvectors`.
     *
     * \note
     * - Ensure that the input matrix is square for eigenvalue computation.
     */
    void computeGeneral() 
    {
#ifdef TL_HAVE_OPENBLAS

        Matrix_t<T, Rows, Cols> A_copy = mMatrix;
        lapack_int info = lapack::geev('N', // No queremos autovectores izquierdos
                                       'V', // Se calculan autovectores derechos
                                       static_cast<int>(mSize), 
                                       A_copy.data(),
                                       static_cast<int>(mSize), mEigenvaluesReal.data(),
                                       mEigenvaluesImag.data(), static_cast<T *>(nullptr),
                                       static_cast<int>(mSize),
                                       mEigenvectors.data(), static_cast<int>(mSize)); // Matriz de autovectores derechos

        TL_ASSERT(info >= 0, "Eigenvalue decomposition did not converge.");

#else
        //constexpr size_t maxIterations = 1000;
        //constexpr T tolerance = static_cast<T>(1e-10);

        //Matrix_t<T, Rows, Cols> A_copy = mMatrix;
        //Matrix_t<T, Rows, Cols> Q_total = Matrix_t<T, Rows, Cols>::identity(mSize, mSize);

        //for (size_t iter = 0; iter < maxIterations; ++iter) {
        //    QRDecomposition<Matrix_t<T, Rows, Cols>> qr(A_copy);
        //    Matrix_t<T, Rows, Cols> Q = qr.q();
        //    Matrix_t<T, Rows, Cols> R = qr.r();

        //    A_copy = R * Q;   // A_{k+1} = R_k * Q_k
        //    Q_total = Q_total * Q;  // Acumulamos la transformación

        //    // Convergencia: verificar si la matriz se ha vuelto casi diagonal
        //    bool converged = true;
        //    for (size_t i = 0; i < mSize; ++i) {
        //        for (size_t j = 0; j < mSize; ++j) {
        //            if (i != j && std::abs(A_copy(i, j)) > tolerance) {
        //                converged = false;
        //                break;
        //            }
        //        }
        //        if (!converged) break;
        //    }

        //    if (converged) break;
        //}

        //for (size_t i = 0; i < mSize; ++i) {
        //    mEigenvaluesReal[i] = A_copy(i, i);  // Los autovalores están en la diagonal
        //}

        //mEigenvaluesImag = Vector<T, Rows>::zero(mSize); // Solo manejamos autovalores reales
        //mEigenvectors = Q_total;  // Los autovectores son las columnas acumuladas de Q

        // Solución 2

        //constexpr size_t maxIterations = 1000;
        //constexpr T tolerance = static_cast<T>(1e-10);

        //Matrix_t<T, Rows, Cols> A_copy = mMatrix;
        //Matrix_t<T, Rows, Cols> Q_total = Matrix_t<T, Rows, Cols>::identity(mSize, mSize);

        //// 🔹 Paso 1: Reducción a forma de Hessenberg
        //for (size_t k = 0; k < mSize - 2; ++k) {
        //    for (size_t i = k + 2; i < mSize; ++i) {
        //        T x = A_copy(i, k);
        //        T y = A_copy(k + 1, k);
        //        if (std::abs(x) < tolerance) continue;

        //        // Crear la rotación de Givens
        //        T r = std::hypot(y, x);
        //        T c = y / r;
        //        T s = -x / r;

        //        // Aplicar la rotación a la matriz A
        //        for (size_t j = k; j < mSize; ++j) {
        //            T temp1 = c * A_copy(k + 1, j) - s * A_copy(i, j);
        //            T temp2 = s * A_copy(k + 1, j) + c * A_copy(i, j);
        //            A_copy(k + 1, j) = temp1;
        //            A_copy(i, j) = temp2;
        //        }

        //        // Aplicar la rotación a la matriz Q_total
        //        for (size_t j = 0; j < mSize; ++j) {
        //            T temp1 = c * Q_total(j, k + 1) - s * Q_total(j, i);
        //            T temp2 = s * Q_total(j, k + 1) + c * Q_total(j, i);
        //            Q_total(j, k + 1) = temp1;
        //            Q_total(j, i) = temp2;
        //        }
        //    }
        //}

        //// 🔹 Paso 2: Iteraciones QR con detección de bloques 2x2
        //for (size_t iter = 0; iter < maxIterations; ++iter) {
        //    QRDecomposition<Matrix_t<T, Rows, Cols>> qr(A_copy);
        //    Matrix_t<T, Rows, Cols> Q = qr.q();
        //    Matrix_t<T, Rows, Cols> R = qr.r();

        //    A_copy = R * Q;   // A_{k+1} = R_k * Q_k
        //    Q_total = Q_total * Q;  // Acumulamos la transformación

        //    // 🔹 Paso 3: Detectar bloques 2x2 en la diagonal
        //    bool converged = true;
        //    for (size_t i = 0; i < mSize - 1; ++i) {
        //        if (std::abs(A_copy(i + 1, i)) > tolerance) {
        //            converged = false;
        //            break;
        //        }
        //    }
        //    if (converged) break;
        //}

        //// 🔹 Paso 4: Extraer autovalores reales y complejos
        //for (size_t i = 0; i < mSize; ++i) {
        //    if (i < mSize - 1 && std::abs(A_copy(i + 1, i)) > tolerance) {
        //        // Bloque 2x2 -> calcular autovalores complejos
        //        T a = A_copy(i, i);
        //        T b = A_copy(i, i + 1);
        //        T c = A_copy(i + 1, i);
        //        T d = A_copy(i + 1, i + 1);
        //        T trace = a + d;
        //        T det = a * d - b * c;
        //        T discriminant = trace * trace - 4 * det;

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
        constexpr size_t maxIterations = 1000;
        constexpr T tolerance = static_cast<T>(1e-10);

        Matrix_t<T, Rows, Cols> A_copy = mMatrix;
        Matrix_t<T, Rows, Cols> Q_total = Matrix_t<T, Rows, Cols>::identity(mSize, mSize);

        // 🔹 Paso 1: Reducción a Hessenberg
        for (size_t k = 0; k < mSize - 2; ++k) {
            for (size_t i = k + 2; i < mSize; ++i) {
                T x = A_copy(i, k);
                T y = A_copy(k + 1, k);
                if (std::abs(x) < tolerance) continue;

                // Rotación de Givens
                T r = std::hypot(y, x);
                T c = y / r;
                T s = -x / r;

                // Aplicar rotación en A
                for (size_t j = 0; j < mSize; ++j) {
                    T temp1 = c * A_copy(k + 1, j) - s * A_copy(i, j);
                    T temp2 = s * A_copy(k + 1, j) + c * A_copy(i, j);
                    A_copy(k + 1, j) = temp1;
                    A_copy(i, j) = temp2;
                }

                // Aplicar en Q_total
                for (size_t j = 0; j < mSize; ++j) {
                    T temp1 = c * Q_total(j, k + 1) - s * Q_total(j, i);
                    T temp2 = s * Q_total(j, k + 1) + c * Q_total(j, i);
                    Q_total(j, k + 1) = temp1;
                    Q_total(j, i) = temp2;
                }
            }
        }

        // 🔹 Paso 2: Iteraciones QR con Shift
        for (size_t iter = 0; iter < maxIterations; ++iter) {
            // Wilkinson Shift para mejorar convergencia
            T mu = A_copy(mSize - 1, mSize - 1);
            Matrix_t<T, Rows, Cols> I = Matrix_t<T, Rows, Cols>::identity(mSize, mSize);
            Matrix_t<T, Rows, Cols> A_shifted = A_copy - mu * I;

            QRDecomposition<Matrix_t<T, Rows, Cols>> qr(A_shifted);
            Matrix_t<T, Rows, Cols> Q = qr.q();
            Matrix_t<T, Rows, Cols> R = qr.r();

            A_copy = R * Q + mu * I; // Aplicar el shift
            Q_total = Q_total * Q; // Acumular transformaciones

            // 🔹 Paso 3: Detección de convergencia
            bool converged = true;
            for (size_t i = 0; i < mSize - 1; ++i) {
                if (std::abs(A_copy(i + 1, i)) > tolerance) {
                    converged = false;
                    break;
                }
            }
            if (converged) break;
        }

        // 🔹 Paso 4: Extraer autovalores (reales o complejos)
        for (size_t i = 0; i < mSize; ++i) {
            if (i < mSize - 1 && std::abs(A_copy(i + 1, i)) > tolerance) {
                // Bloque 2x2 → calcular autovalores complejos
                T a = A_copy(i, i);
                T b = A_copy(i, i + 1);
                T c = A_copy(i + 1, i);
                T d = A_copy(i + 1, i + 1);
                T trace = a + d;
                T det = a * d - b * c;
                T discriminant = trace * trace - 4 * det;

                if (discriminant < 0) {  // Autovalores complejos
                    mEigenvaluesReal[i] = trace / 2;
                    mEigenvaluesImag[i] = std::sqrt(-discriminant) / 2;
                    mEigenvaluesReal[i + 1] = trace / 2;
                    mEigenvaluesImag[i + 1] = -std::sqrt(-discriminant) / 2;
                    i++;  // Saltar índice
                } else {  // Autovalores reales
                    mEigenvaluesReal[i] = (trace + std::sqrt(discriminant)) / 2;
                    mEigenvaluesReal[i + 1] = (trace - std::sqrt(discriminant)) / 2;
                    mEigenvaluesImag[i] = 0;
                    mEigenvaluesImag[i + 1] = 0;
                }
            } else {
                // Autovalores reales
                mEigenvaluesReal[i] = A_copy(i, i);
                mEigenvaluesImag[i] = 0;
            }
        }

        mEigenvectors = Q_total;  // Autovectores = acumulación de Q

#endif
    }
	
private:
	
    Matrix_t<T, Rows, Cols> mMatrix;
    Vector<T, Rows> mEigenvaluesReal;
    Vector<T, Rows> mEigenvaluesImag;
    Matrix_t<T, Rows, Cols> mEigenvectors;
    size_t mSize;
};


template<
    template<typename, size_t, size_t>
class Matrix_t, typename T, size_t Rows, size_t Cols
>
EigenDecomposition<Matrix_t<T, Rows, Cols>>::EigenDecomposition(const Matrix_t<T, Rows, Cols> &a)
  : mMatrix(a), 
    mSize(a.rows())
{
	static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    TL_ASSERT(mMatrix.isSquare(), "Matrix must be square.");

    mEigenvaluesReal = Vector<T, Cols>::zero(mSize);
    mEigenvaluesImag = Vector<T, Cols>::zero(mSize);
    mEigenvectors = Matrix_t<T, Cols, Cols>::zero(mSize, mSize);

    if (mMatrix.isSymmetric()) {
        computeSymmetric();
    } else {
        computeGeneral();
    }

}


/*! \} */ 

} // End namespace tl
