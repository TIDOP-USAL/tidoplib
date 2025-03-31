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


#include "tidop/math/math.h"

/// \cond
#ifdef TL_HAVE_CUDA
/// \endcond

#include <cublas_v2.h>
#include <cuda_runtime.h>
#include <cusolverDn.h>

namespace tl
{


/*! \addtogroup MathBase
 *  \{
 */

namespace cuda
{


/// \cond

template <typename T>
struct CublasTraits;

template <>
struct CublasTraits<float> 
{
    static constexpr auto gemm = cublasSgemm;
    static constexpr auto gemv = cublasSgemv;
    static constexpr auto op = CUBLAS_OP_T;
};

template <>
struct CublasTraits<double> 
{
    static constexpr auto gemm = cublasDgemm;
    static constexpr auto gemv = cublasDgemv;
    static constexpr auto op = CUBLAS_OP_T;
};

/// \endcond

/* Matrix multiplication */

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

        cudaMemcpy(c, c_device, sizeof(T) * m * n, cudaMemcpyDeviceToHost);
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

/* Matrix x Vector */

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



/* Factorización LU */

template <typename T>
struct cusolverTraits;

template <>
struct cusolverTraits<float> 
{
    static constexpr auto getrf_bufferSize = cusolverDnSgetrf_bufferSize;
    static constexpr auto getrf = cusolverDnSgetrf;
    static constexpr auto getrs = cusolverDnSgetrs;
};

template <>
struct cusolverTraits<double>
{
    static constexpr auto getrf_bufferSize = cusolverDnDgetrf_bufferSize;
    static constexpr auto getrf = cusolverDnDgetrf;
    static constexpr auto getrs = cusolverDnDgetrs;
};

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

    // Create cuSOLVER handle
    if (cusolverDnCreate(&cusolverH) != CUSOLVER_STATUS_SUCCESS) {
        throw std::runtime_error("Failed to create cuSOLVER handle.");
    }

    // Allocate device memory
    cudaMalloc(&a_device, sizeof(T) * lda * rows);
    cudaMalloc(&d_pivot, sizeof(int) * rows);
    cudaMalloc(&d_info, sizeof(int));

    // Copy input matrix to device
    cudaMemcpy(a_device, a, sizeof(T) * lda * rows, cudaMemcpyHostToDevice);

    // Query workspace size
    int work_size = 0;
    cusolverTraits<T>::getrf_bufferSize(cusolverH, rows, cols, a_device, lda, &work_size);

    cudaMalloc(&d_work, sizeof(T) * work_size);

    // Perform LU factorization
    cusolverTraits<T>::getrf(cusolverH, rows, cols, a_device, lda, d_work, d_pivot, d_info);

    // Check for success
    int h_info = 0;
    cudaMemcpy(&h_info, d_info, sizeof(int), cudaMemcpyDeviceToHost);
    if (h_info != 0) {
        throw std::runtime_error("LU factorization failed. Matrix may be singular.");
    }

    // Copy results back to host
    cudaMemcpy(a, a_device, sizeof(T) * lda * rows, cudaMemcpyDeviceToHost);
    cudaMemcpy(pivot, d_pivot, sizeof(int) * rows, cudaMemcpyDeviceToHost);

    // Clean up
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

    // Create cuSOLVER handle
    if (cusolverDnCreate(&cusolverH) != CUSOLVER_STATUS_SUCCESS) {
        throw std::runtime_error("Failed to create cuSOLVER handle.");
    }

    // Allocate device memory
    cudaMalloc(&a_device, sizeof(T) * lda * rows);
    cudaMalloc(&b_device, sizeof(T) * ldb * nrhs);
    cudaMalloc(&d_pivot, sizeof(int) * rows);
    cudaMalloc(&d_info, sizeof(int));

    // Copy input data to device
    cudaMemcpy(a_device, a, sizeof(T) * lda * rows, cudaMemcpyHostToDevice);
    cudaMemcpy(b_device, b, sizeof(T) * ldb * nrhs, cudaMemcpyHostToDevice);
    cudaMemcpy(d_pivot, ipiv, sizeof(int) * rows, cudaMemcpyHostToDevice);

    // Solve the system
    cusolverTraits<T>::getrs(cusolverH, CUBLAS_OP_N, rows, nrhs, a_device, lda, d_pivot, b_device, ldb, d_info);

    // Check for success
    int h_info = 0;
    cudaMemcpy(&h_info, d_info, sizeof(int), cudaMemcpyDeviceToHost);
    if (h_info != 0) {
        throw std::runtime_error("Solving linear system failed.");
    }

    // Copy result back to host
    cudaMemcpy(b, b_device, sizeof(T) * ldb * nrhs, cudaMemcpyDeviceToHost);

    // Clean up
    cudaFree(a_device);
    cudaFree(b_device);
    cudaFree(d_pivot);
    cudaFree(d_info);
    cusolverDnDestroy(cusolverH);
}



} // End namespace blas

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_CUBLAS
/// \endcond
