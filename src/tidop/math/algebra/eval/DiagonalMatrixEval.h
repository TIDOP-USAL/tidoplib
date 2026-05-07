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

/*! \file DiagonalMatrixEval.h
 * \brief Evaluator specialization for diagonal matrix expressions.
 *
 * This file provides the `Evaluator` specialization for `DiagonalMatrixExpr`,
 * which represents a diagonal matrix constructed from a vector expression.
 * The evaluator forwards coefficient access to the underlying vector evaluator,
 * interpreting the diagonal entries as a linear sequence. Note that this
 * evaluator only provides 1‑D coefficient access (`coeff(size_t i)`), suitable
 * for contexts where the diagonal matrix is used as a vector (e.g., in some
 * internal calculations). For matrix‑style coefficient access `coeff(r,c)`,
 * a different approach is required.
 *
 * \ingroup Evaluators
 * \see tl::DiagonalMatrixExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/DiagonalMatrixExpr.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `DiagonalMatrixExpr<Expr>`.
 *
 * \tparam Expr The vector expression type underlying the diagonal matrix.
 *
 * This evaluator stores an evaluator for the vector expression that provides
 * the diagonal entries. The `coeff(size_t i)` method returns the i‑th diagonal
 * element, effectively interpreting the diagonal matrix as a vector of its
 * diagonal entries. This is consistent with the view that a diagonal matrix can
 * be stored compactly as a vector. SIMD packet access is not supported.
 */
template<typename Expr>
class Evaluator<DiagonalMatrixExpr<Expr>>
{

private:

    Evaluator<Expr> mExpr; /*!< Evaluator for the underlying vector expression. */
    size_t mCols;

public:

    using value_type = typename DiagonalMatrixExpr<Expr>::value_type;

public:

    /*!
     * \brief Constructs the evaluator from a `DiagonalMatrixExpr`.
     * \param[in] expr The diagonal matrix expression.
     */
    Evaluator(const DiagonalMatrixExpr<Expr> &expr)
      : mExpr(expr.expr()),
        mCols(expr.cols())
    {
    }

    /*!
     * \brief Returns the element at position (r, c).
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return The coefficient at the given position.
     */
    auto coeff(size_t r, size_t c) const -> value_type
    {
        return (r == c) ? mExpr.coeff(r) : value_type(0);
    }

    /*!
     * \brief Returns the i‑th diagonal element (0‑based).
     * \param[in] i Index of the diagonal entry.
     * \return The coefficient at the given linear position.
     */
    auto coeff(size_t i) const -> value_type
    {
        size_t row = i / mCols;
        size_t col = i % mCols;

        return (row == col) ? mExpr.coeff(row) : value_type(0);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief SIMD packet access (not supported).
     * \param[in] i Linear index.
     * \return A dummy `Packed<value_type>`.
     *
     * \note This method always triggers an assertion because diagonal matrices
     *       are often used in non‑contiguous contexts and vectorised access
     *       is not implemented for this expression type.
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