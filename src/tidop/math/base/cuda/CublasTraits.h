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

#include <cublas_v2.h>

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

} // End namespace cuda

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_CUDA
/// \endcond
