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
auto determinant2x2(const Matrix<T, Rows, Cols> &mat) -> T
{
    return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
}

template<typename T, size_t Rows, size_t Cols>
auto determinant3x3(const Matrix<T, Rows, Cols> &mat) -> T
{
    T m00 = mat(0, 0); 
    T m01 = mat(0, 1); 
    T m02 = mat(0, 2);
    T m10 = mat(1, 0); 
    T m11 = mat(1, 1); 
    T m12 = mat(1, 2);
    T m20 = mat(2, 0); 
    T m21 = mat(2, 1); 
    T m22 = mat(2, 2);

    T c00 = m11 * m22 - m12 * m21;
    T c10 = m12 * m20 - m10 * m22;
    T c20 = m10 * m21 - m11 * m20;

    return m00 * c00 + m01 * c10 + m02 * c20;
}

template<typename T, size_t Rows, size_t Cols>
auto determinant4x4(const Matrix<T, Rows, Cols> &mat) -> T
{
    T m00 = mat(0, 0);
    T m01 = mat(0, 1);
    T m02 = mat(0, 2);
    T m03 = mat(0, 3);
    T m10 = mat(1, 0);
    T m11 = mat(1, 1);
    T m12 = mat(1, 2);
    T m13 = mat(1, 3);
    T m20 = mat(2, 0);
    T m21 = mat(2, 1);
    T m22 = mat(2, 2);
    T m23 = mat(2, 3);
    T m30 = mat(3, 0);
    T m31 = mat(3, 1);
    T m32 = mat(3, 2);
    T m33 = mat(3, 3);

    T a0 = m00 * m11 - m01 * m10;
    T a1 = m00 * m12 - m02 * m10;
    T a2 = m00 * m13 - m03 * m10;
    T a3 = m01 * m12 - m02 * m11;
    T a4 = m01 * m13 - m03 * m11;
    T a5 = m02 * m13 - m03 * m12;
    T b0 = m20 * m31 - m21 * m30;
    T b1 = m20 * m32 - m22 * m30;
    T b2 = m20 * m33 - m23 * m30;
    T b3 = m21 * m32 - m22 * m31;
    T b4 = m21 * m33 - m23 * m31;
    T b5 = m22 * m33 - m23 * m32;

    return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
}


template<typename T, size_t Rows, size_t Cols>
auto determinantnxn(const Matrix<T, Rows, Cols> &mat) -> T
{
    T d = consts::one<T>;
    size_t rows = mat.rows();
    size_t cols = mat.cols();

    Matrix<T, DynamicData, DynamicData> matrix = mat;

    for (size_t i = 0; i < rows; ++i) {
        T pivotElement = matrix(i, i);
        size_t pivotRow = i;
        for (size_t r = i + 1; r < rows; ++r) {
            if (std::abs(matrix(r, i)) > std::abs(pivotElement)) {
                pivotElement = matrix(r, i);
                pivotRow = r;
            }
        }

        if (pivotElement == consts::zero<T>) {
            d = consts::zero<T>;
            break;
        }

        if (pivotRow != i) {
            matrix.swapRows(i, pivotRow);
            d = -d;
        }

        d *= pivotElement;

        for (size_t r = i + 1; r < rows; ++r) {
            for (size_t c = i + 1; c < cols; ++c) {
                matrix(r, c) -= matrix(r, i) * matrix(i, c) / pivotElement;
            }
        }
    }

    return d;
}

template<typename T, size_t Rows, size_t Cols>
auto matrix_determinant(const Matrix<T, Rows, Cols> &mat) -> T
{
    static_assert(Rows == DynamicData || Cols == DynamicData || Rows == Cols,
        "Static matrices must be square");

    if constexpr (Rows == 2 && Cols == 2) {
        return determinant2x2(mat);
    } else if constexpr (Rows == 3 && Cols == 3) {
        return determinant3x3(mat);
    } else if constexpr (Rows == 4 && Cols == 4) {
        return determinant4x4(mat);
    } else {
        if (mat.rows() == 2) 
            return determinant2x2(mat);
        if (mat.rows() == 3) 
            return determinant3x3(mat);
        if (mat.rows() == 4) 
            return determinant4x4(mat);
        return determinantnxn(mat);
    }
}


} // namespace detail

} // End namespace tl