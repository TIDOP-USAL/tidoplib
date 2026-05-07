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

/*! \file TransposeEval.h
 * \brief Evaluator specialization for matrix transpose expressions.
 *
 * This file provides the `Evaluator` specialization for `TransposeExpr`,
 * which represents the transpose of a matrix expression. The evaluator
 * reorders the indices when accessing coefficients: `coeff(r, c)` returns
 * the coefficient of the original expression at position `(c, r)`, and
 * linear indexing is adjusted accordingly. SIMD packet access is **not**
 * supported because the transpose introduces a non‑contiguous memory access
 * pattern that cannot be efficiently vectorised in a generic way.
 *
 * \ingroup Evaluators
 * \see tl::TransposeExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/TransposeExpr.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `TransposeExpr<Expr>`.
 *
 * \tparam Expr The matrix expression type being transposed.
 *
 * This evaluator stores an evaluator for the underlying expression and the
 * number of columns of the transposed matrix (i.e., the original number of
 * rows). For coefficient access, it swaps the row and column indices when
 * forwarding to the sub‑expression. Linear indexing is converted back to
 * 2D coordinates using the stored column count, then the indices are swapped.
 *
 * \note SIMD packet access is not implemented because transposed data is
 *       not stored contiguously in memory; attempting to use `packet()` will
 *       trigger an assertion.
 */
template<typename Expr>
class Evaluator<TransposeExpr<Expr>>
{

public:

    using value_type = typename matrix_traits<Expr>::value_type;

private:
	
    Evaluator<Expr> mExpr; /*!< Evaluator for the original expression. */
    size_t mCols;          /*!< Number of columns of the transposed matrix = rows of the original. */

public:

    /*!
     * \brief Constructs the evaluator from a `TransposeExpr`.
     * \param[in] expr The transpose expression.
     */
    Evaluator(const TransposeExpr<Expr>& expr)
      : mExpr(expr.expr()),
        mCols(expr.cols())
    {}

    /*!
     * \brief Returns the element at matrix position (r, c) of the transposed matrix.
     * \param[in] r Row index in the transposed matrix.
     * \param[in] c Column index in the transposed matrix.
     * \return The coefficient from the original expression at position (c, r).
     */
    auto coeff(size_t r, size_t c) const -> value_type
    {
        return mExpr.coeff(c, r);
    }

    /*!
     * \brief Returns the element at linear index i (row‑major order of the transposed matrix).
     * \param[in] i Linear index.
     * \return The coefficient from the original expression at the transposed position.
     *
     * This method computes the 2D coordinates from the linear index using the
     * stored number of columns (`mCols`), swaps row and column, and forwards
     * the call to the underlying evaluator.
     */
    auto coeff(size_t i) const -> value_type
    {
        size_t c = i % mCols;
        size_t r = i / mCols;

        return mExpr.coeff(c, r);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief SIMD packet access (not supported).
     * \param[in] i Linear index.
     * \return A dummy `Packed<value_type>`.
     *
     * \note This method always triggers an assertion because the data of a
     *       transposed matrix is not stored contiguously, making efficient
     *       vectorised access impossible in a generic way.
     */
    auto packet(size_t i) const
    {
        TL_ASSERT(false, "SIMD packet not supported for this expression");
        return Packed<value_type>();
    }
#endif

};

/*! \} */

} // End namespace tl