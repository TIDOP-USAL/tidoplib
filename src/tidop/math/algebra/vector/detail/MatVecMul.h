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

#include "tidop/math/algebra/vector/detail/MatVecMulCPP.h"
#include "tidop/math/algebra/vector/detail/MatVecMulSIMD.h" 
#include "tidop/math/algebra/detail/Evaluator.h"
#include "tidop/math/algebra/matrix/MatrixConfig.h"

namespace tl
{

class MatrixConfig;

namespace detail 
{

template<typename Mat, typename VecIn, typename VecOut>
void mat_vec_mul(const Mat &matrix, const VecIn &vectorIn, VecOut &vectorOut)
{
	//TODO: Es necesario comprobar si es auto y determinar la mejor opción
	
    switch (MatrixConfig::instance().product) {
//#ifdef TL_HAVE_CUDA
//    case MatrixConfig::Product::CuBLAS:
//        cuda::gemv(matrix.rows(), 
//                   matrix.cols(), 
//                   matrix.data(),
//                   vectorIn.data(), 
//                   v_out.data());
//        break;
//#endif
//#ifdef TL_HAVE_OPENBLAS
//    case MatrixConfig::Product::BLAS:
//    {
//        T alpha = 1.;
//        T beta = 0.;
//        size_t lda = matrix.cols();
//        size_t incx = 1;
//        size_t incy = 1;
//        blas::gemv(blas::Order::row_major,
//                   blas::TransposeMode::no_transpose,
//                   matrix.rows(),
//                   matrix.cols(),
//                   alpha,
//                   matrix.data(), lda,
//                   vectorIn.data(), incx,
//                   beta,
//                   v_out.data(), incy);        
//    }
//    break;
//#endif
#ifdef TL_HAVE_SIMD_INTRINSICS
    case MatrixConfig::Product::SIMD:
    {
        decltype(auto) a = require_linear_access(matrix);
        decltype(auto) b = require_linear_access(vectorIn);
        mat_vec_mul_simd(a, b, vectorOut);
        break;
    }
#endif
    case MatrixConfig::Product::CPP:
    default:
    {
        decltype(auto) a = require_linear_access(matrix);
        decltype(auto) b = require_linear_access(vectorIn);
        mat_vec_mul_cpp(a, b, vectorOut);
        break;
    }
    }
}

} // namespace detail

} // namespace tl

