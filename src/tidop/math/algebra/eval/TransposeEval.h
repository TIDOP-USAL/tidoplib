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
#include "tidop/math/algebra/expr/TransposeExpr.h"

namespace tl
{

/*! \addtogroup Algebra
 *  \{
 */

template<typename Expr>
class Evaluator<TransposeExpr<Expr>>
{

public:

    using value_type = typename matrix_traits<Expr>::value_type;

private:
	
    Evaluator<Expr> mExpr;
    size_t mCols;

public:

    Evaluator(const TransposeExpr<Expr>& expr)
      : mExpr(expr.expr()),
        mCols(expr.cols())
    {}

    auto coeff(size_t r, size_t c) const -> value_type
    {
        return mExpr.coeff(c, r);
    }

    auto coeff(size_t i) const -> value_type
    {
        size_t c = i % mCols;
        size_t r = i / mCols;

        return mExpr.coeff(c, r);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    auto packet(size_t i) const
    {
        TL_ASSERT(false, "SIMD packet not supported for this expression");
        return Packed<value_type>();
    }
#endif

};

/*! \} */

} // End namespace tl