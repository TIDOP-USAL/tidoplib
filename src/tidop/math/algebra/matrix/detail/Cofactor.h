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

#include "tidop/math/algebra/matrix/detail/FirstMinor.h"

/// \cond

namespace tl
{
 
template<typename T, size_t Rows, size_t Cols>
class Matrix;

namespace detail
{

template<typename T, size_t Rows, size_t Cols>
[[nodiscard]]
constexpr auto matrix_cofactor(const Matrix<T, Rows, Cols> &mat, size_t r, size_t c) -> T
{
    static_assert(Rows == Cols, "Matrices must be square");

    TL_ASSERT(mat.rows() == mat.cols(), "Matrices must be square");

    T sign = ((r + c) % 2 == 0) ? consts::one<T> : -consts::one<T>;

    return sign * matrix_first_minor(mat, r, c);
}

} // namespace detail

} // namespace tl

/// \endcond