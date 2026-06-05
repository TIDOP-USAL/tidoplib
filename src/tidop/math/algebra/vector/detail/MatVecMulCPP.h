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

namespace detail
{

template<typename LHS, typename RHS, typename Out>
constexpr void mat_vec_mul_cpp(const LHS &matrix, const RHS &vectorIn, Out &vectorOut)
{
    using T = typename matrix_traits<LHS>::value_type;

    size_t rows = matrix.rows();
    size_t cols = matrix.cols();

    for (size_t r = 0; r < rows; r++) {
        T row_sum = 0;
        for (size_t c = 0; c < cols; c++) {
            row_sum += matrix(r, c) * vectorIn[c];
        }
        vectorOut[r] = row_sum;
    }
}

} // namespace detail

} // namespace tl

/// \endcond