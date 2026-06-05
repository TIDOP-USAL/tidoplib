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

/*! \file MatVecMulExpr.h
 * \brief Expression template for matrix‑vector multiplication.
 *
 * This file defines the `MatVecMulExpr` class, which represents a lazy
 * multiplication of a matrix expression by a vector expression. The operation
 * is not performed immediately; instead, the expression object stores references
 * to its operands and provides metadata (size) and access to the sub‑expressions.
 * Evaluation occurs only when the expression is assigned to a concrete vector
 * or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::VectorBase, tl::MatMulExpr, tl::EvaluatorMatVecMulExpr
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
 * \class MatVecMulExpr
 * \brief Lazy expression for matrix‑vector multiplication.
 *
 * \tparam LHS Left‑hand side matrix expression type (must satisfy `MatrixExpr`).
 * \tparam RHS Right‑hand side vector expression type (must satisfy `VectorExpr`).
 *
 * This class represents the product of a matrix and a vector: `mat * vec`.
 * The dimensions must satisfy `mat.cols() == vec.size()`. The resulting
 * expression has `mat.rows()` elements.
 *
 * The actual multiplication is not performed at construction time; instead,
 * the expression stores references to the matrix and vector expressions.
 * When the expression is evaluated (e.g., assigned to a `Vector` object), the
 * product is computed using an optimised routine (see `EvaluatorMatVecMulExpr`).
 *
 * ### Example
 * \code
 * Matrix<double,3,4> A;
 * Vector<double,4> v;
 * auto expr = MatVecMulExpr(A, v);   // lazy: A * v
 * Vector<double,3> w = expr;          // evaluated here
 * \endcode
 */
template<typename LHS, typename RHS>
class MatVecMulExpr 
  : public VectorBase<MatVecMulExpr<LHS, RHS>>
{

private:

    const LHS &mMat;
    const RHS &mVec;

public:

    using value_type = typename matrix_traits<LHS>::value_type;

    /*!
     * \brief Constructs a matrix‑vector multiplication expression.
     * \param[in] mat The matrix expression (left operand).
     * \param[in] vec The vector expression (right operand).
     * \pre `mat.cols() == vec.size()`.
     */
    constexpr MatVecMulExpr(const LHS &mat, const RHS &vec)
      : mMat(mat), 
        mVec(vec)
    {
        TL_ASSERT(mat.cols() == vec.size(), "Matrix-Vector mismatch");
    }

    /*! \brief Returns the number of elements in the result vector ( = mat.rows() ). */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t { return mMat.rows(); }

    /*! \brief Returns the left‑hand side matrix expression. */
    [[nodiscard]]
    constexpr auto lhs() const -> const LHS & { return mMat; }

    /*! \brief Returns the right‑hand side vector expression. */
    [[nodiscard]]
    constexpr auto rhs() const -> const RHS & { return mVec; }

    /*!
     * \brief Checks if either operand's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if either the matrix or the vector expression aliases `ptr`.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool
    {
        return mMat.aliases(ptr) || mVec.aliases(ptr);
    }

};

/*! \} */

} // End namespace tl