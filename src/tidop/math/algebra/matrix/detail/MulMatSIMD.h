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

#include "tidop/math/base/Simd.h"

namespace tl
{

namespace detail
{

#ifdef TL_HAVE_SIMD_INTRINSICS

TL_WARNING_PUSH
TL_DISABLE_WARNING_MSVC(4849)
template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd_kernel_opt3(const LHS &A, const RHS &B, OutMat &C)
{
    using T = typename matrix_traits<OutMat>::value_type;

    const size_t rows = A.rows();
    const size_t dim = A.cols();
    const size_t cols = B.cols();

    constexpr size_t MR = 8;
    constexpr size_t PS = Packed<T>().size();

    // Ajustados para no saturar L1/L2
    constexpr size_t BLOCK_R = 128;
    constexpr size_t BLOCK_K = 64;
    constexpr size_t BLOCK_C = 128;

    // COLLAPSE(2): Creamos una malla 2D de tareas (Filas x Columnas)
    // schedule(dynamic) asegura que los hilos no se queden atascados si un bloque es más pequeño
#pragma omp parallel for collapse(2) schedule(dynamic)
    for (long long ii = 0; ii < static_cast<long long>(rows); ii += BLOCK_R) {
        for (long long jj = 0; jj < static_cast<long long>(cols); jj += BLOCK_C) {

            // Mover kk aquí adentro permite colapsar ii y jj de forma segura.
            // Cada hilo calcula su parche bloque por bloque en profundidad.
            for (size_t kk = 0; kk < dim; kk += BLOCK_K) {

                const size_t i_max = std::min((size_t)ii + BLOCK_R, rows);
                const size_t j_max = std::min((size_t)jj + BLOCK_C, cols);
                const size_t k_max = std::min(kk + BLOCK_K, dim);

                for (size_t i = ii; i + MR <= i_max; i += MR) {

                    size_t j = jj;
                    size_t j_vec_end = j_max - (j_max - j) % PS;

                    for (; j < j_vec_end; j += PS) {

                        // Acumuladores en registros
                        Packed<T> c0, c1, c2, c3, c4, c5, c6, c7;

                        // Carga de C (Se hace de forma natural gracias al caché)
                        c0.loadUnaligned(&C(i + 0, j));
                        c1.loadUnaligned(&C(i + 1, j));
                        c2.loadUnaligned(&C(i + 2, j));
                        c3.loadUnaligned(&C(i + 3, j));
                        c4.loadUnaligned(&C(i + 4, j));
                        c5.loadUnaligned(&C(i + 5, j));
                        c6.loadUnaligned(&C(i + 6, j));
                        c7.loadUnaligned(&C(i + 7, j));

                        for (size_t k = kk; k < k_max; ++k) {

                            Packed<T> b;
                            b.loadUnaligned(&B(k, j)); // El Hardware Prefetcher ama esto

                            Packed<T> a;
                            a.setScalar(A(i + 0, k)); c0 += a * b;
                            a.setScalar(A(i + 1, k)); c1 += a * b;
                            a.setScalar(A(i + 2, k)); c2 += a * b;
                            a.setScalar(A(i + 3, k)); c3 += a * b;
                            a.setScalar(A(i + 4, k)); c4 += a * b;
                            a.setScalar(A(i + 5, k)); c5 += a * b;
                            a.setScalar(A(i + 6, k)); c6 += a * b;
                            a.setScalar(A(i + 7, k)); c7 += a * b;
                        }

                        // Escritura en C
                        c0.storeUnaligned(&C(i + 0, j));
                        c1.storeUnaligned(&C(i + 1, j));
                        c2.storeUnaligned(&C(i + 2, j));
                        c3.storeUnaligned(&C(i + 3, j));
                        c4.storeUnaligned(&C(i + 4, j));
                        c5.storeUnaligned(&C(i + 5, j));
                        c6.storeUnaligned(&C(i + 6, j));
                        c7.storeUnaligned(&C(i + 7, j));
                    }

                    // Resto columnas (Escalar)
                    for (; j < j_max; ++j) {
                        for (size_t r = 0; r < MR; ++r) {
                            T cval = C(i + r, j);
                            for (size_t k = kk; k < k_max; ++k) {
                                cval += A(i + r, k) * B(k, j);
                            }
                            C(i + r, j) = cval;
                        }
                    }
                }

                // Resto filas (Escalar) - Ajustado ligeramente para no perderse respecto a 'ii'
                const size_t start_i = ii + ((i_max - ii) / MR) * MR;
                for (size_t i = start_i; i < i_max; ++i) {
                    for (size_t j = jj; j < j_max; ++j) {
                        T cval = C(i, j);
                        for (size_t k = kk; k < k_max; ++k) {
                            cval += A(i, k) * B(k, j);
                        }
                        C(i, j) = cval;
                    }
                }
            }
        }
    }
}
TL_WARNING_POP


template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd3(const LHS &matrix1, const RHS &matrix2, OutMat &matrix)
{
    using T = typename matrix_traits<OutMat>::value_type;

    size_t rows = matrix1.rows();
    size_t dim = matrix1.cols();
    size_t cols = matrix2.cols();

    constexpr size_t packed_size = Packed<T>().size();
    size_t max_vector = cols - (cols % packed_size);

    size_t r = 0;

    // --- BLOQUE DE 8 FILAS ---
    for (; r + 8 <= rows; r += 8) {
        // SIMD principal
        for (size_t c = 0; c < max_vector; c += packed_size) {
            // ACUMULADORES EN REGISTROS (Cero accesos a RAM en el bucle interno)
            Packed<T> acc0(0), acc1(0), acc2(0), acc3(0), acc4(0), acc5(0), acc6(0), acc7(0);

            for (size_t i = 0; i < dim; i++) {
                Packed<T> b;
                b.loadUnaligned(&matrix2(i, c));

                Packed<T> a;
                a.setScalar(matrix1(r + 0, i)); acc0 += a * b;
                a.setScalar(matrix1(r + 1, i)); acc1 += a * b;
                a.setScalar(matrix1(r + 2, i)); acc2 += a * b;
                a.setScalar(matrix1(r + 3, i)); acc3 += a * b;
                a.setScalar(matrix1(r + 4, i)); acc4 += a * b;
                a.setScalar(matrix1(r + 5, i)); acc5 += a * b;
                a.setScalar(matrix1(r + 6, i)); acc6 += a * b;
                a.setScalar(matrix1(r + 7, i)); acc7 += a * b;
            }

            // Escritura única en memoria
            Packed<T> c_vec;
            c_vec.loadUnaligned(&matrix(r + 0, c)); c_vec += acc0; c_vec.storeUnaligned(&matrix(r + 0, c));
            c_vec.loadUnaligned(&matrix(r + 1, c)); c_vec += acc1; c_vec.storeUnaligned(&matrix(r + 1, c));
            c_vec.loadUnaligned(&matrix(r + 2, c)); c_vec += acc2; c_vec.storeUnaligned(&matrix(r + 2, c));
            c_vec.loadUnaligned(&matrix(r + 3, c)); c_vec += acc3; c_vec.storeUnaligned(&matrix(r + 3, c));
            c_vec.loadUnaligned(&matrix(r + 4, c)); c_vec += acc4; c_vec.storeUnaligned(&matrix(r + 4, c));
            c_vec.loadUnaligned(&matrix(r + 5, c)); c_vec += acc5; c_vec.storeUnaligned(&matrix(r + 5, c));
            c_vec.loadUnaligned(&matrix(r + 6, c)); c_vec += acc6; c_vec.storeUnaligned(&matrix(r + 6, c));
            c_vec.loadUnaligned(&matrix(r + 7, c)); c_vec += acc7; c_vec.storeUnaligned(&matrix(r + 7, c));
        }

        // Resto escalar de columnas (usando acumuladores locales)
        for (size_t c = max_vector; c < cols; c++) {
            T acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0, acc4 = 0, acc5 = 0, acc6 = 0, acc7 = 0;
            for (size_t i = 0; i < dim; i++) {
                T b = matrix2(i, c);
                acc0 += matrix1(r + 0, i) * b;
                acc1 += matrix1(r + 1, i) * b;
                acc2 += matrix1(r + 2, i) * b;
                acc3 += matrix1(r + 3, i) * b;
                acc4 += matrix1(r + 4, i) * b;
                acc5 += matrix1(r + 5, i) * b;
                acc6 += matrix1(r + 6, i) * b;
                acc7 += matrix1(r + 7, i) * b;
            }
            matrix(r + 0, c) += acc0;
            matrix(r + 1, c) += acc1;
            matrix(r + 2, c) += acc2;
            matrix(r + 3, c) += acc3;
            matrix(r + 4, c) += acc4;
            matrix(r + 5, c) += acc5;
            matrix(r + 6, c) += acc6;
            matrix(r + 7, c) += acc7;
        }
    }

    // --- BLOQUE DE 4 FILAS ---
    for (; r + 4 <= rows; r += 4) {
        for (size_t c = 0; c < max_vector; c += packed_size) {
            Packed<T> acc0(0), acc1(0), acc2(0), acc3(0);
            for (size_t i = 0; i < dim; i++) {
                Packed<T> b; b.loadUnaligned(&matrix2(i, c));
                Packed<T> a;
                a.setScalar(matrix1(r + 0, i)); acc0 += a * b;
                a.setScalar(matrix1(r + 1, i)); acc1 += a * b;
                a.setScalar(matrix1(r + 2, i)); acc2 += a * b;
                a.setScalar(matrix1(r + 3, i)); acc3 += a * b;
            }
            Packed<T> c_vec;
            c_vec.loadUnaligned(&matrix(r + 0, c)); c_vec += acc0; c_vec.storeUnaligned(&matrix(r + 0, c));
            c_vec.loadUnaligned(&matrix(r + 1, c)); c_vec += acc1; c_vec.storeUnaligned(&matrix(r + 1, c));
            c_vec.loadUnaligned(&matrix(r + 2, c)); c_vec += acc2; c_vec.storeUnaligned(&matrix(r + 2, c));
            c_vec.loadUnaligned(&matrix(r + 3, c)); c_vec += acc3; c_vec.storeUnaligned(&matrix(r + 3, c));
        }
        for (size_t c = max_vector; c < cols; c++) {
            T acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
            for (size_t i = 0; i < dim; i++) {
                T b = matrix2(i, c);
                acc0 += matrix1(r + 0, i) * b;
                acc1 += matrix1(r + 1, i) * b;
                acc2 += matrix1(r + 2, i) * b;
                acc3 += matrix1(r + 3, i) * b;
            }
            matrix(r + 0, c) += acc0;
            matrix(r + 1, c) += acc1;
            matrix(r + 2, c) += acc2;
            matrix(r + 3, c) += acc3;
        }
    }

    // --- BLOQUE DE 2 FILAS ---
    for (; r + 2 <= rows; r += 2) {
        for (size_t c = 0; c < max_vector; c += packed_size) {
            Packed<T> acc0(0), acc1(0);
            for (size_t i = 0; i < dim; i++) {
                Packed<T> b; b.loadUnaligned(&matrix2(i, c));
                Packed<T> a;
                a.setScalar(matrix1(r + 0, i)); acc0 += a * b;
                a.setScalar(matrix1(r + 1, i)); acc1 += a * b;
            }
            Packed<T> c_vec;
            c_vec.loadUnaligned(&matrix(r + 0, c)); c_vec += acc0; c_vec.storeUnaligned(&matrix(r + 0, c));
            c_vec.loadUnaligned(&matrix(r + 1, c)); c_vec += acc1; c_vec.storeUnaligned(&matrix(r + 1, c));
        }
        for (size_t c = max_vector; c < cols; c++) {
            T acc0 = 0, acc1 = 0;
            for (size_t i = 0; i < dim; i++) {
                T b = matrix2(i, c);
                acc0 += matrix1(r + 0, i) * b;
                acc1 += matrix1(r + 1, i) * b;
            }
            matrix(r + 0, c) += acc0;
            matrix(r + 1, c) += acc1;
        }
    }

    // --- BLOQUE DE 1 FILA (Para matrices impares) ---
    for (; r < rows; r++) {

        for (size_t c = 0; c < max_vector; c += packed_size) {
            Packed<T> acc0(0);
            for (size_t i = 0; i < dim; i++) {
                Packed<T> b; b.loadUnaligned(&matrix2(i, c));
                Packed<T> a;
                a.setScalar(matrix1(r, i)); acc0 += a * b;
            }
            Packed<T> c_vec;
            c_vec.loadUnaligned(&matrix(r, c)); 
            c_vec += acc0;
            c_vec.storeUnaligned(&matrix(r, c));
        }

        for (size_t c = max_vector; c < cols; c++) {
            T acc0 = 0;
            for (size_t i = 0; i < dim; i++) {
                acc0 += matrix1(r, i) * matrix2(i, c);
            }
            matrix(r, c) += acc0;
        }
    }
}

#endif // TL_HAVE_SIMD_INTRINSICS

} // namespace detail

} // namespace tl