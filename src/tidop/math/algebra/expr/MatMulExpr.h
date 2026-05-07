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

/*! \file MatMulExpr.h
 * \brief Expression template for matrix multiplication.
 *
 * This file defines the `MatMulExpr` class, which represents a lazy
 * matrix multiplication of two matrix expressions. The multiplication
 * is not performed immediately; instead, the expression object stores
 * references to its operands and provides metadata (rows, columns) and
 * access to the sub‑expressions. Evaluation occurs only when the expression
 * is assigned to a concrete matrix or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::MatrixBase, tl::MatBinaryExpr, tl::EvaluatorMatMulExpr
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
 * \class MatMulExpr
 * \brief Lazy expression for matrix multiplication.
 *
 * \tparam LHS Left‑hand side matrix expression type.
 * \tparam RHS Right‑hand side matrix expression type.
 *
 * This class represents the product of two matrix expressions `A * B`.
 * The dimensions must satisfy `A.cols() == B.rows()`. The resulting
 * expression has `A.rows()` rows and `B.cols()` columns.
 *
 * The actual multiplication is not performed at construction time;
 * instead, the expression stores references to the operands. When the
 * expression is evaluated (e.g., assigned to a `Matrix` object), the
 * product is computed using an optimised routine (see `EvaluatorMatMulExpr`).
 *
 * ### Example
 * \code
 * Matrix<double,3,4> A;
 * Matrix<double,4,5> B;
 * auto C_expr = MatMulExpr(A, B);   // lazy multiplication
 * Matrix<double,3,5> C = C_expr;    // evaluated here
 * // This is typically done using operators (internally, `MatMulExpr` is used)
 * // Matrix<double,3,5> C = A * B
 * \endcode
 */
template<typename LHS, typename RHS>
class MatMulExpr 
  : public MatrixBase<MatMulExpr<LHS, RHS>>
{

private:

    const LHS &mLhs;
    const RHS &mRhs;

public:

    static_assert(std::is_same_v<
        typename matrix_traits<LHS>::value_type,
        typename matrix_traits<RHS>::value_type>,
        "Mixed types not supported");

    using value_type = typename matrix_traits<LHS>::value_type;

public:

    /*!
     * \brief Constructs a multiplication expression from two sub‑expressions.
     * \param[in] lhs Left‑hand side expression.
     * \param[in] rhs Right‑hand side expression.
     */
    MatMulExpr(const LHS &lhs, const RHS &rhs)
      : mLhs(lhs), 
        mRhs(rhs)
    {
        TL_ASSERT(lhs.cols() == rhs.rows(), "Invalid dimensions for matrix multiplication");
    }

    /*! \brief Returns the number of rows (same as LHS). */
    constexpr auto rows() const noexcept -> size_t { return mLhs.rows(); }

    /*! \brief Returns the number of columns (same as RHS). */
    constexpr auto cols() const noexcept -> size_t { return mRhs.cols(); }

    /*! \brief Returns the left‑hand side expression. */
    auto lhs() const -> const LHS & { return mLhs; }

    /*! \brief Returns the right‑hand side expression. */
    auto rhs() const -> const RHS & { return mRhs; }

    /*!
     * \brief Checks if either operand's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if either LHS or RHS aliases `ptr`.
     */
    auto aliases(const void *ptr)  const -> bool
    {
        return mLhs.aliases(ptr) || mRhs.aliases(ptr);
    }

};

/*! \} */

} // End namespace tl