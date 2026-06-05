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

/*! \file TransposeExpr.h
 * \brief Expression template for matrix transpose.
 *
 * This file defines the `TransposeExpr` class, which represents a lazy
 * transpose of a matrix expression (i.e., `matrix.transpose()`). The
 * transposition is not performed immediately; instead, the expression object
 * stores a reference to the original expression and swaps the row and column
 * counts. Evaluation occurs only when the expression is assigned to a concrete
 * matrix or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::MatrixBase, tl::MatUnaryMinusExpr, tl::EvaluatorTransposeExpr
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
 * \class TransposeExpr
 * \brief Lazy expression for the transpose of a matrix expression.
 *
 * \tparam Expr The matrix expression type to be transposed.
 *
 * This class represents the transpose of a matrix expression: `A^T`.
 * The number of rows of the result equals the number of columns of the
 * original, and the number of columns of the result equals the number of
 * rows of the original. The actual transposition is not performed at
 * construction time; it is stored as an expression template for later
 * evaluation. When the transpose is eventually assigned to a concrete matrix,
 * the elements are copied with swapped indices.
 *
 * ### Example
 * \code
 * Matrix<double,3,4> A;
 * auto trans_expr = TransposeExpr(A);   // lazy: A^T
 * Matrix<double,4,3> B = trans_expr;    // evaluated here
 * \endcode
 */
template<typename Expr>
class TransposeExpr 
  : public MatrixBase<TransposeExpr<Expr>>
{

private:

    const Expr &mExpr;

public:

    using value_type = typename matrix_traits<Expr>::value_type;

public:

    /*!
     * \brief Constructs a transpose expression from a matrix expression.
     * \param[in] expr The expression to transpose.
     */
    constexpr TransposeExpr(const Expr &expr)
      : mExpr(expr) 
    {}

    /*! \brief Returns the number of rows of the transposed matrix (original columns). */
    [[nodiscard]] 
    constexpr auto rows() const noexcept -> size_t { return mExpr.cols(); }

    /*! \brief Returns the number of columns of the transposed matrix (original rows). */
    [[nodiscard]] 
    constexpr auto cols() const noexcept -> size_t { return mExpr.rows(); }

    /*! \brief Returns the underlying expression. */
    [[nodiscard]]
    constexpr auto expr() const -> const Expr & { return mExpr; }

    /*!
     * \brief Checks if the underlying expression's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the expression aliases `ptr`.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const
    {
        return mExpr.aliases(ptr);
    }
};

/*! \} */

} // End namespace tl