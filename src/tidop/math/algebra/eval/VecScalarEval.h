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

/*! \file VecScalarEval.h
 * \brief Evaluator specialization for vector–scalar binary expressions.
 *
 * This file provides the `Evaluator` specialization for `VecScalarExpr`, which
 * represents a binary operation between a vector expression and a scalar value
 * (e.g., `vector * scalar`, `vector / scalar`). The evaluator combines the
 * coefficients of the vector expression with the scalar using the specified
 * operation, supporting both scalar coefficient access and SIMD packet
 * operations when available.
 *
 * \ingroup Evaluators
 * \see tl::VecScalarExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/VecScalarExpr.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `VecScalarExpr<LHS, Scalar, Op>`.
 *
 * \tparam LHS    Left‑hand side vector expression type (must satisfy `VectorExpr`).
 * \tparam Scalar Scalar type (e.g., `double`, `int`).
 * \tparam Op     Binary operation functor (e.g., `MulOp`, `DivOp`).
 *
 * This evaluator stores an evaluator for the left‑hand side expression and a
 * copy of the scalar value. The `coeff()` method returns the result of applying
 * `Op` to the LHS coefficient and the scalar. For SIMD, the scalar is broadcast
 * to a packet and the operation is performed element‑wise.
 */
template<typename LHS, typename Scalar, typename Op>
class Evaluator<VecScalarExpr<LHS, Scalar, Op>>
{

private:

    Evaluator<LHS> mLhs;    /*!< Evaluator for the left‑hand side expression. */
    Scalar mScalar;         /*!< Scalar value. */
    Op mOp;                 /*!< Binary operation functor. */
	
public:

    using value_type = typename VecScalarExpr<LHS, Scalar, Op>::value_type;
    
public:

    /*!
     * \brief Constructs the evaluator from a `VecScalarExpr`.
     * \param[in] expr The source scalar expression.
     */
    Evaluator(const VecScalarExpr<LHS, Scalar, Op> &expr)
      : mLhs(expr.lhs()),
        mScalar(expr.scalar())
    {
    }

    /*!
     * \brief Returns the element at linear index i.
     * \param[in] i Element index.
     * \return The result of `Op(lhs[i], scalar)`.
     */
    auto coeff(size_t i) const -> value_type
    {
        return mOp(mLhs.coeff(i), mScalar);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients starting at index i.
     * \param[in] i Linear index.
     * \return The result of broadcasting the scalar to a packet and applying
     *         `Op` to the LHS packet.
     * \note Only available when SIMD intrinsics are enabled.
     */
    auto packet(size_t i) const
    {
        return mOp(mLhs.packet(i), Packed<value_type>(mScalar));
    }
#endif

};


/*! \} */

} // End namespace tl