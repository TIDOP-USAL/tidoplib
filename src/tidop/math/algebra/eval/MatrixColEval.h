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

/*! \file MatrixColEval.h
 * \brief Evaluator specialization for MatrixCol (column view).
 *
 * This file provides the `Evaluator` specialization for `MatrixCol<Scalar>`,
 * which is a non‑owning view of a single column of a matrix. The evaluator
 * forwards coefficient access to the underlying column view; it does not
 * create temporary storage. SIMD packet access is currently not supported.
 *
 * \ingroup Evaluators
 * \see tl::MatrixCol, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `MatrixCol<Scalar>` (column view of a matrix).
 *
 * \tparam Scalar Element type (may be const or non‑const).
 *
 * This evaluator provides read‑only access to the coefficients of a matrix column.
 * Because a column view already provides efficient indexing, the evaluator simply
 * stores a reference to the original `MatrixCol` object and forwards `coeff()` calls.
 * SIMD packet access is not supported and will trigger an assertion if enabled.
 */
template<typename Scalar>
class Evaluator<MatrixCol<Scalar>>
{

private:

    const MatrixCol<Scalar> &mMatrixCol;
	
public:

    using value_type = std::remove_const_t<Scalar>;
	
public:

    /*!
     * \brief Constructs the evaluator from a `MatrixCol`.
     * \param[in] col The column view.
     */
    constexpr Evaluator(const MatrixCol<Scalar> &col)
      : mMatrixCol(col) {}

    /*!
     * \brief Returns the element at linear index i (row index within the column).
     * \param[in] i Row index (0‑based).
     * \return The coefficient at the given row.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t i) const -> value_type
    {
        return mMatrixCol[i];
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients (not supported for column views).
     * \param[in] i Linear index.
     * \return A dummy `Packed<value_type>`.
     *
     * \note SIMD packet access is currently not implemented for `MatrixCol`.
     *       If called, this method triggers an assertion.
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