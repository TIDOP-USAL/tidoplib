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

#include "tidop/math/algebra/eval/Evaluator.h"
#include "tidop/math/algebra/expr/VecDivScalarExpr.h"

namespace tl
{

/*! \addtogroup Algebra
 *  \{
 */

template<typename LHS, typename Scalar>
class Evaluator<VecDivScalarExpr<LHS, Scalar>>
{

private:

    Evaluator<LHS> mLhs;
    Scalar mScalar;

public:

    using value_type = typename VecDivScalarExpr<LHS, Scalar>::value_type;

public:

    Evaluator(const VecDivScalarExpr<LHS, Scalar> &expr)
      : mLhs(expr.lhs()),
        mScalar(expr.scalar())
    {
    }

    auto coeff(size_t i) const -> value_type
    {
        return mLhs.coeff(i) / mScalar;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    auto packet(size_t i) const
    {
        return mLhs.packet(i) / Packed<value_type>(mScalar);
    }
#endif

};


/*! \} */

} // End namespace tl