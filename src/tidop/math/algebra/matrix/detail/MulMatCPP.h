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

namespace detail
{

template<MatrixExpr LHS, MatrixExpr RHS, typename OutMat>
void mulmat_cpp(const LHS &matrix1, const RHS &matrix2, OutMat &matrix)
{
    using T = typename matrix_traits<OutMat>::value_type;

    //for (size_t r = 0; r < matrix1.rows(); r++) {
    //    for (size_t i = 0; i < matrix1.cols(); i++) {
    //        T a = matrix1(r, i);
    //        for (size_t c = 0; c < matrix2.cols(); c++) {
    //            matrix(r, c) += a * matrix2(i, c);
    //        }
    //    }
    //}
    // Para no tener que inicializar la matriz de salida a 0
    for (size_t r = 0; r < matrix1.rows(); r++) {
        for (size_t c = 0; c < matrix2.cols(); c++) {
            T sum = 0;
            for (size_t i = 0; i < matrix1.cols(); i++) {
                sum += matrix1(r, i) * matrix2(i, c);
            }
            matrix(r, c) = sum;
        }
    }
}

} // namespace detail

} // namespace tl