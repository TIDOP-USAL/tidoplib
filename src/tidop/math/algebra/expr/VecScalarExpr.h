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

/*! \file VecScalarExpr.h
 * \brief Expression template for element‑wise vector‑scalar operations.
 *
 * This file defines the `VecScalarExpr` class, which represents a lazy
 * element‑wise binary operation between a vector expression and a scalar value
 * (e.g., multiplication or division). The operation is not evaluated
 * immediately; instead, the expression object stores a reference to the vector
 * expression and a copy of the scalar, and provides metadata (size) and access
 * to the operands. Evaluation occurs only when the expression is assigned to a
 * concrete vector or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::VectorBase, tl::VecBinaryExpr, tl::EvaluatorVecScalarExpr
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
 * \class VecScalarExpr
 * \brief Lazy expression for element‑wise vector‑scalar operations.
 *
 * \tparam LHS    Left‑hand side vector expression type (must satisfy `VectorExpr`).
 * \tparam Scalar Scalar type (e.g., `double`, `int`).
 * \tparam Op     Binary operation functor (e.g., `MulOp`, `DivOp`). Not stored,
 *                only used as a template parameter to differentiate instances.
 *
 * This class represents an element‑wise operation between a vector expression
 * and a scalar: `result(i) = Op(LHS(i), scalar)`. The size of the result is
 * the same as the LHS vector expression. The operation is not performed at
 * construction time; it is stored as an expression template for later
 * evaluation.
 *
 * The scalar type must match the vector expression's `value_type` (enforced
 * by a `static_assert`). The expression is read‑only; it is typically used
 * on the right‑hand side of an assignment to a concrete vector.
 *
 * ### Example
 * \code
 * Vector<double,3> v = {1, 2, 3};
 * auto scaled = VecScalarExpr(v, 2.0, MulOp{});   // lazy: v * 2
 * Vector<double,3> w = scaled;                    // evaluated here
 *
 * // Also works with expressions:
 * auto expr = VecScalarExpr(v + w, 0.5, MulOp{}); // (v+w) * 0.5
 * \endcode
 */
template<typename LHS, typename Scalar, typename Op>
class VecScalarExpr
  : public VectorBase<VecScalarExpr<LHS, Scalar, Op>>
{

private:

    const LHS &mLhs;
    Scalar mScalar;

public:

    static_assert(std::is_same_v<typename vector_traits<LHS>::value_type,
                  Scalar>, "Mixed types not supported");

    using value_type = typename vector_traits<LHS>::value_type;

public:

    /*!
     * \brief Constructs a scalar expression from a vector expression and a scalar.
     * \param[in] lhs    The vector expression.
     * \param[in] scalar The scalar value.
     */
    constexpr VecScalarExpr(const LHS &lhs, Scalar scalar)
      : mLhs(lhs), 
        mScalar(scalar)
    {}

    /*! \brief Returns the number of elements (same as LHS). */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t { return mLhs.size(); }

    /*! \brief Returns the left‑hand side vector expression. */
    [[nodiscard]]
    constexpr auto lhs() const -> const LHS & { return mLhs; }

    /*! \brief Returns the scalar value. */
    [[nodiscard]]
    constexpr auto scalar() const -> Scalar { return mScalar; }

    /*!
     * \brief Checks if the left‑hand side expression's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the LHS expression aliases `ptr`.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool
    {
        return mLhs.aliases(ptr);
    }
};

/*! \} */

} // End namespace tl