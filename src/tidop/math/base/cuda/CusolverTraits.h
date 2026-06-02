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

#include <cusolverDn.h>

namespace tl
{

/*! \addtogroup MathBase
 *  \{
 */

namespace cuda
{

template <typename T>
struct CusolverTraits;

template <>
struct CusolverTraits<float>
{
    static constexpr auto getrf_bufferSize = cusolverDnSgetrf_bufferSize;
    static constexpr auto getrf = cusolverDnSgetrf;
    static constexpr auto getrs = cusolverDnSgetrs;
};

template <>
struct CusolverTraits<double>
{
    static constexpr auto getrf_bufferSize = cusolverDnDgetrf_bufferSize;
    static constexpr auto getrf = cusolverDnDgetrf;
    static constexpr auto getrs = cusolverDnDgetrs;
};


} // End namespace cuda

/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_CUDA
/// \endcond
