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

#include "tidop/math/algebra/matrix/detail/Cofactor.h"

namespace tl
{

template<typename T, size_t Rows, size_t Cols>
class Matrix;
 
namespace detail
{

template<typename T, size_t Rows, size_t Cols>
auto matrix_cofactor_matrix(const Matrix<T, Rows, Cols> &mat) -> Matrix<T, Rows, Cols>
{
    static_assert(Rows == Cols, "Matrices must be square");

    size_t rows = mat.rows();
    size_t cols = mat.cols();
    TL_ASSERT(rows == cols, "Matrices must be square");

    Matrix<T, Rows, Cols> result;

    if constexpr (Rows == DynamicData || Cols == DynamicData) {
        result.resize(rows, cols);
    }

    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            result(r, c) = matrix_cofactor(mat, r, c);
        }
    }

    return result;
}

} // namespace detail

} // namespace tl