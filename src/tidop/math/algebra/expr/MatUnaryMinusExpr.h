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

/*! \file MatUnaryMinusExpr.h
 * \brief Expression template for matrix unary minus.
 *
 * This file defines the `MatUnaryMinusExpr` class, which represents a lazy
 * unary negation of a matrix expression (i.e., `-matrix`). The operation is
 * not performed immediately; instead, the expression object stores a reference
 * to the operand expression and provides metadata (rows, columns) and access
 * to the sub‑expression. Evaluation occurs only when the expression is assigned
 * to a concrete matrix or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::MatrixBase, tl::MatBinaryExpr, tl::EvaluatorMatUnaryMinusExpr
 */

#pragma once

#include "tidop/math/base/Traits.h"
#include "tidop/math/base/Concepts.h"

namespace tl
{

template<typename Derived>
class MatrixBase;

/*! \addtogroup Expressions
 *  \{
 */

/*!
 * \class MatUnaryMinusExpr
 * \brief Lazy expression for unary minus (negation) of a matrix expression.
 *
 * \tparam Expr The matrix expression type to be negated.
 *
 * This class represents the negation of a matrix expression: `-A`.
 * The dimensions of the result are the same as the original expression.
 * The actual negation is not performed at construction time; it is stored
 * as an expression template for later evaluation.
 */
template<typename Expr>
class MatUnaryMinusExpr 
  : public MatrixBase<MatUnaryMinusExpr<Expr>>
{

private:

    const Expr &mExpr;

public:

    using value_type = typename matrix_traits<Expr>::value_type;

public:

    /*!
     * \brief Constructs a unary minus expression from a matrix expression.
     * \param[in] expr The expression to negate.
     */
    explicit MatUnaryMinusExpr(const Expr &expr)
      : mExpr(expr) 
    {}

    /*! \brief Returns the number of rows (same as original). */
    constexpr auto rows() const noexcept -> size_t { return mExpr.rows(); }

    /*! \brief Returns the number of columns (same as original). */
    constexpr auto cols() const noexcept -> size_t { return mExpr.cols(); }

    /*! \brief Returns the underlying expression. */
    auto expr() const -> const Expr & { return mExpr; }

    /*!
     * \brief Checks if the underlying expression's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the expression aliases `ptr`.
     */
    auto aliases(const void *ptr) const -> bool
    {
        return mExpr.aliases(ptr);
    }

};

/*! \} */

} // End namespace tl