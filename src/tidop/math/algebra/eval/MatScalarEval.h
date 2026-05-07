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

/*! \file MatScalarEval.h
 * \brief Evaluator specialization for matrix–scalar binary expressions.
 *
 * This file provides the `Evaluator` specialization for `MatScalarExpr`, which
 * represents a binary operation between a matrix expression and a scalar value
 * (e.g., `matrix * scalar`, `matrix / scalar`). The evaluator combines the
 * coefficients of the matrix expression with the scalar using the specified
 * operation, supporting both scalar coefficient access and SIMD packet
 * operations when available.
 *
 * \ingroup Evaluators
 * \see tl::MatScalarExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/MatScalarExpr.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `MatScalarExpr<LHS, Scalar, Op>`.
 *
 * \tparam LHS    Left‑hand side matrix expression type (must satisfy `MatrixExpr`).
 * \tparam Scalar Scalar type (e.g., `double`, `int`).
 * \tparam Op     Binary operation functor (e.g., `MulOp`, `DivOp`).
 *
 * This evaluator stores an evaluator for the left‑hand side expression and a
 * copy of the scalar value. The `coeff()` methods return the result of applying
 * `Op` to the LHS coefficient and the scalar. For SIMD, the scalar is broadcast
 * to a packet and the operation is performed element‑wise.
 */
template<typename LHS, typename Scalar, typename Op>
class Evaluator<MatScalarExpr<LHS, Scalar, Op>>
{

private:

    Evaluator<LHS> mLhs;    /*!< Evaluator for the left‑hand side expression. */
    Scalar mScalar;         /*!< Scalar value. */
    Op mOp;                 /*!< Binary operation functor. */
	
public:

    using value_type = typename MatScalarExpr<LHS, Scalar, Op>::value_type;

public:

    /*!
     * \brief Constructs the evaluator from a `MatScalarExpr`.
     * \param[in] expr The source scalar expression.
     */
    Evaluator(const MatScalarExpr<LHS, Scalar, Op> &expr)
      : mLhs(expr.lhs()),
        mScalar(expr.scalar())
    {
    }

    /*!
     * \brief Returns the element at matrix position (r, c).
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return The result of `Op(lhs(r,c), scalar)`.
     */
    auto coeff(size_t r, size_t c) const -> value_type
    {
        return mOp(mLhs.coeff(r, c), mScalar);
    }

    /*!
     * \brief Returns the element at linear index i (row‑major order).
     * \param[in] i Linear index.
     * \return The result of `Op(lhs(i), scalar)`.
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