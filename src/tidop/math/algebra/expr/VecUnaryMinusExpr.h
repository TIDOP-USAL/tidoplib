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

/*! \file VecUnaryMinusExpr.h
 * \brief Expression template for vector unary minus.
 *
 * This file defines the `VecUnaryMinusExpr` class, which represents a lazy
 * unary negation of a vector expression (i.e., `-vector`). The operation is
 * not performed immediately; instead, the expression object stores a reference
 * to the operand expression and provides metadata (size) and access to the
 * sub‑expression. Evaluation occurs only when the expression is assigned to a
 * concrete vector or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::VectorBase, tl::VecBinaryExpr, tl::EvaluatorVecUnaryMinusExpr
 */

#pragma once

#include "tidop/math/base/Traits.h"
#include "tidop/math/base/Concepts.h"

namespace tl
{

template<typename Derived>
class VectorBase;

/*! \addtogroup Expressions
 *  \{
 */

/*!
 * \class VecUnaryMinusExpr
 * \brief Lazy expression for unary minus (negation) of a vector expression.
 *
 * \tparam Expr The vector expression type to be negated.
 *
 * This class represents the negation of a vector expression: `-v`.
 * The size of the result is the same as the original expression.
 * The actual negation is not performed at construction time; it is stored
 * as an expression template for later evaluation.
 *
 * ### Example
 * \code
 * Vector<double,3> v = {1, 2, 3};
 * auto neg_expr = VecUnaryMinusExpr(v);   // lazy: -v
 * Vector<double,3> w = neg_expr;          // evaluated here ⇒ w = {-1, -2, -3}
 * \endcode
 */
template<typename Expr>
class VecUnaryMinusExpr 
  : public VectorBase<VecUnaryMinusExpr<Expr>>
{

private:

    const Expr &mExpr;

public:

    using value_type = typename vector_traits<Expr>::value_type;

public:

    /*!
     * \brief Constructs a unary minus expression from a vector expression.
     * \param[in] expr The expression to negate.
     */
    constexpr VecUnaryMinusExpr(const Expr &expr)
      : mExpr(expr) 
    {}

    /*! \brief Returns the number of elements (same as original). */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t { return mExpr.size(); }

    /*! \brief Returns the underlying expression. */
    [[nodiscard]]
    constexpr auto expr() const -> const Expr & { return mExpr; }

    /*!
     * \brief Checks if the underlying expression's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the expression aliases `ptr`.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool
    {
        return mExpr.aliases(ptr);
    }

};

/*! \} */

} // End namespace tl