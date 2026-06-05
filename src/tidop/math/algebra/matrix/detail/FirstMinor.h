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

#include "tidop/math/algebra/matrix/detail/Determinant.h"

/// \cond

namespace tl
{

template<typename T, size_t Rows, size_t Cols>
class Matrix;
 
namespace detail
{

template<typename T, size_t Rows, size_t Cols>
[[nodiscard]]
constexpr auto matrix_first_minor(const Matrix<T, Rows, Cols> &mat, size_t row, size_t col) -> T
{
    static_assert(Rows == Cols, "Matrices must be square");
    size_t rows = mat.rows();
    size_t cols = mat.cols();
    TL_ASSERT(rows == cols, "Matrices must be square");

    size_t i = 0;
    size_t j = 0;

    // TODO: esto debería ser una vista o una expresión
    // Primero habría que modificar estas funciones para que admitan MatExpr en lugar de una matriz
    Matrix<T> matrix(rows - 1, cols - 1);
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            if (row != r && col != c) {
                matrix(i,j++) = mat(r, c);
                if (j == rows - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }

    return matrix_determinant(matrix);
}

} // namespace detail

} // namespace tl

/// \endcond