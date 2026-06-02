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

#include "tidop/core/base/defs.h"
#include "tidop/math/base/cuda/CublasTraits.h"

#include <cuda_runtime.h>

#include <cstddef>
#include <type_traits>

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace cuda
{

/*!
 * \brief Matrix-vector multiplication (gemv) for floating point types on the GPU.
 *
 * This function performs matrix-vector multiplication using the General Matrix Vector (GEMV) operation
 * for floating point types (`float` or `double`) on the GPU.
 * It uses CUDA and cuBLAS to offload the computation to the GPU.
 *
 * \tparam T The data type (must be `float` or `double`).
 * \param[in] m The number of rows of matrix A.
 * \param[in] n The number of columns of matrix A and the length of the vector.
 * \param[in] matrix Pointer to the matrix A of size `m x n`.
 * \param[in] vector Pointer to the vector B of size `n`.
 * \param[out] vector_out Pointer to the output vector C of size `m`, where the result will be stored.
 */
template <typename T>
void gemv(size_t m, size_t n, const T *matrix, const T *vector, T *vector_out)
{
    try {

        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value,
            "Only float or double types are supported.");

        T *matrix_device = nullptr;
        T *vector_device = nullptr;
        T *vector_out_device = nullptr;
        const T alpha = 1.0;
        const T beta = 0.0;

        cudaError_t result = cudaMalloc((void **)&matrix_device, m * n * sizeof(T));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&vector_device, n * sizeof(T));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&vector_out_device, m * sizeof(T));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaMemcpy(matrix_device, matrix, m * n * sizeof(T), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMemcpy(vector_device, vector, n * sizeof(T), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        cublasHandle_t handle;
        cublasCreate(&handle);

        CublasTraits<T>::gemv(handle, CublasTraits<T>::op, n, m, &alpha,
                              matrix_device, n, vector_device, 1, &beta,
                              vector_out_device, 1);

        result = cudaMemcpy(vector_out, vector_out_device, m * sizeof(T), cudaMemcpyDeviceToHost);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaFree(matrix_device);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaFree(vector_device);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaFree(vector_out_device);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        cublasDestroy(handle);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Error in Matrix x Vector");
    }
}

} // End namespace cuda

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_CUDA
/// \endcond
