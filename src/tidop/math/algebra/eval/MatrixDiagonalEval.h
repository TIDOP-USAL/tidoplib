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

/*! \file MatrixDiagonalEval.h
 * \brief Evaluator specialization for MatrixDiagonal (diagonal view).
 *
 * This file provides the `Evaluator` specialization for `MatrixDiagonal<Scalar>`,
 * which is a non‑owning view of the diagonal of a matrix. The evaluator forwards
 * coefficient access to the underlying diagonal view. SIMD packet access is
 * currently not supported because the diagonal elements are not necessarily
 * stored contiguously (although in a row‑major layout they are spaced by
 * `cols+1` entries, which may be larger than 1). Use of SIMD would require
 * a special stride‑handling mechanism, which is not implemented.
 *
 * \ingroup Evaluators
 * \see tl::MatrixDiagonal, tl::Evaluator
 */

#pragma once

#include "tidop/math/base/Traits.h"
#include "tidop/math/algebra/eval/Evaluator.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `MatrixDiagonal<Scalar>` (diagonal view of a matrix).
 *
 * \tparam Scalar Element type (may be const or non‑const).
 *
 * This evaluator provides read‑only access to the coefficients of a matrix
 * diagonal. It stores a reference to the `MatrixDiagonal` object and forwards
 * `coeff()` calls to it. SIMD packet access is not supported because diagonal
 * elements are not stored contiguously, and implementing efficient vectorised
 * access would require special handling of stride.
 */
template<typename Scalar>
class Evaluator<MatrixDiagonal<Scalar>>
{

private:

    const MatrixDiagonal<Scalar> &mMatrixDiagonal;

public:

    using value_type = typename vector_traits<MatrixDiagonal<Scalar>>::value_type;
	
public:

    /*!
     * \brief Constructs the evaluator from a `MatrixDiagonal`.
     * \param[in] diagonal The diagonal view.
     */
    constexpr Evaluator(const MatrixDiagonal<Scalar> &diagonal)
      : mMatrixDiagonal(diagonal) {}

    /*!
     * \brief Returns the element at index i (the i‑th diagonal element).
     * \param[in] i Index along the diagonal.
     * \return The diagonal element.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t i) const -> value_type
    {
        return mMatrixDiagonal[i];
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief SIMD packet access (not supported for diagonal views).
     * \param[in] i Linear index.
     * \return A dummy `Packed<value_type>`.
     *
     * \note This method always triggers an assertion because diagonal elements
     *       are not stored contiguously, and stride‑aware packet access is
     *       not implemented. Use scalar coefficient access instead.
     */
    [[nodiscard]]
    constexpr auto packet(size_t i) const
    {
        TL_ASSERT(false, "SIMD packet not supported for this expression");
        return Packed<value_type>();
    }
#endif
};


/*! \} */

} // End namespace tl