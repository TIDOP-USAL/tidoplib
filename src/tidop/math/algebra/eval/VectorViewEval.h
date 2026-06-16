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


#pragma once

#include "tidop/math/base/Traits.h"
#include "tidop/math/algebra/eval/Evaluator.h"

namespace tl
{

/*! \addtogroup Evaluators
 *  \{
 */

template<typename Scalar>
class Evaluator<VectorView<Scalar>>
{

private:

    const VectorView<Scalar> &mVectorView;

public:

    using value_type = typename vector_traits<VectorView<Scalar>>::value_type;
	
public:

    /*!
     * \brief Constructs the evaluator from a `VectorView`.
     * \param[in] row The row view.
     */
    constexpr Evaluator(const VectorView<Scalar> &vectorView)
      : mVectorView(vectorView) {}

    /*!
     * \brief Returns the element at linear index i (column index within the row).
     * \param[in] i Column index (0-based).
     * \return The coefficient at the given column.
     */
    [[nodiscard]]
    constexpr auto coeff(size_t i) const -> value_type
    {
        return mVectorView[i];
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of coefficients starting at column index i.
     * \param[in] i Column index.
     * \return A `Packed<T>` containing the coefficients from the row.
     * \note Only available when SIMD intrinsics are enabled.
     */
    [[nodiscard]]
    auto packet(size_t i) const -> Packed<value_type>
    {
        return mVectorView.packet(i);
    }
#endif
};


/*! \} */

} // End namespace tl