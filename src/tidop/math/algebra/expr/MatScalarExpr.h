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

/*! \file MatScalarExpr.h
 * \brief Expression template for element‑wise matrix‑scalar operations.
 *
 * This file defines the `MatScalarExpr` class, which represents a lazy
 * element‑wise binary operation between a matrix expression and a scalar value
 * (e.g., multiplication or division). The operation is not evaluated
 * immediately; instead, the expression object stores a reference to the matrix
 * expression and a copy of the scalar, and provides metadata (rows, columns)
 * and access to the operands. Evaluation occurs only when the expression is
 * assigned to a concrete matrix or forced via `.eval()`.
 *
 * \ingroup Expressions
 * \see tl::MatrixBase, tl::MatBinaryExpr, tl::EvaluatorMatScalarExpr
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
 * \class MatScalarExpr
 * \brief Lazy expression for element‑wise matrix‑scalar operations.
 *
 * \tparam LHS    Left‑hand side matrix expression type (must satisfy `MatrixExpr`).
 * \tparam Scalar Scalar type (e.g., `double`, `int`).
 * \tparam Op     Binary operation functor (e.g., `MulOp`, `DivOp`). Not stored,
 *                only used as a template parameter to differentiate instances.
 *
 * This class represents an element‑wise operation between a matrix expression
 * and a scalar: `result(i,j) = Op(LHS(i,j), scalar)`. The dimensions of the
 * result are the same as the LHS matrix expression. The operation is not
 * evaluated at construction time; it is stored as an expression template
 * for later evaluation.
 */
template<typename LHS, typename Scalar, typename Op>
class MatScalarExpr
  : public MatrixBase<MatScalarExpr<LHS, Scalar, Op>>
{

private:

    const LHS &mLhs;    /*!< Reference to the left‑hand side matrix expression. */
    Scalar mScalar;     /*!< Stored scalar value. */

public:

    static_assert(std::is_same_v<
        typename matrix_traits<LHS>::value_type,
        Scalar>,
        "Mixed types not supported");

    using value_type = typename matrix_traits<LHS>::value_type;

public:

    /*!
     * \brief Constructs a scalar expression from a matrix expression and a scalar.
     * \param[in] lhs    The matrix expression.
     * \param[in] scalar The scalar value.
     */
    MatScalarExpr(const LHS &lhs, Scalar scalar)
      : mLhs(lhs), 
        mScalar(scalar)
    {}

    /*! \brief Returns the number of rows (same as LHS). */
    constexpr auto rows() const noexcept -> size_t { return mLhs.rows(); }

    /*! \brief Returns the number of columns (same as LHS). */
    constexpr auto cols() const noexcept -> size_t { return mLhs.cols(); }

    /*! \brief Returns the left‑hand side matrix expression. */
    auto lhs() const -> const LHS & { return mLhs; }

    /*! \brief Returns the scalar value. */
    auto scalar() const -> Scalar { return mScalar; }

    /*!
     * \brief Checks if the left‑hand side expression's data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the LHS expression aliases `ptr`.
     */
    auto aliases(const void *ptr) const -> bool
    {
        return mLhs.aliases(ptr);
    }
};

/*! \} */

} // End namespace tl