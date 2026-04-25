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

/*! \addtogroup Algebra
 *  \{
 */

template<typename Scalar, size_t Size>
class Evaluator<MatrixRow<Scalar, Size>>
{

private:

    const MatrixRow<Scalar, Size> &mMatrixRow;

public:

    using value_type = typename vector_traits<MatrixRow<Scalar, Size>>::value_type;
	
public:

    Evaluator(const MatrixRow<Scalar, Size> &row) 
      : mMatrixRow(row) {}

    auto coeff(size_t i) const -> value_type
    {
        return mMatrixRow[i];
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    auto packet(size_t i) const
    {
        return mMatrixRow.packet(i);
    }
#endif
};


/*! \} */

} // End namespace tl