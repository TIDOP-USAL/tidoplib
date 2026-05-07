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

/*! \file VectorEval.h
 * \brief Evaluator for dense vector types (e.g., `Vector<T, Size>`).
 *
 * \tparam Vec A type satisfying the `DenseVector` concept.
 *
 * This evaluator provides read‑only access to the coefficients of a concrete
 * dense vector. It stores a reference to the vector and forwards `coeff()` and
 * `packet()` calls directly to the underlying vector. It is used internally
 * whenever a concrete vector appears in an expression tree.
 * \ingroup Evaluators
 */

#pragma once

#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/eval/Evaluator.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

template<DenseVector Vec>
class Evaluator<Vec>
{

private:

    const Vec &mVector; /*!< Reference to the vector being evaluated. */

public:

    using value_type = typename vector_traits<Vec>::value_type;
	
public:

    /*!
     * \brief Constructs the evaluator from a dense vector.
     * \param[in] vector The vector.
     */
    Evaluator(const Vec &vector)
      : mVector(vector) {}

    /*!
     * \brief Returns the element at linear index i.
     * \param[in] i Element index.
     * \return The coefficient at the given position.
     */
    auto coeff(size_t i) const -> value_type
    {
        return mVector(i);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients starting at index i.
     * \param[in] i Linear index.
     * \return A `Packed<T>` containing the coefficients from the vector.
     * \note Only available when SIMD intrinsics are enabled.
     */
    auto packet(size_t i) const
    {
        return mVector.packet(i);
    }
#endif
};


/*! \} */

} // End namespace tl