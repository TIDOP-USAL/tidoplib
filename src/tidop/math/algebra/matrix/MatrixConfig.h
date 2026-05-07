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

/*! \file MatrixConfig.h
 * \brief Configuration for matrix product implementation selection.
 *
 * This file defines the `MatrixConfig` class, which allows runtime selection
 * of the linear algebra backend for matrix multiplication and other operations.
 * The selection can be automatic (based on available libraries and heuristics)
 * or forced to a specific backend such as CUDA (CuBLAS), OpenBLAS, SIMD
 * intrinsics, or plain C++ loops.
 *
 * \ingroup Matrix
 */

#pragma once

#include "tidop/config.h"

namespace tl
{

/*! \addtogroup Matrix
 *  \{
 */

/*!
 * \class MatrixConfig
 * \brief Singleton configuration for matrix product backends.
 *
 * This class provides a global configuration that determines which
 * implementation is used for matrix multiplication (e.g., in `MatMulExpr`).
 * The selection can be changed at runtime via the `product` member, and the
 * decision can be either automatic or forced to a specific backend.
 *
 * ### Example
 * \code
 * // Force using SIMD implementation
 * MatrixConfig::instance().product = MatrixConfig::Product::SIMD;
 *
 * // Use automatic selection (default)
 * MatrixConfig::instance().product = MatrixConfig::Product::AUTO;
 * \endcode
 *
 * \note The actual availability of backends depends on build configuration.
 *       Backends that are not compiled in will not be available even if
 *       selected (the library will fall back to CPP or produce an error).
 */
class TL_EXPORT MatrixConfig
{

public:

    /*!
     * \enum Product
     * \brief Available matrix product implementation backends.
     */
    enum class Product
    {
        AUTO,                     /*!< Automatically select the best available backend. */
#ifdef TL_HAVE_CUDA
        CuBLAS,                   /*!< Use NVIDIA CUDA CuBLAS (if available). */
#endif
#ifdef TL_HAVE_OPENBLAS
        BLAS,                     /*!< Use OpenBLAS (if available). */
#endif
#ifdef TL_HAVE_SIMD_INTRINSICS
        SIMD,                     /*!< Use SIMD intrinsics (if available). */
#endif
        CPP                       /*!< Use plain C++ loops (fallback). */
    };

    /*! \brief Currently selected product backend. Defaults to `Product::AUTO`. */
    Product product = Product::AUTO;

    /*!
     * \brief Returns the singleton instance of the configuration.
     * \return Reference to the global `MatrixConfig` object.
     */
    static auto instance() -> MatrixConfig &
    {
        static MatrixConfig _config;
        return _config;
    }
};

/*! \} */

} // namespace tl


