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

namespace tl
{

template<typename T, size_t Rows, size_t Cols>
class Matrix;
 
namespace detail
{

template<typename T, size_t Rows, size_t Cols>
auto inverse2x2(const Matrix<T, Rows, Cols> &mat, 
                bool *invertibility) -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> result(mat);

    T det = mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);

    if (det != consts::zero<T>) {
        result(0, 0) = mat(1, 1) / det;
        result(0, 1) = -mat(0, 1) / det;
        result(1, 0) = -mat(1, 0) / det;
        result(1, 1) = mat(0, 0) / det;
        if (invertibility) *invertibility = true;
    } else {
        if (invertibility) *invertibility = false;
    }

    return result;
}

template<typename T, size_t Rows, size_t Cols>
auto inverse3x3(const Matrix<T, Rows, Cols> &mat, 
                bool *invertibility) -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix(mat);

    Matrix<T, Rows, Cols> adj = adjoint3x3(mat);
    T det = mat(0, 0) * adj(0, 0) +
            mat(0, 1) * adj(1, 0) +
            mat(0, 2) * adj(2, 0);

    if (det != consts::zero<T>) {

        matrix = adj / det;

        if (invertibility) *invertibility = true;
    } else {
        if (invertibility) *invertibility = false;
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto inverse4x4(const Matrix<T, Rows, Cols> &mat,
                bool *invertibility) -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix(mat);

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
    T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    if (det != consts::zero<T>) {

        matrix(0,0) = (m11 * b5 - m12 * b4 + m13 * b3) / det;
        matrix(0,1) = (-m01 * b5 + m02 * b4 - m03 * b3) / det;
        matrix(0,2) = (m31 * a5 - m32 * a4 + m33 * a3) / det;
        matrix(0,3) = (-m21 * a5 + m22 * a4 - m23 * a3) / det;
        matrix(1,0) = (-m10 * b5 + m12 * b2 - m13 * b1) / det;
        matrix(1,1) = (m00 * b5 - m02 * b2 + m03 * b1) / det;
        matrix(1,2) = (-m30 * a5 + m32 * a2 - m33 * a1) / det;
        matrix(1,3) = (m20 * a5 - m22 * a2 + m23 * a1) / det;
        matrix(2,0) = (m10 * b4 - m11 * b2 + m13 * b0) / det;
        matrix(2,1) = (-m00 * b4 + m01 * b2 - m03 * b0) / det;
        matrix(2,2) = (m30 * a4 - m31 * a2 + m33 * a0) / det;
        matrix(2,3) = (-m20 * a4 + m21 * a2 - m23 * a0) / det;
        matrix(3,0) = (-m10 * b3 + m11 * b1 - m12 * b0) / det;
        matrix(3,1) = (m00 * b3 - m01 * b1 + m02 * b0) / det;
        matrix(3,2) = (-m30 * a3 + m31 * a1 - m32 * a0) / det;
        matrix(3,3) = (m20 * a3 - m21 * a1 + m22 * a0) / det;

        if (invertibility) *invertibility = true;

    } else if (invertibility) {
        *invertibility = false;
    }

    return matrix;
}


template<typename T, size_t Rows, size_t Cols>
auto inversenxn(const Matrix<T, Rows, Cols> &mat,
                bool *invertibility) -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix(mat);

    //LuDecomposition<Matrix<T, Rows, Cols>> lu(mat);

    // Por ahora lo comento hasta que renombre Matrix
    //T det = lu.determinant();
    T det = determinantnxn(mat);
    if (det != consts::zero<T>) {

        //auto indentity = Matrix<T, Rows, Cols>::identity(mat.rows(), mat.cols());
        //matrix = lu.solve(indentity);
        matrix = adjointnxn(mat);
        matrix /= det;

        if (invertibility != nullptr) *invertibility = true;

    } else if (invertibility != nullptr) {
        *invertibility = false;
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto invert_matrix(const Matrix<T, Rows, Cols> &mat, 
                   bool *invertibility) -> Matrix<T, Rows, Cols>
{
    // Verificación en tiempo de compilación (si es estática)
    static_assert(Rows == DynamicData || Cols == DynamicData || Rows == Cols,
        "Static matrices must be square to be invertible");
    static_assert(std::is_floating_point_v<T>, "Integral types not supported");

    // Verificación en tiempo de ejecución (por si es dinámica)
    TL_ASSERT(mat.rows() == mat.cols(), "Matrices must be square");

    if constexpr (Rows == 2 && Cols == 2) {
        return inverse2x2(mat, invertibility);
    } else if constexpr (Rows == 3 && Cols == 3) {
        return inverse3x3(mat, invertibility);
    } else if constexpr (Rows == 4 && Cols == 4) {
        return inverse4x4(mat, invertibility);
    } else {
        if (mat.rows() == 2) 
            return inverse2x2(mat, invertibility);
        if (mat.rows() == 3) 
            return inverse3x3(mat, invertibility);
        if (mat.rows() == 4) 
            return inverse4x4(mat, invertibility);

        return inversenxn(mat, invertibility);
    }
}

} // namespace detail

} // namespace tl