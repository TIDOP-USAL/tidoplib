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

/*! \file MatVecMulEval.h
 * \brief Evaluator specialization for matrix‑vector multiplication expressions.
 *
 * This file provides the `Evaluator` specialization for `MatVecMulExpr`, which
 * represents the multiplication of a matrix expression by a vector expression.
 * Because matrix‑vector multiplication is a reduction operation (each output
 * component is a dot product), it cannot be evaluated lazily coefficient by
 * coefficient. This evaluator pre‑computes the entire product into a temporary
 * vector and then provides coefficient access from that temporary.
 *
 * \ingroup Evaluators
 * \see tl::MatVecMulExpr, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/MatVecMulExpr.h"
#include "tidop/math/algebra/vector/detail/MatVecMul.h"

namespace tl
{

template<typename T, size_t Size>
class Vector;

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `MatVecMulExpr<LHS, RHS>` (matrix‑vector product).
 *
 * \tparam LHS Left‑hand side matrix expression type.
 * \tparam RHS Right‑hand side vector expression type.
 *
 * This evaluator computes the product of the LHS matrix and the RHS vector at
 * construction time (eager evaluation) and stores the result in a dynamic
 * vector (`mTemp`). Subsequent calls to `coeff()` simply read from this
 * pre‑computed vector.
 *
 * \note Because the product is evaluated immediately, using `MatVecMulExpr` in
 *       an expression chain will perform the multiplication as soon as the
 *       evaluator is created. This is acceptable because matrix‑vector
 *       multiplication is non‑trivial and caching the result avoids
 *       recomputation.
 */
template<typename LHS, typename RHS>
class Evaluator<MatVecMulExpr<LHS, RHS>>
{

public:

    using value_type = typename MatVecMulExpr<LHS, RHS>::value_type;

private:

    /*! \brief Temporary vector storing the product. */
    Vector<value_type, DynamicData> mTemp;

public:

    /*!
     * \brief Constructs the evaluator and immediately computes the product.
     * \param[in] expr The `MatVecMulExpr` to evaluate.
     *
     * The temporary vector is allocated to the size of the result (`expr.size()`).
     * The actual multiplication is performed by calling `detail::mat_vec_mul`.
     */
    Evaluator(const MatVecMulExpr<LHS, RHS> &expr)
      : mTemp(expr.size())
    {
        detail::mat_vec_mul(expr.lhs(), expr.rhs(), mTemp);
    }

    /*!
     * \brief Returns the coefficient at index i from the pre‑computed product.
     * \param[in] i Element index (0‑based).
     * \return The i‑th component of the product vector.
     */
    auto coeff(size_t i) const -> value_type
    {
        return mTemp[i];
    }

};


/*! \} */

} // End namespace tl