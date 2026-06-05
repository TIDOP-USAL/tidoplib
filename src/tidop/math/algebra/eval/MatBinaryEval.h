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

/*! \file MatBinaryEval.h
 * \brief Evaluator specialization for matrix binary expressions.
 *
 * This file provides the `Evaluator` specialization for `MatBinaryExpr`.
 * It enables efficient evaluation of element-wise binary operations
 * (e.g., addition, subtraction) on matrices by delegating to the evaluators
 * of the left‑hand side (LHS) and right‑hand side (RHS) expressions.
 *
 * This evaluator is used internally by the matrix expression template system
 * to obtain coefficients and SIMD packets without constructing intermediate
 * matrices.
 *
 * \ingroup Evaluators
 * \see tl::MatBinaryExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/MatBinaryExpr.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `MatBinaryExpr` (binary operation on two matrix expressions).
 *
 * \tparam LHS Left-hand side expression type.
 * \tparam RHS Right-hand side expression type.
 * \tparam Op  Binary operation type (e.g., `AddOp`, `SubOp`).
 *
 * This evaluator computes the element-wise result of applying `Op` to the
 * coefficients of the LHS and RHS expressions. It stores evaluators for both
 * sub‑expressions and forwards `coeff()` and `packet()` calls after applying
 * the operation.
 */
template<typename LHS, typename RHS, typename Op>
class Evaluator<MatBinaryExpr<LHS, RHS, Op>>
{

private:

    Evaluator<LHS> mLhs; /*!< Evaluator for the left-hand side expression. */
    Evaluator<RHS> mRhs; /*!< Evaluator for the right-hand side expression. */
    Op mOp;              /*!< Binary operation functor. */

public:

    using value_type = typename MatBinaryExpr<LHS, RHS, Op>::value_type;

public:

    /*!
     * \brief Constructs an evaluator from a `MatBinaryExpr`.
     * \param[in] expr The source binary expression.
     */
    constexpr Evaluator(const MatBinaryExpr<LHS, RHS, Op> &expr)
      : mLhs(expr.lhs()),
        mRhs(expr.rhs())
    {
    }

    /*!
     * \brief Returns the element at position (r, c) (matrix version).
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return The result of `Op(lhs(r,c), rhs(r,c))`.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t r, size_t c) const -> value_type
    {
        return mOp(mLhs.coeff(r, c), mRhs.coeff(r, c));
    }

    /*!
     * \brief Returns the element at linear index i (vector version).
     * \param[in] i Linear index.
     * \return The result of `Op(lhs(i), rhs(i))`.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t i) const -> value_type
    {
        return mOp(mLhs.coeff(i), mRhs.coeff(i));
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet starting at index i.
     * \param[in] i Linear index.
     * \return The result of `Op(lhs.packet(i), rhs.packet(i))` as a `Packed<T>`.
     * \note Only available when SIMD intrinsics are enabled.
     */
    [[nodiscard]]
    auto packet(size_t i) const
    {
        return mOp(mLhs.packet(i), mRhs.packet(i));
    }
#endif

};


/*! \} */

} // End namespace tl