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

#include "tidop/math/base/cuda/CusolverTraits.h"

#include <cublas_v2.h>
#include <cuda_runtime.h>

#include <stdexcept>
#include <type_traits>

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace cuda
{

template <typename T>
void getrf(int rows, int cols, T *a, int lda, int *pivot, int *info)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value,
        "Only float or double types are supported.");

    T *a_device = nullptr;
    int *d_pivot = nullptr;
    int *d_info = nullptr;
    T *d_work = nullptr;
    cusolverDnHandle_t cusolverH;

    if (cusolverDnCreate(&cusolverH) != CUSOLVER_STATUS_SUCCESS) {
        throw std::runtime_error("Failed to create cuSOLVER handle.");
    }

    cudaMalloc(&a_device, sizeof(T) * lda * rows);
    cudaMalloc(&d_pivot, sizeof(int) * rows);
    cudaMalloc(&d_info, sizeof(int));

    cudaMemcpy(a_device, a, sizeof(T) * lda * rows, cudaMemcpyHostToDevice);

    int work_size = 0;
    CusolverTraits<T>::getrf_bufferSize(cusolverH, rows, cols, a_device, lda, &work_size);

    cudaMalloc(&d_work, sizeof(T) * work_size);

    CusolverTraits<T>::getrf(cusolverH, rows, cols, a_device, lda, d_work, d_pivot, d_info);

    int h_info = 0;
    cudaMemcpy(&h_info, d_info, sizeof(int), cudaMemcpyDeviceToHost);
    if (h_info != 0) {
        throw std::runtime_error("LU factorization failed. Matrix may be singular.");
    }

    cudaMemcpy(a, a_device, sizeof(T) * lda * rows, cudaMemcpyDeviceToHost);
    cudaMemcpy(pivot, d_pivot, sizeof(int) * rows, cudaMemcpyDeviceToHost);

    cudaFree(a_device);
    cudaFree(d_pivot);
    cudaFree(d_info);
    cudaFree(d_work);
    cusolverDnDestroy(cusolverH);
}

template <typename T>
void getrs(int rows, int nrhs, T *a, int lda, int *ipiv, T *b, int ldb, int *info)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value,
        "Only float or double types are supported.");

    T *a_device = nullptr;
    T *b_device = nullptr;
    int *d_pivot = nullptr;
    int *d_info = nullptr;
    cusolverDnHandle_t cusolverH;

    if (cusolverDnCreate(&cusolverH) != CUSOLVER_STATUS_SUCCESS) {
        throw std::runtime_error("Failed to create cuSOLVER handle.");
    }

    cudaMalloc(&a_device, sizeof(T) * lda * rows);
    cudaMalloc(&b_device, sizeof(T) * ldb * nrhs);
    cudaMalloc(&d_pivot, sizeof(int) * rows);
    cudaMalloc(&d_info, sizeof(int));

    cudaMemcpy(a_device, a, sizeof(T) * lda * rows, cudaMemcpyHostToDevice);
    cudaMemcpy(b_device, b, sizeof(T) * ldb * nrhs, cudaMemcpyHostToDevice);
    cudaMemcpy(d_pivot, ipiv, sizeof(int) * rows, cudaMemcpyHostToDevice);

    CusolverTraits<T>::getrs(cusolverH, CUBLAS_OP_N, rows, nrhs, a_device, lda, d_pivot, b_device, ldb, d_info);

    int h_info = 0;
    cudaMemcpy(&h_info, d_info, sizeof(int), cudaMemcpyDeviceToHost);
    if (h_info != 0) {
        throw std::runtime_error("Solving linear system failed.");
    }

    cudaMemcpy(b, b_device, sizeof(T) * ldb * nrhs, cudaMemcpyDeviceToHost);

    cudaFree(a_device);
    cudaFree(b_device);
    cudaFree(d_pivot);
    cudaFree(d_info);
    cusolverDnDestroy(cusolverH);
}

} // End namespace cuda

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_CUDA
/// \endcond
