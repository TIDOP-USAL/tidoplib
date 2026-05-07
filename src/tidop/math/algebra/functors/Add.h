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

/*! \file Add.h
 * \brief Functor for addition operation in expression templates.
 *
 * This file defines the `AddOp` functor, which performs element‑wise addition
 * on scalar values and SIMD packets when available. It is used internally by
 * expression templates such as `VecBinaryExpr` and `MatBinaryExpr` to represent
 * the addition operation.
 *
 * \ingroup Functors
 * \see tl::SubOp, tl::MulOp, tl::DivOp
 */

#pragma once

namespace tl
{

/*! \addtogroup Functors
 *  \{
 */

/*!
 * \struct AddOp
 * \brief Functor that applies addition to its arguments.
 *
 * This functor is stateless and can be used with both scalar types and
 * SIMD packet types (if `TL_HAVE_SIMD_INTRINSICS` is defined). It is a
 * template parameter for expression classes that need to perform addition.
 *
 * ### Example
 * \code
 * AddOp add;
 * double result = add(3.0, 4.0); // result == 7.0
 * \endcode
 */
struct AddOp
{

    /*!
     * \brief Adds two scalar values.
     * \tparam T Arithmetic type (deduced).
     * \param[in] a First operand.
     * \param[in] b Second operand.
     * \return The sum `a + b`.
     */
    template<typename T>
    constexpr T operator()(const T &a, const T &b) const
    {
        return a + b;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Adds two SIMD packets element‑wise.
     * \tparam T Element type (deduced from `Packed<T>`).
     * \param[in] a First packet.
     * \param[in] b Second packet.
     * \return A packet containing the element‑wise sum.
     * \note This overload is only available when SIMD intrinsics are enabled.
     */
    template<typename T>
    auto operator()(const Packed<T> &a, const Packed<T> &b) const
    {
        return a + b;
    }
#endif

};

/*! \} */

} // End namespace tl