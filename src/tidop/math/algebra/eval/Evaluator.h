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

/*! \file Evaluator.h
 * \brief Expression evaluator for lazy evaluation in expression templates.
 *
 * This file defines the `Evaluator` class template and the helper function
 * `make_evaluator`. The evaluator is a key component of the expression template
 * system: each expression type (e.g., `VecBinaryExpr`, `TransposeExpr`) provides
 * a specialization of `Evaluator` that gives access to its coefficients, size,
 * and, when possible, SIMD packets. The `make_evaluator` function creates the
 * appropriate evaluator for a given expression.
 *
 * Users normally do not need to use these directly; they are used internally
 * by the expression evaluation machinery (e.g., in `VectorBase::dotProduct`).
 *
 * \ingroup Evaluators
 */

#pragma once

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \class Evaluator
 * \brief Evaluator for expression types (specialized per expression).
 *
 * \tparam Expr The expression type (e.g., `VecBinaryExpr`, `Vector`, etc.).
 *
 * The primary template is intentionally left undefined; a compile‑time error
 * occurs if a specialization is missing. Each expression type that participates
 * in the expression template system must provide its own specialization of
 * `Evaluator`. The evaluator typically provides the following members:
 * - `coeff(i)`: returns the i‑th coefficient.
 * - `coeff(i, j)`: for matrix expressions, returns the (i,j) coefficient.
 * - `size()`: returns the number of elements (for vectors) or rows*cols.
 * - `rows()` and `cols()` for matrices.
 * - `packet(i)`: returns a SIMD packet of coefficients starting at index i.
 *
 * \note This class is part of the internal implementation and not intended
 *       for direct use in user code.
 *
 * \see make_evaluator
 */
template<typename Expr>
class Evaluator
{
    static_assert(sizeof(Expr) == 0, "Evaluator not specialized for this expression type");
};

/*!
 * \brief Constructs an evaluator for a given expression.
 *
 * \tparam Expr The expression type (deduced).
 * \param[in] expr The expression object.
 * \return An evaluator instance of type `Evaluator<Expr>`.
 *
 * This is a convenience function that uses `std::remove_cvref_t` to obtain
 * the plain expression type before constructing the evaluator. It is typically
 * used inside expression template operations where the expression may be
 * const‑qualified or a reference.
 *
 * \code
 * auto eval = make_evaluator(some_expression);
 * for (size_t i = 0; i < eval.size(); ++i) {
 *     do_something(eval.coeff(i));
 * }
 * \endcode
 */
template<typename Expr>
[[nodiscard]]
constexpr auto make_evaluator(const Expr &expr)
{
    return Evaluator<std::remove_cvref_t<Expr>>(expr);
}

/*! \} */

} // End namespace tl