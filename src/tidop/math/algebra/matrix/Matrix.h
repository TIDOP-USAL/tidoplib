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

#include "tidop/core/base/exception.h"
#include "tidop/core/concurrency/parallel.h"
#include "tidop/math/math.h"
//#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/base/simd.h"
#include "tidop/math/base/blas.h"
#include "tidop/math/base/cuda.h"
#include "tidop/math/base/data.h"
#include "tidop/math/algebra/decomp/lu.h"
#include "tidop/math/algebra/decomp/svd.h"
#include "tidop/math/algebra/matrix/MatrixBase.h"
#include "tidop/math/algebra/matrix/MatrixBlock.h"
#include "tidop/math/algebra/matrix/MatrixRow.h"
#include "tidop/math/algebra/matrix/MatrixCol.h"
#include "tidop/math/algebra/matrix/Kernel.h"
#include "tidop/math/algebra/detail/Evaluator.h"

#include <type_traits>
#include <iomanip>
#include <concepts>

namespace tl
{

/*! \addtogroup Matrix
 *  \{
 */

template<typename T, size_t Rows, size_t Cols>
class Matrix;

//template<typename T>
//class LuDecomposition;
//
//template<typename T>
//class SingularValueDecomposition;

/// \endcond
	

class TL_EXPORT MatrixConfigExpr
{

public:

    enum class Product
    {
		AUTO,
#ifdef TL_HAVE_CUDA
        CuBLAS,
#endif
#ifdef TL_HAVE_OPENBLAS
        BLAS,
#endif
#ifdef TL_HAVE_SIMD_INTRINSICS
        SIMD,
#endif
        CPP
    };

    Product product = Product::AUTO;
//#ifdef TL_HAVE_SIMD_INTRINSICS
//        Product::SIMD;
//#else
//        Product::CPP;
//#endif

    static auto instance() -> MatrixConfigExpr &
    {
        static MatrixConfigExpr _config;
        return _config;
    }
};


namespace internal {

// Función constexpr para decidir el mejor backend según los tamaños
//constexpr auto determine_best_backend(size_t r1, size_t c1, size_t c2) -> MatrixConfigExpr::Product 
//{
//    // Coste aproximado de operaciones (O(N^3))
//    size_t total_ops = r1 * c1 * c2; 
//    size_t max_dim = std::max({r1, c1, c2});
//    size_t min_dim = std::min({r1, c1, c2});
//
//#ifdef TL_HAVE_CUDA
//    // Heurística CUDA: Solo rentable si las matrices son muy grandes (ej. 2400x2400)
//    // Evitamos CUDA si es muy alargada (min_dim pequeño) porque el PCIe no se satura bien
//    if (max_dim >= 2400 && min_dim >= 500) {
//        return MatrixConfigExpr::Product::CuBLAS;
//    }
//#endif
//
//#ifdef TL_HAVE_OPENBLAS
//    // Heurística BLAS: 
//    // Falla en matrices muy asimétricas (ej. 2400x24). Si el lado más pequeño es muy
//    // estrecho, el multithreading de OpenBLAS añade demasiado overhead.
//    if (min_dim >= 9 && max_dim / min_dim < 50) { 
//        return MatrixConfigExpr::Product::BLAS;
//    }
//#endif
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//    // Heurística SIMD: Todo lo que sea menor a 9x9 o muy asimétrico
//    return MatrixConfigExpr::Product::SIMD;
//#else
//    return MatrixConfigExpr::Product::CPP;
//#endif
//}

} // namespace internal

namespace detail
{


template<MatrixExpr A, MatrixExpr B>
auto select_backend(const A &a, const B &b) -> MatrixConfigExpr::Product
{
    auto cfg = MatrixConfigExpr::instance().product;

    // 1. Si el usuario forzó un modo, su palabra es la ley. 
    // Si forzó CuBLAS con enteros, el TL_ASSERT en mulmat le avisará de su error.
    if (cfg != MatrixConfigExpr::Product::AUTO) {
        return cfg;
    }

    // 2. Extraemos el tipo y comprobamos si es de coma flotante
    using T = typename matrix_traits<A>::value_type;
    constexpr bool is_float = std::floating_point<T>;

    size_t M = a.rows();
    size_t N = b.cols();
    size_t K = a.cols();

#ifdef TL_HAVE_CUDA
    // En AUTO, solo consideramos CUDA si el tipo es float/double
    if (is_float && M > 2000 && N > 2000 && K > 2000) {
        return MatrixConfigExpr::Product::CuBLAS;
    }
#endif

#ifdef TL_HAVE_OPENBLAS
    // En AUTO, solo consideramos BLAS si el tipo es float/double
    if (is_float && M > 32 && N > 32 && K > 32) {
        if ((M / N) < 10 && (N / M) < 10) {
            return MatrixConfigExpr::Product::BLAS;
        }
    }
#endif

#ifdef TL_HAVE_SIMD_INTRINSICS
    // Tu implementación SIMD seguramente soporte enteros (AVX2/AVX512 tienen buenas intrucciones enteras)
    // Así que aquí no filtramos por is_float.
    //if (M * N > 256) {
        return MatrixConfigExpr::Product::SIMD;
    //}
#else
        return MatrixConfigExpr::Product::CPP;
#endif
}

#ifdef TL_HAVE_SIMD_INTRINSICS

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd(const LHS &matrix1, const RHS &matrix2, OutMat &matrix)
{
    using T = typename matrix_traits<OutMat>::value_type;

    size_t rows = matrix1.rows();
    size_t dim = matrix1.cols();
    size_t cols = matrix2.cols();

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;
    Packed<T> packed_a1;
    Packed<T> packed_a2;
    Packed<T> packed_a3;
    Packed<T> packed_a4;
    Packed<T> packed_a5;
    Packed<T> packed_a6;
    Packed<T> packed_a7;
    Packed<T> packed_a8;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = cols - cols % packed_size;
    size_t iter = rows - rows % 8;

    T b{};

    for (size_t r = 0; r < iter; r += 8) {
        for (size_t i = 0; i < dim; i++) {

            packed_a1.setScalar(matrix1(r, i));
            packed_a2.setScalar(matrix1(r + 1, i));
            packed_a3.setScalar(matrix1(r + 2, i));
            packed_a4.setScalar(matrix1(r + 3, i));
            packed_a5.setScalar(matrix1(r + 4, i));
            packed_a6.setScalar(matrix1(r + 5, i));
            packed_a7.setScalar(matrix1(r + 6, i));
            packed_a8.setScalar(matrix1(r + 7, i));

            for (size_t c = 0; c < max_vector; c += packed_size) {

                packed_b.loadUnaligned(&matrix2(i, c));

                packed_c.loadUnaligned(&matrix(r, c));
                packed_c += packed_a1 * packed_b;
                packed_c.storeUnaligned(&matrix(r, c));

                packed_c.loadUnaligned(&matrix(r + 1, c));
                packed_c += packed_a2 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 1, c));

                packed_c.loadUnaligned(&matrix(r + 2, c));
                packed_c += packed_a3 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 2, c));

                packed_c.loadUnaligned(&matrix(r + 3, c));
                packed_c += packed_a4 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 3, c));

                packed_c.loadUnaligned(&matrix(r + 4, c));
                packed_c += packed_a5 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 4, c));

                packed_c.loadUnaligned(&matrix(r + 5, c));
                packed_c += packed_a6 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 5, c));

                packed_c.loadUnaligned(&matrix(r + 6, c));
                packed_c += packed_a7 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 6, c));

                packed_c.loadUnaligned(&matrix(r + 7, c));
                packed_c += packed_a8 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 7, c));

            }

            for (size_t c = max_vector; c < cols; c++) {

                b = matrix2(i, c);
                matrix(r, c) += matrix1(r, i) * b;
                matrix(r + 1, c) += matrix1(r + 1, i) * b;
                matrix(r + 2, c) += matrix1(r + 2, i) * b;
                matrix(r + 3, c) += matrix1(r + 3, i) * b;
                matrix(r + 4, c) += matrix1(r + 4, i) * b;
                matrix(r + 5, c) += matrix1(r + 5, i) * b;
                matrix(r + 6, c) += matrix1(r + 6, i) * b;
                matrix(r + 7, c) += matrix1(r + 7, i) * b;
            }

        }
    }

    for (size_t r = iter; r < rows; r++) {
        for (size_t i = 0; i < dim; i++) {

            T a = matrix1(r, i);
            packed_a.setScalar(a);

            for (size_t c = 0; c < max_vector; c += packed_size) {

                packed_b.loadUnaligned(&matrix2(i, c));

                packed_c.loadUnaligned(&matrix(r, c));
                packed_c += packed_a * packed_b;
                packed_c.storeUnaligned(&matrix(r, c));
            }

            for (size_t c = max_vector; c < cols; c++) {
                matrix(r, c) += a * matrix2(i, c);
            }

        }
    }
}


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
            // 🔥 ACUMULADORES EN REGISTROS (Cero accesos a RAM en el bucle interno)
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
            c_vec.loadUnaligned(&matrix(r, c)); c_vec += acc0; c_vec.storeUnaligned(&matrix(r, c));
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

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd_parallel(const LHS &matrix1, const RHS &matrix2, OutMat &matrix)
{
    using T = typename matrix_traits<OutMat>::value_type;

    size_t rows = matrix1.rows();
    size_t dim = matrix1.cols();
    size_t cols = matrix2.cols();

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = cols - cols % packed_size;
    size_t iter = rows - rows % 8;

    // Hacer pruebas con:
    // #pragma omp parallel for schedule(dynamic) private(packed_b, packed_c)
    // #pragma omp parallel for collapse(3) private(packed_b, packed_c)
    // #pragma omp parallel
    // {
    // #pragma omp for collapse(2)
    //#pragma omp parallel for collapse(2) private(packed_b, packed_c)
    #pragma omp parallel for private(packed_b, packed_c) 
    for (int r = 0; r < iter; r += 8) {
        for (int i = 0; i < dim; i++) {

            std::array<Packed<T>, 8> packed_a_array;
            for (int row_offset = 0; row_offset < 8; ++row_offset) {
                packed_a_array[row_offset].setScalar(matrix1(r + row_offset, i));
            }

            for (int c = 0; c < max_vector; c += packed_size) {
                packed_b.loadUnaligned(&matrix2(i, c));

                for (int row_offset = 0; row_offset < 8; ++row_offset) {
                    packed_c.loadUnaligned(&matrix(r + row_offset, c));
                    packed_c += packed_a_array[row_offset] * packed_b;
                    packed_c.storeUnaligned(&matrix(r + row_offset, c));
                }
            }

            for (size_t c = max_vector; c < cols; c++) {
                for (int row_offset = 0; row_offset < 8; ++row_offset) {
                    matrix(r + row_offset, c) += matrix1(r + row_offset, i) * matrix2(i, c);
                }
            }
        }
    }

    #pragma omp parallel for private(packed_a, packed_b, packed_c)
    for (int r = static_cast<int>(iter); r < static_cast<int>(rows); r++) {
        for (size_t i = 0; i < dim; i++) {

            T a = matrix1(r, i);
            packed_a.setScalar(a);

            for (size_t c = 0; c < max_vector; c += packed_size) {
                packed_b.loadUnaligned(&matrix2(i, c));
                packed_c.loadUnaligned(&matrix(r, c));
                packed_c += packed_a * packed_b;
                packed_c.storeUnaligned(&matrix(r, c));
            }

            for (size_t c = max_vector; c < cols; c++) {
                matrix(r, c) += a * matrix2(i, c);
            }
        }
    }
}
template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd_parallel_blocked(const LHS &A, const RHS &B, OutMat &C)
{
    using T = typename matrix_traits<OutMat>::value_type;

    const size_t rows = A.rows();
    const size_t dim = A.cols();
    const size_t cols = B.cols();

    Packed<T> packed_a, packed_b;
    constexpr size_t packed_size = Packed<T>().size();

    // 🔧 Ajusta según CPU (estos son seguros para empezar)
    constexpr size_t BLOCK_R = 64;
    constexpr size_t BLOCK_K = 32;
    constexpr size_t BLOCK_C = 64;

#pragma omp parallel for
    for (long long ii = 0; ii < rows; ii += BLOCK_R) {

        for (size_t kk = 0; kk < dim; kk += BLOCK_K) {

            for (size_t jj = 0; jj < cols; jj += BLOCK_C) {

                const size_t i_max = std::min(ii + BLOCK_R, rows);
                const size_t k_max = std::min(kk + BLOCK_K, dim);
                const size_t j_max = std::min(jj + BLOCK_C, cols);

                // Procesar bloques de 8 filas (tu kernel)
                size_t i = ii;
                for (; i + 7 < i_max; i += 8) {

                    size_t j = jj;
                    size_t j_vec_end = j_max - (j_max - j) % packed_size;

                    for (; j < j_vec_end; j += packed_size) {

                        // 🔥 acumuladores en registros
                        Packed<T> c[8];

                        for (int r = 0; r < 8; ++r) {
                            c[r].loadUnaligned(&C(i + r, j));
                        }

                        // bucle K bloqueado
                        for (size_t k = kk; k < k_max; ++k) {

                            packed_b.loadUnaligned(&B(k, j));

                            for (int r = 0; r < 8; ++r) {
                                packed_a.setScalar(A(i + r, k));
                                c[r] += packed_a * packed_b;
                            }
                        }

                        // 🔥 store una sola vez
                        for (int r = 0; r < 8; ++r) {
                            c[r].storeUnaligned(&C(i + r, j));
                        }
                    }

                    // resto columnas (no SIMD)
                    for (; j < j_max; ++j) {

                        T c[8] = {};

                        for (int r = 0; r < 8; ++r) {
                            c[r] = C(i + r, j);
                        }

                        for (size_t k = kk; k < k_max; ++k) {
                            T b = B(k, j);
                            for (int r = 0; r < 8; ++r) {
                                c[r] += A(i + r, k) * b;
                            }
                        }

                        for (int r = 0; r < 8; ++r) {
                            C(i + r, j) = c[r];
                        }
                    }
                }

                // 🔧 resto filas (no múltiplo de 8)
                for (; i < i_max; ++i) {

                    size_t j = jj;
                    size_t j_vec_end = j_max - (j_max - j) % packed_size;

                    for (; j < j_vec_end; j += packed_size) {

                        Packed<T> c;
                        c.loadUnaligned(&C(i, j));

                        for (size_t k = kk; k < k_max; ++k) {
                            packed_a.setScalar(A(i, k));
                            packed_b.loadUnaligned(&B(k, j));
                            c += packed_a * packed_b;
                        }

                        c.storeUnaligned(&C(i, j));
                    }

                    for (; j < j_max; ++j) {

                        T c = C(i, j);

                        for (size_t k = kk; k < k_max; ++k) {
                            c += A(i, k) * B(k, j);
                        }

                        C(i, j) = c;
                    }
                }
            }
        }
    }
}

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd_parallel_microkernel(const LHS &A, const RHS &B, OutMat &C)
{
    using T = typename matrix_traits<OutMat>::value_type;

    const size_t rows = A.rows();
    const size_t dim = A.cols();
    const size_t cols = B.cols();

    constexpr size_t MR = 8;
    constexpr size_t PS = Packed<T>().size();

    constexpr size_t BLOCK_R = 128;
    constexpr size_t BLOCK_K = 128;
    constexpr size_t BLOCK_C = 128;

#pragma omp parallel for
    for (size_t ii = 0; ii < rows; ii += BLOCK_R) {

        for (size_t kk = 0; kk < dim; kk += BLOCK_K) {

            const size_t i_max = std::min(ii + BLOCK_R, rows);
            const size_t k_max = std::min(kk + BLOCK_K, dim);

            for (size_t jj = 0; jj < cols; jj += BLOCK_C) {

                const size_t j_max = std::min(jj + BLOCK_C, cols);

                // 🔥 recorrer bloques de 8 filas
                for (size_t i = ii; i + MR <= i_max; i += MR) {

                    /// Esto es absurdo
                    // 🔥 PACKING DE A (bloque MR x K)
                    std::vector<T> Apack(MR * (k_max - kk));

                    for (size_t k = kk; k < k_max; ++k) {
                        for (size_t r = 0; r < MR; ++r) {
                            Apack[r * (k_max - kk) + (k - kk)] = A(i + r, k);
                        }
                    }

                    size_t j = jj;
                    size_t j_vec_end = j_max - (j_max - j) % PS;

                    for (; j < j_vec_end; j += PS) {

                        // 🔥 acumuladores SIMD en registros
                        Packed<T> c0, c1, c2, c3, c4, c5, c6, c7;

                        c0.loadUnaligned(&C(i + 0, j));
                        c1.loadUnaligned(&C(i + 1, j));
                        c2.loadUnaligned(&C(i + 2, j));
                        c3.loadUnaligned(&C(i + 3, j));
                        c4.loadUnaligned(&C(i + 4, j));
                        c5.loadUnaligned(&C(i + 5, j));
                        c6.loadUnaligned(&C(i + 6, j));
                        c7.loadUnaligned(&C(i + 7, j));

                        for (size_t k = 0; k < (k_max - kk); ++k) {

                            Packed<T> b;
                            b.loadUnaligned(&B(kk + k, j));

                            Packed<T> a;
                            a.setScalar(Apack[0 * (k_max - kk) + k]); c0 += a * b;
                            a.setScalar(Apack[1 * (k_max - kk) + k]); c1 += a * b;
                            a.setScalar(Apack[2 * (k_max - kk) + k]); c2 += a * b;
                            a.setScalar(Apack[3 * (k_max - kk) + k]); c3 += a * b;
                            a.setScalar(Apack[4 * (k_max - kk) + k]); c4 += a * b;
                            a.setScalar(Apack[5 * (k_max - kk) + k]); c5 += a * b;
                            a.setScalar(Apack[6 * (k_max - kk) + k]); c6 += a * b;
                            a.setScalar(Apack[7 * (k_max - kk) + k]); c7 += a * b;
                        }

                        c0.storeUnaligned(&C(i + 0, j));
                        c1.storeUnaligned(&C(i + 1, j));
                        c2.storeUnaligned(&C(i + 2, j));
                        c3.storeUnaligned(&C(i + 3, j));
                        c4.storeUnaligned(&C(i + 4, j));
                        c5.storeUnaligned(&C(i + 5, j));
                        c6.storeUnaligned(&C(i + 6, j));
                        c7.storeUnaligned(&C(i + 7, j));
                    }

                    // 🔧 resto columnas
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

                // 🔧 resto filas
                for (size_t i = (i_max / MR) * MR; i < i_max; ++i) {

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


template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd_kernel_opt(const LHS &A, const RHS &B, OutMat &C)
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

#pragma omp parallel for
    for (size_t ii = 0; ii < rows; ii += BLOCK_R) {

        for (size_t kk = 0; kk < dim; kk += BLOCK_K) {

            for (size_t jj = 0; jj < cols; jj += BLOCK_C) {

                const size_t i_max = std::min(ii + BLOCK_R, rows);
                const size_t k_max = std::min(kk + BLOCK_K, dim);
                const size_t j_max = std::min(jj + BLOCK_C, cols);

                for (size_t i = ii; i + MR <= i_max; i += MR) {

                    size_t j = jj;
                    size_t j_vec_end = j_max - (j_max - j) % PS;

                    for (; j < j_vec_end; j += PS) {

                        // 🔥 acumuladores en registros
                        Packed<T> c0, c1, c2, c3, c4, c5, c6, c7;

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
                            b.loadUnaligned(&B(k, j));

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

                        c0.storeUnaligned(&C(i + 0, j));
                        c1.storeUnaligned(&C(i + 1, j));
                        c2.storeUnaligned(&C(i + 2, j));
                        c3.storeUnaligned(&C(i + 3, j));
                        c4.storeUnaligned(&C(i + 4, j));
                        c5.storeUnaligned(&C(i + 5, j));
                        c6.storeUnaligned(&C(i + 6, j));
                        c7.storeUnaligned(&C(i + 7, j));
                    }

                    // resto columnas
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

                // resto filas
                for (size_t i = (i_max / MR) * MR; i < i_max; ++i) {

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

template <typename Mat, typename T>
void pack_A(const Mat &A, size_t ii, size_t kk, size_t cur_M, size_t cur_K, size_t MR, T *Apack)
{
    size_t idx = 0;
    // Iteramos en paneles de alto MR
    for (size_t i = 0; i < cur_M; i += MR) {
        size_t m_panel = std::min(MR, cur_M - i);
        for (size_t k = 0; k < cur_K; ++k) {
            // Copiamos los elementos reales
            for (size_t r = 0; r < m_panel; ++r) {
                Apack[idx++] = A(ii + i + r, kk + k);
            }
            // Relleno de ceros para los bordes (Padding)
            for (size_t r = m_panel; r < MR; ++r) {
                Apack[idx++] = T(0);
            }
        }
    }
}

template <typename Mat, typename T>
void pack_B(const Mat &B, size_t kk, size_t jj, size_t cur_K, size_t cur_N, size_t PS, T *Bpack)
{
    size_t idx = 0;
    // Iteramos en paneles de ancho PS (tu vector)
    for (size_t j = 0; j < cur_N; j += PS) {
        size_t n_panel = std::min(PS, cur_N - j);
        for (size_t k = 0; k < cur_K; ++k) {
            // Copiamos los elementos reales
            for (size_t c = 0; c < n_panel; ++c) {
                Bpack[idx++] = B(kk + k, jj + j + c);
            }
            // Relleno de ceros para los bordes (Padding)
            for (size_t c = n_panel; c < PS; ++c) {
                Bpack[idx++] = T(0);
            }
        }
    }
}

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_simd_kernel_opt2(const LHS &A, const RHS &B, OutMat &C)
{
    using T = typename matrix_traits<OutMat>::value_type;

    // ¿constexpr?. Si el tamaño de las matrices es conocido en tiempo de compilación, sí. Si no, no.
    const size_t rows = A.rows();
    const size_t dim = A.cols();
    const size_t cols = B.cols();

    // CONFIGURACIÓN DEL MICRO-KERNEL
    constexpr size_t MR = 8; // MicroKernelConfig<T>::MR;
    constexpr size_t NR = MicroKernelConfig<T>::NR;
    constexpr size_t PS = PackedTraits<Packed<T>>::size;

    // CONFIGURACIÓN DE BLOQUES (ajustar según CPU/cache)
    // Mejor que sea un singleton
    static const CacheSizes system_caches = systemCacheSizes();
    static const MacroKernelSizes blocks = calculateBlockSizes<T, MR, NR>(system_caches);

    //const size_t BLOCK_M = blocks.Mc;
    //const size_t BLOCK_K = blocks.Kc;
    //const size_t BLOCK_N = blocks.Nc;
    constexpr size_t BLOCK_M = 128;
    constexpr size_t BLOCK_K = 64;
    constexpr size_t BLOCK_N = 128;

#pragma omp parallel
    {
        // unique_ptr con new T[] NO inicializa a cero (¡0 ms de sobrecarga!)
        // Sumamos un pequeño extra (+ MR/PS) por si acaso en el padding
        std::unique_ptr<T[]> Apack_ptr(new T[(BLOCK_M + MR) * BLOCK_K]);
        std::unique_ptr<T[]> Bpack_ptr(new T[BLOCK_K * (BLOCK_N + PS)]);

        T *Apack = Apack_ptr.get();
        T *Bpack = Bpack_ptr.get();

        // Repartimos por columnas (Ahora sí habrá iteraciones suficientes)
#pragma omp for schedule(dynamic)
        for (long long jj = 0; jj < cols; jj += BLOCK_N) {
            const size_t cur_N = std::min(BLOCK_N, cols - jj);

            for (size_t kk = 0; kk < dim; kk += BLOCK_K) {
                const size_t cur_K = std::min(BLOCK_K, dim - kk);

                pack_B(B, kk, jj, cur_K, cur_N, PS, Bpack);

                for (long long ii = 0; ii < rows; ii += BLOCK_M) {
                    const size_t cur_M = std::min(BLOCK_M, rows - ii);

                    pack_A(A, ii, kk, cur_M, cur_K, MR, Apack);

                    // --- INICIO DEL MICRO-KERNEL ---
                    for (size_t i = 0; i < cur_M; i += MR) {
                        for (size_t j = 0; j < cur_N; j += PS) {

                            const T *a_ptr = &Apack[i * cur_K];
                            const T *b_ptr = &Bpack[j * cur_K];

                            Packed<T> c0(0), c1(0), c2(0), c3(0), c4(0), c5(0), c6(0), c7(0);

                            for (size_t k = 0; k < cur_K; ++k) {
                                Packed<T> b;
                                b.loadUnaligned(b_ptr);
                                b_ptr += PS;

                                Packed<T> a;
                                a.setScalar(a_ptr[0]); c0 += a * b;
                                a.setScalar(a_ptr[1]); c1 += a * b;
                                a.setScalar(a_ptr[2]); c2 += a * b;
                                a.setScalar(a_ptr[3]); c3 += a * b;
                                a.setScalar(a_ptr[4]); c4 += a * b;
                                a.setScalar(a_ptr[5]); c5 += a * b;
                                a.setScalar(a_ptr[6]); c6 += a * b;
                                a.setScalar(a_ptr[7]); c7 += a * b;

                                a_ptr += MR;
                            }

                            // --- ESCRITURA EN C (Con protección de bordes) ---
                            const size_t c_row = ii + i;
                            const size_t c_col = jj + j;

                            if (c_row + MR <= rows && c_col + PS <= cols) {
                                Packed<T> temp;
                                temp.loadUnaligned(&C(c_row + 0, c_col)); c0 += temp; c0.storeUnaligned(&C(c_row + 0, c_col));
                                temp.loadUnaligned(&C(c_row + 1, c_col)); c1 += temp; c1.storeUnaligned(&C(c_row + 1, c_col));
                                temp.loadUnaligned(&C(c_row + 2, c_col)); c2 += temp; c2.storeUnaligned(&C(c_row + 2, c_col));
                                temp.loadUnaligned(&C(c_row + 3, c_col)); c3 += temp; c3.storeUnaligned(&C(c_row + 3, c_col));
                                temp.loadUnaligned(&C(c_row + 4, c_col)); c4 += temp; c4.storeUnaligned(&C(c_row + 4, c_col));
                                temp.loadUnaligned(&C(c_row + 5, c_col)); c5 += temp; c5.storeUnaligned(&C(c_row + 5, c_col));
                                temp.loadUnaligned(&C(c_row + 6, c_col)); c6 += temp; c6.storeUnaligned(&C(c_row + 6, c_col));
                                temp.loadUnaligned(&C(c_row + 7, c_col)); c7 += temp; c7.storeUnaligned(&C(c_row + 7, c_col));
                            } else {
                                T buf0[PS], buf1[PS], buf2[PS], buf3[PS], buf4[PS], buf5[PS], buf6[PS], buf7[PS];
                                c0.storeUnaligned(buf0); c1.storeUnaligned(buf1);
                                c2.storeUnaligned(buf2); c3.storeUnaligned(buf3);
                                c4.storeUnaligned(buf4); c5.storeUnaligned(buf5);
                                c6.storeUnaligned(buf6); c7.storeUnaligned(buf7);

                                const size_t valid_rows = std::min(MR, rows - c_row);
                                const size_t valid_cols = std::min(PS, cols - c_col);
                                T *buffers[8] = {buf0, buf1, buf2, buf3, buf4, buf5, buf6, buf7};

                                for (size_t r = 0; r < valid_rows; ++r) {
                                    for (size_t c = 0; c < valid_cols; ++c) {
                                        C(c_row + r, c_col + c) += buffers[r][c];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

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

    // 🔥 COLLAPSE(2): Creamos una malla 2D de tareas (Filas x Columnas)
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

#endif // TL_HAVE_SIMD_INTRINSICS

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_cpp(const LHS &matrix1, const RHS &matrix2, OutMat &matrix)
{
    using T = typename matrix_traits<OutMat>::value_type;

    for (size_t r = 0; r < matrix1.rows(); r++) {
        for (size_t i = 0; i < matrix1.cols(); i++) {
            T a = matrix1(r, i);
            for (size_t c = 0; c < matrix2.cols(); c++) {
                matrix(r, c) += a * matrix2(i, c);
            }
        }
    }
}

#ifdef TL_HAVE_OPENBLAS

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_blas(const LHS &matrix1, const RHS &matrix2, OutMat &matrix)
{
    using T = typename matrix_traits<OutMat>::value_type;

    bool mat1_is_symmetric = matrix1.isSymmetric();
    bool mat2_is_symmetric = matrix2.isSymmetric();
    bool mat1_is_upper = matrix1.isUpperTriangular();
    bool mat1_is_lower = matrix1.isLowerTriangular();
    bool mat2_is_upper = matrix2.isUpperTriangular();
    bool mat2_is_lower = matrix2.isLowerTriangular();
    bool mat1_is_triangular = mat1_is_upper || mat1_is_lower;
    bool mat2_is_triangular = mat2_is_upper || mat2_is_lower;

    //if (matrix1.rows() == matrix1.cols() && (mat1_is_symmetric || mat2_is_symmetric)) {
        //T alpha = 1.0;
        //T beta = 0.0;
        //auto lda = matrix1.rows();
        //auto ldb = matrix2.cols();
        //auto ldc = matrix2.cols();
    //    blas::symm(blas::Order::row_major, 
    //               mat1_is_symmetric ? blas::Side::left : blas::Side::right,
    //               blas::TriangularForm::upper,
    //               matrix1.rows(), 
    //               matrix2.cols(), 
    //               alpha,
    //               matrix1.data(), lda
    //               matrix2.data(), ldb,
    //               beta,
    //               matrix.data(), ldc);

    //} else if (matrix1.rows() == matrix1.cols() && (mat1_is_triangular || mat2_is_triangular)) {

    //    blas::Side side;
    //    blas::TriangularForm form;

    //    if (mat1_is_triangular) {
    //        side = blas::Side::left;
    //        form = mat1_is_upper ? blas::TriangularForm::upper : blas::TriangularForm::lower;
    //    } else {
    //        side = blas::Side::right;
    //        form = mat1_is_upper ? blas::TriangularForm::upper : blas::TriangularForm::lower;
    //    }

    //    T alpha = 1.0;
    //    auto lda = m;
    //    auto ldb = n;
    //    matrix = matrix2;
    //    blas::trmm(blas::Order::row_major, 
    //               side, 
    //               form, 
    //               blas::TransposeMode::no_transpose,  
    //               matrix1.rows(),
    //               matrix2.cols(),
    //               alpha,
    //               matrix1.data(), lda,
    //               matrix.data(), ldb);

    //} else {
    T alpha = 1.;
    T beta = 0.;
    auto lda = matrix1.cols();
    auto ldb = matrix2.cols();
    auto ldc = matrix2.cols();
    blas::gemm(blas::Order::row_major,
        blas::TransposeMode::no_transpose,
        blas::TransposeMode::no_transpose,
        matrix1.rows(),
        matrix2.cols(),
        matrix1.cols(),
        alpha,
        matrix1.data(), lda,
        matrix2.data(), ldb,
        beta,
        matrix.data(), ldc);
    //}
}

#endif // TL_HAVE_OPENBLAS


template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat(const LHS &lhs, const RHS &rhs, OutMat &out)
{
    TL_ASSERT(lhs.cols() == rhs.rows(), "A cols != B rows");
    TL_ASSERT(lhs.rows() == out.rows(), "C rows != A rows");
    TL_ASSERT(rhs.cols() == out.cols(), "B cols != C cols");

    using T = typename matrix_traits<OutMat>::value_type;
    auto backend = select_backend(lhs, rhs);

    switch (backend) {

#ifdef TL_HAVE_CUDA
    case MatrixConfigExpr::Product::CuBLAS:
    {
        if constexpr (std::floating_point<T>) {
            decltype(auto) a = require_physical_memory(lhs);
            decltype(auto) b = require_physical_memory(rhs);
            cuda::gemm(a.rows(), b.cols(), a.cols(),
                a.data(), b.data(), out.data());
        } else {
            TL_ASSERT(false, "CuBLAS no soporta tipos enteros");
        }
        break;
    }
#endif

#ifdef TL_HAVE_OPENBLAS
    case MatrixConfigExpr::Product::BLAS:
    {
        if constexpr (std::floating_point<T>) {
            decltype(auto) a = require_physical_memory(lhs);
            decltype(auto) b = require_physical_memory(rhs);
            mulmat_blas(a, b, out);
        } else {
            TL_ASSERT(false, "OpenBLAS no soporta tipos enteros");
        }
        break;
    }
#endif

#ifdef TL_HAVE_SIMD_INTRINSICS
    case MatrixConfigExpr::Product::SIMD:
    {
        //if (out.rows() * out.cols() > 1000) {
        decltype(auto) a = require_linear_access(lhs);
        decltype(auto) b = require_linear_access(rhs);
        if (a.rows() * a.cols() * b.cols() > 1000000) {
            //detail::mulmat_simd_parallel(lhs, rhs, out);
            //detail::mulmat_simd_parallel_blocked(lhs, rhs, out);
            //detail::mulmat_simd_parallel_microkernel(lhs, rhs, out);
            //detail::mulmat_simd_kernel_opt(lhs, rhs, out);
            //detail::mulmat_simd_kernel_opt2(lhs, rhs, out);
            mulmat_simd_kernel_opt3(a, b, out);
        } else {
            //detail::mulmat_simd(lhs, rhs, out);
            //detail::mulmat_simd2(lhs, rhs, out);
            mulmat_simd3(a, b, out);
        }
        break;
    }
#endif

    case MatrixConfigExpr::Product::CPP:
    default:
        mulmat_cpp(lhs, rhs, out);
        break;
    }
}

} // namespace detail

template<typename T, size_t Rows = DynamicData, size_t Cols = DynamicData>
class Matrix
  : public MatrixBase<Matrix<T, Rows, Cols>>
{

    static_assert(Arithmetic<T>, "Matrix requires an arithmetic type (integral or floating-point)");
    static_assert(!std::is_const_v<T>, "Matrix cannot hold const types. Use 'const Matrix<T>' instead.");

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    enum data
    {
        size = DataSize<T, Rows, Cols>::size()
    };

public:

    /*!
     * \brief Default constructor
     */
    Matrix();

    /*!
     * \brief Row-column constructor
     * \param[in] rows Matrix rows
     * \param[in] cols Matrix columns
     */
    Matrix(size_t rows, size_t cols);

    /*!
     * \brief Rows, columns and value constructor
     * \param[in] rows Matrix rows
     * \param[in] cols Matrix columns
     * \param[in] value Matrix value
     */
    Matrix(size_t rows, size_t cols, T value);

    /*!
     * \brief Copy constructor
     * \param[in] mat Matrix object being copied
     */
    Matrix(const Matrix &mat);

    //template<typename MatrixDerived>
    //Matrix(const MatrixDerived &matrix);

    /*!
     * \brief Move constructor
     * \param[in] mat Matrix object being moved
     */
    Matrix(Matrix &&mat) TL_NOEXCEPT;

    /*!
     * \brief Constructor using initializer list for one-dimensional matrix
     * \param[in] values Initializer list containing values for the matrix
     */
    Matrix(std::initializer_list<T> values);

    /*!
     * \brief Constructor using initializer list for two-dimensional matrix
     * \param[in] values Initializer list containing lists of values for the matrix
     */
    Matrix(std::initializer_list<std::initializer_list<T>> values);

    /*!
     * \brief Constructor using raw data for matrix initialization
     * \param[in] data Pointer to the raw data
     * \param[in] rows Number of rows in the matrix
     * \param[in] cols Number of columns in the matrix
     */
    Matrix(const T *data, size_t rows, size_t cols);

    template<MatrixExpr Expr>
    Matrix(const Expr& expr) 
        : mData(Data<T, data::size>(expr.rows() * expr.cols())),
          mRows(Rows == DynamicData ? expr.rows() : Rows),
          mCols(Cols == DynamicData ? expr.cols() : Cols)
    {

        TL_ASSERT(expr.rows() == mRows &&
                  expr.cols() == mCols, "Static matrix cannot be resized");

        *this = expr;
    }
	
    /*!
     * \brief Copy assignment operator
     * \param[in] matrix Object being copied
     * \return Reference to the assigned object
     */
    auto operator=(const Matrix &matrix) -> Matrix &
    {
        if (this != &matrix) {
            this->mData = matrix.mData;
            this->mRows = matrix.mRows;
            this->mCols = matrix.mCols;
        }

        return *this;
    }

    /*!
     * \brief Move assignment operator
     * \param[in] matrix Object being moved
     * \return Reference to the assigned object
     */
    auto operator=(Matrix &&matrix) noexcept -> Matrix &
    {
        if (this != &matrix) {
            this->mData = std::move(matrix.mData);
            this->mRows = std::move(matrix.mRows);
            this->mCols = std::move(matrix.mCols);
        }

        return *this;
    }

    template<MatrixExpr Expr>
    auto operator=(const Expr &expr) -> Matrix& 
    {
        if constexpr (is_matrix_product_v<std::remove_cvref_t<Expr>>) {

            const auto &lhs = expr.lhs();
            const auto &rhs = expr.rhs();

            if (expr.aliases(this->data())) {

                Matrix<T, Rows, Cols> tmp = Matrix<T, Rows, Cols>::zero(lhs.rows(), rhs.cols());

                detail::mulmat(lhs, rhs, tmp);
                *this = std::move(tmp);

            } else {
                
                if constexpr (Rows == DynamicData || Cols == DynamicData) {
                    if (this->rows() != expr.rows() || this->cols() != expr.cols()) {
                        this->resize(expr.rows(), expr.cols());
                    }
                } else {
                    TL_ASSERT(expr.rows() == mRows && expr.cols() == mCols, "Static matrix cannot be resized");
                }
                
                this->fill(0);

                detail::mulmat(lhs, rhs, *this);
            }

            return *this;

        } else {

            if constexpr (Rows == DynamicData || Cols == DynamicData) {
                if (this->rows() != expr.rows() || this->cols() != expr.cols()) {
                    this->resize(expr.rows(), expr.cols());
                }
            } else {
                TL_ASSERT(expr.rows() == mRows && expr.cols() == mCols, "Static matrix cannot be resized");
            }

            if constexpr (!matrix_traits<Expr>::is_element_wise) {
                if (expr.aliases(this->data())) {
                    Matrix<T, Rows, Cols> tmp = expr;
                    *this = std::move(tmp);
                    return *this;
                }
            }

            size_t size = mRows * mCols;
            size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

            if constexpr (matrix_traits<Expr>::has_contiguous_memory) {
                constexpr size_t packed_size = Packed<T>::size();
                size_t max_size = size - (size % packed_size);

                for (; i < max_size; i += packed_size) {
                    Packed<T> result_packet = expr.packet(i);
                    result_packet.storeUnaligned(&this->data()[i]);
                }
            }
#endif

            for (; i < size; ++i) {
                (*this)(i) = expr(i);
            }

            return *this;
        }
    }	

    /*!
     * \brief Number of rows in the matrix
     * \return Number of rows
     */
    constexpr auto rows() const noexcept -> size_t
    {
        return mRows;
    }

    /*!
     * \brief Number of columns in the matrix
     * \return Number of columns
     */
    constexpr auto cols() const noexcept -> size_t
    {
        return mCols;
    }

    /*!
     * \brief Swap two rows
     * \param[in] i First row to swap
     * \param[in] j Second row to swap
     */
    void swapRows(size_t i, size_t j)
    {
        for (size_t c = 0; c < mCols; c++) {
            std::swap(mData[i * mCols + c], mData[j * mCols + c]);
        }
    }

    /*!
     *\brief Swap two cols
     *\param[in] i First cols to swap
     *\param[in] j Second cols to swap
     */
    void swapCols(size_t i, size_t j)
    {
        for (size_t r = 0; r < mRows; r++) {
            std::swap(mData[r * mCols + i], mData[r * mCols + j]);
        }
    }

    /*!
     * \brief Reference to the element at position (r, c)
     * \param[in] r Row of the matrix
     * \param[in] c Column of the matrix
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix.at(0, 0) = 1.5;
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) -> reference
    {
        return mData.at(r * mCols + c);
    }

    /*!
     * \brief Constant reference to the element at position (r, c)
     * \param[in] r Row
     * \param[in] c Column
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) const -> const_reference
    {
        return mData.at(r * mCols + c);
    }

    /*!
     * \brief Reference to the element at position (r, c)
     * \param[in] r Row of the matrix
     * \param[in] c Column of the matrix
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix(0, 0) = 1.5;
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t r, size_t c) noexcept -> reference
    {
        return mData[r * mCols + c];
    }

    /*!
     * \brief Constant reference to the element at position (r, c)
     * \param[in] r Row
     * \param[in] c Column
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t r, size_t c) const noexcept -> const_reference
    {
        return mData[r * mCols + c];
    }

    /*!
     * \brief Reference to the element
     * The position of the element is determined as:
     *   r * this->cols() + c
     * \param[in] position Position of the matrix element
     * \return Value of the matrix at the specified position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) noexcept -> reference
    {
        return mData[position];
    }

    /*!
     * \brief Constant reference to the element
     * The position of the element is determined as:
     *   r * this->cols() + c
     * \param[in] position Position of the matrix element
     * \return Value of the matrix at the specified position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) const noexcept -> const_reference
    {
        return mData[position];
    }

    //auto block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) -> MatrixBlock<T, DynamicData, DynamicData>
    //{
    //    TL_ASSERT(iniRow >= 0 && endRow >= iniRow && endRow < this->rows() &&
    //              iniCol >= 0 && endCol >= iniCol && endCol < this->cols(), "Matrix block out of range");

    //    size_t blockRows = endRow - iniRow + 1;
    //    size_t blockCols = endCol - iniCol + 1;

    //    return MatrixBlock<T, DynamicData, DynamicData>(this->data(), this->cols(), iniRow, iniCol, blockRows, blockCols);
    //}

    //auto block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) const -> const MatrixBlock<const T, DynamicData, DynamicData>
    //{
    //    TL_ASSERT(iniRow >= 0 && endRow >= iniRow && endRow < this->rows() &&
    //              iniCol >= 0 && endCol >= iniCol && endCol < this->cols(), "Matrix block out of range");

    //    size_t blockRows = endRow - iniRow + 1;
    //    size_t blockCols = endCol - iniCol + 1;

    //    return MatrixBlock<const T, DynamicData, DynamicData>(this->data(), this->cols(), iniRow, iniCol, blockRows, blockCols);
    //}

    auto block_(size_t startRow, size_t startCol, size_t numRows, size_t numCols) -> MatrixBlock<T, DynamicData, DynamicData>
    {
        TL_ASSERT(startRow + numRows <= this->rows() &&
                  startCol + numCols <= this->cols(), "Matrix block out of range");

        // Ya no hay que calcular nada, los parámetros van directos al constructor
        return MatrixBlock<T, DynamicData, DynamicData>(this->data(), this->cols(), startRow, startCol, numRows, numCols);
    }

    auto block_(size_t startRow, size_t startCol, size_t numRows, size_t numCols) const -> const MatrixBlock<const T, DynamicData, DynamicData>
    {
        TL_ASSERT(startRow + numRows <= this->rows() &&
                  startCol + numCols <= this->cols(), "Matrix block out of range");

        return MatrixBlock<const T, DynamicData, DynamicData>(this->data(), this->cols(), startRow, startCol, numRows, numCols);
    }

    auto row(size_t r) -> MatrixRow<T> { return MatrixRow<T>(this->data(), r, this->cols()); }
    auto row(size_t r) const -> const MatrixRow<const T> { return MatrixRow<const T>(this->data(), r, this->cols()); }

    auto col(size_t c) -> MatrixCol<T> { return MatrixCol<T>(this->data(), c, this->rows(), this->cols()); }
    auto col(size_t c) const -> const MatrixCol<const T> { return MatrixCol<const T>(this->data(), c, this->rows(), this->cols()); }

    //auto rowBlock(size_t iniRow, size_t endRow) -> auto { return block(iniRow, endRow, 0, this->cols() - 1); }
    //auto rowBlock(size_t iniRow, size_t endRow) const -> auto { return block(iniRow, endRow, 0, this->cols() - 1); }

    //auto colBlock(size_t iniCol, size_t endCol) -> auto { return block(0, this->rows() - 1, iniCol, endCol); }
    //auto colBlock(size_t iniCol, size_t endCol) const -> auto { return block(0, this->rows() - 1, iniCol, endCol); }

    auto operator[](size_t r) -> MatrixRow<T> { return row(r); }
    auto operator[](size_t r) const -> const MatrixRow<const T> { return row(r); }

    /*!
     * \brief Constructs a matrix of zeros
     * \f[
     * A=\begin{bmatrix}
     * 0 & 0 & 0 \\
     * 0 & 0 & 0 \\
     * 0 & 0 & 0 \\
     * \end{bmatrix}
     * \f]
     * \return
     */
    static auto zero(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        return Matrix<T, Rows, Cols>(rows == 0 ? Rows : rows, cols == 0 ? Cols : cols, consts::zero<T>);
    }

    /*!
     * \brief Constructs a matrix of 'ones'
     * \f[
     * A=\begin{bmatrix}
     * 1 & 1 & 1 \\
     * 1 & 1 & 1 \\
     * 1 & 1 & 1 \\
     * \end{bmatrix}
     * \f]
     * \return
     */
    static auto ones(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        return Matrix<T, Rows, Cols>(rows == 0 ? Rows : rows, cols == 0 ? Cols : cols, consts::one<T>);
    }

    /*!
     * \brief Constructs the identity matrix
     * \f[
     * A=\begin{bmatrix}
     * 1 & 0 & 0 \\
     * 0 & 1 & 0 \\
     * 0 & 0 & 1 \\
     * \end{bmatrix}
     * \f]
     * \return
     */
    static auto identity(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        Matrix<T, Rows, Cols> matrix(rows, cols);

        for (size_t r = 0; r < matrix.rows(); r++) {
            for (size_t c = 0; c < matrix.cols(); c++) {
                if (r == c) {
                    matrix(r, c) = consts::one<T>;
                } else {
                    matrix(r, c) = consts::zero<T>;
                }
            }
        }

        return matrix;
    }

    /*!
     * \brief Constructs a matrix with random values
     */
    static auto randon(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        Matrix<T, Rows, Cols> matrix(rows, cols);

        static thread_local std::mt19937 random_number_engine(std::random_device{}());
        std::uniform_real_distribution<> distribution(0.0, 99.0);

        size_t size = matrix.rows() * matrix.cols();

        for (size_t i = 0; i < size; ++i) {
            matrix(i) = static_cast<T>(distribution(random_number_engine));
        }

        return matrix;
    }

    auto data() noexcept -> pointer
    {
        return mData.data();
    }

    auto data() const noexcept -> const_pointer
    {
        return mData.data();
    }

	auto aliases(const void* ptr) const -> bool 
    {
        return static_cast<const void*>(this->data()) == ptr;
    }

    auto packet(size_t i) const noexcept -> Packed<T>
    {
        Packed<T> p;
        p.loadUnaligned(&this->data()[i]);
        return p;
    }

    void resize(size_t rows, size_t cols)
        requires (Rows == DynamicData || Cols == DynamicData)
    {
        if (mRows == rows && mCols == cols) {
            return;
        }

        mRows = rows;
        mCols = cols;

        mData.resize(mRows * mCols);// = Data<T, data::size>(mRows * mCols);
    }

    void fill(T value)
    {
        std::fill(mData.begin(), mData.end(), value);
    }

private:

    Data<T, size> mData;
    size_t mRows{Rows};
    size_t mCols{Cols};
};


/* Matrix alias */

using Matrix2x2i = Matrix<int, 2, 2>;
using Matrix2x2f = Matrix<float, 2, 2>;
using Matrix2x2d = Matrix<double, 2, 2>;
using Matrix3x3i = Matrix<int, 3, 3>;
using Matrix3x3f = Matrix<float, 3, 3>;
using Matrix3x3d = Matrix<double, 3, 3>;
using Matrix4x4i = Matrix<int, 4, 4>;
using Matrix4x4f = Matrix<float, 4, 4>;
using Matrix4x4d = Matrix<double, 4, 4>;




/* Matrix implementation */

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix()
  : mData(Data<T, data::size>()),
    mRows(Rows == DynamicData ? 0 : Rows),
    mCols(Cols == DynamicData ? 0 : Cols)
{

}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(size_t rows, size_t cols)
  : mData(Data<T, data::size>(rows *cols)),
    mRows(Rows == DynamicData ? rows : Rows),
    mCols(Cols == DynamicData ? cols : Cols)
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(size_t rows, size_t cols, T value)
  : mData(Data<T, data::size>(rows *cols, value)),
    mRows(Rows == DynamicData ? rows : Rows),
    mCols(Cols == DynamicData ? cols : Cols)
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(const Matrix &mat)
  : mData(mat.mData),
    mRows(mat.mRows),
    mCols(mat.mCols)
{
}


//template<typename T, size_t Rows, size_t Cols>
//template<typename MatrixDerived>
//Matrix<T, Rows, Cols>::Matrix(const MatrixDerived &matrix)
//{
//    if (Rows == DynamicData && Cols == DynamicData) {
//        mData = Data<T, data::size>(matrix.rows() * matrix.cols());
//        mRows = matrix.rows();
//        mCols = matrix.cols();
//    }
//
//    TL_ASSERT(matrix.rows() == this->rows() &&
//        matrix.cols() == this->cols(), "Static matrix cannot be resized");
//
//    MatrixBase<Matrix<T, Rows, Cols>>::set(matrix);
//}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(Matrix &&mat) noexcept
  : mData(std::move(mat.mData)),
    mRows(std::move(mat.mRows)),
    mCols(std::move(mat.mCols))
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(std::initializer_list<T> values)
{
    if (Rows == DynamicData && Cols == DynamicData) {

        this->mRows = 1;
        this->mCols = values.size();
        mData = Data<T, data::size>(values.size());
        std::copy(values.begin(), values.end(), mData.begin());

    } else {

        mData = Data<T, data::size>();
        this->mRows = Rows;
        this->mCols = Cols;

        size_t n = values.size();
        if (n == data::size) {
            std::copy(values.begin(), values.end(), mData.begin());
        } else if (n < data::size) {
            std::copy(values.begin(), values.end(), mData.begin());
            std::fill(mData.begin() + n, mData.end(), consts::zero<T>);
        } else {
            std::copy(values.begin(), values.begin() + data::size, mData.begin());
        }
    }
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(std::initializer_list<std::initializer_list<T>> values)
{
    if (Rows == DynamicData && Cols == DynamicData) {

        this->mRows = values.size();
        auto it = values.begin();
        this->mCols = it->size();
        mData = Data<T, data::size>(this->mRows * this->mCols);

        auto it_data = mData.begin();
        for (auto it = values.begin(); it != values.end(); it++) {
            size_t n = it->size();
            if (n == mCols) {
                std::copy(it->begin(), it->end(), it_data);
            } else if (n < mCols) {
                std::copy(it->begin(), it->end(), it_data);
                std::fill(it_data + n, mData.end(), consts::zero<T>);
            } else {
                std::copy(it->begin(), it->end() + mCols, it_data);
            }
            it_data += mCols;
        }

    } else {

        auto it_data = mData.begin();
        size_t rows_counter = 0;
        for (auto it = values.begin(); it != values.end(); it++) {
            if (rows_counter < Rows) {
                size_t n = it->size();
                if (n == Cols) {
                    std::copy(it->begin(), it->end(), it_data);
                } else if (n < Cols) {
                    std::copy(it->begin(), it->end(), it_data);
                    std::fill(it_data + n, mData.end(), consts::zero<T>);
                } else {
                    std::copy(it->begin(), it->end() + Cols, it_data);
                }

                it_data += Cols;
                rows_counter++;
            }
        }

        if (it_data != mData.end()) {
            std::fill(it_data, mData.end(), consts::zero<T>);
        }
    }
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(const T *data, size_t rows, size_t cols)
  : mData(Data<T, data::size>(data, rows *cols)),
    mRows(Rows == DynamicData ? rows : Rows),
    mCols(Cols == DynamicData ? cols : Cols)
{
}



/*! \} */

} // namespace tl


