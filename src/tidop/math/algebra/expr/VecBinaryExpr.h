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

/*! \file VecBinaryExpr.h
 * \brief Expression template for element‑wise binary operations on vectors.
 *
 * This file defines the `VecBinaryExpr` class, which represents a lazy
 * element‑wise binary operation (e.g., addition, subtraction) between two
 * vector expressions. The operation is not evaluated immediately; instead,
 * the expression object stores references to its operands and provides
 * metadata (size) and access to the sub‑expressions. Evaluation occurs only
 * when the expression is assigned to a concrete vector or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::VectorBase, tl::VecScalarExpr, tl::EvaluatorVecBinaryExpr
 */

#pragma once

#include "tidop/math/base/Traits.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/functors/Add.h"
#include "tidop/math/algebra/functors/Sub.h"
#include "tidop/math/algebra/functors/Mul.h"
#include "tidop/math/algebra/functors/Div.h"

namespace tl
{

template<typename Derived>
class VectorBase;

/*! \addtogroup Expressions
 *  \{
 */

/*!
 * \class VecBinaryExpr
 * \brief Lazy expression for element‑wise binary operations on vectors.
 *
 * \tparam LHS Left‑hand side vector expression type (must satisfy `VectorExpr`).
 * \tparam RHS Right‑hand side vector expression type (must satisfy `VectorExpr`).
 * \tparam Op  Binary operation functor (e.g., `AddOp`, `SubOp`, `MulOp`, `DivOp`).
 *
 * This class represents an element‑wise binary operation between two vector
 * expressions: `result(i) = Op(lhs(i), rhs(i))`. Both vectors must have the
 * same size (checked at construction). The operation is not performed at
 * construction time; the expression stores references to the operands and
 * the operation functor (which is stateless and only used as a type tag).
 *
 * Evaluation is triggered when the expression is assigned to a concrete
 * vector (e.g., `Vector<T,Size>`) or when `.eval()` is called.
 *
 * ### Example
 * \code
 * Vector<double,3> a = {1,2,3};
 * Vector<double,3> b = {4,5,6};
 * auto sum_expr = VecBinaryExpr(a, b, AddOp{});   // lazy addition
 * Vector<double,3> c = sum_expr;                  // evaluated here -> Vector<double,3> c = a + b
 *
 * // Also works with expressions:
 * Vector<double,3> d = VecBinaryExpr(a * 2, b, AddOp{});      // Vector<double,3> d = a*2 + b
 * \endcode
 */
template<typename LHS, typename RHS, typename Op>
class VecBinaryExpr
  : public VectorBase<VecBinaryExpr<LHS, RHS, Op>>
{

private:

    const LHS &mLhs;
    const RHS &mRhs;

public:

    static_assert(std::is_same_v<
        typename vector_traits<LHS>::value_type,
        typename vector_traits<RHS>::value_type>,
        "Mixed types not supported");

    using value_type = typename vector_traits<LHS>::value_type;

public:

    /*!
     * \brief Constructs a binary expression from two vector sub‑expressions.
     * \param[in] lhs Left‑hand side expression.
     * \param[in] rhs Right‑hand side expression.
     */
    constexpr VecBinaryExpr(const LHS &lhs, const RHS &rhs)
      : mLhs(lhs), mRhs(rhs)
    {
        TL_ASSERT(lhs.size() == rhs.size(), "Vector sizes must match");
    }

    /*! \brief Returns the number of elements (same as both operands). */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t { return mLhs.size(); }

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