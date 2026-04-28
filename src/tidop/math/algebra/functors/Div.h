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

/*! \addtogroup Functors
 *  \{
 */

struct DivOp
{

    template<typename T>
    constexpr T operator()(const T &a, const T &b) const
    {
        return a / b;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    template<typename T>
    auto operator()(const Packed<T> &a, const Packed<T> &a2) const
    {
        if constexpr (std::is_integral_v<T>) {
            // SIMD no válido -> fallback escalar
            constexpr size_t size = PackedTraits<Packed<T>>::size;
            std::array<T, size> a_array;
            std::array<T, size> a2_array;
            a.store(a_array.data());
            a2.store(a2_array.data());
            std::array<T, size> result_array;
            for (size_t i = 0; i < size; ++i) {
                result_array[i] = a_array[i] / a2_array[i];
            }
            Packed<T> result;
            result.load(result_array.data());
            return result;
        } else {
            return a / a2;
        }
    }
#endif

};

/*! \} */

} // End namespace tl