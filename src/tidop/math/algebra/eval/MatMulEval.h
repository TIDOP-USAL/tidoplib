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

/*! \file MatMulEval.h
 * \brief Evaluator specialization for matrix multiplication expressions.
 *
 * This file provides the `Evaluator` specialization for `MatMulExpr`, which
 * represents the product of two matrix expressions. Unlike binary element‑wise
 * operations, matrix multiplication is not evaluated lazily coefficient by
 * coefficient; instead, this evaluator pre‑computes the entire product into
 * a temporary matrix and then provides coefficient access from that temporary.
 * This is necessary because matrix multiplication involves a reduction
 * (inner products) and cannot be expressed as a simple function of individual
 * coefficients of the operands.
 *
 * \ingroup Evaluators
 * \see tl::MatMulExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/MatMulExpr.h"
#include "tidop/math/algebra/matrix/detail/MulMat.h"

namespace tl
{

template<typename T, size_t Rows, size_t Cols>
class Matrix;

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `MatMulExpr` (matrix multiplication of two expressions).
 *
 * \tparam LHS Left‑hand side matrix expression type.
 * \tparam RHS Right‑hand side matrix expression type.
 *
 * This evaluator computes the product of the LHS and RHS expressions at
 * construction time (eager evaluation) and stores the result in an internal
 * dynamic matrix (`mTemp`). Subsequent calls to `coeff()` or `packet()` simply
 * read from this pre‑computed matrix.
 *
 * \note Because the product is evaluated immediately, using `MatMulExpr` in
 *       an expression chain will not defer the multiplication; it will be
 *       performed as soon as the evaluator is created (typically when the
 *       expression is assigned or used in a context that requires evaluation).
 *       This is acceptable because matrix multiplication is computationally
 *       expensive and caching the result avoids recomputation.
 */
template<typename LHS, typename RHS>
class Evaluator<MatMulExpr<LHS,RHS>>
{

public:

    using value_type = typename MatMulExpr<LHS,RHS>::value_type;

private:

    Matrix<value_type, DynamicData, DynamicData> mTemp;
	
public:

    /*!
     * \brief Constructs the evaluator and immediately computes the product.
     * \param[in] expr The `MatMulExpr` to evaluate.
     *
     * The dimensions of the temporary matrix are taken from the expression
     * (`expr.rows()`, `expr.cols()`). The actual multiplication is performed
     * by calling `detail::mulmat` on the evaluated left and right operands.
     */
    constexpr Evaluator(const MatMulExpr<LHS,RHS> &expr)
      : mTemp(expr.rows(), expr.cols())
    {
        if (std::is_constant_evaluated()) {
            //decltype(auto) a = detail::require_linear_access(expr.lhs());
            //decltype(auto) b = detail::require_linear_access(expr.rhs());
            detail::mulmat_cpp(expr.lhs().eval(), expr.rhs().eval(), mTemp);
        } else {
            detail::mulmat(expr.lhs().eval(), expr.rhs().eval(), mTemp);
        }
    }

    /*!
     * \brief Returns the coefficient at row `r`, column `c` from the pre‑computed product.
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return The product coefficient.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t r, size_t c) const -> value_type
    {
        return mTemp(r,c);
    }

    /*!
     * \brief Returns the coefficient at linear index `i` (row‑major order).
     * \param[in] i Linear index.
     * \return The product coefficient.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t i) const -> value_type
    {
        return mTemp(i);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients starting at linear index `i`.
     * \param[in] i Linear index.
     * \return A `Packed<T>` containing the coefficients from the temporary matrix.
     * \note Only available when SIMD intrinsics are enabled.
     */
    [[nodiscard]]
    auto packet(size_t i) const
    {
        return mTemp.packet(i);
    }
#endif
};


/*! \} */

} // End namespace tl