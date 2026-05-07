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

#include "tidop/math/base/Simd.h"

/// \cond

namespace tl
{

namespace detail
{

template<typename LHS, typename RHS, typename Out>
void mat_vec_mul_simd(const LHS &matrix, const RHS &vectorIn, Out &vectorOut)
{
    using T = typename matrix_traits<LHS>::value_type;

    size_t rows = matrix.rows();
    size_t cols = matrix.cols();

    constexpr size_t packed_size = Packed<T>::size();
    size_t max_vector = cols - (cols % packed_size);

    for (size_t r = 0; r < rows; ++r) {
        T row_sum = 0;

        for (size_t i = 0; i < max_vector; i += packed_size) {

            Packed<T> packed_a = vectorIn.packet(i);
            Packed<T> packed_b = matrix.packet(r * cols + i);

            Packed<T> packed_c = packed_a * packed_b;
            row_sum += packed_c.sum();
        }

        for (size_t i = max_vector; i < cols; ++i) {
            row_sum += matrix(r, i) * vectorIn[i];
        }

        vectorOut[r] = row_sum;
    }
}

} // namespace detail

} // namespace tl

/// \endcond