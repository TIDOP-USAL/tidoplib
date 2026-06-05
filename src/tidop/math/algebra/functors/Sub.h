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

/*! \file Sub.h
 * \brief Functor for subtraction operation in expression templates.
 *
 * This file defines the `SubOp` functor, which performs element‑wise subtraction
 * on scalar values and SIMD packets when available. It is used internally by
 * expression templates such as `VecBinaryExpr` and `MatBinaryExpr` to represent
 * the subtraction operation.
 *
 * \ingroup Functors
 * \see tl::AddOp, tl::MulOp, tl::DivOp
 */

#pragma once

namespace tl
{

/*! \addtogroup Functors
 *  \{
 */

/*!
 * \struct SubOp
 * \brief Functor that applies subtraction to its arguments.
 *
 * This functor is stateless and can be used with both scalar types and
 * SIMD packet types (if `TL_HAVE_SIMD_INTRINSICS` is defined). It is a
 * template parameter for expression classes that need to perform subtraction.
 *
 * ### Example
 * \code
 * SubOp sub;
 * double result = sub(5.0, 3.0); // result == 2.0
 * \endcode
 */
struct SubOp
{

    /*!
     * \brief Subtracts two scalar values.
     * \tparam T Arithmetic type (deduced).
     * \param[in] a First operand.
     * \param[in] b Second operand.
     * \return The difference `a - b`.
     */
    template<typename T>
    [[nodiscard]]
    constexpr auto operator()(const T &a, const T &b) const noexcept -> T
    {
        return a - b;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Subtracts two SIMD packets element‑wise.
     * \tparam T Element type (deduced from `Packed<T>`).
     * \param[in] a First packet.
     * \param[in] b Second packet.
     * \return A packet containing the element‑wise difference.
     * \note This overload is only available when SIMD intrinsics are enabled.
     */
    template<typename T>
    [[nodiscard]]
    auto operator()(const Packed<T> &a, const Packed<T> &b) const noexcept -> Packed<T>
    {
        return a - b;
    }
#endif

};

/*! \} */

} // End namespace tl