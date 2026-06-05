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

/*! \file MatrixBlockEval.h
 * \brief Evaluator specialization for MatrixBlock (submatrix view).
 *
 * This file provides the `Evaluator` specialization for `MatrixBlock<T>`, which
 * is a non‑owning view of a contiguous block within a parent matrix. The evaluator
 * simply forwards coefficient access to the underlying block; no temporary storage
 * or computation is performed.
 *
 * \ingroup Evaluators
 * \see tl::MatrixBlock, tl::Evaluator
 */

#pragma once

#include "tidop/math/algebra/eval/Evaluator.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

/*!
 * \brief Evaluator for `MatrixBlock<T>` (block view of a matrix).
 *
 * \tparam T Element type (may be const or non‑const).
 *
 * This evaluator provides read‑only access to the coefficients of a matrix block.
 * Because a block is already a lightweight view, the evaluator simply stores
 * a reference to the original block and forwards `coeff()` calls. SIMD packet
 * access is not supported and triggers an assertion.
 */
template<typename T>
class Evaluator<MatrixBlock<T>>
{

private:

    const MatrixBlock<T> &mBlock;

public:

    using value_type = std::remove_cv_t<T>;

    /*!
     * \brief Constructs the evaluator from a `MatrixBlock`.
     * \param[in] block The block view.
     */
    constexpr Evaluator(const MatrixBlock<T> &block)
        : mBlock(block)
    {}

    /*!
     * \brief Returns the element at matrix position (r, c).
     * \param[in] r Row index.
     * \param[in] c Column index.
     * \return The coefficient at the given position.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t r, size_t c) const -> value_type
    {
        return mBlock(r,c);
    }

    /*!
     * \brief Returns the element at linear index i (row‑major order).
     * \param[in] i Linear index.
     * \return The coefficient at the given linear position.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t i) const -> value_type
    {
        return mBlock(i);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients (not supported for blocks).
     * \param[in] i Linear index.
     * \return A dummy `Packed<value_type>`.
     *
     * \note SIMD packet access is currently not implemented for `MatrixBlock`.
     *       Calling this method will trigger an assertion.
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