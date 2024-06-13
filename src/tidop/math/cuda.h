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
	
    float *a_device, *b_device, *c_device;
    cudaMalloc((void **)&a_device, m * k * sizeof(float));
    cudaMalloc((void **)&b_device, k * n * sizeof(float));
    cudaMalloc((void **)&c_device, m * n * sizeof(float));

    cudaMemcpy(a_device, a, m * k * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(b_device, b, k * n * sizeof(float), cudaMemcpyHostToDevice);

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

    cudaMemcpy(c, c_device, m * n * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(a_device);
    cudaFree(b_device);
    cudaFree(c_device);

    cublasDestroy(handle);
}

template<typename T>
auto gemm(size_t m, size_t n, size_t k,
          const T *a, const T *b, T *c) -> enableIfDouble<T, void>
{
    double *a_device, *b_device, *c_device;
    cudaMalloc((void **)&a_device, m * k * sizeof(double));
    cudaMalloc((void **)&b_device, k * n * sizeof(double));
    cudaMalloc((void **)&c_device, m * n * sizeof(double));

    cudaMemcpy(a_device, a, m * k * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(b_device, b, k * n * sizeof(double), cudaMemcpyHostToDevice);

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

    cudaMemcpy(c, c_device, m * n * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(a_device);
    cudaFree(b_device);
    cudaFree(c_device);

    cublasDestroy(handle);				
}


/* Matrix x Vector */

template<typename T>
auto gemv(size_t m, size_t n, const T *matrix, const T *vector, T *vector_out) -> enableIfFloat<T, void>
{
    T *matrix_device, *vector_device, *vector_out_device;
    const T alpha = 1.0;
    const T beta = 0.0;

    cudaMalloc((void **)&matrix_device, m * n * sizeof(float));
    cudaMalloc((void **)&vector_device, n * sizeof(float));
    cudaMalloc((void **)&vector_out_device, m * sizeof(float));

    cudaMemcpy(matrix_device, matrix, m * n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(vector_device, vector, n * sizeof(float), cudaMemcpyHostToDevice);

    cublasHandle_t handle;
    cublasCreate(&handle);

    cublasSgemv(handle, CUBLAS_OP_T, n, m, &alpha, matrix_device, n, vector_device, 1, &beta, vector_out_device, 1);

    cudaMemcpy(vector_out, vector_out_device, m * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(matrix_device);
    cudaFree(vector_device);
    cudaFree(vector_out_device);

    cublasDestroy(handle);
}

template<typename T>
auto gemv(size_t m, size_t n, const T *matrix, const T *vector, T *vector_out) -> enableIfDouble<T, void>
{
    T *matrix_device, *vector_device, *vector_out_device;
    const T alpha = 1.0;
    const T beta = 0.0;

    cudaMalloc((void **)&matrix_device, m * n * sizeof(double));
    cudaMalloc((void **)&vector_device, n * sizeof(double));
    cudaMalloc((void **)&vector_out_device, m * sizeof(double));

    cudaMemcpy(matrix_device, matrix, m * n * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(vector_device, vector, n * sizeof(double), cudaMemcpyHostToDevice);

    cublasHandle_t handle;
    cublasCreate(&handle);

    cublasDgemv(handle, CUBLAS_OP_T, n, m, &alpha, matrix_device, n, vector_device, 1, &beta, vector_out_device, 1);

    cudaMemcpy(vector_out, vector_out_device, m * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(matrix_device);
    cudaFree(vector_device);
    cudaFree(vector_out_device);

    cublasDestroy(handle);
}




} // End namespace blas

/*! \} */ // end of Math

} // End namespace tl

#endif // TL_HAVE_CUBLAS

