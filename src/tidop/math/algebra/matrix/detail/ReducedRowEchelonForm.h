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

namespace tl
{

template<typename T, size_t Rows, size_t Cols>
class Matrix;
 
namespace detail
{

template<typename T, size_t Rows, size_t Cols>
constexpr auto matrix_reduced_row_echelon_form(Matrix<T, Rows, Cols> matrix) -> Matrix<T, Rows, Cols>
{
    size_t rows = matrix.rows();
    size_t cols = matrix.cols();
    int lead = 0;

    for (int r = 0; r < rows; r++) {

        if (lead >= cols) {
            return matrix;
        }
        int i = r;

        while (isNearlyZero(matrix(i, lead))) {
            i++;
            if (i == rows) {
                i = r;
                lead++;
                if (lead == cols) {
                    return matrix;
                }
            }
        }

        if (r != i) {
            matrix.swapRows(i, r);
        }

        T lv = consts::one<T> / matrix(r, lead);

        //matrix[r] *= lv; -> Si se implementa el operador *= para filas, se puede usar esta línea en lugar del bucle for
        for (size_t c = 0; c < cols; ++c) {
            matrix(r, c) *= lv;
        }

        for (int i = 0; i < rows; i++) {
            if (i != r) {
                T lv2 = matrix(i, lead);
                for (int c = 0; c < cols; c++) {
                    matrix(i, c) -= lv2 * matrix(r, c);
                }
            }
        }

        lead++;
    }

    return matrix;
}


} // namespace detail

} // namespace tl

/// \endcond