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

/*! \file MatBinaryExpr.h
 * \brief Expression template for element-wise binary operations on matrices.
 *
 * This file defines the `MatBinaryExpr` class, which represents a lazy
 * element‑wise binary operation (e.g., addition, subtraction) between two
 * matrix expressions. The operation is not evaluated immediately; instead,
 * the expression object stores references to its operands and provides
 * metadata (rows, columns) and access to the sub‑expressions. Evaluation
 * occurs only when the expression is assigned to a concrete matrix or forced
 * via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::MatrixBase, tl::MatBinaryExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/base/Traits.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/functors/Add.h"
#include "tidop/math/algebra/functors/Sub.h"

namespace tl
{

template<typename Derived>
class MatrixBase;

/*! \addtogroup Expressions
 *  \{
 */

/*!
 * \class MatBinaryExpr
 * \brief Lazy expression for element‑wise binary operations on matrices.
 *
 * \tparam LHS Left‑hand side matrix expression type.
 * \tparam RHS Right‑hand side matrix expression type.
 * \tparam Op  Binary operation functor (e.g., `AddOp`, `SubOp`).
 *
 * This class is part of the expression template system. It stores const
 * references to its left‑hand side (LHS) and right‑hand side (RHS) expressions.
 * The actual element‑wise operation is not performed until the expression is
 * evaluated. The expression inherits from `MatrixBase` and thus can be used
 * in further expressions.
 *
 * The operation is applied element‑wise, meaning that `result(i,j) = Op(lhs(i,j), rhs(i,j))`.
 * Both matrices must have the same dimensions; this is checked at construction.
 *
 * ### Example
 * \code
 * Matrix<double,3,3> A, B;
 * auto sum_expr = MatBinaryExpr(A, B, AddOp{});  // lazy addition
 * Matrix<double,3,3> C = sum_expr;               // evaluated here
 * \endcode
 */
template<typename LHS, typename RHS, typename Op>
class MatBinaryExpr
  : public MatrixBase<MatBinaryExpr<LHS, RHS, Op>>
{

private:

    const LHS &mLhs; /*!< Reference to the left‑hand side expression. */
    const RHS &mRhs; /*!< Reference to the right‑hand side expression. */

public:

    static_assert(std::is_same_v<
        typename matrix_traits<LHS>::value_type,
        typename matrix_traits<RHS>::value_type>,
        "Mixed types not supported");

    using value_type = typename matrix_traits<LHS>::value_type;

public:

    /*!
     * \brief Constructs a binary expression from two sub‑expressions.
     * \param[in] lhs Left‑hand side expression.
     * \param[in] rhs Right‑hand side expression.
     * \pre `lhs.rows() == rhs.rows()` and `lhs.cols() == rhs.cols()`.
     */
    constexpr MatBinaryExpr(const LHS &lhs, const RHS &rhs)
      : mLhs(lhs), mRhs(rhs)
    {
        TL_ASSERT(lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols(), "Matrix sizes must match");
    }

    /*! 
     * \brief Returns the number of rows 
     */
    [[nodiscard]]
    constexpr auto rows() const noexcept -> size_t { return mLhs.rows(); }

    /*!
     * \brief Returns the number of columns 
     */
    [[nodiscard]]
    constexpr auto cols() const noexcept -> size_t { return mLhs.cols(); }

    /*! \brief Returns the left‑hand side expression. */
    [[nodiscard]]
    constexpr auto lhs() const -> const LHS & { return mLhs; }

    /*! \brief Returns the right‑hand side expression. */
    [[nodiscard]]
    constexpr auto rhs() const -> const RHS & { return mRhs; }

    /*!
     * \brief Checks if either operand's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if either LHS or RHS aliases `ptr`.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool
    {
        return mLhs.aliases(ptr) || mRhs.aliases(ptr);
    }

};

/*! \} */

} // End namespace tl