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

/*! \file VecUnaryMinusEval.h
 * \brief Evaluator specialization for vector unary minus expressions.
 *
 * This file provides the `Evaluator` specialization for `VecUnaryMinusExpr`,
 * which represents the negation of a vector expression (i.e., `-vector`).
 * The evaluator simply forwards coefficient access after applying the unary
 * minus operator. Both scalar coefficient access and SIMD packet operations
 * are supported.
 *
 * \ingroup Evaluators
 * \see tl::VecUnaryMinusExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/VecUnaryMinusExpr.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `VecUnaryMinusExpr<Expr>`.
 *
 * \tparam Expr The vector expression type being negated.
 *
 * This evaluator stores an evaluator for the underlying expression and
 * returns the negative of its coefficients. It is a thin wrapper that
 * transforms access to the sub‑expression by applying `operator-`.
 */
template<typename Expr>
class Evaluator<VecUnaryMinusExpr<Expr>>
{

private:

    Evaluator<Expr> mExpr;

public:

    using value_type = typename VecUnaryMinusExpr<Expr>::value_type;

public:

    /*!
     * \brief Constructs the evaluator from a `VecUnaryMinusExpr`.
     * \param[in] expr The source unary minus expression.
     */
    Evaluator(const VecUnaryMinusExpr<Expr> &expr)
      : mExpr(expr.expr())
    {
    }

    /*!
     * \brief Returns the element at linear index i after negation.
     * \param[in] i Element index.
     * \return `-coeff` of the underlying expression at the same index.
     */
    auto coeff(size_t i) const -> value_type
    {
        return -mExpr.coeff(i);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients starting at index i, after negation.
     * \param[in] i Linear index.
     * \return A `Packed<T>` containing the negated coefficients.
     * \note Only available when SIMD intrinsics are enabled.
     */
    auto packet(size_t i) const
    {
        return -mExpr.packet(i);
    }
#endif

};


/*! \} */

} // End namespace tl