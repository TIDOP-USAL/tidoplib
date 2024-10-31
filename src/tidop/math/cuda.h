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

#ifdef TL_HAVE_CUDA
#include <cublas_v2.h>
#include <cuda_runtime.h>
#include <cusolverDn.h>

namespace tl
{


/*! \addtogroup Math
 *  \{
 */

namespace cuda
{

/* Matrix multiplication */

template<typename T>
auto gemm(size_t m, size_t n, size_t k,
          const T *a, const T *b, T *c) -> enableIfFloat<T, void>
{
    try {

        float *a_device, *b_device, *c_device;
        cudaError_t result = cudaMalloc((void **)&a_device, m * k * sizeof(float));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&b_device, k * n * sizeof(float));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&c_device, m * n * sizeof(float));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaMemcpy(a_device, a, m * k * sizeof(float), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMemcpy(b_device, b, k * n * sizeof(float), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        cublasHandle_t handle;
        cublasCreate(&handle);

        const T alpha = 1.f;
        const T beta = 0.f;

        cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                    static_cast<int>(n),
                    static_cast<int>(m),
                    static_cast<int>(k),
                    &alpha, b_device, static_cast<int>(n), a_device, static_cast<int>(k),
                    &beta, c_device, static_cast<int>(n));

        result = cudaMemcpy(c, c_device, m * n * sizeof(float), cudaMemcpyDeviceToHost);
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

template<typename T>
auto gemm(size_t m, size_t n, size_t k,
          const T *a, const T *b, T *c) -> enableIfDouble<T, void>
{
    try {

        double *a_device, *b_device, *c_device;
        cudaError_t result = cudaMalloc((void **)&a_device, m * k * sizeof(double));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&b_device, k * n * sizeof(double));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&c_device, m * n * sizeof(double));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaMemcpy(a_device, a, m * k * sizeof(double), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMemcpy(b_device, b, k * n * sizeof(double), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        cublasHandle_t handle;
        cublasCreate(&handle);

        const T alpha = 1.f;
        const T beta = 0.f;

        cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                    static_cast<int>(n),
                    static_cast<int>(m),
                    static_cast<int>(k),
                    &alpha, b_device, static_cast<int>(n), a_device, static_cast<int>(k),
                    &beta, c_device, static_cast<int>(n));

        result = cudaMemcpy(c, c_device, m * n * sizeof(double), cudaMemcpyDeviceToHost);
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

template<typename T>
auto gemv(size_t m, size_t n, const T *matrix, const T *vector, T *vector_out) -> enableIfFloat<T, void>
{
    try {

        T *matrix_device, *vector_device, *vector_out_device;
        const T alpha = 1.0;
        const T beta = 0.0;

        cudaError_t result = cudaMalloc((void **)&matrix_device, m * n * sizeof(float));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&vector_device, n * sizeof(float));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&vector_out_device, m * sizeof(float));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaMemcpy(matrix_device, matrix, m * n * sizeof(float), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMemcpy(vector_device, vector, n * sizeof(float), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        cublasHandle_t handle;
        cublasCreate(&handle);

        cublasSgemv(handle, CUBLAS_OP_T, n, m, &alpha, matrix_device, n, vector_device, 1, &beta, vector_out_device, 1);

        result = cudaMemcpy(vector_out, vector_out_device, m * sizeof(float), cudaMemcpyDeviceToHost);
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

template<typename T>
auto gemv(size_t m, size_t n, const T *matrix, const T *vector, T *vector_out) -> enableIfDouble<T, void>
{
    try {
        T *matrix_device, *vector_device, *vector_out_device;
        const T alpha = 1.0;
        const T beta = 0.0;

        cudaError_t result = cudaMalloc((void **)&matrix_device, m * n * sizeof(double));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&vector_device, n * sizeof(double));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMalloc((void **)&vector_out_device, m * sizeof(double));
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        result = cudaMemcpy(matrix_device, matrix, m * n * sizeof(double), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));
        result = cudaMemcpy(vector_device, vector, n * sizeof(double), cudaMemcpyHostToDevice);
        TL_ASSERT(result == cudaSuccess, "CUDA Error: {}", cudaGetErrorString(result));

        cublasHandle_t handle;
        cublasCreate(&handle);

        cublasDgemv(handle, CUBLAS_OP_T, n, m, &alpha, matrix_device, n, vector_device, 1, &beta, vector_out_device, 1);

        result = cudaMemcpy(vector_out, vector_out_device, m * sizeof(double), cudaMemcpyDeviceToHost);
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

//template<typename T>
//auto getrf(int rows, int cols, T *a, int lda, int *pivot, int *info) -> enableIfFloat<T, void>
//{
//    float *a_device;
//    int *d_pivot;
//    int *d_info;
//
//    cusolverDnHandle_t cusolverH = nullptr;
//    cusolverStatus_t result = cusolverDnCreate(&cusolverH);
//
//    cudaMalloc((void**)&a_device, sizeof(float) * rows * cols);
//    cudaMalloc((void**)&d_pivot, sizeof(int) * rows);
//    cudaMalloc((void**)&d_info, sizeof(int));
//
//    cudaMemcpy(a_device, a, sizeof(float) * lda * rows, cudaMemcpyHostToDevice);
//    TL_ASSERT(result == cudaSuccess, "cuSOLVER Error: cusolverDnCreate");
//
//    // Perform LU factorization
//
//    int work_size = 0;
//    cusolverDnSgetrf_bufferSize(cusolverH, rows, cols, a_device, lda, &work_size);
//
//    float* d_work;
//    cudaMalloc((void**)&d_work, sizeof(float) * work_size);
//
//    cusolverDnSgetrf(cusolverH, rows, cols, a_device, lda, d_work, d_pivot, d_info);
//    
//    cudaMemcpy(a, a_device, sizeof(float) * rows * cols, cudaMemcpyDeviceToHost);
//
//    cudaFree(a_device);
//    cudaFree(d_pivot);
//    cudaFree(d_info);
//    cudaFree(d_work);
//    cusolverDnDestroy(cusolverH);
//}
//
//template<typename T>
//auto getrf(int rows, int cols, T *a, int lda, int *pivot, int *info) -> enableIfDouble<T, void>
//{
//    double *a_device;
//    int *d_pivot;
//    int *d_info;
//
//    cusolverDnHandle_t cusolverH = nullptr;
//    cusolverStatus_t result = cusolverDnCreate(&cusolverH);
//    TL_ASSERT(result == cudaSuccess, "cuSOLVER Error: cusolverDnCreate");
//
//    cudaMalloc((void**)&a_device, sizeof(double) * rows * cols);
//    cudaMalloc((void**)&d_pivot, sizeof(int) * rows);
//    cudaMalloc((void**)&d_info, sizeof(int));
//
//    cudaMemcpy(a_device, a, sizeof(double) * lda * rows, cudaMemcpyHostToDevice);
//
//    // Perform LU factorization
//
//    int work_size = 0;
//    cusolverDnDgetrf_bufferSize(cusolverH, rows, cols, a_device, lda, &work_size);
//
//    double* d_work;
//    cudaMalloc((void**)&d_work, sizeof(double) * work_size);
//
//    cusolverDnDgetrf(cusolverH, rows, cols, a_device, lda, d_work, d_pivot, d_info);
//
//    cudaMemcpy(a, a_device, sizeof(double) * rows * cols, cudaMemcpyDeviceToHost);
//
//    cudaFree(a_device);
//    cudaFree(d_pivot);
//    cudaFree(d_info);
//    cudaFree(d_work);
//    cusolverDnDestroy(cusolverH);
//}
//
//
//
//template<typename T> 
//auto getrs(int rows, int nrhs, T *a, int lda, int* ipiv, T *b, int ldb, int* info) -> enableIfFloat<T, void>
//{
//    cusolverDnHandle_t cusolverH = nullptr;
//    cusolverStatus_t result = cusolverDnCreate(&cusolverH);
//    TL_ASSERT(result == cudaSuccess, "cuSOLVER Error: cusolverDnCreate");
//
//    float *a_device;
//    float *b_device;
//    int* d_pivot;
//    int* d_info;
//
//    cudaMalloc((void**)&a_device, sizeof(float) * lda * rows);
//    cudaMalloc((void**)&b_device, sizeof(float) * ldb);
//    cudaMalloc((void**)&d_pivot, sizeof(int) * rows);
//    cudaMalloc((void**)&d_info, sizeof(int));
//
//    cudaMemcpy(a_device, a, sizeof(float) * lda * rows, cudaMemcpyHostToDevice);
//    cudaMemcpy(b_device, b, sizeof(float) * ldb, cudaMemcpyHostToDevice);
//
//    cusolverDnSgetrs(cusolverH, CUBLAS_OP_N, rows, nrhs, a_device, lda, d_pivot, b_device, ldb, d_info);
//
//    //T h_X[n];
//    cudaMemcpy(b, b_device, sizeof(float) * rows, cudaMemcpyDeviceToHost);
//
//    cudaFree(a_device);
//    cudaFree(b_device);
//    cudaFree(d_pivot);
//    cudaFree(d_info);
//    cusolverDnDestroy(cusolverH);
//}
//
//template<typename T> 
//auto getrs(int rows, int nrhs, T *a, int lda, int* ipiv, T *b, int ldb, int* info) -> enableIfDouble<T, void>
//{
//    cusolverDnHandle_t cusolverH = nullptr;
//    cusolverStatus_t result = cusolverDnCreate(&cusolverH);
//    TL_ASSERT(result == cudaSuccess, "cuSOLVER Error: cusolverDnCreate");
//
//    double *a_device;
//    double *b_device;
//    int* d_pivot;
//    int* d_info;
//
//    cudaMalloc((void**)&a_device, sizeof(double) * lda * rows);
//    cudaMalloc((void**)&b_device, sizeof(double) * ldb);
//
//    cudaMemcpy(a_device, a, sizeof(double) * lda * rows, cudaMemcpyHostToDevice);
//    cudaMemcpy(b_device, b, sizeof(double) * ldb, cudaMemcpyHostToDevice);
//    cudaMalloc((void**)&d_pivot, sizeof(int) * rows);
//    cudaMalloc((void**)&d_info, sizeof(int));
//
//    cusolverDnDgetrs(cusolverH, CUBLAS_OP_N, rows, nrhs, a_device, lda, d_pivot, b_device, ldb, d_info);
//
//    cudaMemcpy(b, b_device, sizeof(double) * rows, cudaMemcpyDeviceToHost);
//
//    cudaFree(a_device);
//    cudaFree(b_device);
//    cudaFree(d_pivot);
//    cudaFree(d_info);
//    cusolverDnDestroy(cusolverH);
//}


} // End namespace blas

/*! \} */ // end of Math

} // End namespace tl

#endif // TL_HAVE_CUBLAS

