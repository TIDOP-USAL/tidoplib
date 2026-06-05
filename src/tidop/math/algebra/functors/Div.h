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

/*! \file Div.h
 * \brief Functor for division operation in expression templates.
 *
 * This file defines the `DivOp` functor, which performs element‑wise division
 * on scalar values and handles SIMD packets with a fallback for integral types
 * where SIMD division is unavailable. It is used internally by expression
 * templates such as `VecBinaryExpr` and `MatBinaryExpr` to represent division.
 *
 * \ingroup Functors
 * \see tl::AddOp, tl::SubOp, tl::MulOp
 */

#pragma once

namespace tl
{

/*! \addtogroup Functors
 *  \{
 */

/*!
 * \struct DivOp
 * \brief Functor that applies division to its arguments.
 *
 * This functor is stateless. For scalar types it performs direct division.
 * For SIMD packets, when the element type is floating-point, vectorised division
 * is used; for integral types a scalar fallback is employed because many SIMD
 * instruction sets do not support integer division natively.
 *
 * ### Example
 * \code
 * DivOp div;
 * double result = div(12.0, 3.0); // result == 4.0
 * \endcode
 */
struct DivOp
{

    /*!
     * \brief Divides two scalar values.
     * \tparam T Arithmetic type (deduced).
     * \param[in] a Numerator.
     * \param[in] b Denominator.
     * \return The quotient `a / b`.
     */
    template<typename T>
    [[nodiscard]]
    constexpr auto operator()(const T &a, const T &b) const -> T
    {
        return a / b;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Divides two SIMD packets element‑wise (with fallback for integers).
     * \tparam T Element type (deduced from `Packed<T>`).
     * \param[in] a Numerator packet.
     * \param[in] b Denominator packet.
     * \return A packet containing the element‑wise quotient.
     *
     * For floating‑point types, this uses the SIMD division operator.
     * For integral types, it falls back to scalar division on each element
     * due to lack of native SIMD integer division in many ISAs.
     */
    template<typename T>
    [[nodiscard]]
    auto operator()(const Packed<T> &a, const Packed<T> &a2) const -> Packed<T>
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