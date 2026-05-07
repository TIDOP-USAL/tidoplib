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

/*! \file VecBinaryEval.h
 * \brief Evaluator specialization for vector binary expressions.
 *
 * This file provides the `Evaluator` specialization for `VecBinaryExpr`, which
 * represents an element‑wise binary operation between two vector expressions
 * (e.g., addition, subtraction, component‑wise multiplication/division).
 * The evaluator delegates coefficient access and SIMD packet operations to
 * the evaluators of the left‑hand side (LHS) and right‑hand side (RHS)
 * sub‑expressions, applying the binary operation to the results.
 *
 * \ingroup Evaluators
 * \see tl::VecBinaryExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/VecBinaryExpr.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `VecBinaryExpr` (binary operation on two vector expressions).
 *
 * \tparam LHS Left‑hand side vector expression type.
 * \tparam RHS Right‑hand side vector expression type.
 * \tparam Op  Binary operation functor (e.g., `AddOp`, `SubOp`).
 *
 * This evaluator stores evaluators for both sub‑expressions and forwards
 * `coeff()` and `packet()` calls after applying the operation `Op` to the
 * corresponding coefficients or packets. It is used internally to evaluate
 * vector expressions without constructing temporary vectors.
 */
template<typename LHS, typename RHS, typename Op>
class Evaluator<VecBinaryExpr<LHS, RHS, Op>>
{

private:

    Evaluator<LHS> mLhs; /*!< Evaluator for the left‑hand side expression. */
    Evaluator<RHS> mRhs; /*!< Evaluator for the right‑hand side expression. */
    Op mOp;              /*!< Binary operation functor. */

public:

    using value_type = typename VecBinaryExpr<LHS, RHS, Op>::value_type;

public:

    /*!
     * \brief Constructs the evaluator from a `VecBinaryExpr`.
     * \param[in] expr The source binary expression.
     */
    Evaluator(const VecBinaryExpr<LHS, RHS, Op> &expr)
      : mLhs(expr.lhs()),
        mRhs(expr.rhs())
    {
    }

    /*!
     * \brief Returns the element at linear index i.
     * \param[in] i Element index.
     * \return The result of `Op(lhs[i], rhs[i])`.
     */
    auto coeff(size_t i) const -> value_type
    {
        return mOp(mLhs.coeff(i), mRhs.coeff(i));
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients starting at index i.
     * \param[in] i Linear index.
     * \return The result of applying `Op` to the corresponding packets of LHS and RHS.
     * \note Only available when SIMD intrinsics are enabled.
     */
    auto packet(size_t i) const
    {
        return mOp(mLhs.packet(i), mRhs.packet(i));
    }
#endif

};


/*! \} */

} // End namespace tl