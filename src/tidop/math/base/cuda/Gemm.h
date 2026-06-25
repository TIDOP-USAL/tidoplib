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

/// \cond
#ifdef TL_HAVE_CUDA
/// \endcond

#include "tidop/core/base/Defs.h"
#include "tidop/math/base/cuda/CublasTraits.h"

#include <cuda_runtime.h>

#include <cstddef>
#include <stdexcept>
#include <type_traits>

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace cuda
{

/*!
 * \brief Matrix-matrix multiplication (gemm) for floating point types on the GPU.
 *
 * This function performs matrix-matrix multiplication using the General Matrix Multiply (GEMM) operation
 * for floating point types (`float` or `double`) on the GPU.
 * It uses CUDA and cuBLAS to offload the computation to the GPU.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] m The number of rows of matrix A and matrix C.
 * \param[in] n The number of columns of matrix B and matrix C.
 * \param[in] k The number of columns of matrix A and rows of matrix B.
 * \param[in] a Pointer to the matrix A of size `m x k`.
 * \param[in] b Pointer to the matrix B of size `k x n`.
 * \param[out] c Pointer to the matrix C of size `m x n`, where the result will be stored.
 */
template <typename T>
void gemm(size_t m, size_t n, size_t k, const T *a, const T *b, T *c)
{
    try {

        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value,
            "Only float or double types are supported.");

        T *a_device = nullptr;
        T *b_device = nullptr;
        T *c_device = nullptr;
        cublasHandle_t handle;

        if (cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS) {
            throw std::runtime_error("Failed to create cuBLAS handle.");
        }

        cudaError_t result = cudaMalloc((void **)&a_device, sizeof(T) * m * k);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&b_device, sizeof(T) * k * n);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&c_device, sizeof(T) * m * n);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaMemcpy(a_device, a, sizeof(T) * m * k, cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMemcpy(b_device, b, sizeof(T) * k * n, cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        const T alpha = 1.0;
        const T beta = 0.0;

        CublasTraits<T>::gemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                              static_cast<int>(n),
                              static_cast<int>(m),
                              static_cast<int>(k),
                              &alpha, b_device, static_cast<int>(n), a_device, static_cast<int>(k),
                              &beta, c_device, static_cast<int>(n));

        result = cudaMemcpy(c, c_device, sizeof(T) * m * n, cudaMemcpyDeviceToHost);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaFree(a_device);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaFree(b_device);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaFree(c_device);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        cublasDestroy(handle);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error in matrix multiplication");
    }
}

} // End namespace cuda

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_CUDA
/// \endcond
