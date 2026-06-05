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

/*! \file DiagonalMatrixExpr.h
 * \brief Expression for constructing a diagonal matrix from a vector.
 *
 * This file defines the `DiagonalMatrixExpr` class, an expression template that
 * represents a square diagonal matrix whose diagonal entries are taken from a
 * vector expression. The off‑diagonal entries are zero. This expression can be
 * used in matrix arithmetic and will be evaluated lazily.
 *
 * \ingroup Expressions
 * \see tl::MatrixBase, tl::Vector
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
 * \class DiagonalMatrixExpr
 * \brief Expression representing a diagonal matrix constructed from a vector.
 *
 * \tparam Expr A type satisfying `VectorExpr` (the vector expression for the diagonal).
 *
 * This expression creates a square matrix where the diagonal entries are the
 * elements of the vector expression `expr`, and all off‑diagonal entries are zero.
 * The size of the matrix is `expr.size()` × `expr.size()`.
 *
 * This class is an expression; it does not store the matrix coefficients explicitly,
 * only a reference to the vector expression. Therefore, it is lightweight and
 * intended to be used as a temporary in expression templates. The actual matrix
 * is materialised when evaluated (e.g., when assigned to a `Matrix` object).
 *
 * ### Example
 * \code
 * Vector<double, 3> v = {1, 2, 3};
 * auto D = DiagonalMatrixExpr(v);  // represents diag(1,2,3)
 * Matrix<double, 3, 3> M = D;      // evaluates to a concrete diagonal matrix
 * \endcode
 */
template<typename Expr>
class DiagonalMatrixExpr 
  : public MatrixBase<DiagonalMatrixExpr<Expr>>
{

private:

    const Expr &mExpr; /*!< Reference to the vector expression providing the diagonal. */

public:

    using value_type = typename vector_traits<Expr>::value_type;

    /*!
     * \brief Constructor from a vector expression.
     * \param[in] expr The vector expression for the diagonal entries.
     */
    constexpr DiagonalMatrixExpr(const Expr &expr)
      : mExpr(expr)
    {
    }

    /*!
     * \brief Returns the number of rows (which equals the vector size).
     */
    [[nodiscard]]
    constexpr auto rows() const noexcept -> size_t { return mExpr.size(); }

    /*!
     * \brief Returns the number of columns (which equals the vector size).
     */
    [[nodiscard]]
    constexpr auto cols() const noexcept -> size_t { return mExpr.size(); }

    /*!
     * \brief Returns the underlying vector expression.
     * \return Const reference to the stored vector expression.
     */
    [[nodiscard]] 
    constexpr auto expr() const -> const Expr & { return mExpr; }

    /*!
     * \brief Checks whether the expression's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the underlying vector expression aliases `ptr`.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool
    {
        return mExpr.aliases(ptr);
    }

};

/*! \} */

} // End namespace tl