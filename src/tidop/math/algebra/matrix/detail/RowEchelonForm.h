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

namespace tl
{

template<typename T, size_t Rows, size_t Cols>
class Matrix;
 
namespace detail
{


template<typename T, size_t Rows, size_t Cols>
auto matrix_row_echelon_form(Matrix<T, Rows, Cols> matrix) -> Matrix<T, Rows, Cols>
{
    size_t rows = matrix.rows();
    size_t cols = matrix.cols();

    for (size_t i = 0; i < rows; ++i) {

        T pivotElement{};
        size_t pivotRow{};
        size_t colPivotElement{};

        for (size_t c = i; c < cols; ++c) {

            pivotElement = matrix(i, c);
            pivotRow = i;

            for (size_t r = i + 1; r < rows; ++r) {
                if (std::abs(matrix(r, c)) > std::abs(pivotElement)) {
                    pivotElement = matrix(r, c);
                    pivotRow = r;
                }
            }

            if (!isNearlyZero(pivotElement)) {
                colPivotElement = c;
                break;
            }
        }

        if (pivotRow != i) {
            matrix.swapRows(i, pivotRow);
        }

        if (!isNearlyZero(pivotElement)) {

            for (size_t c = i; c < cols; ++c) {
                matrix(i, c) /= pivotElement;
            }

            for (size_t r = i + 1; r < rows; ++r) {
                T scale = matrix(r, colPivotElement);
                if (!isNearlyZero(scale)) {
                    for (size_t c = i; c < cols; ++c) {
                        matrix(r, c) -= scale * matrix(i, c);
                    }
                }
            }
        }
    }

    return matrix;
}

} // namespace detail

} // namespace tl