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

/*! \file MatrixEval.h
 * \brief Evaluator specialization for dense matrices (the actual matrix class).
 *
 * This file provides the `Evaluator` specialization for any type satisfying the
 * `DenseMatrix` concept (i.e., `tl::Matrix` and similar). The evaluator simply
 * stores a reference to the matrix and forwards coefficient and packet access
 * directly to it. No temporary storage or computation is performed.
 *
 * \ingroup Evaluators
 * \see tl::Matrix, tl::Evaluator
 */

#pragma once

#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/eval/Evaluator.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for dense matrix types (e.g., `Matrix<T, Rows, Cols>`).
 *
 * \tparam Mat A type satisfying the `DenseMatrix` concept.
 *
 * This evaluator provides read‑only access to the coefficients of a concrete
 * dense matrix. It stores a reference to the matrix and forwards `coeff()` and
 * `packet()` calls directly to the underlying matrix. It is used internally
 * whenever a concrete matrix appears in an expression tree.
 */
template<DenseMatrix Mat>
class Evaluator<Mat>
{

private:

    const Mat &mMatrix;

public:

    using value_type = typename matrix_traits<Mat>::value_type;
	
public:

    /*!
     * \brief Constructs the evaluator from a dense matrix.
     * \param[in] mat The matrix.
     */
    Evaluator(const Mat& mat) 
      : mMatrix(mat) {}

    /*!
     * \brief Returns the element at matrix position (r, c).
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return The coefficient at the given position.
     */
    auto coeff(size_t r, size_t c) const -> value_type
    {
        return mMatrix(r, c);
    }

    /*!
     * \brief Returns the element at linear index i (row‑major order).
     * \param[in] i Linear index.
     * \return The coefficient at the given linear position.
     */
    auto coeff(size_t i) const -> value_type
    {
        return mMatrix(i);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients starting at linear index i.
     * \param[in] i Linear index.
     * \return A `Packed<T>` containing the coefficients from the matrix.
     * \note Only available when SIMD intrinsics are enabled.
     */
    auto packet(size_t i) const
    {
        return mMatrix.packet(i);
    }
#endif
};


/*! \} */

} // End namespace tl