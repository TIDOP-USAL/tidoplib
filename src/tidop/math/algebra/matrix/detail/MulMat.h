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

#include "tidop/math/base/Cuda.h"
#include "tidop/math/algebra/matrix/detail/MulMatBLAS.h"
#include "tidop/math/algebra/matrix/detail/MulMatCPP.h"
#include "tidop/math/algebra/matrix/detail/MulMatSIMD.h"

/// \cond

namespace tl
{

namespace detail
{

template<MatrixExpr A, MatrixExpr B>
auto select_backend(const A &a, const B &b) -> MatrixConfig::Product
{
    auto cfg = MatrixConfig::instance().product;

    if (cfg != MatrixConfig::Product::AUTO) {
        return cfg;
    }

    using T = typename matrix_traits<A>::value_type;
    constexpr bool is_float = std::floating_point<T>;

    size_t M = a.rows();
    size_t N = b.cols();
    size_t K = a.cols();

#ifdef TL_HAVE_CUDA
    if (is_float && M > 2000 && N > 2000 && K > 2000) {
        return MatrixConfig::Product::CuBLAS;
    }
#endif

#ifdef TL_HAVE_OPENBLAS
    if (is_float && M > 32 && N > 32 && K > 32) {
        if ((M / N) < 10 && (N / M) < 10) {
            return MatrixConfig::Product::BLAS;
        }
    }
#endif

#ifdef TL_HAVE_SIMD_INTRINSICS
    return MatrixConfig::Product::SIMD;
#else
    return MatrixConfig::Product::CPP;
#endif
}

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
    case MatrixConfig::Product::CuBLAS:
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
    case MatrixConfig::Product::BLAS:
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
    case MatrixConfig::Product::SIMD:
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
            // En este caso tengo que inicializar la matriz de salida a 0, porque el micro-kernel no hace acumulación sobre la salida, sino que la escribe directamente.
            out.fill(0);
            mulmat_simd_kernel_opt3(a, b, out);
        } else {
            //detail::mulmat_simd(lhs, rhs, out);
            //detail::mulmat_simd2(lhs, rhs, out);
            out.fill(0);
            mulmat_simd3(a, b, out);
        }
        break;
    }
#endif

    case MatrixConfig::Product::CPP:
    default:
    {
        decltype(auto) a = require_linear_access(lhs);
        decltype(auto) b = require_linear_access(rhs);
        mulmat_cpp(a, b, out);
        break;
    }
    }
}

} // namespace detail

} // namespace tl

/// \endcond