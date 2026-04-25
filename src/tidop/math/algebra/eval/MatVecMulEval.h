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
#include "tidop/math/algebra/expr/MatVecMulExpr.h"
#include "tidop/math/algebra/vector/detail/MatVecMul.h"

namespace tl
{

template<typename T, size_t Size>
class Vector;

/*! \addtogroup Algebra
 *  \{
 */

template<typename LHS, typename RHS>
class Evaluator<MatVecMulExpr<LHS, RHS>>
{

public:

    using value_type = typename MatVecMulExpr<LHS, RHS>::value_type;

private:

    Vector<value_type, DynamicData> mTemp;

public:

    Evaluator(const MatVecMulExpr<LHS, RHS> &expr)
      : mTemp(expr.size())
    {
        detail::mat_vec_mul(expr.lhs(), expr.rhs(), mTemp);
    }

    auto coeff(size_t i) const -> value_type
    {
        return mTemp[i];
    }

};


/*! \} */

} // End namespace tl